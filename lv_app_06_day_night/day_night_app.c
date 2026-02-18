/**
 * @file day_night_app.c
 * @brief 昼夜切换应用业务逻辑
 * @author LuoShu
 * @version 1.0
 * @date 2026-02-18
 */

#include "lvgl.h"
#include "day_night_ui.h"
#include "day_night_util.h"

/** 
 * @brief 动画持续时间 (ms) 
 */
#define ANIM_TIME               500


static ui_t* ui = NULL;

/**
 * @brief 透明度动画回调
 * @details 控制月亮的淡入淡出、背景的整体暗度以及阴影的显隐。
 */
static void app_opa_cb(void* var, int32_t val)
{
    // 子对象索引 1 是 moon (btn_create 中第二个添加的子对象)
    lv_obj_t* moon = lv_obj_get_child(ui->btn, 1);
    lv_obj_set_style_opa(moon, val, 0);

    lv_obj_set_style_bg_opa(ui->background, val, 0);
    
    lv_obj_t* shadow = ui->inner_shadow;
    if (val >= SUBJ_IN_SHD_OPA) 
    {
        lv_obj_set_style_shadow_opa(shadow, val, 0);
    }
}

/**
 * @brief 水平移动动画回调
 * @details 负责按钮的左右移动，以及背景光晕圆环的视差移动。
 */
static void app_hor_move_cb(void* var, int32_t val)
{
    lv_obj_set_x(ui->btn, val);

    for (int i = 0; i < SUBJ_BG_COUNT; i++)
    {
        // 根据按钮位置计算背景圆环的偏移，产生视差效果
        int32_t offset = val + (i * val * SUBJ_DAY_BG_X_OFS_INC) / SUBJ_DAY_BG_X_OFS;
        lv_obj_set_x(ui->bg_circles[i], offset);
    }
}

/**
 * @brief 垂直移动动画回调
 * @details 负责背景层（天空颜色）和前景层（云朵/星星）的上下切换。
 */
static void app_ver_move_cb(void* var, int32_t val)
{
    lv_obj_t* bg = ui->layer_bg;
    lv_obj_t* fg = ui->layer_fg;

    lv_obj_set_y(bg, val);
    lv_obj_set_y(fg, val);
}

/**
 * @brief 启动透明度相关动画
 */
static void app_opa_set_anim(void* var)
{
    lv_anim_t opa_anim;
    anim_init(&opa_anim, var, ANIM_TIME / 2, app_opa_cb, lv_anim_path_linear);    
    if (ui->is_day) {
        lv_anim_set_values(&opa_anim, 0, 255);
    } else {
        lv_anim_set_values(&opa_anim, 255, 0);
    }
    lv_anim_start(&opa_anim);
}

/**
 * @brief 启动位移相关动画
 */
static void app_move_set_anim(void* var)
{
    lv_anim_t ver_move_anim, hor_move_anim;
    anim_init(&ver_move_anim, var, ANIM_TIME, app_ver_move_cb, lv_anim_path_overshoot);
    anim_init(&hor_move_anim, var, ANIM_TIME, app_hor_move_cb, lv_anim_path_overshoot);
    if(ui->is_day) {
        lv_anim_set_values(&ver_move_anim, 0, SUBJ_H * 2);
        lv_anim_set_values(&hor_move_anim, SUBJ_DAY_BG_X_OFS, SUBJ_NIGHT_BG_X_OFS);
    } else {
        lv_anim_set_values(&ver_move_anim, SUBJ_H * 2, 0);
        lv_anim_set_values(&hor_move_anim, SUBJ_NIGHT_BG_X_OFS, SUBJ_DAY_BG_X_OFS);
    }
    lv_anim_start(&ver_move_anim);
    lv_anim_start(&hor_move_anim);
}

/**
 * @brief 点击事件处理
 * @details 限制点击频率，更新动画和 UI 状态。
 */
static void app_clicked_event_cb(lv_event_t* e)
{
    static uint32_t last_click_time = 0;
    
    // 防止动画未结束时重复点击
    if (last_click_time != 0 && (lv_tick_get() - last_click_time <= ANIM_TIME)) return;
    
    last_click_time = lv_tick_get();

    app_opa_set_anim(ui);
    app_move_set_anim(ui);

    ui_inner_shadow_switch(ui);

    ui->is_day = !(ui->is_day);    
}

static void app_add_event(ui_t* ui)
{
    lv_obj_add_event_cb(ui->subject, app_clicked_event_cb, LV_EVENT_CLICKED, NULL);
}

/**
 * @brief 应用入口函数
 */
void day_night_app_entry(void)
{
    lv_obj_t* scr = lv_scr_act();
    ui = ui_create(scr);
    app_add_event(ui);
}