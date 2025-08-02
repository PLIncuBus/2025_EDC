#include "Laser.h"

void Laser_Init(void)
{
    gpio_init(B7, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    Laser(0);
}

