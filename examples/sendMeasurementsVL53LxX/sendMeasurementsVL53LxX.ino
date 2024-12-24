/*
This example shows how to set up and read multiple VL53L1X sensors connected to
the same I2C bus. Each sensor needs to have its XSHUT pin connected to a
different Arduino pin, and you should change sensorCount and the xshutPins array
below to match your setup.

For more information, see ST's application note AN4846 ("Using multiple VL53L0X
in a single design"). The principles described there apply to the VL53L1X as
well.
*/

#include <Wire.h>
#include <VL53L0X.h>
#include <VL53L1X.h>

// The number of sensors in your system.
const uint8_t sensor0Count = 2;
const uint8_t sensor1Count = 4;

// The Arduino pin connected to the XSHUT pin of each sensor.
const uint8_t xshut0Pins[sensor0Count] = { 3 , 6 };
const uint8_t xshut1Pins[sensor1Count] = { 2 , 4 , 5 , 7 };

VL53L0X sensors0[sensor0Count];
VL53L1X sensors1[sensor1Count];

void setup()
{
  while (!Serial) {}
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C

  // Disable/reset all sensors by driving their XSHUT pins low.
  for (uint8_t i = 0; i < sensor0Count; i++) {
    pinMode( xshut0Pins[i] , OUTPUT );
    digitalWrite( xshut0Pins[i] , LOW );
  }
  for (uint8_t i = 0; i < sensor1Count; i++) {
    pinMode( xshut1Pins[i] , OUTPUT );
    digitalWrite( xshut1Pins[i] , LOW );
  }

  // Enable, initialize, and start each sensor, one by one.
  uint8_t addressCount = 0x2A;
  for (uint8_t i = 0; i < sensor0Count; i++) {
    // Stop driving this sensor's XSHUT low. This should allow the carrier
    // board to pull it high. (We do NOT want to drive XSHUT high since it is
    // not level shifted.) Then wait a bit for the sensor to start up.
    pinMode(xshut0Pins[i], INPUT);
    delay(10);

    sensors0[i].setTimeout(500);
    if (!sensors0[i].init()) {
      Serial.print(F("Failed to detect and initialize sensor "));
      Serial.println(i);
      while (1);
    }

    // Each sensor must have its address changed to a unique value other than
    // the default of 0x29 (except for the last one, which could be left at
    // the default). To make it simple, we'll just count up from 0x2A.
    sensors0[i].setAddress( addressCount );
    addressCount++;

    sensors0[i].startContinuous(20);
  }
  for (uint8_t i = 0; i < sensor1Count; i++) {
    // Stop driving this sensor's XSHUT low. This should allow the carrier
    // board to pull it high. (We do NOT want to drive XSHUT high since it is
    // not level shifted.) Then wait a bit for the sensor to start up.
    pinMode(xshut1Pins[i], INPUT);
    delay(10);

    sensors1[i].setTimeout(500);
    if (!sensors1[i].init())
    {
      Serial.print(F("Failed to detect and initialize sensor "));
      Serial.println(i);
      while (1);
    }

    // Each sensor must have its address changed to a unique value other than
    // the default of 0x29 (except for the last one, which could be left at
    // the default). To make it simple, we'll just count up from 0x2A.
    sensors1[i].setAddress( addressCount );
    addressCount++;

    sensors1[i].startContinuous(20);
  }
}


void loop()
{
  for (uint8_t i = 0; i < sensor0Count; i++) {
    Serial.print(sensors0[i].readRangeContinuousMillimeters());
    if (sensors0[i].timeoutOccurred()) { Serial.print(F(" TIMEOUT")); }
    Serial.print('\t');
  }
  for (uint8_t i = 0; i < sensor1Count; i++) {
    Serial.print(sensors1[i].read());
    if (sensors1[i].timeoutOccurred()) { Serial.print(F(" TIMEOUT")); }
    Serial.print('\t');
  }
  Serial.println();
}
