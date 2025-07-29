/*
 * @Author: PLIncuBus wewean@yeah.net
 * @Date: 2025-07-24 19:55:32
 * @LastEditors: PLIncuBus wewean@yeah.net
 * @LastEditTime: 2025-07-29 01:41:22
 * @FilePath: \2025_EDC\SeekFree\project\code\Hardware\Encoder.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __ENCODER_H
#define __ENCODER_H

#include "zf_common_headfile.h"

#define ENCODER1_POSITIVE    (B18)
#define ENCODER1_NEGATIVE    (B17)

#define ENCODER2_POSITIVE    (A27)
#define ENCODER2_NEGATIVE    (B11)



#define ENCODER1_EXTI				 (GPIOA_INT_IRQn)
#define ENCODER2_EXTI				 (GPIOB_INT_IRQn)
typedef enum{
    Encoder1  = 0,
    Encoder2,
    Encoder_Sum,
}Encoder_enum;
extern int16_t Encoder_count[Encoder_Sum];
void Encoder_Init(void);
int16_t Encoder_Count_Get(Encoder_enum index);

#endif

