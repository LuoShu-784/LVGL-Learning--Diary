/**
 * @file font_conf.h
 * @brief 字体应用配置
 * @details 定义具体的字体 ID 枚举、文件路径以及预初始化逻辑。
 * @note 用户应根据实际项目需求修改此文件中的路径和枚举。
 * @author LuoShu
 * @version 1.0
 * @date 2026-02-04
 */

#ifndef __FONT_CONF_H_
#define __FONT_CONF_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "font_manager.h"

/**
 * @brief 字体文件存放的根目录
 * @note 需要用户自己配置路径。
 */
#define FONT_PATH "./build/app_pid/font/"

/**
 * @brief 应用字体 ID 枚举
 * @details 用于 font_manager_acquire 时的唯一标识
 */
typedef enum 
{
    LV_FONT_TYPE_EN = 0,    ///< 英文字体
    LV_FONT_TYPE_CN         ///< 中文字体
} lv_font_type_t;

/** 
 * @brief 英文字体文件 
 */
#define LV_FONT_TYPE_EN_PATH FONT_PATH "FiraCode-Regular-1.ttf"

/** 
 * @brief 英文字体文件 
 */
#define LV_FONT_TYPE_CN_PATH FONT_PATH "也字工厂小石头.ttf"

/**
 * @brief 字体模块预初始化
 * @details 注册所有项目中需要的字体 ID 与路径的对应关系。
 * @note lvgl 初始化后调用。
 */
static inline void lv_font_init(void)
{
    font_manager_register(LV_FONT_TYPE_EN, LV_FONT_TYPE_EN_PATH);
    font_manager_register(LV_FONT_TYPE_CN, LV_FONT_TYPE_CN_PATH); 
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __FONT_CONF_H_ */