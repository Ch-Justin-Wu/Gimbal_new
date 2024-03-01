#include "filter.h"

void struct_lpf_1_order_init(struct_lpf_1_order *this, float lpf_1_order_coefficient)
{
    this->lpf_1_order_out = 0.f;
    this->lpf_1_order_in = 0.f;
    this->lpf_1_order_coefficient = lpf_1_order_coefficient;
}

float struct_lpf_1_order_calc(struct_lpf_1_order *this, float in)
{
    this->lpf_1_order_in = in;
    this->lpf_1_order_out = this->lpf_1_order_out + this->lpf_1_order_coefficient * (this->lpf_1_order_in - this->lpf_1_order_out);
    return this->lpf_1_order_out;
}

void struct_lpf_1_order_param_init(struct_lpf_1_order *this)
{
    this->init = struct_lpf_1_order_init;
    this->calc = struct_lpf_1_order_calc;
}

void struct_kalman_init(struct_kalman *this, float _last_p, float _Q, float _R)
{
    this->Last_P = _last_p;
    // 初始化Q矩阵，用于存储状态转移矩阵的协方差
    this->Q = _Q;
    // 初始化R矩阵，用于存储测量噪声的协方差
    this->R = _R;
}

float struct_kalman_calc(struct_kalman *this, float input)
{
    // 预测协方差方程
    this->Now_P = this->Last_P + this->Q;
    // 卡尔曼增益方程
    this->Kg = this->Now_P / (this->Now_P + this->R);
    // 更新最优值方程
    this->out = this->out + this->Kg * (input - this->out);
    // 更新协方差方程
    this->Last_P = (1 - this->Kg) * this->Now_P;
    return this->out;
}

void struct_kalman_param_init(struct_kalman *this)
{
    this->init = struct_kalman_init;
    this->calc = struct_kalman_calc;
}

void struct_average_filter_init(struct_average_filter *this, uint8_t _filter_length)
{
    this->filter_length = _filter_length;             // 设置滤波器长度
    this->filter_count = 0;                           // 初始化滤波器计数器
    this->filter_sum = 0.f;                           // 初始化滤波器总和
    this->filter_out = 0.f;                           // 初始化滤波器输出
    for (uint8_t i = 0; i < this->filter_length; i++) // 将滤波器缓冲区的所有元素初始化为 0
    {
        this->filter_buf[i] = 0.f;
    }
}

float struct_average_filter_calc(struct_average_filter *this, float in)
{
    this->filter_sum = this->filter_sum - this->filter_buf[this->filter_count] + in; // 更新滤波器总和
    this->filter_buf[this->filter_count] = in;                                      // 将新的输入值存储到滤波器缓冲区的当前计数器位置
    this->filter_count++;                                                            // 增加滤波器计数器
    if (this->filter_count >= this->filter_length)                                   // 如果滤波器计数器达到滤波器长度
    {
        this->filter_count = 0; // 重置滤波器计数器
    }
    this->filter_out = this->filter_sum / this->filter_length; // 计算滤波器输出
    return this->filter_out;                                   // 返回滤波器输出
}

void struct_average_filter_param_init(struct_average_filter *this)
{
    this->init = struct_average_filter_init;
    this->calc = struct_average_filter_calc;
}

void struct_fir_filter_init(struct_fir_filter *this)
{
    // 遍历滤波器的长度
    for (int i = 0; i < FIR_FILTER_LENGTH; i++)
    {
        // 将滤波器系数和输入缓冲区初始化为0
        this->coeffs[i] = 0.0f;
        this->buffer[i] = 0.0f;
    }
    // 将当前缓冲区索引初始化为0
    this->index = 0;
}

void struct_fir_filter_set_coeffs(struct_fir_filter *this, const float *coeffs)
{
    // 遍历滤波器的长度
    for (int i = 0; i < FIR_FILTER_LENGTH; i++)
    {
        // 将输入的系数设置到滤波器系数数组中
        this->coeffs[i] = coeffs[i];
    }
}

float struct_fir_filter_calculate(struct_fir_filter *this, float input)
{
    // 将新的输入值存储到缓冲区的当前索引位置
    this->buffer[this->index] = input;
    float output = 0.0f;
    // 计算卷积
    for (int i = 0; i < FIR_FILTER_LENGTH; i++)
    {
        // 将滤波器系数和缓冲区的对应值相乘，然后累加到输出上
        output += this->coeffs[i] * this->buffer[(this->index + i) % FIR_FILTER_LENGTH];
    }
    // 更新缓冲区索引
    this->index = (this->index + 1) % FIR_FILTER_LENGTH;
    // 返回滤波器的输出
    return output;
}

