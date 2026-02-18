/**
 * @file day_night_ui.c
 * @brief 昼夜切换 UI 布局与绘制实现。
 * @author LuoShu
 * @version 1.0
 * @date 2026-02-18
 */

#include "lvgl.h"
#include "day_night_ui.h"
#include "day_night_ui_conf.h"
#include "day_night_util.h"

/* --- 静态配置数据 --- */

/** 
 * @brief 云朵位置配置表 
 */
const ui_obj_conf_t cloud_conf[CLOUD_COUNT] = {
    {143,  221, -327}, {171,  179, -241}, {171,   93, -197},
    {171,    7, -211}, {214,  -86, -162}, {171, -165, -169},
    {143,  246, -300}, {171,  203, -200}, {171,  117, -157},
    {171,   32, -171}, {214,  -61, -100}, {171, -140, -140}
};

/** 
 * @brief 星星位置配置表
 */
const ui_obj_conf_t star_conf[STAR_COUNT] = {
    {43, -127, -247}, {43,   22, -167}, {29, -191, -211},
    {29,  -57, -180}, {17,  -37, -232}, {17, -143, -146}
};

/** 
 * @brief 陨石坑位置配置表
 */
const ui_obj_conf_t crater_conf[CRATER_COUNT] = {
    {34,  10, -38},
    {55, -27,   7},
    {34,  31,  31} 
};


/**
 * @brief 创建按钮内部子对象 (太阳/月亮)
 * @param parent 按钮对象
 * @param hl_color 高光颜色
 * @param fg_color 前景颜色
 * @return 创建的对象
 */
static lv_obj_t* btn_child_obj_create(lv_obj_t* parent, uint32_t hl_color, uint32_t fg_color)
{
    lv_obj_t* obj = create_circle(parent, BTN_SIZE, 0xffffff);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
    // 高光
    lv_obj_t* highlight = create_circle(obj, BTN_SIZE, hl_color);
    lv_obj_clear_flag(highlight, LV_OBJ_FLAG_CLICKABLE);

    // 前景遮罩
    lv_obj_t* foreground = create_circle(obj, BTN_SIZE, fg_color);
    lv_obj_align(foreground, LV_ALIGN_CENTER, BTN_FG_X_OFS, BTN_FG_Y_OFS);
    lv_obj_set_style_shadow_width(foreground,  BTN_FG_SHD_W, 0);
    lv_obj_set_style_shadow_color(foreground, lv_color_hex(fg_color), 0);
    lv_obj_set_style_shadow_spread(foreground, BTN_FG_SHD_SPREAD, 0);
    lv_obj_clear_flag(foreground, LV_OBJ_FLAG_CLICKABLE);

    // 内阴影
    lv_obj_t* inner_shadow = create_circle(obj, BTN_SIZE, 0xffffff);
    lv_obj_set_style_bg_opa(inner_shadow, LV_OPA_TRANSP, 0);
    lv_obj_align(inner_shadow, LV_ALIGN_CENTER, BTN_IN_SHD_X_OFS, BTN_IN_SHD_Y_OFS);
    lv_obj_set_style_shadow_width(inner_shadow, BTN_IN_SHD_W, 0);
    lv_obj_set_style_shadow_opa(inner_shadow, BTN_IN_SHD_OPA, 0);
    lv_obj_set_style_shadow_spread(inner_shadow, BTN_IN_SHD_SPREAD, 0);
    lv_obj_clear_flag(inner_shadow, LV_OBJ_FLAG_CLICKABLE);
    
    return obj;
}

/**
 * @brief 创建切换按钮主体
 * @details 包含太阳和月亮两个子对象。其中月亮包含陨石坑，且初始状态设为透明。
 */
static lv_obj_t* btn_create(lv_obj_t* parent)
{
    lv_obj_t* btn = create_circle(parent, BTN_SIZE, 0xffffff);
    lv_obj_set_style_shadow_width(btn, BTN_SHD_WIDTH, 0);
    lv_obj_set_style_shadow_ofs_x(btn, BTN_SHD_X_OFS, 0);
    lv_obj_set_style_shadow_ofs_y(btn, BTN_SHD_Y_OFS, 0);
    lv_obj_set_style_shadow_spread(btn, BTN_SHD_SPREAD, 0);
    lv_obj_set_style_shadow_opa(btn, BTN_SHD_OPA, 0);
    lv_obj_set_style_clip_corner(btn, true, 0);
    lv_obj_align(btn, LV_ALIGN_CENTER, BTN_X_OFS, BTN_Y_OFS);
    
    lv_obj_t* sun = btn_child_obj_create(btn, SUN_HL_COLOR, SUN_FG_COLOR);
    lv_obj_t* moon = btn_child_obj_create(btn, MOON_HL_COLOR, MOON_FG_COLOR);

    // 在月亮上添加陨石坑
    for(int i = 0; i < CRATER_COUNT; i++) 
    {
        int size = crater_conf[i].size;
        int x_ofs = crater_conf[i].x_ofs;
        int y_ofs = crater_conf[i].y_ofs;
        lv_obj_t * crater = create_circle(moon, size, CRATER_BG_COLOR);
        lv_obj_align(crater, LV_ALIGN_CENTER, x_ofs, y_ofs);
        lv_obj_set_style_border_width(crater, 2, 0);
        lv_obj_set_style_border_color(crater, lv_color_hex(CRATER_BD_COLOR), 0);
        lv_obj_set_style_border_opa(crater, LV_OPA_COVER, 0);
        lv_obj_clear_flag(crater, LV_OBJ_FLAG_CLICKABLE);
    }
    lv_obj_set_style_opa(moon, LV_OPA_TRANSP, 0); // 默认隐藏月亮
    return btn;

}

/**
 * @brief 批量创建云朵
 * @param is_bg true表示背景层云朵，false表示前景层
 */
static void clouds_create(lv_obj_t* parent, bool is_bg, lv_obj_t** clouds) 
{
    uint32_t color;
    int begin, end;
    if (is_bg)
    {
        color = CLOUD_BG_COLOR;
        begin = 0;
        end = CLOUD_COUNT / 2;
    }
    else
    {
        color = CLOUD_FG_COLOR;
        begin = CLOUD_COUNT / 2;
        end = CLOUD_COUNT;
    }
    for (int i = begin; i < end; i++) 
    {
        int size = cloud_conf[i].size;
        int x_ofs = cloud_conf[i].x_ofs;
        int y_ofs = cloud_conf[i].y_ofs;
        lv_obj_t* cloud = create_circle(parent, size, color);
        lv_obj_align(cloud, LV_ALIGN_BOTTOM_MID, x_ofs, y_ofs);
        lv_obj_clear_flag(cloud, LV_OBJ_FLAG_CLICKABLE);
        clouds[i] = cloud;
    }
}

/**
 * @brief 批量创建星星
 */
static void stars_create(lv_obj_t* parent, lv_obj_t** stars)
{
    for (int i = 0; i < STAR_COUNT; i++) 
    {
        int x_ofs = star_conf[i].x_ofs;
        int y_ofs = star_conf[i].y_ofs - SUBJ_H;
        lv_obj_t* star = create_star(parent, star_conf[i].size, STAR_COLOR);
        lv_obj_align(star, LV_ALIGN_CENTER, x_ofs, y_ofs);
        stars[i] = star;
    }
}

/**
 * @brief 创建背景视差圆环
 */
static void bg_circles_create(lv_obj_t* parent, lv_obj_t** bg_circles)
{
    lv_opa_t opa = SUBJ_BG_OPA;
    int bg_ofs, bg_ofs_inc;
    bg_ofs = SUBJ_DAY_BG_X_OFS;
    bg_ofs_inc = SUBJ_DAY_BG_X_OFS_INC;

    for (int i = 0; i < 3; i++)
    {
        int size = SUBJ_BG_SIZE + i * SUBJ_BG_SIZE_INC;
        
        int x_ofs = bg_ofs + i * bg_ofs_inc; 
        
        lv_obj_t* bg_circle = create_circle(parent, size, 0xffffff);
        lv_obj_align(bg_circle, LV_ALIGN_CENTER, x_ofs, 0);
        lv_obj_set_style_opa(bg_circle, opa, 0);
        opa /= 2;
        bg_circles[i] = bg_circle;
    }
}

/**
 * @brief 创建主体容器的内阴影层
 */
static lv_obj_t* inner_shadow_create(lv_obj_t* parent)
{
    lv_obj_t* inner_shadow = lv_obj_create(parent);
    lv_obj_remove_style_all(inner_shadow);
    lv_obj_center(inner_shadow);
    lv_obj_set_size(inner_shadow, SUBJ_IN_SHD_W, SUBJ_IN_SHD_H);
    lv_obj_set_style_radius(inner_shadow, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_shadow_width(inner_shadow, SUBJ_IN_SHD_WIDTH, 0);
    lv_obj_set_style_shadow_ofs_x(inner_shadow, SUBJ_DAY_IN_SHD_X_OFS, 0);
    lv_obj_set_style_shadow_ofs_y(inner_shadow, SUBJ_DAY_IN_SHD_Y_OFS, 0);
    lv_obj_set_style_shadow_color(inner_shadow, lv_color_hex(SUBJ_DAY_IN_SHD_COLOR), 0);
    lv_obj_set_style_shadow_opa(inner_shadow, SUBJ_IN_SHD_OPA, 0);
    lv_obj_clear_flag(inner_shadow, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);

    return inner_shadow;
}

/**
 * @brief 创建分层结构 (背景/中景/前景)
 */
static void layers_create(ui_t* ui)
{
    lv_obj_t* subject = ui->subject;
    ui->layer_bg = create_subject(subject, SUBJ_W, SUBJ_H * 4);
    ui->layer_mid = create_subject(subject, SUBJ_W * 2, SUBJ_H);
    ui->layer_fg = create_subject(subject, SUBJ_W, SUBJ_H * 4);

    lv_obj_add_flag(ui->subject, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(ui->layer_fg, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_flag(ui->layer_mid, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_EVENT_BUBBLE);

    lv_obj_add_flag(ui->layer_bg, LV_OBJ_FLAG_OVERFLOW_VISIBLE); 
    lv_obj_add_flag(ui->layer_mid, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
    lv_obj_add_flag(ui->layer_fg, LV_OBJ_FLAG_OVERFLOW_VISIBLE);

    lv_obj_set_style_bg_opa(ui->layer_bg, LV_OPA_TRANSP, 0);
    lv_obj_set_style_bg_opa(ui->layer_mid, LV_OPA_TRANSP, 0);
    lv_obj_set_style_bg_opa(ui->layer_fg, LV_OPA_TRANSP, 0);
}

/**
 * @brief 创建昼夜背景块
 * @details 垂直堆叠三个块：白天背景、渐变过渡背景、夜晚背景。用于纵向滚动切换。
 */
static void day_night_bg_create(lv_obj_t* parent)
{
    lv_obj_t* day_bg = create_subject(parent, SUBJ_W, SUBJ_H + 20);
    lv_obj_t* night_bg = create_subject(parent, SUBJ_W, SUBJ_H + 20);

    lv_obj_set_style_bg_color(day_bg, lv_color_hex(SUBJ_DAY_BG_COLOR), 0);
    lv_obj_set_style_bg_color(night_bg, lv_color_hex(SUBJ_NIGHT_BG_COLOR), 0);
    lv_obj_set_style_radius(day_bg, 0, 0);
    lv_obj_set_style_radius(night_bg, 0, 0);
    lv_obj_set_y(night_bg, -SUBJ_H * 2);

    lv_obj_t* trans_bg = create_subject(parent, SUBJ_W, SUBJ_H);
    lv_obj_set_y(trans_bg, - SUBJ_H);
    lv_obj_set_style_radius(trans_bg, 0, 0);
    lv_obj_set_style_bg_color(trans_bg, lv_color_hex(SUBJ_NIGHT_BG_COLOR), 0);
    lv_obj_set_style_bg_grad_color(trans_bg, lv_color_hex(SUBJ_DAY_BG_COLOR), 0);
    lv_obj_set_style_bg_grad_dir(trans_bg, LV_GRAD_DIR_VER, 0);
    lv_obj_set_style_bg_dither_mode(trans_bg, LV_DITHER_ORDERED, 0);

}

/**
 * @brief 切换内阴影样式
 */
void ui_inner_shadow_switch(ui_t* ui)
{
    lv_obj_t* shadow = ui->inner_shadow;
    bool is_day = ui->is_day;

    int x_ofs, y_ofs;
    lv_color_t color;
    if (!is_day)
    {
        x_ofs = SUBJ_DAY_IN_SHD_X_OFS;
        y_ofs = SUBJ_DAY_IN_SHD_Y_OFS;
        color = lv_color_hex(SUBJ_DAY_IN_SHD_COLOR);
    }
    else
    {
        x_ofs = SUBJ_NIGHT_IN_SHD_X_OFS;
        y_ofs = SUBJ_NIGHT_IN_SHD_Y_OFS;
        color = lv_color_hex(SUBJ_NIGHT_IN_SHD_COLOR);
    }
    lv_obj_set_style_shadow_ofs_x(shadow, x_ofs, 0);
    lv_obj_set_style_shadow_ofs_y(shadow, y_ofs, 0);
    lv_obj_set_style_shadow_color(shadow, color, 0);
}

/**
 * @brief UI 创建入口
 */
ui_t* ui_create(lv_obj_t* parent)
{
    ui_t* ui = malloc(sizeof(ui_t));
    if (parent == NULL || ui == NULL) return NULL;

    lv_obj_t* background = create_background(parent, BG_NIGHT_COLOR);
    ui->background = background;

    ui->subject = create_subject(background, SUBJ_W, SUBJ_H);
    lv_obj_set_style_clip_corner(ui->subject, true, 0);
    layers_create(ui);

    day_night_bg_create(ui->layer_bg);

    bg_circles_create(ui->layer_mid, ui->bg_circles);
    
    ui->btn = btn_create(ui->layer_mid);

    clouds_create(ui->layer_bg, true, ui->clouds);
    clouds_create(ui->layer_fg, false, ui->clouds);

    stars_create(ui->layer_fg, ui->stars);

    ui->inner_shadow = inner_shadow_create(ui->layer_mid);

    ui->is_day = true;

    return ui;

}