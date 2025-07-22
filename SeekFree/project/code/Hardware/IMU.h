#ifndef __IMU_H
#define __IMU_H


#include "zf_common_headfile.h"
#include "MyMath.h"
#include "Menu.h"
#include <math.h>

typedef enum{
    imu963ra,
    jy61p,
}IMU_Type_enum;

#define IMU_Type jy61p

#if(IMU_Type == jy61p)

#define JY61P_UART_INDEX              (UART_0)
#define JY61P_UART_BAUNDRATE          (115200)
#define JY61P_UART_TX_PIN             (UART0_TX_A10)
#define JY61P_UART_RX_PIN             (UART0_RX_A11)
#define JY61P_UART_PRIORITY           (UART0_INT_IRQn)

#define WIT_DATA_SUM          11
#define WIT_HEADER            0x55
#define WIT_ANGLE_HEADER      0x53

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



void IMU_Init(uint8_t Timer , uint16_t Offset_Time);
void IMU_Attitude_Process(void);



#endif