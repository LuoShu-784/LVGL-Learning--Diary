/**
 * @file res_conf.h
 * @brief 全局资源路径配置
 * @details 配置应用程序所需的多媒体资源（图片、音频、视频等）的路径。
 * @author LuoShu
 * @version 1.0
 * @date 2026-02-21
 */

#ifndef __RES_CONF_H_
#define __RES_CONF_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "lvgl.h"
#include "font/font_conf.h"

/* ================== 环境路径配置 ================== */
#define IMAGE_PATH  "./lv_app_07_HCNY/res/"               ///< 图片资源路径
#define VEDIO_PATH  "./lv_app_07_HCNY/res/"               ///< 视频资源路径

/* ================== LVGL 虚拟文件系统配置 ================== */

/** 
 * @brief LVGL 注册的驱动器盘符前缀 (例如 A:) 
 */
#define DRIVER_LETTER  "A:"

/**
 * @brief 获取 LVGL 可识别的完整图片路径
 * @param name 图片文件名
 * @return 拼接了盘符的绝对路径字符串
 */
#define GET_IMAGE_PATH(name) (DRIVER_LETTER IMAGE_PATH name)

/* ================== 具体资源文件名 ================== */

#define VEDIO_NAME "gongxinifacai.mp4"  ///< 新年视频文件名
#define IMAGE_NAME "gongxinifacai.png"  ///< 恭喜发财图片文件名

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RES_CONF_H_ */