#include "Sequence.h"
#include <LittleFS.h>


void generateSineSequence(Sequence* currentSequence, const SineWave* waves, double frequency) {

    float period = 1.0f/frequency;
    
    currentSequence->description = "3-Axis Sine Animation";
    currentSequence->fps = 60;
    currentSequence->frames = currentSequence->fps * period;  // 2 segundos, por ejemplo
    currentSequence->armature = "3AxisRig";



    for (int i = 0; i < currentSequence->frames; i++) {
        double t = (double)i / currentSequence->fps;

    currentSequence->U[i] = OFFSET_U + waves[0].amplitude * sin(2 * PI * frequency* t + waves[0].phase);
    currentSequence->V[i] = OFFSET_V + waves[1].amplitude * sin(2 * PI * frequency * t + waves[1].phase);
    currentSequence->W[i] = OFFSET_W + waves[2].amplitude * sin(2 * PI * frequency * t + waves[2].phase);

    Serial.print(currentSequence->U[i]); Serial.print(", ");
    Serial.print(currentSequence->V[i]); Serial.print(", ");
    Serial.println(currentSequence->W[i]);
  }
}
 
void saveSequenceToJson(const Sequence& seq) {
  StaticJsonDocument<4096> doc;  // Aumenta si frames > ~300

  doc["description"] = seq.description;
  doc["fps"] = seq.fps;
  doc["frames"] = seq.frames;
  doc["armature"] = seq.armature;

  JsonArray u = doc["positions"].createNestedArray("U");
  JsonArray v = doc["positions"].createNestedArray("V");
  JsonArray w = doc["positions"].createNestedArray("W");

  for (int i = 0; i < seq.frames; i++) {
    u.add(seq.U[i]);
    v.add(seq.V[i]);
    w.add(seq.W[i]);
  }

  File file = LittleFS.open("/sequence.json", "w");
  if (!file) {
    Serial.println("Error al abrir /sequence.json");
    return;
  }

  serializeJson(doc, file);
  file.close();
  Serial.println("Secuencia guardada en /sequence.json");
}

void sendSequenceToGRBL(const Sequence& seq, HardwareSerial& grblSerial) {
  Serial.println("🔁 Enviando secuencia a GRBL...");

  for (int i = 1; i < seq.frames; i++) {
    // Coordenadas actuales y anteriores
    float x0 = seq.U[i - 1];
    float y0 = seq.V[i - 1];
    float z0 = seq.W[i - 1];

    float x1 = seq.U[i];
    float y1 = seq.V[i];
    float z1 = seq.W[i];

    // Distancia entre puntos
    float dx = x1 - x0;
    float dy = y1 - y0;
    float dz = z1 - z0;
    float distance = sqrt(dx * dx + dy * dy + dz * dz);

    // Tiempo por frame
    float timePerFrame = 1.0f / seq.fps;

    // Velocidad en mm/s
    float speed = distance / timePerFrame;

    // Feedrate en mm/min
    float feedrate = speed * 60.0f;

    // Generar y enviar comando
    char gcode[64];
    snprintf(gcode, sizeof(gcode), "G1 X%.3f Y%.3f Z%.3f F%.1f", x1, y1, z1, feedrate);
    sendGcodeWithAck(grblSerial, gcode);

    Serial.println(gcode);  // Debug
  }

  Serial.println("✅ Secuencia enviada.");
}


void sendGcodeWithAck(HardwareSerial& grblSerial, const char* gcode) {
  grblSerial.println(gcode);
  Serial.println("Sent: " + String(gcode));

  unsigned long start = millis();
  while (millis() - start < 1000) { // Espera hasta 1 segundo
    if (grblSerial.available()) {
      String response = grblSerial.readStringUntil('\n');
      Serial.println("GRBL: " + response);
      if (response.startsWith("ok") || response.startsWith("error")) {
        break; // respuesta recibida, pasamos al siguiente comando
      }
    }
  }
}
