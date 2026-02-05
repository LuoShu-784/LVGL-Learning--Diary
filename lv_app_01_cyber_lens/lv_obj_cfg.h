/**
 * @file lv_obj_cfg.h
 * @brief LVGL 对象属性配置封装接口
 * @details 提供了一组简化的 API，用于快速设置 LVGL 对象的基础属性、背景、边框及阴影效果。
 * @author LuoShu
 * @version 1.0
 * @date 2026-02-04
 */

#ifndef __LV_OBJ_CFG_H_
#define __LV_OBJ_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "lvgl.h"

/**
 * @brief 设置对象的基础几何属性
 * @param[in] obj    目标 LVGL 对象指针
 * @param[in] w      宽度
 * @param[in] h      高度
 * @param[in] align  对齐方式 (如 LV_ALIGN_CENTER)
 * @param[in] x      X 轴偏移量
 * @param[in] y      Y 轴偏移量
 * @param[in] radius 圆角半径 (像素，或 LV_RADIUS_CIRCLE)
 */
void lv_obj_base_cfg(lv_obj_t* obj, int w, int h, lv_align_t align, lv_coord_t x, lv_coord_t y, lv_coord_t radius);

/**
 * @brief 设置对象的背景样式
 * @param[in] obj        目标 LVGL 对象指针
 * @param[in] color      主背景色 (0xRRGGBB 格式)
 * @param[in] grad_color 渐变背景色 (0xRRGGBB 格式)
 * @param[in] grad_dir   渐变方向 (如 LV_GRAD_DIR_VER)
 * @param[in] opa        背景不透明度 (如 LV_OPA_COVER)
 */
void lv_obj_bg_cfg(lv_obj_t* obj, uint32_t color, uint32_t grad_color, lv_grad_dir_t grad_dir, lv_opa_t opa);

/**
 * @brief 设置对象的边框样式
 * @param[in] obj   目标 LVGL 对象指针
 * @param[in] width 边框宽度
 * @param[in] color 边框颜色 (0xRRGGBB 格式)
 * @param[in] opa   边框不透明度
 */
void lv_obj_border_cfg(lv_obj_t* obj, int width, uint32_t color, lv_opa_t opa);

/**
 * @brief 设置对象的阴影样式
 * @param[in] obj   目标 LVGL 对象指针
 * @param[in] width 阴影模糊宽度
 * @param[in] color 阴影颜色 (0xRRGGBB 格式)
 * @param[in] opa   阴影不透明度
 */
void lv_obj_shadow_cfg(lv_obj_t* obj, int width, uint32_t color, lv_opa_t opa);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __LV_OBJ_CFG_H_ */