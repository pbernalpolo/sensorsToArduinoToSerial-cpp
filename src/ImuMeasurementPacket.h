#ifndef _IMU_MEASUREMENT_PACKET_H_
#define _IMU_MEASUREMENT_PACKET_H_


#include "GyroscopeMeasurementPacket.h"
#include "AccelerometerMeasurementPacket.h"



class ImuMeasurementPacket
    : GyroscopeMeasurementPacket , AccelerometerMeasurementPacket
{

};


#endif  // #ifndef _IMU_MEASUREMENT_PACKET_H_
