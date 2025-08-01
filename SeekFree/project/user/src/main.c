/*
 * @Author: PLIncuBus wewean@yeah.net
 * @Date: 2025-07-20 15:14:04
 * @LastEditors: PLIncuBus wewean@yeah.net
 * @LastEditTime: 2025-07-30 17:21:41
 * @FilePath: \SeekFree\project\user\src\main.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*********************************************************************************************************************
* MSPM0G3507 Opensource Library 即（MSPM0G3507 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
* 
* 本文件是 MSPM0G3507 开源库的一部分
* 
* MSPM0G3507 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
* 
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
* 
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
* 
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
* 
* 文件名称          mian
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          MDK 5.37
* 适用平台          MSPM0G3507
* 店铺链接          https://seekfree.taobao.com/
********************************************************************************************************************/

#include "zf_common_headfile.h"
#include "Menu.h"
#include "IMU.h"
#include "Encoder.h"
#include "Motor.h"
#include "Chassis_Control.h"
#include "UpperMonitor.h"
#include "phototube.h"
#include "Path_Planning.h"
#include "StepMotor.h"
#include "StepMotor_Control.h"
#include "Task.h"

// 打开新的工程或者工程移动了位置务必执行以下操作
// 第一步 关闭上面所有打开的文件
// 第二步 project->clean  等待下方进度条走完

// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设

// **************************** 代码区域 ****************************
static void _50HZ_Callback(uint32 state, void *ptr);
static void _20MS_Callback(uint32 state, void *ptr);
static void _20MS_1_Callback(uint32 state, void *ptr);

int main (void)
{
    clock_init(SYSTEM_CLOCK_80M);   // 时钟配置及系统初始化<务必保留>
    debug_init();					// 调试串口信息初始化
	  // 此处编写用户代码 例如外设初始化代码等
		
    //BSP初始化
    SYSCFG_DL_I2C_0_init();

    //硬件初始化
    Menu_Init(20);		
		//IMU初始化   参数：中断频率,陀螺仪静置时间
    IMU_Init(20,2000);
		Encoder_Init();
		Motor_Init();
		phototube_Init();
    StepMotor_Init();
		UpperMonitor_Init();    
    
	// 50HZ定时器中断初始化
//    pit_ms_init( PIT_TIM_A1 , 10 , _50HZ_Callback , NULL ); 
//		pit_ms_init( PIT_TIM_G12,  20,_20MS_Callback,NULL);
//		pit_ms_init( PIT_TIM_G8,  20,_20MS_1_Callback,NULL);
//		SystemClock_Interrupt_Init();
    //应用层初始化
		StepMotor_Control_Init(&StepMotor_Control);
    Chassis_Init(&Differential_Wheel_Info);
    //菜单初始化  参数：中断频率
		system_delay_ms(5000);

		
		
//		Path_Planning_Init();

		Differential_Wheel_Info.mode = stop;
		StepMotor_Control.mode = StepMotor_Initial_Mode;



    // 此处编写用户代码 例如外设初始化代码等
    while(true)
    {
        // 此处编写需要循环执行的代码
					
					Menu_Process();

					phototube_proceed();
					IMU_Attitude_Process();			
					Cha_error = (float)readTrackDate((uint16_t)gray_state.state)/23.5;
					
//		//			UpperMonitor_Cmd_Send(&UpperMonitor_Handle);
					
					StepMotor_Control_Proceed(&StepMotor_Control);

				
				Differential_Wheel_Info.vx_set = 10;
//        // 此处编写需要循环执行的代码
			
			if(Task1_flag){
				
				static uint8_t wait;
				static uint8_t loop;
				Chassis_Proceed(&Differential_Wheel_Info);
				Differential_Wheel_Info.mode = track;
//				if(wait < 30){
//					wait ++;
//					Differential_Wheel_Info.vx_set = 2;
//				}
//				else{
				Differential_Wheel_Info.vx_set = 9.5;//}
			
				if((Encoder_count_sum[Encoder1] > 2500) && (abs((int)Angle_Yaw) < 10  )) {
					loop ++;
				Encoder_count_sum[Encoder1] = 0;
				}
				if(loop >= Task1_Loop_Num){
					Differential_Wheel_Info.mode = stop;
				}	
				
				
			}
			else if(Task2_flag){
					StepMotor_Control.mode = StepMotor_Control_Cal_mode;
					Differential_Wheel_Info.mode = stop;
					if(abs((int)StepMotor_Control.speed_pid[0].error) < Task2_Error_Deadline && abs((int)StepMotor_Control.speed_pid[1].error) < Task2_Error_Deadline ){
							Laser(0);
					}			
			}
			else if(Task3_flag){
					StepMotor_Control.mode = StepMotor_Control_Auto_Aim_mode;
					Differential_Wheel_Info.mode = stop;
					if(abs((int)StepMotor_Control.speed_pid[0].error) < Task3_Error_Deadline && abs((int)StepMotor_Control.speed_pid[1].error) < Task3_Error_Deadline ){
						Laser(0);
					}
				
			}
			else if(Task4_flag){
				static uint8_t wait;
				static uint8_t loop;
				StepMotor_Control.mode = StepMotor_Control_Cal_mode;
				if(abs((int)StepMotor_Control.speed_pid[0].error) < Task3_Error_Deadline && abs((int)StepMotor_Control.speed_pid[1].error) < Task3_Error_Deadline ){
						Laser(0);
				}
				Chassis_Proceed(&Differential_Wheel_Info);
				Differential_Wheel_Info.mode = track;
//				if(wait < 30){
//					wait ++;
//					Differential_Wheel_Info.vx_set = 2;
//				}
//				else{
				Differential_Wheel_Info.vx_set = 9.5;//}
				Differential_Wheel_Info.mode = track;
				if((Encoder_count_sum[Encoder1] > 2500) && (abs((int)Angle_Yaw) < 10  )) {
					loop ++;
				Encoder_count_sum[Encoder1] = 0;
				}
				if(loop >= Task4_Loop_Num){
					
					Differential_Wheel_Info.mode = stop;
				}	
			}
			
    }
}

//static void _50HZ_Callback(uint32 state, void *ptr)
//{
		
		
//}
//			Menu_Process();
//			IMU_Attitude_Process();			
//			phototube_proceed();
//			Cha_error = (float)readTrackDate(gray_state.state)/23.5;
//			Chassis_Proceed(&Differential_Wheel_Info); 


//			Path_Planning_Publish(&Differential_Wheel_Info);
// 			Gimbal_Set_Angle(gimbal_data.Target_Yaw,gimbal_data.Target_Pitch);

