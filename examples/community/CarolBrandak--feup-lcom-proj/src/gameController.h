#include <lcom/lcf.h>
#include "entity.h"
#include "macros.h"

/**
 * @brief Enum to define multiple game states
 * 
 */
typedef enum {ALIVE = 1, DEAD = 2, PAUSE = 3, MENU = 4, HELP = 5} State;

/**
 * @brief Enum to define multiple devices
 * 
 */
typedef enum {TIMER = 1, KEYBOARD = 2, MOUSE = 3, RTC = 4} Device;


/**
 * @brief Struct game that stores the whole game information, it has the current level, current number of lives, bool to know if it is day or night, time array for hours and minutes, gameState to know the current state of the game, Array of entities to store all the entities and then the Mouse struct
 * 
 */
struct Game
{
  int level;
  int lives;
  bool day;
  int time[2];
  State gameState;
  struct Entity entities[4];
  Mouse mouse;
};

/**
 * @brief Initializes the mouse and loads the map as well as all of the menus
 * 
 */
void initGame();

/**
 * @brief Initializes all the game's structs 
 * 
 */
void playGame();

/**
 * @brief Checks which option is the selected one in the pause menu
 * 
 */
void checkPauseOption();

/**
 * @brief Handles all the interrupts from all the devices depending on the current state of the game, for example if the timer receives and interrupt and the state is ALIVE then it will draw the game but if the state is MENU then it will draw the menu and so on
 * 
 * @param device 
 */
void DeviceHandler(Device device);

/**
 * @brief Checks all game events like whether the frog is on top or got hit by any entities, whether the frog has won and if it's time for the turtles to dive
 * 
 */
void checkEvent();

/**
 * @brief Checks the mouse events based on the packet, like if the mouse is on top of something and if the mouse clicked on something 
 * 
 * @param pp Current packet
 */
void checkMouseEvent(struct packet pp);

/**
 * @brief Draws all game entites
 * 
 */
void entDraw();

/**
 * @brief Moves all game entites except the frog
 * 
 */
void entMove();

/**
 * @brief Updates the current level
 * 
 */
void nextLevel();

/**
 * @brief Updates the Mouse struct based on the packet receiv
 * 
 * @param pp Current packet
 */
void updateMouse(struct packet pp);

/**
 * @brief Updates the speed of all entities except the frog (called when the player goes to the next lvel)
 * 
 */
void updateEntitiesSpeed();
