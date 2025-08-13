#ifndef FILE_H
    #define FILE_H

        #include "Arduino.h"
        #include <string.h>
        #include "LittleFS.h"
        #include "ArduinoJson.h"
        #include "Definitions.h"
        #include "Sequence.h"
       /* typedef struct  {
        int m1;
        int m2;
        float t;
        }POINT;

        #define NUM_SEQUENCES 5
        #define MAX_POINTS_PER_SEQUENCE 50  // Por seguridad, límite superior

        POINT *sequences[NUM_SEQUENCES]; // Punteros a arrays dinámicos
        int sequenceLengths[NUM_SEQUENCES];  // Longitudes reales de cada secuencia
        */

        void loadPreferences();
        void savePreferences();
        void loadWave( SineWave* wave);


#endif