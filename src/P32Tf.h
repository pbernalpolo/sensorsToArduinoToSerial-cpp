#ifndef _P32TF_H_
#define _P32TF_H_


#include <HardwareSerial.h>
#include "MeasurementPacket.h"
#include "BarometerMeasurementPacket.h"



class P32Tf
    : public MeasurementPacket,
    public BarometerMeasurementPacket
{
    ////////////////////////////////////////////////////////////////
    // PUBLIC CONSTRUCTORS
    ////////////////////////////////////////////////////////////////
    public:
        P32Tf( int8_t sensorID );
    
    
    ////////////////////////////////////////////////////////////////
    // PUBLIC METHODS
    ////////////////////////////////////////////////////////////////
    public:
        int32_t test();
        double pressureBarometer();
        double temperatureBarometer();
        void printMeasurement();
        void setPressureMeasurement( int32_t pressure );
        void setTemperatureMeasurement( float temp );

};


#endif  // #ifndef _P32TF_H_
