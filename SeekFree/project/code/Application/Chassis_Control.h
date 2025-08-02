#ifndef __CHASSIS_CONTROL_H
#define __CHASSIS_CONTROL_H

#include "zf_common_headfile.h"
#include "PID.h"
#include "Encoder.h"
#include "Motor.h"
#include "IMU.h"
#include "phototube.h"

typedef enum{
    Differential_Wheel,
    Mecanum_Wheel,
    None,
}ChassisMode_enum;

typedef enum{
	track,
	angle,
	stop,
	slow_angle,
	
}Chassis_mode_enum;

#define DIFFRERENTIAL_WHEEL_BASE 1.18
#define Chassis_mode Differential_Wheel

typedef struct{
    
    float vx_set;
		float vz_set;
		float target[2];
    float angle_set;
    int16_t motor_encoder[2]; 
		int32_t motor_encoder_sum[2];
    pid_type_def motor_speed_pid[2];
    pid_type_def motor_angle_pid;
		pid_type_def motor_tube_pid;
		Chassis_mode_enum mode;

}Differential_Wheel_Info_t; 

extern float motor1_speed_pid_kp;
extern float motor1_speed_pid_ki;
extern float motor1_speed_pid_kd; 
extern float motor2_speed_pid_kp;
extern float motor2_speed_pid_ki;
extern float motor2_speed_pid_kd; 
extern Differential_Wheel_Info_t Differential_Wheel_Info;

void Chassis_Kinematics_Solve(Differential_Wheel_Info_t *_Chassis_Kinematics_Solve);

void Chassis_Init(Differential_Wheel_Info_t *_Chassis_Init);
void Chassis_Proceed(Differential_Wheel_Info_t *_Chassis_Proceed);

#endif