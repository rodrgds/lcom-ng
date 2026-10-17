#ifndef _UTILS_H_
#define _UTILS_H_

#include <lcom/lcf.h>
#include "numbers.h"
#include "letters.h"

/** @defgroup utils utils
 * @{
 *
 * Usefull functions, structures and definitions for the working of the project
 */

#define SECONDS(n) (60*n)
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define SCORE_BOARD_SIZE 10
#define LETTERS_WIDTH 11

///To determine what kind of object is in the struct
enum object_tag{PLAYER, ENEMY, FUEL, PLATFORM, PLATFORM_BROKEN, PLATFORM_MOVING, SHOT, BCKGRND,OTHEROBJ};

///Object struct for better management
typedef struct Object{
    int x,y;
    int width, height;
    xpm_image_t sprite;
    enum object_tag object_tag;
    bool active;
}Object;

///Current game screen
enum game_state{MAIN_MENU,GAME,GAME_OVER,SCORE_BOARD,END};

///Creates a new object
Object newObject(int x, int y, xpm_image_t xpm, enum object_tag object_tag);

///Verifies if a o1 colides with o2
bool (colides)(Object o1, Object o2);

///Get the xpm for a given integer from 0 to 9
xpm_map_t getNumberXPM(int n);

///Gets the xpm for a given character
xpm_map_t getLetterXPM(char c);

///Get the char from a scancode
char getCodeChar(uint8_t code);

///Changes the state of the game
void change_game_state(enum game_state* ost, enum game_state nst);

///Draw a text in screen from the user input
void draw_text_from_keyboard(char* str, uint8_t data, int* size, int x, int y);

///Loads the scoreboard from file
void load_scores();

#endif /* _UTILS_H_ */

