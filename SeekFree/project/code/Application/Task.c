/*
 * @Author: PLIncuBus wewean@yeah.net
 * @Date: 2025-07-30 08:49:14
 * @LastEditors: PLIncuBus wewean@yeah.net
 * @LastEditTime: 2025-07-30 17:52:02
 * @FilePath: \2025_EDC\SeekFree\project\code\Application\Task.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "Task.h"

/*****Task1 BEGIN*****/
float Task1_Loop_Num;
void Task1_Process(void)
{
    static uint8_t loop;
	Differential_Wheel_Info.vx_set = 8;
		if(Encoder_count_sum[Encoder1] > 2000 && (int)(Angle_Yaw < 90)){
			loop ++;
		}
		if(loop >= Task1_Loop_Num){
			Differential_Wheel_Info.mode = stop;
		}		
}
/*****Task1 END*****/


/*****Task2 BEGIN*****/
//自动校准
void Task2_Auto_Cal(void)
{
	StepMotor_Control.mode = StepMotor_Control_Cal_mode;
}
//手动校准
void Task2_Mannual_Cal(void)
{
	StepMotor_Control.mode = StepMotor_Control_set_mode;
}
void Task2_Process(void)
{
	Laser(0);
}
/*****Task2 END*****/




/*****Task3 BEGIN*****/
void Task3_Process(void)
{

}
/*****Task3 BEGIN*****/


/*****Task4 BEGIN*****/
void Task4_Process(void)
{

}
/*****Task4 BEGIN*****/