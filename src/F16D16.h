#ifndef _F16D16_H_
#define _F16D16_H_


#include <HardwareSerial.h>
#include "MeasurementPacket.h"
#include "ImuMeasurementPacket.h"



class F16D16
    : public MeasurementPacket
{
    ////////////////////////////////////////////////////////////////
    // PUBLIC CONSTRUCTORS
    ////////////////////////////////////////////////////////////////
    public:
        F16D16( int8_t sensorID );
    
    
    ////////////////////////////////////////////////////////////////
    // PUBLIC METHODS
    ////////////////////////////////////////////////////////////////
    public:
        double xFlow();
        double yFlow();
        double distance();
        void printMeasurement();
        void setFlowMeasurement( int16_t fx , int16_t fy );
        void setDistanceMeasurement( int16_t distance );

};


#endif  // #ifndef _F16D16_H_
