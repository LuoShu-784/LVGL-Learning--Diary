/**
 * @file pid_app.c
 * @brief PID 仿真应用程序核心逻辑实现
 * @details 负责协调 UI 交互、PID 计算引擎以及物理运动模型的更新同步。
 * @author LuoShu
 * @version 1.0
 * @date 2026-02-27
 */

#include "pid_app.h"

static ui_t pid_ui;               ///< UI 控件句柄结构体
static pid_t sim_pid;             ///< PID 控制器状态结构体
static motion_t motion;           ///< 物理运动模型结构体

static float target_pos = 0.0f;   ///< 设定目标位置坐标
static float manual_force = 0.0f; ///< 外部手动施加的干扰力

static ui_opt_type_t current_mode = UI_OPT_PUSH; ///< 当前鼠标交互模式

static void mode_event_cb(lv_event_t* e);
static void reset_event_cb(lv_event_t* e);
static void slider_val_event_cb(lv_event_t* e);
static void slider_clear_event_cb(lv_event_t* e);
static void drag_event_cb(lv_event_t* e);
static void sim_timer_cb(lv_timer_t* timer);

/**
 * @brief PID 应用程序入口函数
 * @details 初始化 UI 布局、PID 参数、物理模型，并注册所有交互事件及仿真定时器。
 */
void pid_app_entry(void)
{
    lv_obj_t* scr = lv_scr_act();

    pid_ui_init(scr, &pid_ui);
    pid_init(&sim_pid);
    motion_init(&motion, 0, 0.005f, 0.0f, 0.000f);

    pid_ui_add_mode_event(&pid_ui, mode_event_cb);
    pid_ui_add_reset_event(&pid_ui, reset_event_cb);
    pid_ui_add_slider_val_event(&pid_ui, slider_val_event_cb);
    pid_ui_add_slider_clear_event(&pid_ui, slider_clear_event_cb);
    
    lv_obj_add_event_cb(scr, drag_event_cb, LV_EVENT_ALL, NULL);
    lv_timer_create(sim_timer_cb, SIM_TIME, NULL);
}

/**
 * @brief 模式切换事件回调
 * @param[in] e LVGL 事件指针，包含触发模式类型
 */
static void mode_event_cb(lv_event_t* e)
{
    ui_opt_type_t opt = (ui_opt_type_t)lv_event_get_user_data(e);
    current_mode = opt;

    lv_obj_set_style_text_color(pid_ui.opt_push, 
        current_mode == UI_OPT_PUSH ? lv_color_hex(COLOR_TEXT) : lv_color_hex(COLOR_TEXT_DIM), 0);
    lv_obj_set_style_text_color(pid_ui.opt_mod, 
        current_mode == UI_OPT_MOD ? lv_color_hex(COLOR_TEXT) : lv_color_hex(COLOR_TEXT_DIM), 0);
}

/**
 * @brief 系统重置事件回调
 * @param[in] e LVGL 事件指针，包含重置对象类型 (小球或目标)
 */
static void reset_event_cb(lv_event_t * e)
{
    ui_rst_type_t rst = (ui_rst_type_t)lv_event_get_user_data(e);
    if (rst == UI_RST_BALL) {
        motion.position = 0.0f;
        motion.velocity = 0.0f;
        motion.acceleration = 0.0f;
        manual_force = 0.0f;
        pid_clear(&sim_pid);
    } else if (rst == UI_RST_TARGET) {
        target_pos = 0.0f;
    }
}

/**
 * @brief 滑动条数值变更回调
 * @param[in] e LVGL 事件指针，映射对应的 PID 参数或物理干扰
 */
static void slider_val_event_cb(lv_event_t* e)
{
    lv_obj_t * slider = lv_event_get_target(e);
    lv_obj_t* val_label = (lv_obj_t*)lv_obj_get_user_data(slider);

    int val_int = lv_slider_get_value(slider);
    if (val_int < 0 && (val_int / 10 == 0)) {
        lv_label_set_text_fmt(val_label, "-0.%d", LV_ABS(val_int % 10));
    } else {
        lv_label_set_text_fmt(val_label, "%d.%d", val_int / 10, LV_ABS(val_int % 10));
    }
    float val_f = val_int / 10.0f;

    if (slider == pid_ui.kp)              sim_pid.kp = val_f;
    else if (slider == pid_ui.ki)         sim_pid.ki = val_f;
    else if (slider == pid_ui.kd)         sim_pid.kd = val_f;
    else if (slider == pid_ui.int_lim)    sim_pid.int_error_max = val_f * INT_ERR_LIM_MUL;
    else if (slider == pid_ui.out_lim) {
        sim_pid.out_max = val_f * OUT_LIM_MUL;
        sim_pid.out_min = -val_f * OUT_LIM_MUL;
    }
    else if (slider == pid_ui.dist) {
        motion_set_disturbance(&motion, val_f * 10);
    }
}

/**
 * @brief 滑动条数值清零回调 (点击数值标签触发)
 * @param[in] e LVGL 事件指针
 */
static void slider_clear_event_cb(lv_event_t* e)
{
    lv_obj_t* val = lv_event_get_target(e);
    lv_obj_t* slider = lv_event_get_user_data(e);


    if (slider == pid_ui.kp)              sim_pid.kp = 0;
    else if (slider == pid_ui.ki)         sim_pid.ki = 0;
    else if (slider == pid_ui.kd)         sim_pid.kd = 0;
    else if (slider == pid_ui.int_lim)    sim_pid.int_error_max = 0;
    else if (slider == pid_ui.out_lim) {
        sim_pid.out_max = 0;
        sim_pid.out_min = 0;
    }
    else if (slider == pid_ui.dist) {
        motion_set_disturbance(&motion, 0);
    }

    lv_label_set_text_fmt(val, "0.0");
    lv_slider_set_value(slider, 0, LV_ANIM_OFF);
}

/**
 * @brief 鼠标/触摸拖拽事件回调
 * @param[in] e LVGL 事件指针
 * @note 根据 current_mode，增加手动干扰力或改变目标位置
 */
static void drag_event_cb(lv_event_t* e) 
{
    lv_obj_t* target = lv_event_get_target(e);
    lv_obj_t* scr = lv_scr_act();
    
    if (target != scr && target != pid_ui.ball && target != pid_ui.target) {
        return; 
    }

    lv_event_code_t code = lv_event_get_code(e);
    lv_indev_t * indev = lv_indev_get_act();
    if (indev == NULL) return;

    if (code == LV_EVENT_PRESSING) {
        lv_point_t vect;
        lv_indev_get_vect(indev, &vect); 

        if (current_mode == UI_OPT_PUSH) {
            manual_force += (vect.x * FORCE_MUL);
        } else if (current_mode == UI_OPT_MOD) {
            target_pos += vect.x;
        }
    }
}

/**
 * @brief 物理仿真主定时器回调
 * @param[in] timer LVGL 定时器指针
 * @details 核心闭环周期：读取位置 -> PID 计算 -> 物理模型更新 -> UI 坐标刷新。
 */
static void sim_timer_cb(lv_timer_t * timer) 
{
    sim_pid.target = target_pos;
    sim_pid.current = motion.position;
    pid_update(&sim_pid);
    
    float total_force = sim_pid.out + manual_force;

    motion_update(&motion, total_force, SIM_TIME / 1000.0f);
    
    manual_force = 0.0f;

    lv_obj_set_x(pid_ui.ball, (int)motion.position + BALL_X_OFS);
    lv_obj_set_x(pid_ui.target, (int)target_pos + BALL_X_OFS);

    lv_label_set_text_fmt(pid_ui.pos_cur, "当前位置: %d", (int)motion.position);
    lv_label_set_text_fmt(pid_ui.pos_tgt, "预期位置: %d", (int)target_pos);
}