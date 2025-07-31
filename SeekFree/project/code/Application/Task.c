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
uint8_t wait;
uint8_t Task1_flag = 0;
uint8_t Task2_flag = 0;
uint8_t Task3_flag = 0;
uint8_t Task4_flag = 0;
void Task1_Process(void)
{
		Task1_flag = 1;	


}
/*****Task1 END*****/


/*****Task2 BEGIN*****/
//自动校准
void Task2_Auto_Cal(void)
{
	StepMotor_Control.mode = StepMotor_Control_Cal_mode;
	Differential_Wheel_Info.mode = stop;
}
//手动校准
void Task2_Mannual_Cal(void)
{
	StepMotor_Control.mode = StepMotor_Control_set_mode;
	Differential_Wheel_Info.mode = stop;
}
//Process
#define Task2_Error_Deadline 10
void Task2_Process(void)
{
	StepMotor_Control.mode = StepMotor_Control_Cal_mode;
	Differential_Wheel_Info.mode = stop;
	if(abs((int)StepMotor_Control.speed_pid[0].error) < Task2_Error_Deadline && abs((int)StepMotor_Control.speed_pid[1].error) < Task2_Error_Deadline ){
		Laser(0);
	}
	
}
/*****Task2 END*****/



#define Task3_Error_Deadline 10
/*****Task3 BEGIN*****/
void Task3_Process(void)
{
	StepMotor_Control.mode = StepMotor_Control_Auto_Aim_mode;
	Differential_Wheel_Info.mode = stop;
	if(abs((int)StepMotor_Control.speed_pid[0].error) < Task3_Error_Deadline && abs((int)StepMotor_Control.speed_pid[1].error) < Task3_Error_Deadline ){
		Laser(0);
	}
}
/*****Task3 BEGIN*****/


/*****Task4 BEGIN*****/
void Task4_Process(void)
{

}
/*****Task4 BEGIN*****/