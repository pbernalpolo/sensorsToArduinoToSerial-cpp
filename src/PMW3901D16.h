#ifndef _PMW3901D16_H_
#define _PMW3901D16_H_


#include <HardwareSerial.h>
#include "MeasurementPacket.h"
#include "ImuMeasurementPacket.h"



class PMW3901D16
    : public MeasurementPacket
{
    ////////////////////////////////////////////////////////////////
    /// PUBLIC CONSTRUCTORS
    ////////////////////////////////////////////////////////////////
    public:
        PMW3901D16( int8_t sensorID );
    
    
    ////////////////////////////////////////////////////////////////
    /// PUBLIC METHODS
    ////////////////////////////////////////////////////////////////
    public:
        double xFlow();
        double yFlow();
        uint8_t surfaceQuality();
        uint8_t rawDataSum();
        uint8_t rawDataMax();
        uint8_t rawDataMin();
        uint16_t shutter();
        double distance();
        void printMeasurement();
        void setFlowMeasurement( int16_t fx , int16_t fy );
        void setSurfaceQuality( uint8_t surfaceQuality );
        void setRawDataSumMaxMin( uint8_t rawDataSum , uint8_t rawDataMax , uint8_t rawDataMin );
        void setShutter( uint16_t shutter );
        void setDistanceMeasurement( int16_t distance );

};


#endif  // #ifndef _PMW3901D16_H_
