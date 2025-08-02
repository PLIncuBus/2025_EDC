#ifndef __STEPMOTOR_CONTROL_H
#define __STEPMOTOR_CONTROL_H


#include "zf_common_headfile.h"
#include "PID.h"
#include "UpperMonitor.h"
#include "StepMotor_Control.h"
#include "IMU.h"

#define StepMotor_Initial_Mode StepMotor_Control_Null_mode



extern float yaw_speed_pid_kp  ;
extern float yaw_speed_pid_ki ; 
extern float yaw_speed_pid_kd  ;
extern float pitch_speed_pid_kp ;
extern float pitch_speed_pid_ki; 
extern float pitch_speed_pid_kd ;
typedef enum{
    StepMotor_Control_set_mode,
    StepMotor_Control_Vision_mode,
    StepMotor_Control_Cal_mode,
		StepMotor_Control_Auto_Aim_mode,
		StepMotor_Control_Stop_mode,
		StepMotor_Control_Null_mode,
	StepMotor_Control_fix_mode
}StepMotor_Control_enum;


typedef struct {
    uint16_t Vision_Target[2];
		uint16_t Vision_Big_Target[2];
    uint16_t Vision_Now[2];
    float out[2];
    pid_type_def speed_pid[2];
		pid_type_def speed_inner_pid[2];
		pid_type_def angle_inner_pid[2];
    StepMotor_Control_enum mode;
		uint16_t dis;
    
}StepMotor_Control_Info_t;

extern int8_t Yaw_dir;
extern StepMotor_Control_Info_t StepMotor_Control;
extern int16_t StepMotor_Pos_Yaw_set,StepMotor_Pos_Pitch_set;
extern float WZ_yaw,WZ_pitch;
void StepMotor_Control_Init(StepMotor_Control_Info_t *_StepMotor_Control_Init);
void StepMotor_Control_Proceed(StepMotor_Control_Info_t *_StepMotor_Control_Proceed);
void StepMotor_Control_Loop(StepMotor_Control_Info_t *_StepMotor_Control_Loop);

#endif