// TO CHANGE THE OPTIMIZATION LEVEL, edit:
// ~/.arduino15/packages/arduino/hardware/avr/1.8.6/platform.txt
// and substitute in compiler flags "-Os" by "-O3"


#define DEVICE_ID 21
#define LED_PIN 13
// choose one mode:
#define MEASUREMENT_PACKET_MODE
//#define PRINT_MODE
//#define BENCHMARK_MODE


// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

#include <avr/wdt.h>  // Arduino watchdog to reset if it gets stuck

#include "G16A16T16.h"
#include "MessageSerializerDeserializer.h"


// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high


G16A16T16 mpu6050mp = G16A16T16( DEVICE_ID );
MessageSerializerDeserializer messageSerializer = MessageSerializerDeserializer();

int16_t counter;

// uncomment "OUTPUT_READABLE_ACCELGYRO" if you want to see a tab-separated
// list of the accel X/Y/Z and then gyro X/Y/Z values in decimal. Easy to read,
// not so easy to parse, and slow(er) over UART.
#define OUTPUT_READABLE_ACCELGYRO

// uncomment "OUTPUT_BINARY_ACCELGYRO" to send all 6 axes of data as 16-bit
// binary, one right after the other. This is very fast (as fast as possible
// without compression or data loss), and easy to parse, but impossible to read
// for a human.
//#define OUTPUT_BINARY_ACCELGYRO


void setup() {
  // first of all, we reset the watchdog, and we enable it
  wdt_reset();
  wdt_enable(WDTO_1S);
  
  // configure Arduino LED pin for output, and activate it during setup
  pinMode( LED_PIN , OUTPUT );
  digitalWrite( LED_PIN , true );

  // join I2C bus (I2Cdev library doesn't do this automatically)
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
  #endif
  
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

  // initialize counter
  counter = 0;

  // we deactivate the led before entering the loop
  digitalWrite( LED_PIN , false );
}


void loop() {
  // first, we reset the watchdog
  wdt_reset();
  
  // read raw accel/gyro measurements from device
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  int16_t temperature = accelgyro.getTemperature();

  // set information in MeasurementPacket
  mpu6050mp.setCount( counter++ );
  mpu6050mp.setGyroscopeMeasurement( gx , gy , gz );
  mpu6050mp.setAccelerometerMeasurement( ax , ay , az );
  mpu6050mp.setTemperatureMeasurement( temperature );
  
  #if defined MEASUREMENT_PACKET_MODE
    int8_t* theBytes = messageSerializer.prepareBytesToWrite( mpu6050mp.bytes() , mpu6050mp.bytesLength() );
    Serial.write( (byte*)theBytes , messageSerializer.preparedBytesToWriteLength() );
  #elif defined PRINT_MODE
    mpu6050mp.print();
  #elif defined BENCHMARK_MODE
    test_measurementFrequency();
  #else
    Serial.println( "Select a valid mode at the top of the script." );
    while( true );
  #endif
}


void test_measurementFrequency() {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  
  int N = 1000;
  float dt0 = 0.0;
  float dt1 = 0.0;
  float dt2 = 0.0;
  float dt3 = 0.0;
  for(int i=0; i<N; i++){
    wdt_reset();
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
  Serial.print( "getMotion6: " );
  Serial.print( 1.0e6/(dt0/N) , 6 );
  Serial.print( " [Hz]   getAcceleration: " );
  Serial.print( 1.0e6/(dt1/N) , 6 );
  Serial.print( " [Hz]   getRotation: " );
  Serial.print( 1.0e6/(dt2/N) , 6 );
  Serial.print( " [Hz]   getTemperature: " );
  Serial.print( 1.0e6/(dt3/N) , 6 );
  Serial.print( " [Hz]" );
  Serial.println();
}

