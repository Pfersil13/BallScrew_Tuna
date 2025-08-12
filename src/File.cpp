#include "File.h"

#include <ArduinoJson.h>
#include <LittleFS.h>

#include <Preferences.h>

Preferences prefs;


// Valores por defecto
unsigned long period = 16666; //ms -> 16.66s
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
  Serial.println("💾 Valores guardados en Preferences");
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
  Serial.println("📂 Valores cargados desde Preferences");
}



bool fileCopy(const char * path_to_copy,const  char * destination_path){
  File file_to_copy;
  File detination_file;

    file_to_copy = LittleFS.open(path_to_copy , "r");
    if(!file_to_copy){
      Serial.println("Failed to open file for reading");
      return 0;
    }
    JsonDocument  doc;
    DeserializationError error = deserializeJson(doc, file_to_copy);
    
    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return 0;
    }else{
      file_to_copy.close();      
      file_to_copy = LittleFS.open(path_to_copy , "r");
      if(!file_to_copy){
        Serial.println("Failed to open file for writing");
        return 0;
      }
      detination_file = LittleFS.open(destination_path , "w");
      if(!detination_file){
        Serial.println("Failed to open file for reading");
        return 0;
      }
      while(file_to_copy.available()){
      detination_file.write(file_to_copy.read());
    }
    detination_file.close();
    file_to_copy.close();
    return 1;
    }
}

bool fileRead( const char * path){
  File file = LittleFS.open(path , "r");
  if(!file){
    Serial.println("Failed to open file for reading");
    return 0;
  }

    Serial.println("Reading_Stored");

    while(file.available()){
      Serial.write(file.read());
      yield();
    }

    file.close();
    return 1;

  }

