#ifndef FOUR_CHANNEL_JUNCTION_H
#define FOUR_CHANNEL_JUNCTION_H

#include "junction_detector.h"

class FourChannelJunction {
private:
    int threshold;
    bool lastReadings[4];
    int debounceTime;
    unsigned long lastJunctionTime;

public:
    FourChannelJunction(int thresh = 500) 
        : threshold(thresh), debounceTime(200), lastJunctionTime(0) {}

    JunctionType detectJunction(int* sensorValues) {
        // Debounce check
        if (millis() - lastJunctionTime < debounceTime) {
            return NO_JUNCTION;
        }

        // Convert analog readings to binary
        for (int i = 0; i < 4; i++) {
            lastReadings[i] = (sensorValues[i] > threshold);
        }

        // Pattern analysis for 4 sensors [L2 L1 R1 R2]
        bool leftMost = lastReadings[0];    // L2
        bool leftInner = lastReadings[1];   // L1
        bool rightInner = lastReadings[2];  // R1
        bool rightMost = lastReadings[3];   // R2

        // All white - line lost or end of line
        if (!leftMost && !leftInner && !rightInner && !rightMost) {
            lastJunctionTime = millis();
            return END_OF_LINE;
        }

        // All black - possible cross junction
        if (leftMost && leftInner && rightInner && rightMost) {
            lastJunctionTime = millis();
            return CROSS_JUNCTION;
        }

        // T-Junction detection
        if ((leftMost && leftInner && rightInner) || 
            (leftInner && rightInner && rightMost)) {
            lastJunctionTime = millis();
            return T_JUNCTION;
        }

        // Left turn detection
        if (leftMost && !rightMost) {
            lastJunctionTime = millis();
            return LEFT_TURN;
        }

        // Right turn detection
        if (!leftMost && rightMost) {
            lastJunctionTime = millis();
            return RIGHT_TURN;
        }

        return NO_JUNCTION;
    }

    // Helper method to check if on straight line
    bool isOnStraightLine(int* sensorValues) {
        return (sensorValues[1] > threshold && sensorValues[2] > threshold &&
                sensorValues[0] < threshold && sensorValues[3] < threshold);
    }

    // Method to get angle deviation from center
    int getDeviation(int* sensorValues) {
        int leftWeight = (sensorValues[0] * 3 + sensorValues[1] * 1);
        int rightWeight = (sensorValues[2] * 1 + sensorValues[3] * 3);
        return leftWeight - rightWeight;
    }
};

#endif 