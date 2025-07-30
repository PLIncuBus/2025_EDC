/*
 * @Author: PLIncuBus wewean@yeah.net
 * @Date: 2025-07-25 19:25:11
 * @LastEditors: PLIncuBus wewean@yeah.net
 * @LastEditTime: 2025-07-29 01:36:49
 * @FilePath: \2025_EDC\SeekFree\project\code\Application\UpperMonitor.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __UPPERMONITOR_H
#define __UPPERMONITOR_H

#include "zf_common_headfile.h"
#include "Chassis_Control.h"
#include "IMU.h"


//-0-VOFA   -1
#define UpperMonitor_Mode 0
#define UpperMonitor_UART_INDEX             (UART_0)         
#define UpperMonitor_UART_BAUNDRATE         (115200)
#define UpperMonitor_UART_TX_PIN            (UART0_TX_A0)
#define UpperMonitor_UART_RX_PIN            (UART0_RX_A1)
#define UpperMonitor_UART_PRIORITY          (UART0_INT_IRQn)

#define VisionMonitor_UART_INDEX            (UART_2) 
#define VisionMonitor_UART_BAUNDRATE        (115200)
#define VisionMonitor_UART_TX_PIN           (UART2_TX_B15)
#define VisionMonitor_UART_RX_PIN           (UART2_RX_B16)
#define VisionMonitor_UART_PRIORITY         (UART2_INT_IRQn)

/**
 * @brief           上位机发送拼接函数
 * @param           //Send_Theme发送主题
 */

#define UpperMonitor_Motor_PID_Loop_Def     Single_Velocity_Loop


typedef enum{
    Single_Velocity_Loop,
    Double_Inner_Loop,
    Double_Out_Loop,
}UpperMonitor_Motor_PID_Loop_enum;


typedef struct 
{
    uint8_t Usart_Send_Buff[100];
    uint8_t Usart_Receive_Buff[100];
    uint8_t Usart_Send_Index;

}UpperMonitor_Handle_Typedef;

extern uint16_t RxData ;
extern uint16_t Vision_values[6];
extern UpperMonitor_Handle_Typedef UpperMonitor_Handle;
extern uint8_t Vision_RxPacket[64]; // 定义接收数据包数组
void UpperMonitor_Init(void);
void UpperMonitor_Motor_PID_Send_Hook(UpperMonitor_Handle_Typedef *UpperMonitor_Handle_Motor_PID_Hook);
void UpperMonitor_Cmd_Send(UpperMonitor_Handle_Typedef *UpperMonitor_Cmd_Send);
void VisionMonitor_parse_rect_data(const char *data);
#endif