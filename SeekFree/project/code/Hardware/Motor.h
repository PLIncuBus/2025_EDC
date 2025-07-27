#ifndef __MOTOR_H
#define __MOTOR_H


#include "zf_common_headfile.h"

#define MOTOR2_DIR_PIN               (B9 )
#define MOTOR2_PWM               (PWM_TIM_A0_CH0_B8 )

#define MOTOR1_DIR_PIN               (B13 )
#define MOTOR1_PWM               (PWM_TIM_A0_CH1_B12)

#define Motor1_DIR								1
#define Motor2_DIR								1


void Motor_Init(void);
void Motor_Cmd(int16_t motor1,int16_t motor2);




#endif