/**
 * @file motion.c
 * @brief 物理运动学仿真模型实现
 * @author LuoShu
 * @version 1.0
 * @date 2026-02-27
 */

#include "motion.h"

/**
 * @brief 初始化物理模型
 */
void motion_init(motion_t* motion, float init_pos, float mass, float disturbance, float damping)
{
    motion->position = init_pos;
    motion->velocity = 0.0f;
    motion->acceleration = 0.0f;
    
    motion->mass = (mass <= 0.001f) ? 1.0f : mass; 
    motion->disturbance = disturbance;
    motion->damping = damping;
}

/**
 * @brief 根据输入力更新运动状态
 */
void motion_update(motion_t* motion, float force_input, float dt) 
{
    float damping_force = -(motion->damping * motion->velocity);
    float resultant = force_input + motion->disturbance + damping_force;
    
    motion->acceleration = resultant / motion->mass;
    
    motion->velocity += motion->acceleration * dt;
    motion->position += motion->velocity * dt;
}

/**
 * @brief 设置环境干扰力
 */
inline void motion_set_disturbance(motion_t* motion, float disturbance) 
{
    motion->disturbance = disturbance;
}