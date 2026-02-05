/**
 * @file piano.c
 * @brief 钢琴组件实现文件
 * @details 包含钢琴样式的定义、按键事件处理逻辑、音符名称生成算法以及 UI 布局生成。
 * @author LuoShu
 * @version 1.0
 * @date 2026-02-04
 */

#include "piano.h"

/**
 * @brief 琴键样式定义
 */
static lv_style_t style_white, style_white_pr;      ///< 白键默认样式与按下样式
static lv_style_t style_black, style_black_pr;      ///< 黑键默认样式与按下样式

/**
 * @brief 动态音符名称缓存池
 * @details 预先分配内存空间用于存储生成的音符字符串（如 "Cs4"），避免 UI 运行时频繁申请内存。
 */
static char g_note_names[PIANO_OCTAVE_COUNT * 12][8]; 
static int g_note_index = 0;                        ///< 当前缓存池的索引偏移

/**
 * @brief 白键对应的基础音名数组
 */
static const char WHITE_NOTE_CHAR[] = {'C', 'D', 'E', 'F', 'G', 'A', 'B'};

/**
 * @brief 初始化钢琴所需的 LVGL 样式
 * @details 设置白键和黑键的背景色、圆角、边框、阴影以及按下时的位移过渡效果，增强立体感。
 */
static void piano_styles_init(void)
{
    /* --- 白键样式（默认）--- */
    lv_style_init(&style_white);
    lv_style_set_radius(&style_white, 6);   
    lv_style_set_bg_color(&style_white, lv_color_white());
    lv_style_set_border_width(&style_white, 1);
    lv_style_set_border_color(&style_white, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_border_side(&style_white, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_BOTTOM);
    lv_style_set_shadow_width(&style_white, 10);    
    lv_style_set_shadow_ofs_y(&style_white, 5);
    lv_style_set_shadow_opa(&style_white, LV_OPA_30);

    /* --- 白键样式（按下）--- */
    lv_style_init(&style_white_pr);
    lv_style_set_bg_color(&style_white_pr, lv_palette_lighten(LV_PALETTE_GREY, 4)); 
    lv_style_set_shadow_ofs_y(&style_white_pr, 2); 
    lv_style_set_translate_y(&style_white_pr, 3);  

    /* --- 黑键样式（默认）--- */
    lv_style_init(&style_black);
    lv_style_set_radius(&style_black, 4);                           
    lv_style_set_bg_color(&style_black, lv_color_make(20, 20, 20)); 
    lv_style_set_bg_grad_color(&style_black, lv_color_make(60, 60, 60));    
    lv_style_set_bg_grad_dir(&style_black, LV_GRAD_DIR_VER);
    lv_style_set_bg_main_stop(&style_black, 200);
    lv_style_set_bg_grad_stop(&style_black, 255);
    lv_style_set_shadow_width(&style_black, 30);     
    lv_style_set_shadow_ofs_y(&style_black, 4);
    lv_style_set_shadow_opa(&style_black, LV_OPA_COVER);

    /* --- 黑键样式（按下）--- */
    lv_style_init(&style_black_pr);
    lv_style_set_bg_color(&style_black_pr, lv_color_black());
    lv_style_set_shadow_ofs_y(&style_black_pr, 1);
    lv_style_set_translate_y(&style_black_pr, 3);
}

/**
 * @brief 执行音符播放
 * @param[in] key_obj 触发播放的按键对象句柄
 * @details 从按键对象的 user_data 中提取音符名称并调用底层播放命令。
 */
static void piano_play_note(lv_obj_t* key_obj) 
{
    if (!key_obj)
    {
#if PIANO_LOG_ENABLE
        LV_LOG_ERROR("[Piano] Invalid key object!");
#endif
        return;
    }

    const char * note_name = (const char *)lv_obj_get_user_data(key_obj);
    if (!note_name)
    {
#if PIANO_LOG_ENABLE
        LV_LOG_ERROR("[Piano] Invalid note name!");
#endif
        return;
    }

    piano_play_note_cmd(note_name);

#if PIANO_LOG_ENABLE
    LV_LOG_USER("\t[Piano] Playing:\t%s.wav", note_name);
#endif
}

/**
 * @brief 琴键按下事件回调函数
 * @param[in] e LVGL 事件指针
 * @details 处理按键点击事件，触发声音播放。
 */
static void piano_press_handler(lv_event_t* e)
{
    lv_obj_t * key = lv_event_get_target(e);
    
    const char * note_name = (const char *)lv_obj_get_user_data(key);
#if PIANO_LOG_ENABLE
    LV_LOG_USER("\t[Piano] Pressed:\t%s", note_name);
#endif
    piano_play_note(key);
}

/**
 * @brief 构建并获取音符名称字符串
 * @param[in] note_char 音符基础字符（如 'C', 'D'）
 * @param[in] octave 八度序号
 * @param[in] is_sharp 是否为升音符号 (#)
 * @return 返回指向缓存池中音符字符串的指针
 * @note 若缓存池溢出，将返回 "ERR"。
 */
static const char* piano_get_note_name(char note_char, int octave, int is_sharp)
{
    if(g_note_index >= (sizeof(g_note_names) / sizeof(g_note_names[0]))) 
    {
#if PIANO_LOG_ENABLE
        LV_LOG_ERROR("[Piano] Note name index overflow!\n");
#endif
        return "ERR"; 
    }
    
    char *target = g_note_names[g_note_index++];
    if(is_sharp) {
        snprintf(target, 8, "%cs%d", note_char, octave); 
    } else {
        snprintf(target, 8, "%c%d", note_char, octave);  
    }
    return target;
}

/**
 * @brief 创建所有琴键对象
 * @param[in] parent 琴键的父容器对象
 * @details 分为两个阶段：
 * 1. 计算并绘制白键，并在 C 键上添加文本标签。
 * 2. 根据乐理规则在白键缝隙上方叠加黑键。
 */
static void piano_create_keys(lv_obj_t * parent)
{
    g_note_index = 0;

    /* --- 生成所有白键 --- */
    for(int i = 0; i < PIANO_W_KEY_COUNT; i++) 
    {
        /* --- 乐理逻辑计算 --- */
        int octave   = PIANO_OCTAVE_START + (i / 7);  
        int note_idx = i % 7;                         
        char note_base = WHITE_NOTE_CHAR[note_idx];   

        const char* note_str = piano_get_note_name(note_base, octave, 0);

        /* --- 几何坐标计算 --- */
        int pos_x      = (i * PIANO_SIZE_W) / PIANO_W_KEY_COUNT;
        int next_pos_x = ((i + 1) * PIANO_SIZE_W) / PIANO_W_KEY_COUNT;
        int key_w      = next_pos_x - pos_x;

        /* --- UI 对象创建 --- */
        lv_obj_t * btn = lv_btn_create(parent);
        lv_obj_set_pos(btn, pos_x, 0);
        lv_obj_set_size(btn, key_w, PIANO_SIZE_H);

        lv_obj_add_style(btn, &style_white, 0);
        lv_obj_add_style(btn, &style_white_pr, LV_STATE_PRESSED);
        
        lv_obj_set_user_data(btn, (void*)note_str);
        lv_obj_add_flag(btn, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(btn, LV_OBJ_FLAG_PRESS_LOCK);
        lv_obj_add_event_cb(btn, piano_press_handler, LV_EVENT_PRESSED, NULL);

        if(note_idx == 0) 
        { 
            lv_obj_t * label = lv_label_create(btn);
            lv_label_set_text(label, note_str);
            lv_obj_set_style_text_color(label, lv_palette_main(LV_PALETTE_GREY), 0);
            lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -10);
        }
    }

    /* --- 生成所有黑键 --- */
    for(int i = 0; i < PIANO_W_KEY_COUNT - 1; i++)
    {
        /* --- 筛选逻辑 --- */
        int note_idx = i % 7;
        
        if (note_idx == 2 || note_idx == 6) continue;

        /* --- 乐理逻辑计算 --- */
        int octave   = PIANO_OCTAVE_START + (i / 7);
        char note_base = WHITE_NOTE_CHAR[note_idx];
        
        const char* note_str = piano_get_note_name(note_base, octave, 1);

        /* --- 几何坐标计算 --- */
        int border_x = ((i + 1) * PIANO_SIZE_W) / PIANO_W_KEY_COUNT;
        int pos_x    = border_x - (PIANO_B_KEY_W / 2);

        /* --- UI 对象创建 --- */
        lv_obj_t * key = lv_btn_create(parent);
        lv_obj_set_pos(key, pos_x, 0);
        lv_obj_set_size(key, PIANO_B_KEY_W, PIANO_B_KEY_H);

        lv_obj_add_style(key, &style_black, 0);
        lv_obj_add_style(key, &style_black_pr, LV_STATE_PRESSED);
        
        lv_obj_set_user_data(key, (void*)note_str);
        lv_obj_add_flag(key, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(key, LV_OBJ_FLAG_PRESS_LOCK);
        lv_obj_add_event_cb(key, piano_press_handler, LV_EVENT_PRESSED, NULL);
    }
}

/**
 * @brief 钢琴应用业务逻辑入口
 */
void piano_app_entry(void)
{
    lv_obj_t* scr = lv_scr_act();

    /* --- 钢琴容器创建 --- */
    lv_obj_t * piano = lv_obj_create(scr);
    lv_obj_remove_style_all(piano);
    lv_obj_set_size(piano, PIANO_SIZE_W, PIANO_SIZE_H); 
    lv_obj_center(piano);                               
    lv_obj_clear_flag(piano, LV_OBJ_FLAG_SCROLLABLE);   

    /* --- 钢琴琴键样式初始化 --- */
    piano_styles_init();       
    
    /* --- 钢琴琴键创建 --- */
    piano_create_keys(piano);
}