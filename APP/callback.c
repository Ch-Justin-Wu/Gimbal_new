#include "callback.h"

// 将device_can1_callback地址赋值给ptr_device_can1_callback
can_rx_callback ptr_device_can1_callback = device_can1_callback;
can_rx_callback ptr_device_can2_callback = device_can2_callback;

void device_can1_callback(struct_can_rx_buffer *can_rx_message)
{
    switch (can_rx_message->header.StdId)
    {

    default:
        break;
    }
}

void device_can2_callback(struct_can_rx_buffer *can_rx_message)
{
    
}
