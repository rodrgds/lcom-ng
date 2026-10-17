#ifndef _SNAKE_STATE_H
#define _SNAKE_STATE_H

enum snake_state{NEW, ALIVE, DEAD, INNACTIVE};

typedef enum snake_state snake_state_t;

typedef struct{
    snake_state_t p1_state;
    snake_state_t p2_state;
}player_state_t;

static snake_state_t snake1_state = DEAD;
static snake_state_t snake2_state = DEAD;

/**
 * @brief Returns the state of both player's snakes
 * 
 * @return player_state_t the state of both snakes
 */
player_state_t get_player_state();

/**
 * @brief Sets the desired snake state to NEW
 * 
 * @param player player 1 or player 2
 * @return int 1 if the argument is invalid, 2 if the state is already set,
 * 0 upon success
 */
int set_snake_state_new(snake_state_t snake);

/**
 * @brief Sets the desired snake state to MOVE
 * 
 * @param player player 1 or player 2
 * @return int 1 if the argument is invalid, 2 if the state is already set,
 * 0 upon success
 */
int set_snake__state_alive(snake_state_t snake);

/**
 * @brief Sets the desired snake state to DEAD
 * 
 * @param player player 1 or player 2
 * @return int 1 if the argument is invalid, 2 if the state is already set,
 * 0 upon success
 */
int set_snake_state_dead(snake_state_t snake);

#endif
