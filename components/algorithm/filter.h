#ifndef __FILTER_H
#define __FILTER_H

#include "struct_typedef.h"
#include "bsp_math.h"
#include "stdint.h"

// 低通滤波器系数
#define LPF_1_HZ_CUT 0.006244f
#define LPF_2_HZ_CUT 0.012410f
#define LPF_5_HZ_CUT 0.030459f
#define LPF_10_HZ_CUT 0.059117f
#define LPF_20_HZ_CUT 0.111635f
#define LPF_30_HZ_CUT 0.158600f
#define LPF_40_HZ_CUT 0.200849f
#define LPF_50_HZ_CUT 0.239057f
#define LPF_60_HZ_CUT 0.273779f
#define LPF_70_HZ_CUT 0.305470f
#define LPF_80_HZ_CUT 0.334511f
#define LPF_90_HZ_CUT 0.361221f
#define LPF_100_HZ_CUT 0.385870f
#define LPF_200_HZ_CUT 0.556863f
#define LPF_500_HZ_CUT 0.758547f

typedef struct struct_lpf_1_order struct_lpf_1_order;

typedef void (*lpf_1_order_init_func)(struct struct_lpf_1_order *this, float lpf_1_order_coefficient);
typedef float (*lpf_1_order_calc_func)(struct struct_lpf_1_order *this, float in);

typedef struct struct_lpf_1_order
{
    float lpf_1_order_out;         // 一阶低通滤波器的输出值
    float lpf_1_order_in;          // 一阶低通滤波器的输入值
    float lpf_1_order_coefficient; // 一阶低通滤波器的系数

    lpf_1_order_init_func init;
    lpf_1_order_calc_func calc;
} struct_lpf_1_order;

void struct_lpf_1_order_param_init(struct_lpf_1_order *this);

typedef struct struct_kalman struct_kalman;

typedef void (*kalman_init_func)(struct struct_kalman *this, float _last_p, float _Q, float _R);
typedef float (*kalman_calc_func)(struct struct_kalman *this, float input);

typedef struct struct_kalman
{
    float Last_P; // 上次估算协方差 不可以为0 ! ! ! ! !
    float Now_P;  // 当前估算协方差
    float out;    // 卡尔曼滤波器输出
    float Kg;     // 卡尔曼增益
    float Q;      // 过程噪声协方差
    float R;      // 观测噪声协方差

    kalman_init_func init;
    kalman_calc_func calc;
} struct_kalman;

void struct_kalman_param_init(struct_kalman *this);

typedef struct struct_average_filter struct_average_filter;

typedef void (*average_filter_init_func)(struct struct_average_filter *this, uint8_t _filter_length);
typedef float (*average_filter_calc_func)(struct struct_average_filter *this, float in);

#define AVERAGE_FILTER_LENGTH_BUFFER 100u // 滤波器长度

typedef struct struct_average_filter
{
    // 滤波器长度
    uint8_t filter_length;                             // 滤波器长度
    uint8_t filter_count;                              // 滤波器计数
    float filter_sum;                                  // 滤波器求和
    float filter_out;                                  // 滤波器输出
    float filter_buf[AVERAGE_FILTER_LENGTH_BUFFER];    // 分配静态内存

    average_filter_init_func init;
    average_filter_calc_func calc;
} struct_average_filter;

void struct_average_filter_param_init(struct_average_filter *this);

typedef struct struct_fir_filter struct_fir_filter;

typedef void (*fir_filter_init_func)(struct struct_fir_filter *this);
typedef void (*fir_filter_set_coeffs_func)(struct struct_fir_filter *this, const float *coeffs);
typedef float (*fir_filter_calculate_func)(struct struct_fir_filter *this, float input);

#define FIR_FILTER_LENGTH 64u // 滤波器长度

typedef struct struct_fir_filter
{
    float coeffs[FIR_FILTER_LENGTH];               // 滤波器系数
    float buffer[FIR_FILTER_LENGTH];               // 输入缓冲区
    int index;                                 // 当前缓冲区索引

    fir_filter_init_func init;
    fir_filter_set_coeffs_func set_coeffs;
    fir_filter_calculate_func calculate;
} struct_fir_filter;

void struct_fir_filter_param_init(struct_fir_filter *this);


#endif
