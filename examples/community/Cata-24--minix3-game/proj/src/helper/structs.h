/**
 * @file structs.h
 * @brief Data Structures that facilitates the logic of the project
 * 
 */

#ifndef _PROJ_STRUCTS_
#define _PROJ_STRUCTS_

#include <stdint.h>
#include <stdbool.h>
#include <lcom/lcf.h>

enum keys {W, A, S, D, BW, BA, BS, BD};
enum direction {UP, DOWN, LEFT, RIGHT, RDOWN, LDOWN, RUP, LUP, STOP};

/**
 * @brief saves the information about the mouse state.
 *  Contains the 3 bytes that form the mouse packet and are used to obtain the rest of theinformation. 
 */
struct mouse_packet {
    uint8_t bytes[3]; // 3 bytes from mouse packet that are in the same order that they are given
    bool lb, rb, mb, x_ovflow, y_ovflow; // information obtained through the first byte
    int16_t x_pos, y_pos; // delta X and delta Y gotten from the second and third byte respectively (16 bits are used because the positions are given with 9 bits)
};
/**
 * @brief Contains information about each sprite.
 */
struct Sprite {
    uint16_t height, width; // heigth and width of the sprite
    int x_pos; // position of the sprite in the X-axis
    int y_pos; // position of the sprite in the Y-axis
    uint32_t *colors; // array that contains the colors of every pixel in the sprite (width x height)
    int speed; // if positive represents the number of pixels per frame the sprite will move, if negative represents the number of frames needed to go through one pixel 
    enum direction dir; // indicates in which direction the sprite is moving (default is STOP) 
};
/**
 * @brief Verifies if the keys w, a, s and d are being pressed.
 */
struct track_press {
    bool w, a, s, d; // key from the keyboard, true if they are being pressed and false if they are not.
};

#endif // _PROJ_STRUCTS_
