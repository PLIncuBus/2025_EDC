/*
 * @Author: PLIncuBus wewean@yeah.net
 * @Date: 2025-07-20 22:17:02
 * @LastEditors: PLIncuBus wewean@yeah.net
 * @LastEditTime: 2025-07-21 21:51:14
 * @FilePath: \SeekFree\project\code\Hardware\SystemClock.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "SystemClock.h"

uint64_t SystemClock_ms;

static void SystemClock_Interrupt_Callback(uint32 state, void *ptr);
/**
 * @brief 系统时钟定时器中断初始化
 * @param  void
 * 
 */
void SystemClock_Interrupt_Init(void)
{
    pit_ms_init(PIT_CH,SYSTEMCLOCK_FREQ,SystemClock_Interrupt_Callback,NULL);                                                  
    interrupt_set_priority(PIT_PRIORITY, 1);
}

/**
 * @brief 任务时钟计时
 * 
 * @param timer 
 */
void SystemClock_start_count_time(sys_count_time_t *timer)
{
    timer->start_time = SystemClock_ms;
    timer->count_now_time = 0;
}

/**
 * @brief 获取任务计时状态
 * @param timer 
 * @return uint8_t 
 */
uint8_t SystemClock_Get_State(sys_count_time_t *timer)
{
    //计时完成
    if((timer->count_now_time) >= (timer->count_end_time))
    {
        return 1;
    }
    //计时
    else
    {
        timer->count_now_time = SystemClock_ms - timer->start_time;
        return 0;
    }
}

/**
 * @brief 定时器中断
 * @param void
 */
static void SystemClock_Interrupt_Callback(uint32 state, void *ptr)
{
    SystemClock_ms += SYSTEMCLOCK_FREQ;
} 
