#ifndef _LCOM_EV_LISTENER_H_
#define _LCOM_EV_LISTENER_H_

#include "../drivers/drivers.h"
#include "../drawer/drawer.h"
#include "../model/sprite/sprite.h"
#include "../model/game/game.h"
#include "../model/app/app.h"
#include "../model/animation/animation.h"
#include "../model/com_manager/com_manager.h"
#include <math.h>

#include "../assets/cards/rotate1.xpm"
#include "../assets/cards/rotate2.xpm"

/**
 * @defgroup ev_listener Event Listener
 * @{
 * 
 * @brief Event listeners and handlers for the application.
 */

/**
 * @brief Type definition for a handler function.
 * @details A handler function processes an event interrupt for the application.
 * @param app Pointer to the application structure.
 * @param interrupt The type of interrupt that occurred.
 */
typedef void(*handler)(app_t *app, interrupt_type_t interrupt);

/**
 * @brief Handles general interrupts for the application. 
 * @param app Pointer to the application structure.
 * @param interrupt The type of interrupt that occurred.
 */
void handle_interrupt(app_t *app, interrupt_type_t interrupt);

/**
 * @brief General interrupt handler.
 * @param app Pointer to the application structure.
 * @param interrupt The type of interrupt that occurred.
 */
void handle_general(app_t *app, interrupt_type_t interrupt);

/**
 * @brief Handles interrupts in the main menu state.
 * @param app Pointer to the application structure.
 * @param interrupt The type of interrupt that occurred.
 */
void handle_main_menu(app_t *app, interrupt_type_t interrupt);

/**
 * @brief Handles interrupts in the game playing state.
 * @param app Pointer to the application structure.
 * @param interrupt The type of interrupt that occurred.
 */
void handle_game_playing(app_t *app, interrupt_type_t interrupt);

/**
 * @brief Handles a hit event triggered by hit button during gameplay.
 * @param ptr Pointer to the context or data related to the hit event.
 */
void handle_hit(void *ptr);

/**
 * @brief Handles interrupts in the game betting state.
 * @param app Pointer to the application structure.
 * @param interrupt The type of interrupt that occurred.
 */
void handle_game_betting(app_t *app, interrupt_type_t interrupt);

/**
 * @brief Handles interrupts in the game over state.
 * @param app Pointer to the application structure.
 * @param interrupt The type of interrupt that occurred.
 */
void handle_game_over(app_t *app, interrupt_type_t interrupt);

/**
 * @brief Handles bet value input.
 * @param app Pointer to the application structure.
 * @param interrupt The type of interrupt that occurred.
 */
void handle_bet_value(app_t *app, interrupt_type_t interrupt);

/**
 * @brief Checks and handles the bet value.
 * @param app Pointer to the application structure.
 */
void handle_bet_value_check(app_t *app);

/**
 * @brief Handles interrupts during the dealer's turn.
 * @param app Pointer to the application structure.
 * @param interrupt The type of interrupt that occurred.
 */
void handle_dealer_turn(app_t *app, interrupt_type_t interrupt);

/**
 * @brief Adds a single animation for the dealer's turning up first card.
 * @param app Pointer to the application structure.
 */
void add_dealer_single_animation(app_t *app);

/** @} */

#endif /* _LCOM_EV_LISTENER_H_ */
