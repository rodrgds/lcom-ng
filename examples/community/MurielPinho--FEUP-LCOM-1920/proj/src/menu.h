#include <lcom/lcf.h>

#ifndef MENU_H
#define MENU_H

/** @defgroup Menu Menu
 * @{
 * Functions to control the game menu
 */

/** @brief Enumeration that represents the states of the menu*/
enum projectState
{
    /** @brief Exit project*/
    Exit,
    /** @brief Project is at the Menu state*/
    Menu,
    /** @brief Project is at the playing state*/
    Playing
};

/** @brief Enumeration that represents the states of the menu*/
enum menuState
{
    /** @brief Selecting option*/
    Selecting,
    /** @brief Intructions menu */
    Instructions,
    /** @brief About menu */
    About,
    /** @brief Play selected*/
    Go
};

/**
 * @brief Struct type for specifying menu variables
 */
typedef struct
{
    enum menuState mState; /**< @brief Enumeration for the states of the menu */
    int menuOption;        /**< @brief int to represent the current option */
} MenuObject;

/** @brief Extern variable that contains the interrutPerFrame */
extern int interruptPerFrame;
/** @brief Extern variable that contains the bitmask from the keyboard */
extern uint8_t kbd_bit;
/** @brief Extern variable that contains the bitmask from the timer */
extern uint8_t timerBit;
/** @brief Extern variable that contains the bitmask from the mouse */
extern uint16_t mouseBit;

/**
 * @brief Function to create the menu object
 * @return Returns the created menu object
 */
MenuObject *createMenu();

/**
 * @brief Function to loop in the menu
 * @return Returns an int that represents the project state
 */
int menuLoop();
/**
 * @brief Function to initialize the menu variables and objects
 * @return Returns 0 if successfull
 */
int initMenu();
/**
 * @brief Function to handle the menu input
 * @return Returns 0 if successfull
 */
int menuInput();
/**
 * @brief Function to handle the mouse input
 * @return Returns 1 if left button is pressed
 */
int handleMouse();
/**
 * @brief Function to draw the main menu sprites
 */
void drawMenu();
/**
 * @brief Function to draw the instructions menu sprites
 */
void drawInstructions();
/**
 * @brief Function to draw the about menu sprites
 */
void drawAbout();
/**
 * @brief Function to update menu screen
 */
void updateMenuScreen();
/**
 * @}
 */
#endif /*MENU_H*/
