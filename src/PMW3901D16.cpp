
#include "PMW3901D16.h"


////////////////////////////////////////////////////////////////
// PUBLIC CONSTRUCTORS
////////////////////////////////////////////////////////////////

PMW3901D16::PMW3901D16( int8_t sensorID )
    : MeasurementPacket( sensorID , MeasurementPacket::PMW3901D16_ID , 12 )
{
}



////////////////////////////////////////////////////////////////
// PUBLIC METHODS
////////////////////////////////////////////////////////////////

double PMW3901D16::xFlow()
{
    return this->int16Decode( 0 );
}


double PMW3901D16::yFlow()
{
    return this->int16Decode( 2 );
}


uint8_t PMW3901D16::surfaceQuality()
{
    return this->uint8Decode( 4 );
}


uint8_t PMW3901D16::rawDataSum()
{
    return this->uint8Decode( 5 );
}


uint8_t PMW3901D16::rawDataMax()
{
    return this->uint8Decode( 6 );
}


uint8_t PMW3901D16::rawDataMin()
{
    return this->uint8Decode( 7 );
}


uint16_t PMW3901D16::shutter()
{
    return this->uint16Decode( 8 );
}


double PMW3901D16::distance()
{
    return this->int16Decode( 10 );
}


void PMW3901D16::printMeasurement()
{
    Serial.print( "fx: " );
    Serial.print( (int16_t)this->xFlow() );
    Serial.print( "  fy: " );
    Serial.print( (int16_t)this->yFlow() );
    Serial.print( "    surfaceQuality: " );
    Serial.print( this->surfaceQuality() );
    Serial.print( "    rawDataSum: " );
    Serial.print( this->rawDataSum() );
    Serial.print( "  rawDataMax: " );
    Serial.print( this->rawDataMax() );
    Serial.print( "  rawDataMin: " );
    Serial.print( this->rawDataMin() );
    Serial.print( "    shutter: " );
    Serial.print( this->shutter() );
    Serial.print( "    d: " );
    Serial.print( (int16_t)this->distance() );
    Serial.println();
}


void PMW3901D16::setFlowMeasurement( int16_t fx , int16_t fy )
{
    this->int16Encode( fx , 0 );
    this->int16Encode( fy , 2 );
}


void PMW3901D16::setSurfaceQuality( uint8_t surfaceQuality )
{
    this->uint8Encode( surfaceQuality , 4 );
}


void PMW3901D16::setRawDataSumMaxMin( uint8_t rawDataSum , uint8_t rawDataMax , uint8_t rawDataMin )
{
    this->uint8Encode( rawDataSum , 5 );
    this->uint8Encode( rawDataMax , 6 );
    this->uint8Encode( rawDataMin , 7 );
}


void PMW3901D16::setShutter( uint16_t shutter )
{
    this->uint16Encode( shutter , 8 );
}


void PMW3901D16::setDistanceMeasurement( int16_t distance )
{
    this->int16Encode( distance , 10 );
}
