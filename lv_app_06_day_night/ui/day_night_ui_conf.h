/**
 * @file day_night_ui_conf.h
 * @brief 昼夜切换按钮 UI 配置文件
 * @author LuoShu
 * @version 1.0
 * @date 2026-02-18
 */

#ifndef __DAY_NIGHT_UI_CONF_H_
#define __DAY_NIGHT_UI_CONF_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "lvgl.h"

/* --- 整体背景色 --- */
#define BG_NIGHT_COLOR     0x424242

/* --- 按钮主体配置 --- */
#define BTN_SIZE            150     ///< 按钮直径
#define BTN_X_OFS           (-150)  ///< 按钮初始 X 偏移
#define BTN_Y_OFS           0       ///< 按钮初始 Y 偏移

/* --- 按钮阴影与高光配置 --- */
#define BTN_SHD_WIDTH       20
#define BTN_SHD_X_OFS       7
#define BTN_SHD_Y_OFS       5
#define BTN_SHD_OPA         LV_OPA_60
#define BTN_SHD_SPREAD      3

/* --- 按钮内部元素公共偏移 --- */
#define BTN_FG_X_OFS        10
#define BTN_FG_Y_OFS        10
#define BTN_FG_SHD_W        20
#define BTN_FG_SHD_SPREAD   7

/* --- 按钮内阴影 (营造凹陷感) --- */
#define BTN_IN_SHD_X_OFS    -5
#define BTN_IN_SHD_Y_OFS    -3
#define BTN_IN_SHD_W        50
#define BTN_IN_SHD_OPA      LV_OPA_50
#define BTN_IN_SHD_SPREAD   7

/* --- 太阳样式 --- */
#define SUN_HL_COLOR        0xffe971 ///< 太阳高光色
#define SUN_FG_COLOR        0xffc323 ///< 太阳主体色

/* --- 月亮样式 --- */
#define MOON_HL_COLOR       0xfefed2 ///< 月亮高光色
#define MOON_FG_COLOR       0xc3c8d2 ///< 月亮主体色

/* --- 月球表面陨石坑配置 --- */
#define CRATER_COUNT        3        ///< 月球表面陨石坑数量
#define CRATER_BG_COLOR     0x96a0b4
#define CRATER_BD_COLOR     0x6d7483

/* --- 主体容器配置 --- */
#define SUBJ_W              520      ///< 切换开关总宽度
#define SUBJ_H              200      ///< 切换开关总高度

#define SUBJ_DAY_BG_COLOR       0x4685c0 ///< 白天天空背景色
#define SUBJ_NIGHT_BG_COLOR     0x191e32 ///< 夜晚天空背景色

/* --- 主体内阴影 --- */
#define SUBJ_IN_SHD_W               (SUBJ_W - 10)
#define SUBJ_IN_SHD_H               (SUBJ_H - 10)
#define SUBJ_IN_SHD_WIDTH           20
#define SUBJ_IN_SHD_OPA             LV_OPA_50

// 白天模式下的内阴影参数
#define SUBJ_DAY_IN_SHD_COLOR       0x244563
#define SUBJ_DAY_IN_SHD_X_OFS       40
#define SUBJ_DAY_IN_SHD_Y_OFS       -60

// 夜晚模式下的内阴影参数
#define SUBJ_NIGHT_IN_SHD_COLOR     0x000000
#define SUBJ_NIGHT_IN_SHD_X_OFS     -400
#define SUBJ_NIGHT_IN_SHD_Y_OFS     0

/* --- 背景光晕圆环 --- */
#define SUBJ_BG_SIZE                300
#define SUBJ_BG_SIZE_INC            70      ///< 每一层圆环增加的尺寸
#define SUBJ_DAY_BG_X_OFS           -160
#define SUBJ_DAY_BG_X_OFS_INC       30
#define SUBJ_NIGHT_BG_X_OFS         160
#define SUBJ_NIGHT_BG_X_OFS_INC     -30
#define SUBJ_BG_OPA                 LV_OPA_20
#define SUBJ_BG_COUNT               3

/* --- 云朵配置 --- */
#define CLOUD_BG_COLOR      0xa3c2e0
#define CLOUD_FG_COLOR      0xffffff
#define CLOUD_COUNT         12

/* --- 星星配置 --- */
#define STAR_COUNT      6
#define STAR_COLOR      0xffffff

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DAY_NIGHT_UI_CONF_H_ */