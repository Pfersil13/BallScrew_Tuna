#include "WifiFunctions.h"
#include "HTML.h"
#include "secrets.h"



extern Sequence currentSequence;

unsigned long currentMillisOTA;
unsigned long LastMillis;

uint32_t wifi_up_time = 350000;
bool boot = 0;
bool conected = 0;
AsyncWebServer server(80);

static bool initialized = false;
static bool connected = false;

extern HardwareSerial GRBLSerial; // UART1




bool connectWifi(uint32_t timeout_ms)
{
  static bool initialized = false;
  static bool connected = false;

  if (!initialized) {
    Serial.println("🔌 Inicializando WiFi...");
    WiFi.setHostname(hostname);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    WiFi.setTxPower(WIFI_POWER_13dBm);
    initialized = true;
  }

  if (!connected) {
    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && (millis() - start) < timeout_ms) {
      Serial.print(".");
      delay(500);
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\n Conectado a WiFi");
      Serial.print("IP: "); Serial.println(WiFi.localIP());
      Serial.print("Host: "); Serial.println(WiFi.getHostname());

      if (MDNS.begin(WiFi.getHostname())) {
        Serial.println(" mDNS iniciado");
      }


            server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", PAGE_MAIN);
      });

      server.on("/sequence", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html);
      });

      server.on("/sequence.json", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(LittleFS, "/sequence.json", "application/json");
    });

    /*server.on("/UPDATE_SLIDER", HTTP_GET, [] (AsyncWebServerRequest *request){
      String inputMessage;
        // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
        if (request->hasParam(PARAM_INPUT)) {
          inputMessage = request->getParam(PARAM_INPUT)->value();
        }
        else {
          inputMessage = "No message sent";
        }
        Serial.println(inputMessage);
        request->send(200, "text/plain", "OK");
    });

    server.on("/upload", HTTP_POST, [] (AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", PAGE_MAIN);
          }, handleUpload);
*/
      ElegantOTA.begin(&server);
      WebSerial.begin(&server);
      //WebSerial.println("📡 OTA y WebSerial listos");
      /* Attach Message Callback */
      WebSerial.onMessage([](uint8_t *data, size_t len) {
        String command = "";

        for (size_t i = 0; i < len; i++) {
          command += (char)data[i];
        }

  WebSerial.println("📡 Enviando a GRBL: " + command);
  GRBLSerial.print(command);      // manda el comando tal cual
  GRBLSerial.print("\n");         // fuerza el salto de línea
});
      server.begin();

      initMQTT();
      connected = true;
    } else {
      Serial.println("\n Fallo al conectar WiFi");
      Serial.print("Estado: "); Serial.println(WiFi.status());
      return false;
    }
  }

  return true;
}




WiFiClientSecure  espClient;
PubSubClient client(espClient);


// MQTT constants
// broker address, port, and client name


void initMQTT(){
  espClient.setCACert(root_ca);
  client.setServer(MQTT_BROKER_ADRESS, MQTT_PORT);
  client.setCallback(callback);
  //espClient.setInsecure();  // No valida el certificado del servidor 
    // Suscripciones
  client.subscribe(CFG_MOT_MAX_VEL);
  client.subscribe(CFG_MOT_MAX_ACCEL);
  client.subscribe(CFG_MOT_VEL);
  client.subscribe(CFG_MOT_ACCEL);
  client.subscribe(CFG_MOT_STEPS_PER_MM);
  client.subscribe(CFG_MOT_MICROSTEP);
  client.subscribe(CFG_MOT_TORQUE_PROFILE);
  client.subscribe(CFG_MOT_DIR_INVERT);
  client.subscribe(CFG_MOT_HOLD_CURRENT);
  client.subscribe(CFG_MOT_OFFSET_ORIGIN);
  client.subscribe(CFG_MOT_POS_LIMIT_MIN);
  client.subscribe(CFG_MOT_POS_LIMIT_MAX);
  client.subscribe(CFG_MOT_CURRENT_MA);

  client.subscribe(CFG_HOME_MAX_DIST);
  client.subscribe(CFG_HOME_SPEED);
  client.subscribe(CFG_HOME_DIR);
  client.subscribe(CFG_HOME_MODE);

  client.subscribe(CFG_SEQ_POINTS);
  client.subscribe(CFG_SEQ_SIN_PHASE);
  client.subscribe(CFG_SEQ_FREQ);
  client.subscribe(CFG_SEQ_AMPL_SCALE);
  client.subscribe(CFG_SEQ_TYPE);

  client.subscribe(CFG_SYS_MODE);
  client.subscribe(CFG_SYS_DEBUG_LEVEL);
  client.subscribe(CFG_SYS_ID);
  client.subscribe(CFG_SYS_OTA_URL);

  client.subscribe(CFG_CON_PORT);
  client.subscribe(CFG_CON_SSID);
  client.subscribe(CFG_CON_PASSWORD);
}

void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0'; // Asegúrate de terminar la cadena
  String msg = String((char*)payload);

  Serial.print("📨 Topic recibido: ");
  Serial.print(topic);
  Serial.print(" → ");
  Serial.println(msg);

  if (strcmp(topic, CFG_MOT_MAX_VEL) == 0) {
    float maxVel = msg.toFloat();
    // mot.setMaxVel(maxVel);  // ejemplo
  } 
  else if (strcmp(topic, CFG_SEQ_FREQ) == 0) {
    float freq = msg.toFloat();
    // setSequenceFrequency(freq);
  }
  else if (strcmp(topic, CFG_SEQ_SIN_PHASE) == 0) {
    //setSequenceSin(freq);
  }else if (String(topic) == CFG_SEQ_POINTS) {
    // Cargar JSON con posiciones personalizadas
    DynamicJsonDocument doc(4096);
    DeserializationError error = deserializeJson(doc, msg);
    if (!error) {
      currentSequence.fps = doc["fps"];
      currentSequence.frames = doc["frames"];
      currentSequence.description = doc["description"].as<String>();
      currentSequence.armature = doc["armature"].as<String>();

      JsonArray u = doc["positions"]["U"];
      JsonArray v = doc["positions"]["V"];
      JsonArray w = doc["positions"]["W"];

      for (int i = 0; i < currentSequence.frames; i++) {
        currentSequence.U[i] = u[i];
        currentSequence.V[i] = v[i];
        currentSequence.W[i] = w[i];
      }
    }
  } else if (String(topic) == CFG_SEQ_FREQ) {
    //FREQ = msg.toFloat();
    //generateSinSequence(A, FREQ, 0, 2 * PI / 3, 4 * PI / 3);
  }
  else {
    Serial.println("⚠️ Topic no manejado");
  }
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32", mqtt_username, mqtt_password)) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void MQTTloop(){
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void Pub(){
  client.publish("/Hola", "value");
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


