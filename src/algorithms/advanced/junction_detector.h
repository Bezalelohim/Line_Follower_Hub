#ifndef JUNCTION_DETECTOR_H
#define JUNCTION_DETECTOR_H

enum JunctionType {
    NO_JUNCTION,
    LEFT_TURN,
    RIGHT_TURN,
    T_JUNCTION,
    CROSS_JUNCTION,
    END_OF_LINE
};

class JunctionDetector {
private:
    int numSensors;
    int threshold;
    bool* lastReadings;
    
public:
    JunctionDetector(int num_sensors, int thresh = 500) 
        : numSensors(num_sensors), threshold(thresh) {
        lastReadings = new bool[num_sensors];
    }

    ~JunctionDetector() {
        delete[] lastReadings;
    }

    JunctionType detectJunction(int* sensorValues) {
        // Convert analog readings to binary
        for (int i = 0; i < numSensors; i++) {
            lastReadings[i] = (sensorValues[i] > threshold);
        }

        // Detect junction patterns
        bool allBlack = true;
        bool allWhite = true;
        bool leftTurn = false;
        bool rightTurn = false;

        for (int i = 0; i < numSensors; i++) {
            if (lastReadings[i]) allWhite = false;
            else allBlack = false;

            if (i < numSensors/2 && lastReadings[i]) leftTurn = true;
            if (i >= numSensors/2 && lastReadings[i]) rightTurn = true;
        }

        if (allWhite) return END_OF_LINE;
        if (allBlack) return CROSS_JUNCTION;
        if (leftTurn && rightTurn) return T_JUNCTION;
        if (leftTurn) return LEFT_TURN;
        if (rightTurn) return RIGHT_TURN;
        
        return NO_JUNCTION;
    }
};

#endif 