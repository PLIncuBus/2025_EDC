#include "phototube.h"


_gray_state gray_state;
//只需要的到gray_status[0]的值就是线的位置
float gray_status[2]={0},gray_status_backup[2][20]={0};//灰度传感器状态与历史值
uint32_t gray_status_worse=0;	//灰度管异常状态计数器
float Cha_error;
int16_t result;

gpio_pin_enum gray_bits[12] = {A28,
															 B26,
															 B6,
															 B27,
															 B24,
															 B5,
															 B25,
															 A17,
															 B10,
															 B4,
															 B20,
															 A16
};

void phototube_Init(void)
{
//    while(!I2C_CheckDevice(0x20));
	for(uint8_t i = 0; i < 12;i ++)
	{
			gpio_init(gray_bits[i], GPI, GPIO_HIGH, GPI_PULL_DOWN);
	}


}

void gpio_input_check_channel_12_linewidth_20mm(void)
{
	for(uint16_t i=19;i>0;i--)
	{
		gray_status_backup[1][i]=gray_status_backup[1][i-1];
	}
	gray_status_backup[1][0]=gray_status[1];
	switch(gray_state.state)
	{
		case 0xFFFE: gray_status[1] = -11; break; 		//000000000001b
		case 0xFFFC: gray_status[1] = -10; break;     //000000000011b
		case 0xFFFD: gray_status[1] = -9;  break;     //000000000010b
		case 0xFFF8: gray_status[1] = -9;  break;     //000000000111b
		case 0xFFF9: gray_status[1] = -8;  break;     //000000000110b
		case 0xFFFB: gray_status[1] = -7;  break;     //000000000100b
		case 0xFFF1: gray_status[1] = -7;  break;     //000000001110b
		case 0xFFF3: gray_status[1] = -6;  break;     //000000001100b
		case 0xFFF7: gray_status[1] = -5;  break;     //000000001000b
		case 0xFFE3: gray_status[1] = -5;  break;     //000000011100b
		case 0xFFE7: gray_status[1] = -4;  break;     //000000011000b
		case 0xFFEF: gray_status[1] = -3;  break;     //000000010000b
		case 0xFFC7: gray_status[1] = -3;  break;     //000000111000b
		case 0xFFCF: gray_status[1] = -2;  break;     //000000110000b
		case 0xFFDF: gray_status[1] = -1;  break;     //000000100000b
		case 0xFF8F: gray_status[1] = -1;  break;     //000001110000b
		case 0xFF9F: gray_status[1] = 0;   break;     //000001100000b
		case 0xFFBF: gray_status[1] = 1;   break;     //000001000000b
		case 0xFF1F: gray_status[1] = 1;   break;     //000011100000b
		case 0xFF3F: gray_status[1] = 2;   break;     //000011000000b
		case 0xFF7F: gray_status[1] = 3;   break;     //000010000000b
		case 0xFE3F: gray_status[1] = 3;   break;     //000111000000b
		case 0xFE7F: gray_status[1] = 4;   break;     //000110000000b
		case 0xFEFF: gray_status[1] = 5;   break;     //000100000000b
		case 0xFC7F: gray_status[1] = 5;   break;     //001110000000b
		case 0xFCFF: gray_status[1] = 6;   break;     //001100000000b
		case 0xFDFF: gray_status[1] = 7;   break;     //001000000000b
		case 0xF8FF: gray_status[1] = 7;   break;     //011100000000b
		case 0xF9FF: gray_status[1] = 8;   break;     //011000000000b
		case 0xFBFF: gray_status[1] = 9;   break;     //010000000000b
		case 0xF1FF: gray_status[1] = 9;   break;     //111000000000b
		case 0xF3FF: gray_status[1] = 10;  break;     //110000000000b
		case 0xF7FF: gray_status[1] = 11;  break;     //100000000000b
		case 0xFFFF:
			
		default://其它特殊情况单独判断
		{
			gray_status[1]=0;
		}
	}	
}

int16_t  readTrackDate(uint16_t dat)
{
    
    if (dat == 0xFFFF) {
        return -45;  
    }

    char x1 = 0, x2 = 0;
    for (int8_t i = 11; i >= 0; i--)  
    {
        if (x1 == 0)  
        {
            if ((dat & (1 << i)) != 0)  
            {
                x1 = i * 4 + 1;     
            }
        }
        else  
        {
            if ((dat & (1 << i)) == 0)  
            {
                x2 = (i + 1) * 4 + 1;
                break;
            }
        }
    }
    if (dat == 0xF000) {
        return -45;  
    }
//		if(x1 ==  1){
//		return -23.5;}
//		if(x1 == 45 || x1 ==41 || x1 ==37)
//		{
//			Differential_Wheel_Info.mode = slow_angle;
//		}

    return (22.5-((x1 + x2) / 2.0f));  
		
}


void phototube_proceed(void)
{
for (int8_t i = 0; i < 12; i++) {
    if (gpio_get_level(gray_bits[i])) {
       gray_state.state |= (1 << i);  // ?? 
    } else {
       gray_state.state &= ~(1 << i);   // ?? 
    }}
for(int8_t j = 12 ; j < 16 ; j++){
		gray_state.state |= (1 << j); 
}
//    mspm0_i2c_read(pca9555_Slave_Addr,pca9555_INPUT_PORT_REGISTER0,2,&gray_state.state);
//		result =  gray_state.state;
//		gpio_input_check_channel_12_linewidth_20mm();
//		Cha_error = readTrackDate(gray_state.state);
}