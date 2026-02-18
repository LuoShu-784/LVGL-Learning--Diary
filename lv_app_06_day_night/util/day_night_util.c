/**
 * @file day_night_util.c
 * @brief 昼夜切换通用工具库实现
 * @author LuoShu
 * @version 1.0
 * @date 2026-02-18
 */

#include "lvgl.h"
#include "day_night_util.h"

/**
 * @brief 基础圆形创建实现
 */
lv_obj_t* create_circle(lv_obj_t* parent, int size, uint32_t color) 
{
    lv_obj_t* circle = lv_obj_create(parent);
    lv_obj_remove_style_all(circle);
    lv_obj_set_align(circle, LV_ALIGN_CENTER);
    lv_obj_set_size(circle, size, size);
    lv_obj_set_style_radius(circle, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_opa(circle, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(circle, lv_color_hex(color), 0);
    lv_obj_clear_flag(circle, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
    return circle;
}

/**
 * @brief 主体容器创建实现
 * @details 通过创建一个带有裁剪属性但又取消裁剪标志的对象，作为布局基准。
 */
lv_obj_t* create_subject(lv_obj_t* parent, int w, int h)
{
    lv_obj_t* subject = lv_obj_create(parent);
    lv_obj_center(subject);
    lv_obj_set_size(subject, w, h);
    lv_obj_set_style_border_width(subject, 0, 0);
    lv_obj_set_style_outline_width(subject, 0, 0);
    lv_obj_set_style_shadow_width(subject, 0, 0);
    lv_obj_set_style_shadow_opa(subject, LV_OPA_TRANSP, 0);
    lv_obj_set_style_clip_corner(subject, true, 0);
    lv_obj_set_style_radius(subject, LV_RADIUS_CIRCLE, 0);
    lv_obj_clear_flag(subject, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_clip_corner(subject, false, 0); // 注意：此处根据逻辑可能需要后续调整
    return subject;
}

/**
 * @brief 星星自定义绘图事件回调
 * @param e 事件对象
 */
static void star_draw_event_cb(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    lv_draw_ctx_t * draw_ctx = lv_event_get_draw_ctx(e);

    lv_area_t coords;
    lv_obj_get_coords(obj, &coords);

    lv_color_t color = lv_obj_get_style_bg_color(obj, LV_PART_MAIN);

    int32_t w = lv_area_get_width(&coords);
    int32_t r = w / 2;

    // 定义四个角的遮罩区域
    lv_area_t area[4] = {
        {coords.x1 - r, coords.y1 - r, coords.x1 + r - 1, coords.y1 + r - 1},   // 左上
        {coords.x2 - r + 1, coords.y1 - r, coords.x2 + r, coords.y1 + r - 1},   // 右上
        {coords.x1 - r, coords.y2 - r + 1, coords.x1 + r - 1, coords.y2 + r},   // 左下
        {coords.x2 - r + 1, coords.y2 - r + 1, coords.x2 + r, coords.y2 + r}    // 右下
    };

    lv_draw_mask_radius_param_t mask[4];
    for (int i = 0; i < 4; i++) {
        lv_draw_mask_radius_init(&mask[i], &area[i], LV_RADIUS_CIRCLE, true);
    }

    int16_t id[4];
    for (int i = 0; i < 4; i++) {
        id[i] = lv_draw_mask_add(&mask[i], NULL);
    }

    // 绘制矩形
    lv_draw_rect_dsc_t rect_dsc;
    lv_draw_rect_dsc_init(&rect_dsc);
    rect_dsc.bg_color = color;
    rect_dsc.bg_opa = LV_OPA_COVER;
    lv_draw_rect(draw_ctx, &rect_dsc, &coords);

    for (int i = 0; i < 4; i++) {
        lv_draw_mask_remove_id(id[i]);
    }
}

/**
 * @brief 创建带有自定义绘图回调的星星对象
 */
lv_obj_t* create_star(lv_obj_t* parent, int size, uint32_t color)
{
    lv_obj_t* star = lv_obj_create(parent);
    lv_obj_remove_style_all(star);
    lv_obj_set_align(star, LV_ALIGN_CENTER);
    lv_obj_set_size(star, size, size);
    lv_obj_set_style_bg_color(star, lv_color_hex(color), 0);
    lv_obj_add_event_cb(star, star_draw_event_cb, LV_EVENT_DRAW_MAIN, NULL);
    lv_obj_clear_flag(star, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
    return star;
}

/**
 * @brief 基础背景创建
 */
lv_obj_t* create_background(lv_obj_t* parent, uint32_t color)
{
    lv_obj_t* background = lv_obj_create(parent);
    lv_obj_remove_style_all(background);
    lv_obj_set_align(background, LV_ALIGN_CENTER);
    lv_obj_set_size(background, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_color(background, lv_color_hex(color), 0);
    lv_obj_clear_flag(background, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
    return background;
}

/**
 * @brief 动画初始化辅助函数
 */
void anim_init(lv_anim_t* anim, void* var, int32_t time, lv_anim_exec_xcb_t exec_xcb, lv_anim_path_cb_t path_cb)
{
    lv_anim_init(anim);
    lv_anim_set_var(anim, (void*)var);
    lv_anim_set_time(anim, time);
    lv_anim_set_exec_cb(anim, exec_xcb);
    lv_anim_set_path_cb(anim, path_cb);
}