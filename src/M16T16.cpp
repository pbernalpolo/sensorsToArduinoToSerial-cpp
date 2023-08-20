
#include "M16T16.h"


////////////////////////////////////////////////////////////////
// PUBLIC CONSTRUCTORS
////////////////////////////////////////////////////////////////

M16T16::M16T16( int8_t sensorID )
    : MeasurementPacket( sensorID , MeasurementPacket::M16T16_ID , 8 )
{
}



////////////////////////////////////////////////////////////////
// PUBLIC METHODS
////////////////////////////////////////////////////////////////

double M16T16::xMagnetometer()
{
    return this->int16Decode( 0 );
}


double M16T16::yMagnetometer()
{
    return this->int16Decode( 2 );
}


double M16T16::zMagnetometer()
{
    return this->int16Decode( 4 );
}


double M16T16::temperatureMagnetometer()
{
    return this->int16Decode( 6 );
}


void M16T16::printMeasurement()
{
    Serial.print( "mx: " );
    Serial.print( (int16_t)this->xMagnetometer() );
    Serial.print( "  my: " );
    Serial.print( (int16_t)this->yMagnetometer() );
    Serial.print( "  mz: " );
    Serial.print( (int16_t)this->zMagnetometer() );
    Serial.print( "    temp: " );
    Serial.print( (int16_t)this->temperatureMagnetometer() );
    Serial.println();
}


void M16T16::setMagnetometerMeasurement( int16_t mx , int16_t my , int16_t mz )
{
    this->int16Encode( mx , 0 );
    this->int16Encode( my , 2 );
    this->int16Encode( mz , 4 );
}


void M16T16::setTemperatureMeasurement( int16_t temp )
{
    this->int16Encode( temp , 6 );
}
