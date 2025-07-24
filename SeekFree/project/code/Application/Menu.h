#ifndef __MENU_H
#define __MENU_H

#include "zf_common_headfile.h"
#include "IMU.h"
#include "Encoder.h"
#include "Motor.h"

#define     Menu_ShowStr         ips114_show_string
#define     Menu_ShowFloat       ips114_show_float
#define 		Menu_ShowInt				 ips114_show_int
#define     Menu_Clear           ips114_clear

#define     KEY_ENSURE           KEY_1
#define     Menu_Ensure_Detect   (key_get_state(KEY_ENSURE) == KEY_SHORT_PRESS) 
#define     KEY_UP               KEY_2
#define     Menu_Up_Detect       (key_get_state(KEY_UP)     == KEY_SHORT_PRESS) 
#define     KEY_DOWN             KEY_3
#define     Menu_Down_Detect     (key_get_state(KEY_DOWN)   == KEY_SHORT_PRESS) 
#define     KEY_BACK             KEY_4
#define     Menu_Back_Detect     (key_get_state(KEY_BACK)   == KEY_SHORT_PRESS) 
  

typedef enum{ 
    Menu_Ensure= 0,
    Menu_Up,
    Menu_Down,
    Menu_Back,
    Button_Sum,
}Menu_Button_enum;

typedef enum{
    Type_SubMenu,               //从菜单
    Type_ParMenu,               //参数菜单
    Type_BehMenu,               //行为菜单
		Type_ShoMenu,								//实时菜单
}Menu_enum;

typedef enum{
		Idle_Menu,
		Proc_Menu,
}Menu_State_enum;

typedef struct Menu_Info_t{
    uint8_t MenuLen;
    uint8_t string[7];
    Menu_enum type;
    struct Menu_Info_t *Father;
    struct Menu_Info_t *Child;
    void (*Menu_Fun)();
		Menu_State_enum State;
		
}Menu_Info_t;

void Menu_Init(uint8_t Timer);
void Menu_Process(void);



#endif