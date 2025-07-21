#include "Math.h"

/**
 * @brief 一阶滤波
 * 
 */
float firstOrderFilter(float in_data,float final,float a)
{
	final = a*in_data + (1-a)*final;	
	return(final);
}