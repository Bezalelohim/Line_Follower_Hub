#ifndef EIGHT_CHANNEL_JUNCTION_H
#define EIGHT_CHANNEL_JUNCTION_H

#include "junction_detector.h"

class EightChannelJunction {
private:
    int threshold;
    bool lastReadings[8];
    int debounceTime;
    unsigned long lastJunctionTime;
    int consecutiveReadings;

public:
    EightChannelJunction(int thresh = 500) 
        : threshold(thresh), debounceTime(200), 
          lastJunctionTime(0), consecutiveReadings(0) {}

    JunctionType detectJunction(int* sensorValues) {
        // Debounce check
        if (millis() - lastJunctionTime < debounceTime) {
            return NO_JUNCTION;
        }

        // Convert analog readings to binary
        int activeCount = 0;
        for (int i = 0; i < 8; i++) {
            lastReadings[i] = (sensorValues[i] > threshold);
            if (lastReadings[i]) activeCount++;
        }

        // Pattern analysis for 8 sensors [L4 L3 L2 L1 R1 R2 R3 R4]
        bool leftSection = (lastReadings[0] || lastReadings[1]);  // L4 or L3
        bool leftCenter = (lastReadings[2] || lastReadings[3]);   // L2 or L1
        bool rightCenter = (lastReadings[4] || lastReadings[5]);  // R1 or R2
        bool rightSection = (lastReadings[6] || lastReadings[7]); // R3 or R4

        // All sensors white - possible end of line
        if (activeCount == 0) {
            consecutiveReadings++;
            if (consecutiveReadings > 3) {  // Require multiple readings
                lastJunctionTime = millis();
                consecutiveReadings = 0;
                return END_OF_LINE;
            }
        } else {
            consecutiveReadings = 0;
        }

        // Cross junction detection (at least 6 sensors active)
        if (activeCount >= 6) {
            lastJunctionTime = millis();
            return CROSS_JUNCTION;
        }

        // T-Junction detection
        if ((leftSection && leftCenter && rightCenter) || 
            (leftCenter && rightCenter && rightSection)) {
            if (activeCount >= 5) {
                lastJunctionTime = millis();
                return T_JUNCTION;
            }
        }

        // Acute angle detection (45 degrees)
        if (leftSection && !rightSection && !rightCenter) {
            lastJunctionTime = millis();
            return LEFT_TURN;
        }

        if (!leftSection && !leftCenter && rightSection) {
            lastJunctionTime = millis();
            return RIGHT_TURN;
        }

        return NO_JUNCTION;
    }

    // Helper method to detect line thickness
    int getLineThickness(int* sensorValues) {
        int activeCount = 0;
        for (int i = 0; i < 8; i++) {
            if (sensorValues[i] > threshold) activeCount++;
        }
        return activeCount;
    }

    // Method to get precise angle deviation
    float getDeviation(int* sensorValues) {
        long weightedSum = 0;
        int sum = 0;
        
        for (int i = 0; i < 8; i++) {
            if (sensorValues[i] > threshold) {
                weightedSum += (long)(i - 3.5) * 1000;  // Center at 3.5
                sum++;
            }
        }
        
        if (sum > 0) {
            return (float)weightedSum / sum;
        }
        return 0;
    }

    // Check if the line is roughly centered
    bool isLineCentered(int* sensorValues) {
        return (sensorValues[3] > threshold || sensorValues[4] > threshold) &&
               !(sensorValues[0] > threshold || sensorValues[7] > threshold);
    }
};

#endif 