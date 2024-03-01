#ifndef __PID_H
#define __PID_H

#include "bsp_math.h"

#ifdef __cplusplus
extern "C"
{
#endif

  // ö��
  enum
  {
    LAST = 0,
    NOW = 1,
  };

  typedef struct struct_pid struct_pid;

  typedef void (*pid_init)(struct struct_pid *this, float __max_output, float __intergral_limit, float __dead_band, float __max_err, float __p, float __i, float __d, uint16_t __separation_threshold);
  typedef void (*pid_reset)(struct struct_pid *this, float kp, float ki, float kd);
  typedef float (*pid_calc)(struct struct_pid *this, float _get, float _set);

  typedef struct struct_pid
  {
    /* p��i��d���� */
    float p;
    float i;
    float d;

    /* Ŀ��ֵ������ֵ�����ֵ */
    float set;
    float get;
    float err[2];

    /* p��i��d������������� */
    float pout;
    float iout;
    float dout;

    /* pid��ʽ������������ */
    float out;

    float max_err;
    float dead_band; // err < dead_band return
    /* pid����������  */
    float max_output;

    /* pid����������޷� */
    float intergral_limit;
    /* pid���ַ������� */
    float separation_threshold;
    /* ����ӦKp */
    float k1;
    float k2;
    float k3;

    pid_init init;
    pid_reset reset;
    pid_calc calc;

  } struct_pid;

  void pid_param_init(struct_pid *this);
  
#ifdef __cplusplus
}
#endif




#endif
