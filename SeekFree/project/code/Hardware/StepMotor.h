#ifndef __STEPMOTOR_H
#define __STEPMOTOR_H


#include "zf_common_headfile.h"


#define StepMotor_UART_INDEX            (UART_3) 
#define StepMotor_UART_BAUNDRATE        (115200)
#define StepMotor_UART_TX_PIN           (UART3_TX_A26)
#define StepMotor_UART_RX_PIN           (UART3_RX_A25)
#define StepMotor_UART_PRIORITY         (UART3_INT_IRQn)
typedef struct GIMBAL_DATA
{
	float Current_Yaw;
	float Current_Pitch;
	float Target_Yaw;
	float Target_Pitch;
	
}Gimbal_Data;
extern Gimbal_Data gimbal_data;
void StepMotor_SetPosition(uint8_t Motor_Num, float Angle, int16_t Speed, uint8_t Acc);
void StepMotor_Init(void);
void StepMotor_Send(void);
void Gimbal_Set_Speed(int16_t Yaw ,int16_t Pitch );
void  Gimbal_Set_Angle(float Yaw ,float Pitch );
void save_gimbal_to_flash();
void load_gimbal_from_flash();


#endif