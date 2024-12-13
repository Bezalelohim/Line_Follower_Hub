cmake_minimum_required(VERSION 3.12)
project(LineFollowerHub VERSION 1.0)

# Set Arduino specific variables
set(ARDUINO_PATH "/usr/share/arduino" CACHE PATH "Path to Arduino installation")
set(ARDUINO_BOARD "uno" CACHE STRING "Arduino board type")
set(ARDUINO_PORT "/dev/ttyACM0" CACHE STRING "Arduino port")

# Find Arduino if it's being used
find_package(Arduino REQUIRED)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/sensors
    ${CMAKE_SOURCE_DIR}/src/algorithms
    ${CMAKE_SOURCE_DIR}/src/motors
    ${CMAKE_SOURCE_DIR}/src/utils
)

# Source files for the library
set(LIB_SOURCES
    src/sensors/sensor_interface.h
    src/sensors/three_channel/three_channel.h
    src/sensors/four_channel/four_channel.h
    src/sensors/eight_channel/eight_channel.h
    src/algorithms/pid/pid_controller.h
    src/algorithms/advanced/junction_detector.h
    src/algorithms/advanced/four_channel_junction.h
    src/algorithms/advanced/eight_channel_junction.h
    src/algorithms/advanced/maze_solver.h
    src/motors/motor_interface.h
    src/motors/l298n_driver.h
    src/utils/debug_utils.h
)

# Create library
add_library(line_follower_lib STATIC ${LIB_SOURCES})

# Example targets
set(EXAMPLE_TARGETS
    3ch_pid
    4ch_pid
    8ch_advanced
)

# Create targets for each example
foreach(target ${EXAMPLE_TARGETS})
    # Generate Arduino sketch
    generate_arduino_firmware(${target}
        SRCS examples/${target}/${target}.ino
        LIBS line_follower_lib
        BOARD ${ARDUINO_BOARD}
        PORT ${ARDUINO_PORT}
    )
endforeach()

# Testing configuration (if needed)
option(BUILD_TESTING "Build tests" OFF)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

# Installation rules
install(TARGETS line_follower_lib
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY src/
    DESTINATION include/line_follower
    FILES_MATCHING PATTERN "*.h"
)

# Documentation
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
endif()

# Package configuration
include(CPack)
set(CPACK_PACKAGE_NAME "line_follower_hub")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Line Follower Robot Hub")
set(CPACK_PACKAGE_VENDOR "Your Organization")

# Output configuration information
message(STATUS "Configuration Summary:")
message(STATUS "  Arduino Board: ${ARDUINO_BOARD}")
message(STATUS "  Arduino Port: ${ARDUINO_PORT}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Install Prefix: ${CMAKE_INSTALL_PREFIX}") 