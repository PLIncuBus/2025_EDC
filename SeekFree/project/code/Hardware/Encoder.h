#ifndef __ENCODER_H
#define __ENCODER_H

#include "zf_common_headfile.h"

#define ENCODER1_POSITIVE    (B18)
#define ENCODER1_NEGATIVE    (B17)

#define ENCODER2_POSITIVE    (A26)
#define ENCODER2_NEGATIVE    (A27)



#define ENCODER_EXTI				 (GPIOA_INT_IRQn)

typedef enum{
    Encoder1  = 0,
    Encoder2,
    Encoder_Sum,
}Encoder_enum;

void Encoder_Init(void);
int16_t Encoder_Count_Get(Encoder_enum index);

#endif

