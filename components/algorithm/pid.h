#ifndef __PID_H
#define __PID_H

#include "bsp_math.h"

#ifdef __cplusplus
extern "C"
{
#endif

  // 枚举
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
    /* p、i、d参数 */
    float p;
    float i;
    float d;

    /* 目标值、反馈值、误差值 */
    float set;
    float get;
    float err[2];

    /* p、i、d各项计算出的输出 */
    float pout;
    float iout;
    float dout;

    /* pid公式计算出的总输出 */
    float out;

    float max_err;
    float dead_band; // err < dead_band return
    /* pid最大输出限制  */
    float max_output;

    /* pid积分输出项限幅 */
    float intergral_limit;
    /* pid积分分离设置 */
    float separation_threshold;
    /* 自适应Kp */
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
