#include "Sequence.h"
#include <LittleFS.h>
extern  bool  SerialDebugSequence ;
extern  bool SerialDebug;

bool first_time = 0;
uint16_t counter = 0;
void generateSineSequence(Sequence* currentSequence,  SineWave* waves) {

    float frequency = waves[0].frequency;
    float period = 1.0f/frequency;
    Serial.print("Freq: ");
    Serial.print(frequency);
    Serial.print("Period: ");
    Serial.println(period);
    currentSequence->description = "3-Axis Sine Animation";
    currentSequence->fps = 30;
    currentSequence->frames = currentSequence->fps * period;  // 2 segundos, por ejemplo

    while(currentSequence->frames  > MAX_FRAMES- 10){
      currentSequence->fps--;
      currentSequence->frames = currentSequence->fps * period;
    }

    currentSequence->armature = "3AxisRig";
    Serial.print("Frames");
    Serial.print( currentSequence->frames);
    Serial.print("FPS");
    Serial.println( currentSequence->fps);
    for (int i = 0; i < currentSequence->frames; i++) {
        double t = (double)i / currentSequence->fps;

    currentSequence->U[i] = waves[0].offset + waves[0].amplitude * sin(2 * PI * frequency* t + waves[0].phase*PI/180.f);
    currentSequence->V[i] = waves[1].offset + waves[1].amplitude * sin(2 * PI * frequency * t + waves[1].phase*PI/180.f);
    currentSequence->W[i] = waves[2].offset + waves[2].amplitude * sin(2 * PI * frequency * t + waves[2].phase*PI/180.f);

    Serial.print(currentSequence->U[i]); Serial.print(", ");
    Serial.print(currentSequence->V[i]); Serial.print(", ");
    Serial.println(currentSequence->W[i]);
    yield();
  }
  Serial.println("FIN");
}
 
void saveSequenceToJson( Sequence& seq) {
  JsonDocument doc;  // Aumenta si frames > ~300

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

void sendSequenceToGRBL( Sequence& seq, HardwareSerial& grblSerial) {
  //Serial.println("🔁 Enviando secuencia a GRBL...");
 
    // Coordenadas actuales y anteriores
    float x0 = seq.U[counter - 1];
    float y0 = seq.V[counter - 1];
    float z0 = seq.W[counter - 1];

    float x1 = seq.U[counter];
    float y1 = seq.V[counter];
    float z1 = seq.W[counter];

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
    if(first_time == 0){
      feedrate = 100;
      first_time = 1;
    }
   
    snprintf(gcode, sizeof(gcode), "G1 X%.3f Y%.3f Z%.3f F%.1f", x1, y1, z1, feedrate);
    bool sended = sendGcodeWithAck(grblSerial, gcode);
    if(sended){
      counter++;
      if(SerialDebugSequence == true)
        Serial.println(gcode);  // Debug
      if(counter >= seq.frames){
        Serial.println("✅ Secuencia enviada.");
        counter = 0;
      }
    }
}

String ShowOnWeb( Sequence& seq){
 JsonDocument doc;

  doc["frames"] = seq.frames;
  doc["fps"] = seq.fps;

  JsonObject positions = doc.createNestedObject("positions");
  JsonArray u = positions.createNestedArray("U");
  JsonArray v = positions.createNestedArray("V");
  JsonArray w = positions.createNestedArray("W");

  for (int i = 0; i < seq.frames; i++) {
    u.add(seq.U[i]);
    v.add(seq.V[i]);
    w.add(seq.W[i]);
  }

  String output;
  serializeJson(doc, output);
  return output;
}

