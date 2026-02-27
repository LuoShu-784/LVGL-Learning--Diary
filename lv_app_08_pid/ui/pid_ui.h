/**
 * @file pid_ui.h
 * @brief PID 仿真 UI 渲染与控件定义
 * @details 包含 UI 配色方案、布局参数以及 UI 句柄结构体定义。
 * @author LuoShu
 * @version 1.0
 * @date 2026-02-27
 */

#ifndef __PID_UI_H_
#define __PID_UI_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "lvgl.h"
#include "font_conf.h"

#define COLOR_BG            0x222518    ///< 背景色
#define COLOR_TEXT          0xffffff    ///< 主文字颜色
#define COLOR_TEXT_DIM      0xa0a0a0    ///< 辅助/暗色文字颜色

#define PANEL_CTRL_W        lv_pct(100) ///< 控制面板宽度
#define PANEL_CTRL_H        lv_pct(70)  ///< 控制面板高度
#define PANEL_LINE_LEN      200         ///< 分割线长度

#define FLEX_PAD_GAP        150         ///< Flex 布局间距

#define FONT_SIZE_TITLE     30          ///< 标题字体大小
#define FONT_SIZE_TEXT      27          ///< 正文字体大小

#define BALL_SIZE           36          ///< 小球直径
#define BALL_COLOR          0xf26552    ///< 小球颜色
#define BALL_X_OFS          0           ///< 小球 X 偏移
#define BALL_Y_OFS          -27         ///< 小球 Y 偏移
#define BALL_MARK_COLOR     0x52ed52    ///< 小球中心标记颜色
#define BALL_MARK_W         2           ///< 标记宽度
#define BALL_MARK_H         16          ///< 标记高度

#define TACK_X_OFS          0           ///< 轨道 X 偏移
#define TACK_Y_OFS          -45         ///< 轨道 Y 偏移
#define TACK_WIDTH          2           ///< 轨道宽度
#define TACK_COLOR          0x5a5a5a    ///< 轨道颜色
#define TACK_DASH_WIDTH     6           ///< 虚线宽度
#define TACK_DASH_GAP       6           ///< 虚线间距

#define SLIDER_HEIGHT           6       ///< 滑块高度
#define SLIDER_LABEL_WIDTH      120     ///< 滑块标签宽度
#define SLIDER_VALUE_WIDTH      80      ///< 滑块值标签宽度
#define SLIDER_BG_COLOR         0x3a402c ///< 滑块背景色
#define SLIDER_FILLED_COLOR     0x4a5b66 ///< 已填色
#define SLIDER_KNOB_COLOR       0x84a5b8 ///< 旋钮颜色

#define KP_MAX              5000        ///< P 参数最大值 (*10)
#define KI_MAX              5000        ///< I 参数最大值 (*10)
#define KD_MAX              5000        ///< D 参数最大值 (*10)
#define INT_LIM_MAX         5000        ///< 积分限制最大值
#define OUT_LIM_MAX         5000        ///< 输出限制最大值
#define OUT_LIM_DEF_VAL     5000        ///< 默认输出限制
#define OUT_LIM_DEF_STR     "500.0"     ///< 默认输出限制字符串
#define DIST_MAX            5000        ///< 干扰力最大值

/**
 * @enum ui_opt_type_t
 * @brief 鼠标操作模式枚举
 */
typedef enum 
{
    UI_OPT_PUSH,    ///< 推动小球模式
    UI_OPT_MOD      ///< 修改目标位置模式
} ui_opt_type_t;

/**
 * @enum ui_rst_type_t
 * @brief 复位操作类型枚举
 */
typedef enum
{
    UI_RST_BALL,    ///< 重置小球状态
    UI_RST_TARGET,  ///< 重置目标位置
} ui_rst_type_t;

/**
 * @struct ui_t
 * @brief 存储所有 UI 对象的句柄，以便外部管理和更新
 */
typedef struct 
{
    lv_obj_t* opt_push;   ///< “推动小球”按钮
    lv_obj_t* opt_mod;    ///< “修改目标”按钮
    lv_obj_t* rst_ball;   ///< “复位小球”按钮
    lv_obj_t* rst_tgt;    ///< “复位目标”按钮
    lv_obj_t* dist;       ///< 干扰力滑动条
    lv_obj_t* pos_cur;    ///< 当前位置显示标签
    lv_obj_t* pos_tgt;    ///< 目标位置显示标签

    lv_obj_t* kp;         ///< KP 滑动条
    lv_obj_t* ki;         ///< KI 滑动条
    lv_obj_t* kd;         ///< KD 滑动条
    lv_obj_t* int_lim;    ///< 积分限幅滑动条
    lv_obj_t* out_lim;    ///< 输出限幅滑动条

    lv_obj_t* ball;       ///< 小球视觉对象
    lv_obj_t* target;     ///< 目标视觉对象
} ui_t;

/**
 * @brief 初始化整个 PID 应用的 UI
 * @param[in] parent 父对象容器
 * @param[out] ui 填充 UI 句柄的结构体指针
 */
void pid_ui_init(lv_obj_t * parent, ui_t * ui);

/**
 * @brief 为模式切换按钮添加事件回调
 * @param[in] ui UI 句柄
 * @param[in] event_cb 回调函数指针
 */
void pid_ui_add_mode_event(ui_t* ui, lv_event_cb_t event_cb);

/**
 * @brief 为复位按钮添加事件回调
 * @param[in] ui UI 句柄
 * @param[in] event_cb 回调函数指针
 */
void pid_ui_add_reset_event(ui_t* ui, lv_event_cb_t event_cb);

/**
 * @brief 为所有参数滑动条添加值改变事件回调
 * @param[in] ui UI 句柄
 * @param[in] event_cb 回调函数指针
 */
void pid_ui_add_slider_val_event(ui_t* ui, lv_event_cb_t event_cb);

/**
 * @brief 为滑动条的数值标签添加清零点击事件
 * @param[in] ui UI 句柄
 * @param[in] event_cb 回调函数指针
 */
void pid_ui_add_slider_clear_event(ui_t* ui, lv_event_cb_t event_cb);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PID_UI_H_ */