/**
 * @file font_manager.h
 * @brief 动态字体管理器接口
 * @details 该模块提供基于 FreeType 的运行时字体加载方案。
 * @author LuoShu
 * @version 1.0
 * @date 2026-02-04
 */

#ifndef _FONT_MANAGER_H_
#define _FONT_MANAGER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

/**
 * @brief 自定义字体 ID 类型
 * @note 建议在 font_conf.h 或用户头文件中使用 enum 定义具体的 ID 值。
 */
typedef int font_id_t;

/**
 * @brief 注册字体信息
 * @param[in] id 用户定义的字体 ID (需保证唯一性)
 * @param[in] path 字体文件系统路径 (如 "S:/fonts/arial.ttf")
 * @details 仅保存字体路径与 ID 的映射关系，此时不会加载具体的字体文件，不消耗显存。
 */
void font_manager_register(font_id_t id, const char *path);

/**
 * @brief 获取指定大小的字体实例
 * @param[in] id 已注册的字体 ID
 * @param[in] size 字体高度 (像素/磅数，具体取决于 FreeType 配置)
 * @return 指向 LVGL 字体对象的指针 (lv_font_t *)
 */
lv_font_t *font_manager_acquire(font_id_t id, uint16_t size);

/**
 * @brief 销毁字体管理器
 * @details 释放所有缓存的字体实例 (FreeType 句柄) 以及注册表内存。
 * @warning 调用此函数后，所有由该管理器创建的 lv_font_t 指针将失效。
 */
void font_manager_destroy(void);

#ifdef __cplusplus
}
#endif

#endif // _FONT_MANAGER_H_