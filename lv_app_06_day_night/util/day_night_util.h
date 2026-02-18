/**
 * @file day_night_util.h
 * @brief 昼夜切换通用工具库接口
 * @author LuoShu
 * @version 1.0
 * @date 2026-02-18
 */

#ifndef __DAY_NIGHT_UTIL_H_
#define __DAY_NIGHT_UTIL_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "lvgl.h"
#include <stdlib.h>

/**
 * @brief 创建一个圆形对象
 * @param parent 父对象
 * @param size 直径 (像素)
 * @param color 填充颜色 (0xRRGGBB)
 * @return 创建的 LVGL 对象指针
 */
lv_obj_t* create_circle(lv_obj_t* parent, int size, uint32_t color);

/**
 * @brief 创建胶囊形主体容器 (用于做遮罩或背景)
 * @param parent 父对象
 * @param w 宽度
 * @param h 高度
 * @return 创建的 LVGL 对象指针
 * @note 该对象默认不响应点击，且不裁剪溢出内容
 */
lv_obj_t* create_subject(lv_obj_t* parent, int w, int h);

/**
 * @brief 创建一个四角星形对象
 * @param parent 父对象
 * @param size 尺寸
 * @param color 颜色
 * @return 创建的 LVGL 对象指针
 * @details 内部使用 DRAW 事件和 mask 技术绘制四角星。
 */
lv_obj_t* create_star(lv_obj_t* parent, int size, uint32_t color);

/**
 * @brief 创建全屏背景层
 * @param parent 父对象
 * @param color 背景颜色
 * @return 创建的 LVGL 对象指针
 */
lv_obj_t* create_background(lv_obj_t* parent, uint32_t color);

/**
 * @brief 初始化 LVGL 动画结构体
 * @param[out] anim 动画对象指针
 * @param[in] var 动画目标变量/对象
 * @param[in] time 动画时长 (ms)
 * @param[in] exec_xcb 动画执行回调函数
 * @param[in] path_cb 动画路径算法 (如 lv_anim_path_linear)
 */
void anim_init(lv_anim_t* anim, void* var, int32_t time, lv_anim_exec_xcb_t exec_xcb, lv_anim_path_cb_t path_cb);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DAY_NIGHT_UTIL_H_ */