#include "UpperMonitor.h"



void UpperMonitor_Callback (uint32 state, void *ptr);
void VisionMonitor_Callback (uint32 state, void *ptr);
UpperMonitor_Handle_Typedef UpperMonitor_Handle;

extern uint8_t Vision_RxFlag; 

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
    uart_init(VisionMonitor_UART_INDEX,VisionMonitor_UART_BAUNDRATE,VisionMonitor_UART_TX_PIN,VisionMonitor_UART_RX_PIN);
    uart_set_interrupt_config(VisionMonitor_UART_INDEX, UART_INTERRUPT_CONFIG_RX_ENABLE);		// 使能串口接收中断
    interrupt_set_priority(VisionMonitor_UART_PRIORITY, 0);  
    uart_set_callback(VisionMonitor_UART_INDEX, VisionMonitor_Callback, NULL);	
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
//        Usart_Send_Buff[0] = Differential_Wheel_Info.target[1];
//        Usart_Send_Buff[1] = Differential_Wheel_Info.motor_encoder[1];  
					Usart_Send_Buff[0]=1.0;
          Usart_Send_Buff[1]=2.0;
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
uint16_t Vision_values[6] = {0};
void VisionMonitor_parse_rect_data(const char *data)
{
	
		if(Vision_RxFlag==1)
	{	
			
		
			
			char buffer[128];
			strncpy(buffer, data, sizeof(buffer) - 1);
			buffer[sizeof(buffer) - 1] = '\0';

			char *token = strtok(buffer, ",");
			int count = 0;

			while (token != NULL && count < 6) {
				Vision_values[count++] = (uint16_t)atoi(token);
				token = strtok(NULL, ",");
			}

		if (count != 6) {Vision_RxFlag=0;
			return;
		}
		

		
		
		Vision_RxFlag=0;
	
	}
   

}



#endif

/**
 * @brief 回调函数
 * 
 * @param state 
 * @param ptr 
 */
uint8_t Vision_TxPacket[4]; // 定义发送数据包数组，数据包格式：FF 01 02 03 04 FE
uint8_t Vision_RxPacket[64]; // 定义接收数据包数组
uint8_t Vision_RxFlag;       // 定义接收数据包标志位
uint16_t RxData ;
void VisionMonitor_Callback(uint32 state, void *ptr)
{
    static uint8_t RxState = 0;   // 定义表示当前状态机状态的静态变量
    static uint8_t pRxPacket = 0; // 定义表示当前接收数据位置的静态变量
    // 接发送过来的数据保存在变量中
//    uint16_t RxData ;
    uart_read_byte(
        VisionMonitor_UART_INDEX,&RxData); // 读取数据寄存器，存放在接收的数据变量
    if (RxState == 0 && Vision_RxFlag == 0) {
      if (RxData == '@' ) // 如果数据确实是包头&& Vision_RxFlag == 0
      {
        RxState = 1;   // 置下一个状态
        pRxPacket = 0; // 数据包的位置归零
      }
    } else if (RxState == 1) // 收到包尾
    {
      if (RxData == '#') {
        RxState = 2;
      } else {
        Vision_RxPacket[pRxPacket] = RxData; // 将数据存入数据包数组的指定位置
        pRxPacket++;                         // 数据包的位置自增
      }
    }
    /*当前状态为2，接收数据包第二个包尾*/
    else if (RxState == 2) {
      if (RxData == '$') // 如果数据确实是包尾部
      {
        RxState = 0; // 状态归0
        Vision_RxPacket[pRxPacket] = '\0';        
				// printf("Serial_RxPacket: %s\r\n", Serial_RxPacket);
        Vision_RxFlag = 1; // 接收数据包标志位置1，成功接收一个数据包
      }
    }
  

}


/**
 * @brief 回调函数
 * 
 * @param state 
 * @param ptr 
 */
void UpperMonitor_Callback (uint32 state, void *ptr)
{

}


