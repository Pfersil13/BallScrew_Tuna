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

    bool connectWifi(uint32_t timeout_ms);

    void initMQTT();
    void callback(char* topic, byte* message, unsigned int length);
    void reconnect();
    void MQTTloop();
    void Pub();
    void recvMsg(uint8_t *data, size_t len);

#endif