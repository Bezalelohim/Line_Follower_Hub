#ifndef MOTOR_INTERFACE_H
#define MOTOR_INTERFACE_H

class MotorInterface {
protected:
    int maxSpeed;

public:
    MotorInterface(int max_speed = 255) : maxSpeed(max_speed) {}
    virtual void init() = 0;
    virtual void setSpeed(int leftSpeed, int rightSpeed) = 0;
    virtual void stop() = 0;
    virtual void brake() = 0;
};

#endif 