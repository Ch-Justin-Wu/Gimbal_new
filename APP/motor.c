/* Includes ------------------------------------------------------------------*/
#include "motor.h"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
static uint8_t *allocate_tx_data(CAN_HandleTypeDef *__hcan, enum_can_id __can_id)
{
    uint8_t *tmp_tx_data_ptr = NULL;
    if (__hcan->Instance == CAN1)
    {
        switch (__can_id)
        {
        case CAN_MOTOR_ID_0x201:
            tmp_tx_data_ptr = &can1_0x200_tx_data[0];
            break;
        case CAN_MOTOR_ID_0x202:
            tmp_tx_data_ptr = &can1_0x200_tx_data[2];
            break;
        case CAN_MOTOR_ID_0x203:
            tmp_tx_data_ptr = &can1_0x200_tx_data[4];
            break;
        case CAN_MOTOR_ID_0x204:
            tmp_tx_data_ptr = &can1_0x200_tx_data[6];
            break;
        case CAN_MOTOR_ID_0x206:
            tmp_tx_data_ptr = &can1_0x1ff_tx_data[2];
            break;
            // gm6020
        case CAN_MOTOR_ID_0x208:
            tmp_tx_data_ptr = &can1_0x1ff_tx_data[6];
            break;
        default:
            break;
        }
    }

    return tmp_tx_data_ptr;
}

void struct_c620_init(struct_c620 *this, CAN_HandleTypeDef *__hcan, enum_can_id __can_id, enum_filter_type __filter_type)
{
    if (__hcan->Instance == CAN1)
    {
        this->can_manage_object = &can1_manange_object;
    }
    else if (__hcan->Instance == CAN2)
    {
        this->can_manage_object = &can2_manage_object;
    }
    this->can_id = __can_id;
    this->can_tx_data = allocate_tx_data(__hcan, __can_id);
    this->filter_type = __filter_type;
    this->msg_cnt = 0;
    this->out = 0.0f;
}

uint8_t struct_c620_get_measure(struct_c620 *this)
{
    uint16_t tmp_angle = 0;
    int16_t tmp_current = 0, tmp_given_current = 0, tmp_temperature = 0;

    struct_c620_data_reverse *tmp_buffer = (struct_c620_data_reverse *)this->can_manage_object->rx_buffer.data;

    // 处理大小端
    math_endian_reverse_16((void *)&tmp_buffer->angle_reverse, (void *)&tmp_angle);
    math_endian_reverse_16((void *)&tmp_buffer->current_reverse, (void *)&tmp_current);
    math_endian_reverse_16((void *)&tmp_buffer->given_current_reverse, (void *)&tmp_given_current);

    this->data.last_angle = this->data.angle;
    this->data.angle = tmp_angle;

    this->data.real_current = tmp_current;
    this->data.now_rpm = tmp_current;

    this->data.given_current = tmp_given_current / (-5);
    this->data.now_temperature = tmp_buffer->temperature_reverse;

    if (this->data.angle - this->data.last_angle > 4096)
        this->data.total_round--;
    else if (this->data.angle - this->data.last_angle < -4096)
        this->data.total_round++;

    this->data.total_angle = this->data.total_round * 8192 + this->data.angle - this->data.offset_angle;

    // Return the Data Length Code (DLC) from the received CAN message.
    return (this->can_manage_object->rx_buffer.header.DLC);
}

void struct_c620_get_offset(struct_c620 *this)
{
    struct_c620_data_reverse *tmp_buffer = (struct_c620_data_reverse *)this->can_manage_object->rx_buffer.data;
    uint16_t tmp_angle = 0;

    math_endian_reverse_16((void *)&tmp_buffer->angle_reverse, (void *)&tmp_angle);

    this->data.offset_angle = tmp_angle;
}

void struct_c620_out_put(struct_c620 *this)
{
    this->can_tx_data[0] = (int16_t)this->out >> 8;
    this->can_tx_data[1] = (int16_t)this->out;
}

void struct_c620_CAN_RxCpltCallback(struct_c620 *this, uint8_t *rx_data)
{
    if (this->msg_cnt++ <= 50)
    {
        this->get_moto_offset(this);
    }
    else
    {
        this->get_moto_measure(this);
    }
}

void struct_c620_TIM_PID_PeriodElapsedCallback(struct_c620 *this)
{
    switch (this->filter_type)
    {
    case FILTER_LPF_1_ORDER:
        this->data.now_rpm = this->lpf_1_order.calc(&(this->lpf_1_order), this->data.now_rpm);
        break;
    case FILTER_AVERAGE:
        this->data.now_rpm = this->average_filter.calc(&(this->average_filter), this->data.now_rpm);
        break;
    case FILTER_KALMAN:
        this->data.now_rpm = this->kalman.calc(&(this->kalman), this->data.now_rpm);
        break;
    case FILTER_UNUSED:
        // 逻辑代码
        break;
    default:
        // 逻辑代码
        break;
    }
    this->pid_speed.calc(&(this->pid_speed), this->data.now_rpm, this->data.target_rpm);
    this->out = this->pid_speed.out;
    this->out_put(this);
}

void struct_c620_param_init(struct_c620 *this)
{

    struct_lpf_1_order_param_init(&this->lpf_1_order);
    struct_average_filter_param_init(&this->average_filter);
    struct_kalman_param_init(&this->kalman);

    pid_param_init(&this->pid_speed);
    pid_param_init(&this->pid_position);

    this->init = struct_c620_init;
    this->get_moto_measure = struct_c620_get_measure;
    this->get_moto_offset = struct_c620_get_offset;
    this->out_put = struct_c620_out_put;
    this->CAN_RxCpltCallback = struct_c620_CAN_RxCpltCallback;
    this->TIM_PID_PeriodElapsedCallback = struct_c620_TIM_PID_PeriodElapsedCallback;
}
