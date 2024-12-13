#ifndef SENSOR_INTERFACE_H
#define SENSOR_INTERFACE_H

class SensorInterface {
protected:
    int* sensorPins;
    int numSensors;
    int threshold;

public:
    SensorInterface(int* pins, int count, int thresh = 500) 
        : sensorPins(pins), numSensors(count), threshold(thresh) {}
    
    virtual void init() = 0;
    virtual int* readSensors() = 0;
    virtual int getPosition() = 0;
    virtual void calibrate() = 0;
};

#endif 