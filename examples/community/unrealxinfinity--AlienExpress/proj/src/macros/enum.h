#ifndef ENUM_H
#define ENUM_H

/**
 * @brief The values of the state of the game
 */
typedef enum{
    MENU,
    LEVEL1,
    PAUSE,
    GAME_OVER
}game_state_t;

/**
 * @brief The values of the action a user can take
 */
typedef enum{
    START,
    EXIT,
    AFK
}action_t;

/**
 * @brief The values of the movement of entities
 */
typedef enum{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    STILL
}movement_t;
#endif
