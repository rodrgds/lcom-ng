#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#include "graphics.h"
#include "timer.h"

#define WHITE 0xFFFFFF
#define RED 0xED1C24
#define BLUE 0x3F48CC
#define GREEN 0x22B14C
#define YELLOW 0xFFFF00
#define TRANSPARENCY_BYTE 0xAE
#define XPM_DIRECT XPM_8_8_8_8

/** @defgroup drawings Drawings
 * @{
 * Pixmap drawing functions
 */
struct posit{
    uint16_t x; /**< square x position */
    uint16_t y; /**< square y position */
    uint32_t color; /**< current square color */
    uint8_t * sprite; /**< current square pixmap */
    xpm_image_t sprite_info; /**< current square dimensions */
};

struct posit board[28];

struct player{
    uint8_t playerNum; /**< player number (from 0 to 3) */
    uint8_t position; /**< player's current board position */
    uint8_t hp; /**< current player's health points */
    uint32_t color; /**< player's color */
    uint8_t* sprite; /**< player's pixmap */
    xpm_image_t sprite_info; /**< player's pixmap dimensions */
};

struct player players[4];

typedef struct {
    int x,y;             /**< current sprite position */
    int width, height;   /**< sprite dimensions */
    int xspeed, yspeed;  /**< speeds in the x and y directions */
    uint8_t* map;           /**< sprite pixmap (xpm) */
} Cursor;

/**
 * @brief Creates the cursor sprite
 * @param pic cursor pixmap
 * @param x x pos
 * @param y y pos
 * @param xspeed speed on x
 * @param yspeed speed on y
 * @return Cursor Sprite
 */
Cursor* (create_sprite)(xpm_map_t pic, int x, int y, int xspeed, int yspeed);
/**
 * @brief Animates the cursor sprite
 * @param sp Cursor Pointer
 * @return 1 if error, 0 if OK
 */
int (animate_sprite)(Cursor*sp);
/**
 * @brief Erases the sprite, drawing the background passed as a param
 * @param spr Cursor sprite
 * @param db Graphic Memory Address
 * @param bckgrdSpr Background sprite
 * @return 1 if error, 0 if OK
 */
int (erase_sprite)(Cursor* spr, uint8_t* db, uint8_t* bckgrdSpr);
/**
 * @brief Moves the cursor
 * @param spr Cursor sprite
 * @param db Graphic Memory Address
 * @param bckgrdSpr Background Sprite
 * @return 1 if error, 0 if OK
 */
int (move_sprite)(Cursor* spr, uint8_t* db, uint8_t* bckgrdSpr);
/**
 * @brief Redraws the background sprite in the location specified
 * @param x x start position
 * @param y y start position
 * @param width width of the block
 * @param height height of the block
 * @param db Graphic Memory Address
 * @param bckgrdSpr Background Sprite
 * @return 1 if error, 0 if OK
 */
int (clear_image)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t* db, uint8_t* bckgrdSpr);
/**
 * @brief Draws the main backgrounds
 * @param bkgrd Background pixmap
 * @param adrs Graphic Memory Address
 * @return 1 if error, 0 if OK
 */
int (draw_background)(xpm_map_t bkgrd, uint8_t* adrs);
/**
 * @brief Draws a certain pixmap in a given position
 * @param xpm the pixmap to draw
 * @param adrs Graphic Memory Address
 * @param x x position
 * @param y y position
 * @return 1 if error, 0 if OK
 */
int (draw_option)(xpm_map_t xpm, uint8_t* adrs, uint16_t x, uint16_t y);
/**
 * @brief Draws the main menu options in their correct positions
 * @param first First option to be drawn
 * @param second Second option to be drawn
 * @param third Third option to be drawn
 * @param adrs Graphic Memory Address
 * @return 1 if error, 0 if OK
 */
int (draw_menu_options)(xpm_map_t first, xpm_map_t second, xpm_map_t third, uint8_t* adrs);
/**
 * @brief Draws the main game board
 * @param square Starting square pixmap
 * @param adrs Graphic Memory Address
 * @param xi Starting x pos
 * @param yi Starting y pos
 * @return 1 if error, 0 if OK
 */
int(draw_tabuleiro)(xpm_map_t square, uint8_t* adrs, uint16_t xi, uint16_t yi);
/**
 * @brief Paints a square
 * @param square New square pixmap
 * @param adrs Graphic Memory Address
 * @param position Board position to paint
 * @param color New color
 * @return 1 if error, 0 if OK
 */
int (paint_square)(xpm_map_t square, uint8_t* adrs, uint8_t position, uint32_t color);
/**
 * @brief Draws the player
 * @param Plr Pointer to the player to be drawn
 * @param adrs Graphic Memory Address
 * @param position Board position to draw him in
 * @return 1 if error, 0 if OK
 */
int (draw_player)(struct player* Plr, uint8_t* adrs, uint8_t position);
/**
 * @brief Draws the starting state for all players
 * @param redPlr Red Player pixmap
 * @param bluePlr Blue Player pixmap
 * @param greenPlr Green Player pixmap
 * @param yellowPlr Yellow Player pixmap
 * @param adrs Graphic Memory Address
 * @param colorchosen The User's chosen color to play as
 * @return 1 if error, 0 if OK
 */
int draw_start_players(xpm_map_t redPlr, xpm_map_t bluePlr, xpm_map_t greenPlr, xpm_map_t yellowPlr, uint8_t* adrs, uint8_t colorchosen);
/**
 * @brief Resets back to background's previous state
 * @param sprite Background's loaded pixmap
 * @param spriteinfo Background's dimensions
 * @param prevImgInfo Previously added pixmap's dimensions to remove
 * @param adrs Graphic Memory Address
 * @param xi Starting previous pixmaps x pos
 * @param yi Starting previous pixmaps y pos
 * @param x Starting backgrounds pixmaps x pos
 * @param y Starting backgrounds pixmaps y pos
 * @return 1 if error, 0 if OK
 */
int resetBackground(uint8_t * sprite, xpm_image_t spriteinfo,xpm_image_t prevImgInfo, uint8_t* adrs, uint16_t xi, uint16_t yi, uint16_t x, uint16_t y);
/**
 * @brief Draws the timer
 * @param sprite Background's loaded pixmap
 * @param spriteinfo Background's dimensions
 * @param numbers Array of the pixmaps for all numbers
 * @param chosenTime Game time
 * @param elapsedTime Elapsed Time
 * @param adrs Graphic Memory Address
 * @return 1 if error, 0 if OK
 */
int drawTime(uint8_t * sprite, xpm_image_t spriteinfo, xpm_map_t numbers[], uint16_t chosenTime, uint16_t elapsedTime, uint8_t* adrs);
/**
 * @brief Draws the starting state of the slot machine
 * @param slotbg Slot Pixmap
 * @param leverState Current lever state pixmap (usually first state)
 * @param adrs Graphic Memory Address
 * @return 1 if error, 0 if OK
 */
int drawSlots(xpm_map_t slotbg, xpm_map_t leverState, uint8_t* adrs);
/**
 * @brief Moves the lever
 * @param lever Lever's current state pixmap
 * @param spriteBkgrd Background's loaded pixmap
 * @param bkrd_info Background's dimensions
 * @param adrs Graphic Memory Address
 * @return 1 if error, 0 if OK
 */
int throwDice(xpm_map_t lever,uint8_t *spriteBkgrd, xpm_image_t bkrd_info, uint8_t * adrs);
/**
 * @brief Draws the dice numbers
 * @param number Generated number's pixmap
 * @param spriteBkrd Background's loaded pixmap
 * @param x Starting x pos
 * @param y Starting y pos
 * @param adrs Graphic Memory Address
 * @return 1 if error, 0 if OK
 */
int drawNum(xpm_map_t number, uint8_t* spriteBkrd, uint16_t x, uint16_t y, uint8_t* adrs);
/**
 * @brief Draws the cursor sprite
 * @param curs Cursor sprite
 * @param adrs Graphic Memory Address
 * @return 1 if error, 0 if OK
 */
int drawCursor(Cursor* curs, uint8_t* adrs);
/**
 * @brief Draws the player's hearts
 * @param heart Array of heart's pixmaps
 * @param adrs Graphic Memory Address
 * @return 1 if error, 0 if OK
 */
int drawHearts(xpm_map_t heart[], uint8_t *adrs);
/**
 * @brief Draws the health points of all the players
 * @param spriteBkgrd Background's loaded pixmap
 * @param numbers Array of all the number's pixmaps
 * @param adrs Graphic Memory Address
 * @return 1 if error, 0 if OK
 */
int drawHP(uint8_t* spriteBkgrd, xpm_map_t numbers[], uint8_t* adrs);
/**
 * @brief Kills a given player
 * @param Plr Player to be "killed"
 * @param whiteSquare
 * @param adrs
 * @return 1 if error, 0 if OK
 */
int killPlayer(struct player Plr, xpm_map_t whiteSquare, uint8_t* adrs);
/**
 * @brief Draws the Real Time Clock
 * @param spriteBkgrd Background's loaded pixmap
 * @param spriteBkgrdInfo Background's dimensions
 * @param numbers Array of all the number's pixmaps
 * @param adrs Graphic Memory Address
 * @return 1 if error, 0 if OK
 */
int drawRTC(uint8_t* spriteBkgrd, xpm_image_t spriteBkgrdInfo, xpm_map_t numbers[], uint8_t* adrs);
/** @} end of Drawings */


