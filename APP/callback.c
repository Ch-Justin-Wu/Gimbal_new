#include "callback.h"

// 将device_can1_callback地址赋值给ptr_device_can1_callback
can_rx_callback ptr_device_can1_callback = device_can1_callback;
can_rx_callback ptr_device_can2_callback = device_can2_callback;

void device_can1_callback(struct_can_rx_buffer *can_rx_message)
{
    switch (can_rx_message->header.StdId)
    {
    // case 0x201:
    //     omni_chassis.motor_chassis[0].CAN_RxCpltCallback(&(omni_chassis.motor_chassis[0]), can_rx_message->data);
    //     break;
    // case 0x202:
    //     omni_chassis.motor_chassis[1].CAN_RxCpltCallback(&(omni_chassis.motor_chassis[1]), can_rx_message->data);
    //     break;
    // case 0x203:
    //     omni_chassis.motor_chassis[2].CAN_RxCpltCallback(&(omni_chassis.motor_chassis[2]), can_rx_message->data);
    //     break;
    // case 0x204:
    //     omni_chassis.motor_chassis[3].CAN_RxCpltCallback(&(omni_chassis.motor_chassis[3]), can_rx_message->data);
    //     break;
    // case 0x208:
    //     yaw_gm6020.CAN_RxCpltCallback(&yaw_gm6020, can_rx_message->data);
    //     break;
    case 0x206:
        struct_pitch.CAN_RxCpltCallback(&struct_pitch, can_rx_message->data);
        break;
    default:
        break;
    }
}

void device_can2_callback(struct_can_rx_buffer *can_rx_message)
{
    
}
