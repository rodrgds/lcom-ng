#ifndef GAME_H
#define GAME_H

#include <lcom/lcf.h>

#include <time.h> 
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "keyboard.h"
#include "rtc.h"
#include "rtc_macros.h"
#include "mouse.h"
#include "vc_macros.h"
#include "i8042.h"
#include "vc.h"

/**@brief Struct that stores the name of a player*/
typedef struct name{
  char * nameAr;                /**< @brief the name of the player in an array of char*/
  unsigned int nameSize;        /**<@brief the number of letter of the name*/
  letter * nameXpm;             /**< @brief the name of the player in an array of xpms*/
} name;

/**@brief Struct that stores the information about a highscore*/
typedef struct highScore{
  int speed;            /**< @brief number of letter per second*/
  realTime date;        /**< @brief the date of the highscore*/
  name name;          /**< @brief the name of the player*/
} highScore ;

/**@brief Mouse left button state and position*/
typedef struct mouse{
  int mouseX;           /**< @brief horizontal position of the cursor on the screen*/
  int mouseY;           /**< @brief vertical position of the cursor on the screen*/
  bool lbPressed;       /**< @brief true if left button is pressed and false otherwise*/
}mousePos;

/**@brief Game mode identifier*/
typedef enum mode{
  MENU,                 /**< @brief Menu Screen*/
  CLASSIC,              /**< @brief Classic Game Mode*/
  TIMED,                /**< @brief Timed Game Mode*/
  RESULTS,              /**< @brief Results Screen*/
  HIGHSCORES,           /**< @brief High scores screen*/
  SAVE_SCORE,            /**< @brief Save score screen - ask for the name of the player*/
  EXIT                  /**< @brief Exit the game*/
} mode;

/**@brief Indicates the type of key that was pressed*/
typedef enum keyPressed{
  DOT,                  /**< @brief Final dot was pressed*/
  CAPS,                 /**< @brief Caps or Shift was pressed or release*/
  NOT_VALID,            /**< @brief A key that is not a letter nor a recognized symbol was pressed*/
  VALID,                /**< @brief A letter was pressed - makecode; or BackSpace*/
  ERRORSTATE,           /**< @brief Indicate too much errors were made*/
  ESC                   /**< @brief Esc key was pressed */
} keyPressed;

/**@brief The state of the game*/
typedef struct gameState{
  mode mode;        /**< @brief The current mode of the game*/
  bool start;       /**< @brief determines if it is the first frame to draw*/
  bool drawGame;    /**< @brief Indicates if a new fram should be drawn or not*/
} gameState;

/**@brief Indicates the result of a game*/
typedef struct result{
  int timeSpentSec;           /**< @brief Seconds spent writing the last sentence*/
  int timeSpentCentSec;       /**< @brief Centiseconds spente writing the last sentence*/
  int speed;                  /**< @brief Average number of letters written by second*/
} result;

/**@brief Mouse event enumeration for the actual state*/
typedef struct Game{

  uint8_t * road_sprite;                         /**< @brief Game Scenery Sprite*/
  uint8_t * car_sprite;                          /**< @brief Car Sprite*/
  uint8_t * field_sprite;                        /**< @brief Filed to write Sprite*/
  uint8_t * error_sprite;                        /**< @brief Error Sign Sprite*/
  uint8_t * menu_sprite;                         /**< @brief Menu Sprite*/
  uint8_t * button1_sprite;                      /**< @brief Classic Mode Unpressed Button Sprite*/
  uint8_t * button2_sprite;                      /**< @brief Timed Mode Unpressed Button Sprite*/
  uint8_t * button1Pressed_sprite;               /**< @brief Classic Mode Pressed Button Sprite*/
  uint8_t * button2Pressed_sprite;               /**< @brief Timed Mode Pressed Button Sprite*/
  uint8_t * mouse_sprite;                        /**< @brief Mouse Cursor Sprite*/
  uint8_t * results_sprite;                      /**< @brief Results Image Sprite*/
  uint8_t * menu_button_sprite;                  /**< @brief Back to Menu Unpressed Button Sprite*/
  uint8_t * menu_button_pressed_sprite;          /**< @brief Back to Menu Pressed Button Sprite*/
  uint8_t * redlight_sprite;                     /**< @brief Redlight Sprite*/
  uint8_t * yellowlight_sprite;                  /**< @brief Yellowlight Sprite*/
  uint8_t * greenlight_sprite;                   /**< @brief Greenlight Sprite*/
  uint8_t * highscores_sprite;                   /**< @brief High Scores Image Sprite*/
  uint8_t * highscores_button_pressed_sprite;    /**< @brief High Scores Pressed Button Sprite*/
  uint8_t * highscores_button_unpressed_sprite;  /**< @brief Save Scores Unpressed Button Sprite*/
  uint8_t * saveScores_sprite;                   /**< @brief Save Scores Image Sprite*/
  uint8_t * saveScore_button_sprite;             /**< @brief Save Scores Pressed Button Sprite*/
  uint8_t * saveScore_button_unpressed_sprite;   /**< @brief Save Scores Unpressed Button Sprite*/

  xpm_image_t carImg;                            /**< @brief Game Scenery xpm*/
  xpm_image_t roadImg;                           /**< @brief Car Sprite xpm*/
  xpm_image_t fieldImg;                          /**< @brief Filed to write xpm*/
  xpm_image_t errorImg;                          /**< @brief Error Sign xpm*/
  xpm_image_t menuImg;                           /**< @brief Menu xpm*/
  xpm_image_t button1Img;                        /**< @brief Classic Mode Unpressed Button xpm*/
  xpm_image_t button2Img;                        /**< @brief Timed Mode Unpressed Button xpm*/
  xpm_image_t button1PressedImg;                 /**< @brief Classic Mode Pressed Button xpm*/
  xpm_image_t button2PressedImg;                 /**< @brief Timed Mode Pressed Button xpm*/
  xpm_image_t mouseImg;                          /**< @brief Mouse Cursor xpm*/
  xpm_image_t resultsImg;                        /**< @brief Results Image xpm*/
  xpm_image_t menu_buttonImg;                    /**< @brief Back to Menu Unpressed Button xpm*/
  xpm_image_t menu_button_pressedImg;            /**< @brief Back to Menu Pressed Button xpm*/
  xpm_image_t redlightImg;                       /**< @brief Redlight xpm*/
  xpm_image_t yellowlightImg;                    /**< @brief Yellowlight xpm*/
  xpm_image_t greenlightImg;                     /**< @brief Greenlight xpm*/
  xpm_image_t highscoresImg;                     /**< @brief High Scores Image xpm*/
  xpm_image_t highscores_button_pressedImg;      /**< @brief High Scores Pressed Button xpm*/
  xpm_image_t highscores_button_unpressedImg;    /**< @brief Save Scores Unpressed Button xpm*/
  xpm_image_t saveScoresImg;                     /**< @brief Save Scores Image xpm*/
  xpm_image_t saveScore_buttonImg;               /**< @brief Save Scores Pressed Button xpm*/
  xpm_image_t saveScore_button_unpressedImg;     /**< @brief Save Scores Unpressed Button xpm*/

  gameState state;            /**< @brief Keep track of the state of the game*/

  unsigned int moveCar;       /**< @brief Position where the car is going to be drawn*/
  float oneMove;              /**< @brief Number of pixels the car moves when the input is correct*/
  
  unsigned int nLet;              /**< @brief total number of letter written in a game mode*/
  unsigned int numberOfLetters;   /**< @brief number of letters of a sentence in classic mode*/
  keyPressed keyState;            /**< @brief Indicates the type of key that was pressed*/
  bool completed;                 /**< @brief Indicates that the sentence is complete*/
  bool errorState;                /**< @brief Signs a error state - maximum number of errors*/
  uint8_t errors;                 /**< @brief Number of errors*/
  bool capital;                   /**< @brief True if Caps or Shift is active*/
  int CapsMakeCount;              /**< @brief Auxiliar variable to check Caps key state*/

  realTime * date;                /**<@brief The real date to show on the main menu*/
                 
  unsigned int NUMBEROFSCORES;    /**< @brief The number of scores registed (5 max)*/
  highScore* highscores;          /**< @brief Game high scores (5 max)*/
  name * name;                    /**< @brief Name of the player*/

  mousePos mc;                    /**< @brief Mouse left button and position information*/

  result result;                  /**< @brief Result of the last played game*/
  
} Game;

typedef enum inputValidation{
  CORRECT,
  WRONG,
  BACKSPACE,
  OTHER
} inputValidation;

Game * game;

/**
 *@brief Load letters,sentences and xpm's, set graphics mode and start game
 * 
 * @return 0 on success and 1 otherwise
 */
int gameStart();

/**
 *@brief  Load letters,sentences and xpm's, set graphics mode and start game
 * 
 *@return Return 0 upon success and non-zero otherwise
 */
int gameStateHandler();

/**
 * @brief Draw menu components, buttons and mouse and go to game modes if buttons are pressed
 * 
 * @param timedResults
 * @param sec3counter  
 * @param timerreset
*/
void menu_mode(bool * timedResults, bool * sec3counter, bool * timerreset);

/**
 * @brief Draw Menu's components on screen depending on the position/pressed buttons of the mouse
 */
void drawMainMenu();

/**
 * @brief Draw classic mode sentence,input and handle classic game mode state
 * 
 * @param sentence The sentence that the user must write
 * @param input The letters typed by the user
 * @param index The index of the last valid letter typed
 * @param sec3counter  
 * @param timerreset
*/
void classic_mode(letter ** sentence, letter ** input, unsigned int * index, bool * sec3counter, bool * timerreset);

/**
 * @brief Draw the initial game components for the Classic Mode on the screen.
 * 
 * Generate a random sentence and allocate space for input. Define how much the car moves on each correct input based on the size of the generated sentence.
 * 
 * @param sentence The sentence that the user must write
 * @param input The letters typed by the user
 */
void classicModeStart(letter ** sentence, letter ** input);

/**
 * @brief Draw game components for Classic Mode on screen
 * 
 * @param sentence The sentence that the user must write
 * @param input The letters typed by the user
 * @param index The index of the last valid letter typed
 */
void classicModeDraw(letter * sentence, letter * input, unsigned int i);

/**
 * @brief Draw timed mode sentence,input and handle timed game mode state
 * 
 * @param sentence The sentence that the user must write
 * @param input The letters typed by the user
 * @param index The index of the last valid letter typed
 * @param sec3counter  
 * @param timerreset
 * @param timedResults
*/
void timed_mode(letter ** sentence, letter ** input, unsigned int * index, bool * sec3counter, bool * timerreset, bool * timedResults);

/**
 * @brief Draw the initial game components for the Timed Mode on the screen.
 * 
 * Generate a random sentence and allocate space for input. Define how much the car moves on each correct input based on the size of the generated sentence.
 * 
 * @param sentence The sentence that the user must write
 * @param input The letters typed by the user
 */
void timedModeStart(letter ** sentence, letter ** input);

/**
 * @brief Draw game components for Timed Mode on screen
 * 
 * @param sentence The sentence that the user must write
 * @param input The letters typed by the user
 * @param index The index of the last valid letter typed
 */
void timedModeDraw(letter * sentence, letter * input, unsigned int i);

/**
 * @brief Perform game actions according to the game State
 * 
 * @param sentence The sentence that the user must write
 * @param input The letters typed by the user
 * @param i The index of the last valid letter typed
 */
void kbc_handle(letter ** sentence, letter ** input, unsigned int * i);

/** @brief Handle mouse movements
 * 
 *  @param p mouse packet
*/
void mouse_handler(struct packet * p);

/**
 *@brief  Displays a 3 2 1 counter before the start of the game mode
*/
void counterStart();

/**
 * @brief Draws the results layout
 */
void resultScreen();

/**
  * @brief Calculates statistics to send to resultScreen() to display
  */
void sentenceCompleted();

/**
  * @brief Calculates statistics to send to resultScreenTimed() to display
*/
void sentenceCompletedTimed();

/**
  * @brief  Check the scancode read from the keyboard to determine next action to take
  * 
  * @param scancode The scancode read from the keyboard
  * 
  * @return a variable that indicates te type of key pressed
  */
keyPressed checkScanCode(uint8_t * scancode);

/**
 * @brief  Checks if the input of a letter by the user corresponds to the one at index i in the sentence
 * 
 * @param let the input sentence to validate
 * @param scancode the scancode read from de keyboard
 * @param i the index of the letter to validate
 * 
 * @return a variable that indicates if the input is correct, wrong, a backspace or other  
  */
inputValidation validateLetter(letter * let, uint8_t * scancode, letter ** input, unsigned i);

/**
 * @brief  Generate a random sentence from a array of sentences
 * 
 * @param sentence the sentence generated and updated inside the function
*/
void generateRandomSentence(letter ** sentence);

/**
 * @brief  Draw the time elapsed since the start of the timed mode
*/
void drawTimeElapsed();

/**
* @brief Draw the date
*/
void draw_date();

/** @brief Show a screen with the highest scores (top 5)*/
void highScoresScreen();

/**
 * @brief Shows a screen with a "buffer" where the user should enter a username to save the score
*/
void save_score_screen();

/**
 * @brief Handles keyboard interruptions and validates scancodes to write the player's username
 * 
 * @param scancode The scancode read from the keyboard
*/
void playerName(uint8_t * scancode);

/** @brief Go to main menu if button is pressed and save score if in Save Score mode*/
void back_to_menu();

/** @brief Free space allocated for the sprites*/
void free_sprites();

#endif /* GAME_H */
