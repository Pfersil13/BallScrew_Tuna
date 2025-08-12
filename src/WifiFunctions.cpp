#include "WifiFunctions.h"
#include "HTML.h"
#include "secrets.h"
#include "File.h"
// Variables globales para los valores:
extern unsigned long period ;
extern float amplitude[];
extern float offset[] ;
extern float pahse[] ;

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


bool connectWifi(uint32_t timeout_ms) {
  static bool initialized = false;
  static bool connected = false;
  static unsigned long startAttemptTime = 0;

  // Paso 1: Inicializar WiFi una sola vez
  if (!initialized) {
    Serial.println("🔌 Inicializando WiFi...");
    WiFi.setHostname(hostname);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    WiFi.setTxPower(WIFI_POWER_13dBm);
    startAttemptTime = millis();
    initialized = true;
  }

  // Paso 2: Comprobar si ya está conectado
  if (!connected) {
    wl_status_t status = WiFi.status();

    if (status == WL_CONNECTED) {
      Serial.println("\n✅ Conectado a WiFi");
      Serial.print("IP: "); Serial.println(WiFi.localIP());
      Serial.print("Host: "); Serial.println(WiFi.getHostname());

      if (MDNS.begin(WiFi.getHostname())) {
        Serial.println("📡 mDNS iniciado");
      }

      // Configurar servidor y OTA
      server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", PAGE_MAIN);
      });

      server.on("/sequence", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html);
      });

      server.on("/sequence.json", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, "/sequence.json", "application/json");
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
        StaticJsonDocument<256> doc;
        DeserializationError error = deserializeJson(doc, body);
        if (error) {
          Serial.println("Error parseando JSON: " + String(error.c_str()));
          return;
        }

        period = doc["period"];
        amplitude[0] = doc["amp1"];
        amplitude[1] = doc["amp2"];
        amplitude[2] = doc["amp3"];
        offset[0] = doc["offset1"];
        offset[1] = doc["offset2"];
        offset[2] = doc["offset3"];

        Serial.printf("Period: %lu\n", period);
        Serial.printf("Amplitudes: %.2f %.2f %.2f\n", amplitude[0], amplitude[1], amplitude[2]);
        Serial.printf("Offsets: %.2f %.2f %.2f\n", offset[0], offset[1], offset[2]);

        // Limpiar body para próxima vez
        body = "";
      }
    });

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
        WebSerial.println("📡 Enviando a GRBL: " + command);
        GRBLSerial.print(command);
        GRBLSerial.print("\n");
      });

      server.begin();
      initMQTT();

      connected = true;
    } 
    else {
      // Mostrar puntos de progreso cada medio segundo
      static unsigned long lastDotTime = 0;
      if (millis() - lastDotTime > 500) {
        Serial.print(".");
        lastDotTime = millis();
      }

      // Timeout
      if (millis() - startAttemptTime > timeout_ms) {
        //Serial.println("\n⚠️ Tiempo de conexión agotado");
        return false;
      }
    }
  }

  return connected;
}



WiFiClientSecure  espClient;
PubSubClient client(espClient);


// MQTT constants
// broker address, port, and client name
 static const char *root_ca PROGMEM = R"EOF(
    -----BEGIN CERTIFICATE-----
    MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
    TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
    cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
    WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
    ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
    MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
    h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
    0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
    A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
    T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
    B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
    B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
    KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
    OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
    jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
    qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
    rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
    HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
    hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
    ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
    3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
    NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
    ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
    TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
    jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
    oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
    4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
    mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
    emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
    -----END CERTIFICATE-----
    )EOF";


void initMQTT(){
  //espClient.setCACert(root_ca);
  client.setServer(MQTT_BROKER_ADRESS, MQTT_PORT);
  client.setCallback(callback);
  espClient.setInsecure();  // No valida el certificado del servidor 
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


