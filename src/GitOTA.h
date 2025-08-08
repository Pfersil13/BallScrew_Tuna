#ifndef GITOTA_H
    #define GITOTA_H

    #include <Arduino.h>
    #include <ESP32OTAPull.h>

    #define VERSION "1.0.0"

    void CheckNewFirmware();
    void callback_percent(int offset, int totallength);
    const char *errtext(int code);



#endif