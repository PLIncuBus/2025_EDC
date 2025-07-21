#include "IMU.h"


/**
 * @brief IMU初始化
 * @note  IMU初始化不成功会导致程序卡死在这里
 */
void IMU_Init(void)
{
    while(imu963ra_init());
}

/**
 * @brief IMU姿态解算
 * 
 */
float GyroOffsetYdata = 0, GyroOffsetZdata = 0, GyroOffsetXdata = 0; // 陀螺仪零漂值
float Acc_x = 0, Acc_y = 0, Acc_z = 0, Gyro_x = 0, Gyro_y = 0, Gyro_z = 0, Gyro_x_v = 0, Gyro_y_v = 0, Gyro_z_v = 0;
void IMU_Attitude_Process(void)
{

    //数据获取
    imu963ra_get_acc(); 
    imu963ra_get_gyro();



}


void IcmTransition(void)
{
    float alpha = 0.9;
    // 一阶低通滤波，单位g
    Acc_x = (((float)imu963ra_acc_x) * alpha) / 4096 + Acc_x * (1 - alpha);
    Acc_y = (((float)imu963ra_acc_y) * alpha) / 4096 + Acc_y * (1 - alpha);
    Acc_z = (((float)imu963ra_acc_z) * alpha) / 4096 + Acc_z * (1 - alpha);

    // 陀螺仪角速度必须转换为弧度制角速度: deg/s -> rad/s
    Gyro_x = ((float)imu963ra_gyro_x - GyroOffsetXdata) * PI / 180 / 16.4f;
    Gyro_y = ((float)imu963ra_gyro_y - GyroOffsetYdata) * PI / 180 / 16.4f;
    Gyro_z = ((float)imu963ra_gyro_z - GyroOffsetZdata) * PI / 180 / 16.4f;
	
	Gyro_z_lifter=imu_alpha * Gyro_z + (1 - imu_alpha) * Gyro_z;
	
	Gyro_z=Gyro_z_lifter;
	
	//角速度 角度制 数量级在200 Gyro_z 是0-4
	Gyro_z_v=((float)imu963ra_gyro_z - GyroOffsetZdata)/16.4f;
}



