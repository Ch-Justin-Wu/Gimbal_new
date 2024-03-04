#ifndef BSP_CAN_H
#define BSP_CAN_H
#include "struct_typedef.h"
#include "stm32f4xx_hal.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief �ṹ�嶨�壺CAN���ջ�����
 */
typedef struct struct_can_rx_buffer
{
    CAN_RxHeaderTypeDef header; /**< CAN��Ϣͷ */
    uint8_t data[8];            /**< CAN��Ϣ���� */
} struct_can_rx_buffer;

/**
 * @brief CAN���ջص��������Ͷ���
 * @param rx_buffer ���յ���CAN��Ϣ������
 */
typedef void (*can_rx_callback)(struct_can_rx_buffer *);

/**
 * @brief �ṹ�嶨�壺CANͨ�Ź������
 */
typedef struct struct_can_manange_object
{
    CAN_HandleTypeDef *can_handler;       /**< CAN��� */
    struct_can_rx_buffer rx_buffer;       /**< CAN���ջ����� */
    can_rx_callback can_rx_callback_func; /**< CAN���ջص����� */
} struct_can_manange_object;

extern struct_can_manange_object can1_manange_object;
extern struct_can_manange_object can2_manage_object;

extern uint8_t can1_0x200_tx_data[8];
extern uint8_t can1_0x1ff_tx_data[8];
extern uint8_t can1_0x141_tx_data[8];

extern uint8_t can2_0x101_tx_data[8];

void can_filter_init(void);
void can_init(CAN_HandleTypeDef *hcan, can_rx_callback callback_function);

void TIM_CAN_PeriodElapsedCallback();



#ifdef __cplusplus
}
#endif


#endif
