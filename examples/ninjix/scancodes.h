/**
 * @file scancodes.h
 * @brief Keyboard scancode constants for keys used by the game.
 * @ingroup group_app
 *
 * Only the keys actually consumed by gameplay and menu input are defined.
 * This avoids pulling in a full scancode table for the few bindings needed
 * (ESC, WASD, arrows, enter, backspace, digits 1-5).
 */

#ifndef PROJ_SCANCODES_H
#define PROJ_SCANCODES_H

#define SCANCODE_ESC 0x01
#ifndef ESC_BREAK
#define ESC_BREAK 0x81
#endif
#define SCANCODE_BACKSPACE 0x0E
#define SCANCODE_W 0x11
#define SCANCODE_ENTER 0x1C
#define SCANCODE_S 0x1F
#define SCANCODE_A 0x1E
#define SCANCODE_D 0x20
#define SCANCODE_UP 0x48
#define SCANCODE_LEFT 0x4B
#define SCANCODE_RIGHT 0x4D
#define SCANCODE_DOWN 0x50
#define SCANCODE_1 0x02
#define SCANCODE_2 0x03
#define SCANCODE_3 0x04
#define SCANCODE_4 0x05
#define SCANCODE_5 0x06
#define SCANCODE_N 0x31

#endif
