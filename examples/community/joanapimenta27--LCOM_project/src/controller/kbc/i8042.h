/**
 * @file i8042.h
 * @brief Header file for the i8042 keyboard controller driver.
 */

#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define IRQ_KEYBOARD    1
#define IRQ_MOUSE       12

#define BREAK_ESC       0x81
#define MAKE_CODE       BIT(7)
#define TWO_BYTES       0xE0
#define ENABLE_INT      BIT(0)

#define PARITY_ERROR    BIT(7)
#define TIMEOUT_ERROR   BIT(6)
#define MOUSE_DATA      BIT(5)
#define FULL_IN_BUFFER  BIT(1)
#define FULL_OUT_BUFFER BIT(0)

#define KBC_STATUS_REG  0x64
#define KBC_IN_CMD      0x64
#define KBC_OUT_CMD     0x60
#define KBC_READ_CMD    0x20
#define KBC_WRITE_CMD   0x60

#define WAIT_KBC        20000
#define MAX_ATTEMPS     10

#define FIRST_BYTE           BIT(3)
#define MOUSE_RESPONSE_OK    0xFA
#define MOUSE_TESTER         0xD4
#define MOUSE_LEFT_BUTTON    BIT(0)
#define MOUSE_RIGHT_BUTTON   BIT(1)
#define MOUSE_MIDDLE_BUTTON  BIT(2)
#define MOUSE_X_DELTA        BIT(4)
#define MOUSE_Y_DELTA        BIT(5)
#define MOUSE_X_SIGNAL       BIT(6)
#define MOUSE_Y_SIGNAL       BIT(7)

#define MOUSE_SCROLL_UP   0x10
#define MOUSE_SCROLL_DOWN 0xF0

struct package {
    uint8_t bytes[3];
    bool lb;       // Left button
    bool mb;       // Middle button
    bool rb;       // Right button
    bool x_ov;     // X overflow
    bool y_ov;     // Y overflow
    int16_t delta_x;
    int16_t delta_y;
};

#endif /* _LCOM_I8042_H_ */
