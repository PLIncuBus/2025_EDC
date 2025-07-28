#ifndef __PHOTOTUBE_H
#define __PHOTOTUBE_H


#include "zf_common_headfile.h"
#include "BSP_I2C.h"

#define pca9555_Slave_Addr 0x20
#define pca9555_INPUT_PORT_REGISTER0                    0x00   

typedef struct
{
	uint8_t bit1	:1;
	uint8_t bit2	:1;
	uint8_t bit3	:1;
	uint8_t bit4	:1;
	uint8_t bit5	:1;
	uint8_t bit6	:1;
	uint8_t bit7	:1;
	uint8_t bit8	:1;
	uint8_t bit9	:1;
	uint8_t bit10	:1;
	uint8_t bit11	:1;
	uint8_t bit12	:1;
	uint8_t bit13	:1;
	uint8_t bit14	:1;
	uint8_t bit15	:1;
	uint8_t bit16	:1;
}gray_flags;


typedef union
{
	uint16_t state;
	gray_flags gray;
}_gray_state; 


extern float gray_status[2],gray_status_backup[2][20];
extern uint32_t gray_status_worse;
extern _gray_state gray_state; 
extern float Cha_error;



void phototube_Init(void);
void phototube_proceed(void);
int16_t  readTrackDate(uint16_t dat);

#endif