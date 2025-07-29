#ifndef __STEPMOTOR_H
#define __STEPMOTOR_H


#include "zf_common_headfile.h"


#define StepMotor_UART_INDEX            (UART_3) 
#define StepMotor_UART_BAUNDRATE        (115200)
#define StepMotor_UART_TX_PIN           (UART3_TX_A26)
#define StepMotor_UART_RX_PIN           (UART3_RX_A25)
#define StepMotor_UART_PRIORITY         (UART3_INT_IRQn)

void StepMotor_SetPosition(uint8_t Motor_Num,uint32_t Pulse, int16_t Speed, uint8_t Acc);
void StepMotor_Init(void);
void StepMotor_Send(void);




#endif