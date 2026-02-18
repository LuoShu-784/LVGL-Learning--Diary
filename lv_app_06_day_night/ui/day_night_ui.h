/**
 * @file day_night_ui.h
 * @brief 昼夜切换 UI 逻辑接口
 * @author LuoShu
 * @version 1.0
 * @date 2026-02-18
 */

#ifndef __DAY_NIGHT_UI_H_
#define __DAY_NIGHT_UI_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "lvgl.h"
#include "day_night_ui_conf.h"

/**
 * @brief UI 对象位置配置结构体
 * @details 用于批量定义云朵、陨石坑等元素的位置和大小。
 */
typedef struct {
    int size;   ///< 元素大小
    int x_ofs;  ///< X 轴偏移
    int y_ofs;  ///< Y 轴偏移
} ui_obj_conf_t;


/**
 * @brief 昼夜切换 UI 上下文结构体
 * @details 包含所有关键的 LVGL 对象指针，用于在动画和事件处理中引用。
 */
typedef struct {

    lv_obj_t* background;       ///< 全屏背景
    lv_obj_t* subject;          ///< 主体容器 (Mask容器)
    lv_obj_t* layer_bg;         ///< 背景层 (存放远景云)
    lv_obj_t* layer_mid;        ///< 中间层 (存放圆环光晕、按钮)
    lv_obj_t* layer_fg;         ///< 前景层 (存放近景云、星星)


    lv_obj_t* bg_circles[3];    ///< 背景视差圆环数组
    lv_obj_t* btn;              ///< 太阳/月亮切换按钮
    
    lv_obj_t* clouds[CLOUD_COUNT]; ///< 云朵对象数组
    lv_obj_t* stars[STAR_COUNT];   ///< 星星对象数组
    lv_obj_t* inner_shadow;        ///< 主体容器的内阴影

    bool is_day;                ///< 当前状态标志 (true: 白天, false: 夜晚)
} ui_t;


/**
 * @brief 创建昼夜切换 UI
 * @param parent 父对象 (通常是屏幕)
 * @return 创建成功的 UI 上下文指针
 */
ui_t* ui_create(lv_obj_t* parent);

/**
 * @brief 切换内阴影样式
 * @param ui UI 上下文指针
 * @details 根据当前的 is_day 状态，更新主体容器内阴影的颜色和偏移，增强立体感切换。
 */
void ui_inner_shadow_switch(ui_t* ui);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DAY_NIGHT_UI_H_ */