#include "bsp_can.h"
#include "main.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

struct_can_manange_object can1_manange_object = {0};
struct_can_manange_object can2_manage_object = {0};

uint8_t can1_0x200_tx_data[8] = {0};
uint8_t can1_0x1ff_tx_data[8] = {0};
//领控电机
uint8_t can1_0x141_tx_data[8] = {0};

uint8_t can2_0x101_tx_data[8] = {0};

void can_filter_init(void)
{

    CAN_FilterTypeDef can_filter_st;
    can_filter_st.FilterActivation = ENABLE;
    can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;
    can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;
    can_filter_st.FilterIdHigh = 0x0000;
    can_filter_st.FilterIdLow = 0x0000;
    can_filter_st.FilterMaskIdHigh = 0x0000;
    can_filter_st.FilterMaskIdLow = 0x0000;
    can_filter_st.FilterBank = 0;
    can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;

    if (HAL_CAN_ConfigFilter(&hcan1, &can_filter_st) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_CAN_Start(&hcan1) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
    {
        Error_Handler();
    }

    can_filter_st.SlaveStartFilterBank = 14;
    can_filter_st.FilterBank = 14;
    if (HAL_CAN_ConfigFilter(&hcan2, &can_filter_st)!= HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_CAN_Start(&hcan2) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
    {
        Error_Handler();
    }
}

void can_init(CAN_HandleTypeDef *hcan, can_rx_callback callback_function)
{

    if (hcan->Instance == CAN1)
    {
        can1_manange_object.can_handler = hcan;
        can1_manange_object.can_rx_callback_func = callback_function;
    }
    else if (hcan->Instance == CAN2)
    {
        can2_manage_object.can_handler = hcan;
        can2_manage_object.can_rx_callback_func = callback_function;
    }
}

/**
 * @brief 使用指定的CAN句柄发送CAN数据。
 *
 * 此函数使用提供的CAN句柄发送具有给定ID、数据和长度的CAN数据。
 *
 * @param hcan 用于发送数据的CAN句柄。
 * @param id CAN消息的ID。
 * @param data 指向要发送的数据缓冲区的指针。
 * @param length 数据缓冲区的长度。
 * @return 指示操作成功或失败的HAL状态。
 */
uint8_t can_send_data(CAN_HandleTypeDef *hcan, uint16_t id, uint8_t *data, uint8_t lenth)
{
    CAN_TxHeaderTypeDef can_tx_header;
    uint32_t tx_mailbox;

    assert_param(hcan != NULL);

    can_tx_header.StdId = id;
    can_tx_header.IDE = CAN_ID_STD;
    can_tx_header.RTR = CAN_RTR_DATA;
    can_tx_header.DLC = lenth;

    return (HAL_CAN_AddTxMessage(hcan, &can_tx_header, data, &tx_mailbox));
}

void TIM_CAN_PeriodElapsedCallback()
{
    //static uint8_t tx_cnt = 0;

    // 底盘控制+yaw(maybe)
    can_send_data(&hcan2, 0x101, can2_0x101_tx_data, 8);

    //can_send_data(&hcan1, 0x1ff, can1_0x1ff_tx_data, 8);
}

/**
 * @brief HAL库CAN接收FIFO0中断
 *
 * @param hcan CAN编号
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    if (hcan->Instance == CAN1)
    {
        HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &can1_manange_object.rx_buffer.header, can1_manange_object.rx_buffer.data);
        can1_manange_object.can_rx_callback_func(&can1_manange_object.rx_buffer);
    }
    // else if (hcan->Instance == CAN2)
    // {
    //     HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &can2_manage_object.rx_buffer.header, can2_manage_object.rx_buffer.data);
    //     can2_manage_object.can_rx_callback_func(&can2_manage_object.rx_buffer);
    // }
}