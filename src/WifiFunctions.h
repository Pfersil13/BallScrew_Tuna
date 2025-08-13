#ifndef WIFIFUNCTIONS_H
    #define WIFIFUNCTIONS_H

    
    #include <Arduino.h>
    #include "Definitions.h"
    #include <ESPmDNS.h>
    #include <WiFiClientSecure.h>
    #include <AsyncTCP.h>
    #include <ESPAsyncWebServer.h>
    #include <ElegantOTA.h>
    #include "WebSerial.h"
    #include <PubSubClient.h>
    #include "ArduinoJson.h"
    #include "Sequence.h"
    #include <LittleFS.h>

    bool connectWifi();
    void recvMsg(uint8_t *data, size_t len);
    void seqUpdate();

#endif