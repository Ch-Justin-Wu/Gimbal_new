#include "init.h"
#include "delay.h"


void init_all(void)
{
    
    delay_init();
    remote_control_init();
    struct_chassis_param_init(&omni_chassis);
    struct_pitch_init(&struct_pitch);
    omni_chassis.init(&omni_chassis);
    can_filter_init();

    can_init(&hcan1, ptr_device_can1_callback);
    can_init(&hcan2, ptr_device_can2_callback);
    HAL_TIM_PWM_Start(&htim10, TIM_CHANNEL_1);
    IMU_Init();

    HAL_TIM_Base_Start_IT(&htim3);
    HAL_TIM_Base_Start_IT(&htim4);
}
