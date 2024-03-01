/**
  ***************************************************************************
  * @file       remote_control.c/h
  * @brief      ң����������ң������ͨ������SBUS��Э�鴫�䣬����DMA���䷽ʽ��ԼCPU
  *             ��Դ�����ô��ڿ����ж�������������
  *
  *
  ***************************************************************************
  */
#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H
#include "struct_typedef.h"
#include "bsp_rc.h"

#define SBUS_RX_BUF_NUM 36u

#define RC_FRAME_LENGTH 18u

#define RC_CH_VALUE_MIN ((uint16_t)364)
#define RC_CH_VALUE_OFFSET ((uint16_t)1024)
#define RC_CH_VALUE_MAX ((uint16_t)1684)

/* ----------------------- RC Switch Definition----------------------------- */
#define RC_SW_UP ((uint16_t)1)
#define RC_SW_MID ((uint16_t)3)
#define RC_SW_DOWN ((uint16_t)2)
#define switch_is_down(s) (s == RC_SW_DOWN)
#define switch_is_mid(s) (s == RC_SW_MID)
#define switch_is_up(s) (s == RC_SW_UP)
/* ----------------------- PC Key Definition-------------------------------- */
#define KEY_PRESSED_OFFSET_W ((uint16_t)1 << 0)
#define KEY_PRESSED_OFFSET_S ((uint16_t)1 << 1)
#define KEY_PRESSED_OFFSET_A ((uint16_t)1 << 2)
#define KEY_PRESSED_OFFSET_D ((uint16_t)1 << 3)
#define KEY_PRESSED_OFFSET_SHIFT ((uint16_t)1 << 4)
#define KEY_PRESSED_OFFSET_CTRL ((uint16_t)1 << 5)
#define KEY_PRESSED_OFFSET_Q ((uint16_t)1 << 6)
#define KEY_PRESSED_OFFSET_E ((uint16_t)1 << 7)
#define KEY_PRESSED_OFFSET_R ((uint16_t)1 << 8)
#define KEY_PRESSED_OFFSET_F ((uint16_t)1 << 9)
#define KEY_PRESSED_OFFSET_G ((uint16_t)1 << 10)
#define KEY_PRESSED_OFFSET_Z ((uint16_t)1 << 11)
#define KEY_PRESSED_OFFSET_X ((uint16_t)1 << 12)
#define KEY_PRESSED_OFFSET_C ((uint16_t)1 << 13)
#define KEY_PRESSED_OFFSET_V ((uint16_t)1 << 14)
#define KEY_PRESSED_OFFSET_B ((uint16_t)1 << 15)
/* ----------------------- Data Struct ------------------------------------- */
/**
 * @brief 
 * ch[0]:��ҡ��ˮƽ�г� ��->��:-660~+660
 * ch1:��ҡ�˴�ֱ�г� ��->��:-660~+660
 * ch2:��ҡ��ˮƽ�г� ��->��:-660~+660
 * 
 * ch3:��ҡ�˴�ֱ�г� ��->��:-660~+660
 * ch4:�����ť ��->��:-660~+660
 * 
 * s1:���ϲ��� 3:�м� 1:�� 2:��
 * s0:���ϲ��� 3:�м� 1:�� 2:��
 * 
 */
typedef struct
{
        struct
        {
                int16_t ch[5];
                char s[2];
        } __attribute__((__packed__)) rc;
        struct
        {
                int16_t x;
                int16_t y;
                int16_t z;
                uint8_t press_l;
                uint8_t press_r;
        } __attribute__((__packed__)) mouse;
        struct
        {
                uint16_t v;
        } __attribute__((__packed__)) key;

} __attribute__((__packed__)) RC_ctrl_t;

/* ----------------------- Internal Data ----------------------------------- */

/**
 * @brief          ң������ʼ��
 * @param[in]      none
 * @retval         none
 */
void remote_control_init(void);

/**
 * @brief          ��ȡң��������ָ��
 * @param[in]      none
 * @retval         ң��������ָ��
 */
extern const RC_ctrl_t *get_remote_control_point(void);
extern RC_ctrl_t rc_ctrl;

#endif