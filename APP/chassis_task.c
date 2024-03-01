#include "chassis_task.h"
#include "bsp_imu.h"
// 0.5ms
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    static uint8_t pid_timer_1ms = 0;
    if (htim->Instance == TIM3)
    {
        const uint8_t pid_period = 2;
        pid_timer_1ms++;
        if (pid_timer_1ms == pid_period)
        {
            pid_timer_1ms = 0;
        }
        // pitch_TIM_PID_PeriodElapsedCallback(&struct_pitch);
        // TIM_CAN_PeriodElapsedCallback();
    }

    if (htim->Instance == TIM4)
    {

        INS_task();
    }
}
