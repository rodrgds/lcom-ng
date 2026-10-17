#ifndef ACTIONS_H
#define ACTIONS_H

/**
 * @brief Possible actions of the user: presses an arrow, selects an option, wants to exit or do nothing;
 * 
 */
enum action {UP, DOWN, LEFT, RIGHT, SELECT, EXIT, NOTHING};

typedef enum action action_t;

#endif
