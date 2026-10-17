#ifndef	_VGA_H
#define _VGA_H

//AH function returns
#define FUNC_OK			0x00
#define FUNC_CALL_FAIL	0x01
#define FUNC_NOT_SUPP	0x02
#define	FUNC_INV_VM		0x03

//AX function values
#define VBE_CONT_INFO	0x4F00
#define VBE_INFO		0x4F01
#define	VBE_SET			0x4F02

#define TEXT_MODE		0x03

#define VBE_INT			0x10

typedef enum _colorMode { direct, indexed}colorMode;

struct VbeInfoBlock {
   char VbeSignature[4];             // == "VESA"
   uint16_t VbeVersion;                 // == 0x0300 for VBE 3.0
   uint16_t OemStringPtr[2];            // isa vbeFarPtr
   uint8_t Capabilities[4];
   uint16_t VideoModePtr[2];         // isa vbeFarPtr
   uint16_t TotalMemory;             // as # of 64KB blocks
} __attribute__((packed));

int (set_pixel)(uint16_t x, uint16_t y, uint32_t color); /* Function to set pixel */

static char *video_mem; /* Process (virtual) address to which VRAM is mapped */
static unsigned h_res; /* Horizontal resolution in pixels */
static unsigned v_res; /* Vertical resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

static uint8_t RedMaskSize;
static uint8_t RedScreenMask;
static uint8_t GreenMaskSize;
static uint8_t GreenScreenMask;
static uint8_t BlueMaskSize;
static uint8_t BlueScreenMask;

static char *secBuf;

static colorMode cm;

int getHRes();
int getVRes();
int getBitsPerPixel();
colorMode getcm();

uint8_t getRSM();
uint8_t getGSM();
uint8_t getBSM();

int drawXPM(const char *xpm[], int x, int y);
int clearScreen();

int drawXPMdouble(const char *xpm[], int x, int y);
void drawNewXPM(xpm_image_t xpm, int x, int y);
int clearScreenDouble();

int (set_pixel_double)(uint16_t x, uint16_t y, uint32_t color);
int vg_draw_rectangle_double(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

void swap();

int vg_vbe_get_info(vg_vbe_contr_info_t *info_p);

#endif
