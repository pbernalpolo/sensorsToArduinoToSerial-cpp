#include <Wire.h>
#include <SPI.h>
#include <avr/wdt.h>  // Arduino watchdog to reset if it gets stuck
#include "Bitcraze_PMW3901.h"
#include "Adafruit_VL53L1X.h"
#include "F16D16.h"
#include "MessageSerializerDeserializer.h"



// ================= CONFIG ==================
const int N_PAIRS = 4;   // <--- change this number of pairs

int pair_ID[N_PAIRS] = { 81, 82, 83, 84 };

// PMW3901 chip-select pins
int pmwCSPins[N_PAIRS] = {2, 4, 6, 8};   // Pins for each PMW3901 (down, right, left, up)

// VL53L1X XSHUT pins
int vlXShutPins[N_PAIRS] = {3, 5, 7, 9}; // Pins for each VL53L1X (down, right, left, up)
// ===========================================

// Create sensor objects dynamically
Bitcraze_PMW3901* flowSensors[N_PAIRS];
Adafruit_VL53L1X* distSensors[N_PAIRS];

F16D16* flowDistanceMeasurementPacket[N_PAIRS];
MessageSerializerDeserializer messageSerializer = MessageSerializerDeserializer();
int16_t counter;

// Data
int16_t deltaX, deltaY;
int16_t distance;

void setup() {
  // first of all, we reset the watchdog, and we enable it
  wdt_reset();
  wdt_enable(WDTO_2S);

  Serial.begin(115200);
  while (!Serial) delay(10);

  Wire.begin();
  SPI.begin();

  // -------- Initialize VL53L1X --------
  // First, hold all VL53L1X in reset
  for (int i = 0; i < N_PAIRS; i++) {
    pinMode(vlXShutPins[i], OUTPUT);
    digitalWrite(vlXShutPins[i], LOW);
  }
  delay(10);

  for (int i = 0; i < N_PAIRS; i++) {
    digitalWrite(vlXShutPins[i], HIGH);
    delay(10);

    distSensors[i] = new Adafruit_VL53L1X();

    uint8_t newAddr = 0x30 + i;  // unique I2C addr
    if (!distSensors[i]->begin(newAddr, &Wire)) {
      Serial.print("Failed to init VL53L1X #");
      Serial.print(i);
      Serial.print( ". Status: " );
      Serial.println( distSensors[i]->vl_status );
      while (1) delay(10);
    }
    Serial.print( "Configured VL53L1X at 0x" );
    Serial.println( newAddr , HEX );

    if( !distSensors[i]->startRanging() ) {
      Serial.print(F("Couldn't start ranging: "));
      Serial.println(distSensors[i]->vl_status);
      while (1) delay(10);
    }
    Serial.println(F("Ranging started"));

    // Valid timing budgets: 15, 20, 33, 50, 100, 200 and 500ms!
    distSensors[i]->setTimingBudget(50);
    Serial.print(F("Timing budget (ms): "));
    Serial.println(distSensors[i]->getTimingBudget());
    // Intermeasurement period must be >/= timing budget.
    distSensors[i]->VL53L1X_SetInterMeasurementInMs( 80 );
    
    distSensors[i]->VL53L1X_SetDistanceMode( 2 );
    
    /*VL53L1X_Version_t version;
    distSensors[i]->VL53L1X_GetSWVersion( &version );
    Serial.println( version.major );
    Serial.println( version.minor );
    Serial.println( version.build );
    Serial.println( version.revision );*/

    uint16_t out;
    uint16_t out2;
    distSensors[i]->VL53L1X_GetAmbientRate( &out );
    Serial.print( "out: " );
    //Serial.print( out );
    //Serial.print( " " );
    Serial.println( out );
    //Serial.println( distSensors[i]-> );
    /*
    distSensors[i]->VL53L1X_SetDistanceThreshold(100, 300, 3, 1);
    distSensors[i]->VL53L1X_SetInterruptPolarity(0);
    */
  }

  // -------- Initialize PMW3901 --------
  for (int i = 0; i < N_PAIRS; i++) {
    flowSensors[i] = new Bitcraze_PMW3901(pmwCSPins[i]);
    if (!flowSensors[i]->begin()) {
      Serial.print("Failed to init PMW3901 #");
      Serial.println(i);
      while (1) delay( 10 );
    }
  }

  // Initialize measurement packets.
  for( int i=0; i<N_PAIRS; i++ ) {
    flowDistanceMeasurementPacket[i] = new F16D16( pair_ID[i] );

  }

  counter = 0;

  Serial.println("Setup complete!");

  wdt_reset();
  delay(900);
  wdt_reset();
  wdt_enable(WDTO_1S);
}

void loop() {
  // first, we reset the watchdog
  wdt_reset();
  
  for (int i = 0; i < N_PAIRS; i++) {
    // --- Optical flow ---
    flowSensors[i]->readMotionCount(&deltaX, &deltaY);

    // --- Distance ---
    if( distSensors[i]->dataReady() ) {
      distance = distSensors[i]->distance(); // mm
      distSensors[i]->clearInterrupt();
    } else {
      distance = -2;
    }

    // --- Print ---
    /*Serial.print("Pair ");
    Serial.print(i);
    Serial.print(" | Flow dx=");
    Serial.print(deltaX);
    Serial.print(" dy=");
    Serial.print(deltaY);
    Serial.print(" | Dist=");
    Serial.print(distance);
    Serial.println(" mm");*/
    flowDistanceMeasurementPacket[i]->setCount( counter );
    counter++;
    flowDistanceMeasurementPacket[i]->setFlowMeasurement( deltaX , deltaY );
    flowDistanceMeasurementPacket[i]->setDistanceMeasurement( distance );

    int8_t* theBytes = messageSerializer.prepareBytesToWrite( flowDistanceMeasurementPacket[i]->bytes() , flowDistanceMeasurementPacket[i]->bytesLength() );
    Serial.write( (byte*)theBytes , messageSerializer.preparedBytesToWriteLength() );
  }

  delay(100); // adjust sample speed
}
