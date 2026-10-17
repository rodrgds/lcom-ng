#pragma once
// AH register values
#define AH_DEFAULT 0x4F
#define AH_BIOS 0x00

// AH return values
#define CALL_SUCCESS 0x00
#define CALL_FAIL 0X01
#define HW_ERROR 0X02
#define VIDEO_ERROR 0X03

// AL functions
#define CONTROLLER_INFO 0x00
#define MODE_INFO 0x01
#define MODE_SET 0x02


// Graphics modes (Color Model_Resolution_Bits/Pixel)
#define IDX_1024x768_8 0x105
#define DC_640x480_15 0x110
#define DC_800x600_24 0x115
#define DC_1280x1024_16 0x11A
#define DC_1152x864_32 0x14C
