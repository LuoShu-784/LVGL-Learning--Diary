/**
 * @file audio_conf.h
 * @brief 音频配置与播放接口抽象层
 * @details 本文件提供了音频模块初始化和播放音频文件的基础接口。
 * @attention 必须修改此文件中的实现，适配您的音频驱动
 * @author LuoShu
 * @version 1.0
 * @date 2026-02-07
 */

#ifndef __AUDIO_CONF_H_
#define __AUDIO_CONF_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stdlib.h"
#include "stdio.h"

/** @brief 音频资源文件的默认存放目录 
 *  @note 确保目标文件系统中存在此路径，且存有对应的 .wav 文件
 */
#define AUDIO_PATH  "./build/app_switch/audio/"

/**
 * @brief 初始化音频模块
 * @note 请替换为具体初始化代码。
 */
static inline void audio_init(void)
{

}

/**
 * @brief 播放指定的音频文件
 * @param[in] url 音频文件名（不含路径和后缀，例如 "audio_file"）
 */
static inline void audio_play(const char* url)
{
    char cmd[64];
    // 构建命令：aplay -q /path/filename.wav &
    snprintf(cmd, sizeof(cmd), "aplay -q %s%s.wav &", AUDIO_PATH, url); 
    system(cmd);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __AUDIO_CONF_H_ */