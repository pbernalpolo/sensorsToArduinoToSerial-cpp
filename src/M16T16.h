#ifndef _M16T16_H_
#define _M16T16_H_


#include <HardwareSerial.h>
#include "MeasurementPacket.h"
#include "MagnetometerMeasurementPacket.h"



class M16T16
    : public MeasurementPacket,
    public MagnetometerMeasurementPacket
{
    ////////////////////////////////////////////////////////////////
    // PUBLIC CONSTRUCTORS
    ////////////////////////////////////////////////////////////////
    public:
        M16T16( int8_t sensorID );
    
    
    ////////////////////////////////////////////////////////////////
    // PUBLIC METHODS
    ////////////////////////////////////////////////////////////////
    public:
        double xMagnetometer();
        double yMagnetometer();
        double zMagnetometer();
        double temperatureMagnetometer();
        void printMeasurement();
        void setMagnetometerMeasurement( int16_t mx , int16_t my , int16_t mz );
        void setTemperatureMeasurement( int16_t temp );

};


#endif  // #ifndef _M16T16_H_
