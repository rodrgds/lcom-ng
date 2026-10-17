#ifndef CHESS_H
#define CHESS_H

#include <lcom/lcf.h>
#include "clock.h"
#include "../sprites/sprite.h"
#include "../devices/keyboard/key_map.h"

/** @defgroup chess chess
 *  @brief Functions that compose the game's logic
*/

extern Sprite *board_img;
extern Sprite *sel_img;
extern Sprite *bB; 
extern Sprite *bK;
extern Sprite *bN;
extern Sprite *bP;
extern Sprite *bQ;
extern Sprite *bR;
extern Sprite *wB;
extern Sprite *wK;
extern Sprite *wN;
extern Sprite *wP;
extern Sprite *wQ;
extern Sprite *wR;

/**
 * @brief Enumerated type for the different types of pieces
 */
typedef enum {
    EMPTY,
    KING,
    QUEEN,
    BISHOP,
    KNIGHT,
    ROOK,
    PAWN
} Type;

/**
 * @brief Enumerated type for the different colors of pieces
 */

typedef enum {
    UNDEFINED,
    WHITE,
    BLACK
} Color;

/**
 * @brief Struct for a piece
 */
typedef struct {
    Type type;
    Color color;
} Piece;

/**
 * @brief Struct for a position
 */
typedef struct {
    int row;
    int col;
} Position;

static Piece board[8][8]; /*!< Board matrix */
static bool white_turn; /*!< True if it's white's turn, false otherwise */
static bool selected; /*!< true if a piece is selected, false otherwise */
static int sel_row = -1, sel_col = -1;/*!< selected piece coordinates */
static Piece prev[8][8]; /*!< previous board matrix */
static bool game_over; /*!< true if game is over, false otherwise */ 
static int kbd_row = -1, kbd_col = -1; /*!< selected piece coordinates from keyboard */

extern uint8_t time_rtc[3]; /*!< time[hours,minutes,seconds] array of the RTC*/

/**
 * @brief Initializes the game board, setting the pieces in their initial positions
*/
void load_board();

/**
 * @brief Draws the game board background
 * @return 0 upon success, 1 otherwise
*/
int draw_board();

/**
 * @brief Draws a piece in the specified coordinates with the specified color
 * @param i Row of the piece
 * @param j Column of the piece
 * @param color Color of the piece
 * @return 0 upon success, 1 otherwise
*/
int draw_piece(int i, int j, uint32_t color);

/**
 * @brief Draws all the pieces in the board
 * @return 0 upon success, 1 otherwise
*/
int draw_pieces();

/**
 * @brief Selects a piece in the specified coordinates and sets the selected flag to true
 * @param row Row of the piece
 * @param col Column of the piece
*/
void select_piece(int row, int col);

/**
 * @brief Selects a piece in the specified coordinates
 * @param x Mouse X coordinate
 * @param y Mouse Y coordinate
*/
void mouse_select_piece(int x, int y);

/**
 * @brief Selects a piece based on the keyboard input
*/
void keyboard_select_piece();

/**
 * @brief Deselects the selected piece and sets the selected flag to false
*/
void deselect_piece();

/**
 * @brief Checks if a piece is selected
 * @return true if a piece is selected, false otherwise
*/
bool is_selected();

/**
 * @brief Gets the selected piece's position
 * @return Position of the selected piece
*/
Position get_selected();

/**
 * @brief Draws the red background for the selected piece
 * @return 0 upon success, 1 otherwise
*/
int draw_selected();

/**
 * @brief Moves the selected piece to the specified coordinates
 * @param row Row of the piece
 * @param col Column of the piece
*/
void move_piece(int row, int col);

/**
 * @brief Moves the selected piece to the specified coordinates
 * @param xf Mouse X coordinate
 * @param yf Mouse Y coordinate
*/
void mouse_move_piece(int xf, int yf);

/**
 * @brief Translates the scancode given into chess coordinates for piece selection and movement
 * @param scancode Scancode to translate
*/
void keyboard_handle_input(uint8_t scancode);

/**
 * @brief Checks if the specified move is valid
 * @param xf Row of the destination
 * @param yf Column of the destination
 * @return true if the move is valid, false otherwise
*/
bool is_valid_move(int xf, int yf);

/**
 * @brief Gets an array with all the valid moves for the selected piece
 * @param size Pointer to the size of the array, aka the number of valid moves
 * @return Array with all the valid moves for the selected piece 
*/
Position *get_valid_moves(int *size);

/**
 * @brief Gets an array with all the valid moves for the piece in the specified coordinates
 * @param size Pointer to the size of the array, aka the number of valid moves
 * @param row Row of the piece
 * @param col Column of the piece
 * @return Array with all the valid moves for the specified piece 
*/
Position *get_moves(int *size, int row, int col);

/**
 * @brief Gets the king's position
 * @return Position of the king
*/
Position get_king_position();

/**
 * @brief Checks if the king is in check
 * @return true if the king is in check, false otherwise
*/
bool is_check();

/**
 * @brief Checks if a move to the specified coordinates leaves the king in check
 * @param row Row of the destination
 * @param col Column of the destination
 * @return true if the king isn't in check after the move, false otherwise
*/
bool can_move(int row, int col);

/**
 * @brief Checks if the king is in checkmate
 * @return true if the king is in checkmate, false otherwise
*/
bool is_checkmate();

/**
 * @brief Copies the board matrix from src to dest
 * @param dest Destination board matrix
 * @param src Source board matrix
*/
void copy_board(Piece dest[8][8], Piece src[8][8]);

/**
 * @brief Undoes the last move
*/
void undo_move();

/**
 * @brief Sets the game_over flag to true and stops the clock
*/
void set_game_over();


/* Piece movement functions */

/**
 * @brief Gets an array with all the valid moves for the king
 * @param size Pointer to the size of the array, aka the number of valid moves
 * @return Array with all the valid moves for the king
*/
Position *get_valid_king_moves(int *size);

/**
 * @brief Gets an array with all the valid moves for the queen
 * @param size Pointer to the size of the array, aka the number of valid moves
 * @return Array with all the valid moves for the queen
*/
Position *get_valid_queen_moves(int *size);

/**
 * @brief Gets an array with all the valid moves for the bishop
 * @param size Pointer to the size of the array, aka the number of valid moves
 * @return Array with all the valid moves for the bishop
*/
Position *get_valid_bishop_moves(int *size);

/**
 * @brief Gets an array with all the valid moves for the knight
 * @param size Pointer to the size of the array, aka the number of valid moves
 * @return Array with all the valid moves for the knight
*/
Position *get_valid_knight_moves(int *size);

/**
 * @brief Gets an array with all the valid moves for the rook
 * @param size Pointer to the size of the array, aka the number of valid moves
 * @return Array with all the valid moves for the rook
*/
Position *get_valid_rook_moves(int *size);

/**
 * @brief Gets an array with all the valid moves for the pawn
 * @param size Pointer to the size of the array, aka the number of valid moves
 * @return Array with all the valid moves for the pawn
*/
Position *get_valid_pawn_moves(int *size);


/* Auxiliary drawing functions */

/**
 * @brief Draws the king xpm in the specified coordinates with the specified color
 * @param x X coordinate of the king
 * @param y Y coordinate of the king
 * @param color Color of the king (black/white)
 * @return 0 upon success, 1 otherwise
*/
int draw_king(int x, int y, uint32_t color);

/**
 * @brief Draws the queen xpm in the specified coordinates with the specified color
 * @param x X coordinate of the queen
 * @param y Y coordinate of the queen
 * @param color Color of the queen (black/white)
 * @return 0 upon success, 1 otherwise
*/
int draw_queen(int x, int y, uint32_t color);

/**
 * @brief Draws the bishop xpm in the specified coordinates with the specified color
 * @param x X coordinate of the bishop
 * @param y Y coordinate of the bishop
 * @param color Color of the bishop (black/white)
 * @return 0 upon success, 1 otherwise
*/
int draw_bishop(int x, int y, uint32_t color);

/**
 * @brief Draws the knight xpm in the specified coordinates with the specified color
 * @param x X coordinate of the knight
 * @param y Y coordinate of the knight
 * @param color Color of the knight (black/white)
 * @return 0 upon success, 1 otherwise
*/
int draw_knight(int x, int y, uint32_t color);

/**
 * @brief Draws the rook xpm in the specified coordinates with the specified color
 * @param x X coordinate of the rook
 * @param y Y coordinate of the rook
 * @param color Color of the rook (black/white)
 * @return 0 upon success, 1 otherwise
*/
int draw_rook(int x, int y, uint32_t color);

/**
 * @brief Draws the pawn xpm in the specified coordinates with the specified color
 * @param x X coordinate of the pawn
 * @param y Y coordinate of the pawn
 * @param color Color of the pawn (black/white)
 * @return 0 upon success, 1 otherwise
*/
int draw_pawn(int x, int y, uint32_t color);

#endif
