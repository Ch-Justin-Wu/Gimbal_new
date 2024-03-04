#ifndef __MOTOR_H
#define __MOTOR_H

#define motor_num 4

#include "mytype.h"
#include "can.h"
#include "pid.h"
#include "bsp_can.h"
#include "filter.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif

#define KP 15000
#define KI 10
#define KD 0
#define OUTPUT_LIMIT 16000
#define INTE_OUTPUT_LIMIT 5000

typedef enum enum_can_id
{
	CAN_CHASSIS_ID_0x101 = 0x101,

	CAN_LK_MOTOR_ID_0x141 = 0x141,

	CAN_MOTOR_ID_UNDEFINED = 0x200,
	CAN_MOTOR_ID_0x201,
	CAN_MOTOR_ID_0x202,
	CAN_MOTOR_ID_0x203,
	CAN_MOTOR_ID_0x204,
	CAN_MOTOR_ID_0x205,
	CAN_MOTOR_ID_0x206,
	CAN_MOTOR_ID_0x207,
	CAN_MOTOR_ID_0x208

} enum_can_id;

typedef enum enum_filter_type
{
	FILTER_UNUSED,
	FILTER_LPF_1_ORDER,
	FILTER_AVERAGE,
	FILTER_KALMAN
} enum_filter_type;

typedef struct struct_c620_data_reverse
{
	uint16_t angle_reverse;
	int16_t current_reverse;
	int16_t given_current_reverse;
	int8_t temperature_reverse;
	uint8_t reverses;
} __attribute__((packed)) struct_c620_data_reverse;

typedef struct struct_c620_data
{
	int16_t now_rpm;
	int16_t target_rpm;

	int16_t real_current;
	int16_t given_current;
	uint8_t now_temperature;
	uint16_t angle;		 // abs angle range:[0,8191]
	uint16_t last_angle; // abs angle range:[0,8191]
	uint16_t offset_angle;
	int32_t total_round;
	int32_t total_angle;
	float target_angle;
} struct_c620_data;

typedef struct struct_c620 struct_c620;

typedef void (*c620_init)(struct struct_c620 *this, CAN_HandleTypeDef *hcan, enum_can_id __can_id, enum_filter_type __filter_type);
typedef uint8_t (*c620_get_measure)(struct struct_c620 *this);
typedef void (*c620_get_offset)(struct struct_c620 *this);
typedef void (*c620_out_put)(struct struct_c620 *this);
typedef void (*c620_can_rx_cplt_callback)(struct struct_c620 *this, uint8_t *rx_data);
typedef void (*c620_tim_pid_period_elapsed_callback)(struct struct_c620 *this);

typedef struct struct_c620
{
	struct_pid pid_speed;
	struct_pid pid_position;

	struct_c620_data data;

	struct_lpf_1_order lpf_1_order;
	struct_average_filter average_filter;
	struct_kalman kalman;

	struct_can_manange_object *can_manage_object;
	// 收数据绑定的can id
	enum_can_id can_id;
	// 发送缓存区
	uint8_t *can_tx_data;

	// 滤波器类型
	enum_filter_type filter_type;

	u32 msg_cnt;
	float out;

	c620_init init;
	c620_get_measure get_moto_measure;
	c620_get_offset get_moto_offset;
	c620_out_put out_put;
	c620_can_rx_cplt_callback CAN_RxCpltCallback;
	c620_tim_pid_period_elapsed_callback TIM_PID_PeriodElapsedCallback;

} struct_c620;

void struct_c620_param_init(struct_c620 *this);

#endif
