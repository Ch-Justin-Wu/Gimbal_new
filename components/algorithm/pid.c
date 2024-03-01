#include "pid.h"
#include <math.h>

void struct_pid_init(struct_pid *this, float __max_output, float __intergral_limit, float __dead_band, float __max_err, float __p, float __i, float __d, uint16_t __separation_threshold)
{
    this->max_output = __max_output;
    this->intergral_limit = __intergral_limit;
    this->dead_band = __dead_band;
    this->max_err = __max_err;
    this->p = __p;
    this->i = __i;
    this->d = __d;
    this->separation_threshold = __separation_threshold;
}

void struct_pid_reset(struct_pid *this, float kp, float ki, float kd)
{
    this->p = kp;
    this->i = ki;
    this->d = kd;
}

float struct_pid_calc(struct_pid *this, float _get, float _set)
{
    // 设置当前的误差值
    this->get = _get;
    this->set = _set;
    this->err[NOW] = _set - _get;

    // 检查当前的误差是否超过了最大误差限制
    if (this->max_err != 0 && MATH_ABS(this->err[NOW]) > this->max_err)
        return 0;

    // 检查当前的误差是否在dead_band范围内
    if (this->dead_band != 0 && MATH_ABS(this->err[NOW]) < this->dead_band)
        return 0; // 死区控制

    // 自适应Kp调节
    if (this->k1 != 0 || this->k2 != 0 || this->k3 != 0)
    {
        this->p = this->k1 * log10f(this->k2 * MATH_ABS(this->err[NOW]) + this->k3);
    }

    // 计算比例部分
    this->pout = this->p * this->err[NOW];

    if (this->separation_threshold != 0 && MATH_ABS(this->err[NOW]) > this->separation_threshold)
    {
        this->iout = 0;
    }

    else if (!this->separation_threshold || MATH_ABS(this->err[NOW]) <= this->separation_threshold)
    {
        // 更新积分部分
        this->iout += this->i * this->err[NOW];
    }

    // 计算微分部分
    this->dout = this->d * (this->err[NOW] - this->err[LAST]);

    // 限制积分部分
    ABS_LIMIT(&(this->iout), this->intergral_limit);

    // 计算总的控制输出
    this->out = this->pout + this->iout + this->dout;

    // 限制总的控制输出
    ABS_LIMIT(&(this->out), this->max_output);

    // 更新下一次迭代的误差
    this->err[LAST] = this->err[NOW];

    return this->out;
}

void pid_param_init(struct_pid *this)
{
    this->init = struct_pid_init;
    this->reset = struct_pid_reset;
    this->calc = struct_pid_calc;
}
