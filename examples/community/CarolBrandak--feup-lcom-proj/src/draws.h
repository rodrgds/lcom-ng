#include <lcom/lcf.h>
#include "graphics_card.h"
#include "rtc.h"
#include "mouse.h"
#include "images/menu.xpm"
#include "images/map.xpm"
#include "images/bar.xpm"
#include "images/help.xpm"
#include "images/numbers/zero.xpm"
#include "images/numbers/one.xpm"
#include "images/numbers/two.xpm"
#include "images/numbers/three.xpm"
#include "images/numbers/four.xpm"
#include "images/numbers/five.xpm"
#include "images/numbers/six.xpm"
#include "images/numbers/seven.xpm"
#include "images/numbers/eight.xpm"
#include "images/numbers/nine.xpm"
#include "images/numbers/ten.xpm"
#include "images/numbers/points.xpm"
#include "images/lives.xpm"
#include "images/pause.xpm"
#include "images/optionBar.xpm"
#include "images/time.xpm"



extern int xRes;
extern int yRes;

/**
 * @brief Struct pause to store the xpms of the pause menu
 * 
 */
struct Pause
{
  xpm_image_t img;
  xpm_image_t img2;
  int x,y;
  bool option;
};


/**
 * @brief Struct bar to store the xpms of the information bar displayed during the game
 * 
 */
struct Bar
{
  xpm_image_t img[15];
};


/**
 * @brief Struct background to store the background xpm
 * 
 */
struct Background
{
  xpm_image_t img;
  int x,y;

};

/**
 * @brief Struct menu to store the main menu xpm
 * 
 */
struct Menu
{
  xpm_image_t img;
  int x,y;

};

/**
 * @brief Struct help to store the help option xpm
 * 
 */
struct Help
{
  xpm_image_t img;
  int x,y;

};


/**
 * @brief Draws a xpm to a buffer on position x and y of the screen  
 * 
 */
void (draw(int x, int y, int img_width, int img_height, uint32_t * map));

/**
 * @brief Draws a xpm to a buffer on position x and y of the screen but with the image rotated 180 degress
 * 
 */
void (drawDown(int x, int y, int img_width, int img_height, uint32_t * map));

/**
 * @brief Draws a xpm to a buffer on position x and y of the screen but with the image rotated 270  degress to the right
 * 
 */
void (drawLeft(int x, int y, int img_width, int img_height, uint32_t * map));

/**
 * @brief Draws a xpm to a buffer on position x and y of the screen but with the image rotated 90  degress to the right
 * 
 */
void (drawRight(int x, int y, int img_width, int img_height, uint32_t * map));

/**
 * @brief Draws the hours and minutes on the information bar
 * 
 * @param time array containing the int values of hours and minutes
 */
void drawTime(int time[2]);

/**
 * @brief Draws the information bar
 * 
 * @param level Current level
 * @param lives number of lives
 * @param time array containing the int values of hours and minutes
 */
void drawBar(int level, int lives, int time[2]);

/**
 * @brief Draws an overlay to display which option is being chosen in the pause menu
 * 
 * @param option true for option one, false for option two
 */
void drawPauseOption(bool option);

/**
 * @brief Draws the pause menu xpm
 * 
 */
void drawPause();

/**
 * @brief Draws the help screen xpm
 * 
 */
void drawHelp();

/**
 * @brief Draws the mouse
 * 
 * @param mouse struct mouse containing mouse position and image
 */
void drawMouse(Mouse mouse);

/**
 * @brief Draws the main menu
 * 
 */
void drawMenu();

/**
 * @brief Draws the background where it can either be day or night
 * 
 */
int (drawBackground(bool day));

/**
 * @brief Loads all the xpms that are going to be used apart from the entities
 * 
 */
void loadMap();

