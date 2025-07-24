#ifndef __CHASSIS_CONTROL_H
#define __CHASSIS_CONTROL_H

#include "zf_common_headfile.h"
#include "PID.h"
#include "Encoder.h"

typedef enum{
    Differential_Wheel,
    Mecanum_Wheel,
    None,
}ChassisMode_enum;


#define Chassis_mode Differential_Wheel

typedef struct{
    
    float vx_set;
    float angle_set;
    int16_t *motor_encoder[2]; 
    pid_type_def *motor_speed_pid[2];

}Differential_Wheel_Info_t; 

extern float motor1_speed_pid_kp = 2;
extern float motor1_speed_pid_ki = 0;
extern float motor1_speed_pid_kd = 0; 
extern float motor2_speed_pid_kp = 2;
extern float motor2_speed_pid_ki = 0;
extern float motor2_speed_pid_kd = 0; 
extern Differential_Wheel_Info_t *Differential_Wheel;


void Chassis_Init(Differential_Wheel_Info_t *_Chassis_Init);
void Chassis_Proceed(Differential_Wheel_Info_t *_Chassis_Proceed);

#endif