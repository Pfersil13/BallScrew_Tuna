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
    };

    #define FPS 60 //fps
    #define A 1.0              // amplitud
    #define FREQ 1.0           // frecuencia en Hz
    #define PERIOD 1.f/FREQ

    #define OFFSET_U 0
    #define OFFSET_V -3
    #define OFFSET_W 0

    #define MAX_RANGE_U 50 //º
    #define MAX_RANGE_V 55 //nn
    #define MAX_RANGE_W 17 //º

    void generateSineSequence(Sequence* currentSequence, const SineWave* waves, double frequency);
    void saveSequenceToJson(const Sequence& seq);

    void sendSequenceToGRBL(const Sequence& seq, HardwareSerial& grblSerial);


#endif