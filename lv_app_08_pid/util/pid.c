/**
 * @file pid.c
 * @brief PID 控制算法实现
 * @author LuoShu
 * @date 2026-02-27
 */

#include "pid.h"

/**
 * @brief 初始化 PID 控制器参数
 */
void pid_init(pid_t *pid)
{
    pid_clear(pid);
    pid->kp = 0.0f;
    pid->ki = 0.0f;
    pid->kd = 0.0f;
    pid->int_error_max = 0.0f;
    pid->out_max = 0.0f;
    pid->out_min = 0.0f;
}

/**
 * @brief 清除 PID 运行时的中间变量 (误差及积分项)
 */
void pid_clear(pid_t *pid)
{
	pid->error = 0;
	pid->last_error = 0;
	pid->int_error = 0;
}

/**
 * @brief 设置 PID 三项增益参数
 */
inline void pid_set_param(pid_t * pid, float kp, float ki, float kd)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
}

/**
 * @brief 计算 PID 输出
 */
void pid_update(pid_t *pid)
{
    pid->last_error = pid->error;
    float last_target = pid->target - pid->last_error; 
    
    pid->error = pid->target - pid->current;
    
    if (pid->ki != 0)
    {
		pid->int_error += pid->error;

        if (pid->int_error_max > 0) {
            float i_max_limit = pid->int_error_max / pid->ki; 
            if (pid->int_error > i_max_limit) {
                pid->int_error = i_max_limit;
            } else if (pid->int_error < -i_max_limit) {
                pid->int_error = -i_max_limit;
            }
        }
    }
    else
    {
        pid->int_error = 0;
    }
    
    
    pid->out = (pid->kp * pid->error)
             + (pid->ki * pid->int_error)
             + (pid->kd * (pid->error - pid->last_error));
    
    if (pid->out_max > 0 || pid->out_min < 0) {
        if (pid->out > pid->out_max) { pid->out = pid->out_max; }
        if (pid->out < pid->out_min) { pid->out = pid->out_min; }
    }
}