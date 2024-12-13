#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H

class DebugUtils {
public:
    static void printSensorValues(int* values, int numSensors) {
        Serial.print("Sensors: ");
        for (int i = 0; i < numSensors; i++) {
            Serial.print(values[i]);
            Serial.print("\t");
        }
        Serial.println();
    }

    static void printPIDValues(float error, float correction) {
        Serial.print("Error: ");
        Serial.print(error);
        Serial.print("\tCorrection: ");
        Serial.println(correction);
    }

    static void printJunction(JunctionType junction) {
        Serial.print("Junction: ");
        switch (junction) {
            case NO_JUNCTION: Serial.println("None"); break;
            case LEFT_TURN: Serial.println("Left Turn"); break;
            case RIGHT_TURN: Serial.println("Right Turn"); break;
            case T_JUNCTION: Serial.println("T Junction"); break;
            case CROSS_JUNCTION: Serial.println("Cross Junction"); break;
            case END_OF_LINE: Serial.println("End of Line"); break;
        }
    }
};

#endif 