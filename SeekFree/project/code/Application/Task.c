#include "Task.h"

float Task1_Loop_Num;
void Task1_Process(void)
{
    static uint8_t loop;
		if(1){
			loop ++;
		}
		if(loop >= Task1_Loop_Num){
			Differential_Wheel_Info.mode = stop;
		}

		
}
void Task2_Process(void)
{
//    Menu_ShowStr(0,0,"Task2");
}
void Task3_Process(void)
{
//    Menu_ShowStr(0,0,"Task3");
}
void Task4_Process(void)
{
//    Menu_ShowStr(0,0,"Task4");
}