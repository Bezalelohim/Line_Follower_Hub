#include <LineFollowerHub.h>

// Pin definitions for 4 sensors
const int sensorPins[] = {A0, A1, A2, A3};
const int BASE_SPEED = 200;  // Base motor speed (0-255)
const int MAX_SPEED = 255;   // Maximum motor speed

// Create objects
FourChannelSensor sensors(sensorPins);
PIDController pid(1.0, 0.1, 0.5);  // Kp, Ki, Kd values
L298NDriver motors(7, 8, 9, 10, 5, 6);  // IN1, IN2, IN3, IN4, ENA, ENB

void setup() {
    // Initialize Serial for debugging
    Serial.begin(9600);
    Serial.println("Line Follower Starting...");

    // Initialize sensors and motors
    sensors.init();
    motors.init();

    // Perform sensor calibration
    Serial.println("Calibrating sensors...");
    Serial.println("Move robot across the line several times");
    
    // Calibrate for 5 seconds
    sensors.startCalibration();
    for(int i = 0; i < 5000; i += 100) {
        sensors.calibrate();
        delay(100);
    }
    sensors.endCalibration();
    
    Serial.println("Calibration complete!");
    delay(1000);  // Wait before starting
}

void loop() {
    // Read position (-1000 to +1000)
    int position = sensors.getPosition();
    
    // Compute PID correction
    float correction = pid.compute(0, position);  // Target position is 0
    
    // Calculate motor speeds
    int leftSpeed = BASE_SPEED + correction;
    int rightSpeed = BASE_SPEED - correction;
    
    // Constrain speeds to valid range
    leftSpeed = constrain(leftSpeed, 0, MAX_SPEED);
    rightSpeed = constrain(rightSpeed, 0, MAX_SPEED);
    
    // Apply speeds to motors
    motors.setSpeed(leftSpeed, rightSpeed);
    
    // Debug output
    if (Serial) {
        static unsigned long lastDebugTime = 0;
        if (millis() - lastDebugTime > 100) {  // Print every 100ms
            Serial.print("Position: ");
            Serial.print(position);
            Serial.print("\tCorrection: ");
            Serial.print(correction);
            Serial.print("\tSpeeds (L,R): ");
            Serial.print(leftSpeed);
            Serial.print(",");
            Serial.println(rightSpeed);
            
            lastDebugTime = millis();
        }
    }
    
    // Check for junction detection
    if (sensors.isJunction()) {
        Serial.println("Junction detected!");
        // Add your junction handling code here
    }
    
    // Small delay to prevent overwhelming the system
    delay(10);
}