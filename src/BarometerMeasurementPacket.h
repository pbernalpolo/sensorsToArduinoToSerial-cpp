#ifndef _BAROMETER_MEASUREMENT_PACKET_H_
#define _BAROMETER_MEASUREMENT_PACKET_H_



class BarometerMeasurementPacket
{
    ////////////////////////////////////////////////////////////////
    // PUBLIC ABSTRACT METHODS
    ////////////////////////////////////////////////////////////////
    public:
        virtual double pressureBarometer() = 0;
        virtual double temperatureBarometer() = 0;

};


#endif  // #ifndef _BAROMETER_MEASUREMENT_PACKET_H_
