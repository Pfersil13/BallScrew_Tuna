#include "WifiFunctions.h"
#include "HTML.h"
#include "secrets.h"
#include "File.h"

 bool  SerialDebugSequence =  false;
bool SerialDebug =  false;
extern bool ackReceived;
#define RETY_INTERVAL_MS 200
// Variables globales para los valores:
extern float period ;
extern float amplitude[];
extern float offset[] ;
extern float phase[] ;

extern SineWave wave[];
extern Sequence currentSequence;
String object;
unsigned long currentMillisOTA;
unsigned long LastMillis;

uint32_t wifi_up_time = 350000;
bool boot = 0;
bool conected = 0;

bool needsUpdate = false;
bool updatedOnce = true;

AsyncWebServer server(80);
extern HardwareSerial GRBLSerial; // UART1


bool connectWifi() {
  static bool initialized = false;
  static bool connected = false;
  static unsigned long startAttemptTime = 0;
  static unsigned long lastRetryTime = 0;

  // Paso 1: Inicializar WiFi una sola vez
  if (!initialized) {
    Serial.println("🔌 Inicializando WiFi...");
    WiFi.setHostname(hostname);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    WiFi.setTxPower(WIFI_POWER_13dBm);
    startAttemptTime = millis();
    lastRetryTime = 0; // forzar primer intento
    initialized = true;
  }

  // Paso 2: Comprobar si ya está conectado
  if (!connected) {
    if (millis() - lastRetryTime >= RETY_INTERVAL_MS) {
      lastRetryTime = millis();
      wl_status_t status = WiFi.status();

    if (status == WL_CONNECTED) {
      Serial.println("\nConectado a WiFi");
      Serial.print("IP: "); Serial.println(WiFi.localIP());
      Serial.print("Host: "); Serial.println(WiFi.getHostname());

      if (MDNS.begin(WiFi.getHostname())) {
        Serial.println("mDNS iniciado");
      }

      // Configurar servidor y OTA
      server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", PAGE_MAIN);
      });

      server.on("/sequence.json", HTTP_GET, [](AsyncWebServerRequest *request){
        if(updatedOnce == true){
          String Output = ShowOnWeb(currentSequence);
          Serial.println("Updated");
          request->send(200, "application/json", Output);
          //updatedOnce = false;
      } else {
        request->send(204); // No Content
      }
      });

      server.on("/updateSequence", HTTP_POST, 
    [](AsyncWebServerRequest *request){
      // Cuando termina el POST, responder al cliente:
      request->send(200, "application/json", "{\"status\":\"OK\"}");
    },
    NULL,
    [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
      // Esta función se llama con el cuerpo en partes (puede venir en varios trozos)
      // Solo parsear cuando es la última parte (index + len == total)
      if (index == 0) {
        // Limpiar buffer, etc., si usas uno
      }

      // Usaremos una String temporal para acumular (o un buffer estático)
      static String body;

      body += String((char*)data).substring(0, len);

      if (index + len == total) {
        // Última parte recibida, parsear JSON
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, body);
        if (error) {
          Serial.println("Error parseando JSON: " + String(error.c_str()));
          return;
        }
        float indice;
      if (doc.containsKey("period")) indice = doc["period"];

      period = 125.0f/indice;
      Serial.println("Viedno Periodo");
      const char* ampKeys[] = {"amp1","amp2","amp3"};
      const char* offsetKeys[] = {"offset1","offset2","offset3"};
      const char* phaseKeys[] = {"fase1","fase2","fase3"}; // usa "fase*" si ese es tu JSON

      for (int i = 0; i < 3; i++) {
          if (doc.containsKey(ampKeys[i])) amplitude[i] = doc[ampKeys[i]].as<float>();
          if (doc.containsKey(offsetKeys[i])) offset[i] = doc[offsetKeys[i]].as<float>();
          if (doc.containsKey(phaseKeys[i])) phase[i] = doc[phaseKeys[i]].as<float>();
      }



        savePreferences();
        needsUpdate = true;

        Serial.printf("Period: %.2f\n", period);
        Serial.printf("Amplitudes: %.2f %.2f %.2f\n", amplitude[0], amplitude[1], amplitude[2]);
        Serial.printf("Fases: %.2f %.2f %.2f\n", phase[0], phase[1], phase[2]);
        Serial.printf("Offsets: %.2f %.2f %.2f\n", offset[0], offset[1], offset[2]);
        // Limpiar body para próxima vez
        body = "";

      }
      
    });

    /*server.on("/getSequence", HTTP_GET, [](AsyncWebServerRequest *request){
      Serial.println("Intentandio");
      if(updatedOnce == true){
        String Output = ShowOnWeb(currentSequence);
        Serial.println("Updated");
        request->send(200, "application/json", Output);
        updatedOnce = false;
      } else {
        request->send(204); // No Content
    }
    });*/

    server.on("/load", HTTP_GET, [](AsyncWebServerRequest *request) {
      loadPreferences(); // para asegurarnos de que lee lo último

      String json = "{";
      json += "\"period\":" + String(period) + ",";
      json += "\"amp1\":" + String(amplitude[0]) + ",";
      json += "\"amp2\":" + String(amplitude[1]) + ",";
      json += "\"amp3\":" + String(amplitude[2]) + ",";
      json += "\"offset1\":" + String(offset[0]) + ",";
      json += "\"offset2\":" + String(offset[1]) + ",";
      json += "\"offset3\":" + String(offset[2]);
      json += "}";

      request->send(200, "application/json", json);
    });

      ElegantOTA.begin(&server);
      WebSerial.begin(&server);
      WebSerial.onMessage([](uint8_t *data, size_t len) {
        String command = "";
        for (size_t i = 0; i < len; i++) {
          command += (char)data[i];
        }
         // Comandos especiales
        if (command.equalsIgnoreCase("SerialDebugGRBLOn")) {
            SerialDebugSequence = true;
            WebSerial.println("Debug GRBL activado");
            return;
        }
        if (command.equalsIgnoreCase("SerialDebugGRBLOff")) {
            SerialDebugSequence = false;
            WebSerial.println("Debug GRBL desactivado");
            return;
        }
        if (command.equalsIgnoreCase("SerialDebugOn")) {
            SerialDebug = true;
            WebSerial.println("Debug global activado");
            return;
        }
        if (command.equalsIgnoreCase("SerialDebugOff")) {
            SerialDebug = false;
            WebSerial.println("Debug global desactivado");
            return;
        }
        
        WebSerial.println("Enviando a GRBL: " + command);
        GRBLSerial.print(command);
        GRBLSerial.print("\n");
      });


      server.begin();
      Serial.println("Server started");
      connected = true;
    }else {
        Serial.print(".");
    }} 
    }

  return connected;
}

/* Message callback of WebSerial */
void recvMsg(uint8_t *data, size_t len){
  WebSerial.println("Received Data...");
  String d = "";
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }
  WebSerial.println(d);
}


void seqUpdate(){
  if (needsUpdate) {
        loadWave(wave);
        Serial.printf("Period: %.2f\n", period);
        Serial.printf("Amplitudes: %.2f %.2f %.2f\n", wave[0].amplitude,wave[1].amplitude,wave[2].amplitude);
        Serial.printf("Fases: %.2f %.2f %.2f\n",wave[0].phase,wave[1].phase,wave[2].phase);
        Serial.printf("Offsets: %.2f %.2f %.2f\n", wave[0].offset,wave[1].offset,wave[2].offset);
        GRBLSerial.println("!");
        GRBLSerial.println("\x18");
        generateSineSequence(&currentSequence, wave);
        
        sendUnlockAndWaitForOk(GRBLSerial);
        ackReceived = true;
        needsUpdate = false;
        updatedOnce = true;
    }
}


