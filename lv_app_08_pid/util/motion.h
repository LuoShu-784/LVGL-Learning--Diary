/**
 * @file motion.h
 * @brief 物理运动学仿真模型定义
 * @details 模拟受力、质量、阻尼和干扰影响下的一维线性运动。
 * @author LuoShu
 * @version 1.0
 * @date 2026-02-27
 */

#ifndef __MOTION_H_
#define __MOTION_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @struct motion_t
 * @brief 物理运动状态及环境参数
 */
typedef struct 
{
    float position;     ///< 当前位移 (px)
    float velocity;     ///< 当前速度 (px/s)
    float acceleration; ///< 当前加速度 (px/s^2)

    float mass;         ///< 质量 (kg)
    float disturbance;  ///< 环境恒定干扰力
    float damping;      ///< 阻尼系数 (阻力与速度成正比)
} motion_t;

/**
 * @brief 初始化物理模型
 * @param[out] motion 模型结构体指针
 * @param[in] init_pos 初始位置
 * @param[in] mass 质量（需大于0）
 * @param[in] disturbance 初始干扰
 * @param[in] damping 阻尼
 */
void motion_init(motion_t* motion, float init_pos, float mass, float disturbance, float damping);

/**
 * @brief 根据输入力更新运动状态
 * @param[in,out] motion 模型结构体指针
 * @param[in] force_input PID 输出力 + 手动力
 * @param[in] dt 步进时间间隔 (s)
 */
void motion_update(motion_t* motion, float force_input, float dt);

/**
 * @brief 设置环境干扰力
 * @param[in,out] motion 模型结构体指针
 * @param[in] disturbance 干扰力数值
 */
void motion_set_disturbance(motion_t* motion, float disturbance);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MOTION_H_ */