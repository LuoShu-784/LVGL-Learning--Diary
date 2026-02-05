/**
 * @file cyber_lens.c
 * @brief 赛博风格镜头 UI 组件
 * @details 使用 LVGL 基础对象 绘制一个高仿真的相机镜头 UI。
 * 包含阴影、渐变、透视模拟和光学反光效果。
 * @author LuoShu
 * @version 1.0
 * @date 2026-02-04
 */

#include "lvgl.h"
#include "lv_obj_cfg.h"
#include <stdio.h>

/**
 * @brief 绘制金属螺丝
 * @param[in] parent 父对象
 * @param[in] x 螺丝中心的 X 坐标偏移
 * @param[in] y 螺丝中心的 Y 坐标偏移
 * @details 通过三层结构（沉孔阴影、金属头、螺丝槽）模拟立体感。
 */
static void create_screw_realistic(lv_obj_t * parent, int x, int y) 
{
    // 沉孔，模拟金属外壳凹陷
    lv_obj_t * hole = lv_obj_create(parent);
    lv_obj_base_cfg(hole, 22, 22, LV_ALIGN_CENTER, x, y, LV_RADIUS_CIRCLE);
    lv_obj_bg_cfg(hole, 0x050505, 0xffffff, LV_GRAD_DIR_NONE, LV_OPA_COVER);

    // 沉孔边缘微弱高光，增加立体感
    lv_obj_border_cfg(hole, 1, 0x555555, LV_OPA_30);

    // 螺丝头金属主体，水平渐变模拟圆柱光泽
    lv_obj_t * head = lv_obj_create(hole);
    lv_obj_base_cfg(head, 14, 14, LV_ALIGN_CENTER, 0, 0, LV_RADIUS_CIRCLE);
    lv_obj_bg_cfg(head, 0x777777, 0xDDDDDD, LV_GRAD_DIR_HOR, LV_OPA_COVER);

    // 螺丝槽 也就是"一字"或"十字"槽的核心部分
    lv_obj_t * groove = lv_obj_create(head);
    lv_obj_base_cfg(groove, 6, 6, LV_ALIGN_CENTER, 0, 0, LV_RADIUS_CIRCLE);
    lv_obj_bg_cfg(groove, 0x222222, 0xffffff, LV_GRAD_DIR_NONE, LV_OPA_COVER);
    lv_obj_border_cfg(groove, 1, 0xffffff, LV_OPA_60);
}

/**
 * @brief 创建赛博镜头主界面
 * @details 核心绘制函数，按以下层级顺序构建 UI：
 * 1. 背景与机身底座
 * 2. 四颗装饰性螺丝
 * 3. 外部金属环与镜筒内壁
 * 4. 光学玻璃主体
 * 5. 内部光圈结构
 * 6. 多层光学镀膜反光 (洋红、青色)
 * 7. 表面高光与镜面渐变
 * 8. 红色状态指示灯
 */
void cyber_lens_ctreate(void)
{
    lv_obj_t * scr = lv_scr_act();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x050505), 0);

    // --- 机身底座 (Body Base) ---
    lv_obj_t *body = lv_obj_create(scr);
    lv_obj_base_cfg(body, 280, 240, LV_ALIGN_CENTER, 0, 0, 30);
    lv_obj_bg_cfg(body, 0x181818, 0x080808, LV_GRAD_DIR_VER, LV_OPA_COVER);
    lv_obj_border_cfg(body, 2, 0x444444, LV_OPA_50);

    // --- 装饰螺丝 ---
    create_screw_realistic(body, -115, -95); // 左上
    create_screw_realistic(body,  115, -95); // 右上
    create_screw_realistic(body, -115,  95); // 左下
    create_screw_realistic(body,  115,  95); // 右下
    
    // --- 外部金属环 ---
    lv_obj_t * lens_base = lv_obj_create(body);
    lv_obj_base_cfg(lens_base, 200, 200, LV_ALIGN_CENTER, 0, 0, LV_RADIUS_CIRCLE);
    lv_obj_bg_cfg(lens_base, 0x050505, 0xffffff, LV_GRAD_DIR_NONE, LV_OPA_COVER);
    lv_obj_border_cfg(lens_base, 1, 0x333333, LV_OPA_COVER); // 微弱的金属边缘
    
    // --- 镜筒内壁 ---
    lv_obj_t * inner_barrel = lv_obj_create(lens_base);
    lv_obj_base_cfg(inner_barrel, 140, 140, LV_ALIGN_CENTER, 0, 0, LV_RADIUS_CIRCLE);
    lv_obj_bg_cfg(inner_barrel, 0x111111, 0xffffff, LV_GRAD_DIR_NONE, LV_OPA_COVER);
    lv_obj_border_cfg(inner_barrel, 4, 0x2d2d2d, LV_OPA_COVER);

    // --- 光学玻璃 ---
    lv_obj_t * glass_main = lv_obj_create(inner_barrel);
    lv_obj_base_cfg(glass_main, 125, 125, LV_ALIGN_CENTER, 0, 0, LV_RADIUS_CIRCLE);
    lv_obj_bg_cfg(glass_main, 0x0a000a, 0x200020, LV_GRAD_DIR_HOR, LV_OPA_COVER);
    lv_obj_set_style_clip_corner(glass_main, true, 0); // 关键：开启裁剪，确保反光只在玻璃内显示
    
    // --- 内部光圈 ---
    lv_obj_t* aperture = lv_obj_create(glass_main);
    lv_obj_base_cfg(aperture, 60, 60, LV_ALIGN_CENTER, 0, 0, LV_RADIUS_CIRCLE);
    lv_obj_bg_cfg(aperture, 0x000000, 0xffffff, LV_GRAD_DIR_NONE, LV_OPA_80);
    lv_obj_shadow_cfg(aperture, 40, 0x330011, LV_OPA_COVER);
    lv_obj_set_style_shadow_spread(aperture, 10, 0);

    // --- 镀膜反光效果 ---

    // 洋红色柔光
    lv_obj_t * ref_magenta = lv_obj_create(glass_main);
    lv_obj_base_cfg(ref_magenta, 100, 100, LV_ALIGN_TOP_LEFT, -20, -20, LV_RADIUS_CIRCLE);
    lv_obj_bg_cfg(ref_magenta, 0xaa00aa, 0xffffff, LV_GRAD_DIR_NONE, LV_OPA_30);
    lv_obj_shadow_cfg(ref_magenta, 30, 0xaa00aa, LV_OPA_30);
    
    // 青绿色反光
    lv_obj_t * ref_cyan = lv_obj_create(glass_main);
    lv_obj_base_cfg(ref_cyan, 110, 110, LV_ALIGN_TOP_LEFT, 30, 30, LV_RADIUS_CIRCLE);
    lv_obj_bg_cfg(ref_cyan, 0x00aaaa, 0xffffff, LV_GRAD_DIR_NONE, LV_OPA_20);
    lv_obj_shadow_cfg(ref_cyan, 30, 0x00aaaa, LV_OPA_20);
    
    // 内部高光反光环
    lv_obj_t * ref_glint = lv_obj_create(glass_main);
    lv_obj_base_cfg(ref_glint, 90, 90, LV_ALIGN_CENTER, 0, 0, LV_RADIUS_CIRCLE);
    lv_obj_bg_cfg(ref_glint, 0xffffff, 0xffffff, LV_GRAD_DIR_NONE, LV_OPA_0);
    lv_obj_border_cfg(ref_glint, 2, 0x44ffaa, LV_OPA_40);
    lv_obj_shadow_cfg(ref_glint, 20, 0x00ff88, LV_OPA_20);

    // 顶部高光点
    lv_obj_t * highlight = lv_obj_create(glass_main);
    lv_obj_base_cfg(highlight, 10, 10, LV_ALIGN_TOP_LEFT, 35, 35, 8);
    lv_obj_bg_cfg(highlight, 0xffffff, 0xffffff, LV_GRAD_DIR_NONE, LV_OPA_COVER);
    lv_obj_shadow_cfg(highlight, 15, 0xffffff, LV_OPA_20);
    
    // 玻璃表面的镜面渐变
    lv_obj_t * sheen = lv_obj_create(glass_main);
    lv_obj_base_cfg(sheen, 125, 60, LV_ALIGN_BOTTOM_MID, 0, 0, 0);
    lv_obj_bg_cfg(sheen, 0x000000, 0xffffff, LV_GRAD_DIR_VER, LV_OPA_30);
    lv_obj_set_style_bg_main_stop(sheen, 0, 0);
    lv_obj_set_style_bg_grad_stop(sheen, 255, 0);

    // --- 红色信号灯 ---
    lv_obj_t * dot = lv_obj_create(body);
    lv_obj_base_cfg(dot, 8, 8, LV_ALIGN_TOP_RIGHT, -50, 20, LV_RADIUS_CIRCLE);
    lv_obj_bg_cfg(dot, 0xff0000, 0xffffff, LV_GRAD_DIR_NONE, LV_OPA_80);
    lv_obj_border_cfg(dot, 20, 0xff0000, LV_OPA_50);
    lv_obj_shadow_cfg(dot, 20, 0xff0000, LV_OPA_50);
}

/**
 * @brief 应用入口函数
 * @details 外部调用的唯一接口，负责初始化并显示赛博镜头效果。
 */
void lv_app_entry(void)
{
    cyber_lens_ctreate();
}