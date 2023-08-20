#ifndef _ACCELEROMETER_MEASUREMENT_PACKET_H_
#define _ACCELEROMETER_MEASUREMENT_PACKET_H_



class AccelerometerMeasurementPacket
{
    ////////////////////////////////////////////////////////////////
    // PUBLIC ABSTRACT METHODS
    ////////////////////////////////////////////////////////////////
    public:
        virtual double xAccelerometer() = 0;
        virtual double yAccelerometer() = 0;
        virtual double zAccelerometer() = 0;
        virtual double temperatureAccelerometer() = 0;

};


#endif  // #ifndef _ACCELEROMETER_MEASUREMENT_PACKET_H_
