/**
 @file game.h
 @author Daniel Marques
 @author João Almeida

 Prototypes of the functions that will be the core of the game.
 */

#ifndef GAME_H
#define GAME_H

/** @defgroup game game
 * @{
 *
 * Core functions of the game, Driver_Receive loop inside here
 */

#define MOUNTAIN_COLOR 	 0xFC60	//0x0908
#define MOUNTAIN_Y 			500
#define STICK_MAX_SIZE 		525
#define STICK_COLOR 		0xE71C
#define NUM_SCORES 			1

#define PATH_TO_SCORES					"/scores.txt"
#define PATH_TO_STAND 		 			"/imgs/StickHero_BW.bmp"
#define PATH_TO_WALK 			 			"/imgs/StickHero_Walk.bmp"
#define PATH_TO_KICK 			 			"/imgs/StickHero_Kick.bmp"
#define PATH_TO_BACKGROUND 			"/imgs/Background.bmp"
#define PATH_TO_MOUSE 		 			"/imgs/cursor.bmp"
#define PATH_TO_PLAYS 		 			"/imgs/Play_Selected.bmp"
#define PATH_TO_PLAYUN		 			"/imgs/Play_Unselected.bmp"
#define PATH_TO_EXITS		   			"/imgs/Exit_Selected.bmp"
#define PATH_TO_EXITUN 		 			"/imgs/Exit_Unselected.bmp"
#define PATH_TO_LOGO       			"/imgs/StickHero.bmp"
#define PATH_TO_NEWHIGH    			"/imgs/NewHigh.bmp"
#define PATH_TO_ONE				 			"/imgs/1.bmp"
#define PATH_TO_TWO		     			"/imgs/2.bmp"
#define PATH_TO_THREE			 			"/imgs/3.bmp"
#define PATH_TO_FOUR       			"/imgs/4.bmp"
#define PATH_TO_FIVE       			"/imgs/5.bmp"
#define PATH_TO_SIX        			"/imgs/6.bmp"
#define PATH_TO_SEVEN      			"/imgs/7.bmp"
#define PATH_TO_EIGHT      			"/imgs/8.bmp"
#define PATH_TO_NINE       			"/imgs/9.bmp"
#define PATH_TO_ZERO       			"/imgs/0.bmp"
#define PATH_TO_SCORE						"/imgs/Score.bmp"
#define PATH_TO_HSCORE					"/imgs/Highscore.bmp"
#define PATH_TO_HOUR_ONE	 	 		"/imgs/1_s.bmp"
#define PATH_TO_HOUR_TWO	      "/imgs/2_s.bmp"
#define PATH_TO_HOUR_THREE			"/imgs/3_s.bmp"
#define PATH_TO_HOUR_FOUR       "/imgs/4_s.bmp"
#define PATH_TO_HOUR_FIVE       "/imgs/5_s.bmp"
#define PATH_TO_HOUR_SIX        "/imgs/6_s.bmp"
#define PATH_TO_HOUR_SEVEN      "/imgs/7_s.bmp"
#define PATH_TO_HOUR_EIGHT      "/imgs/8_s.bmp"
#define PATH_TO_HOUR_NINE       "/imgs/9_s.bmp"
#define PATH_TO_HOUR_ZERO       "/imgs/0_s.bmp"
#define PATH_TO_HOUR_SEPARATOR  "/imgs/:_s.bmp"

typedef enum {
	MAIN_MENU, IN_GAME, WALKING, LOSE, REPLAY_MENU
} game_status_t;
typedef enum {
	EXIT_GAME, START_GAME
} options_t;

/** @name  Stick Struct*/
/**@{
 *
 * Struct that keeps info Stores information about the stick
 */
typedef struct {
	unsigned int width;  /**< @brief  Width of the stick   */ /**< @brief   */
	unsigned int x; /**< @brief   Bottom-Left x coordinate */ /**< @brief   */
	unsigned int y; /**< @brief   Bottom-Left y coordinate */ /**< @brief   */
	unsigned int size; /**< @brief    The current length of the stick */ /**< @brief   */
	unsigned int max_size; /**< @brief  Maximum stick size  */
	unsigned int status; /**< @brief   The status of the stick, 1 if vertical , 0 if horizontal */
	unsigned short color; /**< @brief  The color of the stick  */
} stick_t;
/** @} end of Stick Struct */

 /** @name  Platform Struct*/
 /**@{
  *
  * Struct that keeps info about the platforms of the game
  */
typedef struct {
	unsigned int x; /**< @brief Bottom-Left x coordinate  */
	unsigned int y; /**< @brief Bottom-Left y coordinate  */
	unsigned int size_x; /**< @brief The horizontal size of the mountain  */
	unsigned int size_y; /**< @brief  The vertical size of the mountain */
	unsigned short color; /**< @brief  The color of the mountain */
} mountain_t;
/** @} end of Platform Struct */

 /** @name  Ninja Struct*/
 /**@{
	*
	* Struct that keeps info about the position of the ninja
	*/
typedef struct {
	unsigned int x; /**< @brief  Top-Left x coordinate  */
	unsigned int y; /**< @brief  Top-Left y coordinate  */
	unsigned int size_x; /**< @brief  Width of ninja, depends on status. 0 -> 40 , 1 -> 44 , 2 -> 44  */
	unsigned int size_y; /**< @brief  Height of the ninja */
	unsigned int status; /**< @brief  Status of the ninja 0 -> Stand , 1 -> Walking , 2 -> Kicking */
	uint16_t *stand_img; /**< @brief   Path to the image of the ninja standing */
	uint16_t *walk_img; /**< @brief   Path to the image of the ninja walking */
} ninja_t;
/** @} end of Ninja Struct */

/** @name  Mouse Struct*/
/**@{
 *
 * Struct that keeps info about the position of the mouse
 */
typedef struct {
	int x; /**< @brief X coordinate of the mouse (top left corner)  */
	int y; /**< @brief  Y coordinate of the mouse (top left corner) */
	uint16_t *cursor; /**< @brief  Image of the mouse  */
	unsigned int width; /**< @brief  Width of the image of the mouse */
	unsigned int height; /**< @brief  Height of the image of the mouse */
} mouse_t;
/** @} end of Mouse Struct */

/** @name  Highscore Numbers Struct*/
/**@{
 *
 * Struct that keeps info about the images of Highscore
 */
typedef struct {
	uint16_t *one; /**< @brief Image of number one  */
	uint16_t *two; /**< @brief  Image of number two  */
	uint16_t *three; /**< @brief  Image of number three    */
	uint16_t *four; /**< @brief  Image of number four    */
	uint16_t *five; /**< @brief  Image of number five    */
	uint16_t *six; /**< @brief  Image of number six    */
	uint16_t *seven; /**< @brief Image of number seven     */
	uint16_t *eight; /**< @brief  Image of number eight    */
	uint16_t *nine; /**< @brief  Image of number nine    */
	uint16_t *zero; /**< @brief Image of number zero     */
	uint16_t *new_high; /**< @brief   Image of the new_highscore stamp   */
	uint16_t *score; /**< @brief   Image of the word score   */
	uint16_t *high_score; /**< @brief   Image of the word highscore   */
	unsigned int width_of_1; /**< @brief  Width of the image of number one    */
	unsigned int width; /**< @brief   Widht of the rest of the numbers   */
	unsigned int height; /**< @brief   Height of all the numbers   */
	unsigned int n_width; /**< @brief   Widht of the new highscore image   */
	unsigned int n_height; /**< @brief   Height of the new highscore image   */
	unsigned int s_width; /**< @brief   Widht of the score image   */
	unsigned int s_height; /**< @brief   Height of the score image   */
	unsigned int h_width; /**< @brief   Widht of the high_score image   */
	unsigned int h_height; /**< @brief   Height of the high_score image   */
} highscore_t;
/** @} end of Highscore Numbers Struct */

 /** @name  Menu Buttons Struct*/
 /**@{
  *
  * Struct that keeps info about the images of the Menu buttons
  */
typedef struct {
	uint16_t *play_s; /**< @brief  Image of Play button selected  */
	uint16_t *play_un; /**< @brief  Image of Play button unselected  */
	uint16_t *exit_s; /**< @brief   Image of Exit button selected */
	uint16_t *exit_un; /**< @brief  Image of Exit button unselected  */
	unsigned int play_w; /**< @brief  Width of the Play button  */
	unsigned int play_h; /**< @brief   Height of the Play button */
	unsigned int exit_w; /**< @brief  Width of the Exit button  */
	unsigned int exit_h; /**< @brief  Height of the Exit button  */
	unsigned int playx; /**< @brief  X coordinate of the Play button  */
	unsigned int playy; /**< @brief  Y coordinate of the Play button  */
	unsigned int exitx; /**< @brief  X coordinate of the Exit button  */
	unsigned int exity; /**< @brief  Y coordinate of the Exit button   */
} menus_t;
/** @} end of Menu Buttons Struct*/

/** @name  Hours Struct*/
/**@{
 *
 * Struct that keeps info about the current hour, and number images
 */
typedef struct{
	uint16_t *zero; /**<  @brief  Image of the number zero   */
	uint16_t *one; /**<  @brief  Image of the number one   */
	uint16_t *two; /**<  @brief  Image of the number two   */
	uint16_t *three; /**<  @brief  Image of the number three   */
	uint16_t *four; /**<  @brief  Image of the number four  */
	uint16_t *five; /**<  @brief  Image of the number five */
	uint16_t *six; /**<  @brief   Image of the number six  */
	uint16_t *seven; /**<  @brief  Image of the number seven   */
	uint16_t *eight; /**<  @brief  Image of the number eight   */
	uint16_t *nine; /**<  @brief  Image of the number nine   */
	uint16_t *two_points; /**<  @brief   Image of the separator  */
	unsigned int width; /**<  @brief   Width of the separator image  */
	unsigned int height; /**<  @brief   Height of the separator image  */
	unsigned int hour; /**<  @brief   Current hour  */
	unsigned int minute; /**<  @brief  Current minute   */
	unsigned int second; /**<  @brief   Current second   */
}hours_t;
/** @} end of Hours Struct */

/** @name  Game Struct*/
/**@{
 *
 * Struct that keeps info about all objects of the game
 */
typedef struct {
	stick_t stick;  /**<  @brief  Information about the stick   */
	ninja_t ninja; /**<  @brief  Information about the ninja   */
	highscore_t h_numbers; /**<  @brief   Images and sizes of the Highscore numbers images  */
	hours_t hour; /**<  @brief   mages and sizes of the Hour numbers images  */
	mountain_t mountain1; /**<  @brief   Stores information about the mountain the hero is on  */
	mountain_t mountain2; /**<  @brief   Stores ingormation about the mountain the hero will try to jump to  */
	mouse_t mouse; /**<  @brief   Information about the mouse   */
	menus_t menus; /**<  @brief   Information about the menus buttons  */
	uint16_t *background_img; /**<  @brief   The background image of the game  */
	uint16_t *buffer; /**<  @brief  The 2nd buffer   */
	uint16_t *logo; /**<  @brief  The "Stick Hero" image   */
	unsigned int logo_w; /**<  @brief  Width of the logo image   */
	unsigned int logo_h; /**<  @brief  Height of the logo image   */
	unsigned long int score; /**<  @brief  Current score   */
	char *current_dir; /**<  @brief  String with current directory (read from user input)   */
} game_t;
/** @} end of Game Struct */

/**
 @brief Driver_Receive loop , main component of the game
 @param game Struct that holds all information about the game
 @param TIMER_BIT Bit of the timer interrupts
 @param MOUSE_BIT Bit of the mouse interrupts
 @param KBD_BIT Bit of the keyboard interrupts


 */
int runGame(game_t *game, unsigned int TIMER_BIT, unsigned int MOUSE_BIT,
		unsigned int KBD_BIT);

/**
 @brief Prepares or resets the game
 @param game Struct that holds all information about the game
 @param load_images Wheter or not to load images (if 0 means it is a repeat of the game)
 @details Resets all information about the game to the initial stage
 @return 1 if an image could not be loaded, 0 if all is good
 */
int setGame(game_t *game, unsigned int load_images);

/**
 @brief Used to get Highscore outside of game.c
 @return current highscore
 */
unsigned long int getHighscore();

/**
 @brief Used to set Highscore outside of game.c
 @param score Value to set highscore
 */
void setHighscore(unsigned long int score);

/**
 @brief Randomly generates a new Mountain
 @param game Struct that holds all information about the game
 */
void generateNewMountain(game_t *game);

#endif
