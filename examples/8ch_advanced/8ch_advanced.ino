#include "../../src/sensors/eight_channel/eight_channel.h"
#include "../../src/algorithms/pid/pid_controller.h"
#include "../../src/algorithms/advanced/junction_detector.h"
#include "../../src/algorithms/advanced/maze_solver.h"
#include "../../src/motors/l298n_driver.h"
#include "../../src/utils/debug_utils.h"

// Pin definitions for 8 sensors
int sensorPins[] = {A0, A1, A2, A3, A4, A5, A6, A7};

// Motor pins
const int MOTOR_LEFT_EN = 5;
const int MOTOR_RIGHT_EN = 6;
const int MOTOR_LEFT_FWD = 7;
const int MOTOR_LEFT_BWD = 8;
const int MOTOR_RIGHT_FWD = 9;
const int MOTOR_RIGHT_BWD = 10;

// Create objects
EightChannelSensor sensors(sensorPins);
PIDController pid(1.0, 0.1, 0.5);
JunctionDetector junctionDetector(8);
MazeSolver mazeSolver;
L298NDriver motors(MOTOR_LEFT_FWD, MOTOR_LEFT_BWD, 
                   MOTOR_RIGHT_FWD, MOTOR_RIGHT_BWD,
                   MOTOR_LEFT_EN, MOTOR_RIGHT_EN);

const int BASE_SPEED = 150;
bool solving = true;

void setup() {
    Serial.begin(9600);
    sensors.init();
    motors.init();
    
    Serial.println("Calibrating sensors...");
    calibrateSensors();
    Serial.println("Calibration complete!");
    
    delay(2000);  // Wait before starting
}

void calibrateSensors() {
    // Move robot back and forth during calibration
    for (int i = 0; i < 5; i++) {
        motors.setSpeed(100, -100);
        delay(200);
        sensors.calibrate();
        motors.setSpeed(-100, 100);
        delay(200);
        sensors.calibrate();
    }
    motors.stop();
}

void loop() {
    int* sensorValues = sensors.readSensors();
    int position = sensors.getPosition();
    JunctionType junction = junctionDetector.detectJunction(sensorValues);

    // Debug output
    DebugUtils::printSensorValues(sensorValues, 8);
    DebugUtils::printJunction(junction);

    if (junction != NO_JUNCTION && solving) {
        motors.stop();
        char turn = mazeSolver.decideTurn(junction);
        executeTurn(turn);
    } else {
        // Normal line following with PID
        float correction = pid.compute(0, position);
        DebugUtils::printPIDValues(position, correction);
        
        int leftSpeed = BASE_SPEED + correction;
        int rightSpeed = BASE_SPEED - correction;
        motors.setSpeed(leftSpeed, rightSpeed);
    }
}

void executeTurn(char turn) {
    switch (turn) {
        case 'L':
            motors.setSpeed(-100, 100);
            delay(500);
            break;
        case 'R':
            motors.setSpeed(100, -100);
            delay(500);
            break;
        case 'B':
            motors.setSpeed(100, -100);
            delay(1000);
            break;
    }
} 