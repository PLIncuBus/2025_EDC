#ifndef __BSP_I2C_H
#define __BSP_I2C_H



#include "zf_common_headfile.h"

#define I2C_TIMEOUT_MS  (1)

int mspm0_i2c_read(unsigned char slave_addr,
                    unsigned char reg_addr,
                    unsigned char length,
                    unsigned char *data);
int mspm0_i2c_write(unsigned char slave_addr,
                     unsigned char reg_addr,
                     unsigned char length,
                     unsigned char const *data);

bool I2C_CheckDevice( uint8_t address);

#endif