/**
 * @file font_manager.c
 * @brief 动态字体管理器实现
 * @details 维护两个核心链表：
 * 1. 注册表 (Registry): 存储 ID 到文件路径的映射。
 * 2. 实例缓存 (Cache): 存储已生成的 (ID + Size) -> lv_font_t 实例。
 * @author LuoShu
 * @version 1.0
 * @date 2026-02-04
 */

#include "font_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief 字体注册信息节点
 * @details 存储在 s_reg_list 中
 */
typedef struct {
    font_id_t id;   ///< 用户定义的字体 ID
    char *path;     ///< 字体文件的物理路径 (深拷贝存储)
} font_reg_info_t;

/**
 * @brief 字体实例缓存节点
 * @details 存储在 s_instance_list 中
 */
typedef struct {
    font_id_t id;          ///< 所属字体 ID
    uint16_t size;         ///< 字体大小
    lv_ft_info_t *ft_info; ///< FreeType 及其字体句柄 (包含 lv_font_t)
} font_instance_t;


static bool s_is_init = false;      ///< 模块初始化标志
static lv_ll_t s_reg_list;          ///< 注册列表 (Registry)
static lv_ll_t s_instance_list;     ///< 实例列表 (Cache)

/**
 * @brief 内部初始化检查
 * @details 懒汉式初始化 LVGL 链表。
 */
static void _check_init(void)
{
    if (s_is_init == false) 
    {
        _lv_ll_init(&s_reg_list, sizeof(font_reg_info_t));
        _lv_ll_init(&s_instance_list, sizeof(font_instance_t));
        s_is_init = true;
    }
}

/**
 * @brief 在注册列表中查找字体信息
 * @param[in] id 待查找的 ID
 * @return 注册节点指针，未找到返回 NULL
 */
static font_reg_info_t *_find_in_registry(font_id_t id)
{
    font_reg_info_t *node = _lv_ll_get_head(&s_reg_list);
    while (node) 
    {
        if (node->id == id) {
            return node;
        }
        node = _lv_ll_get_next(&s_reg_list, node);
    }
    return NULL;
}

/**
 * @brief 在缓存列表中查找已生成的实例
 * @param[in] id 字体 ID
 * @param[in] size 字体大小
 * @return 实例节点指针，未找到返回 NULL
 */
static font_instance_t *_find_in_cache(font_id_t id, uint16_t size)
{
    font_instance_t *node = _lv_ll_get_head(&s_instance_list);
    while (node) 
    {
        if (node->id == id && node->size == size) {
            return node;
        }
        node = _lv_ll_get_next(&s_instance_list, node);
    }
    return NULL;
}

/**
 * @brief 核心加载逻辑：调用 LVGL FreeType 接口创建新字体
 * @param[in] reg_info 注册信息节点
 * @param[in] size 请求的字体大小
 * @return 新生成的 lv_font_t 指针，失败返回 NULL
 */
static lv_font_t *_create_new_font(font_reg_info_t *reg_info, uint16_t size)
{
    // 申请 FreeType 配置结构体
    lv_ft_info_t *info = (lv_ft_info_t *)malloc(sizeof(lv_ft_info_t));
    if (info == NULL) {
        LV_LOG_ERROR("font_manager: malloc failed for ft_info");
        return NULL;
    }

    // 填充参数
    info->name = reg_info->path;
    info->weight = size;
    info->style = FT_FONT_STYLE_NORMAL;
    info->mem = NULL;
    info->mem_size = 0;

    // 初始化 FreeType
    if (!lv_ft_font_init(info)) 
    {
        LV_LOG_ERROR("font_manager: freetype init failed for %s", reg_info->path);
        free(info); // 失败回滚
        return NULL;
    }

    // 记录到缓存链表，防止下次重复加载
    font_instance_t *new_node = _lv_ll_ins_tail(&s_instance_list);
    new_node->id = reg_info->id;
    new_node->size = size;
    new_node->ft_info = info;

    return info->font;
}

/* --- 公共接口 --- */

void font_manager_register(font_id_t id, const char *path)
{
    _check_init();

    // 防止重复注册同一个 ID
    if (_find_in_registry(id) != NULL) {
        LV_LOG_WARN("font_manager: ID %d already registered", id);
        return;
    }

    font_reg_info_t *new_node = _lv_ll_ins_tail(&s_reg_list);
    
    new_node->id = id;

    // 手动实现字符串深拷贝
    size_t len = strlen(path) + 1;
    new_node->path = (char *)malloc(len);
    if (new_node->path != NULL) {
        strcpy(new_node->path, path);
    } else {
        LV_LOG_ERROR("font_manager: malloc failed for path");
    }
}

lv_font_t *font_manager_acquire(font_id_t id, uint16_t size)
{
    _check_init();

    // 先查缓存，如果之前加载过一样大小的，直接返回
    font_instance_t *cached = _find_in_cache(id, size);
    if (cached != NULL) {
        return cached->ft_info->font;
    }

    // 查注册表，找到对应的文件路径
    font_reg_info_t *reg_info = _find_in_registry(id);
    if (reg_info == NULL) {
        LV_LOG_ERROR("font_manager: ID %d not found", id);
        return NULL;
    }

    // 没缓存过，创建新的
    return _create_new_font(reg_info, size);
}

void font_manager_destroy(void)
{
    if (!s_is_init) return;

    // 清理所有缓存的字体实例
    font_instance_t *inst_node = _lv_ll_get_head(&s_instance_list);
    while (inst_node) 
    {
        if (inst_node->ft_info) {
            // 这里仅仅释放结构体内存，lv_ft 内部资源的释放依赖 LVGL 的退出机制
            free(inst_node->ft_info);
        }
        inst_node = _lv_ll_get_next(&s_instance_list, inst_node);
    }
    _lv_ll_clear(&s_instance_list);

    // 清理注册信息
    font_reg_info_t *reg_node = _lv_ll_get_head(&s_reg_list);
    while (reg_node) 
    {
        if (reg_node->path) {
            free(reg_node->path); // 释放我们手动 malloc 的路径字符串
        }
        reg_node = _lv_ll_get_next(&s_reg_list, reg_node);
    }
    _lv_ll_clear(&s_reg_list);

    s_is_init = false;
}