#ifndef _MESSAGE_SERIALIZER_DESERIALIZER_H_
#define _MESSAGE_SERIALIZER_DESERIALIZER_H_


#define MESSAGE_MAX_SIZE 50


#include <stdint.h>
#include <HardwareSerial.h>
#include "SimpleChecksum.h"
#include "StaticArray.h"



class MessageSerializerDeserializer
{
    ////////////////////////////////////////////////////////////////
    // PRIVATE CONSTANTS
    ////////////////////////////////////////////////////////////////
    private:
        static const int N_CHECKSUMS = 2;  // number of checksums
        static const int8_t MAGIC_BYTE = '\n';  // magic byte to indicate the start of a new package
    
    
    ////////////////////////////////////////////////////////////////
    // PRIVATE VARIABLES
    ////////////////////////////////////////////////////////////////
    private:
        StaticArray<int8_t,MESSAGE_MAX_SIZE> messageIn;  // pointer to array of bytes that will contain the message being received
        StaticArray<int8_t,MESSAGE_MAX_SIZE> messageOut;  // pointer to array of bytes to send
        // checksum computed with the incoming message
        SimpleChecksum checksumComputed;
        // checksum that arrives with the incoming message
        SimpleChecksum checksumReceived;
        int state;  // state in the management of the incoming message
        int nReceivedBytes;  // number of received bytes of the current incoming message
        
    
    
    ////////////////////////////////////////////////////////////////
    // PUBLIC CONSTRUCTORS
    ////////////////////////////////////////////////////////////////
    public:
        MessageSerializerDeserializer();
    
    
    
    ////////////////////////////////////////////////////////////////
    // PUBLIC METHODS
    ////////////////////////////////////////////////////////////////
    public:
        // manages the bytes of the incoming message
        int8_t* manageInputByte( const int8_t newByte );
        int receivedBytesLength() const;
        // prepares the bytes of an outgoing message
        int8_t* prepareBytesToWrite( const int8_t* message , const int messageLength );
        int preparedBytesToWriteLength() const;

};


#endif  // #ifndef _MESSAGE_SERIALIZER_DESERIALIZER_H_
