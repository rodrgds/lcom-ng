#pragma once

/* Player 1: */

/* WASD */

/** @brief Make scancode for the W key (player 1 up). */
#define W_MAKE 0x11
/** @brief Break scancode for the W key (player 1 up). */
#define W_BREAK 0x91
/** @brief Make scancode for the A key (player 1 left). */
#define A_MAKE 0x1E
/** @brief Break scancode for the A key (player 1 left). */
#define A_BREAK 0x9E
/** @brief Make scancode for the S key (player 1 down). */
#define S_MAKE 0x1F
/** @brief Break scancode for the S key (player 1 down). */
#define S_BREAK 0x9F
/** @brief Make scancode for the D key (player 1 right). */
#define D_MAKE 0x20
/** @brief Break scancode for the D key (player 1 right). */
#define D_BREAK 0xA0
/** @brief Make scancode for the P key (toggle shop/pause). */
#define P_MAKE  0x19
/** @brief Break scancode for the P key (toggle shop/pause). */
#define P_BREAK 0x99


/* Player 2: */

/* Arrow Keys (extended scancodes) */

/** @brief Prefix byte that precedes extended (player 2) scancodes. */
#define EXTENDED 0xE0
/** @brief Make scancode for the Up arrow key (player 2 up). */
#define UP_MAKE 0x48
/** @brief Break scancode for the Up arrow key (player 2 up). */
#define UP_BREAK 0xC8
/** @brief Make scancode for the Left arrow key (player 2 left). */
#define LEFT_MAKE 0x4B
/** @brief Break scancode for the Left arrow key (player 2 left). */
#define LEFT_BREAK 0xCB
/** @brief Make scancode for the Right arrow key (player 2 right). */
#define RIGHT_MAKE 0x4D
/** @brief Break scancode for the Right arrow key (player 2 right). */
#define RIGHT_BREAK 0xCD
/** @brief Make scancode for the Down arrow key (player 2 down). */
#define DOWN_MAKE 0x50
/** @brief Break scancode for the Down arrow key (player 2 down). */
#define DOWN_BREAK 0xD0


/** @brief Break scancode for the Escape key (quit the application). */
#define ESC_BREAK 0x81
