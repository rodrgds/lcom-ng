#ifndef __CONFIG_H
#define __CONFIG_H

/* Graphics VBE mode and related constants */
#define SCREEN_RES_MODE                         0x115
#define BYTES_PER_PIXEL                         4
#define X_SCEEEN_RES                            800
#define Y_SCREEN_RES                            600

/* Colors */
#define TRANSPARENT_COLOR                       0xFFFFFE

/* Game configurations */
#define ACCEPTANCE_Y_TOP                        60
#define ACCEPTANCE_Y_BOT                        108 

/* Lane geometry*/
#define NOTE_SPRITE_SIZE                        16
#define NOTE_SCALE                              3
#define TARGET_SIZE                             48 
#define NOTE_SIZE                               48
#define NUM_LANES                               4
#define GAME_DURATION_SECS                      60
#define NOTE_SPAWN_MAX_DELAY                    120
#define LANE_START_X                            150
#define LANE_SPACING                            70

/* Game speed */
#define BASE_SPEED                              2.0
#define SPEED_INCREMENT                         0.1

/*  Timing */
#define GAME_DURATION_SECS 60
#define SPAWN_STAGGER_MAX 120
#define CHEERLEADER_POSE_DURATION_TICKS         25
#define NOTE_SPEED_INCREASE_INTERVAL_S          2

/* Auxiliary macros */
#define HEART_SPAWN_PROB_PERCENTAGE             15
#define HEART_MIN_GAP                           5
#define HEART_CHEER_POSE                        NUM_LANES

#endif /* __CONFIG_H */
