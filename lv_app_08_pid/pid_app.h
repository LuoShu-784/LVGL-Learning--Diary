/**
 * @file pid_app.h
 * @brief PID 应用程序配置头文件
 * @details 定义了仿真环境所需的各类参数。
 * @author LuoShu
 * @version 1.0
 * @date 2026-02-27
 */

#ifndef __PID_APP_H_
#define __PID_APP_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "lvgl.h"
#include "pid_ui.h"
#include "pid.h"
#include "motion.h"
#include <stdbool.h>

#define FORCE_MUL           50    ///< 拖拽力量缩放系数
#define INT_ERR_LIM_MUL     10    ///< 积分限幅显示值与实际值的转换乘数
#define OUT_LIM_MUL         100   ///< 输出限幅显示值与实际值的转换乘数
#define SIM_TIME            2     ///< 仿真定时器周期 (单位: ms)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PID_APP_H_ */