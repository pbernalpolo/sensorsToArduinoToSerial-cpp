// TO CHANGE THE OPTIMIZATION LEVEL, edit:
// ~/.arduino15/packages/arduino/hardware/avr/1.8.6/platform.txt
// and substitute in compiler flags "-Os" by "-O3"


#define DEVICE_ID_MPU6050 11
#define DEVICE_ID_HMC5883L 12
#define DEVICE_ID_BMP085 13
#define LED_PIN 13
#define IMU_MEASUREMENTS_PER_MAGNETOMETER_MEASUREMENT 20
#define IMU_MEASUREMENTS_PER_PRESSURE_MEASUREMENTS 40

// Comment the following line to enable PRINT_MEASUREMENTS_MODE
#define MEASUREMENT_PACKET_MODE


// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"
#include "HMC5883L.h"
#include "BMP085.h"

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

#include <avr/wdt.h>  // Arduino watchdog to reset if it gets stuck

#include "G16A16T16.h"
#include "M16T16.h"
#include "P32Tf.h"
#include "MessageSerializerDeserializer.h"


// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high

// class default I2C address is 0x1E
// specific I2C addresses may be passed as a parameter here
// this device only supports one I2C address (0x1E)
HMC5883L mag;

// class default I2C address is 0x77
// specific I2C addresses may be passed as a parameter here
// (though the BMP085 supports only one address)
BMP085 barometer;

G16A16T16 mpu6050mp = G16A16T16( DEVICE_ID_MPU6050 );
M16T16 hmc5883lmp = M16T16( DEVICE_ID_HMC5883L );
P32Tf bmp085mp = P32Tf( DEVICE_ID_BMP085 );
MessageSerializerDeserializer messageSerializer = MessageSerializerDeserializer();

int32_t temperatureAveraged;
int16_t counterGA;
int16_t counterM;
int16_t counterP;
int16_t counterNextM;
int16_t counterNextP;


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
  mag.initialize();
  barometer.initialize();
  
  // configure devices
  accelgyro.setRate( 8 );  // while ODR_w=8kHz, ODR_a=1kHz => we divide by 8 to obtain the same sample rates and the same accel measurement is not used twice // no divider -> max sample rate
  accelgyro.setDLPFMode( MPU6050_DLPF_BW_256 );  // no low-pass filter -> max sample rate
  accelgyro.setDHPFMode( MPU6050_DHPF_RESET );  // no high-pass filter
  accelgyro.setFullScaleAccelRange( MPU6050_ACCEL_FS_16 );  // 16g
  accelgyro.setFullScaleGyroRange( MPU6050_GYRO_FS_2000 );  // 2000deg/s

  mag.setMode( HMC5883L_MODE_CONTINUOUS );
  mag.setDataRate( HMC5883L_RATE_75 );
  mag.setSampleAveraging( HMC5883L_AVERAGING_1 );

  // verify connection
  //Serial.println("Testing device connections...");
  //Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed" );
  //Serial.println(mag.testConnection() ? "HMC5883L connection successful" : "HMC5883L connection failed" );
  //Serial.println(barometer.testConnection() ? "BMP085 connection successful" : "BMP085 connection failed" );

  // initialize averaged temperatures
  temperatureAveraged = 0;
  
  // initialize counters
  counterGA = 0;
  counterM = 0;
  counterP = 0;
  counterNextM = 0;
  counterNextP = 0;

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
  counterGA++;
  mpu6050mp.setCount( counterGA );
  mpu6050mp.setGyroscopeMeasurement( gx , gy , gz );
  mpu6050mp.setAccelerometerMeasurement( ax , ay , az );
  mpu6050mp.setTemperatureMeasurement( temperature );
  
  // measure with the magnetometer only sometimes
  temperatureAveraged += temperature;
  if( counterNextM >= IMU_MEASUREMENTS_PER_MAGNETOMETER_MEASUREMENT ) {
    int16_t mx, my, mz;
    mag.getHeading(&mx, &my, &mz);
    counterM++;
    hmc5883lmp.setCount( counterM );
    hmc5883lmp.setMagnetometerMeasurement( mx , my , mz );
    hmc5883lmp.setTemperatureMeasurement( (int16_t)(temperatureAveraged/IMU_MEASUREMENTS_PER_MAGNETOMETER_MEASUREMENT) );
    temperatureAveraged = 0;
    counterNextM = 0;
  }
  counterNextM++;

  // measure with the barometer only sometimes
  if( counterNextP >= IMU_MEASUREMENTS_PER_PRESSURE_MEASUREMENTS ) {
    barometer.setControl( BMP085_MODE_TEMPERATURE );
    float temp = barometer.getTemperatureC();
    barometer.setControl( BMP085_MODE_PRESSURE_1 );
    int32_t p = barometer.getPressure();
    counterM++;
    bmp085mp.setCount( counterM );
    bmp085mp.setPressureMeasurement( p );
    bmp085mp.setTemperatureMeasurement( temp );
    counterNextP = 0;
  }
  counterNextP++;

  #if defined MEASUREMENT_PACKET_MODE
    int8_t* theBytes = messageSerializer.prepareBytesToWrite( mpu6050mp.bytes() , mpu6050mp.bytesLength() );
    Serial.write( (byte*)theBytes , messageSerializer.preparedBytesToWriteLength() );
    if( counterNextM == 1 ) {
      theBytes = messageSerializer.prepareBytesToWrite( hmc5883lmp.bytes() , hmc5883lmp.bytesLength() );
      Serial.write( (byte*)theBytes , messageSerializer.preparedBytesToWriteLength() );
    }
    if( counterNextP == 1 ) {
      theBytes = messageSerializer.prepareBytesToWrite( bmp085mp.bytes() , bmp085mp.bytesLength() );
      Serial.write( (byte*)theBytes , messageSerializer.preparedBytesToWriteLength() );
    }
  #else
    mpu6050mp.print();
    hmc5883lmp.print();
    bmp085mp.print();
  #endif
  
}
