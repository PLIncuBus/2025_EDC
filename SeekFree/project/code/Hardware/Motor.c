#include "Motor.h"

/**
 * @brief 电机初始化
 * 
 */
void Motor_Init(void)
{  
    //电机1
    gpio_init(MOTOR1_DIR_PIN, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            // GPIO 初始化为输出 默认上拉输出高
    pwm_init(MOTOR1_PWM, 17000, 0);                                                  // PWM 通道初始化频率 17KHz 占空比初始为 0
    
    //电机2
    gpio_init(MOTOR2_DIR_PIN, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            // GPIO 初始化为输出 默认上拉输出高
    pwm_init(MOTOR2_PWM, 17000, 0);                                                  // PWM 通道初始化频率 17KHz 占空比初始为 0
}

/**
 * @brief 电机指令
 * 
 */
void Motor_Cmd(int16_t motor1,int16_t motor2)
{
	
		if (motor1 >= 0) {
				gpio_set_level(MOTOR1_DIR_PIN,Motor1_DIR);
				pwm_set_duty(MOTOR1_PWM, motor1 * (PWM_DUTY_MAX / 100)); 
		}
		else if(motor1 < 0){
				gpio_set_level(MOTOR1_DIR_PIN,!Motor1_DIR);
				pwm_set_duty(MOTOR1_PWM, -motor1 * (PWM_DUTY_MAX / 100));
		}
		
		if	(motor2 >= 0){
				gpio_set_level(MOTOR2_DIR_PIN,Motor2_DIR);
				pwm_set_duty(MOTOR2_PWM, motor2 * (PWM_DUTY_MAX / 100));
		}
		else if(motor2 < 0){
				gpio_set_level(MOTOR2_DIR_PIN,!Motor2_DIR);
				pwm_set_duty(MOTOR2_PWM, -motor2 * (PWM_DUTY_MAX / 100));
		}
		
}











