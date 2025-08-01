#include "StepMotor_Control.h"
StepMotor_Control_Info_t StepMotor_Control;
int16_t StepMotor_Pos_Yaw_set,StepMotor_Pos_Pitch_set;
float yaw_speed_pid_kp = 0.08;
float yaw_speed_pid_ki = 0;
float yaw_speed_pid_kd = 0; 
float pitch_speed_pid_kp = 0.05;
float pitch_speed_pid_ki = 0;
float pitch_speed_pid_kd = 0;
float inner_yaw_speed_pid_kp = 1.6;
float inner_yaw_speed_pid_ki = 0;
float inner_yaw_speed_pid_kd = 0; 
float inner_pitch_speed_pid_kp = 1.2;
float inner_pitch_speed_pid_ki = 0;
float inner_pitch_speed_pid_kd = 0;
float Laser_Vision_Pos[2] = {388,236};



void StepMotor_Control_Init(StepMotor_Control_Info_t *_StepMotor_Control_Init)
{
    float StepMotor_speed_pid[2][3];
		float StepMotor_inner_speed_pid[2][3];
	
	
    StepMotor_speed_pid[0][0] =  yaw_speed_pid_kp;
    StepMotor_speed_pid[0][1] =  yaw_speed_pid_ki;
    StepMotor_speed_pid[0][2] =  yaw_speed_pid_kd;  
    StepMotor_speed_pid[1][0] =  pitch_speed_pid_kp;
    StepMotor_speed_pid[1][1] =  pitch_speed_pid_ki;
    StepMotor_speed_pid[1][2] =  pitch_speed_pid_kd; 


		StepMotor_inner_speed_pid[0][0] = inner_yaw_speed_pid_kp;
    StepMotor_inner_speed_pid[0][1] =  inner_yaw_speed_pid_ki;
    StepMotor_inner_speed_pid[0][2] =  inner_yaw_speed_pid_kd;  
    StepMotor_inner_speed_pid[1][0] =  inner_pitch_speed_pid_kp;
    StepMotor_inner_speed_pid[1][1] =  inner_pitch_speed_pid_ki;
    StepMotor_inner_speed_pid[1][2] =  inner_pitch_speed_pid_kd; 

    //PID限幅
    const float StepMotor_speed_pid_max_out = 20;
    const float StepMotor_speed_pid_max_iout = 60;
		
		//PID限幅
    const float StepMotor_inner_speed_pid_max_out = 50;
    const float StepMotor_inner_speed_pid_max_iout = 50;

    for(uint8_t i = 0; i < 2;i ++){
        PID_init(&_StepMotor_Control_Init->speed_pid[i], PID_POSITION, StepMotor_speed_pid[i],StepMotor_speed_pid_max_out,StepMotor_speed_pid_max_iout);
		    PID_init(&_StepMotor_Control_Init->speed_inner_pid[i], PID_POSITION, StepMotor_inner_speed_pid[i],StepMotor_inner_speed_pid_max_out,StepMotor_inner_speed_pid_max_iout);

    }
}
static void StepMotor_Update(StepMotor_Control_Info_t *_StepMotor_Update)
{
		static uint16_t Last_Vision_Target[2]; 
		static uint16_t Last_Vision_Big_Target[2];
		static StepMotor_Control_enum Last_mode = StepMotor_Initial_Mode;
		static StepMotor_Control_enum Last_Keep_mode = StepMotor_Control_Null_mode;//Cal
	
	
		VisionMonitor_parse_rect_data((char*)Vision_RxPacket);
		//values 赋值
		StepMotor_Control.Vision_Big_Target[0] = Vision_values[0];
		StepMotor_Control.Vision_Big_Target[1] = Vision_values[1];
		StepMotor_Control.Vision_Target[0]     = Vision_values[2];
		StepMotor_Control.Vision_Target[1]     = Vision_values[3];
		StepMotor_Control.Vision_Now[0]        = Vision_values[4];
		StepMotor_Control.Vision_Now[1]        = Vision_values[5];
		
	
//		if((StepMotor_Control.Vision_Big_Target[0] == 0 || StepMotor_Control.Vision_Big_Target[1] == 0 ) && StepMotor_Control.mode != StepMotor_Control_Auto_Aim_mode){
//			StepMotor_Control.mode = StepMotor_Control_Stop_mode;
//		}else if(Last_mode == StepMotor_Control_Stop_mode && StepMotor_Control.mode != StepMotor_Control_Auto_Aim_mode){
//			StepMotor_Control.mode = Last_Keep_mode;
//		}
//		else {
//		}

//		
//		for(uint8_t j = 0;j < 2; j ++){
//		Last_Vision_Target[j] = StepMotor_Control.Vision_Target[j];
//		Last_Vision_Big_Target[j] = StepMotor_Control.Vision_Big_Target[j];
//		}
//		Last_mode = StepMotor_Control.mode;
//		if(StepMotor_Control.mode != StepMotor_Control_Stop_mode){
//				Last_Keep_mode = StepMotor_Control.mode;
//		}
	
}

void StepMotor_Control_Loop(StepMotor_Control_Info_t *_StepMotor_Control_Loop)
{
//	if(_StepMotor_Control_Loop->mode == StepMotor_Control_Auto_Aim_mode){
////		_StepMotor_Control_Loop->speed_pid[0].Kp = 0.5;
////		Gimbal_Set_Speed(10,0);if(_StepMotor_Control_Loop->Vision_Big_Target[0] != 0 && _StepMotor_Control_Loop->Vision_Big_Target[1] != 0 ){
////			static uint8_t loop;
////			if(loop > 5){
////			Gimbal_Set_Speed(0,0);
////				loop = 0;
////			_StepMotor_Control_Loop->mode = StepMotor_Control_Cal_mode;
////				_StepMotor_Control_Loop->speed_pid[0].Kp = yaw_speed_pid_kp;
////			}
////		}
////		Gimbal_Set_Angle(0,0);
//		_StepMotor_Control_Loop->mode = StepMotor_Control_Cal_mode;
//	}
		if(_StepMotor_Control_Loop->mode == StepMotor_Control_Auto_Aim_mode){

			if(abs(Gimbal_Angle_Yaw) < 5 && abs(Gimbal_Angle_Yaw) < 5){
				_StepMotor_Control_Loop->mode = StepMotor_Control_Cal_mode;
			
			}
					PID_calc(&_StepMotor_Control_Loop->speed_inner_pid[0],(float)Gimbal_Angle_Yaw,0);
		PID_calc(&_StepMotor_Control_Loop->speed_inner_pid[1],(float)Gimbal_Angle_Pitch,0);
		Gimbal_Set_Speed(-_StepMotor_Control_Loop->speed_inner_pid[0].out,_StepMotor_Control_Loop->speed_inner_pid[1].out);

		}
	
	
else if(_StepMotor_Control_Loop->mode == StepMotor_Control_set_mode){
		Gimbal_Set_Angle(StepMotor_Pos_Yaw_set,-StepMotor_Pos_Pitch_set);
	}
	else if(_StepMotor_Control_Loop->mode == StepMotor_Control_Cal_mode){
		static float temp[2];
		for(uint8_t i = 0 ;i < 2; i ++ )
    {
        	PID_calc(&_StepMotor_Control_Loop->speed_pid[i],(float)Laser_Vision_Pos[i],(float)_StepMotor_Control_Loop->Vision_Big_Target[i]);
					
		}
		temp[0] = (float)Gimbal_Angle_Yaw - (float)_StepMotor_Control_Loop->speed_pid[0].out;
		temp[1] = (float)Gimbal_Angle_Pitch - (float)_StepMotor_Control_Loop->speed_pid[1].out;

		PID_calc(&_StepMotor_Control_Loop->speed_inner_pid[0],(float)Gimbal_Angle_Yaw,temp[0]);
		PID_calc(&_StepMotor_Control_Loop->speed_inner_pid[1],(float)Gimbal_Angle_Pitch,temp[1]);
		Gimbal_Set_Speed(-_StepMotor_Control_Loop->speed_inner_pid[0].out,_StepMotor_Control_Loop->speed_inner_pid[1].out);

	}
	else if(_StepMotor_Control_Loop->mode == StepMotor_Control_Stop_mode){
//		Gimbal_Set_Speed(0,0);
//	}
//	else if(_StepMotor_Control_Loop->mode == StepMotor_Control_Vision_mode){    
//	for(uint8_t i = 0 ;i < 2; i ++ )
//  {
//      PID_calc(&_StepMotor_Control_Loop->speed_pid[i],(float)_StepMotor_Control_Loop->Vision_Now[i],(float)_StepMotor_Control_Loop->Vision_Big_Target[i]);		
//	}
//			Gimbal_Set_Speed(_StepMotor_Control_Loop->speed_pid[0].out,-_StepMotor_Control_Loop->speed_pid[1].out);
	}

}	





void StepMotor_Control_Proceed(StepMotor_Control_Info_t *_StepMotor_Control_Proceed)
{
    StepMotor_Update(_StepMotor_Control_Proceed);
    StepMotor_Control_Loop(_StepMotor_Control_Proceed);
}