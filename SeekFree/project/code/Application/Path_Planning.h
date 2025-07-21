#ifndef __PATH_PLANNING_H
#define __PATH_PLANNING_H

#include "zf_common_headfile.h"
#include "Linear_Interpolation.h"
#include "Chassis_Control.h"
#include "SystemClock.h"


typedef struct _Path_Plan_t
{
    float time;                     //节点时间
    float dis;                      //节点位移
    float vel;                      //节点速度
    float acc;                      //节点加速度

    void * task;                    //节点任务

    output_li5_t solve;             //5阶计算解算结果

}Path_Plan_Li5_t;


typedef struct _path_quene_t
{
    Path_Plan_Li5_t* quene;           //路径队列
    sys_count_time_t timer; //当前路径节点计时器
    char p_node;            //当前路径节点

}Path_Plan_Quene_t;


typedef enum{
    out_type_speed,
    out_type_displacement,
    out_type_acceleration,
}Path_Plan_Out_t;

void Path_Planning_Init(void);
void Path_Planning_Publish(Differential_Wheel_Info_t * _Path_Planning_Publish);

#endif