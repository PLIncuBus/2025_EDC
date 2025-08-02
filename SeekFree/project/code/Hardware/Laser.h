#ifndef __LASER_H
#define __LASER_H


#include "zf_common_headfile.h"

#define Laser(x)  gpio_set_level(B7,x)

void Laser_Init(void);

#endif