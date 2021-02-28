/*
 * mpu6050.c
 *
 *  Created on: Feb 25, 2021
 *      Author: Rullyan Gabriel dos Santos
 */

#include "mpu6050.h"

#define MPU6050_ADDR 0xD0

#define SMPLRT_DIV_REG 0x19
#define GYRO_CONFIG_REG 0x1B
#define ACCEL_CONFIG_REG 0x1C
#define ACCEL_XOUT_H_REG 0x3B
#define TEMP_OUT_H_REG 0x41
#define GYRO_XOUT_H_REG 0x43
#define PWR_MGMT_1_REG 0x6B
#define WHO_AM_I_REG 0x75

void MPU6050_Init (I2C_HandleTypeDef *I2Cx){
	uint8_t check;
	uint8_t Data;

	/* Check device ID WHO_AM_I */
	HAL_I2C_Mem_Read (I2Cx, MPU6050_ADDR,WHO_AM_I_REG,1, &check, 1, 1000);

	if (check == 114)  // 0x68 will be returned by the sensor if everything goes well
	{
		/* power management register 0X6B we should write all 0's to wake the sensor up */
		Data = 0;
		HAL_I2C_Mem_Write(I2Cx, MPU6050_ADDR, PWR_MGMT_1_REG, 1,&Data, 1, 1000);

		/* Set DATA RATE of 1KHz by writing SMPLRT_DIV register */
		Data = 0x07;
		HAL_I2C_Mem_Write(I2Cx, MPU6050_ADDR, SMPLRT_DIV_REG, 1, &Data, 1, 1000);

		/* Set accelerometer configuration in ACCEL_CONFIG Register
		   XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0 -> � 2g */
		Data = 0x00;
		HAL_I2C_Mem_Write(I2Cx, MPU6050_ADDR, ACCEL_CONFIG_REG, 1, &Data, 1, 1000);

	  	/* Set Gyroscopic configuration in GYRO_CONFIG Register
		   XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> � 250 �/s */
		Data = 0x00;
		HAL_I2C_Mem_Write(I2Cx, MPU6050_ADDR, GYRO_CONFIG_REG, 1, &Data, 1, 1000);
	}
}

void MPU6050_Read_Accel (I2C_HandleTypeDef *I2Cx, MPU6050_t *DataStruct)
{
	uint8_t Rec_Data[6];

	/* Read 6 BYTES of data starting from ACCEL_XOUT_H register */

	HAL_I2C_Mem_Read (I2Cx, MPU6050_ADDR, ACCEL_XOUT_H_REG, 1, Rec_Data, 6, 1000);

	DataStruct->Accel_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data [1]);
	DataStruct->Accel_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data [3]);
	DataStruct->Accel_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data [5]);

	/* convert the RAW values into acceleration in 'g'
       we have to divide according to the Full scale value set in FS_SEL
       I have configured FS_SEL = 0. So I am dividing by 16384.0
       for more details check ACCEL_CONFIG Register */

	DataStruct->Ax = DataStruct->Accel_X_RAW/16384.0;
	DataStruct->Ay = DataStruct->Accel_Y_RAW/16384.0;
	DataStruct->Az = DataStruct->Accel_Z_RAW/16384.0;
}

void MPU6050_Read_Gyro (I2C_HandleTypeDef *I2Cx, MPU6050_t *DataStruct)
{
	uint8_t Rec_Data[6];

	// Read 6 BYTES of data starting from GYRO_XOUT_H register

	HAL_I2C_Mem_Read (I2Cx, MPU6050_ADDR, GYRO_XOUT_H_REG, 1, Rec_Data, 6, 1000);

	DataStruct->Gyro_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data [1]);
	DataStruct->Gyro_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data [3]);
	DataStruct->Gyro_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data [5]);

	/* convert the RAW values into dps (�/s)
       we have to divide according to the Full scale value set in FS_SEL
       I have configured FS_SEL = 0. So I am dividing by 131.0
       for more details check GYRO_CONFIG Register */

	DataStruct->Gx = DataStruct->Gyro_X_RAW/131.0;
	DataStruct->Gy = DataStruct->Gyro_Y_RAW/131.0;
	DataStruct->Gz = DataStruct->Gyro_Z_RAW/131.0;
}
