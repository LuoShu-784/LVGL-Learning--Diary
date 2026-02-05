/**
 * @file lv_obj_cfg.c
 * @brief LVGL 对象属性配置实现
 * @details 封装了 lv_obj_set_style_* 系列函数。
 * @author LuoShu
 * @version 1.0
 * @date 2026-02-04
 */

#include "lv_obj_cfg.h"

/**
 * @brief 基础几何属性配置实现
 */
void lv_obj_base_cfg(lv_obj_t* obj, int w, int h, lv_align_t align, lv_coord_t x, lv_coord_t y, lv_coord_t radius)
{
    lv_obj_remove_style_all(obj);           ///< 清除默认样式，确保从零开始配置
    lv_obj_set_size(obj, w, h);
    lv_obj_align(obj, align, x, y);
    lv_obj_set_style_radius(obj, radius, 0);
}

/**
 * @brief 背景样式配置实现
 */
void lv_obj_bg_cfg(lv_obj_t* obj, uint32_t color, uint32_t grad_color, lv_grad_dir_t grad_dir, lv_opa_t opa)
{
    lv_obj_set_style_bg_color(obj, lv_color_hex(color), 0);
    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(grad_color), 0);
    lv_obj_set_style_bg_grad_dir(obj, grad_dir, 0);
    lv_obj_set_style_bg_opa(obj, opa, 0);
}

/**
 * @brief 边框样式配置实现
 */
void lv_obj_border_cfg(lv_obj_t* obj, int width, uint32_t color, lv_opa_t opa)
{
    lv_obj_set_style_border_width(obj, width, 0); 
    lv_obj_set_style_border_color(obj, lv_color_hex(color), 0);
    lv_obj_set_style_border_opa(obj, opa, 0);
}

/**
 * @brief 阴影样式配置实现
 */
void lv_obj_shadow_cfg(lv_obj_t* obj, int width, uint32_t color, lv_opa_t opa)
{
    lv_obj_set_style_shadow_width(obj, width, 0);
    lv_obj_set_style_shadow_color(obj, lv_color_hex(color), 0);
    lv_obj_set_style_shadow_opa(obj, opa, 0);
}