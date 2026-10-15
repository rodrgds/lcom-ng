/**
 * @file controllerMouse.h
 * @brief Contains all state machines functions and other functions to monitor the mouse's movement
 * @date 2024-06-02
 * 
 * 
 */

#include <stdlib.h>
#include <stdbool.h>
#include <lcom/lcf.h> //have to include this so struct packet is known



extern struct packet mouse_packet;
extern uint8_t tolerance;
/**
 * @brief Mouse state enum
 * 
 */
typedef enum {
  INIT,
  DUP,
  DDOWN,
  VERTEX,
  DSIDE,
  FAIL,
  SUCCESS
} mouseState;



/**
 * @brief Checks if mouse is moving with negative displacements in the mouse packets, allowing a certain tolerance
 * 
 * @param tolerance chosen tolerance
 * @return true if it is moving, false if not
 */
bool mouse_is_descending(int16_t tolerance);
/**
 * @brief Checks if mouse is moving with positive displacements in the mouse packets, allowing a certain tolerance
 * 
 * @param tolerance chosen tolerance
 * @return true if it is moving, false if not
 */
bool mouse_is_ascending(uint8_t tolerance);
/**
 * @brief When called, a state machine is initiated and verifies if the movement made with the mouse constitutes an inverted 'V' shape, the user can choose a movement tolerance and a minimum length for each of the two diagonal lines in the inverted 'V'
 * 
 * @param tolerance chosen tolerance
 * @param x_len minimum line length
 */
void stateMachineInvertedV(int tolerance, int x_len);
/**
 * @brief When called, a state machine is initiated and verifies if the movement made with the mouse constitutes a 'V' shape, the user can choose a movement tolerance and a minimum length for each of the two diagonal lines in the 'V'
 * 
 * @param tolerance chosen tolerance
 * @param x_len minimum line length
 */
void stateMachineV(int tolerance, int x_len);
/**
 * @brief When called, a state machine is initiated and verifies if the movement made with the mouse constitutes a vertical line, the user can choose a movement tolerance and a minimum length for the line, there is no difference between a up->down movement or a down->up movement
 * 
 * @param tolerance chosen tolerance
 * @param x_len minimum line length
 */
void stateMachineVLine(int tolerance, int y_len);
/**
 * @brief When called, a state machine is initiated and verifies if the movement made with the mouse constitutes a horizontal line, the user can choose a movement tolerance and a minimum length for the line, there is no difference between a left->right movement or a right->left movement
 * 
 * @param tolerance chosen tolerance
 * @param x_len minimum line length
 */
void stateMachineHLine(int tolerance, int y_len);
/**
 * @brief When called, a state machine is initiated and verifies if the movement made with the mouse constitutes a diagonal line with a slope of (approximately) one, the user can choose a movement tolerance and a minimum length for the line, there is no difference between a left->right movement or a right->left movement
 * 
 * @param tolerance chosen tolerance
 * @param x_len minimum line length
 */
void stateMachineDLine(int tolerance, int y_len);
/**
 * @brief Adds the packet's delta_y and delta_x displacements upon an interrupt, checks if the mouse's x and y positions are out of bounds, if so it doesn't let it pass. Also checks for delta_x and delta_y overflow.
 * 
 * @param mouse_x_pos mouse's x axis position
 * @param mouse_y_pos mouse's y axis position
 */
void moveMouse(int *mouse_x_pos, int *mouse_y_pos);



