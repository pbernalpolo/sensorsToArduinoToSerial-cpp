
#include "MeasurementPacket.h"
#include "G16A16TGA16M16TM16.h"
#include "G16A16T16.h"
#include "G16A16M16T16.h"
#include "M16T16.h"
#include "P32Tf.h"



////////////////////////////////////////////////////////////////
// PUBLIC CONSTRUCTORS
////////////////////////////////////////////////////////////////

MeasurementPacket::MeasurementPacket( int8_t sensorId , int16_t sensorTypeId , int numberOfBytesNeeded )
{
    int nBytes = MeasurementPacket::NUMBER_OF_BYTES_FOR_SENSOR_TYPE +
                 MeasurementPacket::NUMBER_OF_BYTES_FOR_SENSOR_ID +
                 MeasurementPacket::NUMBER_OF_BYTES_FOR_COUNT +
                 numberOfBytesNeeded;
    this->b = StaticArray<int8_t,PACKET_MAX_SIZE>::ofSize( nBytes );
    this->setSensorTypeId( sensorTypeId );
    this->setSensorId( sensorId );
}
    
    
    
////////////////////////////////////////////////////////////////
// PUBLIC METHODS
////////////////////////////////////////////////////////////////
    
void MeasurementPacket::setBytes( const int8_t* theBytes )
{
    //this->b = theBytes;
    for( int i=0; i<this->b.size(); i++ ) {
        this->b.set( i , theBytes[i] );
    }
}


int8_t* MeasurementPacket::bytes() const
{
    return this->b.data();
}


int MeasurementPacket::bytesLength() const
{
    return this->b.size();
}


int16_t MeasurementPacket::sensorTypeId() const
{
    return MeasurementPacket::sensorTypeIdDecode( this->b.data() );
}


int8_t MeasurementPacket::sensorId() const
{
    return MeasurementPacket::sensorIdDecode( this->b.data() );
}


void MeasurementPacket::setCount( int16_t value )
{
    MeasurementPacket::int16Encode( value , this->b.data() , MeasurementPacket::NUMBER_OF_BYTES_FOR_SENSOR_TYPE +
                                                             MeasurementPacket::NUMBER_OF_BYTES_FOR_SENSOR_ID );
}


int16_t MeasurementPacket::count() const
{
    return MeasurementPacket::int16Decode( this->b.data() , MeasurementPacket::NUMBER_OF_BYTES_FOR_SENSOR_TYPE +
                                                            MeasurementPacket::NUMBER_OF_BYTES_FOR_SENSOR_ID );
}


void MeasurementPacket::print() const
{
    Serial.print( "TID: " );
    Serial.print( this->sensorTypeId() );
    Serial.print( "      SID: " );
    Serial.print( this->sensorId() );
    Serial.print( "      count: " );
    Serial.print( this->count() );
    Serial.print( "      " );
    this->printMeasurement();
}



////////////////////////////////////////////////////////////////
// PUBLIC STATIC METHODS
////////////////////////////////////////////////////////////////

MeasurementPacket* MeasurementPacket::fromBytes( const int8_t* b )
{
    if( b == NULL ) return NULL;
    int16_t sensorTypeId = MeasurementPacket::sensorTypeIdDecode( b );
    int8_t sensorId = MeasurementPacket::sensorIdDecode( b );
    MeasurementPacket* mp;
    switch( sensorTypeId ) {
        case MeasurementPacket::G16A16TGA16M16TM16_ID:
            mp = new G16A16TGA16M16TM16( sensorId );
            break;
        case MeasurementPacket::G16A16T16_ID:
            mp = new G16A16T16( sensorId );
            break;
        case MeasurementPacket::G16A16M16T16_ID:
            mp = new G16A16M16T16( sensorId );
            break;
        case MeasurementPacket::M16T16_ID:
            mp = new M16T16( sensorId );
            break;
        case MeasurementPacket::P32Tf_ID:
            mp = new P32Tf( sensorId );
            break;
        default:
            mp = NULL;
    }
    if( mp != NULL ) {
        mp->setBytes( b );
    }
    return mp;
}


int16_t MeasurementPacket::sensorTypeIdDecode( const int8_t* b )
{
    return MeasurementPacket::int16Decode( b , 0 );
}


int8_t MeasurementPacket::sensorIdDecode( const int8_t* b )
{
    return MeasurementPacket::int8Decode( b , MeasurementPacket::NUMBER_OF_BYTES_FOR_SENSOR_TYPE );
}


void MeasurementPacket::int8Encode( int8_t value , int8_t* b , int index )
{
    b[index] = value;
}


int8_t MeasurementPacket::int8Decode( const int8_t* b , int index )
{
    return b[index];
}


void MeasurementPacket::uint8Encode( uint8_t value , int8_t* b , int index )
{
    b[index] = value;
}


uint8_t MeasurementPacket::uint8Decode( const int8_t* b , int index )
{
    return b[index];
}


void MeasurementPacket::int16Encode( int16_t value , int8_t* b , int index )
{
    // we create a int8_t pointer pointing to the address of the int16
    int8_t* packets = (int8_t*) &value;
    b[index] = packets[0];
    b[index+1] = packets[1];
}


int16_t MeasurementPacket::int16Decode( const int8_t* b , int index )
{
    // we create a int16 pointer pointing to the first byte
    int16_t* valuePointer = (int16_t*) &b[index];
    // we return the value of the int16 pointed by the pointer
    return *valuePointer;
}


void MeasurementPacket::uint16Encode( uint16_t value , int8_t* b , int index )
{
    // we create a int8_t pointer pointing to the address of the int16
    int8_t* packets = (int8_t*) &value;
    b[index] = packets[0];
    b[index+1] = packets[1];
}


uint16_t MeasurementPacket::uint16Decode( const int8_t* b , int index )
{
    // we create a uint16 pointer pointing to the first byte
    uint16_t* valuePointer = (uint16_t*) &b[index];
    // we return the value of the uint16 pointed by the pointer
    return *valuePointer;
}


void MeasurementPacket::int32Encode( int32_t value , int8_t* b , int index )
{
    // we create a int8_t pointer pointing to the address of the int32
    int8_t* packets = (int8_t*) &value;
    b[index] = packets[0];
    b[index+1] = packets[1];
    b[index+2] = packets[2];
    b[index+3] = packets[3];
}


int32_t MeasurementPacket::int32Decode( const int8_t* b , int index )
{
    // we create a int32 pointer pointing to the first byte
    int32_t* valuePointer = (int32_t*) &b[index];
    // we return the value of the int32 pointed by the pointer
    return *valuePointer;
}


void MeasurementPacket::uint32Encode( uint32_t value , int8_t* b , int index )
{
    // we create a int8_t pointer pointing to the address of the int32
    int8_t* packets = (int8_t*) &value;
    b[index] = packets[0];
    b[index+1] = packets[1];
    b[index+2] = packets[2];
    b[index+3] = packets[3];
}


uint32_t MeasurementPacket::uint32Decode( const int8_t* b , int index )
{
    // we create a uint32 pointer pointing to the first byte
    uint32_t* valuePointer = (uint32_t*) &b[index];
    // we return the value of the uint32 pointed by the pointer
    return *valuePointer;
}


void MeasurementPacket::int64Encode( int64_t value , int8_t* b , int index )
{
    // we create a int8_t pointer pointing to the address of the int64
    int8_t* packets = (int8_t*) &value;
    b[index] = packets[0];
    b[index+1] = packets[1];
    b[index+2] = packets[2];
    b[index+3] = packets[3];
    b[index+4] = packets[4];
    b[index+5] = packets[5];
    b[index+6] = packets[6];
    b[index+7] = packets[7];
}


int64_t MeasurementPacket::int64Decode( const int8_t* b , int index )
{
    // we create a int64 pointer pointing to the first byte
    int64_t* valuePointer = (int64_t*) &b[index];
    // we return the value of the int64 pointed by the pointer
    return *valuePointer;
}


void MeasurementPacket::floatEncode( float value , int8_t* b , int index )
{
    // we create a int8_t pointer pointing to the address of the float
    int8_t* packets = (int8_t*) &value;
    b[index] = packets[0];
    b[index+1] = packets[1];
    b[index+2] = packets[2];
    b[index+3] = packets[3];
}


float MeasurementPacket::floatDecode( const int8_t* b , int index )
{
    // we create a float pointer pointing to the first byte
    float* valuePointer = (float*) &b[index];
    // we return the value of the float pointed by the pointer
    return *valuePointer;
}


void doubleEncode( double value , int8_t* b , int index )
{
    // we create a int8_t pointer pointing to the address of the double
    int8_t* packets = (int8_t*) &value;
    b[index] = packets[0];
    b[index+1] = packets[1];
    b[index+2] = packets[2];
    b[index+3] = packets[3];
    b[index+4] = packets[4];
    b[index+5] = packets[5];
    b[index+6] = packets[6];
    b[index+7] = packets[7];
}


double doubleDecode( const int8_t* b , const int index )
{
    // we create a double pointer pointing to the first byte
    double* valuePointer = (double*) &b[index];
    // we return the value of the double pointed by the pointer
    return *valuePointer;
}



////////////////////////////////////////////////////////////////
// PROTECTED METHODS
////////////////////////////////////////////////////////////////

void MeasurementPacket::int8Encode( int8_t value , int index ) const
{
    MeasurementPacket::int8Encode( value , this->b.data() , MeasurementPacket::indexShifted( index ) );
}


int8_t MeasurementPacket::int8Decode( int index ) const
{
    return MeasurementPacket::int8Decode( this->b.data() , MeasurementPacket::indexShifted( index ) );
}


void MeasurementPacket::uint8Encode( uint8_t value , int index ) const
{
    MeasurementPacket::uint8Encode( value , this->b.data() , MeasurementPacket::indexShifted( index ) );
}


uint8_t MeasurementPacket::uint8Decode( int index ) const
{
    return MeasurementPacket::uint8Decode( this->b.data() , MeasurementPacket::indexShifted( index ) );
}


void MeasurementPacket::int16Encode( int16_t value , int index ) const
{
    MeasurementPacket::int16Encode( value , this->b.data() , MeasurementPacket::indexShifted( index ) );
}


int16_t MeasurementPacket::int16Decode( int index ) const
{
    return MeasurementPacket::int16Decode( this->b.data() , MeasurementPacket::indexShifted( index ) );
}


void MeasurementPacket::uint16Encode( uint16_t value , int index ) const
{
    MeasurementPacket::uint16Encode( value , this->b.data() , MeasurementPacket::indexShifted( index ) );
}


uint16_t MeasurementPacket::uint16Decode( int index ) const
{
    return MeasurementPacket::uint16Decode( this->b.data() , MeasurementPacket::indexShifted( index ) );
}


void MeasurementPacket::int32Encode( int32_t value , int index ) const
{
    MeasurementPacket::int32Encode( value , this->b.data() , MeasurementPacket::indexShifted( index ) );
}


int32_t MeasurementPacket::int32Decode( int index ) const
{
    return MeasurementPacket::int32Decode( this->b.data() , MeasurementPacket::indexShifted( index ) );
}


void MeasurementPacket::uint32Encode( uint32_t value , int index ) const
{
    MeasurementPacket::uint32Encode( value , this->b.data() , MeasurementPacket::indexShifted( index ) );
}


uint32_t MeasurementPacket::uint32Decode( int index ) const
{
    return MeasurementPacket::uint32Decode( this->b.data() , MeasurementPacket::indexShifted( index ) );
}


void MeasurementPacket::int64Encode( int64_t value , int index ) const
{
    MeasurementPacket::int64Encode( value , this->b.data() , MeasurementPacket::indexShifted( index ) );
}


int64_t MeasurementPacket::int64Decode( int index ) const
{
    return MeasurementPacket::int64Decode( this->b.data() , MeasurementPacket::indexShifted( index ) );
}


void MeasurementPacket::floatEncode( float value , int index ) const
{
    MeasurementPacket::floatEncode( value , this->b.data() , MeasurementPacket::indexShifted( index ) );
}


float MeasurementPacket::floatDecode( int index ) const
{
    return MeasurementPacket::floatDecode( this->b.data() , MeasurementPacket::indexShifted( index ) );
}


void MeasurementPacket::doubleEncode( double value , int index ) const
{
    MeasurementPacket::doubleEncode( value , this->b.data() , MeasurementPacket::indexShifted( index ) );
}


double MeasurementPacket::doubleDecode( int index ) const
{
    return MeasurementPacket::doubleDecode( this->b.data() , MeasurementPacket::indexShifted( index ) );
}



////////////////////////////////////////////////////////////////
// PRIVATE METHODS
////////////////////////////////////////////////////////////////

void MeasurementPacket::setSensorTypeId( int16_t value )
{
    MeasurementPacket::int16Encode( value , this->b.data() , 0 );
}


void MeasurementPacket::setSensorId( int8_t value )
{
    MeasurementPacket::int8Encode( value , this->b.data() , MeasurementPacket::NUMBER_OF_BYTES_FOR_SENSOR_TYPE );
}



////////////////////////////////////////////////////////////////
// PRIVATE STATIC METHODS
////////////////////////////////////////////////////////////////

int MeasurementPacket::indexShifted( int index )
{
    return ( MeasurementPacket::NUMBER_OF_BYTES_FOR_SENSOR_TYPE +
             MeasurementPacket::NUMBER_OF_BYTES_FOR_SENSOR_ID +
             MeasurementPacket::NUMBER_OF_BYTES_FOR_COUNT +
             index );
}
