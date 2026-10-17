#include <lcom/lcf.h>

#define MODE 0x14C	
#define KBC_OUT_BUF 0x60
#define WAIT_KBC 20000
#define KBC_PAR_ERR BIT(7)
#define KBC_TO_ERR BIT(6)
#define KBC_ST_REG 0x64
#define KBC_CMD_REG 0x64
#define KBC_OBF BIT(0)
#define READ_CMDB 0x20
#define WRITE_CMBD 0x60
#define UP_ARROW 0x48
#define DOWN_ARROW 0x50
#define LEFT_ARROW 0x4b
#define RIGHT_ARROW 0x4d
#define ENTER 0x1c
#define Q 0x10
#define EXIT 0x81
#define ENTITY_MOVE 25
#define TURTLE_DIVE 180
#define DELAY_HOUR 32
#define MOUSE_FPS 2
#define STARTING_CAR_SPEED 3
#define STARTING_TURTLE_SPEED 1
#define STARTING_TRUCK_SPEED 1
#define STARTING_LOG1_SPEED 1
#define STARTING_LOG2_SPEED 2
#define STARTING_LOG3_SPEED 3
#define ENABLE_MOUSE 0xF4
#define DISABLE_MOUSE 0xF5

