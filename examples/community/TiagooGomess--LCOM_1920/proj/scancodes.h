#include <lcom/lcf.h>

// break codes
#define A_BREAK 0x9E
#define B_BREAK 0xB0
#define C_BREAK 0xAE
#define D_BREAK 0xA0
#define E_BREAK 0x92
#define F_BREAK 0xA1
#define G_BREAK 0xA2
#define H_BREAK 0xA3
#define I_BREAK 0x97
#define J_BREAK 0xA4
#define K_BREAK 0xA5
#define L_BREAK 0xA6
#define M_BREAK 0xB2
#define N_BREAK 0xB1
#define O_BREAK 0x98
#define P_BREAK 0x99
#define Q_BREAK 0x90
#define R_BREAK 0x93
#define S_BREAK 0x9F
#define T_BREAK 0x94
#define U_BREAK 0x96
#define V_BREAK 0xAF
#define W_BREAK 0x91
#define X_BREAK 0xAD
#define Y_BREAK 0x95
#define Z_BREAK 0xAC

#define ENTER_BREAK 0x9C
#define BACKSPACE_BREAK 0x8E
#define SPACE_BREAK 0xB9


/**
 * @brief - Converte o break code obtido para o respetivo caracter.
 * @param scancode - Break code obtido.
 * @return - Caracter correspondente.
 */
char scancode_to_char(uint8_t scancode);

/**
 * @brief - Converte o break code obtido para o respetivo caracter, na forma de char*.
 * @param scancode - Break code obtido.
 * @return - Caracter correspondente, na forma de char*.
 */
char* scancode_to_char_pointer(uint8_t scancode);

/**
 * @brief - Verifica se o break code obtido corresponde a uma letra.
 * @param scancode - Break code obtido.
 * @return - true se for uma letra; false, caso não seja.
 */
bool is_valid_scancode(uint8_t scancode);

