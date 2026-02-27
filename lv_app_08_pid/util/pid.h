/**
 * @file pid.h
 * @brief PID 控制器算法及其参数结构定义
 */

#ifndef __PID_H_
#define __PID_H_

#include "lvgl.h"
#include "math.h"

/**
 * @struct pid_t
 * @brief PID 控制器状态与参数结构体
 */
typedef struct {
    float target;        /**< 设定目标值 */
    float current;       /**< 当前反馈值 */
    float out;           /**< 控制器输出值 */
    
    float error;         /**< 当前误差 */
    float last_error;    /**< 上一次误差（用于微分项） */
    float int_error;     /**< 误差积分累积值 */
    
    float int_error_max; /**< 积分误差限幅值（抗饱和） */
    
    float kp;            /**< 比例系数 */
    float ki;            /**< 积分系数 */
    float kd;            /**< 微分系数 */
    
    float out_max;       /**< 输出上限 */
    float out_min;       /**< 输出下限 */
} pid_t;

/**
 * @brief 初始化 PID 结构体，所有参数清零
 * @param pid 指向 PID 结构体的指针
 */
void pid_init(pid_t *pid);

/**
 * @brief 清除 PID 运行时的中间变量（误差、积分项）
 * @param pid 指向 PID 结构体的指针
 */
void pid_clear(pid_t *pid);

/**
 * @brief 更新 PID 计算结果
 * @details 该函数应在定时器或固定周期任务中调用
 * @param pid 指向 PID 结构体的指针
 */
void pid_update(pid_t *pid);

/**
 * @brief 快速设置 PID 三项系数
 * @param pid 指向 PID 结构体的指针
 * @param kp 比例系数
 * @param ki 积分系数
 * @param kd 微分系数
 */
void pid_set_param(pid_t * pid, float kp, float ki, float kd);

#endif