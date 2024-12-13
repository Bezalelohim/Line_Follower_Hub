#ifndef THREE_CHANNEL_H
#define THREE_CHANNEL_H

#include "../sensor_interface.h"

class ThreeChannelSensor : public SensorInterface {
private:
    int sensorValues[3];
    int calibratedMin[3];
    int calibratedMax[3];

public:
    ThreeChannelSensor(int leftPin, int centerPin, int rightPin);
    void init() override;
    int* readSensors() override;
    int getPosition() override;
    void calibrate() override;
};

#endif 