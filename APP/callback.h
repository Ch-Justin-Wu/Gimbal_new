#ifndef __CALLBACK_H
#define __CALLBACK_H

// #include "motor.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "chassis_move.h"
#include "gimbal_yaw.h"
#include "gimbal_pitch.h"
#ifdef __cplusplus
}
#endif

void device_can1_callback(struct_can_rx_buffer *can_rx_message);
void device_can2_callback(struct_can_rx_buffer *can_rx_message);
extern can_rx_callback ptr_device_can1_callback;
extern can_rx_callback ptr_device_can2_callback;
#endif
