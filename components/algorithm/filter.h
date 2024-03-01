#ifndef __FILTER_H
#define __FILTER_H

#include "struct_typedef.h"
#include "bsp_math.h"
#include "stdint.h"

// ��ͨ�˲���ϵ��
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
    float lpf_1_order_out;         // һ�׵�ͨ�˲��������ֵ
    float lpf_1_order_in;          // һ�׵�ͨ�˲���������ֵ
    float lpf_1_order_coefficient; // һ�׵�ͨ�˲�����ϵ��

    lpf_1_order_init_func init;
    lpf_1_order_calc_func calc;
} struct_lpf_1_order;

void struct_lpf_1_order_param_init(struct_lpf_1_order *this);

typedef struct struct_kalman struct_kalman;

typedef void (*kalman_init_func)(struct struct_kalman *this, float _last_p, float _Q, float _R);
typedef float (*kalman_calc_func)(struct struct_kalman *this, float input);

typedef struct struct_kalman
{
    float Last_P; // �ϴι���Э���� ������Ϊ0 ! ! ! ! !
    float Now_P;  // ��ǰ����Э����
    float out;    // �������˲������
    float Kg;     // ����������
    float Q;      // ��������Э����
    float R;      // �۲�����Э����

    kalman_init_func init;
    kalman_calc_func calc;
} struct_kalman;

void struct_kalman_param_init(struct_kalman *this);

typedef struct struct_average_filter struct_average_filter;

typedef void (*average_filter_init_func)(struct struct_average_filter *this, uint8_t _filter_length);
typedef float (*average_filter_calc_func)(struct struct_average_filter *this, float in);

#define AVERAGE_FILTER_LENGTH_BUFFER 100u // �˲�������

typedef struct struct_average_filter
{
    // �˲�������
    uint8_t filter_length;                             // �˲�������
    uint8_t filter_count;                              // �˲�������
    float filter_sum;                                  // �˲������
    float filter_out;                                  // �˲������
    float filter_buf[AVERAGE_FILTER_LENGTH_BUFFER];    // ���侲̬�ڴ�

    average_filter_init_func init;
    average_filter_calc_func calc;
} struct_average_filter;

void struct_average_filter_param_init(struct_average_filter *this);

typedef struct struct_fir_filter struct_fir_filter;

typedef void (*fir_filter_init_func)(struct struct_fir_filter *this);
typedef void (*fir_filter_set_coeffs_func)(struct struct_fir_filter *this, const float *coeffs);
typedef float (*fir_filter_calculate_func)(struct struct_fir_filter *this, float input);

#define FIR_FILTER_LENGTH 64u // �˲�������

typedef struct struct_fir_filter
{
    float coeffs[FIR_FILTER_LENGTH];               // �˲���ϵ��
    float buffer[FIR_FILTER_LENGTH];               // ���뻺����
    int index;                                 // ��ǰ����������

    fir_filter_init_func init;
    fir_filter_set_coeffs_func set_coeffs;
    fir_filter_calculate_func calculate;
} struct_fir_filter;

void struct_fir_filter_param_init(struct_fir_filter *this);


#endif
