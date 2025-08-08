#ifndef DEFINITIONS_H
    #define DEFINITIONS_H

    #include <Arduino.h>
    
        /* ──────────── MOTOR PARAMETERS ──────────── */
    #define CFG_MOT_MAX_VEL             "/mot/max_vel"          // Velocidad máxima (mm/s)
    #define CFG_MOT_MAX_ACCEL           "/mot/max_accel"        // Aceleración máxima (mm/s²)
    #define CFG_MOT_VEL                 "/mot/vel"              // Velocidad actual (mm/s)
    #define CFG_MOT_ACCEL               "/mot/accel"            // Aceleración actual (mm/s²)
    #define CFG_MOT_STEPS_PER_MM        "/mot/steps_mm"         // Pasos por mm
    #define CFG_MOT_MICROSTEP           "/mot/microstep"        // Microstepping (1,2,4,8…256)
    #define CFG_MOT_TORQUE_PROFILE      "/mot/torque"           // Perfil de torque (bajo/medio/alto)
    #define CFG_MOT_DIR_INVERT          "/mot/dir_inv"          // Invertir dirección (0/1)
    #define CFG_MOT_HOLD_CURRENT        "/mot/hold_cur"         // Corriente en reposo (mA)
    #define CFG_MOT_OFFSET_ORIGIN       "/mot/origin_offset"    // Offset de origen (mm)
    #define CFG_MOT_POS_LIMIT_MIN       "/mot/pos_min"          // Límite inferior (mm)
    #define CFG_MOT_POS_LIMIT_MAX       "/mot/pos_max"          // Límite superior (mm)
    #define CFG_MOT_CURRENT_MA          "/mot/current_ma"       // Corriente RMS (mA)

    /* ──────────── HOMING PARAMETERS ──────────── */
    #define CFG_HOME_MAX_DIST           "/home/max_dist"        // Distancia máxima de homing (mm)
    #define CFG_HOME_SPEED              "/home/speed"           // Velocidad de homing (mm/s)
    #define CFG_HOME_DIR                "/home/dir"             // Dirección de homing (+1 / -1)
    #define CFG_HOME_MODE               "/home/double_stage"    // (opcional) Doble etapa (0/1)

    /* ──────────── SEQUENCE PARAMETERS ──────────── */
    #define CFG_SEQ_POINTS              "/seq/points"           // Lista de puntos (JSON/TXT)
    #define CFG_SEQ_SIN_PHASE           "/seq/sin_phase"        // Senos desfasados (bool)
    #define CFG_SEQ_FREQ                "/seq/freq"             // Frecuencia o duración total
    #define CFG_SEQ_AMPL_SCALE          "/seq/ampl_scale"       // Escalado amplitud/velocidad
    #define CFG_SEQ_TYPE                "/seq/type"             // Tipo de secuencia (preset)

    /* ──────────── SISTEMA / GENERAL ──────────── */
    #define CFG_SYS_MODE                "/sys/mode"             // Modo (continuo, trigger, etc.)
    #define CFG_SYS_DEBUG_LEVEL         "/sys/debug"            // Verbosity / log level
    #define CFG_SYS_ID                  "/sys/id"               // Identificador de nodo
    #define CFG_SYS_OTA_URL             "/sys/ota_url"          // URL de firmware OTA

    #define CFG_CON_PORT                 "/con/port"               // PORT
    #define CFG_CON_SSID                 "/con/ssid"               // SSID
    #define CFG_CON_PASSWORD            "/con/pass"          //PASSWOrd

#endif