# Line Follower Hub

**Warning: This code is part of a university project and includes components developed with the assistance of AI. The AI has been used to add functionalities, restructure code, and help build tests. Please ensure you understand the code and its implications before using it in critical applications.**

A comprehensive Arduino library for line follower robots supporting multiple sensor configurations and advanced algorithms.

## Features

- Multiple sensor configurations (3, 4, or 8 channels)
- PID-based line following
- Advanced junction detection
- Maze solving capabilities
- L298N motor driver support
- Debug utilities for development

## Version History

### v3.2.1 (2023)
- Advanced maze solving algorithms
- Eight-channel sensor support
- Improved junction detection
- Enhanced debugging utilities

### v2.5.0 (2023)
- Four-channel sensor implementation
- PID controller refinements
- Added motor driver abstraction
- Junction detection capabilities

### v1.0.0 (2020)
- Initial release
- Basic three-channel sensor support
- Simple line following algorithms
- L298N motor driver support

## Installation

### Method 1: Arduino IDE Library Manager (Recommended)
1. Open Arduino IDE
2. Navigate to `Sketch -> Include Library -> Manage Libraries`
3. Search for "LineFollowerHub"
4. Click Install

### Method 2: Manual Installation
1. Download this repository as a ZIP file
2. Open Arduino IDE
3. Go to `Sketch -> Include Library -> Add .ZIP Library`
4. Select the downloaded ZIP file

## Hardware Setup

### Supported Configurations

#### 3-Channel Setup
- Connect IR sensors to analog pins:
  - Left Sensor → A0
  - Center Sensor → A1
  - Right Sensor → A2

#### 4-Channel Setup
- Connect IR sensors to analog pins:
  - Far Left → A0
  - Left → A1
  - Right → A2
  - Far Right → A3

#### 8-Channel Setup
- Connect IR sensors to analog pins A0 through A7

### Motor Connection (L298N)
- Enable A → Pin 5
- Enable B → Pin 6
- Input 1 → Pin 7
- Input 2 → Pin 8
- Input 3 → Pin 9
- Input 4 → Pin 10

## Basic Usage

### Simple 4-Channel Example

```cpp
include <LineFollowerHub.h>
// Pin definitions for 4 sensors
int sensorPins[] = {A0, A1, A2, A3};
// Create objects
FourChannelSensor sensors(sensorPins);
PIDController pid(1.0, 0.1, 0.5);
L298NDriver motors(7, 8, 9, 10, 5, 6);
void setup() {
sensors.init();
motors.init();
}
void loop() {
int position = sensors.getPosition();
float correction = pid.compute(0, position);
// Apply motor speeds
int leftSpeed = BASE_SPEED + correction;
int rightSpeed = BASE_SPEED - correction;
motors.setSpeed(leftSpeed, rightSpeed);
}
```

## Advanced Features

### Junction Detection
The library includes junction detection capabilities for:
- Left/Right turns
- T-junctions
- Cross junctions
- Dead ends

### Maze Solving
Built-in maze solving algorithms using:
- Left-hand rule
- Path optimization
- Junction memory

## Configuration

### PID Tuning
Adjust PID parameters in your code:

```cpp
PIDController pid(Kp, Ki, Kd);
// Example: PIDController pid(1.0, 0.1, 0.5);
```

### Sensor Calibration
Always calibrate sensors before use:
```cpp
void setup() {
sensors.init();
sensors.calibrate(); // Run calibration routine
}
```

### Debugging
Use DebugUtils to log sensor values and robot state:

```cpp
DebugUtils::log("Sensor value: " + String(sensorValue));
DebugUtils::log("Robot position: " + String(position));
``` 

Monitor output through Serial Monitor at 9600 baud.

## Examples

The library includes several example sketches:
- `3ch_pid`: Basic 3-channel line following
- `4ch_pid`: 4-channel PID control
- `8ch_advanced`: Advanced features with maze solving

### Contributors
- Bezalelohim 
- mellbillB 