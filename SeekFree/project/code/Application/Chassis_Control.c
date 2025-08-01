/*
 * @Author: PLIncuBus wewean@yeah.net
 * @Date: 2025-07-21 20:19:06
 * @LastEditors: PLIncuBus wewean@yeah.net
 * @LastEditTime: 2025-07-26 20:58:08
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
//float motor_angle_pid_kp = 0.8;
//float motor_angle_pid_ki = 0.08;
//float motor_angle_pid_kd = 2;

float motor_angle_pid_kp = 0.25;
float motor_angle_pid_ki = 0;
float motor_angle_pid_kd = 0;

float motor_tube_pid_kp = 12;
float motor_tube_pid_ki = 0;
float motor_tube_pid_kd = 0;

static void Chassis_Kinematics_Solve(Differential_Wheel_Info_t *_Chassis_Kinematics_Solve);
Differential_Wheel_Info_t Differential_Wheel_Info;


/**
 * @brief Chassis初始化
 * 
 */
void Chassis_Init(Differential_Wheel_Info_t *_Chassis_Init)
{
    //PID
    float motor_speed_pid[2][3];
    
    motor_speed_pid[0][0] = motor1_speed_pid_kp;
    motor_speed_pid[0][1] = motor1_speed_pid_ki;
    motor_speed_pid[0][2] = motor1_speed_pid_kd;  
    motor_speed_pid[1][0] = motor1_speed_pid_kp;
    motor_speed_pid[1][1] = motor1_speed_pid_ki;
    motor_speed_pid[1][2] = motor1_speed_pid_kd;  

    float motor_angle_pid[3];

    motor_angle_pid[0] = motor_angle_pid_kp;
    motor_angle_pid[1] = motor_angle_pid_ki;
    motor_angle_pid[2] = motor_angle_pid_kd;
	
	
		float motor_tube_pid[3];

    motor_tube_pid[0] = motor_tube_pid_kp;
    motor_tube_pid[1] = motor_tube_pid_ki;
    motor_tube_pid[2] = motor_tube_pid_kd;


    //PID限幅
    const float motor_speed_pid_max_out = 60;
    const float motor_speed_pid_max_iout = 60;

    const float motor_angle_pid_max_out = 40;
    const float motor_angle_pid_max_iout = 40;
		
		const float motor_tube_pid_max_out = 60;
    const float motor_tube_pid_max_iout = 60;

    PID_init(&_Chassis_Init->motor_angle_pid, PID_POSITION, motor_angle_pid,motor_angle_pid_max_out,motor_angle_pid_max_iout);
    PID_init(&_Chassis_Init->motor_tube_pid, PID_POSITION, motor_tube_pid,motor_tube_pid_max_out,motor_tube_pid_max_iout);

		for(uint8_t i = 0; i < 2;i ++){
        PID_init(&_Chassis_Init->motor_speed_pid[i], PID_POSITION, motor_speed_pid[i],motor_speed_pid_max_out,motor_speed_pid_max_iout);
		
    }

}


/**
 * @brief Chassis数据更新
 * 
 * @param _Chassis_Update 
 */
static void Chassis_Update(Differential_Wheel_Info_t *_Chassis_Update)
{
    //速度Update
    static float Last_Angle_Yaw ;
		static float Last_Angle_set ;
    _Chassis_Update->motor_encoder[0] = (float)Encoder_Count_Get(Encoder1);
    _Chassis_Update->motor_encoder[1] = (float)Encoder_Count_Get(Encoder2);
		Encoder_count[0] = 0;
		Encoder_count[1] = 0;
		_Chassis_Update->motor_encoder_sum[0] = Encoder_count_sum[Encoder1];
		_Chassis_Update->motor_encoder_sum[1] = Encoder_count_sum[Encoder2];
		

    
    //角度Update
//    if(Angle_Yaw -  Last_Angle_Yaw > 179 ){
//        Angle_Yaw -= 360; 
//    }
//    else if(Angle_Yaw - Last_Angle_Yaw < -179){
//        Angle_Yaw += 360;
//    }
//		if(Differential_Wheel_Info.angle_set - Last_Angle_set > 179){
//				Differential_Wheel_Info.angle_set -= 360;
//		}
//		else if(Differential_Wheel_Info.angle_set - Last_Angle_set < -179){
//				Differential_Wheel_Info.angle_set += 360;
//		}
    Last_Angle_Yaw = Angle_Yaw;
	
}

///**
// * @brief PID控制
// * 
// * @param _Chassis_Control_Loop 
// */
//static void Chassis_Control_Loop(Differential_Wheel_Info_t *_Chassis_Control_Loop)
//{
//		
//    _Chassis_Control_Loop->motor_speed_pid[0].Kp = motor1_speed_pid_kp;
//    _Chassis_Control_Loop->motor_speed_pid[0].Ki = motor1_speed_pid_ki;
//    _Chassis_Control_Loop->motor_speed_pid[0].Kd = motor1_speed_pid_kd;

//    _Chassis_Control_Loop->motor_speed_pid[1].Kp = motor1_speed_pid_kp;
//    _Chassis_Control_Loop->motor_speed_pid[1].Ki = motor1_speed_pid_ki;
//    _Chassis_Control_Loop->motor_speed_pid[1].Kd = motor1_speed_pid_kd;


//    _Chassis_Control_Loop->motor_angle_pid.Kp = motor_angle_pid_kp;
//    _Chassis_Control_Loop->motor_angle_pid.Ki = motor_angle_pid_ki;
//    _Chassis_Control_Loop->motor_angle_pid.Kd = motor_angle_pid_kd;
//	
//		if(_Chassis_Control_Loop->mode == track){
//			PID_calc(&_Chassis_Control_Loop->motor_tube_pid,Cha_error,0);
//			_Chassis_Control_Loop->vz_set = _Chassis_Control_Loop->motor_tube_pid.out;
//		}
//		else{
//		PID_calc(&_Chassis_Control_Loop->motor_angle_pid,(int)Angle_Yaw,_Chassis_Control_Loop->angle_set);
//		_Chassis_Control_Loop->vz_set = _Chassis_Control_Loop->motor_angle_pid.out;}
//	
//	
//		Chassis_Kinematics_Solve(_Chassis_Control_Loop);
//	
//    for(uint8_t i = 0 ;i < 2; i ++ )
//    {
//        PID_calc(&_Chassis_Control_Loop->motor_speed_pid[i],(float)_Chassis_Control_Loop->motor_encoder[i],_Chassis_Control_Loop->target[i]);		
//		}
//		if(_Chassis_Control_Loop->mode == stop){
//			Motor_Cmd(0,0);
//		}
//		else{
////			if(_Chassis_Control_Loop->motor_speed_pid[0].out > 20 )
////			{
////				_Chassis_Control_Loop->motor_speed_pid[0].out = 20;
////				
////			}
////			else if(_Chassis_Control_Loop->motor_speed_pid[0].out < -20)
////			{
////				_Chassis_Control_Loop->motor_speed_pid[0].mode = -20;
////			}
////			if(_Chassis_Control_Loop->motor_speed_pid[1].out > 20 )
////			{
////				_Chassis_Control_Loop->motor_speed_pid[1].out = 20;
////				
////			}
////			else if(_Chassis_Control_Loop->motor_speed_pid[1].out < -20)
////			{
////				_Chassis_Control_Loop->motor_speed_pid[1].mode = -20;
////			}
////		if(_Chassis_Control_Loop->mode == slow_angle)
////		{
////			static uint8_t j = 0;
////			j ++;
////			if(j < 2){
////				Motor_Cmd(10,10);
////			}
////			else {
////				static uint8_t k = 0;
////				k++;
////				if(k < 20){
////					Motor_Cmd(-10,10);
////				}
////				else{
////					_Chassis_Control_Loop->mode = track;
////					j = 0;
////					k = 0;
////				}
////			}

////				}
//				
//		

//    Motor_Cmd(_Chassis_Control_Loop->motor_speed_pid[0].out, _Chassis_Control_Loop->motor_speed_pid[1].out);
//	}
//	
////_Chassis_Control_Loop->motor_angle_pid.out + -	_Chassis_Control_Loop->motor_angle_pid.out +_Chassis_Control_Loop->motor_speed_pid[0].out
//    
//}
/**
 * @brief PID控制
 * 
 * @param _Chassis_Control_Loop 
 */
static void Chassis_Control_Loop(Differential_Wheel_Info_t *_Chassis_Control_Loop)
{
		
    _Chassis_Control_Loop->motor_speed_pid[0].Kp = motor1_speed_pid_kp;
    _Chassis_Control_Loop->motor_speed_pid[0].Ki = motor1_speed_pid_ki;
    _Chassis_Control_Loop->motor_speed_pid[0].Kd = motor1_speed_pid_kd;

    _Chassis_Control_Loop->motor_speed_pid[1].Kp = motor1_speed_pid_kp;
    _Chassis_Control_Loop->motor_speed_pid[1].Ki = motor1_speed_pid_ki;
    _Chassis_Control_Loop->motor_speed_pid[1].Kd = motor1_speed_pid_kd;


    _Chassis_Control_Loop->motor_angle_pid.Kp = motor_angle_pid_kp;
    _Chassis_Control_Loop->motor_angle_pid.Ki = motor_angle_pid_ki;
    _Chassis_Control_Loop->motor_angle_pid.Kd = motor_angle_pid_kd;
	
		if(_Chassis_Control_Loop->mode == track){
			PID_calc(&_Chassis_Control_Loop->motor_tube_pid,Cha_error,0);
			_Chassis_Control_Loop->vz_set = _Chassis_Control_Loop->motor_tube_pid.out;
		}
		else{
		PID_calc(&_Chassis_Control_Loop->motor_angle_pid,(int)Angle_Yaw,_Chassis_Control_Loop->angle_set);
		_Chassis_Control_Loop->vz_set = _Chassis_Control_Loop->motor_angle_pid.out;}
	
	
		Chassis_Kinematics_Solve(_Chassis_Control_Loop);
	
    for(uint8_t i = 0 ;i < 2; i ++ )
    {
        PID_calc(&_Chassis_Control_Loop->motor_speed_pid[i],(float)_Chassis_Control_Loop->motor_encoder[i],_Chassis_Control_Loop->target[i]);		
		}
		if(_Chassis_Control_Loop->mode == stop){
			Motor_Cmd(0,0);
		}
		else{
			if(_Chassis_Control_Loop->motor_speed_pid[0].out > 28 )
			{
				_Chassis_Control_Loop->motor_speed_pid[0].out = 28;
				
			}
			else if(_Chassis_Control_Loop->motor_speed_pid[0].out < -28)
			{
				_Chassis_Control_Loop->motor_speed_pid[0].out = -28;
			}
			if(_Chassis_Control_Loop->motor_speed_pid[1].out > 28 )
			{
				_Chassis_Control_Loop->motor_speed_pid[1].out = 28;
				
			}
			else if(_Chassis_Control_Loop->motor_speed_pid[1].out < -28)
			{
				_Chassis_Control_Loop->motor_speed_pid[1].out = -28;
			}
    Motor_Cmd(_Chassis_Control_Loop->motor_speed_pid[0].out, _Chassis_Control_Loop->motor_speed_pid[1].out);}
	
//_Chassis_Control_Loop->motor_angle_pid.out + -	_Chassis_Control_Loop->motor_angle_pid.out +_Chassis_Control_Loop->motor_speed_pid[0].out
    
}
/**
 * @brief Chassis底盘解算
 * 
 * @param _Chassis_Proceed 
 */
static void Chassis_Kinematics_Solve(Differential_Wheel_Info_t *_Chassis_Kinematics_Solve)
{
	_Chassis_Kinematics_Solve->target[0] = _Chassis_Kinematics_Solve->vx_set - _Chassis_Kinematics_Solve->vz_set*DIFFRERENTIAL_WHEEL_BASE/2;
	_Chassis_Kinematics_Solve->target[1] = _Chassis_Kinematics_Solve->vx_set + _Chassis_Kinematics_Solve->vz_set*DIFFRERENTIAL_WHEEL_BASE/2;	
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