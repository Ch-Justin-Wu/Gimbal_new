#include "chassis_move.h"

struct_chassis omni_chassis;

static uint8_t *allocate_tx_data(CAN_HandleTypeDef *__hcan, enum_can_id __can_id)
{
    uint8_t *tmp_tx_data_ptr = NULL;
    if (__hcan->Instance == CAN1)
    {
        switch (__can_id)
        {

        default:
            break;
        }
    }
    else if (__hcan->Instance == CAN2)
    {
        switch (__can_id)
        {
        case CAN_CHASSIS_ID_0x101:
            tmp_tx_data_ptr = &can2_0x101_tx_data[0];
            break;
        default:
            break;
        }
    }
    return tmp_tx_data_ptr;
}

// ��ʼ�����̽ṹ��
void struct_chassis_init(struct_chassis *this)
{
    this->chassis_move_mode = NO_FOLLOW_GIMBAL;
    // ���ó�ʼ�ٶ�Ϊ0
    this->vx_set = 0;
    this->vy_set = 0;
    this->wz_set = 0;
    // can����
    this->can_tx_data = allocate_tx_data(&hcan2, CAN_CHASSIS_ID_0x101);
    // ��ʼ������ٶ�Ϊ0
    for (uint8_t i = 0; i < motor_num; i++)
    {
        this->wheel_speed[i] = 0;
    }

    // ��ʼ��ң����һ�׵�ͨ�˲���
    for (uint8_t i = 0; i < 3; i++)
    {
        this->lpf_1_order_rc[i].init(&(this->lpf_1_order_rc[i]), LPF_2_HZ_CUT);
    }
}

// ���õ��̵��ٶ�
static inline void struct_chassis_set_vx_vy_wz(struct_chassis *this, fp32 vx, fp32 vy, fp32 wz)
{
    this->vx_set = vx;
    this->vy_set = vy;
    this->wz_set = wz;
}

// ���ٶȽ���һ�׵�ͨ�˲�
static inline void struct_chassis_speed_lpf(struct_chassis *this)
{
    this->vx_set = this->lpf_1_order_rc[0].calc(&(this->lpf_1_order_rc[0]), this->vx_set);
    this->vy_set = this->lpf_1_order_rc[1].calc(&(this->lpf_1_order_rc[1]), this->vy_set);
    this->wz_set = this->lpf_1_order_rc[2].calc(&(this->lpf_1_order_rc[2]), this->wz_set);
}

// ��ң������ȡ�ٶ�����
void struct_chassis_set_vx_vy_wz_rc(struct_chassis *this)
{
    const float k_vx = 4.0f;
    const float k_vy = 4.0f;
    const float k_wz = 3.0f;
    if (this->chassis_move_mode == FOLLOW_GIMBAL)
    {

        this->vx_set = rc_ctrl.rc.ch[1] * k_vx;
        this->vy_set = (-rc_ctrl.rc.ch[0]) * k_vy;
        this->wz_set = (-rc_ctrl.rc.ch[2]) * 0.f;
    }
    else if (this->chassis_move_mode == NO_FOLLOW_GIMBAL)
    {
        this->vx_set = rc_ctrl.rc.ch[1] * k_vx;
        this->vy_set = (-rc_ctrl.rc.ch[0]) * k_vy;
        this->wz_set = (-rc_ctrl.rc.ch[2]) * k_wz;
    }
}

void chassis_speed_output(struct_chassis *this)
{
    this->can_tx_data[0] = (int16_t)(this->vx_set) >> 8;
    this->can_tx_data[1] = (int16_t)(this->vx_set);
    this->can_tx_data[2] = (int16_t)(this->vy_set) >> 8;
    this->can_tx_data[3] = (int16_t)(this->vy_set);
    this->can_tx_data[4] = (int16_t)(this->wz_set) >> 8;
    this->can_tx_data[5] = (int16_t)(this->wz_set);
}

// �������ڻص�����
void struct_chassis_TIM_Calculate_PeriodElapsedCallback(struct_chassis *this)
{
    // ��ң������ȡ�ٶ�����
    struct_chassis_set_vx_vy_wz_rc(this);
    // ���ٶȽ���һ�׵�ͨ�˲�
    struct_chassis_speed_lpf(this);
    // ����ٶ�
    chassis_speed_output(this);
}

// ��ʼ�����̲���
void struct_chassis_param_init(struct_chassis *this)
{

    // ��ʼ��һ�׵�ͨ�˲�������
    for (uint8_t i = 0; i < 3; i++)
    {
        struct_lpf_1_order_param_init(&(this->lpf_1_order_rc[i]));
    }

    this->init = struct_chassis_init;
    this->TIM_Calculate_PeriodElapsedCallback = struct_chassis_TIM_Calculate_PeriodElapsedCallback;
}