#ifndef _MEASUREMENT_PACKET_H_
#define _MEASUREMENT_PACKET_H_


#define PACKET_MAX_SIZE 50


#include <stdint.h>
#include <HardwareSerial.h>
#include "StaticArray.h"




class MeasurementPacket
{
    ////////////////////////////////////////////////////////////////
    // PUBLIC CONSTANT PARAMETERS
    ////////////////////////////////////////////////////////////////
    public:
        static const int16_t G16A16TGA16M16TM16_ID = 0;
        static const int16_t G16A16T16_ID = 1;
        static const int16_t G16A16M16T16_ID = 2;
        static const int16_t M16T16_ID = 3;
        static const int16_t P32Tf_ID = 4;
    

    ////////////////////////////////////////////////////////////////
    // PRIVATE CONSTANT PARAMETERS
    ////////////////////////////////////////////////////////////////
    private:
        static const int NUMBER_OF_BYTES_FOR_SENSOR_TYPE = 2;
        static const int NUMBER_OF_BYTES_FOR_SENSOR_ID = 1;
        static const int NUMBER_OF_BYTES_FOR_COUNT = 2;
        


    ////////////////////////////////////////////////////////////////
    // PROTECTED VARIABLES
    ////////////////////////////////////////////////////////////////
    protected:
        StaticArray<int8_t,PACKET_MAX_SIZE> b;  // buffer of bytes
    
    
    
    ////////////////////////////////////////////////////////////////
    // PUBLIC ABSTRACT METHODS
    ////////////////////////////////////////////////////////////////
    public:
        virtual void printMeasurement() = 0;
    
    
    
    ////////////////////////////////////////////////////////////////
    // PUBLIC CONSTRUCTORS
    ////////////////////////////////////////////////////////////////
    public:
        MeasurementPacket( int8_t sensorId , int16_t sensorTypeId , int numberOfBytesNeeded );
    
    
    
    ////////////////////////////////////////////////////////////////
    // PUBLIC METHODS
    ////////////////////////////////////////////////////////////////
    public:
        void setBytes( const int8_t* theBytes );
        int8_t* bytes() const;
        int bytesLength() const;  // size of bytes buffer
        int16_t sensorTypeId() const;
        int8_t sensorId() const;
        void setCount( int16_t value );
        int16_t count() const;
        void print() const;
    

    
    ////////////////////////////////////////////////////////////////
    // PUBLIC STATIC METHODS
    ////////////////////////////////////////////////////////////////
    public:
        static MeasurementPacket* fromBytes( const int8_t* b );
        static int16_t sensorTypeIdDecode( const int8_t* b );
        static int8_t sensorIdDecode( const int8_t* b );
        static void int8Encode( int8_t value , int8_t* b , int index );
        static int8_t int8Decode( const int8_t* b , int index );
        static void int16Encode( int16_t value , int8_t* b , int index );
        static int16_t int16Decode( const int8_t* b , int index );
        static void int32Encode( int32_t value , int8_t* b , int index );
        static int32_t int32Decode( const int8_t* b , int index );
        static void int64Encode( int64_t value , int8_t* b , int index );
        static int64_t int64Decode( const int8_t* b , int index );
        static void floatEncode( float value , int8_t* b , int index );
        static float floatDecode( const int8_t* b , int index );
        static void doubleEncode( double value , int8_t* b , int index );
        static double doubleDecode( const int8_t* b , int index );

    
    ////////////////////////////////////////////////////////////////
    // PROTECTED METHODS
    ////////////////////////////////////////////////////////////////
    protected:
        void int8Encode( int8_t value , int index ) const;
        int8_t int8Decode( int index ) const;
        void int16Encode( int16_t value , int index ) const;
        int16_t int16Decode( int index ) const;
        void int32Encode( int32_t value , int index ) const;
        int32_t int32Decode( int index ) const;
        void int64Encode( int64_t value , int index ) const;
        int64_t int64Decode( int index ) const;
        void floatEncode( float value , int index ) const;
        float floatDecode( int index ) const;
        void doubleEncode( double value , int index ) const;
        double doubleDecode( int index ) const;
    

    ////////////////////////////////////////////////////////////////
    // PRIVATE METHODS
    ////////////////////////////////////////////////////////////////
    private:
        void setSensorTypeId( int16_t value );
        void setSensorId( int8_t value );


    ////////////////////////////////////////////////////////////////
    // PRIVATE STATIC METHODS
    ////////////////////////////////////////////////////////////////
    private:
        static int indexShifted( int index );

};


#endif  // #ifndef _MEASUREMENT_PACKET_H_
