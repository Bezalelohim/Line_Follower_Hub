#include "../../src/sensors/three_channel/three_channel.h"
#include "../../src/algorithms/pid/pid_controller.h"

// Pin definitions
const int LEFT_SENSOR = A0;
const int CENTER_SENSOR = A1;
const int RIGHT_SENSOR = A2;

const int MOTOR_LEFT_FWD = 5;
const int MOTOR_LEFT_BWD = 6;
const int MOTOR_RIGHT_FWD = 9;
const int MOTOR_RIGHT_BWD = 10;

// Create objects
int sensorPins[] = {LEFT_SENSOR, CENTER_SENSOR, RIGHT_SENSOR};
ThreeChannelSensor sensors(LEFT_SENSOR, CENTER_SENSOR, RIGHT_SENSOR);
PIDController pid(1.0, 0.0, 0.1);

const int BASE_SPEED = 150;

void setup() {
    Serial.begin(9600);
    sensors.init();
    sensors.calibrate();

    // Setup motor pins
    pinMode(MOTOR_LEFT_FWD, OUTPUT);
    pinMode(MOTOR_LEFT_BWD, OUTPUT);
    pinMode(MOTOR_RIGHT_FWD, OUTPUT);
    pinMode(MOTOR_RIGHT_BWD, OUTPUT);
}

void loop() {
    int position = sensors.getPosition();
    float correction = pid.compute(0, position);

    // Apply motor speeds
    int leftSpeed = BASE_SPEED + correction;
    int rightSpeed = BASE_SPEED - correction;

    // Constrain speeds
    leftSpeed = constrain(leftSpeed, 0, 255);
    rightSpeed = constrain(rightSpeed, 0, 255);

    // Drive motors
    analogWrite(MOTOR_LEFT_FWD, leftSpeed);
    analogWrite(MOTOR_LEFT_BWD, 0);
    analogWrite(MOTOR_RIGHT_FWD, rightSpeed);
    analogWrite(MOTOR_RIGHT_BWD, 0);
} 