#include "Chassis_Control.h"

float motor1_speed_pid_kp = 2;
float motor1_speed_pid_ki = 0;
float motor1_speed_pid_kd = 0; 
float motor2_speed_pid_kp = 2;
float motor2_speed_pid_ki = 0;
float motor2_speed_pid_kd = 0; 


Differential_Wheel_Info_t *Differential_Wheel;


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
        PID_init(&_Chassis_Init->motor_speed_pid[i], PID_POSITION, motor_speed_pid[i],motor_speed_pid_max_out,);
    }

}


/**
 * @brief Chassis数据更新
 * 
 * @param _Chassis_Update 
 */
static void Chassis_Update(Differential_Wheel_Info_t *_Chassis_Update)
{
    _Chassis_Update->motor_encoder[0] = Encoder_Count_Get(Encoder1);
    _Chassis_Update->motor_encoder[1] = Encoder_Count_Get(Encoder2);

}

/**
 * @brief PID控制
 * 
 * @param _Chassis_Control_Loop 
 */
static void Chassis_Control_Loop(Differential_Wheel_Info_t *_Chassis_Control_Loop)
{

    _Chassis_Control_Loop->motor_speed_pid[0]->Kp = motor1_speed_pid_kp;
    _Chassis_Control_Loop->motor_speed_pid[0]->Ki = motor1_speed_pid_ki;
    _Chassis_Control_Loop->motor_speed_pid[0]->Kd = motor1_speed_pid_kd;
    _Chassis_Control_Loop->motor_speed_pid[1]->Kp = motor1_speed_pid_kp;
    _Chassis_Control_Loop->motor_speed_pid[1]->Ki = motor1_speed_pid_ki;
    _Chassis_Control_Loop->motor_speed_pid[1]->Kd = motor1_speed_pid_kd;

    for(uint8_t i = 0 ;i < 2; i ++ )
    {
        PID_calc(&_Chassis_Control_Loop->motor_speed_pid[i],(float)_Chassis_Control_Loop->motor_encoder[i],_Chassis_Control_Loop->vx_set);
    }
    
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