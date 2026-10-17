#include <lcom/lcf.h>


#include "keyboard.h"
#include "mouse.h"
#include "video_gr.h"
#include "Bitmap.h"


/**
 * @file menus.h
 * @brief Project Module with functions related with menus
 */


/** @defgroup menus menus
 * @{
 * Functions to manage the game menus
 */


/**
 * @brief Struct to store game info
 * On this struct we keep pointers to bitmaps used in the game, as well
 * as the difficuly and mouse informations
 */
typedef struct {
  Bitmap* mainTittle0;    //!< Background for initial menu (with "press enter to continue")
  Bitmap* mainTittle1;    //!< Background for initial menu (without)

  Bitmap* diffMenu;       //!< Background for main menu
  Bitmap* hardBox;        //!< Bitmap for box with "HARD"
  Bitmap* mediumBox;      //!< Bitmap for box with "MEDIUM"
  Bitmap* easyBox;        //!< Bitmap for box with "EASY"

  int difficulty;         //!< Difficulty (1 to 3)

  Bitmap* cursor;         //!< Pointer for cursor's bitmap
  int cursor_x;           //!< X position for the cursor
  int cursor_y;           //!< Y position for the cursor

  Bitmap* pauseMenu;      //!< Bitmap for pause menu
  Bitmap* gameOverMenu;   //!< Bitmap for game over menu

} menus_t;

/**
 * @brief Function to start the game
 * @return Struct with game info
 */
menus_t startMenus();

/**
 * @brief Inicial Menu function
 * @return 0 uppon success
 */
int mainTitle(menus_t* menus);

/**
 * @brief Main Menu function
 * @return 0 to proceed
 * @return 1 to exit
 */
int diffMenu(menus_t* menus);

