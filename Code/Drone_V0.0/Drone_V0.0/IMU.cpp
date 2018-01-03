/**
******************************************************************************
* File Name         : IMU.cpp
* Description       : Object for all IMU objects (MPU9255, AK8963, BMP180)
* Author			: Juan Morency Trudel
* Version           : 1.0.0
* Date				: June 2017
******************************************************************************
*/

#include "IMU.h"

bool imuInitialized  = false;
uint32_t sumCountAk8963 = 0, sumCountMpu9255 = 0, sumCountBmp180 = 0; // for update freq calculation
float sumAk8963 = 0.0f, sumMpu9255 = 0.0f, sumBmp180 = 0.0f;        // for update freq calculation

//constructor
IMU::IMU()
{
}

/**
	* @brief Initialize the different sensors and also measures the initial yaw orientation
	*/
void IMU::initialize()
{
	mpu9255.initialize();
	ak8963.initialize();
	bmp180.initialize();
	
	//not yet ready
	// Measure the initial yaw orientation
	//
	//mpu9255.measureMeanRawAccelerationAndRotation();
	//ak8963.measureMeanRawMagneticField();
	//currentRawAcc = mpu9255.getRawAcceleration();
	//currentRawGyr = mpu9255.getRawRotation();
	//currentRawMag = ak8963.getRawMagneticField();
	//sei();
	//for(int i = 0; i < 10; i++)
	//{
		//mpu9255DataReady = true;
		//updateImuAndMadgwick();
		//_delay_ms(1);
		//ak8963DataReady = true;
		//updateImuAndMadgwick();
		//_delay_ms(1);
		//sprintf(buffer, "test");
		//FloatToString(floatbuff, deltaTimeMpu9255);
		//strcat (buffer,"\t\t delta1:");
		//strcat (buffer,floatbuff);
		//FloatToString(floatbuff, deltaTimeAk8963);
		//strcat (buffer,"\t delta2:");
		//strcat (buffer,floatbuff);
		//serialTransmit(buffer);
		//_delay_ms(1);
	//}
//
	////print yaw pitch roll
//
	//// print quaternions
	//sprintf(buffer, " MEAN STARTING:");
	//FloatToString(floatbuff, q0);
	//strcat (buffer,"\t\t q0:");
	//strcat (buffer,floatbuff);
	//FloatToString(floatbuff, q1);
	//strcat (buffer,"\t q1:");
	//strcat (buffer,floatbuff);
	//FloatToString(floatbuff, q2);
	//strcat (buffer,"\t q2:");
	//strcat (buffer,floatbuff);
	//FloatToString(floatbuff, q3);
	//strcat (buffer,"\t q3:");
	//strcat (buffer,floatbuff);
	//cli();			
	//sprintf(buffer, " MEAN STARTING:");
	//FloatToString(floatbuff, roll);
	//strcat (buffer,"roll:");
	//strcat (buffer,floatbuff);
	//FloatToString(floatbuff, pitch);
	//strcat (buffer,"\t pitch:");
	//strcat (buffer,floatbuff);
	//FloatToString(floatbuff, yaw);
	//strcat (buffer,"\t yaw:");
	//strcat (buffer,floatbuff);
	//strcat (buffer,"\n");
	//serialTransmit(buffer);
	
	imuInitialized  = true;
}

/**
	* @brief Main IMU function that should be called all the time in the main while loop. It updates the values
			of the sensors, calculates the real values from the raw reading and does the Madgwick algorithm to
			calculate the Euler angles from the thes IMU readings when the data is ready. The dataReady flags are
			set in the interrupt.cpp file in the ISR(TWI_vect) when the data is ready. The time since the last
			update is also measured independently for each sensor to get the effective update rate of each
			sensor. This is done by dividing sum variable (which is the total time since the last sensor rate 
			update) by the sumCount variable (which is the number of readings taken since
			the last sensor rate update). These variables are global and can be used anywhere to measure this
			update rate. These values must be put to 0 after the rate is taken. 
	*/
void IMU::updateImuAndMadgwick()
{
	if(ak8963DataReady)
	{
		ak8963DataReady = false;
		ak8963.setRawMagneticField(currentRawMag);
		ak8963.calculateMag();

		//calculate the time it takes before the next call of this function
		Now = TCNT0;
		//the 8 is counter 0 prescaler
		deltaTimeAk8963 = ((Now - lastUpdateAk8963 + OCR0A*timer0OverflowCountAk8963)*8/(float)F_CPU);
		timer0OverflowCountAk8963 = 0;
		lastUpdateAk8963 = Now;
		sumAk8963 += deltaTimeAk8963; // sum for averaging filter update rate
		sumCountAk8963++;

		// Page 38 of PS-MPU-9255 gives the axes orientations
		// the x axis on the MPU9255 represents the forward direction. Since on our board the front is actually the y
		// direction, a 90 degree rotation clockwise has to be made. For this x = y and y = -x for all 3 sensors.
		// on top of this, to make sure the convention of positive pitch yaw roll is respected, the final x is multiplied by -1
		// for all sensors (x,y,z) becomes (-y,-x,z)
		
		// for the magnetometer, an extra modification is required	
		// Sensors x (y)-axis of the accelerometer/gyro is aligned with the y (x)-axis of the magnetometer;
		// the magnetometer z-axis (+ down) is misaligned with z-axis (+ up) of accelerometer and gyro!
		// so we interchange x and y and multiply the Z axis by -1.
		// the total change is (x,y,z) becomes (-x,-y,-z))
		
		xMagneticFieldMadgwick = -(float)ak8963.getMagneticFieldX();
		yMagneticFieldMadgwick = -(float)ak8963.getMagneticFieldY();
		zMagneticFieldMadgwick = -(float)ak8963.getMagneticFieldZ();		
		
		MadgwickAHRSupdate(xAccelerationMadgwick, yAccelerationMadgwick, zAccelerationMadgwick,
							xRotationMadgwick, yRotationMadgwick, zRotationMadgwick,
							xMagneticFieldMadgwick, yMagneticFieldMadgwick, zMagneticFieldMadgwick);
		calcEulerAngles();
	}

	else if(bmp180DataReady)
	{
		bmp180DataReady = false;
		bmp180.setRawTemperature(currentRawTemp);
		bmp180.setRawPressure(currentRawPress);
		bmp180.calculateTrueValues();
		//calculate the time it takes before the next call of this function
		Now = TCNT0;
		//the 8 is counter 0 prescaler
		deltaTimeBmp180 = ((Now - lastUpdateBmp180 + OCR0A*timer0OverflowCountBmp180)*8/(float)F_CPU);
		timer0OverflowCountBmp180 = 0;
		lastUpdateBmp180 = Now;
		sumBmp180 += deltaTimeBmp180; // sum for averaging filter update rate
		sumCountBmp180++;		
	}

	else if(mpu9255DataReady)
	{
		mpu9255DataReady = false;
		//These functions updates the values of the class variables with the FSM buffer variables
		mpu9255.setRawAcceleration(currentRawAcc);
		mpu9255.setRawRotation(currentRawGyr);

		mpu9255.calculateAccRotTemp();

		//calculate the time it takes before the next call of this function
		Now = TCNT0;
		//the 8 is counter 0 prescaler
		deltaTimeMpu9255 = ((Now - lastUpdateMpu9255 + OCR0A*timer0OverflowCountMpu9255)*8/(float)F_CPU);
		timer0OverflowCountMpu9255 = 0;
		lastUpdateMpu9255 = Now;
		sumMpu9255 += deltaTimeMpu9255; // sum for averaging filter update rate
		sumCountMpu9255++;
		xAccelerationMadgwick = -(float)mpu9255.getAccelerationY()/10000;
		yAccelerationMadgwick = -(float)mpu9255.getAccelerationX()/10000;
		zAccelerationMadgwick = (float)mpu9255.getAccelerationZ()/10000;
		xRotationMadgwick = -(float)mpu9255.getRotationY()*M_PI_OVER_1800;
		yRotationMadgwick = -(float)mpu9255.getRotationY()*M_PI_OVER_1800;
		zRotationMadgwick = (float)mpu9255.getRotationZ()*M_PI_OVER_1800;
		
		MadgwickAHRSupdateIMU(xAccelerationMadgwick, yAccelerationMadgwick, zAccelerationMadgwick,
								xRotationMadgwick, yRotationMadgwick, zRotationMadgwick);
		calcEulerAngles();
	}
}




