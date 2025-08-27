
#include "F16D16.h"


////////////////////////////////////////////////////////////////
// PUBLIC CONSTRUCTORS
////////////////////////////////////////////////////////////////

F16D16::F16D16( int8_t sensorID )
    : MeasurementPacket( sensorID , MeasurementPacket::F16D16_ID , 6 )
{
}



////////////////////////////////////////////////////////////////
// PUBLIC METHODS
////////////////////////////////////////////////////////////////

double F16D16::xFlow()
{
    return this->int16Decode( 0 );
}


double F16D16::yFlow()
{
    return this->int16Decode( 2 );
}


double F16D16::distance()
{
    return this->int16Decode( 4 );
}


void F16D16::printMeasurement()
{
    Serial.print( "fx: " );
    Serial.print( (int16_t)this->xFlow() );
    Serial.print( "  fy: " );
    Serial.print( (int16_t)this->yFlow() );
    Serial.print( "    d: " );
    Serial.print( (int16_t)this->distance() );
    Serial.println();
}


void F16D16::setFlowMeasurement( int16_t fx , int16_t fy )
{
    this->int16Encode( fx , 0 );
    this->int16Encode( fy , 2 );
}


void F16D16::setDistanceMeasurement( int16_t distance )
{
    this->int16Encode( distance , 4 );
}
