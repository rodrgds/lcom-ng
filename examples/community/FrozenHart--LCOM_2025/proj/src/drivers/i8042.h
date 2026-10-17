#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042
 * @{
 *
 * Constants for programming the i8042 Keyboard Controller.
 */

#define DELAY_US 20000  /** @brief 20 ms delay*/
#define MAX_ATTEMPTS 10 /** @brief Maximum number of attempts to read the status register */
#define KEYBOARD_IRQ 1  /** @brief Keyboard IRQ line */
#define MOUSE_IRQ 12    /** @brief Mouse IRQ line */

/* Makecodes and Breakcodes addresses */
#define KBC_ESC_BREAK_CODE 0x81 /** @brief Break code for the ESC key */
#define KBC_ESC_MAKE_CODE 0x01  /** @brief Make code for the ESC key */

/* I/O port addresses */
#define KBC_STATUS_REG 0x64 /** @brief Status register address */
#define KBC_CMD_IO 0x60     /** @brief Output buffer address and Input buffer address for Arguments of KBC commands */
#define KBC_CMD_REG 0x64    /** @brief Input buffer address for Commands to the KBC */

/* KBC Commands */
typedef enum {
  KBC_READ_CMD_BYTE = 0x20,         /** @brief Read Command Byte */
  KBC_WRITE_CMD_BYTE = 0x60,        /** @brief Write Command Byte */
  KBC_SELF_TEST = 0xAA,             /** @brief Check KBC (Self-test) */
  KBC_CHECK_KBD_INTERFACE = 0xAB,   /** @brief Check Keyboard Interface */
  KBC_DISABLE_KBD_INTERFACE = 0xAD, /** @brief Disable Keyboard Interface */
  KBC_ENABLE_KBD_INTERFACE = 0xAE,  /** @brief Enable Keyboard Interface */
  KBC_ENABLE_MOUSE_COMANDS = 0xD4,  /** @brief Enables device driver ti issue commands directly to the mouse, mouse commands */
  KBC_DISABLE_MOUSE = 0xA7,         /** @brief Disables device driver to issue commands directly to the mouse */
  KBC_ENABLE_MOUSE = 0xA8,          /** @brief Enables device driver to issue commands directly to the mouse */
  KBC_CHECK_MOUSE_INTERFACE = 0xA9, /** @brief Check Mouse Interface returns 0 if OK */
  KBC_WRITE_BYTE_MOUSE = 0xD4,      /** @brief Write byte to mouse  commands the KBC to forward its argument to the mouse without any interpretation */
  KBC_CUSTOM_CMD                    /** @brief Custom Command Byte, value to be defined by user */
} kbc_commands_t;

/* KBC Command Byte Responses */
#define KBC_SELF_TEST_OK 0x55    /** @brief Self-test passed */
#define KBC_SELF_TEST_ERROR 0xFC /** @brief Self-test failed */

/* KBC Command Byte Components */
typedef enum {
  DIS2 = BIT(5), /** disable mouse interface */
  DIS = BIT(4),  /** disable keyboard interface */
  INT2 = BIT(1), /** enable interrupt on OBF, from mouse */
  INT = BIT(0)   /** enable interrupt on OBF, from keyboard */
} kbc_command_byte_components_t;

/* KBC Status Register Bits */
#define KBC_PARITY BIT(7)  /** @brief Parity error - invalid data */
#define KBC_TIMEOUT BIT(6) /** @brief Timeout error - invalid data */
#define KBC_AUX BIT(5)     /** @brief Mouse data */
#define KBC_INH BIT(4)     /** @brief Inhibit flag: 0 if keyboard is inhibited */
#define KBC_A2 BIT(3)      /** @brief A2 input line: irrelevant for LCOM */
#define KBC_SYS BIT(2)     /** @brief System flag: irrelevant for LCOM */
#define KBC_IBF BIT(1)     /** @brief Input buffer full don’t write commands or arguments */
#define KBC_OBF BIT(0)     /** @brief Output buffer full - data available for reading */

// --------------------------------------------------------------------------------
// Keyboard Key Codes
// --------------------------------------------------------------------------------
/**
 * @brief Enum to identify keys by a unique identifier.
 */
typedef enum {
  MAKE_CODE,
  KEY_A,
  KEY_B,
  KEY_C,
  KEY_D,
  KEY_E,
  KEY_F,
  KEY_G,
  KEY_H,
  KEY_I,
  KEY_J,
  KEY_K,
  KEY_L,
  KEY_M,
  KEY_N,
  KEY_O,
  KEY_P,
  KEY_Q,
  KEY_R,
  KEY_S,
  KEY_T,
  KEY_U,
  KEY_V,
  KEY_W,
  KEY_X,
  KEY_Y,
  KEY_Z,
  KEY_1,
  KEY_2,
  KEY_3,
  KEY_4,
  KEY_5,
  KEY_6,
  KEY_7,
  KEY_8,
  KEY_9,
  KEY_0,
  KEY_SPACE,
  KEY_ENTER,
  KEY_BACKSPACE,
  KEY_TAB,
  KEY_ESCAPE,
  KEY_UP,
  KEY_DOWN,
  KEY_LEFT,
  KEY_RIGHT,
} key_identifier_t;

/**
 * @brief Structure to represent a key and its make and break codes.
 */
typedef struct {
  key_identifier_t id; /** @brief The unique identifier for the key. */
  char key;            /** @brief The character or key name. */
  uint8_t make_code;   /** @brief The make code for the key. */
  uint8_t break_code;  /** @brief The break code for the key. */
} key_code_map_t;

/**
 * @brief Map of keys to their make and break codes.
 */
static const key_code_map_t key_code_map[] = {
  {KEY_A, 'A', 0x1E, 0x9E},          /** @brief 'A' key */
  {KEY_B, 'B', 0x30, 0xB0},          /** @brief 'B' key */
  {KEY_C, 'C', 0x2E, 0xAE},          /** @brief 'C' key */
  {KEY_D, 'D', 0x20, 0xA0},          /** @brief 'D' key */
  {KEY_E, 'E', 0x12, 0x92},          /** @brief 'E' key */
  {KEY_F, 'F', 0x21, 0xA1},          /** @brief 'F' key */
  {KEY_G, 'G', 0x22, 0xA2},          /** @brief 'G' key */
  {KEY_H, 'H', 0x23, 0xA3},          /** @brief 'H' key */
  {KEY_I, 'I', 0x17, 0x97},          /** @brief 'I' key */
  {KEY_J, 'J', 0x24, 0xA4},          /** @brief 'J' key */
  {KEY_K, 'K', 0x25, 0xA5},          /** @brief 'K' key */
  {KEY_L, 'L', 0x26, 0xA6},          /** @brief 'L' key */
  {KEY_M, 'M', 0x32, 0xB2},          /** @brief 'M' key */
  {KEY_N, 'N', 0x31, 0xB1},          /** @brief 'N' key */
  {KEY_O, 'O', 0x18, 0x98},          /** @brief 'O' key */
  {KEY_P, 'P', 0x19, 0x99},          /** @brief 'P' key */
  {KEY_Q, 'Q', 0x10, 0x90},          /** @brief 'Q' key */
  {KEY_R, 'R', 0x13, 0x93},          /** @brief 'R' key */
  {KEY_S, 'S', 0x1F, 0x9F},          /** @brief 'S' key */
  {KEY_T, 'T', 0x14, 0x94},          /** @brief 'T' key */
  {KEY_U, 'U', 0x16, 0x96},          /** @brief 'U' key */
  {KEY_V, 'V', 0x2F, 0xAF},          /** @brief 'V' key */
  {KEY_W, 'W', 0x11, 0x91},          /** @brief 'W' key */
  {KEY_X, 'X', 0x2D, 0xAD},          /** @brief 'X' key */
  {KEY_Y, 'Y', 0x15, 0x95},          /** @brief 'Y' key */
  {KEY_Z, 'Z', 0x2C, 0xAC},          /** @brief 'Z' key */
  {KEY_1, '1', 0x02, 0x82},          /** @brief '1' key */
  {KEY_2, '2', 0x03, 0x83},          /** @brief '2' key */
  {KEY_3, '3', 0x04, 0x84},          /** @brief '3' key */
  {KEY_4, '4', 0x05, 0x85},          /** @brief '4' key */
  {KEY_5, '5', 0x06, 0x86},          /** @brief '5' key */
  {KEY_6, '6', 0x07, 0x87},          /** @brief '6' key */
  {KEY_7, '7', 0x08, 0x88},          /** @brief '7' key */
  {KEY_8, '8', 0x09, 0x89},          /** @brief '8' key */
  {KEY_9, '9', 0x0A, 0x8A},          /** @brief '9' key */
  {KEY_0, '0', 0x0B, 0x8B},          /** @brief '0' key */
  {KEY_SPACE, ' ', 0x39, 0xB9},      /** @brief Space key */
  {KEY_ENTER, '\n', 0x1C, 0x9C},     /** @brief Enter key */
  {KEY_BACKSPACE, '\b', 0x0E, 0x8E}, /** @brief Backspace key */
  {KEY_TAB, '\t', 0x0F, 0x8F},       /** @brief Tab key */
  {KEY_ESCAPE, '\033', 0x01, 0x81},  /** @brief Escape key */
  {KEY_UP, 'u', 0x48, 0xC8},         /** @brief Up arrow key */
  {KEY_DOWN, 'd', 0x50, 0xD0},       /** @brief Down arrow key */
  {KEY_LEFT, 'l', 0x4B, 0xCB},       /** @brief Left arrow key */
  {KEY_RIGHT, 'r', 0x4D, 0xCD}       /** @brief Right arrow key */
};
// --------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------
// Mouse Button Definitions
// ---------------------------------------------------------------------------------
#define MOUSE_LB BIT(0)    // Left button
#define MOUSE_RB BIT(1)    // Right button
#define MOUSE_MB BIT(2)    // Middle button
#define MOUSE_MSB_X BIT(4) // MSB of X displacement
#define MOUSE_MSB_Y BIT(5) // MSB of Y displacement
#define MOUSE_X_OVF BIT(6) // X overflow
#define MOUSE_Y_OVF BIT(7) // Y overflow

#define NEGATIVE 0xFF00 // Negative value for signed 9-bit integer

typedef enum {
  MOUSE_RESET = 0xFF,                  /** @brief Mouse reset */
  MOUSE_RESEND = 0xFE,                 /** @brief Resend, for serial communication errors*/
  MOUSE_SET_DEFAULTS = 0xF6,           /** @brief Set default values */
  MOUSE_DISABLE_DATA_REPORTING = 0xF5, /** @brief Disable data reporting, should be sent before any other command*/
  MOUSE_ENABLE_DATA_REPORTING = 0xF4,  /** @brief Enable data reporting, in stream mode only */
  MOUSE_SET_SAMPLE_RATE = 0xF3,        /** @brief Sets state sampling rate */
  MOUSE_SET_REMOTE_MODE = 0xF0,        /** @brief Sends data on request only */
  MOUSE_READ_DATA = 0xEB,              /** @brief Sends data packet request */
  MOUSE_SET_STREAM_MODE = 0xEA,        /** @brief Sends data events */
  MOUSE_STATUS_REQUEST = 0xE9,         /** @brief Gets mouse configuration (3 bytes) */
  MOUSE_SET_RESOLUTION = 0xE8,         /** @brief Sets Resolution */
  MOUSE_SET_SCALING_2_1 = 0xE7,        /** @brief Sets scaling to 2:1, Acceleration mode*/
  MOUSE_SET_SCALING_1_1 = 0xE6,        /** @brief Sets scaling to 1:1, Linear Mode */
  MOUSE_CUSTOM_CMD
} mouse_commands_t;

// Acknowledgment byte types
#define ACK 0xFA   /** @brief Everything is OK */
#define NACK 0xFE  /** @brief Invalid byte (may be because of a serial communication error) */
#define ERROR 0xFC /** @brief Second consecutive invalid byte */
// ---------------------------------------------------------------------------------
/**@}*/

#endif /* _LCOM_I8042_H */
