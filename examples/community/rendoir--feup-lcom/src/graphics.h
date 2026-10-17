/**
	@file graphics.h
	@author Daniel Marques
	@author João Almeida

	Prototypes of the functions that will handle all the graphics part of this project.
*/

/** @defgroup graphics graphics
 * @{
 *
 * Functions responsible for "moving" the various objects
 */

 #ifndef GRAPHICS_H
 #define GRAPHICS_H

#define VBE_BIOS_INTERRUPT 0x10
#define VBE_MODE_INFO 0x01
#define VBE_CONTROL_INFO 0x00
#define VBE_CALL_SET_MODE 0x4F02
#define GRAPHICS_MODE 0x114
#include "vbe.h"
#include "game.h"

#define MOUNTAIN_MOVE 4 //Changes how fast the moutains move
#define BLACK 0x0000
#define HIGHSCORE_X 10
#define HIGHSCORE_Y 10
#define HIGHSCORE_SEPARATION 5
#define N_HIGH_Y 450
#define HOUR_X  728
#define HOUR_Y  8

/** @struct bit_map_file_header_t
  @brief Used to hold information about the bmp header

  @var type The type of the file (bmp, png , jpeg etc. must be 0x4D42
  @var size Size in bytes of the bitmap file
  @var reserved1 Reserved, must be 0
  @var reserved2 Reserved, must be 0
  @var off_bits The offset in bytes from the bit_map_file_header to the bitmap bits(pixels)

  Based on http://stackoverflow.com/a/14279511
*/
typedef struct __attribute__((__packed__))
{
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t off_bits;
}bit_map_file_header_t;

/** @struct bit_map_info_header_t
  @brief Used to hold information about the bmp image

  @var size Size of this struct
  @var width Width of the image in pixels
  @var height Height of the image in pixels
  @var planes Number of color planes, must be 1
  @var bit_count Specifies the numebr of bit per pixel
  @var compression Type of compression
  @var size_image Size of the image in bytes
  @var x_pels_meter Number of pixels per meter in x axis
  @var y_pels_meter Number of pixels per meter in y axis
  @var clr_use Number of color used by the bitmap
  @var clr_important Number of colors that are important

  Based on http://stackoverflow.com/a/14279511
*/
typedef struct
{
    uint32_t size;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bit_count;
    uint32_t compression;
    uint32_t size_image;
    uint32_t x_pels_meter;
    uint32_t y_pels_meter;
    uint32_t clr_used;
    uint32_t clr_important;
}bit_map_info_header_t;

/**
  @brief Used to move the mouse
  @param game Pointer to struct that holds all information of the game
  @param status Status of the game (0 Walking , 1 In-Game , 2 Main-Menu)
  @param option Option in main menu (0 = PLAY , 1 = EXIT)
*/
void moveMouse(game_t *game, unsigned int status, unsigned int option);

/**
  @brief Used to grow stick on every interrupt, checks if size > v_res and if max_size reached
  @param game Pointer to struct that holds all information of the game

  @return 1 if stick size below maximum size , 0 if maximum size reached.
*/
unsigned int growStick(game_t *game);

/**
  @brief Paints a section of the screen
  @param canvas Place to paint to
  @param x X coordinate of place to paint (top left corner)
  @param y Y coordinate of place to paint (top left corner)
  @param size_x Width of the section of screen to paint
  @param size_y Height of the section of screen to paint
  @param color Color with which to paint the section

  @details Based on lab5 function but we added some more functionalities
*/
void paintScreen(uint16_t *canvas ,unsigned int x , unsigned int y , int size_x, int size_y, unsigned short color);

/**
  @brief Paints bmp on screen
  @param canvas Buffer to paint the BMP on
  @param x Top-Left coordinate
  @param y Top-Left coordinate
  @param img Array of the image
  @param width Width of the image
  @param height Height of the image
  @param ignore_color Color to ignore(not to paint) if -1 then ignore none

  @return 0 if all ok , 1 if user tries to write off-screen

*/
int paintBMP(uint16_t *canvas ,unsigned int x , unsigned int y , uint16_t *img , unsigned int width , unsigned int height, int ignore_color);

/**
  @brief Paints the hour on screen
  @param game Struct that holds all information about the game
  @details The hours, minutes and seconds were obtained using the RTC
*/
void paintHour(game_t *game);

/**
  @brief Moves mountains each timer interrupt, when at end it checks ninja position
  @param game Struct that holds all information about the game
  @param cont Used to make walk animation

  @return 0 if ninja is on top of mountain, 1 if stick too small , 2 if stick too big
*/
unsigned int moveMountains(game_t *game, unsigned int cont);

/**
  @brief Used to start the game or replay
  @param game Struct that holds all information about the game
*/
void startGame(game_t *game);

/**
  @brief Updates the highscore image
  @param game Struct that holds all information about the game
  @param canvas Place to paint the Highscore
  @param x X coordinate to start painting (top left corner)
  @param y Y coordinate to start painting (top left corner)
  @param highscore Highscore to be printed on screen
*/
void updateHighscore(game_t *game, uint16_t *canvas, unsigned int x , unsigned int y, unsigned int highscore);

/**
  @brief Repaints the 2nd buffer
  @param game Struct that holds all the information of the game
  @param status Current status of the game (0 Walking , 1 In-Game , 2 Main-Menu, 3 Falling, 4 Replay Menu)
  @param option Option selected (0 = PLAY ,  1 = EXIT)
*/
void updateBuffer(game_t *game, unsigned int status , unsigned int option);

/**
  @brief Changes which button of the menu is selected
  @param game Struct that holds all the information of the game
  @param option Which option is selected (0 = PLAY , 1 = EXIT)
*/
void updateMenu(game_t *game, unsigned int option);

/**
  @brief Animation to make ninja fall when GAME OVER
  @param game Struct that holds all the information of the game
  @return 0 if ninja is still falling , 1 if animation over
*/
unsigned int ninjaFallAnimation(game_t *game);

/**
  @brief Paints the final score of the user centered
  @param game Struct that holds all the information of the game
  @param score Score of the player
*/
void paintFinalScore(game_t *game , unsigned int score);




/**
  @brief Function to load the bmp data into the structs and array
  @param filename Path to the bmp to load
  @param bitmapInfoHeader Pointer to struct which will hold the information about the bmp image
  @return pointer to array with color of the pixels of the bmp

  For some reason in the array it returns the image is upside down
  Based on http://stackoverflow.com/a/14279511
*/
uint16_t *LoadBitmapFile(char *filename, bit_map_info_header_t *bit_map_info_header);

/**
  @brief Manipulates the given array (represents an image), this is needed in bmp read
          or they would be in weird position
  @param arr The array to reverse
  @param w Width of the image
  @param w Height of the image

  @return The reversed array
*/
uint16_t *correctImage(uint16_t *arr ,unsigned int w , unsigned int h);


/**
  @brief Computes where to start painting numbers in order for them to be centered
  @param w_of_1 Width of the number 1
  @param w_of_others Width of the other numbers
  @param separation How many pixels separate each number
  @param number Number to be painted
  @param x X coordinate that will be the center of the number
  @return X coordinate to start painting
*/
unsigned int centerNumbers(unsigned int number , unsigned int w_of_1 , unsigned int w_of_others , unsigned int separation, unsigned int x);

/**
  @brief Separates a number by its digits
  @param number Number to be separated
  @param size Variable that will represent the size of the array (number of digits)
  @return Array with single digits in the original order of the number
*/
unsigned int *separateNumbers(unsigned int number, unsigned int *size);


/* ------------------------------------------------------------------------------------------------------
	           -------------------------- PURELY UTILITY FUNCTIONS ------------------------
	 ------------------------------------------------------------------------------------------------------*/

/**
  @brief Used to map the VRAM
  @param vbe Struct that will hold the information about VRAM
  @details Imported from lab5
*/
void *vramMap(vbe_mode_info_t *vbe);

/**
  @brief Initializes the graphics mode
  @param mode Which graphics mode to use
  @details Imported from lab5
*/
void *vgInit(unsigned short mode);

/**
  @brief Exits graphics mode and returns to text mode
  @return 1 if something went wront , 0 if all good
  @details Imported from lab5
*/
int vgExit();

/**
  @brief Used to obtain h_res oustside of graphics.c file
  @return h_res
*/
unsigned int getHRes();
/**
  @brief Used to obtain v_res oustside of graphics.c file
  @return v_res
*/
unsigned int getVRes();

/**
  @brief Used to obtain the pointer to the video memory array
  @return video_mem
*/
uint16_t *getVideoMem();

#endif
