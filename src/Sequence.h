#ifndef SEQUENCE_H
    #define SEQUENCE_H

    #include <Arduino.h>

    #include "ArduinoJson.h"
    #include "GRBL.h"
    
    #define MAX_FRAMES 1000  // Ajustable según memoria

    struct Sequence {
        String description;
        int fps;
        int frames;
        String armature;
        float U[MAX_FRAMES];
        float V[MAX_FRAMES];
        float W[MAX_FRAMES];
    };

    struct SineWave {
        double frequency;
        double amplitude;
        double phase; // en radianes
        double offset; // 
    };
  

    void generateSineSequence(Sequence* currentSequence,  SineWave* waves);
    void saveSequenceToJson( Sequence& seq);

    void sendSequenceToGRBL( Sequence& seq, HardwareSerial& grblSerial);
    String ShowOnWeb( Sequence& seq);


#endif