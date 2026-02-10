/**
 * @file login_app.c
 * @brief 登录界面应用
 * @details 实现现代风格登录界面。
 * @author LuoShu
 * @version 1.0
 * @date 2026-02-09
 */

#include "lvgl.h"
#include <stdio.h>
#include <string.h>
#include "audio_conf.h"

/* ================== 用户配置宏 ================== */

#define LOGIN_PASSWORD          "114514"        ///< 系统默认登录密码
#define LOGIN_USERNAME          "LuoShu"        ///< 显示的用户名
#define LOGIN_SUCCESS_AUDIO     "login_success" ///< 登录成功提示音文件名 (不含后缀)
#define LOGIN_FAIL_AUDIO        "login_fail"    ///< 登录失败提示音文件名 (不含后缀)

/* ================== 布局尺寸配置 ================== */

#define USER_PANEL_WIDTH        LV_PCT(35)      ///< 左侧用户信息面板宽度
#define USER_PANEL_HEIGHT       LV_PCT(100)     ///< 左侧用户信息面板高度
#define USER_NAME_X_OFFSET      0               ///< 用户名文本 X 轴偏移
#define USER_NAME_Y_OFFSET      10              ///< 用户名文本 Y 轴偏移

#define AVATAR_SIZE             128             ///< 头像容器整体尺寸
#define AVATAR_HEAD_SIZE        LV_PCT(31)      ///< 头像头部圆圈大小
#define AVATAR_HEAD_X_OFFSET    0               ///< 头部 X 轴偏移
#define AVATAR_HEAD_Y_OFFSET    18              ///< 头部 Y 轴偏移
#define AVATAR_BODY_SIZE        LV_PCT(60)      ///< 头像身体半圆大小
#define AVATAR_BODY_X_OFFSET    0               ///< 身体 X 轴偏移
#define AVATAR_BODY_Y_OFFSET    15              ///< 身体 Y 轴偏移

#define TEXTAREA_WIDTH          260             ///< 密码输入框宽度

#define KEYBOARD_WIDTH          LV_PCT(65)      ///< 右侧键盘区域宽度
#define KEYBOARD_HEIGHT         LV_PCT(100)     ///< 右侧键盘区域高度

/**
 * @brief 密码输入框事件回调函数
 * @param[in] e LVGL 事件对象
 * @details 处理输入框的交互逻辑：
 */
static void pass_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);

    if(code == LV_EVENT_READY) 
    {
        const char * txt = lv_textarea_get_text(ta);
        
        if(strcmp(txt, LOGIN_PASSWORD) == 0) 
        {
            // 验证成功：绿色边框
            lv_obj_set_style_border_color(ta, lv_color_hex(0x00FF00), LV_STATE_FOCUSED);
            lv_obj_set_style_border_width(ta, 2, LV_STATE_FOCUSED);
            audio_play(LOGIN_SUCCESS_AUDIO);
        } 
        else 
        {
            // 验证失败：红色边框并清空
            lv_textarea_set_text(ta, "");
            lv_obj_set_style_border_color(ta, lv_color_hex(0xFF0000), LV_STATE_FOCUSED);
            lv_obj_set_style_border_width(ta, 2, LV_STATE_FOCUSED);
            audio_play(LOGIN_FAIL_AUDIO);
        }
    }
    else if(code == LV_EVENT_VALUE_CHANGED) 
    {
        // 输入时恢复默认焦点样式
        lv_obj_set_style_border_color(ta, lv_palette_main(LV_PALETTE_CYAN), LV_STATE_FOCUSED);
        lv_obj_set_style_border_width(ta, 2, LV_STATE_FOCUSED);
    }
}

/**
 * @brief 创建全屏渐变背景
 * @param[in] parent 父容器
 * @return 创建的背景对象指针
 */
static lv_obj_t* create_background(lv_obj_t* parent)
{
    lv_obj_t * bg = lv_obj_create(parent);
    lv_obj_set_size(bg, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_pad_all(bg, 0, 0);
    lv_obj_set_style_border_width(bg, 0, 0);
    lv_obj_set_style_radius(bg, 0, 0);

    lv_obj_set_style_bg_color(bg, lv_palette_darken(LV_PALETTE_INDIGO, 3), 0);
    lv_obj_set_style_bg_grad_color(bg, lv_palette_darken(LV_PALETTE_BLUE, 2), 0);
    lv_obj_set_style_bg_grad_dir(bg, LV_GRAD_DIR_HOR, 0);

    return bg;
}

/**
 * @brief 绘制用户信息区域 (头像与用户名)
 * @param[in] parent 用户信息面板容器
 * @details 通过 LVGL 的几何形状组合绘制一个抽象的人像图标。
 */
static void create_user_info(lv_obj_t * parent)
{
    // 头像外框容器
    lv_obj_t * avatar = lv_obj_create(parent);
    lv_obj_set_size(avatar, AVATAR_SIZE, AVATAR_SIZE);
    lv_obj_set_style_radius(avatar, LV_RADIUS_CIRCLE, 0);
    
    lv_obj_set_style_bg_color(avatar, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(avatar, LV_OPA_20, 0); // 半透明背景
    lv_obj_set_style_border_width(avatar, 2, 0);
    lv_obj_set_style_border_color(avatar, lv_color_white(), 0);
    lv_obj_set_style_border_opa(avatar, LV_OPA_50, 0);
    lv_obj_clear_flag(avatar, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_clip_corner(avatar, true, 0); // 关键：开启裁剪
    
    lv_obj_set_style_pad_all(avatar, 0, 0); 

    // 绘制头部 (Head)
    lv_obj_t * head = lv_obj_create(avatar);
    lv_obj_set_size(head, AVATAR_HEAD_SIZE, AVATAR_HEAD_SIZE); 
    lv_obj_set_style_radius(head, LV_RADIUS_CIRCLE, 0);
    
    lv_obj_set_style_bg_color(head, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(head, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(head, 0, 0);
    
    lv_obj_align(head, LV_ALIGN_TOP_MID, AVATAR_HEAD_X_OFFSET, AVATAR_HEAD_Y_OFFSET);
    lv_obj_clear_flag(head, LV_OBJ_FLAG_SCROLLABLE);

    // 绘制身体 (Body)
    lv_obj_t * body = lv_obj_create(avatar);
    lv_obj_set_size(body, AVATAR_BODY_SIZE, AVATAR_BODY_SIZE);
    lv_obj_set_style_radius(body, LV_RADIUS_CIRCLE, 0);
    
    lv_obj_set_style_bg_color(body, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(body, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(body, 0, 0);
    
    lv_obj_align(body, LV_ALIGN_BOTTOM_MID, AVATAR_BODY_X_OFFSET, AVATAR_BODY_Y_OFFSET); 

    // 用户名标签
    lv_obj_t * label = lv_label_create(parent);
    lv_label_set_text(label, LOGIN_USERNAME);
    lv_obj_set_style_text_color(label, lv_color_white(), 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_24, 0);
    lv_obj_align_to(label, avatar, LV_ALIGN_OUT_BOTTOM_MID, USER_NAME_X_OFFSET, USER_NAME_Y_OFFSET); 
    
}

/**
 * @brief 创建密码输入框
 * @param[in] parent 用户信息面板容器
 * @details 配置为单行密码模式，设置占位符，并绑定验证回调事件。
 */
static lv_obj_t* create_user_textarea(lv_obj_t * parent)
{
    lv_obj_t* ta = lv_textarea_create(parent);
    lv_textarea_set_one_line(ta, true);
    lv_textarea_set_password_mode(ta, true); // 开启密码掩码 (*号)
    lv_textarea_set_placeholder_text(ta, "Password");
    lv_obj_set_width(ta, TEXTAREA_WIDTH);
    lv_obj_add_event_cb(ta, pass_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_add_state(ta, LV_STATE_FOCUSED);
    
    // 样式设置
    lv_obj_set_style_text_font(ta, &lv_font_montserrat_24, 0);
    lv_obj_set_style_bg_opa(ta, LV_OPA_TRANSP, 0);
    lv_obj_set_style_text_color(ta, lv_color_white(), 0);
    lv_obj_set_style_text_align(ta, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_style_radius(ta, 4, 0);
    lv_obj_set_style_border_side(ta, LV_BORDER_SIDE_FULL, 0);
    lv_obj_set_style_border_width(ta, 1, 0);
    lv_obj_set_style_border_color(ta, lv_color_white(), 0);
    
    // 聚焦状态样式
    lv_obj_set_style_border_width(ta, 2, LV_STATE_FOCUSED);
    lv_obj_set_style_border_color(ta, lv_palette_main(LV_PALETTE_CYAN), LV_STATE_FOCUSED);
    return ta;
}

/**
 * @brief 创建左侧用户信息容器
 * @param[in] parent 背景对象
 */
static lv_obj_t* create_user_panel(lv_obj_t * parent)
{
    lv_obj_t* panel = lv_obj_create(parent);
    lv_obj_set_size(panel, USER_PANEL_WIDTH, USER_PANEL_HEIGHT);
    lv_obj_align(panel, LV_ALIGN_LEFT_MID, 0, 0); 
    
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(panel, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    
    lv_obj_set_style_bg_opa(panel, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(panel, 0, 0);
    lv_obj_set_style_pad_gap(panel, 20, 0); // 控件间距
    lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE);
    return panel;
}

/**
 * @brief 创建右侧虚拟键盘
 * @param[in] parent 背景对象
 */
static lv_obj_t* create_keyboard(lv_obj_t * parent)
{
    lv_obj_t* kb = lv_keyboard_create(parent);
    lv_obj_set_size(kb, KEYBOARD_WIDTH, KEYBOARD_HEIGHT);
    lv_obj_align(kb, LV_ALIGN_RIGHT_MID, 0, 0);
    
    lv_obj_set_style_bg_opa(kb, LV_OPA_TRANSP, 0); 
    lv_obj_set_style_border_width(kb, 0, 0);

    // 默认按键样式
    lv_obj_set_style_bg_color(kb, lv_color_white(), LV_PART_ITEMS);
    lv_obj_set_style_bg_opa(kb, LV_OPA_10, LV_PART_ITEMS);
    lv_obj_set_style_border_width(kb, 1, LV_PART_ITEMS);
    lv_obj_set_style_border_color(kb, lv_color_white(), LV_PART_ITEMS);
    lv_obj_set_style_border_opa(kb, LV_OPA_30, LV_PART_ITEMS);
    lv_obj_set_style_text_color(kb, lv_color_white(), LV_PART_ITEMS);
    lv_obj_set_style_radius(kb, 4, LV_PART_ITEMS);

    // 按下状态样式
    lv_obj_set_style_bg_color(kb, lv_palette_main(LV_PALETTE_CYAN), LV_PART_ITEMS | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(kb, LV_OPA_80, LV_PART_ITEMS | LV_STATE_PRESSED);
    lv_obj_set_style_text_color(kb, lv_color_black(), LV_PART_ITEMS | LV_STATE_PRESSED);

    return kb;
}

/**
 * @brief 登录应用入口函数
 * @details 构建 UI 结构，并将键盘与输入框关联。
 */
void login_app_entry(void)
{
    // 硬件初始化
    audio_init();

    // 创建基础图层
    lv_obj_t* scr = lv_scr_act();
    lv_obj_t* bg = create_background(scr);
    
    // 创建左侧面板
    lv_obj_t* panel = create_user_panel(bg);
    create_user_info(panel);
    lv_obj_t* ta = create_user_textarea(panel);

    // 创建右侧键盘并绑定
    lv_obj_t* kb = create_keyboard(bg);
    lv_keyboard_set_textarea(kb, ta);
}