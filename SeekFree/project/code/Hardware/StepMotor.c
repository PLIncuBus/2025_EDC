#include "StepMotor.h"


uint8_t StepMotor_Data[20];//发送的数据数组
uint16_t RxBuffer1[10] = {0};//收到的数据数组
uint16_t Motor_StepMotor_Current = 0;//丝杆相电流
uint8_t StepMotor_SetPosition_flag=0;//位置式（放在usart任务中运行）
uint8_t StepMotor_SetSpeed_flag=0;//速度式（放在usart任务中运行）
uint8_t StepMotor_Get_position_error_flag=0;//误差值（放在usart任务中运行）
long StepMotor_position_error_receive=0;//收到的未处理位置式误差
float StepMotor_position_error=0;//处理后的位置式误差
uint8_t StepMotor_fifo_get_data[64];
fifo_struct StepMotor_uart_data_fifo;
uint32 StepMotor_fifo_data_count = 0;
Gimbal_Data gimbal_data=
{
	.Target_Pitch=0,
	.Target_Yaw=0,
	
};
void StepMotor_JiaoZhun(void);
void StepMotor_Enable(uint8_t Motor_Num);
void StepMotor_Disable(uint8_t Motor_Num);
void StepMotor_Reset(uint8_t Motor_Num);
void StepMotor_Clear(uint8_t Motor_Num);
void StepMotor_Get_PID(void);
void StepMotor_Get_position_error(void);
void StepMotor_Read_Current(void);
void StepMotor_Set_PID(uint32_t KP, uint32_t KI, uint32_t KD);
void StepMotor_SetSpeed(uint8_t Motor_Num,int16_t Speed, uint8_t Acc);

void StepMotor_Send(void);
void StepMotor_Callback(uint32 state, void *ptr);

// 绝对值函数
uint32_t My_ABS(int32_t temp)
{
      if (temp >= 0)
            return temp;
      else
            return -temp;
}




/**
 * @brief 初始化
 * 
 */
void StepMotor_Init(void)
{
    uart_init(StepMotor_UART_INDEX,StepMotor_UART_BAUNDRATE,StepMotor_UART_TX_PIN,StepMotor_UART_RX_PIN);
    uart_set_interrupt_config(StepMotor_UART_INDEX, UART_INTERRUPT_CONFIG_RX_ENABLE);		// 使能串口接收中断
    interrupt_set_priority(StepMotor_UART_PRIORITY, 0);  
    uart_set_callback(StepMotor_UART_INDEX, StepMotor_Callback, NULL);	
    StepMotor_Reset(0x01);
    system_delay_ms(100);
    StepMotor_JiaoZhun();
    system_delay_ms(10);
    StepMotor_Enable(0x01);
	  system_delay_ms(10);
	  StepMotor_Reset(0x02);
    system_delay_ms(100);
    StepMotor_JiaoZhun();
    system_delay_ms(10);
    StepMotor_Enable(0x02);
}   




void StepMotor_JiaoZhun(void)
{
      StepMotor_Data[0] = 0x05;
      StepMotor_Data[1] = 0x9A;
      StepMotor_Data[2] = 0x02;
      StepMotor_Data[3] = 0x00;
      StepMotor_Data[4] = 0x6B;
    //   fifo_write_buffer(&StepMotor_uart_data_fifo,StepMotor_Data,4);
    //   fifo_read_buffer(&StepMotor_uart_data_fifo,StepMotor_fifo_get_data);
      uart_write_buffer(StepMotor_UART_INDEX,StepMotor_Data,5);
      system_delay_ms(10);
}

void StepMotor_Enable(uint8_t Motor_Num)
{
      StepMotor_Data[0] = Motor_Num;
      StepMotor_Data[1] = 0xF3;
      StepMotor_Data[2] = 0xAB;
      StepMotor_Data[3] = 0x01;
      StepMotor_Data[4] = 0x00;
      StepMotor_Data[5] = 0x6B;
      uart_write_buffer(StepMotor_UART_INDEX,StepMotor_Data,6);
      system_delay_ms(10);
}
void StepMotor_Disable(uint8_t Motor_Num)
{
      StepMotor_Data[0] = Motor_Num;
      StepMotor_Data[1] = 0xF3;
      StepMotor_Data[2] = 0xAB;
      StepMotor_Data[3] = 0x00;
      StepMotor_Data[4] = 0x00;
      StepMotor_Data[5] = 0x6B;
    //   usart_SendArray(StepMotor_Data,6);
    //   delay_ms(10);
}
//让电机紧急刹车 将当前位置清零
void StepMotor_Reset(uint8_t Motor_Num)
{
      StepMotor_Data[0] = Motor_Num;
      StepMotor_Data[1] = 0xFE;
      StepMotor_Data[2] = 0x98;
      StepMotor_Data[3] = 0x00;
      StepMotor_Data[4] = 0x6B;
      uart_write_buffer(StepMotor_UART_INDEX,StepMotor_Data,5);
      system_delay_ms(10);
      StepMotor_Data[0] = Motor_Num;
      StepMotor_Data[1] = 0x0A;
      StepMotor_Data[2] = 0x6D;
      StepMotor_Data[3] = 0x6B;
      uart_write_buffer(StepMotor_UART_INDEX,StepMotor_Data,4);
      system_delay_ms(10);
}
//将当前位置清零
void StepMotor_Clear(uint8_t Motor_Num)
{
      StepMotor_Data[0] = Motor_Num;
      StepMotor_Data[1] = 0x0A;
      StepMotor_Data[2] = 0x6D;
      StepMotor_Data[3] = 0x6B;
    //   usart_SendArray(StepMotor_Data,4);
    //   delay_ms(10);
}

//读取pid
void StepMotor_Get_PID(void)
{
			StepMotor_Data[0]=0x01;
			StepMotor_Data[1]=0x21;
			StepMotor_Data[2]=0x6B;
}
//读取位置式误差
void StepMotor_Get_position_error(void)
{
			StepMotor_Data[0]=0x01;
			StepMotor_Data[1]=0x37;
			StepMotor_Data[2]=0x6B;
			StepMotor_Get_position_error_flag=1;
			
}
//设置pid
void StepMotor_Set_PID(uint32_t KP, uint32_t KI, uint32_t KD)
{
      StepMotor_Data[0] = 0x01;
      StepMotor_Data[1] = 0x4A;
      StepMotor_Data[2] = 0xC3;
      StepMotor_Data[3] = 0x01;
      StepMotor_Data[4] = KP >> 24;
      StepMotor_Data[5] = (KP << 8) >> 24;
      StepMotor_Data[6] = (KP << 16) >> 24;
      StepMotor_Data[7] = (KP << 24) >> 24;
      StepMotor_Data[8] = KI >> 24;
      StepMotor_Data[9] = (KI << 8) >> 24;
      StepMotor_Data[10] = (KI << 16) >> 24;
      StepMotor_Data[11] = (KI << 24) >> 24;
      StepMotor_Data[12] = KD >> 24;
      StepMotor_Data[13] = (KD << 8) >> 24;
      StepMotor_Data[14] = (KD << 16) >> 24;
      StepMotor_Data[15] = (KD << 24) >> 24;
      StepMotor_Data[16] = 0x6B;
	    // usart_SendArray(StepMotor_Data,17);
    //   delay_ms(10);
}
//读取相电流
void StepMotor_Read_Current(void)
{
      StepMotor_Data[0] = 0x01;
      StepMotor_Data[1] = 0x27;
      StepMotor_Data[2] = 0x6B;
    //   usart_SendArray(StepMotor_Data,3);
    //   delay_ms(20);
}

// Speed 单位RPM
// Acc   0---255
void StepMotor_SetSpeed(uint8_t Motor_Num,int16_t Speed, uint8_t Acc)
{
      uint8_t Direction;
      uint16_t Speed_Temp = My_ABS(Speed);
      StepMotor_Data[0] = Motor_Num;
      StepMotor_Data[1] = 0xF6;
      if (Speed >= 0)
                  Direction = 0x01;
      else
                  Direction = 0x00;
      StepMotor_Data[2] = Direction;
      StepMotor_Data[3] = Speed_Temp >> 8;
      StepMotor_Data[4] = (Speed_Temp << 8) >> 8;
      StepMotor_Data[5] = Acc;
      StepMotor_Data[6] = 0x00;
      StepMotor_Data[7] = 0x6B;
			uart_write_buffer(StepMotor_UART_INDEX, StepMotor_Data, 8);

}

// Speed 单位RPM(每分钟转多少圈)
// Acc   0---255 (0代表关闭梯形加减速)
// Pulse 3200脉冲一圈
// 绝对脉冲
void StepMotor_SetPosition(uint8_t Motor_Num, float Angle, int16_t Speed, uint8_t Acc)
{
	 uint8_t Direction;
		 if (Angle >= 0)
    {
        Direction = 1;
    }
    else
    {
        Direction = 0;
    }
		Angle=My_ABS(Angle);
		uint32_t Pulse=(uint32_t)((float)Angle/360 *3200);
    uint8_t command_data[13]; 
    uint16_t Speed_Temp = My_ABS(Speed);
   

    command_data[0] = Motor_Num;
    command_data[1] = 0xFD;
   

    command_data[2] = Direction;
    
    command_data[3] = (uint8_t)(Speed_Temp >> 8);   // 速度高8位
    command_data[4] = (uint8_t)(Speed_Temp);        // 速度低8位
    
    command_data[5] = Acc;

    command_data[6] = (uint8_t)(Pulse >> 24); // 脉冲第1个字节
    command_data[7] = (uint8_t)(Pulse >> 16); // 脉冲第2个字节
    command_data[8] = (uint8_t)(Pulse >> 8);  // 脉冲第3个字节
    command_data[9] = (uint8_t)(Pulse);       // 脉冲第4个字节

    command_data[10] = 0x01;
    command_data[11] = 0x00;
    command_data[12] = 0x6B;

    uart_write_buffer(StepMotor_UART_INDEX, command_data, 13);
}
void Gimbal_Set_Angle(float Yaw ,float Pitch )
{
		StepMotor_SetPosition(0x01,Yaw,100,0);  //yaw
		system_delay_ms(10);
		StepMotor_SetPosition(0x02,Pitch,100,0);	//pitch
		system_delay_ms(10);
}
void Gimbal_Set_Speed(int16_t Yaw ,int16_t Pitch )
{
		StepMotor_SetSpeed(0x01,Yaw,0);  //yaw
		system_delay_ms(10);
		StepMotor_SetSpeed(0x02,Pitch,0);	//pitch
		system_delay_ms(10);
}


// Flash 位置定义（你可以根据需要选择 sector_num 和 page_num）
#define GIMBAL_FLASH_SECTOR    0
#define GIMBAL_FLASH_PAGE      0

// 保存 gimbal_data 到 Flash
void save_gimbal_to_flash()
{
	
    // 将 gimbal_data 拷贝到 union 缓冲区
//    flash_union_buffer[0].float_type = gimbal_data.Current_Yaw;
//    flash_union_buffer[1].float_type = gimbal_data.Current_Pitch;
//    flash_union_buffer[2].float_type = gimbal_data.Target_Yaw;
//    flash_union_buffer[3].float_type = gimbal_data.Target_Pitch;

		flash_union_buffer[0].float_type = 10.0;
    flash_union_buffer[1].float_type =10.0;
    flash_union_buffer[2].float_type =10.0;
    flash_union_buffer[3].float_type =10.0;

    // 写入 Flash
    flash_write_page_from_buffer(GIMBAL_FLASH_SECTOR, GIMBAL_FLASH_PAGE);
   
}

// 从 Flash 读取 gimbal_data
void load_gimbal_from_flash()
{
    // 从 Flash 读取到缓冲区
    flash_read_page_to_buffer(GIMBAL_FLASH_SECTOR, GIMBAL_FLASH_PAGE);

    // 从缓冲区赋值给 gimbal_data
    gimbal_data.Current_Yaw    = flash_union_buffer[0].float_type;
    gimbal_data.Current_Pitch  = flash_union_buffer[1].float_type;
    gimbal_data.Target_Yaw     = flash_union_buffer[2].float_type;
    gimbal_data.Target_Pitch   = flash_union_buffer[3].float_type;

}
///**
// * @brief 发送
// * 
// */
//void StepMotor_Send(void)
//{
//    StepMotor_fifo_data_count = fifo_used(&StepMotor_uart_data_fifo);
//    if(StepMotor_fifo_data_count != 0){
//       fifo_read_buffer(&StepMotor_uart_data_fifo,StepMotor_fifo_get_data,&StepMotor_fifo_data_count,FIFO_READ_AND_CLEAN); 
//       uart_write_buffer(StepMotor_UART_INDEX,StepMotor_fifo_get_data,StepMotor_fifo_data_count);
//    }

//    
//}



/**
 * @brief 回调函数
 * 
 * @param state 
 * @param ptr 
 */
void StepMotor_Callback(uint32 state, void *ptr)
{

}


