#include "Menu.h"

//信号量
uint8_t Menu_Button[Button_Sum];

//先声明
static void Menu_Button_Interface(void);
static void Menu_Behaviour(void);
static void Menu_Display(void);
Menu_Info_t Motor_PID_Menu[2];
Menu_Info_t Task_Menu[4];
Menu_Info_t PID_Menu[1];
Menu_Info_t Hardware_Info_Menu[2];
Menu_Info_t Motor_PID_SET_Menu[3];
void Task1(void);
void Task2(void);
void Task3(void);
void Task4(void);
void IMU_Info_Show(void);
void Chassis_Info_Show(void);
/*****第一级主菜单BEGIN*****/

Menu_Info_t Main_Menu[3] = {    
    {   3      ,   "Info"   ,   Type_SubMenu    ,   NULL    ,Hardware_Info_Menu,   NULL , .Param_Info = {
        .step = 0,
        .max = 0,
        .min = 0,
        .param = NULL },
        Idle_Menu  },
    {   3      ,   "Task"   ,   Type_SubMenu    ,   NULL    ,   Task_Menu   ,   NULL    , .Param_Info.max = 0, .Param_Info = {
        .step = 0,
        .max = 0,
        .min = 0,
        .param = NULL },
        Idle_Menu  },
    {   3      ,   "PID"    ,   Type_SubMenu    ,   NULL    ,   PID_Menu    ,   NULL    ,.Param_Info = {
        .step = 0,
        .max = 0,
        .min = 0,
        .param = NULL }, 
        Idle_Menu  },
};
/*****第一级主菜单END*****/

/*****第二级菜单BEGIN******/

Menu_Info_t Task_Menu[4] = {
    {   4      ,   "Task1"  ,   Type_ShoMenu    ,  Main_Menu ,   NULL    ,   Task1     ,.Param_Info = {
        .step = 0,
        .max = 0,
        .min = 0,
        .param = NULL },
        Idle_Menu},
    {   4      ,   "Task2"  ,   Type_ShoMenu    ,  Main_Menu ,   NULL    ,   Task2     ,.Param_Info = {
        .step = 0,
        .max = 0,
        .min = 0,
        .param = NULL },
         Idle_Menu},
    {   4      ,   "Task3"  ,   Type_ShoMenu    ,  Main_Menu ,   NULL    ,   Task3     ,.Param_Info = {
        .step = 0,
        .max = 0,
        .min = 0,
        .param = NULL },
         Idle_Menu},
    {   4      ,   "Task4"  ,   Type_ShoMenu    ,  Main_Menu ,   NULL    ,   Task4     ,.Param_Info = {
        .step = 0,
        .max = 0,
        .min = 0,
        .param = NULL },
        Idle_Menu},
};

Menu_Info_t PID_Menu[1] = {
    {   1      ,   "Motor"  ,   Type_SubMenu    ,  Main_Menu ,   Motor_PID_Menu    ,   NULL   	 ,.Param_Info = {
        .step = 0,
        .max = 0,
        .min = 0,
        .param = NULL },
         Idle_Menu},
};

Menu_Info_t Hardware_Info_Menu[2] = {
    {   2      ,   "IMU"  ,    Type_ShoMenu    ,  Main_Menu ,   NULL    ,   IMU_Info_Show ,.Param_Info = {
        .step = 0,
        .max = 0,
        .min = 0,
        .param = NULL },
         Idle_Menu},
    {   2      ,  "Chasis",    Type_ShoMenu    ,  Main_Menu ,   NULL    , Chassis_Info_Show,.Param_Info = {
        .step = 0,
        .max = 0,
        .min = 0,
        .param = NULL },
         Idle_Menu},

};

/*****第二级菜单END******/

/*****第三级菜单BEGIN******/

Menu_Info_t Motor_PID_Menu[2] = {

        {   2      ,   "Speed"    ,  Type_SubMenu   , PID_Menu ,  Motor_PID_SET_Menu    ,   NULL ,.Param_Info = {
        .step = 0,
        .max = 0,
        .min = 0,
        .param = NULL },
         Idle_Menu},
        {   2      ,   "Position" , Type_SubMenu    , PID_Menu ,   NULL    ,   NULL ,.Param_Info = {
        .step = 0,
        .max = 0,
        .min = 0,
        .param = NULL },
         Idle_Menu},
};

/*****第三级菜单END******/

/*****第四级菜单BEGIN******/

Menu_Info_t Motor_PID_SET_Menu[3] = {

        {   3      ,   "KP"    ,  Type_ParMenu   , Motor_PID_Menu ,   NULL    ,   NULL ,.Param_Info = {
        .step = 0.01,
        .max = 30,
        .min = 0,
        .param = &motor1_speed_pid_kp},
         Idle_Menu},
        {   3      ,   "KI" ,    Type_ParMenu    , Motor_PID_Menu ,   NULL    ,   NULL ,.Param_Info = {
        .step = 0.01,
        .max = 4,
        .min = 0,
        .param = &motor1_speed_pid_ki },
         Idle_Menu},
        {   3      ,   "KD"    ,  Type_ParMenu   , Motor_PID_Menu ,   NULL    ,   NULL ,.Param_Info = {
        .step = 0.01,
        .max = 4,
        .min = 0,
        .param = &motor1_speed_pid_kd },
         Idle_Menu},
};

/*****第四级菜单END******/
//test
void Task1(void)
{
    Motor_Cmd(10,10);
}
void Task2(void)
{
    Menu_ShowStr(0,0,"Task2");
}
void Task3(void)
{
    Menu_ShowStr(0,0,"Task3");
}
void Task4(void)
{
    Menu_ShowStr(0,0,"Task4");
}
void IMU_Info_Show(void)
{
    Menu_ShowStr(0,0,"YawAngle");
    Menu_ShowFloat(0,16,Angle_Yaw,3,5);
	Menu_ShowStr(0,32,"Roll");
    Menu_ShowFloat(0,48,roll,3,3);
	Menu_ShowStr(0,64,"Pitch");
	Menu_ShowFloat(0,80,pitch,3,3);

}
void Chassis_Info_Show(void)
{
    Menu_ShowStr(0,0,"Encoder1");
    Menu_ShowInt(100,0,Encoder_Count_Get(Encoder1),5);
    Menu_ShowStr(0,16,"Encoder2");
    Menu_ShowInt(100,16,Encoder_Count_Get(Encoder2),5);

}


/**
 * @brief 菜单运行
 * @note  运行在主函数或定时器中断
 */
void Menu_Process(void)
{
    //按键扫描
    key_scanner();
    //获取按键接口
    Menu_Button_Interface();
    //菜单行为
    Menu_Behaviour();
    //绘制菜单
    Menu_Display();
}

/**
 * @brief 菜单初始化
 * @param Timer 
 */
void Menu_Init(uint8_t Timer)
{
    ips114_set_dir(IPS114_PORTAIT);
    ips114_set_color(RGB565_RED, RGB565_BLACK);
    ips114_init();
    ips114_clear();
    key_init(Timer);

}

/**
 * @brief 菜单按键接口
 * 
 */
static void Menu_Button_Interface(void)
{
    Menu_Button[Menu_Ensure] =  Menu_Ensure_Detect;
    Menu_Button[Menu_Up]     =  Menu_Up_Detect;
    Menu_Button[Menu_Down]   =  Menu_Down_Detect;
    Menu_Button[Menu_Back]   =  Menu_Back_Detect;
}

/**
 * @brief 菜单行为
 * 
 */
static Menu_Info_t *Cur_Menu = Main_Menu;
static Menu_Info_t *Last_Cur_Menu = NULL;
static uint8_t Last_Cur_Menu_index = 0;
static int16_t Last_Cur_Menu_Param_Step = 0;
static uint8_t Cur_Menu_index = 0;
static int16_t Cur_Menu_Param_Step = 0;			
static uint8_t Menu_Back_Cnt = 0;
static float 	Cur_Menu_Param_base = 0;
static void Menu_Behaviour(void)
{
		
    //上行行为
   if(Menu_Button[Menu_Up])
   {
		 if(Cur_Menu[Cur_Menu_index].State == Proc_Menu){
            if(Cur_Menu[Cur_Menu_index].type == Type_ParMenu){
                Cur_Menu_Param_Step ++;
            }
		 }
		 else{
        if((Cur_Menu_index - 1) < 0 ){
            Cur_Menu_index = Cur_Menu[Cur_Menu_index].MenuLen - 1;
        }
        else{
            Cur_Menu_index -- ;
        }
				Menu_Back_Cnt = 0;
			}

   }

    //下行行为
    else if(Menu_Button[Menu_Down])
    {
				if(Cur_Menu[Cur_Menu_index].State == Proc_Menu){
                     if(Cur_Menu[Cur_Menu_index].type == Type_ParMenu){
                    Cur_Menu_Param_Step --;
                     }
				}
				else{
        if((Cur_Menu_index + 1)  >= Cur_Menu[Cur_Menu_index].MenuLen){
            Cur_Menu_index = 0;
        }
        else{
            Cur_Menu_index ++ ;
        }
				Menu_Back_Cnt = 0;
			}

    }
   
    //确认行为
    else if(Menu_Button[Menu_Ensure])
    {
        //执行Fun函数
        if(Cur_Menu[Cur_Menu_index].type == Type_BehMenu)
        {    
            if(Cur_Menu[Cur_Menu_index].Menu_Fun == NULL);
            else{
                Cur_Menu[Cur_Menu_index].Menu_Fun();
            }
        }
        //跳转子节点
        else if(Cur_Menu[Cur_Menu_index].type == Type_SubMenu)
        {   
            if(Cur_Menu[Cur_Menu_index].Child == NULL);
            else{
                Cur_Menu = Cur_Menu[Cur_Menu_index].Child;
                Cur_Menu_index = 0;
            }
        }
				//实时显示参数
				else if(Cur_Menu[Cur_Menu_index].type == Type_ShoMenu)
				{
					if(Cur_Menu[Cur_Menu_index].Menu_Fun == NULL);
								else{
						Menu_Clear();
						Cur_Menu[Cur_Menu_index].State = Proc_Menu;		 
					}
				}
        //参数设置
        else if(Cur_Menu[Cur_Menu_index].type == Type_ParMenu)
        {
            Cur_Menu[Cur_Menu_index].State = Proc_Menu;
						Cur_Menu_Param_base = *Cur_Menu[Cur_Menu_index].Param_Info.param;
        }
				Menu_Back_Cnt = 0;
				
    }

    //返回行为
    else if(Menu_Button[Menu_Back])
    {

				Cur_Menu[Cur_Menu_index].State = Idle_Menu;
        Cur_Menu_Param_Step = 0;
				if(Cur_Menu[Cur_Menu_index].type == Type_ParMenu && Menu_Back_Cnt == 0){
					Menu_Back_Cnt ++;
				}
        else if(Cur_Menu[Cur_Menu_index].Father == NULL) Menu_Back_Cnt = 0;
        else{
            Cur_Menu = Cur_Menu[Cur_Menu_index].Father;
            Cur_Menu_index = 0;
						Menu_Back_Cnt = 0;
        }
    }
		
		
		
		
		//实时显示参数
		if(Cur_Menu[Cur_Menu_index].type == Type_ShoMenu && Cur_Menu[Cur_Menu_index].State == Proc_Menu)
		{
            if(Cur_Menu[Cur_Menu_index].Menu_Fun == NULL);
            else{
                Cur_Menu[Cur_Menu_index].Menu_Fun();
            }
		}
		//参数设置
    if(Cur_Menu[Cur_Menu_index].type == Type_ParMenu && Cur_Menu[Cur_Menu_index].State == Proc_Menu)
		{
			if(Last_Cur_Menu_Param_Step != Cur_Menu_Param_Step){
            *Cur_Menu[Cur_Menu_index].Param_Info.param  = Cur_Menu_Param_base + Cur_Menu_Param_Step*Cur_Menu[Cur_Menu_index].Param_Info.step;
						if(*Cur_Menu[Cur_Menu_index].Param_Info.param > Cur_Menu[Cur_Menu_index].Param_Info.max){
								*Cur_Menu[Cur_Menu_index].Param_Info.param = Cur_Menu[Cur_Menu_index].Param_Info.max;
						}
						else if(*Cur_Menu[Cur_Menu_index].Param_Info.param < Cur_Menu[Cur_Menu_index].Param_Info.min){
								*Cur_Menu[Cur_Menu_index].Param_Info.param = Cur_Menu[Cur_Menu_index].Param_Info.min;
						}          
			}
       Menu_ShowFloat(100,16*Cur_Menu_index,*Cur_Menu[Cur_Menu_index].Param_Info.param,3,5);     
		}
}


/**
 * @brief 绘制菜单
 * 
 */
static void Menu_Display(void)
{    

		Last_Cur_Menu_Param_Step = Cur_Menu_Param_Step;
    if(Cur_Menu == Last_Cur_Menu ){
        if(Cur_Menu_index == Last_Cur_Menu_index ){
            return;
        }
        else{
            Menu_ShowStr(0,Last_Cur_Menu_index*16," ");
            Menu_ShowStr(0,Cur_Menu_index*16,">");
        }
    }
		
    else if(Cur_Menu != Last_Cur_Menu || Last_Cur_Menu == NULL){
        Menu_Clear();
        Menu_ShowStr(0,Cur_Menu_index*16,">");
        for(uint8_t i = 0 ;i < Cur_Menu[Cur_Menu_index].MenuLen; i ++)
        {
            Menu_ShowStr(8,i*16,(const char *)Cur_Menu[i].string);
        }   
    }

		Last_Cur_Menu_index = Cur_Menu_index;
    Last_Cur_Menu = Cur_Menu;
}

