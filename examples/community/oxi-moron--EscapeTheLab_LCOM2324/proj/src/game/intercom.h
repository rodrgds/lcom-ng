#ifndef G6_INTERCOM_H
#define G6_INTERCOM_H

#include <lcom/lcf.h>

#include "../drivers/serial_port.h"
#include "../drivers/uart_macros.h"
#include "game_macros.h"


static uint8_t word[PASSWORD_SIZE] = {0, 0, 0, 0};
static uint8_t password[PASSWORD_SIZE] = {L_SCANCODE, C_SCANCODE, O_SCANCODE, M_SCANCODE};
static int current_index = 0;

/**
 * @brief Sends (and receives) a letter to/from the other player via the in-game intercom.
 * @param player_no Number of the current player.
 * @param scancode Scancode of the letter to be sent.
 * @return 0 if successful, non-zero otherwise.
 */
int (intercom_send_letter) (uint8_t player_no, uint8_t scancode);

/**
 * @brief Gets the letters received through the intercom.
 * @param letters Address where the letters will be stored.
 */
void (intercom_get_letters) (uint8_t* letters);

/**
 * @brief Gets the secret password to finish the game.
 * @param pw Address where the password will be stored.
 */
void (intercom_get_password) (uint8_t* pw);

/**
 * @brief Checks if the player has received the secret password via the intercom.
 * @return True if the password has been received, false otherwise.
 */
bool intercom_password_entered();

/**
 * @brief Resets the letters received through the intercom.
 */
void (intercom_reset) ();

#endif //G6_INTERCOM_H
