#ifndef __LK_MOTOR_H
#define __LK_MOTOR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "motor.h"

    typedef struct struct_lk_motor struct_lk_motor;

    typedef void (*lk_motor_init)(struct_lk_motor *this);
    typedef void (*lk_motor_output)(struct_lk_motor *this, int16_t speed);
    //can
    typedef void (*lk_motor_can_rx_callback)(struct_lk_motor *this,uint8_t *data);
    typedef void (*lk_motor_TIM_Calculate_PeriodElapsedCallback)(struct_lk_motor *this);

    typedef struct struct_lk_motor
    {
        struct_can_manange_object *can_manage_object;

        struct_pid pid_speed;
        struct_pid pid_posistion;

        // 收数据绑定的can id
        enum_can_id can_id;
        // 发送缓存区
        uint8_t *can_tx_data;

        // 电机速度
        int16_t speed;
        //电机温度
        uint8_t temperature;
        
        int16_t set_speed;

        uint32_t msg_cnt;
        float out;

        lk_motor_init init;
        lk_motor_output output;
        lk_motor_can_rx_callback can_rx_callback;
        lk_motor_TIM_Calculate_PeriodElapsedCallback TIM_Calculate_PeriodElapsedCallback;

    } struct_lk_motor;

#ifdef __cplusplus
}
#endif

#endif /* __LK_MOTOR_H */