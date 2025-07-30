#ifndef __TASK_H
#define __TASK_H

#include "zf_common_headfile.h"
#include "Chassis_Control.h"
#include "StepMotor_Control.h"
#include "Laser.h"
extern float Task1_Loop_Num;

void Task1_Process(void);

void Task2_Process(void);
void Task2_Mannual_Cal(void);
void Task2_Auto_Cal(void);

void Task3_Process(void);

void Task4_Process(void);




#endif