#include "File.h"

#include <ArduinoJson.h>
#include <LittleFS.h>

#include <Preferences.h>

Preferences prefs;


// Valores por defecto
float period = 16.6; //ms -> 16.66s
float amplitude[3] = {20, 30, 9};
float offset[3] = {0, 4, 0};
float phase[3] = {180,0,180};

void savePreferences() {

  prefs.begin("sculpture", false);
  prefs.putULong("period", period);
  prefs.putFloat("amp0", amplitude[0]);
  prefs.putFloat("amp1", amplitude[1]);
  prefs.putFloat("amp2", amplitude[2]);
  prefs.putFloat("off0", offset[0]);
  prefs.putFloat("off1", offset[1]);
  prefs.putFloat("off2", offset[2]);
  prefs.putFloat("pha0", phase[0]);
  prefs.putFloat("pha1", phase[1]);
  prefs.putFloat("pha2", phase[2]);
  prefs.end();
  Serial.println("Valores guardados en Preferences");
}

void loadPreferences() {
  prefs.begin("sculpture", true);
  period       = prefs.getULong("period", 1000);
  amplitude[0] = prefs.getFloat("amp0", 0);
  amplitude[1] = prefs.getFloat("amp1", 0);
  amplitude[2] = prefs.getFloat("amp2", 0);
  offset[0]    = prefs.getFloat("off0", 0);
  offset[1]    = prefs.getFloat("off1", 0);
  offset[2]    = prefs.getFloat("off2", 0);
  phase[0]    = prefs.getFloat("pha0", 0);
  phase[1]    = prefs.getFloat("pha1", 0);
  phase[2]    = prefs.getFloat("pha2", 0);
  prefs.end();
  Serial.println("Valores cargados desde Preferences");
}


void loadWave(SineWave* wave){
  for (int i = 0; i < 3; i++) {
    wave[i].frequency = 1.0f/period;
    wave[i].amplitude = amplitude[i];
    wave[i].offset = offset[i];
    wave[i].phase = phase[i];
        }
  }