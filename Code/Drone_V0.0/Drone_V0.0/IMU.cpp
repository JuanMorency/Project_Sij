#include "IMU.h"

int16_t tempRaw;
uint32_t timer;
uint8_t i2cData[14];
bool imuInitialized  = false;
uint32_t sumCountAk8963 = 0, sumCountMpu9255 = 0, sumCountBmp180 = 0; // for update freq calculation
float sumAk8963 = 0.0f, sumMpu9255 = 0.0f, sumBmp180 = 0.0f;        // for update freq calculation

//constructor
IMU::IMU()
{
}
void IMU::initialize()
{
	mpu9255.initialize();
	ak8963.initialize();
	bmp180.initialize();
	imuInitialized  = true;
}

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
		

		MadgwickAHRSupdate(-(float)mpu9255.getAccelerationY()/10000, -(float)mpu9255.getAccelerationX()/10000, (float)mpu9255.getAccelerationZ()/10000,
		-(float)mpu9255.getRotationY()*M_PI/1800.0f, -(float)mpu9255.getRotationY()*M_PI/1800.0f, (float)mpu9255.getRotationZ()*M_PI/1800.0f,
		-(float)ak8963.getMagneticFieldX(), -(float)ak8963.getMagneticFieldY(), -(float)ak8963.getMagneticFieldZ());
		//old without
		//MadgwickAHRSupdate((float)mpu9255.getAccelerationY()/10000, -(float)mpu9255.getAccelerationX()/10000, (float)mpu9255.getAccelerationZ()/10000,
		//(float)mpu9255.getRotationY()*M_PI/1800.0f, -(float)mpu9255.getRotationY()*M_PI/1800.0f, (float)mpu9255.getRotationZ()*M_PI/1800.0f,
		//(float)ak8963.getMagneticFieldX(), -(float)ak8963.getMagneticFieldY(), -(float)ak8963.getMagneticFieldZ());
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

		MadgwickAHRSupdateIMU(-(float)mpu9255.getAccelerationY()/10000, -(float)mpu9255.getAccelerationX()/10000, (float)mpu9255.getAccelerationZ()/10000,
		-(float)mpu9255.getRotationY()*M_PI/1800.0f, -(float)mpu9255.getRotationY()*M_PI/1800.0f, (float)mpu9255.getRotationZ()*M_PI/1800.0f);
		
		//calcEulerAngles();
	}
}




