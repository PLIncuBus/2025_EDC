#include "UpperMonitor.h"



void UpperMonitor_Callback (uint32 state, void *ptr);
UpperMonitor_Handle_Typedef UpperMonitor_Handle;



/**
 * @brief           上位机数据发送
 * @param           //UpperMonitor_Cmd_Send 
 */
void UpperMonitor_Cmd_Send(UpperMonitor_Handle_Typedef *UpperMonitor_Cmd_Send)
{
	UpperMonitor_Motor_PID_Send_Hook(UpperMonitor_Cmd_Send);
    #if (UpperMonitor_Def == VOFA_JustFlow)
        const uint8_t UpperMonitor_VOFA_JustFlow_Tail_Buff[4] = { 0x00 , 0x00 , 0x80 , 0x7F };     
        memcpy( UpperMonitor_Cmd_Send->Usart_Send_Buff + UpperMonitor_Cmd_Send->Usart_Send_Index, UpperMonitor_VOFA_JustFlow_Tail_Buff , 4 * sizeof(uint8_t));
        UpperMonitor_Cmd_Send->Usart_Send_Index += 4*sizeof(uint8_t); 
				uart_write_buffer(UpperMonitor_UART_INDEX,&UpperMonitor_Cmd_Send->Usart_Send_Buff,UpperMonitor_Cmd_Send->Usart_Send_Index);
        UpperMonitor_Cmd_Send->Usart_Send_Index = 0;

    #endif
}

/**
 * @brief 上位机初始化
 * 
 */
void UpperMonitor_Init(void)
{
	#if (IMU_Type != 1 && JY61P_Mode != 0)
    uart_init(UpperMonitor_UART_INDEX,UpperMonitor_UART_BAUNDRATE,UpperMonitor_UART_TX_PIN,UpperMonitor_UART_RX_PIN);
//    uart_set_interrupt_config(UpperMonitor_UART_INDEX, UART_INTERRUPT_CONFIG_RX_ENABLE);		// 使能串口接收中断
//    interrupt_set_priority(UpperMonitor_UART_PRIORITY, 0);  
//    uart_set_callback(UpperMonitor_UART_INDEX, UpperMonitor_Callback, NULL);			    // 定义中断接收函数
	#endif
	}

#if (UpperMonitor_Mode == 0)




/**
 * @brief           模拟阶跃信号
 * @param[in]       //float *Step_Signal_array,uint8_t Step_Siganl_size,uint8_t repeat_cnt
 * @return          float 
 */
static float UpperMonitor_Step_Signal_Sim(float *Step_Signal_array,uint8_t Step_Siganl_size,uint8_t repeat_cnt)
{
    static uint8_t index,scale;
    index = scale / repeat_cnt;
    scale ++;
    if(scale > Step_Siganl_size) index = 0;
    return Step_Signal_array[index];   
}


/**
 * @brief           上位机PID发送钩子函数
 * @param[in]       
 * @note            
 */
void UpperMonitor_Motor_PID_Send_Hook(UpperMonitor_Handle_Typedef *UpperMonitor_Handle_Motor_PID_Hook)
{
    //阶跃信号数值
    const float Step_Signal[] = {3,2,1,3,5,1,0};
    //阶跃信号重复次数
    const uint8_t Step_Signal_repeat_cnt = 100;
    //上位机包
    static float Usart_Send_Buff[10];

    #if(Chassis_Def == AGV_Handle_Typedef)
        //单环速度环
        #if (UpperMonitor_Motor_PID_Loop_Def == Single_Velocity_Loop) 
        Usart_Send_Buff[0] = Differential_Wheel_Info.target[1];//UpperMonitor_Step_Signal_Sim((float *)Step_Signal,sizeof(Step_Signal)/sizeof(float),Step_Signal_repeat_cnt);
//        Differential_Wheel_Info.vx_set = Usart_Send_Buff[0];
        Usart_Send_Buff[1] = Differential_Wheel_Info.motor_encoder[1];   
        
        //双环内环速度环
        #elif (UpperMonitor_Motor_PID_Loop_Def == Double_Inner_Loop)
        Usart_Send_Buff[0] = UpperMonitor_Step_Signal_Sim(Step_Signal,sizeof(Step_Signal)/sizeof(float),Step_Signal_repeat_cnt);
        Usart_Send_Buff[1] = ;

        //双环外环角度环
        #elif (UpperMonitor_Motor_PID_Loop_Def == Double_Out_Loop)
        Usart_Send_Buff[0] = UpperMonitor_Step_Signal_Sim(Step_Signal,sizeof(Step_Signal)/sizeof(float),Step_Signal_repeat_cnt);
        Usart_Send_Buff[1] = ;

        #endif
         
    memcpy(UpperMonitor_Handle_Motor_PID_Hook->Usart_Send_Buff,Usart_Send_Buff,2*sizeof(float));
    UpperMonitor_Handle_Motor_PID_Hook->Usart_Send_Index += 2*sizeof(float);

    #endif   
}


#endif

/**
 * @brief 回调函数
 * 
 * @param state 
 * @param ptr 
 */
void UpperMonitor_Callback (uint32 state, void *ptr)
{

}


