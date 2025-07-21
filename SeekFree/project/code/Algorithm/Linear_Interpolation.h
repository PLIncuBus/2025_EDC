#ifndef __LINEAR_INTERPOLATION_H
#define __LINEAR_INTERPOLATION_H

#include "zf_common_headfile.h"

typedef struct _input_li5_t
{
    float t0; //开始时间
    float s0; //开始位移
    float v0; //开始速度
    float a0; //开始加速度

    float t1; //结束时间
    float s1; //结束加速度
    float v1; //结束速度
    float a1; //结束加速度

}input_li5_t;


typedef struct _output_li5_t
{
    //解算出五阶方程的7个参数
    float t0;
    float a0;
    float a1;
    float a2;
    float a3;
    float a4;
    float a5;

}output_li5_t;




output_li5_t li5th_solve(input_li5_t input);

float get_displacement_from_li5(output_li5_t *input_solve, float now_time);
float get_speed_from_li5(output_li5_t *input_solve, float now_time);
float get_acceleration_from_li5(output_li5_t *input_solve, float now_time);




#endif