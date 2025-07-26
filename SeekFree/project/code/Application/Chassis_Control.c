/*
 * @Author: PLIncuBus wewean@yeah.net
 * @Date: 2025-07-21 20:19:06
 * @LastEditors: PLIncuBus wewean@yeah.net
 * @LastEditTime: 2025-07-25 16:50:10
 * @FilePath: \SeekFree\project\code\Application\Chassis_Control.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "Chassis_Control.h"

float motor1_speed_pid_kp = 1.25;
float motor1_speed_pid_ki = 0.45;
float motor1_speed_pid_kd = 0; 
float motor2_speed_pid_kp = 1.25;
float motor2_speed_pid_ki = 0.45;
float motor2_speed_pid_kd = 0; 


Differential_Wheel_Info_t Differential_Wheel_Info;


/**
 * @brief Chassis初始化
 * 
 */
void Chassis_Init(Differential_Wheel_Info_t *_Chassis_Init)
{
    //PID
    static float motor_speed_pid[2][3];
    motor_speed_pid[0][0] = motor1_speed_pid_kp;
    motor_speed_pid[0][1] = motor1_speed_pid_ki;
    motor_speed_pid[0][2] = motor1_speed_pid_kd;  
    motor_speed_pid[1][0] = motor1_speed_pid_kp;
    motor_speed_pid[1][1] = motor1_speed_pid_ki;
    motor_speed_pid[1][2] = motor1_speed_pid_kd;  

    //PID限幅
    const float motor_speed_pid_max_out = 60;
    const float motor_speed_pid_max_iout = 60;


    for(uint8_t i = 0; i < 2;i ++){
        PID_init(&_Chassis_Init->motor_speed_pid[i], PID_DELTA, motor_speed_pid[i],motor_speed_pid_max_out,motor_speed_pid_max_iout);
    }

}


/**
 * @brief Chassis数据更新
 * 
 * @param _Chassis_Update 
 */
static void Chassis_Update(Differential_Wheel_Info_t *_Chassis_Update)
{
    _Chassis_Update->motor_encoder[0] = (float)Encoder_Count_Get(Encoder1);
    _Chassis_Update->motor_encoder[1] = (float)Encoder_Count_Get(Encoder2);
		Encoder_count[0] = 0;
		Encoder_count[1] = 0;
		
	

}

/**
 * @brief PID控制
 * 
 * @param _Chassis_Control_Loop 
 */
static void Chassis_Control_Loop(Differential_Wheel_Info_t *_Chassis_Control_Loop)
{
//		Chassis_Init(_Chassis_Control_Loop);

    for(uint8_t i = 0 ;i < 2; i ++ )
    {
        PID_calc(&_Chassis_Control_Loop->motor_speed_pid[i],(float)_Chassis_Control_Loop->motor_encoder[i],_Chassis_Control_Loop->vx_set);		
		}
		Motor_Cmd(_Chassis_Control_Loop->motor_speed_pid[0].out,_Chassis_Control_Loop->motor_speed_pid[1].out);//
    
}
/**
 * @brief Chassis处理
 * 
 * @param _Chassis_Proceed 
 */
void Chassis_Proceed(Differential_Wheel_Info_t *_Chassis_Proceed)
{
    Chassis_Update(_Chassis_Proceed);
    Chassis_Control_Loop(_Chassis_Proceed);
}