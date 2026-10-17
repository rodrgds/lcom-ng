#ifndef CONTEXT_H_
#define CONTEXT_H_

#define MAX_SPRITES 21
#define MAX_NUMBERS 10


#define SPRITE_MOUSE_Idx 0
#define SPRITE_PLAYbtn_Idx 1
#define SPRITE_QUITbtn_Idx 2
#define SPRITE_INSTbtn_Idx 3
#define SPRITE_MULTIbtn_Idx 4
#define SPRITE_BACKbtn_Idx 5
#define SPRITE_MAINMENUbtn_Idx 6
#define SPRITE_INITscreen_Idx 7
#define SPRITE_INSTscreen_Idx 8
#define SPRITE_GAMEscreen_Idx 9
#define SPRITE_WINscree_Idx 10
#define SPRITE_LOSEscree_Idx 11
#define SPRITE_BOY_Idx 12
#define SPRITE_GIRL_Idx 13
#define SPRITE_WALL_Idx 14
#define SPRITE_DOOR_Idx 15
#define SPRITE_BARRIER_Idx 16
#define SPRITE_LEVERleft_Idx 17
#define SPRITE_LEVERright_Idx 18
#define SPRITE_FIRE_Idx 19
#define SPRITE_ICE_Idx 20

#define SPRITE_NUMBERS_0_Idx 0
#define SPRITE_NUMBERS_1_Idx 1
#define SPRITE_NUMBERS_2_Idx 2
#define SPRITE_NUMBERS_3_Idx 3
#define SPRITE_NUMBERS_4_Idx 4
#define SPRITE_NUMBERS_5_Idx 5
#define SPRITE_NUMBERS_6_Idx 6
#define SPRITE_NUMBERS_7_Idx 7
#define SPRITE_NUMBERS_8_Idx 8
#define SPRITE_NUMBERS_9_Idx 9



///@brief Enumeration representing the status (ON or OFF) of a component or feature.
typedef enum {
    ON,
    OFF,
} Status;

///@brief Enumeration representing the different game states.
typedef enum {
    START_MENU,
    GAMEWIN_MENU,
    GAMEOVER_MENU,
    INSTRUCTIONS_MENU,
    PLAYING,
} GameState;

///@brief Structure representing a sprite in the game.
struct sprite {
  uint16_t xpos, ypos;
  uint16_t height, width;
  uint32_t *xpm;
  uint8_t is_pressed; 
};

///@brief Structure representing a position in the game.
struct position {
  uint16_t x,y;
};

///@brief Structure representing a lever in the game.
struct lever {
  uint16_t x,y;
  uint8_t is_pressed;
};

///@brief Structure representing a barrier in the game.
struct barrier {
  uint16_t x,y;
  uint8_t is_open;
};

///@brief Structure representing a trap in the game.
struct trap {
  uint16_t x,y;
  uint8_t type;
};

typedef struct sprite sprite_t;
typedef struct watergirl watergirl_t;
typedef struct fireboy fireboy_t;
typedef struct position position_t;

typedef struct lever lever_t;
typedef struct barrier barrier_t;
typedef struct trap trap_t;

///@brief Structure representing the game context.
struct gamecontext {
    int numWalls;
    int numTraps;

    int levers_countdown;
    int startLeversCountdown;

    int map_countdown;
    int startMapCountdown;

    Status gamestatus;
    GameState gamestate;
    position_t* walls;
    lever_t* levers;
    lever_t* levers_match;
    barrier_t* barriers;
    trap_t* traps;
    sprite_t sprites[MAX_SPRITES];
    sprite_t numbers[MAX_NUMBERS];
};

struct gamecontext context;

#endif
