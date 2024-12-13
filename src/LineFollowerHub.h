#ifndef LINE_FOLLOWER_HUB_H
#define LINE_FOLLOWER_HUB_H

// Include all the necessary headers
#include "sensors/SensorInterface.h"
#include "sensors/ThreeChannel.h"
#include "sensors/FourChannel.h"
#include "sensors/EightChannel.h"
#include "algorithms/PIDController.h"
#include "algorithms/advanced/JunctionDetector.h"
#include "algorithms/advanced/FourChannelJunction.h"
#include "algorithms/advanced/EightChannelJunction.h"
#include "algorithms/advanced/MazeSolver.h"
#include "motors/MotorInterface.h"
#include "motors/L298NDriver.h"
#include "utils/DebugUtils.h"

#endif 