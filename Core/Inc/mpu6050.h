/*
 * mpu6050.h
 *
 *  Created on: Feb 25, 2021
 *      Author: Rullyan Gabriel dos Santos
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#endif /* INC_MPU6050_H_ */

#include <stdint.h>
#include "main.h"

//MPU6050 structure
typedef struct {

	int16_t Accel_X_RAW;
	int16_t Accel_Y_RAW;
	int16_t Accel_Z_RAW;
	float Ax;
	float Ay;
	float Az;

	int16_t Gyro_X_RAW;
	int16_t Gyro_Y_RAW;
	int16_t Gyro_Z_RAW;
	float Gx;
	float Gy;
	float Gz;

}MPU6050_t;

void MPU6050_Init (I2C_HandleTypeDef *I2Cx);
void MPU6050_Read_Accel (I2C_HandleTypeDef *I2Cx, MPU6050_t *DataStruct);
void MPU6050_Read_Gyro (I2C_HandleTypeDef *I2Cx, MPU6050_t *DataStruct);
