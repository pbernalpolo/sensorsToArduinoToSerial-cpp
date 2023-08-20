#ifndef _GYROSCOPE_MEASUREMENT_PACKET_H_
#define _GYROSCOPE_MEASUREMENT_PACKET_H_



class GyroscopeMeasurementPacket
{
    ////////////////////////////////////////////////////////////////
    // PUBLIC ABSTRACT METHODS
    ////////////////////////////////////////////////////////////////
    public:
        virtual double xGyroscope() = 0;
        virtual double yGyroscope() = 0;
        virtual double zGyroscope() = 0;
        virtual double temperatureGyroscope() = 0;

};


#endif  // #ifndef _GYROSCOPE_MEASUREMENT_PACKET_H_
