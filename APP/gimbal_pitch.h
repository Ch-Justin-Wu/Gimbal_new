#ifndef __GIMBAL_PITCH_H
#define __GIMBAL_PITCH_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "motor.h"

#ifdef __cplusplus
}
#endif

extern struct_c620 struct_pitch;

void struct_pitch_init(struct_c620 *this);
void pitch_TIM_PID_PeriodElapsedCallback(struct_c620 *this);
#define PITCH_MOTOR_ID 0x141

#endif // __GIMBAL_PITCH_H