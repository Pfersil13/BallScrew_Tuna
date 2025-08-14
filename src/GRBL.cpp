#include "GRBL.h"
extern  bool  SerialDebugSequence ;
extern  bool SerialDebug;

void sendGRBLConfig(HardwareSerial &grbl) {

  grbl.println("$22=1"); //HOMING OK 515
grbl.println("$10=1"); //report
  grbl.println("$21=0"); //HARD LIMITS

  grbl.println("$24=300"); //HARD LIMITS
  grbl.println("$25=150"); //HARD LIMITS
  grbl.println("$27=2"); //HARD LIMITS

  grbl.println("$100=" + String(GRBL_STEPS_PER_DEG_U));
  delay(10);
  grbl.println("$101=" + String(GRBL_STEPS_PER_DEG_V));
  delay(10);
  grbl.println("$102=" + String(GRBL_STEPS_PER_DEG_W));
  delay(10);
  grbl.println("$110=" + String(GRBL_MAX_VELOCITY_U));
  delay(10);
  grbl.println("$111=" + String(GRBL_MAX_VELOCITY_V));
  delay(10);
  grbl.println("$112=" + String(GRBL_MAX_VELOCITY_W));
  delay(10);
  grbl.println("$0=" + String(GRBL_MICROSTEPPING));
  delay(10);

  // Aceleraciones
  grbl.println("$120=" + String(GRBL_ACCEL_U));
  delay(10);
  grbl.println("$121=" + String(GRBL_ACCEL_V));
  delay(10);
  grbl.println("$122=" + String(GRBL_ACCEL_W));
  delay(10);
  // Límites de recorrido
  grbl.println("$130=" + String(MAX_RANGE_U));
  delay(10);
  grbl.println("$131=" + String(MAX_RANGE_V));
  delay(10);
  grbl.println("$132=" + String(MAX_RANGE_W));

  grbl.println("G90 G21"); //mm and absolute

grbl.println("G54 "); //mm and absolute
grbl.println("$3 = 2"); //mm and absolute
  delay(10);
 
}


void doHomingAndWait(HardwareSerial &grbl) {
    bool msgSeen = false;
    bool done = false;

    grbl.println("$H"); // Inicia homing

    unsigned long startTime = millis();

    while (!done) {
        if (grbl.available()) {
            String line = grbl.readStringUntil('\n');
            line.trim();

            Serial.println("GRBL: " + line); // Debug

            if (line.startsWith("[MSG:]")) {
                msgSeen = true; // Ya vimos el mensaje de final
            } 
            else if (msgSeen && line == "ok") {
                done = true; // Este es el ok final
            }
        }

        // Seguridad: por si acaso se cuelga
        if (millis() - startTime > 120000) { 
            Serial.println("Timeout esperando homing");
            break;
        }
    }

    Serial.println("Homing COMPLETADO.");
}


void sendUnlockAndWaitForOk(HardwareSerial &grbl) {
    String responseLine = "";
    unsigned long timeout = 100;   // Timeout máximo 10s para evitar bloqueo infinito
    unsigned long startTime = millis();

    while (true) {
        // Enviar comando $X
        grbl.println("$X");
        Serial.println("Enviando $X para desbloquear alarma...");

        responseLine = "";

        // Esperar respuesta hasta timeout
        while (millis() - startTime < timeout) {
            if (grbl.available()) {
                char c = grbl.read();
                Serial.write(c);

                if (c == '\n') {
                    responseLine.trim();
                    responseLine.replace("\r", "");

                    Serial.print("Respuesta GRBL: [");
                    Serial.print(responseLine);
                    Serial.println("]");

                    if (responseLine == "ok") {
                        Serial.println("Alarma desbloqueada correctamente.");
                        return;  // Salir de la función cuando recibe ok
                    }
                    responseLine = "";
                } else {
                    responseLine += c;
                }
            }
        }

        Serial.println("Timeout esperando respuesta 'ok', reenviando $X...");
        startTime = millis();  // Reiniciar timer para reenviar $X
    }
}


// Variables globales
static String grblLine = "";
bool ackReceived = true;
bool errorReceived = false;
char gcode[32];

void readGrbl(HardwareSerial &grbl) {
  while (grbl.available()) {
    char c = grbl.read();
    if(SerialDebug == true)
    Serial.write(c);
    WebSerial.write(c);

    if (c == '\n') {
      grblLine.trim();

      // Detectar ACK
      if (grblLine.startsWith("ok")) {
        ackReceived = true;
      }
      else if (grblLine.startsWith("error")) {
        errorReceived = true;
       
      }
      // Detectar alarmas
      else if (grblLine.indexOf("ALARM_X_MAX") != -1) {
        Serial.println("OKKK");
        sendUnlockAndWaitForOk(grbl);
        snprintf(gcode, sizeof(gcode), "G92 X%.3f", GRBL_MAX_POS_U);
        grbl.println(gcode);
        grbl.println("G91");
        grbl.println("G1 X-3 F200");
        grbl.println("G90");;
      }
      else if (grblLine.indexOf("ALARM_X_MIN") != -1) {
        Serial.println("OKKK");
        sendUnlockAndWaitForOk(grbl);
        snprintf(gcode, sizeof(gcode), "G92 X%.3f", GRBL_MIN_POS_U);
        grbl.println(gcode);
        grbl.println("G91");
        grbl.println("G1 X3 F200");
        grbl.println("G90");
      }
      else if (grblLine.indexOf("ALARM_Y_MAX") != -1) {
        sendUnlockAndWaitForOk(grbl);
        snprintf(gcode, sizeof(gcode), "G95 Y%.3f", GRBL_MAX_POS_V);
        grbl.println(gcode);
        grbl.println("G91");
        grbl.println("G1 Y-3 F200");
        grbl.println("G90");
      }
      else if (grblLine.indexOf("ALARM_Y_MIN") != -1) {
        sendUnlockAndWaitForOk(grbl);
        snprintf(gcode, sizeof(gcode), "G92 Y%.3f", GRBL_MIN_POS_V);
        grbl.println(gcode);
        grbl.println("G91");
        grbl.println("G1 Y3 F200");
        grbl.println("G90");
      }
      else if (grblLine.indexOf("ALARM_Z_MAX") != -1) {
        sendUnlockAndWaitForOk(grbl);
        snprintf(gcode, sizeof(gcode), "G92 Z%.3f", GRBL_MAX_POS_W);
        grbl.println(gcode);
        grbl.println("G91");
        grbl.println("G1 Z3 F200");
        grbl.println("G90");
      }
      else if (grblLine.indexOf("ALARM_Z_MIN") != -1) {
        sendUnlockAndWaitForOk(grbl);
        snprintf(gcode, sizeof(gcode), "G92 Z%.3f", GRBL_MIN_POS_W);
        grbl.println(gcode);
        grbl.println("G91");
        grbl.println("G1 Z-3 F200");
        grbl.println("G90");
      }

      grblLine = ""; // Reset línea
    }
    else {
      grblLine += c;
    }
  }
}



bool sendGcodeWithAck(HardwareSerial& grblSerial, const char* gcode) {
   if (ackReceived == false) {
    // Todavía estamos esperando respuesta de un comando previo
    return false;
  }

    ackReceived = false;
    errorReceived = false;

    grblSerial.println(gcode);
    if(SerialDebugSequence == true)
    Serial.println("Sent: " + String(gcode));
    return true; // Se envió con éxito
}

 