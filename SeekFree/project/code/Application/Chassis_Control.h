#ifndef __CHASSIS_CONTROL_H
#define __CHASSIS_CONTROL_H

#include "zf_common_headfile.h"



typedef enum{
    Differential_Wheel,
    Mecanum_Wheel,
    None,
}ChassisMode_enum;


#define Chassis_mode Differential_Wheel

typedef struct{
    float vx_set;
    float angle_set;  
}Differential_Wheel_Info_t; 


#endif