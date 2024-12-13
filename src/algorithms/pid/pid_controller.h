#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

class PIDController {
private:
    float kp, ki, kd;
    float lastError;
    float integral;
    unsigned long lastTime;

public:
    PIDController(float p, float i, float d) 
        : kp(p), ki(i), kd(d), lastError(0), integral(0), lastTime(0) {}
    
    float compute(float setpoint, float input) {
        unsigned long now = millis();
        float deltaTime = (now - lastTime) / 1000.0;
        lastTime = now;

        float error = setpoint - input;
        integral += error * deltaTime;
        float derivative = (error - lastError) / deltaTime;
        lastError = error;

        return kp * error + ki * integral + kd * derivative;
    }
};

#endif 