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
    // ��ʼ��Q�������ڴ洢״̬ת�ƾ����Э����
    this->Q = _Q;
    // ��ʼ��R�������ڴ洢����������Э����
    this->R = _R;
}

float struct_kalman_calc(struct_kalman *this, float input)
{
    // Ԥ��Э�����
    this->Now_P = this->Last_P + this->Q;
    // ���������淽��
    this->Kg = this->Now_P / (this->Now_P + this->R);
    // ��������ֵ����
    this->out = this->out + this->Kg * (input - this->out);
    // ����Э�����
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
    this->filter_length = _filter_length;             // �����˲�������
    this->filter_count = 0;                           // ��ʼ���˲���������
    this->filter_sum = 0.f;                           // ��ʼ���˲����ܺ�
    this->filter_out = 0.f;                           // ��ʼ���˲������
    for (uint8_t i = 0; i < this->filter_length; i++) // ���˲���������������Ԫ�س�ʼ��Ϊ 0
    {
        this->filter_buf[i] = 0.f;
    }
}

float struct_average_filter_calc(struct_average_filter *this, float in)
{
    this->filter_sum = this->filter_sum - this->filter_buf[this->filter_count] + in; // �����˲����ܺ�
    this->filter_buf[this->filter_count] = in;                                      // ���µ�����ֵ�洢���˲����������ĵ�ǰ������λ��
    this->filter_count++;                                                            // �����˲���������
    if (this->filter_count >= this->filter_length)                                   // ����˲����������ﵽ�˲�������
    {
        this->filter_count = 0; // �����˲���������
    }
    this->filter_out = this->filter_sum / this->filter_length; // �����˲������
    return this->filter_out;                                   // �����˲������
}

void struct_average_filter_param_init(struct_average_filter *this)
{
    this->init = struct_average_filter_init;
    this->calc = struct_average_filter_calc;
}

void struct_fir_filter_init(struct_fir_filter *this)
{
    // �����˲����ĳ���
    for (int i = 0; i < FIR_FILTER_LENGTH; i++)
    {
        // ���˲���ϵ�������뻺������ʼ��Ϊ0
        this->coeffs[i] = 0.0f;
        this->buffer[i] = 0.0f;
    }
    // ����ǰ������������ʼ��Ϊ0
    this->index = 0;
}

void struct_fir_filter_set_coeffs(struct_fir_filter *this, const float *coeffs)
{
    // �����˲����ĳ���
    for (int i = 0; i < FIR_FILTER_LENGTH; i++)
    {
        // �������ϵ�����õ��˲���ϵ��������
        this->coeffs[i] = coeffs[i];
    }
}

float struct_fir_filter_calculate(struct_fir_filter *this, float input)
{
    // ���µ�����ֵ�洢���������ĵ�ǰ����λ��
    this->buffer[this->index] = input;
    float output = 0.0f;
    // ������
    for (int i = 0; i < FIR_FILTER_LENGTH; i++)
    {
        // ���˲���ϵ���ͻ������Ķ�Ӧֵ��ˣ�Ȼ���ۼӵ������
        output += this->coeffs[i] * this->buffer[(this->index + i) % FIR_FILTER_LENGTH];
    }
    // ���»���������
    this->index = (this->index + 1) % FIR_FILTER_LENGTH;
    // �����˲��������
    return output;
}

