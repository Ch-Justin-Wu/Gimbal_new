#ifndef INIT_H
#define INIT_H


#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "can.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "bsp_can.h"
#include "chassis_move.h"
#include "pid.h"
#include "callback.h"
#include "gimbal_yaw.h"
#include "gimbal_pitch.h"
#include "bsp_imu.h"

    void init_all(void);
#ifdef __cplusplus
}
#endif

#endif
