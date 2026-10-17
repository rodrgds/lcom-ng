#pragma once
#include "bitmap.h"

typedef enum {
        MAIN_MENU_STATE, GAME_STATE
}State;
typedef struct {
        int IRQ_SET_KBD;
        int IRQ_SET_TIMER;
        int IRQ_SET_MOUSE;

        int done, draw;

        unsigned long scancode;

        int notFirst;

        Bitmap* cursor;
        State currentState;
        void* state;
} ObstacleCourse;

/**
 * @brief Initializes game
 * @return The struct with the game state
 */
ObstacleCourse* startObstacleCourse();

/**
 * @brief Updates whole game
 * @param course Game
 */
void updateObstacleCourse(ObstacleCourse* course);

/**
 * @brief Allows for choosing which scenario to draw like the menu or the race according to the state
 * @param course Game
 */
void drawObstacleCourse(ObstacleCourse* course);

/**
 * @brief Unsubscribes all peripherics and frees all resources used in the game
 * @param course game
 */
void stopObstacleCourse(ObstacleCourse* course);
