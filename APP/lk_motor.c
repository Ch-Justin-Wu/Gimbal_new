#include "lk_motor.h"

struct_lk_motor picth_motor;

static uint8_t *allocate_tx_data(CAN_HandleTypeDef *__hcan, enum_can_id __can_id)
{
    uint8_t *tmp_tx_data_ptr = NULL;
    if (__hcan->Instance == CAN1)
    {
        switch (__can_id)
        {
        case CAN_LK_MOTOR_ID_0x141:
            tmp_tx_data_ptr = &can1_0x141_tx_data[0];
            break;
        default:
            break;
        }
    }
    else if (__hcan->Instance == CAN2)
    {
        switch (__can_id)
        {

        default:
            break;
        }
    }
    return tmp_tx_data_ptr;
}

void struct_lk_motor_init(struct_lk_motor *this)
{
    this->can_tx_data = allocate_tx_data(&hcan1, CAN_LK_MOTOR_ID_0x141);
    this->can_manage_object = &can1_manange_object;

    // 初始化电机速度为0
    this->speed = 0;
    this->temperature = 0;
    this->set_speed = 0;

    //pid
    
}

void struct_lk_motor_data_process(struct_lk_motor *this, uint8_t *data)
{
    if (data[0] == 0xA8)
    {
        this->speed = (int16_t)((data[5] << 8) | data[4]);
        this->temperature = (uint8_t)data[1];
    }
}

void struct_lk_motor_output(struct_lk_motor *this, int16_t speed)
{
    // 0xA1
    this->can_tx_data[0] = 0xA1;
    this->can_tx_data[4] = (uint8_t)speed;
    this->can_tx_data[5] = (uint8_t)(speed >> 8);
}

void struct_lk_motor_can_rx_callback(struct_lk_motor *this)
{
    uint8_t *data = (uint8_t *)this->can_manage_object->rx_buffer.data;
    struct_lk_motor_data_process(this, data);
}

void struct_lk_motor_TIM_Calculate_PeriodElapsedCallback(struct_lk_motor *this)
{
    this->out =this->pid_speed.calc(&this->pid_speed, this->speed, this->set_speed);
    this->output(this, (int16_t)this->out);
}

void struct_lk_motor_set_speed(struct_lk_motor *this, int16_t __speed)
{
    this->set_speed = __speed;
}

void struct_lk_motor_param_init(struct_lk_motor *this)
{
    this->init = struct_lk_motor_init;
    this->output = struct_lk_motor_output;
    this->can_rx_callback = struct_lk_motor_can_rx_callback;
    this->TIM_Calculate_PeriodElapsedCallback = struct_lk_motor_TIM_Calculate_PeriodElapsedCallback;

    //pid
    pid_param_init(&this->pid_speed);
    pid_param_init(&this->pid_posistion);
}