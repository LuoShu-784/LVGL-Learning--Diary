/**
 * @file switch_app.c
 * @brief "无用开关" (Useless Switch) 趣味应用实现
 * @author LuoShu
 * @version 1.0
 * @date 2026-02-07
 */

#include "lvgl.h"
#include <stdio.h>
#include "font_conf.h"
#include "audio_conf.h"

/** * @name 提示信息(Message) UI 配置
 */
#define MSG_FONT_SIZE       120         ///< 提示文字的字号大小
#define MSG_ALIGN_X_OFFSET  70          ///< 提示文字的 X 轴偏移量（基于左中对齐）
#define MSG_ALIGN_Y_OFFSET  0           ///< 提示文字的 Y 轴偏移量
#define MSG_COLOR_HEX       0x00FF00    ///< 提示文字颜色

/** @name 开关(Switch) UI 配置
 */
#define SW_SIZE_W           300         ///< 开关控件的宽度
#define SW_SIZE_H           140         ///< 开关控件的高度
#define SW_ALIGN_X_OFFSET   -20         ///< 开关的 X 轴偏移量（基于右中对齐）
#define SW_ALIGN_Y_OFFSET   0           ///< 开关的 Y 轴偏移量

/** 
 * @brief 定义情绪状态的总数
 */
#define STATE_NUM     5

/** @brief 不同阶段的提示文本库
 *  @details 随着用户不断尝试打开开关，文本语气逐渐升级。
 */
static const char* msgs[STATE_NUM + 1] = {
    "系统待机中...",
    "哎，别乱动。",
    "手痒是吧？",
    "警告：系统已急",
    "正在格式化...",
    "累了，毁灭吧!"
};

/** @brief 对应每个阶段的音效文件名
 *  @note 需配合 audio_conf.h 中的播放接口使用
 */
static const char* audio_wavs[STATE_NUM + 1] = {
    "我的刀盾",    ///< [0] 默认
    "我的刀盾",    ///< [1]
    "比比拉布",    ///< [2]
    "巴巴博一",    ///< [3]
    "咕咕嘎嘎",    ///< [4]
    "八嘎呀路",    ///< [5]
};

/** * @brief 对应每个阶段的开关指示器颜色
 */
static const lv_palette_t color[STATE_NUM + 1] = {
    LV_PALETTE_BLUE,
    LV_PALETTE_BLUE,
    LV_PALETTE_GREEN,
    LV_PALETTE_YELLOW,
    LV_PALETTE_RED,
    LV_PALETTE_RED
};

/** @brief 开关自动关闭的延迟时间 (毫秒)
 *  @details 数值越小，开关回弹越快。
 */
static const uint32_t time_ms[STATE_NUM + 1] = {
    700, 
    700,
    600,
    500,
    200,
    200,
};

/** 
 * @brief 用于显示提示信息的 LVGL 标签对象 
 */
static lv_obj_t* msg;

/**
 * @brief 定时器回调函数：强制关闭开关
 * @param[in] t 定时器指针，其 user_data 存储了开关对象(sw)
 * @details 当定时器触发时，检查开关是否处于打开状态，如果是则强制将其关闭。
 */
static void timer_callback(lv_timer_t* t) 
{
    lv_obj_t* sw = t->user_data;
    
    // 强制关闭开关
    if(lv_obj_has_state(sw, LV_STATE_CHECKED)) 
    {
        lv_obj_clear_state(sw, LV_STATE_CHECKED);
        lv_event_send(sw, LV_EVENT_VALUE_CHANGED, NULL); // 触发刷新
    }
}

/**
 * @brief 开关事件处理器
 * @param[in] e LVGL 事件对象
 * @details 处理开关的点击逻辑：
 * 1. 仅响应开关被打开的动作。
 * 2. 累加点击次数。
 * 3. 更新提示文字、开关颜色并播放音效。
 * 4. 启动单次定时器，在指定延迟后自动关闭开关。
 */
static void sw_event_handler(lv_event_t* e) 
{
    lv_obj_t* sw = lv_event_get_target(e);
    static uint32_t sw_count = 0;

    // 只有当开关变成 ON 时才触发逻辑
    if (!lv_obj_has_state(sw, LV_STATE_CHECKED)) return;

    // 状态索引递增，防止越界
    int state_idx = ++sw_count;
    if(state_idx > STATE_NUM) state_idx = STATE_NUM;

    // 更新文本
    lv_label_set_text(msg, msgs[state_idx]);
    
    // 更新开关颜色
    lv_obj_set_style_bg_color(sw, lv_palette_main(color[state_idx]), LV_PART_INDICATOR | LV_STATE_CHECKED);
    
    // 如果是最终状态，背景也变色
    if (state_idx == STATE_NUM) {
        lv_obj_set_style_bg_color(sw, lv_palette_main(color[state_idx]), 0);
    }

    // 启动自动关闭逻辑和音效
    lv_timer_create(timer_callback, time_ms[state_idx], sw);
    audio_play(audio_wavs[state_idx]);
}

/**
 * @brief 创建开关 UI
 * @param[in] parent 父对象（通常是当前屏幕）
 * @details 
 * 1. 加载指定大小的中文字体。
 * 2. 创建并配置提示标签（颜色、字体、对齐）。
 * 3. 创建并配置巨型开关（尺寸、对齐、事件回调）。
 */
static void create_useless_switch(lv_obj_t* parent) 
{
    // 获取字体资源
    lv_font_t* font_cn = font_manager_acquire(LV_FONT_TYPE_CN, MSG_FONT_SIZE);
    
    // 创建提示标签
    msg = lv_label_create(parent);
    lv_label_set_text(msg, msgs[0]);
    lv_obj_align(msg, LV_ALIGN_LEFT_MID, MSG_ALIGN_X_OFFSET, MSG_ALIGN_Y_OFFSET);
    lv_obj_set_style_text_color(msg, lv_color_hex(MSG_COLOR_HEX), 0); // 设置为黑客绿
    lv_obj_set_style_text_font(msg, font_cn, 0);

    // 创建开关
    lv_obj_t* sw = lv_switch_create(parent);
    lv_obj_set_size(sw, SW_SIZE_W, SW_SIZE_H);
    lv_obj_align(sw, LV_ALIGN_RIGHT_MID, SW_ALIGN_X_OFFSET, SW_ALIGN_Y_OFFSET);
    lv_obj_add_event_cb(sw, sw_event_handler, LV_EVENT_VALUE_CHANGED, NULL);
}

/**
 * @brief 应用程序入口
 */
void switch_app_entry(void)
{
    // 模块初始化
    lv_font_init();
    audio_init();

    // 获取当前屏幕并设置背景黑
    lv_obj_t* scr = lv_scr_act();
    lv_obj_set_style_bg_color(scr, lv_color_black(), 0);

    // 构建界面
    create_useless_switch(scr);
}