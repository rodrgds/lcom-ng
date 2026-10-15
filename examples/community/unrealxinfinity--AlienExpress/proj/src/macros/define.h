#ifndef DEFINE_H
#define DEFINE_H


#include <lcom/lcf.h>

#define ERROR           0xFFFF
#define BLACK           0

//timer
#define TIMER_IRQ_LINE              0
#define TIMER_STATUS                0x43
#define TIMER_0                     0x40
#define CONST_FREQ                  1193182
#define SELECT_TIMER(n)                (n << 6)
#define SELECT_RB_TIMER(n)             BIT((n) + 1)
#define TIMER_RB_CMD                (BIT(7) | BIT(6))
#define TIMER_COUNT_MODE            BIT(5)
#define TIMER_LSB_MSB               (BIT(4) | BIT(5))




//keyboard
#define KBD_HOOK_ID                 1
#define KBD_IRQ_LINE                1
#define KBC_STATUS                  0x64
#define OUT_BUFFER_BIT              BIT(0)
#define IN_BUFFER_BIT               BIT(1)
#define PARITY_ERROR                BIT(7)
#define TIME_OUT_ERROR              BIT(6)
#define OUT_BUFFER                  0x60
#define IN_BUFFER                   0x20
#define STREAMING_MODE              0xF4
#define ACK                         0xFA
#define RESEND                      0xFE
#define DELAY                       20000

#define MAKE_ESC                    0x01
#define MAKE_ENTER                  0x1C
#define MAKE_W                      0x11
#define MAKE_A                      0x1E
#define MAKE_D                      0x20
#define MAKE_S                      0x1F
#define MAKE_P                      0x19
#define MAKE_O                      0x18
#define MAKE_I                      0x17
#define MAKE_L                      0x26
#define MAKE_1                      0x02
#define MAKE_2                      0x03
#define MAKE_3                      0x04
#define B(n)                        (n | BIT(7))


//mouse
#define STREAM_MODE                 0xEA
#define DATA_REPORT                 0xF4
#define MOUSE_HOOK_ID               3
#define MOUSE_IRQ_LINE              12
#define WRITE_MOUSE_CMD             0xD4
#define ACK                         0xFA
#define MOUSE_CONTROL_BIT           BIT(3)
#define CLICK_LEFT                  BIT(0)
#define CLICK_RIGHT                 BIT(1)
#define CLICK_MIDDLE                BIT(2)
#define DELTA_X                     BIT(4)
#define DELTA_Y                     BIT(5)
#define X_OVERFLOW                  BIT(6)
#define Y_OVERFLOW                  BIT(7)
#define SET_NEGATIVE                0xFF00

//graphics
#define INTNO                       0x10
#define AL                          0x02
#define AH                          0x4F
#define BX                          BIT(14)
#define INDEXED_MODE                0x105
#define DIRECT_COLOR_1_5_5_5        0x110
#define DIRECT_COLOR_8_8_8          0x115
#define DIRECT_COLOR_5_6_5          0x11A
#define DIRECT_COLOR_8_8_8_8        0x14C


//RTC

#define RTC_HOOK_ID                 2
#define RTC_IRQ_LINE                8
#define RTC_ADRESS                  0x70
#define RTC_OUTPUT                  0x71
#define RTC_COMMAND                 11
#define RTC_COMMAND2                12
#define RTC_COMMAND3                10
#define RTC_SECONDS                 0
#define RTC_MINUTES                 2
#define RTC_HOURS                   4
#define RTC_DAYS                    7
#define RTC_MONTHS                  8
#define RTC_YEARS                   9

//entities
#define UNBREAKABLE_WALL            0xB2B2B2
#define PLAYER                      0x161516
#define ENEMY                       0x161319
#define WORKER                      0X15110F
#define PAUSE_SCREEN                0x00000D       
#define WOLF                        0xFFFFFD
#define PACKAGE                     0x000000
#define PISTOL_BULLET               0xECFFA8
#define BULLET                      0x000003
#define AMMO                        0x000006
#define PIZZA                       0x000007
#define WATCH                       0x000008
#define BOOTS                       0x000009
#define BACKPACK                    0x00000A
#define BONE                        0x00000C
#define GRENADE                     0x00000B
#define PISTOL                      0xFFFFFF
#define HEALTH                      0xFF0000
#define EMPTY_HEALTH                0x000002
#define EMPTY_INVENTORY             0x696969

#endif
