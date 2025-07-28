#ifndef __SYSTEMCLOCK_H
#define __SYSTEMCLOCK_H

#include "zf_common_headfile.h"

#define PIT_CH                  (PIT_TIM_G0 )                                      		// 使用的周期中断编号 如果修改 需要同步对应修改周期中断编号与 isr.c 中的调用
#define PIT_PRIORITY            (TIMG0_INT_IRQn)  
#define SYSTEMCLOCK_FREQ        (10) 


typedef struct _sys_count_time_t 
{
    char count_time_state;
    long long int start_time;
    long long int count_now_time;
    long long int count_end_time;

}sys_count_time_t;



void SystemClock_Interrupt_Init(void);
void SystemClock_start_count_time(sys_count_time_t *timer);
uint8_t SystemClock_Get_State(sys_count_time_t *timer);


extern uint64_t SystemClock_ms;


#endif