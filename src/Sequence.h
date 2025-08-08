#ifndef SEQUENCE_H
    #define SEQUENCE_H

    #include <Arduino.h>

    #include "ArduinoJson.h"
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

    #define OFFSET_U 90
    #define OFFSET_V 90
    #define OFFSET_W 90
    void generateSineSequence(Sequence* currentSequence, const SineWave* waves, double frequency);
    void saveSequenceToJson(const Sequence& seq);

    void sendGcodeWithAck(HardwareSerial& grblSerial, const char* gcode);


#endif