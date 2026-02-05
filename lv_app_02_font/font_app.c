/**
 * @file font_app.c
 * @brief 字体功能演示应用
 * @details 展示如何使用动态请求不同大小的字体并应用到 Label 控件上。
 * @author LuoShu
 * @version 1.0
 * @date 2026-02-04
 */

#include "lvgl.h"
#include "font_conf.h"

/**
 * @brief 字体应用入口函数
 * @details 创建演示界面，包含中文与英文标签
 */
void font_app_entry(void)
{
    lv_obj_t* src = lv_scr_act();

    /* ======== 创建中文标签 ======== */
    lv_obj_t* label_cn = lv_label_create(src);

    /* ======== 动态获取中文字体 ======== */
    lv_font_t* font_cn = font_manager_acquire(LV_FONT_TYPE_CN, 100);
    lv_obj_set_style_text_font(label_cn, font_cn, 0);

    lv_label_set_text(label_cn, "洛书·嵌入式进阶");
    lv_obj_align(label_cn, LV_ALIGN_CENTER, 0, -30);

    /* ======== 创建英文标签 ======== */
    lv_obj_t* label_en = lv_label_create(src);
    
    /* ======== 动态获取英文字体 ======== */
    lv_font_t* font_en = font_manager_acquire(LV_FONT_TYPE_EN, 60);
    lv_obj_set_style_text_font(label_en, font_en, 0);
    
    lv_label_set_text(label_en, "LVGL-FreeType-114514");
    lv_obj_align_to(label_en, label_cn, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

}