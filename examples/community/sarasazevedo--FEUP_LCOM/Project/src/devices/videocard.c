#include <lcom/lcf.h>
#include "videocard.h"


/* Variables to store information about the videocard's current submode and associated frame-buffer*/

static uint16_t submode;    /* Graphics Submode */
static uint16_t hres;       /* XResolution of the screen */
static uint16_t vres;       /* YResolution of the screen */
static uint8_t bitsPerPixel;       /* BitsPerPixel to store color*/
static uint32_t physicalAddress;   /* PhysBasePtr */

static uint8_t redMaskSize;        /*Size of the color's red component */
static uint8_t greenMaskSize;      /*Size of the color's green component */
static uint8_t blueMaskSize;       /*Size of the color's blue component */
static uint8_t redFieldPosition;   /*Position of the color's red component*/
static uint8_t greenFieldPosition;  /*Position of the color's green component*/
static uint8_t blueFieldPosition;    /*Position of the color's blue component*/

static int bytesPerPixel;  /* No of bytes per pixel in the frame buffer*/
static int frameBufferSize;  /* Number of bytes in the frame_buffer */

static uint8_t *frame_buffer;		/* Virtual address to which VRAM is mapped */
static uint8_t *temp_buffer;    /* An extra buffer for smoother graphics */



/* ---------------- Functions to set the videocards' graphic submode, initialize global variables about the chosen Graphics Mode and to deal with the frame buffers -------------------------*/


//Initializes the videocard's global variables with information about the chosen submode
int (set_submode_info)(uint16_t mode) {
  vbe_mode_info_t mode_info;
  memset(&mode_info, 0, sizeof(mode_info));  //Initialize parameters with zero
  
  if (vbe_get_mode_info(mode, &mode_info) != 0) return 1;
  
  //Parameters from mode_info struct
  submode = mode;
  hres = mode_info.XResolution;
  vres = mode_info.YResolution;
  bitsPerPixel = mode_info.BitsPerPixel;
  physicalAddress = mode_info.PhysBasePtr;
  redMaskSize = mode_info.RedMaskSize;
  greenMaskSize = mode_info.GreenMaskSize;
  blueMaskSize = mode_info.BlueMaskSize;
  redFieldPosition = mode_info.RedFieldPosition;
  greenFieldPosition = mode_info.GreenFieldPosition;
  blueFieldPosition = mode_info.BlueFieldPosition;


  //Calculate bytesPerPixel and frameBufferSize using the previous found values
  set_bytesPerPixel();
  set_frameBufferSize();


  return 0;
}


//Initializes the videocard's frame_buffer global variable with the initial virtual address of a frame-buffer with the correct size for the chosen submode and allocates memory for the extra temporary frame buffer
int (map_video_memory)(uint16_t mode) {

  int r;
  struct minix_mem_range mr; /* physical memory range */
  memset(&mr, 0, sizeof(mr));	
  unsigned int vram_size = frameBufferSize;
  unsigned int vram_base = physicalAddress;

  /* Allow memory mapping */
  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;
  if( 0 != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) return 1;

  /* Map memory */
  frame_buffer = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  if(frame_buffer == MAP_FAILED) return 1;

  /* Allocate memory for the extra temporary frame buffer */
  temp_buffer = (uint8_t*)malloc(frameBufferSize);

  if(temp_buffer == NULL) {
    panic("couldn’t write to temp buffer");
    return 1;
  }
  return 0;

}

int vbe_vertical_retrace(uint16_t x, uint16_t y) {
  reg86_t r86;

  memset(&r86, 0, sizeof(r86)); 
  r86.ah = AH_VBE_FUNCTION; 
  r86.al = SET_DISPLAY_START_CONTROL; // VBE set/get display start
  r86.bx = submode | VBE_SET_START_VERTICAL_RETRACE;   // Set display start during vertical retrace
  r86.cx = x;      
  r86.dx = y;     
  r86.intno = INT_VIDEO_SERVICES;

  if (sys_int86(&r86) != OK) {
    return 1;
  }

  return 0;
}

//Copies the temporary frame buffer to the main frame buffer
void (swap_buffer)() {
  if (vbe_vertical_retrace(0, 0) != 0) {
    printf("Error configuring vertical retrace.\n");
  }
  memcpy((void*)frame_buffer,(void*)temp_buffer,frameBufferSize);
}


void(destroy_buffer)() {
  free(temp_buffer);
}




//Configures the videocard to work in Graphics Mode in the chosen submode using VBE Function 0x4F02 to do that, prepares the videocard's variables with information about the chosen submode and maps memory for the frame-buffers
int (vg_set_graphics_mode)(uint16_t mode) {

  //Prepares videocard global variables with information about the chosen submode and mapped frame-buffers
  if (set_submode_info(mode) != 0) return 1;
  if (map_video_memory(mode) != 0) return 1;
  
  reg86_t r86;    //Create a reg86_t variable
  
  //Initialize the struct's parameters with zero
  memset(&r86, 0, sizeof(r86));	

  //Define the struct's parameters with the correct values
  r86.intno = INT_VIDEO_SERVICES;  //INT 0X10 to use BIOS video services
  r86.ah = AH_VBE_FUNCTION;       //VBE Function "Set VBE Mode" 0x4f
  r86.al = AL_SET_VBE_MODE;       //VBE Function "Set VBE Mode" 0x02
  r86.bx = mode | LINEAR_MEMORY;  //Submode with bit 14 set to work in linear mode

  //Uses VBE Function 0x4F02 to set a VBE Mode
  if( sys_int86(&r86) != 0 ) {
    return 1;
  }
  return 0;
}




/* ----------- Getter Functions to use global variable functions in Graphics file ------------------------ */

uint16_t (get_hres)() {
  return hres;
}


uint16_t (get_vres)() {
  return vres;
}

uint16_t (get_bitsPerPixel)() {
  return bitsPerPixel;
}

//Related to color components, transforming the values to int to use with bitwise operations
int (get_redMaskSize)() {
  return (int)redMaskSize;
}

int (get_greenMaskSize)() {
  return (int)greenMaskSize;
}

int (get_blueMaskSize)() {
  return (int) blueMaskSize;
}

int (get_redFieldPosition)() {
  return (int)redFieldPosition;
}

int (get_greenFieldPosition)() {
  return (int)greenFieldPosition;
}

int (get_blueFieldPosition)() {
  return (int)blueFieldPosition;
}




/* ----------- Helper Functions that perform calculations ------------------------ */

//Find how many bytes per pixel are used for the current videocard's submode, rounding up, and set the videocard's bytespPerPixel variable
void (set_bytesPerPixel)() {

    bytesPerPixel= ((bitsPerPixel + 7) / 8);


}


//Find how many bytes the videocard's frame-buffer has and set the videocard's frameBufferSize variable
void (set_frameBufferSize)() {

  int noPixels = vres * hres;

  frameBufferSize = (noPixels * bytesPerPixel);

}



/* ----------- Helper Function to help paiting pixels ------------------------ */


//Paints pixel (x,y) with the specified color by copying the color's bytes to the pixel's memory area
int (fill_pixel)(uint16_t x, uint16_t y, uint32_t color) {

    //Verify if pixel coordinates are valid
    if (x<0 || y < 0 || x> hres || y>vres || (color ==  TRANSPARENCY_COLOR_8_8_8_8)) return 0;  //Don't paint pixels that are outside the screen boundings

    int pixel_index =  y * hres + x;  //The index of pixel (x,y) in the sequential list of pixels (in linear mode, starting with index 0)
    int pixel_offset =  pixel_index * bytesPerPixel; //Find the offset of pixel (x,y) in the frame-buffer memory

    //memcpy(void *dest, const void *src, size_t n);
    if (memcpy(&temp_buffer[pixel_offset], &color, bytesPerPixel) == NULL) return 1;

    return 0;

}

//Loads a XPM image from a XPM pixmap, with the xpm_image_type being XPM_8_8_8_8 (direct color mode with 32 bits per pixel)
xpm_image_t my_load_xpm(xpm_map_t xpm_map) {
    xpm_image_t new_xpm;

    xpm_load(xpm_map, XPM_8_8_8_8, &new_xpm);
  return new_xpm;
}
















