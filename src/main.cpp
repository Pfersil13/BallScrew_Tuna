#include <Arduino.h>

#include <SPI.h>
#include <ArduinoJson.h>
#include "File.h"
#include "LittleFS.h"
#include "WebSerial.h"
#include "WifiFunctions.h"
#include "Sequence.h"
#include "GRBL.h"


unsigned long startMillis;
const unsigned long limiteMillis = 16UL * 3600UL * 1000UL; // 16 horas
const unsigned long wifiTime =   15*60* 1000UL; // 15min 
const unsigned long wifiTimeBlocked =   1*60* 1000UL; // 15min 
HardwareSerial GRBLSerial(1); // UART1


SineWave wave[3];
Sequence currentSequence;

void setup(){

  Serial.begin(115200);
  GRBLSerial.begin(115200, SERIAL_8N1, GRBL_RX, GRBL_TX); // 115200 es el baud rate típico de GRBL

  delay(5000);
  Serial.println("Setup started");

  // Cargar al iniciar 
  //savePreferences();
  loadPreferences();
  loadWave(wave);
  Serial.println("Setup ended");


  //wave[0].amplitude = 20;
  //wave[1].amplitude = 20;//30;
  //wave[2].amplitude = 9;

  //wave[0].phase = 180;
  //wave[1].phase =0;
  //wave[2].phase = 180;

  generateSineSequence(&currentSequence,wave); //0.06

   delay(1000);

  sendGRBLConfig(GRBLSerial);
  delay(1000);
  doHomingAndWait(GRBLSerial);
  startMillis = millis();
  GRBLSerial.println("$21=1"); //HARD LIMITS
  delay(100);
  Serial.println("G92 GOOO");
  char gcode[32];
  snprintf(gcode, sizeof(gcode), "G92 X%.3f Y%.3f Z%.3f", -GRBL_MIN_POS_U,-GRBL_MIN_POS_V, -GRBL_MIN_POS_W); //20 40 10
  GRBLSerial.println(gcode);
  //GRBLSerial.println("G92 X20 Y40 Z10");
  delay(100);
  Serial.println("G1 GOOO");
  GRBLSerial.println("G1 X0 Y0 Z0 F1000");

  }

 


 
void loop() {

 static bool wifiReady = false;

  // 1. Terminal físico (Serial) → GRBL
  while (Serial.available()) {
    char c = Serial.read();
    GRBLSerial.write(c);
  }


sendSequenceToGRBL(currentSequence, GRBLSerial);
readGrbl(GRBLSerial);



      if(millis() - startMillis <= wifiTime){ //Si menor 15min
        if (!connectWifi()) {
          wifiReady = false;
         // Serial.println("Intento");
          int status = WiFi.status();
          //Serial.print("WiFi Status: ");
          //Serial.println(status);
        }
        else{
          wifiReady = true;
        }
      }


      //Si pasan 16h
      if (millis() - startMillis >= limiteMillis) {
        Serial.println("⏳ Tiempo límite alcanzado. Parando máquina.");
        GRBLSerial.println("G1 X0 Y0 Z0 F100");  // Feed hold
        delay(10000);
        GRBLSerial.println("!");  // Feed hold
        delay(500);
        GRBLSerial.println("\x18"); // Ctrl+X reset GRBL
        while (true) delay(1000);   // Bucle infinito para no seguir
      }

      //Ejecuta todo lo relacionado con el wifi
      if(wifiReady){
        //while(millis() - startMillis < wifiTimeBlocked ){
        //  ElegantOTA.loop();
        //  WebSerial.loop();
        //}
        //MQTTloop();
        ElegantOTA.loop();
        WebSerial.loop();
      }


      seqUpdate();

}
 
 





