#include <Arduino.h>

#include <SPI.h>
#include <ArduinoJson.h>
#include "File.h"
#include "LittleFS.h"
#include "WebSerial.h"
#include "WifiFunctions.h"
#include "Sequence.h"
#define SERIAL_DEBUG

HardwareSerial GRBLSerial(1); // UART1

#define GRBL_TX 17  // TX del ESP32 -> RX del GRBL
#define GRBL_RX 18  // RX del ESP32 <- TX del GRBL
void sendSequenceToGRBL(const Sequence& seq, HardwareSerial& grblSerial);
SineWave wave[3];
Sequence currentSequence;


void setup(){

  Serial.begin(115200);
  GRBLSerial.begin(115200, SERIAL_8N1, GRBL_RX, GRBL_TX); // 115200 es el baud rate típico de GRBL

  delay(5000);
  Serial.println("Setup started");

  if (!LittleFS.begin()) {
      Serial.println("Failed to mount LittleFS");
      return;
    }


  /*if (!readSequencesFromFile("/secuencias.json", &mySystem.machine.sequence)) {
      Serial.println("Failed to read sequences");
      return;
    }
  printSequences(&mySystem.machine.sequence);
  */
  Serial.println("Setup ended");
   wave[0].amplitude = 1;
  wave[1].amplitude = 1;
  wave[2].amplitude = 1;

  wave[0].phase = 0;
  wave[1].phase =PI;
  wave[2].phase = PI;

  generateSineSequence(&currentSequence,wave,0.1);
  //saveSequenceToJson(currentSequence);

  GRBLSerial.println("$X");         // Desbloquear
  GRBLSerial.println("G21 G91");    // mm + incremental
  GRBLSerial.println("G0 X1000 Y1000"); // Movimiento

  }

 


 
void loop() {

 static bool wifiReady = false;

  // 1. Terminal físico (Serial) → GRBL
  while (Serial.available()) {
    char c = Serial.read();
    GRBLSerial.write(c);
  }

  // 2. GRBL → Serial y WebSerial
  while (GRBLSerial.available()) {
    char c = GRBLSerial.read();
    Serial.write(c);         // Monitor físico
    WebSerial.write(c);      // Web terminal
  }

  
 if (!connectWifi(10000)) {
    wifiReady = false;
    Serial.println("Intento");
    int status = WiFi.status();
    Serial.print("WiFi Status: ");
    Serial.println(status);
  }
  else{
    wifiReady = true;
  }
    
  //Ejecuta todo lo relacionado con el wifi
  if(wifiReady){
    MQTTloop();
    //Pub();
    //delay(5000);
    ElegantOTA.loop();
    WebSerial.loop();
  }

//Serial.println("HI");
delay(100);
//sendSequenceToGRBL(currentSequence, GRBLSerial);

}
 
 





