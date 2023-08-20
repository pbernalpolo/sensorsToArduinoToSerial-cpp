#ifndef _MAGNETOMETER_MEASUREMENT_PACKET_H_
#define _MAGNETOMETER_MEASUREMENT_PACKET_H_



class MagnetometerMeasurementPacket
{
    ////////////////////////////////////////////////////////////////
    // PUBLIC ABSTRACT METHODS
    ////////////////////////////////////////////////////////////////
    public:
        virtual double xMagnetometer() = 0;
        virtual double yMagnetometer() = 0;
        virtual double zMagnetometer() = 0;
        virtual double temperatureMagnetometer() = 0;

};


#endif  // #ifndef _MAGNETOMETER_MEASUREMENT_PACKET_H_
