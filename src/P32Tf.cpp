
#include "P32Tf.h"


////////////////////////////////////////////////////////////////
// PUBLIC CONSTRUCTORS
////////////////////////////////////////////////////////////////

P32Tf::P32Tf( int8_t sensorID )
    : MeasurementPacket( sensorID , MeasurementPacket::P32Tf_ID , 8 )
{
}



////////////////////////////////////////////////////////////////
// PUBLIC METHODS
////////////////////////////////////////////////////////////////

double P32Tf::pressureBarometer()
{
    return (double)this->int32Decode( 0 );
}


int32_t P32Tf::test()
{
    return this->int32Decode( 0 );
}


double P32Tf::temperatureBarometer()
{
    return this->floatDecode( 4 );
}


void P32Tf::printMeasurement()
{
    Serial.print( "pressure: " );
    Serial.print( this->pressureBarometer() );
    Serial.print( "    temp: " );
    Serial.print( this->temperatureBarometer() , 3 );
    Serial.println();
}


void P32Tf::setPressureMeasurement( int32_t pressure )
{
    this->int32Encode( pressure , 0 );
}


void P32Tf::setTemperatureMeasurement( float temp )
{
    this->floatEncode( temp , 4 );
}
