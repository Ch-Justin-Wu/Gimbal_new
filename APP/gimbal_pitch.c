#include "gimbal_pitch.h"
#include "remote_control.h"

//struct_c620 struct_pitch;

void struct_pitch_init(struct_c620 *this)
{
    struct_c620_param_init(this);

    this->init(this, &hcan1, CAN_MOTOR_ID_0x206, FILTER_UNUSED);
    switch (this->filter_type)
    {
    case FILTER_LPF_1_ORDER:
        this->lpf_1_order.init(&(this->lpf_1_order), LPF_50_HZ_CUT);
        break;
    case FILTER_AVERAGE:
        this->average_filter.init(&(this->average_filter), 2);
        break;
    case FILTER_KALMAN:
        this->kalman.init(&(this->kalman), 0.01f, 0.001f, 0.001f);
        break;
    case FILTER_UNUSED:
        // No operation for FILTER_UNUSED
        break;
    default:
        // Handle unexpected filter_type
        break;
    }

    // 设置PID控制器的参数
    const float max_total_output = 16000.0f;
    const float max_interval_output = 6000.0f;

    const float position_pid[3] = {6.0f, 0.0f, 0.0f};
    const float speed_pid[3] = {10.0f, 2.5f, 20.0f};
    const float position_dead_band = 10.0f;

    this->pid_position.init(&(this->pid_position), 1500.0f, 0.0f, position_dead_band, 0,
                            position_pid[0], position_pid[1], position_pid[2], 0);
    this->pid_speed.init(&(this->pid_speed), max_total_output, max_interval_output, 0.0f, 0,
                         speed_pid[0], speed_pid[1], speed_pid[2], 0);

    this->pid_speed.k1 = 0.0f;
    this->pid_speed.k2 = 0.0f;
    this->pid_speed.k3 = 0.0f;
}

// 机械转子角度0-8191
void struct_pitch_position_loop(struct_c620 *this, float __target_angle)
{

    this->data.target_angle = __target_angle;
    float tmp_position_out = this->pid_position.calc(&(this->pid_position), this->data.total_angle / 8192.0f * 360.0f, this->data.target_angle);
    this->data.target_rpm = tmp_position_out;
}

void pitch_TIM_PID_PeriodElapsedCallback(struct_c620 *this)
{
    float tmp_angle = 0;
    tmp_angle = (rc_ctrl.rc.ch[3] + 660.0f) / 660.0f / 2.0f * 1610.0f - 0.0f; // 范围-10到1600

    struct_pitch_position_loop(this, tmp_angle);
    this->TIM_PID_PeriodElapsedCallback(this);
}