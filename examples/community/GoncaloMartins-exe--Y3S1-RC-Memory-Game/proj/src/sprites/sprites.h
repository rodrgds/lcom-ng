#ifndef SPRITES_H
#define SPRITES_H

#include <lcom/lcf.h>
#include <stdint.h>

#include "../controllers/video/graphic.h"
#include "../controllers/utils.h"
#include "../controllers/timer/timer.h"
#include "../controllers/timer/i8254.h"

//Backgorunds
#include "../images/backgrounds/background.xpm"
#include "../images/backgrounds/loading_1.xpm"
#include "../images/backgrounds/loading_2.xpm"
#include "../images/backgrounds/loading_3.xpm"
#include "../images/backgrounds/loading_4.xpm"
#include "../images/backgrounds/loading_5.xpm"
#include "../images/backgrounds/loading_6.xpm"
#include "../images/backgrounds/loading_7.xpm"
#include "../images/backgrounds/loading_8.xpm"
#include "../images/backgrounds/loading_9.xpm"
#include "../images/backgrounds/loading_10.xpm"
#include "../images/backgrounds/loading_11.xpm"
#include "../images/backgrounds/loading_12.xpm"
#include "../images/backgrounds/loading_13.xpm"
#include "../images/backgrounds/loading_14.xpm"
#include "../images/backgrounds/loading_15.xpm"
#include "../images/backgrounds/loading_16.xpm"
#include "../images/backgrounds/Instruções.xpm"

//Imagens
#include "../images/cursor.xpm"

//Numeros
#include "../images/numbers/numbers_xpm.h"

//Tempo
#include "images/time/time_xpm.h"

//Alfabeto
#include "../images/alphabet/A.xpm"
#include "../images/alphabet/B.xpm"
#include "../images/alphabet/C.xpm"
#include "../images/alphabet/D.xpm"
#include "../images/alphabet/E.xpm"
#include "../images/alphabet/F.xpm"
#include "../images/alphabet/G.xpm"
#include "../images/alphabet/H.xpm"
#include "../images/alphabet/I.xpm"
#include "../images/alphabet/J.xpm"
#include "../images/alphabet/K.xpm"
#include "../images/alphabet/L.xpm"
#include "../images/alphabet/M.xpm"
#include "../images/alphabet/N.xpm"
#include "../images/alphabet/O.xpm"
#include "../images/alphabet/P.xpm"
#include "../images/alphabet/Q.xpm"
#include "../images/alphabet/R.xpm"
#include "../images/alphabet/S.xpm"
#include "../images/alphabet/T.xpm"
#include "../images/alphabet/U.xpm"
#include "../images/alphabet/V.xpm"
#include "../images/alphabet/W.xpm"
#include "../images/alphabet/X.xpm"
#include "../images/alphabet/Y.xpm"
#include "../images/alphabet/Z.xpm"
#include "../images/alphabet/letterSpace.xpm"

//Cartas
#include "../images/cards/1.xpm"
#include "../images/cards/2.xpm"
#include "../images/cards/3.xpm"
#include "../images/cards/4.xpm"
#include "../images/cards/5.xpm"
#include "../images/cards/6.xpm"
#include "../images/cards/7.xpm"
#include "../images/cards/8.xpm"
#include "../images/cards/9.xpm"

//Abacate
#include "../images/cards/abacate/cartaAbacate1.xpm"
#include "../images/cards/abacate/cartaAbacate2.xpm"
#include "../images/cards/abacate/cartaAbacate3.xpm"
#include "../images/cards/abacate/cartaAbacate4.xpm"
#include "../images/cards/abacate/cartaAbacate5.xpm"
#include "../images/cards/abacate/cartaAbacate6.xpm"
#include "../images/cards/abacate/cartaAbacate7.xpm"
#include "../images/cards/abacate/cartaAbacate8.xpm"

//Banana
#include "../images/cards/banana/cartaBanana1.xpm"
#include "../images/cards/banana/cartaBanana2.xpm"
#include "../images/cards/banana/cartaBanana3.xpm"
#include "../images/cards/banana/cartaBanana4.xpm"
#include "../images/cards/banana/cartaBanana5.xpm"
#include "../images/cards/banana/cartaBanana6.xpm"
#include "../images/cards/banana/cartaBanana7.xpm"
#include "../images/cards/banana/cartaBanana8.xpm"

//Cereija
#include "../images/cards/cereija/cartaCereija1.xpm"
#include "../images/cards/cereija/cartaCereija2.xpm"
#include "../images/cards/cereija/cartaCereija3.xpm"
#include "../images/cards/cereija/cartaCereija4.xpm"
#include "../images/cards/cereija/cartaCereija5.xpm"
#include "../images/cards/cereija/cartaCereija6.xpm"
#include "../images/cards/cereija/cartaCereija7.xpm"
#include "../images/cards/cereija/cartaCereija8.xpm"

//Coco
#include "../images/cards/coco/cartaCoco1.xpm"
#include "../images/cards/coco/cartaCoco2.xpm"
#include "../images/cards/coco/cartaCoco3.xpm"
#include "../images/cards/coco/cartaCoco4.xpm"
#include "../images/cards/coco/cartaCoco5.xpm"
#include "../images/cards/coco/cartaCoco6.xpm"
#include "../images/cards/coco/cartaCoco7.xpm"
#include "../images/cards/coco/cartaCoco8.xpm"

//Laranja
#include "../images/cards/laranja/cartaLaranja1.xpm"
#include "../images/cards/laranja/cartaLaranja2.xpm"
#include "../images/cards/laranja/cartaLaranja3.xpm"
#include "../images/cards/laranja/cartaLaranja4.xpm"
#include "../images/cards/laranja/cartaLaranja5.xpm"
#include "../images/cards/laranja/cartaLaranja6.xpm"
#include "../images/cards/laranja/cartaLaranja7.xpm"
#include "../images/cards/laranja/cartaLaranja8.xpm"

//Maçã
#include "../images/cards/maca/cartaMaçã1.xpm"
#include "../images/cards/maca/cartaMaçã2.xpm"
#include "../images/cards/maca/cartaMaçã3.xpm"
#include "../images/cards/maca/cartaMaçã4.xpm"
#include "../images/cards/maca/cartaMaçã5.xpm"
#include "../images/cards/maca/cartaMaçã6.xpm"
#include "../images/cards/maca/cartaMaçã7.xpm"
#include "../images/cards/maca/cartaMaçã8.xpm"

//Melancia
#include "../images/cards/melancia/cartaMelancia1.xpm"
#include "../images/cards/melancia/cartaMelancia2.xpm"
#include "../images/cards/melancia/cartaMelancia3.xpm"
#include "../images/cards/melancia/cartaMelancia4.xpm"
#include "../images/cards/melancia/cartaMelancia5.xpm"
#include "../images/cards/melancia/cartaMelancia6.xpm"
#include "../images/cards/melancia/cartaMelancia7.xpm"
#include "../images/cards/melancia/cartaMelancia8.xpm"

//Morango
#include "../images/cards/morango/cartaMorango1.xpm"
#include "../images/cards/morango/cartaMorango2.xpm"
#include "../images/cards/morango/cartaMorango3.xpm"
#include "../images/cards/morango/cartaMorango4.xpm"
#include "../images/cards/morango/cartaMorango5.xpm"
#include "../images/cards/morango/cartaMorango6.xpm"
#include "../images/cards/morango/cartaMorango7.xpm"
#include "../images/cards/morango/cartaMorango8.xpm"


/// @brief Transparent color used in sprites
#define SPRITE_TRANSPARENTE 0xA020F0

/// @brief Constant representing the DIRECT_COLOR mode
#define DIRECT_COLOR 0x06

/**
 * @brief Structure representing a Sprite.
 * 
 * A sprite consists of a 2D image with specified width and height, and a buffer of pixel colors.
 */
typedef struct Sprite {
    uint16_t height;     ///< Height of the sprite in pixels
    uint16_t width;      ///< Width of the sprite in pixels
    uint32_t *colors;    ///< Pointer to color data (pixel buffer)
} Sprite;

/**
 * @brief Structure representing an RGB color.
 */
typedef struct {
    uint8_t r; ///< Red component (0–255)
    uint8_t g; ///< Green component (0–255)
    uint8_t b; ///< Blue component (0–255)
} ColorRGB;

/**
 * @brief Enum representing different types of fruits.
 */
typedef enum {
    ABACATE,   ///< Avocado
    BANANA,    ///< Banana
    CEREIJA,   ///< Cherry
    COCO,      ///< Coconut
    LARANJA,   ///< Orange
    MACA,      ///< Apple
    MELANCIA,  ///< Watermelon
    MORANGO    ///< Strawberry
} Fruta;

/**
 * @brief Predefined 16-color Minix palette in RGB format.
 * 
 * Indexed by Minix color index (0–15).
 */
static ColorRGB minix_palette[16] = {
    {0x00, 0x00, 0x00}, // 0 - Black
    {0x80, 0x00, 0x00}, // 1 - Dark Red
    {0x00, 0x80, 0x00}, // 2 - Dark Green
    {0x80, 0x80, 0x00}, // 3 - Brown
    {0x00, 0x00, 0x80}, // 4 - Dark Blue
    {0x80, 0x00, 0x80}, // 5 - Dark Magenta
    {0x00, 0x80, 0x80}, // 6 - Dark Cyan
    {0xC0, 0xC0, 0xC0}, // 7 - Light Gray
    {0x80, 0x80, 0x80}, // 8 - Dark Gray
    {0x00, 0x00, 0xFF}, // 9 - Light Blue
    {0x00, 0xFF, 0x00}, // 10 - Light Green
    {0x00, 0xFF, 0xFF}, // 11 - Light Cyan
    {0xFF, 0x00, 0x00}, // 12 - Light Red
    {0xFF, 0x00, 0xFF}, // 13 - Light Magenta
    {0xFF, 0xFF, 0x00}, // 14 - Yellow
    {0xFF, 0xFF, 0xFF}  // 15 - White
};

/// @brief Pointer to the sprite used for the mouse cursor
Sprite *mouseCursorSprite;

/// @brief Current X coordinate of the mouse cursor
int cursor_x;

/// @brief Current Y coordinate of the mouse cursor
int cursor_y;

//Backgorunds
Sprite *background;
Sprite *loading1;
Sprite *loading2;
Sprite *loading3;
Sprite *loading4;
Sprite *loading5;
Sprite *loading6;
Sprite *loading7;
Sprite *loading8;
Sprite *loading9;
Sprite *loading10;
Sprite *loading11;
Sprite *loading12;
Sprite *loading13;
Sprite *loading14;
Sprite *loading15;
Sprite *loading16;
Sprite *instructions;

/// @brief Sprite for letters initialization
Sprite *letter_A;
Sprite *letter_B;
Sprite *letter_C;
Sprite *letter_D;
Sprite *letter_E;
Sprite *letter_F;
Sprite *letter_G;
Sprite *letter_H;
Sprite *letter_I;
Sprite *letter_J;
Sprite *letter_K;
Sprite *letter_L;
Sprite *letter_M;
Sprite *letter_N;
Sprite *letter_O;
Sprite *letter_P;
Sprite *letter_Q;
Sprite *letter_R;
Sprite *letter_S;
Sprite *letter_T;
Sprite *letter_U;
Sprite *letter_V;
Sprite *letter_W;
Sprite *letter_X;
Sprite *letter_Y;
Sprite *letter_Z;
Sprite *letter_SPACE;


/// @brief Sprite for cards
Sprite *c1;
Sprite *c2;
Sprite *c3;
Sprite *c4;
Sprite *c5;
Sprite *c6;
Sprite *c7;
Sprite *c8;
Sprite *c9;

/// @brief Sprite for Abacate cards
Sprite *cartaAbacate1;
Sprite *cartaAbacate2;
Sprite *cartaAbacate3;
Sprite *cartaAbacate4;
Sprite *cartaAbacate5;
Sprite *cartaAbacate6;
Sprite *cartaAbacate7;
Sprite *cartaAbacate8;

/// @brief Sprite for Banana cards
Sprite *cartaBanana1;
Sprite *cartaBanana2;
Sprite *cartaBanana3;
Sprite *cartaBanana4;
Sprite *cartaBanana5;
Sprite *cartaBanana6;
Sprite *cartaBanana7;
Sprite *cartaBanana8;

/// @brief Sprite for Cereija cards
Sprite *cartaCereija1;
Sprite *cartaCereija2;
Sprite *cartaCereija3;
Sprite *cartaCereija4;
Sprite *cartaCereija5;
Sprite *cartaCereija6;
Sprite *cartaCereija7;
Sprite *cartaCereija8;

/// @brief Sprite for cartaCoco cards
Sprite *cartaCoco1;
Sprite *cartaCoco2;
Sprite *cartaCoco3;
Sprite *cartaCoco4;
Sprite *cartaCoco5;
Sprite *cartaCoco6;
Sprite *cartaCoco7;
Sprite *cartaCoco8;

/// @brief Sprite for Laranja cards
Sprite *cartaLaranja1;
Sprite *cartaLaranja2;
Sprite *cartaLaranja3;
Sprite *cartaLaranja4;
Sprite *cartaLaranja5;
Sprite *cartaLaranja6;
Sprite *cartaLaranja7;
Sprite *cartaLaranja8;

/// @brief Sprite for Maça cards
Sprite *cartaMaca1;
Sprite *cartaMaca2;
Sprite *cartaMaca3;
Sprite *cartaMaca4;
Sprite *cartaMaca5;
Sprite *cartaMaca6;
Sprite *cartaMaca7;
Sprite *cartaMaca8;

/// @brief Sprite for Melancia cards
Sprite *cartaMelancia1;
Sprite *cartaMelancia2;
Sprite *cartaMelancia3;
Sprite *cartaMelancia4;
Sprite *cartaMelancia5;
Sprite *cartaMelancia6;
Sprite *cartaMelancia7;
Sprite *cartaMelancia8;

/// @brief Sprite for Morango cards
Sprite *cartaMorango1;
Sprite *cartaMorango2;
Sprite *cartaMorango3;
Sprite *cartaMorango4;
Sprite *cartaMorango5;
Sprite *cartaMorango6;
Sprite *cartaMorango7;
Sprite *cartaMorango8;

//teste
Sprite* abacate_frames[34];
Sprite* banana_frames[34];
Sprite* cereija_frames[34];
Sprite* coco_frames[34];
Sprite* laranja_frames[34];
Sprite* maca_frames[34];
Sprite* melancia_frames[34];
Sprite* morango_frames[34];

/// @brief Sprite for digits from 0 to 9
extern Sprite *digit_0, *digit_1, *digit_2, *digit_3, *digit_4;
extern Sprite *digit_5, *digit_6, *digit_7, *digit_8, *digit_9;

/// @brief Sprite for colon(:)
extern Sprite *digit_colon;

extern Sprite *s_T, *s_I, *s_M, *s_E;


/**
 * @brief Creates a new sprite from the given XPM map.
 * 
 * @param spriteMap XPM map to load the sprite from
 * @return Sprite* Pointer to the created sprite
 */
Sprite *sprite_novo(xpm_map_t spriteMap);

/**
 * @brief Frees the memory occupied by the given sprite.
 * 
 * @param sprite Pointer to the sprite to be destroyed
 */
void sprite_destruir(Sprite *sprite);

/**
 * @brief Draws the given sprite at the specified position.
 * 
 * @param sprite Pointer to the sprite to draw
 * @param posX Horizontal position on screen
 * @param posY Vertical position on screen
 * @return int 0 on success, non-zero on failure
 */
int sprite_desenhar(Sprite *sprite, int posX, int posY);

/**
 * @brief Converts an RGB color to a Minix color index.
 * 
 * @param r Red component (0–255)
 * @param g Green component (0–255)
 * @param b Blue component (0–255)
 * @return uint8_t Minix color index
 */
uint8_t rgb_to_minix_index(uint8_t r, uint8_t g, uint8_t b);

void sprites_carregar_1();
void sprites_carregar_2();
void sprites_carregar_3();
void sprites_carregar_4();
void sprites_carregar_5();
void sprites_carregar_6();
void sprites_carregar_7();
void sprites_carregar_8();
void sprites_carregar_9();
void sprites_carregar_10();
void sprites_carregar_11();
void sprites_carregar_12();
void sprites_carregar_13();
void sprites_carregar_14();
void sprites_carregar_15();
void sprites_carregar_16();

void sprites_carregar();

/**
 * @brief Loads fruit cards (sprites) for the specified fruit type.
 * 
 * @param fruta Enum value representing the fruit type
 */
void carregar_cartas_fruta(Fruta fruta);

/**
 * @brief Frees all common sprites previously loaded.
 */
void sprites_liberar();

/**
 * @brief Frees memory used by the fruit cards of the given type.
 * 
 * @param fruta Enum value representing the fruit type
 */
void libertar_cartas_fruta(Fruta fruta);

/**
 * @brief Draws the main menu screen.
 */
void desenhar_menu();

/**
 * @brief Draws the instructions/help screen.
 */
void desenhar_instrucoes();

/**
 * @brief Delays the program for approximately half a second.
 * 
 * @return int 0 on success, non-zero on failure
 */
int espera_meio_segundo();

/**
 * @brief Checks whether the mouse cursor is over a horizontal line of letters.
 * 
 * @param x_start X coordinate where the line starts
 * @param y Y coordinate of the line
 * @param letter_count Number of letters in the line
 * @param spacing Horizontal spacing between letters
 * @param letter_width Width of each letter
 * @param letter_height Height of each letter
 * @return true If the cursor is over the line
 * @return false Otherwise
 */
bool is_cursor_over_line(int x_start, int y, int letter_count, int spacing, int letter_width, int letter_height);

/**
 * @brief Draws a line of letters (sprites).
 * 
 * @param letters Array of letter sprites
 * @param count Number of letters to draw
 * @param x_start Starting X coordinate
 * @param y Y coordinate
 * @param spacing Horizontal spacing between letters
 */
void draw_line(Sprite **letters, int count, int x_start, int y, int spacing);

/**
 * @brief Draws a single letter sprite, optionally with a highlight.
 * 
 * @param letter Pointer to the sprite representing the letter
 * @param x X coordinate
 * @param y Y coordinate
 * @param highlight Whether to highlight the letter
 */
void draw_letter_with_highlight(Sprite *letter, int x, int y, bool highlight);

/**
 * @brief Draws a timer with the given time in seconds.
 * 
 * @param seconds Time to display (in seconds)
 * @param x X coordinate for drawing the timer
 * @param y Y coordinate for drawing the timer
 */
void desenhar_timer(uint32_t seconds, int x, int y);

void desenhar_parabens(int tempo_segundos);

#endif // SPRITES_H

