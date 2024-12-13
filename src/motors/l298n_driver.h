#ifndef L298N_DRIVER_H
#define L298N_DRIVER_H

#include "motor_interface.h"

class L298NDriver : public MotorInterface {
private:
    int leftFwd, leftBwd, rightFwd, rightBwd;
    int leftEn, rightEn;  // Enable pins for PWM

public:
    L298NDriver(int left_fwd, int left_bwd, int right_fwd, int right_bwd,
                int left_en, int right_en, int max_speed = 255)
        : MotorInterface(max_speed),
          leftFwd(left_fwd), leftBwd(left_bwd),
          rightFwd(right_fwd), rightBwd(right_bwd),
          leftEn(left_en), rightEn(right_en) {}

    void init() override {
        pinMode(leftFwd, OUTPUT);
        pinMode(leftBwd, OUTPUT);
        pinMode(rightFwd, OUTPUT);
        pinMode(rightBwd, OUTPUT);
        pinMode(leftEn, OUTPUT);
        pinMode(rightEn, OUTPUT);
    }

    void setSpeed(int leftSpeed, int rightSpeed) override {
        leftSpeed = constrain(leftSpeed, -maxSpeed, maxSpeed);
        rightSpeed = constrain(rightSpeed, -maxSpeed, maxSpeed);

        // Left motor
        if (leftSpeed >= 0) {
            digitalWrite(leftFwd, HIGH);
            digitalWrite(leftBwd, LOW);
            analogWrite(leftEn, leftSpeed);
        } else {
            digitalWrite(leftFwd, LOW);
            digitalWrite(leftBwd, HIGH);
            analogWrite(leftEn, -leftSpeed);
        }

        // Right motor
        if (rightSpeed >= 0) {
            digitalWrite(rightFwd, HIGH);
            digitalWrite(rightBwd, LOW);
            analogWrite(rightEn, rightSpeed);
        } else {
            digitalWrite(rightFwd, LOW);
            digitalWrite(rightBwd, HIGH);
            analogWrite(rightEn, -rightSpeed);
        }
    }

    void stop() override {
        analogWrite(leftEn, 0);
        analogWrite(rightEn, 0);
    }

    void brake() override {
        digitalWrite(leftFwd, HIGH);
        digitalWrite(leftBwd, HIGH);
        digitalWrite(rightFwd, HIGH);
        digitalWrite(rightBwd, HIGH);
        analogWrite(leftEn, maxSpeed);
        analogWrite(rightEn, maxSpeed);
    }
};

#endif 