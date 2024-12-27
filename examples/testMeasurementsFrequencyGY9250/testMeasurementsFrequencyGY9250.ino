
// TO CHANGE THE OPTIMIZATION LEVEL, edit:
// ~/.arduino15/packages/arduino/hardware/avr/1.8.6/platform.txt
// and substitute in compiler flags "-Os" by "-O3"

/*
updateGyro:
  0.000927 [s]
  1079.079223 [Hz]
updateAccel:
  0.000925 [s]
  1080.842773 [Hz]
updateTemperature:
  0.000607 [s]
  1647.630981 [Hz]
updateCompass:
  0.001119 [s]
  893.594299 [Hz]
*/


#define LED_PIN 13


#include <SparkFunMPU9250-DMP.h>

#include <avr/wdt.h>  // Arduino watchdog to reset if it gets stuck

#include "G16A16T16.h"
#include "M16T16.h"
#include "MessageSerializerDeserializer.h"


MPU9250_DMP imu;


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

  // Call imu.begin() to verify communication with and
  // initialize the MPU-9250 to it's default values.
  // Most functions return an error code - INV_SUCCESS (0)
  // indicates the IMU was present and successfully set up
  if( imu.begin() != INV_SUCCESS ) {
    while (1) {
      //Serial.println("Unable to communicate with MPU-9250");
      //Serial.println("Check connections, and try again.");
      //Serial.println();
      // with above statements the memory limit is exceeded
      Serial.println( "MPU9250 not found." );
      delay(3000);
    }
  }

  // Use setSensors to turn on or off MPU-9250 sensors.
  // Any of the following defines can be combined:
  // INV_XYZ_GYRO, INV_XYZ_ACCEL, INV_XYZ_COMPASS,
  // INV_X_GYRO, INV_Y_GYRO, or INV_Z_GYRO
  // Enable all sensors:
  imu.setSensors( INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS );

  // Use setGyroFSR() and setAccelFSR() to configure the
  // gyroscope and accelerometer full scale ranges.
  // Gyro options are +/- 250, 500, 1000, or 2000 dps
  imu.setGyroFSR( 2000 ); // Set gyro to 2000 dps
  // Accel options are +/- 2, 4, 8, or 16 g
  imu.setAccelFSR( 16 ); // Set accel to +/-2g
  // Note: the MPU-9250's magnetometer FSR is set at 
  // +/- 4912 uT (micro-tesla's)

  // setLPF() can be used to set the digital low-pass filter
  // of the accelerometer and gyroscope.
  // Can be any of the following: 188, 98, 42, 20, 10, 5
  // (values are in Hz).
  imu.setLPF(5); // Set LPF corner frequency to 5Hz

  // The sample rate of the accel/gyro can be set using
  // setSampleRate. Acceptable values range from 4Hz to 1kHz
  imu.setSampleRate( 1000 );

  // Likewise, the compass (magnetometer) sample rate can be
  // set using the setCompassSampleRate() function.
  // This value can range between: 1-100Hz
  imu.setCompassSampleRate( 100 ); // Set mag rate to 10Hz

  // we deactivate the led before entering the loop
  digitalWrite( LED_PIN , false );
}


void loop() {
  int N = 1000;
  float dt0 = 0.0;
  float dt1 = 0.0;
  float dt2 = 0.0;
  float dt3 = 0.0;
  for(int i=0; i<N; i++){
    wdt_reset();
    unsigned long t0 = micros();
    imu.updateGyro();
    unsigned long t1 = micros();
    imu.updateAccel();
    unsigned long t2 = micros();
    imu.updateTemperature();
    unsigned long t3 = micros();
    imu.updateCompass();
    unsigned long t4 = micros();
    dt0 += t1-t0;
    dt1 += t2-t1;
    dt2 += t3-t2;
    dt3 += t4-t3;
  }
  printPeriodAndFrequency( "updateGyro:" , (dt0/N)/1.0e6 , 1.0e6/(dt0/N) );
  printPeriodAndFrequency( "updateAccel:" , (dt1/N)/1.0e6 , 1.0e6/(dt1/N) );
  printPeriodAndFrequency( "updateTemperature:" , (dt2/N)/1.0e6 , 1.0e6/(dt2/N) );
  printPeriodAndFrequency( "updateCompass:" , (dt3/N)/1.0e6 , 1.0e6/(dt3/N) );
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

