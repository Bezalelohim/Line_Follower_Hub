#ifndef FOUR_CHANNEL_H
#define FOUR_CHANNEL_H

#include "../sensor_interface.h"

class FourChannelSensor : public SensorInterface {
private:
    int sensorValues[4];
    int calibratedMin[4];
    int calibratedMax[4];
    const int numSamples = 10;  // Number of samples for calibration

public:
    FourChannelSensor(int* pins) : SensorInterface(pins, 4) {
        for (int i = 0; i < 4; i++) {
            calibratedMin[i] = 1023;
            calibratedMax[i] = 0;
        }
    }

    void init() override {
        for (int i = 0; i < numSensors; i++) {
            pinMode(sensorPins[i], INPUT);
        }
    }

    int* readSensors() override {
        for (int i = 0; i < numSensors; i++) {
            sensorValues[i] = analogRead(sensorPins[i]);
        }
        return sensorValues;
    }

    void calibrate() override {
        // Calibration routine
        for (int sample = 0; sample < numSamples; sample++) {
            for (int i = 0; i < numSensors; i++) {
                int value = analogRead(sensorPins[i]);
                calibratedMin[i] = min(calibratedMin[i], value);
                calibratedMax[i] = max(calibratedMax[i], value);
            }
            delay(10);
        }
    }

    int getPosition() override {
        readSensors();
        long weighted_sum = 0;
        int sum = 0;
        
        for (int i = 0; i < numSensors; i++) {
            int value = map(sensorValues[i], 
                          calibratedMin[i], calibratedMax[i], 
                          0, 1000);
            value = constrain(value, 0, 1000);
            
            weighted_sum += (long)value * (i * 1000);
            sum += value;
        }

        if (sum > 0) {
            return (weighted_sum / sum) - 1500;  // Center position at 0
        }
        return 0;  // Default position
    }

    // Additional helper methods specific to 4-channel configuration
    bool isOnLine() {
        readSensors();
        for (int i = 0; i < numSensors; i++) {
            if (sensorValues[i] > threshold) {
                return true;
            }
        }
        return false;
    }

    bool isLineCenter() {
        readSensors();
        // Check if the two middle sensors detect the line
        return (sensorValues[1] > threshold && sensorValues[2] > threshold);
    }
};

#endif 