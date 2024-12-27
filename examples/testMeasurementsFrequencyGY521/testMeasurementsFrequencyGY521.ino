// TO CHANGE THE OPTIMIZATION LEVEL, edit:
// ~/.arduino15/packages/arduino/hardware/avr/1.8.6/platform.txt
// and substitute in compiler flags "-Os" by "-O3"

/*
getMotion6:
  0.001796 [s]
  556.859802 [Hz]
getAcceleration:
  0.000987 [s]
  1013.495666 [Hz]
getRotation:
  0.000988 [s]
  1012.125244 [Hz]
getTemperature:
  0.000583 [s]
  1714.230834 [Hz]
*/


#define LED_PIN 13


// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif


// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high


void setup() {
  // configure Arduino LED pin for output, and activate it during setup
  pinMode( LED_PIN , OUTPUT );
  digitalWrite( LED_PIN , true );

  // initialize serial communication
  //Serial.begin(9600);
  //Serial.begin(14400);
  //Serial.begin(19200);
  //Serial.begin(28800);
  //Serial.begin(38400);
  //Serial.begin(57600);
  Serial.begin(115200);
  //Serial.begin(230400);
  //Serial.begin(250000);
  //Serial.begin(500000);
  //Serial.begin(1000000);
  //Serial.begin(2000000);
  while( !Serial );
  
  // join I2C bus (I2Cdev library doesn't do this automatically)
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
  #endif
  
  // initialize device
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();
  
  // configure device
  accelgyro.setRate( 8 );  // while ODR_w=8kHz, ODR_a=1kHz => we divide by 8 to obtain the same sample rates and the same accel measurement is not used twice // no divider -> max sample rate
  accelgyro.setDLPFMode( MPU6050_DLPF_BW_256 );  // no low-pass filter -> max sample rate
  accelgyro.setDHPFMode( MPU6050_DHPF_RESET );  // no high-pass filter
  accelgyro.setFullScaleAccelRange( MPU6050_ACCEL_FS_16 );  // 16g
  accelgyro.setFullScaleGyroRange( MPU6050_GYRO_FS_2000 );  // 2000deg/s

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  // we deactivate the led before entering the loop
  digitalWrite( LED_PIN , false );
}


void loop() {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  
  int N = 1000;
  float dt0 = 0.0;
  float dt1 = 0.0;
  float dt2 = 0.0;
  float dt3 = 0.0;
  for(int i=0; i<N; i++){
    unsigned long t0 = micros();
    accelgyro.getMotion6( &ax , &ay , &az , &gx , &gy , &gz );
    unsigned long t1 = micros();
    accelgyro.getAcceleration( &ax , &ay , &az );
    unsigned long t2 = micros();
    accelgyro.getRotation( &gx , &gy , &gz );
    unsigned long t3 = micros();
    int16_t T = accelgyro.getTemperature();
    unsigned long t4 = micros();
    dt0 += t1-t0;
    dt1 += t2-t1;
    dt2 += t3-t2;
    dt3 += t4-t3;
  }
  printPeriodAndFrequency( "getMotion6:" , (dt0/N)/1.0e6 , 1.0e6/(dt0/N) );
  printPeriodAndFrequency( "getAcceleration:" , (dt1/N)/1.0e6 , 1.0e6/(dt1/N) );
  printPeriodAndFrequency( "getRotation:" , (dt2/N)/1.0e6 , 1.0e6/(dt2/N) );
  printPeriodAndFrequency( "getTemperature:" , (dt3/N)/1.0e6 , 1.0e6/(dt3/N) );
  Serial.println();
}


void printPeriodAndFrequency( char* name , double period , double frequency )
{
  Serial.println( name );
  Serial.print( "  " );
  Serial.print( period , 6 );
  Serial.print( " [s]\n  " );
  Serial.print( frequency , 6 );
  Serial.println( " [Hz]" );
}
