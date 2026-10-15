#ifndef _LCOM_APP_H_
#define _LCOM_APP_H_

#include <lcom/lcf.h>
#include "../sprite/sprite.h"
#include "../animation/animation.h"
#include "../cursor/cursor.h"
#include "../../drivers/drivers.h"
#include "../game/game.h"
#include "../font/font.h"
#include "../banner/banner.h"

#include "../../assets/background.xpm"
#include "../../assets/buttons/buttons.h"

/** @defgroup models Models */

/**
 * @defgroup app App
 * @ingroup models
 * @{
 * 
 * @brief Application module for initializing and managing the application state.
 */

/** 
 * @defgroup kb_scancodes Keyboard Scancodes
 * @{
 */
#define KB_ESC 0x81
#define KB_ENTER 0x9c
#define KB_BACKSPC 0x0e
#define KB_X 0xad
#define KB_Y 0x95
#define KB_C 0xae

#define KB_0 0x8b
#define KB_1 0x82
#define KB_2 0x83
#define KB_3 0x84
#define KB_4 0x85
#define KB_5 0x86
#define KB_6 0x87
#define KB_7 0x88
#define KB_8 0x89
#define KB_9 0x8a

/** @} */

/**
 * @brief Enum representing the application states.
 */
typedef enum {
	MAIN_MENU,         /**< @brief Main menu state. */
	GAME_BET,          /**< @brief Game betting state. */
	GAME_PLAY,         /**< @brief Game playing state. */
	GAME_OVER,         /**< @brief Game over state. */
	GAME_DEALER_TURN,  /**< @brief Game dealer turn state. */
	EXIT               /**< @brief Exit state. */
} app_state_t;

/**
 * @struct app_t
 * @brief Structure to represent the application.
 * @details This structure contains all the elements required to manage the application state:
 * sprites, buttons, game state, cursor, and other UI components.
 */
typedef struct {
	cursor_t cursor;                    /**< Cursor object. */
	sprite_t *background;               /**< Background sprite. */
	queue_t *buttons_main_menu;         /**< Queue of main menu buttons. */
	queue_t *buttons_game_playing;      /**< Queue of game playing buttons. */
	queue_t *buttons_game_over;         /**< Queue of game over buttons. */
	sprite_t *button_bet;               /**< Bet button sprite. */
	font_t font;                        /**< Font object. */
	banner_t banner;                    /**< Banner object. */
	game_t game;                        /**< Game state object. */
	app_state_t state;                  /**< Current application state. */
} app_t;

/**
 * @struct bit_no_t
 * @brief Structure to hold interrupt bit numbers.
 */
typedef struct {
  uint8_t kb;    /**< Keyboard interrupt bit number. */
  uint8_t mouse; /**< Mouse interrupt bit number. */
  uint8_t uart;  /**< UART interrupt bit number. */
  uint8_t timer; /**< Timer interrupt bit number. */
  uint8_t rtc;   /**< RTC interrupt bit number. */
} bit_no_t;

/**
 * @brief Enum representing different types of interrupts.
 */
typedef enum {
	TIMER,     /**< Timer interrupt. */
	KEYBOARD,  /**< Keyboard interrupt. */
	MOUSE,     /**< Mouse interrupt. */
	RTC,       /**< RTC interrupt. */
	UART       /**< UART interrupt. */
} interrupt_type_t;

/**
 * @brief Initializes the application.
 * 
 * @details Allocates memory for the application, initializes its state and various components.
 * @return Pointer to the initialized application, or NULL if initialization fails.
 */
app_t *app_init();

/**
 * @brief Frees the memory allocated for the application and its components.
 * 
 * @param app Pointer to the application to be destroyed.
 */
void 	app_destroy(app_t *app);

/**
 * @brief Initializes the main menu buttons.
 * 
 * @param queue Pointer to the queue of main menu buttons.
 * @return 0 on success, 1 on failure.
 */
int		app_buttons_main_menu_init(queue_t **queue);

/**
 * @brief Initializes the game playing buttons.
 * 
 * @param queue Pointer to the queue of game playing buttons.
 * @return 0 on success, 1 on failure.
 */
int		app_buttons_game_playing_init(queue_t **queue);

/**
 * @brief Initializes the game over buttons.
 * 
 * @param queue Pointer to the queue of game over buttons.
 * @return 0 on success, 1 on failure.
 */
int		app_buttons_game_over_init(queue_t **queue);

/** @} */

#endif /* _LCOM_EV_APP_H_ */
