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
    // ���õ�ǰ�����ֵ
    this->get = _get;
    this->set = _set;
    this->err[NOW] = _set - _get;

    // ��鵱ǰ������Ƿ񳬹�������������
    if (this->max_err != 0 && MATH_ABS(this->err[NOW]) > this->max_err)
        return 0;

    // ��鵱ǰ������Ƿ���dead_band��Χ��
    if (this->dead_band != 0 && MATH_ABS(this->err[NOW]) < this->dead_band)
        return 0; // ��������

    // ����ӦKp����
    if (this->k1 != 0 || this->k2 != 0 || this->k3 != 0)
    {
        this->p = this->k1 * log10f(this->k2 * MATH_ABS(this->err[NOW]) + this->k3);
    }

    // �����������
    this->pout = this->p * this->err[NOW];

    if (this->separation_threshold != 0 && MATH_ABS(this->err[NOW]) > this->separation_threshold)
    {
        this->iout = 0;
    }

    else if (!this->separation_threshold || MATH_ABS(this->err[NOW]) <= this->separation_threshold)
    {
        // ���»��ֲ���
        this->iout += this->i * this->err[NOW];
    }

    // ����΢�ֲ���
    this->dout = this->d * (this->err[NOW] - this->err[LAST]);

    // ���ƻ��ֲ���
    ABS_LIMIT(&(this->iout), this->intergral_limit);

    // �����ܵĿ������
    this->out = this->pout + this->iout + this->dout;

    // �����ܵĿ������
    ABS_LIMIT(&(this->out), this->max_output);

    // ������һ�ε��������
    this->err[LAST] = this->err[NOW];

    return this->out;
}

void pid_param_init(struct_pid *this)
{
    this->init = struct_pid_init;
    this->reset = struct_pid_reset;
    this->calc = struct_pid_calc;
}
