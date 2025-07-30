#include "Encoder.h"

static void Encoder1_EXTI_Callback(uint32 state, void *ptr);
static void Encoder2_EXTI_Callback(uint32 state, void *ptr);
/**
 * @brief 编码器初始化
 * 
 */
void Encoder_Init(void)
{
    exti_init(ENCODER1_POSITIVE, EXTI_TRIGGER_FALLING , Encoder1_EXTI_Callback, NULL);                           
    gpio_init(ENCODER1_NEGATIVE, GPI, GPIO_HIGH, GPI_PULL_UP); 

    exti_init(ENCODER2_POSITIVE, EXTI_TRIGGER_FALLING , Encoder2_EXTI_Callback, NULL);                            
    gpio_init(ENCODER2_NEGATIVE, GPI, GPIO_HIGH, GPI_PULL_UP);

}

/**
 * @brief 编码器计数
 * 
 */
int16_t Encoder_count[Encoder_Sum];
int32_t Encoder_count_sum[Encoder_Sum];
void Encoder1_EXTI_Callback(uint32 state, void *ptr)
{
    if(gpio_get_level(ENCODER1_NEGATIVE) == 1){
        Encoder_count[Encoder1]++;
				Encoder_count_sum[Encoder1]++;
    }
    else if(gpio_get_level(ENCODER1_NEGATIVE) == 0){
        Encoder_count[Encoder1]--;
				Encoder_count_sum[Encoder1]--;
    }

}

void Encoder2_EXTI_Callback(uint32 state, void *ptr)
{
    if(gpio_get_level(ENCODER2_NEGATIVE) == 1){
        Encoder_count[Encoder2]--;
			Encoder_count_sum[Encoder2]--;
    }
    else{
        Encoder_count[Encoder2]++;
			Encoder_count_sum[Encoder2]++;
    }

}

int16_t Encoder_Count_Get(Encoder_enum index){
    return Encoder_count[index];
}
