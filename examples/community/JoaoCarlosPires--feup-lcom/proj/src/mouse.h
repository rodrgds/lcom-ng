#ifndef MOUSE_H
#define MOUSE_H

#include <stdbool.h>
#include <stdint.h>

#define STATUS_REG 0x64

#define MOUSE_IRQ 12

#define OUT_BUFF 0x60

#define IN_BUFF 0x60

#define PAR_ERR BIT(7)

#define TMT_ERR BIT(6)

#define AUX BIT(5)

#define INH BIT(4)

#define A2 BIT(3)

#define SYS BIT(2)

#define IBF BIT(1)

#define OBF BIT(0)

#define DELAY_US 20000

#define KBC_CMD_REG 0x64

#define KBC_READ_CMD 0x20

#define KBC_WRITE_CMD 0x60

#define KBC_WRITE_MOUSE 0xD4

#define KBC_EN_DATA 0xF4

#define KBC_DIS_DATA 0xF5

#define KBC_SET_STREAM 0xEA

#define KBC_READ_MOUSE 0xEB

#define KBC_SET_REMOTE 0xF0

#define SE_8_TO_16 0xFF00

/**
 * @brief Command Stream Function
 *
 * @param arg variable to either Enable or Disable Mouse
 */
void mouse_command_stream(uint8_t arg);

/**
 * @brief Mouse Interrupt Handler
 *
 */
void(mouse_ih)();

/**
 * @brief Function to Subscribe Mouse Interruptions
 *
 */
void(mouse_subscribe)();

/**
 * @brief Function to Unsubscribe Mouse Interruptions
 *
 */
void(mouse_unsubscribe)();

/**
 * @brief Function to Disable Mouse Interruptions
 *
 */
void disable_mouse_ih();

/**
 * @brief Function to Enable Mouse Interruptions
 *
 */
void enable_mouse_ih();

/**
 * @brief Sets the variables lb, x_mouse and y_mouse with the values of mouse's left buttom, mouse's x position and mouse's y position, respectively
 *
 * @param packets variable with the mouse packets
 * @param lb pointer to the variable to be set with mouse's left buttom state
 * @param x_mouse pointer variable corresponding to coordinate x of the mouse's position
 * @param y_mouse pointer variable corresponding to coordinate y of the mouse's position
 */
void parse(uint8_t packets[], uint8_t *lb, uint16_t *x_mouse, uint16_t *y_mouse);

#endif
