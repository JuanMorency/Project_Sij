#include "IMU.h"

int16_t tempRaw;
uint32_t timer;
uint8_t i2cData[14];
bool InertMUInitialized  = false;
uint32_t sumCount = 0; // for update freq calculation
float sum = 0.0f;        // for update freq calculation

//constructor
IMU::IMU()
{
	
}
void IMU::initialize()
{
	mpu9255.initialize();
	ak8963.initialize();
	//bmp180.initialize();
	InertMUInitialized  = true;
}

void IMU::takeMeasures()
{
	mpu9255.updateRawData();			
	//mpu9255.calculateAccRotTemp();
	//ak8963.updateRawData();
	//ak8963.calculateMag();
	//
	////this function updates the raw values on it's own with a state machine. 
	////it is assumed that the delay between calls will be greater than 10 ms
	////which is the time required for the conversion of the pressure
	////this function has to be called 5 times for the value to actually update
	//bmp180.CalculateTemperaturePressureAndAltitude();
	//
	this->acc.X = mpu9255.getAcceleration().X;
	this->acc.Y = mpu9255.getAcceleration().Y;
	this->acc.Z = mpu9255.getAcceleration().Z;
	this->rot.X = mpu9255.getRotation().X;
	this->rot.Y = mpu9255.getRotation().Y;
	this->rot.Z = mpu9255.getRotation().Z;
	this->mag.X = ak8963.getMagneticField().X;
	this->mag.Y = ak8963.getMagneticField().Y;
	this->mag.Z = ak8963.getMagneticField().Z;
	this->pres =  bmp180.getPressure();
	this->temp =  bmp180.getTemperature();
	this->alt =  bmp180.getAltitude();
 
	//calculate the time it takes for all takes for before the next call of this function
	Now = TCNT0;
	//the 8 is counter 0 prescaler
	deltat = ((Now - lastUpdate + OCR0A*timer0_overflow_count)*8/(float)F_CPU);
	timer0_overflow_count = 0;
	lastUpdate = Now;

	sum += deltat; // sum for averaging filter update rate
	sumCount++;

	// Page 38 of PS-MPU-9255 gives the axes orientations
	// Sensors x (y)-axis of the accelerometer/gyro is aligned with the y (x)-axis of the magnetometer;
	// the magnetometer z-axis (+ down) is misaligned with z-axis (+ up) of accelerometer and gyro!
	// so we inverse the Z of the magnetometer and 
  
	// the x axis on the MPU9255 represents the forward direction. Since on our board the front is actually the y 
	// direction, a 90 degree rotation clockwise has to be made. For this x = y and y = -x for all 3 sensors.
	// the magnetometer becomes x, -y, -z
	MadgwickAHRSupdate((float)acc.Y/10000, -(float)acc.X/10000, (float)acc.Z/10000, (float)rot.Y*M_PI/1800.0f, -(float)rot.X*M_PI/1800.0f,
	(float)rot.Z*M_PI/1800.0f, mag.X, -mag.Y, -mag.Z);
	calcEulerAngles();
}




