/**
 * @file HCNY_app.c
 * @brief 新年特效展示应用 (Happy Chinese New Year)
 * @author LuoShu
 * @version 1.0
 * @date 2026-02-21
 */

#include "lvgl.h"
#include "res_conf.h"
#include <stdlib.h>
#include <stdio.h>

/** 
 * @brief 图片动画持续时间 (毫秒) 
 */
#define IMAGE_ANIM_TIME 2000

/**
 * @brief 图片缩放动画回调函数
 * @param[in] var 动画目标对象 (lv_obj_t 指针)
 * @param[in] val 当前动画插值 (缩放比例)
 */
void img_zoom_anim_cb(void *var, int32_t val)
{
    lv_obj_t * img = (lv_obj_t*)var;
    lv_img_set_zoom(img, val);
}

/**
 * @brief 图片旋转动画回调函数
 * @param[in] var 动画目标对象 (lv_obj_t 指针)
 * @param[in] val 当前动画插值 (旋转角度，0.1度为单位)
 */
void img_rotate_anim_cb(void *var, int32_t val)
{
    lv_obj_t * img = (lv_obj_t*)var;
    lv_img_set_angle(img, val);
}

/**
 * @brief 结束定时器回调函数
 */
void img_timer_cb(lv_timer_t* timer)
{
    // do sth...
}

/**
 * @brief 图片动画完成回调函数
 * @param[in] a 触发此回调的动画对象
 * @details 图片展示动画完成后，启动一个 500ms 的延时定时器，随后触发退出流程。
 */
void img_anim_ready_cb(lv_anim_t * a)
{
    lv_timer_t* timer = lv_timer_create(img_timer_cb, 500, NULL);
    lv_timer_set_repeat_count(timer, 1);
}

/**
 * @brief 初始化并启动图片展示阶段
 * @details 创建图片控件，设置数据源，并同时应用缩放 (Zoom) 与旋转 (Angle) 动画。
 */
void HCNY_img_init(void)
{
    lv_obj_t * img = lv_img_create(lv_scr_act());
    lv_img_set_src(img, GET_IMAGE_PATH(IMAGE_NAME));
    lv_obj_center(img);
    lv_img_set_zoom(img, 0); // 初始缩放设为 0

    // 缩放动画
    lv_anim_t zoom_anim;
    lv_anim_init(&zoom_anim);
    lv_anim_set_var(&zoom_anim, img);
    lv_anim_set_exec_cb(&zoom_anim, img_zoom_anim_cb);
    lv_anim_set_values(&zoom_anim, 0, 228); // 放大至 256 为原大小的比例
    lv_anim_set_time(&zoom_anim, IMAGE_ANIM_TIME);
    lv_anim_start(&zoom_anim);

    // 旋转动画
    lv_anim_t rotate_anim;
    lv_anim_init(&rotate_anim);
    lv_anim_set_var(&rotate_anim, img);
    lv_anim_set_exec_cb(&rotate_anim, img_rotate_anim_cb);
    lv_anim_set_values(&rotate_anim, 0, 3600); // 旋转 360 度
    lv_anim_set_ready_cb(&rotate_anim, img_anim_ready_cb);
    lv_anim_set_time(&rotate_anim, IMAGE_ANIM_TIME);
    lv_anim_start(&rotate_anim);
}

/**
 * @brief 进度条数值更新回调
 * @param[in] var 进度条对象
 * @param[in] val 当前进度值 (0-100)
 * @details 同步更新进度条 UI 以及右侧的百分比文本标签。
 */
void bar_set_anim_cb(void *var, int32_t val)
{
    lv_obj_t* bar = (lv_obj_t*)var;
    lv_obj_t* percent = lv_obj_get_user_data(bar); // 从 user_data 中获取绑定的文本标签
    lv_bar_set_value(bar, val, LV_ANIM_OFF);
    lv_label_set_text_fmt(percent, "%d%%", val);
}

/**
 * @brief 成功提示延时结束回调
 * @param[in] timer 定时器对象
 * @details 延时结束后，销毁提示文本，并进入图片展示阶段。
 */
void text_timer_cb(lv_timer_t * timer)
{
    lv_obj_del(timer->user_data); // user_data 中存储了提示文本对象
    HCNY_img_init();
}

/**
 * @brief 进度条动画完成回调
 * @param[in] a 触发此回调的动画对象
 */
void bar_anim_ready_cb(lv_anim_t * a)
{
    lv_obj_del(a->var);

    lv_obj_t* text = lv_label_create(lv_scr_act());

    lv_font_t* font = font_manager_acquire(LV_FONT_TYPE_CN, 100);
    lv_obj_set_style_text_font(text, font, 0);

    lv_obj_set_style_text_color(text, lv_color_hex(0xffffff), 0);

    lv_label_set_text(text, "解冻成功！");
    lv_obj_center(text);

    // 提示显示 1000ms 后切换场景
    lv_timer_t* timer = lv_timer_create(text_timer_cb, 1000, text);
    lv_timer_set_repeat_count(timer, 1);
}

/**
 * @brief 初始化进度条周边文本
 * @param[in] parent 进度条容器对象
 * @details 创建“解冻ing”标题与动态更新的百分比标签，并将百分比标签绑定到 parent 的 user_data。
 */
void HCNY_text_init(lv_obj_t * parent)
{
    lv_obj_t* text = lv_label_create(parent);
    lv_obj_t* percent = lv_label_create(parent);

    lv_font_t* font = font_manager_acquire(LV_FONT_TYPE_CN, 50);
    lv_obj_set_style_text_font(text, font, 0);
    lv_obj_set_style_text_font(percent, font, 0);

    lv_obj_set_style_text_color(text, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_color(percent, lv_color_hex(0xffffff), 0);

    lv_label_set_text(text, "解冻ing");
    lv_label_set_text(percent, "0%");
    
    // 排版调整
    lv_obj_align(text, LV_ALIGN_OUT_TOP_MID, 20, -60);
    lv_obj_align(percent, LV_ALIGN_OUT_RIGHT_MID, 220, -20);

    lv_obj_set_user_data(parent, (void*)percent);
}

/**
 * @brief 初始化进度条及进场动画
 * @details 创建进度条 UI，并挂载 0-100 的数值增长动画。
 */
void HCNY_bar_init(void)
{
    lv_obj_t * bar = lv_bar_create(lv_scr_act());
    lv_obj_set_size(bar, 200, 20);
    lv_obj_center(bar);
    
    lv_obj_set_style_bg_color(bar,lv_color_hex(0xdc143c), LV_PART_INDICATOR); // 进度条前景色 (深红)
    lv_obj_set_style_bg_color(bar,lv_color_hex(0x00bfff), LV_PART_MAIN);      // 进度条背景色 (深天蓝)
    lv_obj_add_flag(bar, LV_OBJ_FLAG_OVERFLOW_VISIBLE); // 允许子对象(文本)显示在区域外
    
    HCNY_text_init(bar);

    lv_anim_t bar_anim;
    lv_anim_init(&bar_anim);
    lv_anim_set_var(&bar_anim, bar);
    lv_anim_set_values(&bar_anim, 0, 100);
    lv_anim_set_exec_cb(&bar_anim, bar_set_anim_cb);
    lv_anim_set_ready_cb(&bar_anim, bar_anim_ready_cb);
    lv_anim_set_time(&bar_anim, 1000);
    lv_anim_set_repeat_count(&bar_anim, 1);
    lv_anim_start(&bar_anim);
}

/**
 * @brief 新年特效应用入口函数
 */
void HCNY_app_entry(void)
{
    lv_obj_t* scr = lv_scr_act();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), 0);
    lv_obj_clear_flag(scr, LV_OBJ_FLAG_SCROLLABLE);
    
    HCNY_bar_init();
}