
//This code is for 10 DOF IMU with (MPU6050,HMC5883L,BMP085)
// it uses Kalman filter for MPU6050

#include "Kalman.h" 
#include <Wire.h>
#include <HMC5883L.h>
#define BMP085_ADDRESS 0x77
#define RESTRICT_PITCH 
HMC5883L compass;
int error = 0;
int ac1 ,ac2 ,ac3;
int b1,b2,mb,mc,md,b5;
const unsigned char OSS = 0;  
unsigned int ac4 ,ac5 ,ac6;
Kalman kalmanX; Kalman kalmanY;
double accX, accY, accZ,gyroX, gyroY, gyroZ;
double gyroXangle, gyroYangle,compAngleX, compAngleY,kalAngleX, kalAngleY;
int16_t tempRaw;
uint32_t timer;
uint8_t i2cData[14];


void setup() {
  Serial.begin(115200);
  Wire.begin();
  bmp085Calibration();
  TWBR = ((F_CPU / 400000L) - 16) / 2; 
  i2cData[0] = 7; 
  i2cData[1] = 0x00; 
  i2cData[2] = 0x00; 
  i2cData[3] = 0x00; 
  while (i2cWrite(0x19, i2cData, 4, false)); 
  while (i2cWrite(0x6B, 0x01, true)); 
  while (i2cRead(0x75, i2cData, 1));
  if (i2cData[0] != 0x68) { 
  Serial.print(F("Error reading sensor"));
  while (1);
  Serial.println("Starting the I2C interface.");
  Serial.println("Constructing new HMC5883L");
  compass = HMC5883L(); 
  Serial.println("Setting scale to +/- 1.3 Ga");
  error = compass.SetScale(1.3); 
  if(error != 0) 
  Serial.println(compass.GetErrorText(error));
  Serial.println("Setting measurement mode to continous.");
  error = compass.SetMeasurementMode(Measurement_Continuous); 
  if(error != 0) 
  Serial.println(compass.GetErrorText(error));
  }

  delay(100); 
  while (i2cRead(0x3B, i2cData, 6));
  accX = (i2cData[0] << 8) | i2cData[1];
  accY = (i2cData[2] << 8) | i2cData[3];
  accZ = (i2cData[4] << 8) | i2cData[5];
#ifdef RESTRICT_PITCH 
  double roll  = atan2(accY, accZ) * RAD_TO_DEG;
  double pitch = atan(-accX / sqrt(accY * accY + accZ * accZ)) * RAD_TO_DEG;
#else 
  double roll  = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
  double pitch = atan2(-accX, accZ) * RAD_TO_DEG;
#endif
  kalmanX.setAngle(roll); // Set starting angle
  kalmanY.setAngle(pitch);
  gyroXangle = roll;
  gyroYangle = pitch;
  compAngleX = roll;
  compAngleY = pitch;
  timer = micros();
}

void loop() {
  
      //****************** BMP085 *****************\\
 
  float temperature = bmp085GetTemperature(bmp085ReadUT()); //MUST be called first
  float pressure = bmp085GetPressure(bmp085ReadUP());
  float atm = pressure / 101325; // "standard atmosphere"
  float altitude = calcAltitude(pressure); //Uncompensated caculation - in Meters 
  Serial.print("Temperature: ");
  Serial.print(temperature, 2); //display 2 decimal places
  Serial.println("deg C");
  Serial.print("Standard Atmosphere: ");
  Serial.println(atm, 4); //display 4 decimal places
  Serial.print("Altitude: ");
  Serial.print(altitude, 2); //display 2 decimal places
  Serial.println(" M");
  Serial.println();//line break
  delay(500); //wait a second and get values again.
 
 //******************** MBU6050 ************************\\
 
  while (i2cRead(0x3B, i2cData, 14));
  accX = ((i2cData[0] << 8) | i2cData[1]);
  accY = ((i2cData[2] << 8) | i2cData[3]);
  accZ = ((i2cData[4] << 8) | i2cData[5]);
  tempRaw = (i2cData[6] << 8) | i2cData[7];
  gyroX = (i2cData[8] << 8) | i2cData[9];
  gyroY = (i2cData[10] << 8) | i2cData[11];
  gyroZ = (i2cData[12] << 8) | i2cData[13];
  double dt = (double)(micros() - timer) / 1000000; // Calculate delta time
  timer = micros();
#ifdef RESTRICT_PITCH // Eq. 25 and 26
  double roll  = atan2(accY, accZ) * RAD_TO_DEG;
  double pitch = atan(-accX / sqrt(accY * accY + accZ * accZ)) * RAD_TO_DEG;
#else // Eq. 28 and 29
  double roll  = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
  double pitch = atan2(-accX, accZ) * RAD_TO_DEG;
#endif
  double gyroXrate = gyroX / 131.0; // Convert to deg/s
  double gyroYrate = gyroY / 131.0; // Convert to deg/s
#ifdef RESTRICT_PITCH
 if ((roll < -90 && kalAngleX > 90) || (roll > 90 && kalAngleX < -90)) {
    kalmanX.setAngle(roll);
    compAngleX = roll;
    kalAngleX = roll;
    gyroXangle = roll;
  } else
    kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt); // Calculate the angle using a Kalman filter
  if (abs(kalAngleX) > 90)
    gyroYrate = -gyroYrate; // Invert rate, so it fits the restriced accelerometer reading
    kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt);
#else
  if ((pitch < -90 && kalAngleY > 90) || (pitch > 90 && kalAngleY < -90)) {
    kalmanY.setAngle(pitch);
    compAngleY = pitch;
    kalAngleY = pitch;
    gyroYangle = pitch;
  } else
    kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt); // Calculate the angle using a Kalman filter
  if (abs(kalAngleY) > 90)
    gyroXrate = -gyroXrate; // Invert rate, so it fits the restriced accelerometer reading
  kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt); // Calculate the angle using a Kalman filter
#endif
  gyroXangle += gyroXrate * dt; // Calculate gyro angle without any filter
  gyroYangle += gyroYrate * dt;
  compAngleX = 0.93 * (compAngleX + gyroXrate * dt) + 0.07 * roll; // Calculate the angle using a Complimentary filter
  compAngleY = 0.93 * (compAngleY + gyroYrate * dt) + 0.07 * pitch;
  if (gyroXangle < -180 || gyroXangle > 180)
  gyroXangle = kalAngleX;
  if (gyroYangle < -180 || gyroYangle > 180)
  gyroYangle = kalAngleY;
#if 0 // Set to 1 to activate
  Serial.print(accX); Serial.print("\t");
  Serial.print(accY); Serial.print("\t");
  Serial.print(accZ); Serial.print("\t");
  Serial.print(gyroX); Serial.print("\t");
  Serial.print(gyroY); Serial.print("\t");
  Serial.print(gyroZ); Serial.print("\t");
  Serial.print("\t");
#endif
  Serial.print("gyro-accel roll Angle");Serial.print(kalAngleX); Serial.print("\t");
  Serial.print("\t");
 Serial.print("gyro-accel pitch Angle"); Serial.print(kalAngleY); Serial.print("\t");
#if 0 // Set to 1 to print the temperature
  Serial.print("\t");
  double temperature = (double)tempRaw / 340.0 + 36.53;
  Serial.print(temperature); Serial.print("\t");
#endif
  Serial.print("\r\n");
  delay(2);
  
  //********************** compass ***************************\\
  
   MagnetometerRaw raw = compass.ReadRawAxis();
 MagnetometerScaled scaled = compass.ReadScaledAxis();
 int MilliGauss_OnThe_XAxis = scaled.XAxis;// (or YAxis, or ZAxis)
 float heading = atan2(scaled.YAxis, scaled.XAxis);
 float declinationAngle = 0.0215;
 heading += declinationAngle;
 if(heading < 0)
 heading += 2*PI;
 if(heading > 2*PI)
 heading -= 2*PI;
 float headingDegrees = heading * 180/M_PI; 
 Output(headingDegrees);
}
void Output(float headingDegrees)
{  
  Serial.print("Compass Heading:\t");
  Serial.print(headingDegrees);
  Serial.println(" Degrees   \t"); }    

//************* bmp085C *************\\ 

void bmp085Calibration()
{
  ac1 = bmp085ReadInt(0xAA);
  ac2 = bmp085ReadInt(0xAC);
  ac3 = bmp085ReadInt(0xAE);
  ac4 = bmp085ReadInt(0xB0);
  ac5 = bmp085ReadInt(0xB2);
  ac6 = bmp085ReadInt(0xB4);
  b1 = bmp085ReadInt(0xB6);
  b2 = bmp085ReadInt(0xB8);
  mb = bmp085ReadInt(0xBA);
  mc = bmp085ReadInt(0xBC);
  md = bmp085ReadInt(0xBE);
}
float bmp085GetTemperature(unsigned int ut){
  long x1, x2;
  x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;
  x2 = ((long)mc << 11)/(x1 + md);
  b5 = x1 + x2;
  float temp = ((b5 + 8)>>4);
  temp = temp /10;
  return temp;
}
long bmp085GetPressure(unsigned long up){
  long x1, x2, x3, b3, b6, p;
  unsigned long b4, b7;
  b6 = b5 - 4000;
  x1 = (b2 * (b6 * b6)>>12)>>11;
  x2 = (ac2 * b6)>>11;
  x3 = x1 + x2;
  b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;
  x1 = (ac3 * b6)>>13;
  x2 = (b1 * ((b6 * b6)>>12))>>16;
  x3 = ((x1 + x2) + 2)>>2;
  b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;
  b7 = ((unsigned long)(up - b3) * (50000>>OSS));
  if (b7 < 0x80000000)
    p = (b7<<1)/b4;
  else
    p = (b7/b4)<<1;
  x1 = (p>>8) * (p>>8);
  x1 = (x1 * 3038)>>16;
  x2 = (-7357 * p)>>16;
  p += (x1 + x2 + 3791)>>4;
  long temp = p;
  return temp;
}
char bmp085Read(unsigned char address)
{
  unsigned char data;
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_ADDRESS, 1);
  while(!Wire.available())
  return Wire.read();
}
int bmp085ReadInt(unsigned char address)
{
  unsigned char msb, lsb;
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_ADDRESS, 2);
  while(Wire.available()<2)
  msb = Wire.read();
  lsb = Wire.read();
  return (int) msb<<8 | lsb;
}
unsigned int bmp085ReadUT(){
  unsigned int ut;
 Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x2E);
  Wire.endTransmission();
 delay(5);
  ut = bmp085ReadInt(0xF6);
  return ut;
}
unsigned long bmp085ReadUP(){
  unsigned char msb, lsb, xlsb;
  unsigned long up = 0;
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x34 + (OSS<<6));
  Wire.endTransmission();
  delay(2 + (3<<OSS));
  msb = bmp085Read(0xF6);
  lsb = bmp085Read(0xF7);
  xlsb = bmp085Read(0xF8);
  up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-OSS);
  return up;
}
void writeRegister(int deviceAddress, byte address, byte val) {
  Wire.beginTransmission(deviceAddress); // start transmission to device 
  Wire.write(address);       
  Wire.write(val);         
  Wire.endTransmission();     
}
int readRegister(int deviceAddress, byte address){
  int v;
  Wire.beginTransmission(deviceAddress);
  Wire.write(address); 
  Wire.endTransmission();
  Wire.requestFrom(deviceAddress, 1); 
  while(!Wire.available()) {}
  v = Wire.read();  return v;
}
float calcAltitude(float pressure){
  float A = pressure/101325;
  float B = 1/5.25588;
  float C = pow(A,B);
  C = 1 - C;
  C = C /0.0000225577;
  return C;
}
