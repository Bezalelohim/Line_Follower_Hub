#include "../../src/sensors/four_channel/four_channel.h"
#include "../../src/algorithms/pid/pid_controller.h"
#include "../../src/motors/l298n_driver.h"
#include "../../src/utils/debug_utils.h"

// Pin definitions for 4 sensors (from left to right)
int sensorPins[] = {A0, A1, A2, A3};

// Motor pins
const int MOTOR_LEFT_EN = 5;
const int MOTOR_RIGHT_EN = 6;
const int MOTOR_LEFT_FWD = 7;
const int MOTOR_LEFT_BWD = 8;
const int MOTOR_RIGHT_FWD = 9;
const int MOTOR_RIGHT_BWD = 10;

// Create objects
FourChannelSensor sensors(sensorPins);
PIDController pid(1.0, 0.1, 0.5);  // Adjust these values based on your robot
L298NDriver motors(MOTOR_LEFT_FWD, MOTOR_LEFT_BWD, 
                   MOTOR_RIGHT_FWD, MOTOR_RIGHT_BWD,
                   MOTOR_LEFT_EN, MOTOR_RIGHT_EN);

const int BASE_SPEED = 150;
bool isCalibrated = false;

void setup() {
    Serial.begin(9600);
    sensors.init();
    motors.init();
    
    Serial.println("Press any key to start calibration...");
    while (!Serial.available());
    Serial.read();
    
    Serial.println("Calibrating sensors...");
    calibrateSensors();
    Serial.println("Calibration complete!");
    
    isCalibrated = true;
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
    if (!isCalibrated) return;

    int* sensorValues = sensors.readSensors();
    int position = sensors.getPosition();

    // Debug output
    DebugUtils::printSensorValues(sensorValues, 4);
    
    if (sensors.isOnLine()) {
        // Normal line following with PID
        float correction = pid.compute(0, position);
        DebugUtils::printPIDValues(position, correction);
        
        int leftSpeed = BASE_SPEED + correction;
        int rightSpeed = BASE_SPEED - correction;
        
        // Constrain speeds
        leftSpeed = constrain(leftSpeed, 0, 255);
        rightSpeed = constrain(rightSpeed, 0, 255);
        
        motors.setSpeed(leftSpeed, rightSpeed);
    } else {
        // Lost the line - implement recovery behavior
        motors.stop();
        Serial.println("Line lost!");
        
        // Simple recovery: rotate until line is found
        while (!sensors.isOnLine()) {
            motors.setSpeed(-100, 100);  // Rotate in place
            delay(50);
        }
    }

    delay(10);  // Small delay to prevent overwhelming the serial output
} 