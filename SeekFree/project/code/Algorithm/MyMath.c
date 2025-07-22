/*
 * @Author: PLIncuBus wewean@yeah.net
 * @Date: 2025-07-21 22:10:08
 * @LastEditors: PLIncuBus wewean@yeah.net
 * @LastEditTime: 2025-07-22 13:15:59
 * @FilePath: \SeekFree\project\code\Algorithm\Math.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "MyMath.h"

/**
 * @brief 一阶滤波
 * 
 */
float firstOrderFilter(float in_data,float final,float a)
{
	final = a*in_data + (1-a)*final;	
	return(final);
}


float myRsqrt(float num)
{
    float halfx = 0.5f * num;
    float y = num;
    long i = *(long *)&y;
    i = 0x5f375a86 - (i >> 1);

    y = *(float *)&i;
    y = y * (1.5f - (halfx * y * y));
    y = y * (1.5f - (halfx * y * y));

    return y;
}

double fast_atan2(double y, double x)
{
    if (x == 0)
    {
        if (y > 0)
            return PI / 2;
        else if (y < 0)
            return -PI / 2;
        else
            return 0.0;
    }
    else
    {
        double angle = atan(y / x);
        if (x < 0)
        {
            angle += (y >= 0) ? PI : -PI;
        }
        return angle;
    }
}