#ifndef GRBL_H
    #define GRBL_H

    #include <Arduino.h>
    #include "Sequence.h"
#include "WebSerial.h"
    // grbl_config.h
    #pragma once

    #define GRBL_TX 17  // TX del ESP32 -> RX del GRBL
    #define GRBL_RX 18  // RX del ESP32 <- TX del GRBL

    #define GRBL_STEPS_PER_DEG_U  444.0 //  Steps/º
    #define GRBL_STEPS_PER_DEG_V  200//640.0 //  Steps/mm
    #define GRBL_STEPS_PER_DEG_W  444.0 //  Steps/º

    #define GRBL_MAX_VELOCITY_U   1000
    #define GRBL_MAX_VELOCITY_V   1000
    #define GRBL_MAX_VELOCITY_W   500

        // Aceleración (mm/s²)
    #define GRBL_ACCEL_U          30
    #define GRBL_ACCEL_V          30
    #define GRBL_ACCEL_W          30

    #define GRBL_MICROSTEPPING    16

    #define GRBL_MIN_POS_U        -MAX_RANGE_U/2.f
    #define GRBL_MAX_POS_U        +MAX_RANGE_U/2.f

    #define GRBL_MIN_POS_V        -MAX_RANGE_V/2.f
    #define GRBL_MAX_POS_V        +MAX_RANGE_V/2.f

    #define GRBL_MIN_POS_W        -MAX_RANGE_W/2.f
    #define GRBL_MAX_POS_W        +MAX_RANGE_W/2.f

    

    void sendGRBLConfig(HardwareSerial &grbl);
    void doHomingAndWait(HardwareSerial &grbl);
    void sendUnlockAndWaitForOk(HardwareSerial &grbl);
    void readGrbl(HardwareSerial &grbl);
    bool sendGcodeWithAck(HardwareSerial& grblSerial, const char* gcode);
    #endif