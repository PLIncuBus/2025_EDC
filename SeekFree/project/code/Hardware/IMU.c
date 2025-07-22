#include "IMU.h"

float GyroOffsetYdata=0, GyroOffsetZdata=0,GyroOffsetXdata=0;	//陀螺仪零漂值
float Acc_x=0, Acc_y=0, Acc_z=0, Gyro_x=0, Gyro_y=0, Gyro_z=0,Gyro_x_v = 0 ,Gyro_y_v = 0,Gyro_z_v=0;				//储存单位转换之后的数据
float Angle_Yaw=0;              													// 偏航角
float pitch=0;
float roll=0;
static float I_ex=0, I_ey=0, I_ez=0;  														// 误差积分
static float icm_kp= 0.17;    																		// 加速度计的收敛速率比例增益 
static float icm_ki= 0.004;   																		// 陀螺仪收敛速率的积分增益
static float Q_0 = 1; 																						// 四元数初始化
static float Q_1 = 0;
static float Q_2 = 0;
static float Q_3 = 0;
static float delta_T = 0.010; // 采样周期ms 这个要和中断周期对应！！！！！



#if (IMU_Type == jy61p)

static void JY61P_uart_rx_interrupt_Callback(uint32 state, void *ptr);

/**
 * @brief jy61p初始化
 * 
 */
void JY61P_Init(void)
{
    uart_init(JY61P_UART_INDEX,JY61P_UART_BAUNDRATE,JY61P_UART_TX_PIN,JY61P_UART_RX_PIN);
    uart_set_interrupt_config(JY61P_UART_INDEX, UART_INTERRUPT_CONFIG_RX_ENABLE);		// 使能串口接收中断
    interrupt_set_priority(JY61P_UART_PRIORITY, 0);  
    uart_set_callback(JY61P_UART_INDEX, JY61P_uart_rx_interrupt_Callback, NULL);			    // 定义中断接收函数
}








/**
 * @brief JY61P串口回调函数
 * 
 * @param state 
 * @param ptr 
 */
void JY61P_uart_rx_interrupt_Callback(uint32 state, void *ptr)
{
    if()



}


#define IMU_Init JY61P_Init

#elif (IMU_Type == imu963ra)

void IcmTransition(void);
void IcmAHRSupdate(void);
void IcmEulerAngle(void);
void gyroOffsetInit(void);

/**
 * @brief IMU初始化
 * @note  IMU初始化不成功会导致程序卡死在这里
 * @param Offset_Time IMU初始化时间
 */
void IMU_Init(uint8_t Timer , uint16_t Offset_Time)
{
    delta_T = ((float)Timer)/1000;
    while(imu963ra_init());
    Menu_ShowStr(0,0,"IMU INIT...");
    system_delay_ms(Offset_Time);
    gyroOffsetInit();
    Menu_Clear();
    Menu_ShowStr(0,0,"IMU_INIT_SUCESS!");
}

/**
 * @brief IMU姿态解算
 * 
 */
void IMU_Attitude_Process(void)
{

    //数据获取
		imu963ra_get_gyro();
    imu963ra_get_acc(); 
    

    /*******陀螺仪和加速度仪数据进行单位转换并消除零漂*******/
    IcmTransition();

    /*******姿态融合*******/
    IcmAHRSupdate();

    /*******四元数解算姿态角*******/
    IcmEulerAngle();


}


void IcmTransition(void)
{
    float alpha = 0.3;
    // 一阶低通滤波，单位g
    Acc_x = (((float)imu963ra_acc_x) * alpha) / 4096 + Acc_x * (1 - alpha);
    Acc_y = (((float)imu963ra_acc_y) * alpha) / 4096 + Acc_y * (1 - alpha);
    Acc_z = (((float)imu963ra_acc_z) * alpha) / 4096 + Acc_z * (1 - alpha);

    // 陀螺仪角速度必须转换为弧度制角速度: deg/s -> rad/s
    Gyro_x = ((float)imu963ra_gyro_x - GyroOffsetXdata) * PI / 180 / 16.4f;
    Gyro_y = ((float)imu963ra_gyro_y - GyroOffsetYdata) * PI / 180 / 16.4f;
    Gyro_z = ((float)imu963ra_gyro_z - GyroOffsetZdata) * PI / 180 / 16.4f;
	
	
//		Gyro_z_lifter=imu_alpha * Gyro_z + (1 - imu_alpha) * Gyro_z;
//	
//		Gyro_z=Gyro_z_lifter;
//	
//		//角速度 角度制 数量级在200 Gyro_z 是0-4
//		Gyro_z_v=((float)imu963ra_gyro_z - GyroOffsetZdata)/16.4f;
}

void IcmAHRSupdate(void)
{
    float halfT = 0.5 * delta_T;    // 采样周期一半
    float vx, vy, vz;               // 当前姿态计算得来的重力在三轴上的分量
    float ex, ey, ez;               // 当前加速计测得的重力加速度在三轴上的分量与用当前姿态计算得来的重力在三轴上的分量的误差
    
    float q0 = Q_0;  								//四元数
    float q1 = Q_1;
    float q2 = Q_2;
    float q3 = Q_3;
    
    float q0q0 = q0 * q0;  					//先相乘，方便后续计算
    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    float q0q3 = q0 * q3;
    float q1q1 = q1 * q1;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q3q3 = q3 * q3;
		
		float norm;

    // 正常静止状态为-g 反作用力。
    if(Acc_x * Acc_y * Acc_z == 0) // 加计处于自由落体状态时(此时g = 0)不进行姿态解算，因为会产生分母无穷大的情况
        return;

    // 对加速度数据进行归一化 得到单位加速度 (a^b -> 载体坐标系下的加速度)
    norm = myRsqrt(Acc_x * Acc_x + Acc_y * Acc_y + Acc_z * Acc_z); 
    Acc_x = Acc_x * norm;
    Acc_y = Acc_y * norm;
    Acc_z = Acc_z * norm;

    // 载体坐标系下重力在三个轴上的分量
    vx = 2 * (q1q3 - q0q2);
    vy = 2 * (q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;

    // g^b 与 a^b 做向量叉乘，得到陀螺仪的校正补偿向量e的系数
    ex = Acc_y * vz - Acc_z * vy;
    ey = Acc_z * vx - Acc_x * vz;
    ez = Acc_x * vy - Acc_y * vx;

    // 误差累加
    I_ex += halfT * ex;  
    I_ey += halfT * ey;
    I_ez += halfT * ez;

    // 使用PI控制器消除向量积误差(陀螺仪漂移误差)
    Gyro_x = Gyro_x + icm_kp* ex + icm_ki* I_ex;
    Gyro_y = Gyro_y + icm_kp* ey + icm_ki* I_ey;
    Gyro_z = Gyro_z + icm_kp* ez + icm_ki* I_ez;

    // 一阶龙格库塔法求解四元数微分方程，其中halfT为测量周期的1/2，gx gy gz为b系陀螺仪角速度。
    q0 = q0 + (-q1 * Gyro_x - q2 * Gyro_y - q3 * Gyro_z) * halfT;
    q1 = q1 + (q0 * Gyro_x + q2 * Gyro_z - q3 * Gyro_y) * halfT;
    q2 = q2 + (q0 * Gyro_y - q1 * Gyro_z + q3 * Gyro_x) * halfT;
    q3 = q3 + (q0 * Gyro_z + q1 * Gyro_y - q2 * Gyro_x) * halfT;

    // 单位化四元数在空间旋转时不会拉伸，仅有旋转角度，下面算法类似线性代数里的正交变换
    norm = myRsqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    Q_0 = q0 * norm;
    Q_1 = q1 * norm;
    Q_2 = q2 * norm;
    Q_3 = q3 * norm;
}

void IcmEulerAngle(void)
{
	float q0 = Q_0;
	float q1 = Q_1;
	float q2 = Q_2;
	float q3 = Q_3;
	
	// atan2返回输入坐标点与坐标原点连线与X轴正方形夹角的弧度值
	pitch = asin(2 * q0 * q2 - 2 * q1 * q3) * 180 / PI; 
	roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 180 / PI; 
  Angle_Yaw = atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2 * q2 - 2 * q3 * q3 + 1) * 180 / PI; 
}

/*************************************************************************
 *  函数名称：void gyroOffsetInit(void)
 *  功能说明：陀螺仪清零漂
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2024年8月22日
 *  备    注：
 *************************************************************************/
void gyroOffsetInit(void)
{
    int Gnum;
    GyroOffsetXdata = 0;
    GyroOffsetYdata = 0;
    GyroOffsetZdata = 0;

    for (Gnum = 0; Gnum < 1000; Gnum++)
    {
        /*******获取陀螺仪和加速度仪数据*******/
        imu963ra_get_gyro();
        imu963ra_get_acc();
        GyroOffsetXdata += imu963ra_gyro_x;
        GyroOffsetYdata += imu963ra_gyro_y;
        GyroOffsetZdata += imu963ra_gyro_z;
        system_delay_ms(1);
    }

    GyroOffsetXdata /= 1000;
    GyroOffsetYdata /= 1000;
    GyroOffsetZdata /= 1000;
}

/*************************************************************************
 *  函数名称：void AttitudeCalculate(void)
 *  功能说明：陀螺仪角加速度换算真实值
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2024年8月22日
 *  备    注：在没使用姿态解算又要利用角加速度时调用
 *************************************************************************/
void AttitudeCalculate(void)
{
    /*******获取陀螺仪和加速度仪数据*******/
    imu963ra_get_gyro();
    Gyro_z = (((float)imu963ra_gyro_z - GyroOffsetZdata) * PI / 180 / 16.4f);
	
	//角速度 角度制
		Gyro_z_v=((float)imu963ra_gyro_z - GyroOffsetZdata);
}


void ImuCalculate(float *error)
{
				if(*error>179)
			{
				*error-=360;
			}
			else if(*error<-179)
			{
				*error+=360;
			}
			
			*error/=180; //归一化
}
#endif