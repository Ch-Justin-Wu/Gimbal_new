#ifndef __CHASSIS_MOVE_H
#define __CHASSIS_MOVE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "motor.h"
#include <stdio.h>
#include "remote_control.h"
#include "gimbal_yaw.h"

#ifdef __cplusplus
}
#endif

typedef enum enum_chassis_move_mode
{
    FOLLOW_GIMBAL,
    NO_FOLLOW_GIMBAL
} enum_chassis_move_mode;

typedef struct struct_chassis struct_chassis;

typedef void (*chassis_init)(struct struct_chassis *this);
typedef void (*chassis_TIM_Calculate_PeriodElapsedCallback)(struct struct_chassis *this);

typedef struct struct_chassis
{
    enum_chassis_move_mode chassis_move_mode;
    // ·¢ËÍ»º´æÇø
    uint8_t *can_tx_data;
    
    struct_lpf_1_order lpf_1_order_rc[3];

    float vx_set;
    float vy_set;
    float wz_set;
    float wheel_speed[4];

    chassis_init init;
    chassis_TIM_Calculate_PeriodElapsedCallback TIM_Calculate_PeriodElapsedCallback;

} struct_chassis;

extern struct_chassis omni_chassis;

void struct_chassis_param_init(struct_chassis *this);

#endif
