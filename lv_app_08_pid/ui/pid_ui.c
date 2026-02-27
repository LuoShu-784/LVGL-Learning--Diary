/**
 * @file pid_ui.c
 * @brief PID 仿真器 UI 界面的实现文件
 * @details 负责基于 LVGL 库构建控制面板、参数滑动条以及物理仿真显示区域。
 */

#include "pid_ui.h"

static lv_font_t * title_font = NULL; /**< 标题字体句柄 */
static lv_font_t * text_font = NULL;  /**< 常规文本字体句柄 */

/**
 * @brief 创建一个基础样式的 UI 容器对象
 * @param parent 父容器指针
 * @param w 宽度
 * @param h 高度
 * @return 指向创建的 lv_obj_t 对象的指针
 */
static lv_obj_t* ui_create_obj(lv_obj_t* parent, int w, int h)
{
    lv_obj_t* obj = lv_obj_create(parent);
    lv_obj_remove_style_all(obj);
    lv_obj_set_size(obj, w, h);
    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    return obj;
}

/**
 * @brief 创建一个文本按钮（基于 Label 实现）
 * @param parent 父容器指针
 * @param text 按钮显示的文本内容
 * @return 指向该 Label 对象的指针，该对象已被设为可点击
 */
static lv_obj_t* ui_create_btn(lv_obj_t* parent, const char* text) 
{
    lv_obj_t* btn = lv_label_create(parent);
    lv_obj_set_style_text_font(btn, text_font, 0);
    lv_label_set_text(btn, text);
    lv_obj_set_style_text_color(btn, lv_color_hex(COLOR_TEXT), 0);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CLICKABLE); 
    return btn;
}

/**
 * @brief 创建一个横向排列的 Flex 布局容器
 * @param parent 父容器指针
 * @return 指向创建的 Flex 容器对象的指针
 */
static lv_obj_t * ui_create_flex(lv_obj_t * parent) 
{
    lv_obj_t * row = ui_create_obj(parent, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(row, LV_OPA_TRANSP, 0);
    lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_gap(row, FLEX_PAD_GAP, 0);
    return row;
}

/**
 * @brief 创建一个带字体和宽度设置的标签
 * @param parent 父容器指针
 * @param text 文本内容
 * @param font 使用的字体指针
 * @param w 标签宽度
 * @return 指向创建的 Label 对象的指针
 */
static lv_obj_t* ui_create_label(lv_obj_t* parent, const char* text, const lv_font_t* font, lv_coord_t w)
{
    lv_obj_t * label = lv_label_create(parent);
    lv_obj_set_style_text_font(label, font, 0);
    lv_label_set_text(label, text);
    lv_obj_set_width(label, w);
    return label;
}

/**
 * @brief 创建一个带标题和数值显示的滑动条组合控件
 * @details 内部结构：[名称标签] --- [滑动条] --- [数值显示标签]
 * 其中滑动条通过 user_data 绑定了对应的数值标签，便于在回调中更新。
 * @param parent 父容器指针
 * @param name 滑动条名称
 * @param min 最小值
 * @param max 最大值
 * @return 指向创建的 slider 对象的指针
 */
static lv_obj_t * ui_create_slider(lv_obj_t* parent, const char* name, int min, int max) 
{
    lv_obj_t* row = ui_create_obj(parent, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_opa(row, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_gap(row, 15, 0);

    lv_obj_t* label = ui_create_label(row, name, text_font, SLIDER_LABEL_WIDTH);
    lv_obj_set_style_text_color(label, lv_color_hex(COLOR_TEXT), 0);

    lv_obj_t* slider = lv_slider_create(row);
    lv_obj_set_height(slider, SLIDER_HEIGHT);
    lv_obj_set_flex_grow(slider, 1);
    lv_slider_set_range(slider, min, max);
    lv_obj_set_style_bg_color(slider, lv_color_hex(SLIDER_BG_COLOR), LV_PART_MAIN);
    lv_obj_set_style_bg_color(slider, lv_color_hex(SLIDER_FILLED_COLOR), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(slider, lv_color_hex(SLIDER_KNOB_COLOR), LV_PART_KNOB);
    lv_obj_set_style_pad_all(slider, 2, LV_PART_KNOB); 

    lv_obj_t * value = lv_label_create(row);
    lv_obj_set_style_text_font(value, text_font, 0);
    lv_label_set_text(value, "0.0");
    lv_obj_set_style_text_color(value, lv_color_hex(COLOR_TEXT), 0);
    lv_obj_set_width(value, SLIDER_VALUE_WIDTH);
    lv_obj_set_style_border_width(value, 1, 0);
    lv_obj_set_style_border_side(value, LV_BORDER_SIDE_BOTTOM, 0);
    lv_obj_set_style_border_color(value, lv_color_hex(0x555555), 0);
    lv_obj_set_style_pad_bottom(value, -1, 0);
    lv_obj_set_style_text_align(value, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_add_flag(value, LV_OBJ_FLAG_CLICKABLE);

    /* 关键绑定：滑动条关联对应的数值标签 */
    lv_obj_set_user_data(slider, (void*)value);
    return slider;
}

/**
 * @brief 创建左侧控制面板
 * @details 包含模式切换（推动/修改）、系统复位以及环境干扰力控制。
 * @param parent 父容器指针
 * @param ui 存储控件句柄的 UI 结构体指针
 */
void ui_create_ctrl_panel_left(lv_obj_t* parent, ui_t* ui)
{
    lv_obj_t* left_panel = ui_create_obj(parent, lv_pct(48), lv_pct(100));
    lv_obj_align(left_panel, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_style_bg_opa(left_panel, LV_OPA_TRANSP, 0);
    lv_obj_set_flex_flow(left_panel, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(left_panel, 10, 0);
    lv_obj_set_style_pad_gap(left_panel, 5, 0);

    lv_obj_t* title = ui_create_label(left_panel, "全局设置", title_font, lv_pct(100));
    lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(COLOR_TEXT), 0);

    lv_obj_t* mouse_flex = ui_create_flex(left_panel);
    lv_obj_t* mouse_operation = ui_create_label(mouse_flex, "鼠标操作", text_font, 120);
    lv_obj_set_style_text_color(mouse_operation , lv_color_hex(COLOR_TEXT), 0);
    ui->opt_push = ui_create_btn(mouse_flex, "推动小球");
    ui->opt_mod = ui_create_btn(mouse_flex, "修改目标");
    lv_obj_set_style_text_color(ui->opt_mod, lv_color_hex(COLOR_TEXT_DIM), 0);

    lv_obj_t * reset_flex = ui_create_flex(left_panel);
    lv_obj_t * reset_operation = ui_create_label(reset_flex, "复位操作", text_font, 120);
    lv_obj_set_style_text_color(reset_operation, lv_color_hex(COLOR_TEXT), 0);
    ui->rst_ball = ui_create_btn(reset_flex, "复位小球");
    ui->rst_tgt = ui_create_btn(reset_flex, "复位目标");

    ui->dist = ui_create_slider(left_panel, "干扰恒力", 0, DIST_MAX);

    lv_obj_t * coord_row = ui_create_flex(left_panel);
    
    lv_obj_t* pos_cur = ui_create_label(coord_row, "当前位置: 0", text_font, 220);
    lv_obj_set_style_text_color(pos_cur, lv_color_hex(BALL_COLOR), 0);
    ui->pos_cur = pos_cur;

    lv_obj_t* pos_tgt = ui_create_label(coord_row, "预期位置: 0", text_font, 220);
    lv_obj_set_style_text_color(pos_tgt, lv_color_hex(COLOR_TEXT_DIM), 0);
    ui->pos_tgt = pos_tgt;
}

/**
 * @brief 创建右侧 PID 参数设置面板
 * @details 包含 KP, KI, KD 以及积分/输出限幅的滑动条。
 * @param parent 父容器指针
 * @param ui 存储控件句柄的 UI 结构体指针
 */
void ui_create_ctrl_panel_right(lv_obj_t* parent, ui_t* ui)
{
    static lv_point_t line_point[] = { {0, 0}, {0, PANEL_LINE_LEN} };
    lv_obj_t * line = lv_line_create(parent);
    lv_line_set_points(line, line_point, 2);
    lv_obj_align(line, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_line_width(line, 1, 0);
    lv_obj_set_style_line_color(line, lv_color_hex(0x555555), 0);

    lv_obj_t * right_panel = ui_create_obj(parent, lv_pct(48), lv_pct(100));
    lv_obj_align(right_panel, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_set_flex_flow(right_panel, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_bg_opa(right_panel, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_all(right_panel, 0, 0);
    lv_obj_set_style_pad_gap(right_panel, 0, 0);

    lv_obj_t * title = ui_create_label(right_panel, "PID 参数", title_font, lv_pct(100));
    lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(COLOR_TEXT), 0);

    ui->kp = ui_create_slider(right_panel, "KP", 0, KP_MAX);
    ui->ki = ui_create_slider(right_panel, "KI", 0, KI_MAX);
    ui->kd = ui_create_slider(right_panel, "KD", 0, KD_MAX);
    ui->int_lim = ui_create_slider(right_panel, "积分限幅", 0, INT_LIM_MAX);
    ui->out_lim = ui_create_slider(right_panel, "输出限幅", 0, OUT_LIM_MAX);
    
    /* 设置默认输出限幅初值 */
    lv_slider_set_value(ui->out_lim, OUT_LIM_DEF_VAL, LV_ANIM_OFF);
    lv_label_set_text(lv_obj_get_user_data(ui->out_lim), OUT_LIM_DEF_STR);
}

/**
 * @brief 创建底部的物理仿真动画区域
 * @details 包含轨道虚线、目标位置标记（圆环）和小球对象（实心圆）。
 * @param parent 父容器指针
 * @param ui 存储控件句柄的 UI 结构体指针
 */
void ui_create_simulation_area(lv_obj_t* parent, ui_t* ui)
{
    static lv_point_t track_points[] = { {0, 0}, {0, 0} };
    track_points[1].x = lv_obj_get_width(parent);

    lv_obj_t * track_line = lv_line_create(parent);
    lv_line_set_points(track_line, track_points, 2);
    lv_obj_align(track_line, LV_ALIGN_BOTTOM_MID, TACK_X_OFS, TACK_Y_OFS); 
    lv_obj_set_style_line_width(track_line, TACK_WIDTH, 0);
    lv_obj_set_style_line_color(track_line, lv_color_hex(TACK_COLOR), 0);
    lv_obj_set_style_line_dash_width(track_line, TACK_DASH_WIDTH, 0);
    lv_obj_set_style_line_dash_gap(track_line, TACK_DASH_GAP, 0);

    lv_obj_t* target = ui_create_obj(parent, BALL_SIZE, BALL_SIZE);
    lv_obj_set_style_radius(target, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_opa(target, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(target, 2, 0);
    lv_obj_set_style_border_color(target, lv_color_hex(COLOR_TEXT_DIM), 0);
    lv_obj_align(target, LV_ALIGN_BOTTOM_MID, BALL_X_OFS, BALL_Y_OFS);
    lv_obj_clear_flag(target, LV_OBJ_FLAG_CLICKABLE);
    ui->target = target;

    lv_obj_t* ball = ui_create_obj(parent, BALL_SIZE, BALL_SIZE);
    lv_obj_set_style_radius(ball, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(ball, lv_color_hex(BALL_COLOR), 0);
    lv_obj_align(ball, LV_ALIGN_BOTTOM_MID, BALL_X_OFS, BALL_Y_OFS); 
    lv_obj_clear_flag(ball, LV_OBJ_FLAG_CLICKABLE);
    ui->ball = ball;

    /* 小球内部的竖直指示标记 */
    lv_obj_t* ball_mark = ui_create_obj(ball, BALL_MARK_W, BALL_MARK_H);
    lv_obj_set_style_bg_color(ball_mark, lv_color_hex(BALL_MARK_COLOR), 0);
    lv_obj_center(ball_mark);
    lv_obj_clear_flag(ball_mark, LV_OBJ_FLAG_CLICKABLE);
}

/**
 * @brief PID UI 初始化总入口
 * @param parent UI 渲染的父对象（通常是当前 Screen）
 * @param ui 指向 UI 句柄结构体的指针，用于保存生成的控件
 */
void pid_ui_init(lv_obj_t* parent, ui_t* ui)
{
    /* 从字体管理器获取特定大小的字体 */
    title_font = font_manager_acquire(LV_FONT_TYPE_CN, FONT_SIZE_TITLE);
    text_font = font_manager_acquire(LV_FONT_TYPE_CN, FONT_SIZE_TEXT);

    lv_obj_set_style_bg_color(parent, lv_color_hex(COLOR_BG), 0);
    lv_obj_clear_flag(parent, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(parent, LV_OBJ_FLAG_OVERFLOW_VISIBLE);

    /* 创建顶部控制面板容器 */
    lv_obj_t * ctrl_panel = ui_create_obj(parent, PANEL_CTRL_W, PANEL_CTRL_H);
    lv_obj_align(ctrl_panel, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_opa(ctrl_panel, LV_OPA_TRANSP, 0);

    ui_create_ctrl_panel_left(ctrl_panel, ui);
    ui_create_ctrl_panel_right(ctrl_panel, ui);

    /* 创建底部仿真区 */
    ui_create_simulation_area(parent, ui);
}

/**
 * @brief 为“鼠标操作”切换按钮添加事件回调
 * @param ui UI 句柄
 * @param event_cb 回调函数
 */
void pid_ui_add_mode_event(ui_t* ui, lv_event_cb_t event_cb)
{
    lv_obj_add_event_cb(ui->opt_push, event_cb, LV_EVENT_CLICKED, (void*)UI_OPT_PUSH);
    lv_obj_add_event_cb(ui->opt_mod, event_cb, LV_EVENT_CLICKED, (void*)UI_OPT_MOD);
}

/**
 * @brief 为“系统复位”按钮添加事件回调
 * @param ui UI 句柄
 * @param event_cb 回调函数
 */
void pid_ui_add_reset_event(ui_t* ui, lv_event_cb_t event_cb)
{
    lv_obj_add_event_cb(ui->rst_ball, event_cb, LV_EVENT_CLICKED, (void*)UI_RST_BALL);
    lv_obj_add_event_cb(ui->rst_tgt, event_cb, LV_EVENT_CLICKED, (void*)UI_RST_TARGET);
}

/**
 * @brief 为所有参数滑动条添加数值改变回调
 * @param ui UI 句柄
 * @param event_cb 回调函数
 */
void pid_ui_add_slider_val_event(ui_t* ui, lv_event_cb_t event_cb)
{
    lv_obj_add_event_cb(ui->kp, event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(ui->ki, event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(ui->kd, event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(ui->int_lim, event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(ui->out_lim, event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(ui->dist, event_cb, LV_EVENT_VALUE_CHANGED, NULL);
}

/**
 * @brief 为所有滑动条的数值显示标签添加点击清零功能
 * @details 遍历所有滑动条，获取其关联的标签，并绑定点击事件。
 * 点击标签时，回调函数将通过 event_user_data 获取到对应的滑动条对象并执行重置。
 * @param ui UI 句柄
 * @param event_cb 回调函数
 */
void pid_ui_add_slider_clear_event(ui_t* ui, lv_event_cb_t event_cb)
{
    lv_obj_t* sliders[6];
    sliders[0] = ui->kp;
    sliders[1] = ui->ki;
    sliders[2] = ui->kd;
    sliders[3] = ui->int_lim;
    sliders[4] = ui->out_lim;
    sliders[5] = ui->dist;

    for (int i = 0; i < 6; i++)
    {
        /* 通过 user_data 找到对应的数值 Label */
        lv_obj_t* val = lv_obj_get_user_data(sliders[i]);
        lv_obj_add_flag(val, LV_OBJ_FLAG_CLICKABLE);
        /* 将 slider 对象作为参数传给回调函数 */
        lv_obj_add_event_cb(val, event_cb, LV_EVENT_CLICKED, (void*)sliders[i]);
    }
}