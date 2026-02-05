/**
 * @file piano.h
 * @brief 钢琴组件头文件
 * @details 定义了钢琴 UI 的基础配置宏以及组件接口等。
 * @author LuoShu
 * @version 1.0
 * @date 2026-02-04
 */

#ifndef __PIANO_H_
#define __PIANO_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include "lvgl/lvgl.h" 

/* ================== 用户配置区 ================== */

#define PIANO_SIZE_W        1424        ///< 钢琴控件的总宽度
#define PIANO_SIZE_H        280         ///< 钢琴控件的总高度

#define PIANO_OCTAVE_START  5           ///< 钢琴起始八度序号（例如 4 代表从 C4 开始）
#define PIANO_OCTAVE_COUNT  2           ///< 钢琴包含的八度数量

#define PIANO_AUDIO_PATH "lv_app_03_piano/audio/"    ///< 音频文件存放路径

#define PIANO_LOG_ENABLE    1                       ///< 是否使能钢琴组件的日志输出 (1: 开启, 0: 关闭)

/* ================== 自动计算区 (勿改) ================== */

#define PIANO_W_KEY_COUNT   (PIANO_OCTAVE_COUNT * 7)                ///< 自动计算的总白键数量

#define PIANO_W_KEY_W       (PIANO_SIZE_W / PIANO_W_KEY_COUNT)      ///< 基础白键宽度
#define PIANO_W_KEY_H       (PIANO_SIZE_H)                          ///< 白键高度

#define PIANO_B_KEY_W       (PIANO_W_KEY_W * 60 / 100)              ///< 黑键宽度（按白键宽度的 60% 计算）
#define PIANO_B_KEY_H       (PIANO_W_KEY_H * 65 / 100)              ///< 黑键高度（按白键高度的 65% 计算）
#define PIANO_B_KEY_OFFSET  (PIANO_W_KEY_W - (PIANO_B_KEY_W / 2))   ///< 黑键相对于白键左侧的偏移量

/* ================== 用户自定义实现接口 ================== */

/**
 * @brief 钢琴应用初始化接口
 * @details 此功能需由用户根据具体硬件平台实现。可用于初始化音频外设、加载音频驱动等。
 * @attention 此函数应在钢琴组件启动阶段（piano_app_entry）之前被调用。
 */
static inline void piano_app_init(void)
{
    // do sth...
}

/**
 * @brief 钢琴音符播放执行接口
 * @param[in] note_name 待播放的音符标识符（例如："C4", "Cs4"）。
 * @details 此功能需由用户自行实现音频触发逻辑。
 */
static inline void piano_play_note_cmd(const char *note_name)
{
    // do sth...
}


/* ================== 钢琴应用接口 ================== */

/**
 * @brief 钢琴应用入口函数
 * @details 负责创建钢琴主容器、初始化样式并生成所有琴键对象。
 */
void piano_app_entry(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PIANO_H_ */