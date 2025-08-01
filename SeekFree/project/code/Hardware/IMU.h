#ifndef __IMU_H
#define __IMU_H

#include "zf_common_headfile.h"
#include "MyMath.h"
#include "Menu.h"
#include <math.h>

typedef enum{
    imu963ra = 0 ,
    jy61p = 1,
}IMU_Type_enum;


#define IMU_Type 1
#define JY61P_Mode 1
#if(IMU_Type == 1)



struct Angle
{
	short Angle[3];
	short T;
};

#define JY61P_SOFT_IIC_DELAY          (100)                      
#define JY61P_SCL_PIN                 (A1)
#define JY61P_SDA_PIN							    (A0)
#define JY61P_DEV_ADDR                (0x50) 
#define JY61P2_DEV_ADDR								(0x49)
#define JY61P_ANGLE_ADDR							(0x3F)
#define JY61P_PITCH_ADDR							(0x3E)
#define JY61P_UART_INDEX              (UART_1)
#define JY61P_UART_BAUNDRATE          (115200)
#define JY61P_UART_TX_PIN             (UART1_TX_A8)
#define JY61P_UART_RX_PIN             (UART1_RX_A9)
#define JY61P_UART_PRIORITY           (UART1_INT_IRQn)

#define WIT_DATA_SUM          11
#define WIT_HEADER            0x55
#define WIT_ANGLE_HEADER      0x53

#define IMU_Init             JY61P_Init
#define IMU_Attitude_Process JY61P_Analysis_Process



#endif


extern float Acc_x;
extern float Acc_y;
extern float Acc_z;
extern float Gyro_x;
extern float Gyro_y;
extern float Gyro_z;
extern float Gyro_z_v;
extern float GyroOffsetZdata ;
extern float pitch;
extern float roll;
extern float Angle_Yaw;
extern float Gimbal_Angle_Yaw;
extern float Gimbal_Angle_Pitch;


void IMU_Init(uint8_t Timer , uint16_t Offset_Time);
void IMU_Attitude_Process(void);



#endif