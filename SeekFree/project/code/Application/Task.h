#ifndef __TASK_H
#define __TASK_H

#include "zf_common_headfile.h"
#include "Chassis_Control.h"
#include "StepMotor_Control.h"
#include "Laser.h"
#include "Motor.h"


#define Task2_Error_Deadline 16
#define Task3_Error_Deadline 16

extern float Task1_Loop_Num;
extern float Task4_Loop_Num;
extern uint8_t Task1_flag;
extern uint8_t Task2_flag;
extern uint8_t Task3_flag;
extern uint8_t Task4_flag;
void Task1_Process(void);
void Task2_Process(void);
void Task2_Mannual_Cal(void);
void Task2_Auto_Cal(void);

void Task3_Process(void);

void Task4_Process(void);




#endif