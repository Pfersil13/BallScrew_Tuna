#include "File.h"

#include <ArduinoJson.h>
#include <LittleFS.h>





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

