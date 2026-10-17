#include <lcom/lcf.h>

#include "../include/snake_controller.h"
#include "../../state/include/state.h"

int
snake_step(snake_t* snake, board_t* board, action_t action)
{
    switch (action) {
        case UP: 
            if (snake->size == 1 || snake->head_direction != SOUTH)
                snake->head_direction = NORTH;
            break;
        case DOWN: 
            if (snake->size == 1 || snake->head_direction != NORTH)
                snake->head_direction = SOUTH;
            break;
        case LEFT: 
            if (snake->size == 1 || snake->head_direction != EAST)
                snake->head_direction = WEST;
            break;
        case RIGHT: 
            if (snake->size == 1 || snake->head_direction != WEST)
                snake->head_direction = EAST;
            break;
        default: break;
    }
    return snake_move(snake, board);
}

int
snake_move(snake_t* snake, board_t* board)
{
    switch (snake->head_direction) {
        case NORTH: return snake_move_up(snake, board);
        case SOUTH: return snake_move_down(snake, board);
        case EAST: return snake_move_right(snake, board);
        case WEST: return snake_move_left(snake, board);
    }
    return 0;
}

int 
snake_move_left(snake_t* snake, board_t* board)
{
    if (board->fruit->position->x == position_get_left(snake_get_head_position(snake))->x &&
        board->fruit->position->y == position_get_left(snake_get_head_position(snake))->y) {
        snake_increase_size(snake, position_get_left(snake_get_head_position(snake)));
        return 0;
    }

    if (board_is_empty(board, position_get_left(snake_get_head_position(snake))))
        snake_set_position(snake, position_get_left(snake_get_head_position(snake)));
    else {
        set_app_state_menu();
        return 1;
    }
    return 0;
}

int
snake_move_right(snake_t* snake, board_t* board)
{
    if (board->fruit->position->x == position_get_right(snake_get_head_position(snake))->x &&
        board->fruit->position->y == position_get_right(snake_get_head_position(snake))->y) {
        snake_increase_size(snake, position_get_right(snake_get_head_position(snake)));
        return 0;
    }

    if (board_is_empty(board, position_get_right(snake_get_head_position(snake))))
        snake_set_position(snake, position_get_right(snake_get_head_position(snake)));
    else {
        set_app_state_menu();
        return 1;
    }
    return 0;
}

int 
snake_move_up(snake_t* snake, board_t* board)
{
    if (board->fruit->position->x == position_get_up(snake_get_head_position(snake))->x &&
        board->fruit->position->y == position_get_up(snake_get_head_position(snake))->y) {
        snake_increase_size(snake, position_get_up(snake_get_head_position(snake)));
        return 0;
    }

    if (board_is_empty(board, position_get_up(snake_get_head_position(snake))))
        snake_set_position(snake, position_get_up(snake_get_head_position(snake)));
    else {
        set_app_state_menu();
        return 1;
    }
    return 0;
}

int
snake_move_down(snake_t* snake, board_t* board)
{
    if (board->fruit->position->x == position_get_down(snake_get_head_position(snake))->x &&
        board->fruit->position->y == position_get_down(snake_get_head_position(snake))->y) {
        snake_increase_size(snake, position_get_down(snake_get_head_position(snake)));
        return 0;
    }

    if (board_is_empty(board, position_get_down(snake_get_head_position(snake))))
        snake_set_position(snake, position_get_down(snake_get_head_position(snake)));
    else {
        set_app_state_menu();
        return 1;
    }
    return 0;
}
