#include "graphics.h"

/** @defgroup graphics graphics
 * @{
 *
 * Contais tha code to control the video graphics
 */

static int border_between_numbers = 5;
static uint16_t h_res;
static uint16_t v_res;
static uint8_t bits_per_pixel, bytes;
static uint8_t mem_model;
static uint8_t *video_mem;
static uint8_t *video_mem_buffer;

static uint8_t GreenMask, RedMask, BlueMask ,posgreen, posred, posblue;
uint16_t vd_mode;

///Array with sprites of the numbers from 0 to 9
xpm_image_t numbers_sprites[10];

///Draw video buffer into video memory 
void (draw_frame)()
{
    memcpy(video_mem, video_mem_buffer, h_res*v_res*bytes);
}

///Dealocates video buffer
void (free_buffer)()
{
    //free(video_mem);
    free(video_mem_buffer);
}

///Enters video mode in the given mode
int (vg_enter)(uint16_t mode){
    vd_mode = mode;
    reg86_t r;
    memset(&r, 0, sizeof(r));
    r.ax = 0x4F02;// VBE call, function 02 -- set VBE mode//
    r.bx = 1<<14|mode; // set bit 14: linear framebuffer
    r.intno = 0x10;
    if( sys_int86(&r) != OK )
    {  
         printf("set_vbe_mode: sys_int86() failed \n");
        return 1;
    }

    return 0;
}

///Initializes video grafics variables
void* (vg_init)(uint16_t mode){
    vbe_mode_info_t vbeModeInfo;
    vbe_get_mode_info(mode, &vbeModeInfo);
    h_res = vbeModeInfo.XResolution;
    v_res = vbeModeInfo.YResolution;
    bits_per_pixel = vbeModeInfo.BitsPerPixel;
    bytes = ceil(bits_per_pixel/8);
    mem_model = vbeModeInfo.MemoryModel;
    GreenMask = vbeModeInfo.GreenMaskSize;
    RedMask = vbeModeInfo.RedMaskSize;
    BlueMask = vbeModeInfo.BlueMaskSize;
    posred = vbeModeInfo.RedFieldPosition;
    posblue =  vbeModeInfo.BlueFieldPosition;
    posgreen =  vbeModeInfo.GreenFieldPosition;

    int r;
    struct minix_mem_range mr;/* physical memory range */
    unsigned int vram_base; /* VRAM’s physical addresss */
    unsigned int vram_size; /* VRAM’s size, but you can use the frame-buffer size, instead */
    //void *video_mem; /* frame-buffer VM address */

    vram_base= vbeModeInfo.PhysBasePtr;
    vram_size = v_res * h_res * (bits_per_pixel /8);

    /* Allow memory mapping */
    mr.mr_base = (phys_bytes) vram_base;
    mr.mr_limit = mr.mr_base + vram_size;

    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    /* Map memory */
    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
    if(video_mem == MAP_FAILED) panic("couldn’t map video memory");
    video_mem_buffer = malloc(sizeof(uint8_t)*h_res*v_res*bytes);

    vg_enter(mode);


    memcpy(video_mem_buffer, video_mem, h_res*v_res*bytes);
    
    return video_mem;

}


///Turns the given x,y position pixel to the given color
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color){

    if(x >= h_res || y >= v_res)

        return 1;

    memcpy(&video_mem_buffer[((y*h_res)+x)*bytes],&color,bytes);
    
    return 0;

}

///Draw horizontal line in given x,y position with given width and color
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t width, uint32_t color) {

    for (int i = 0 ; i < width; i++)

        vg_draw_pixel(x + i, y, color);

    return 0;

}


///Draws a rectangle of given color with given size in given position
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {

    for (int i = 0 ; i < height; i++)

        vg_draw_hline(x, y + i, width, color);

    return 0;

}

///Draw a sprite in position (x,y), from XPM
int (draw_sprite_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y)
{
    xpm_image_t sprite;
    xpm_load(xpm,XPM_INDEXED,&sprite);

    for(int i = 0; i < sprite.height; i++)
    {
        for(int j = 0; j < sprite.width; j++)
        {
            if(sprite.bytes[i*sprite.width+j] != 0)
                vg_draw_pixel(x+j,y+i,sprite.bytes[i*sprite.width+j]);
        }
    }
    return 0;
}

///Draw a sprite in position (x,y), from sprite
int (draw_sprite)(xpm_image_t sprite, uint16_t x, uint16_t y)
{
    for(int i = 0; i < sprite.height; i++)
    {
        for(int j = 0; j < sprite.width; j++)
        {
            if(sprite.bytes[i*sprite.width+j] != 0)
                vg_draw_pixel(x+j,y+i,sprite.bytes[i*sprite.width+j]);
        }
    }
    return 0;
}

///Draw a sprite in position (x,y), from struct object
int (draw_object)(Object object)
{
    draw_sprite(object.sprite, object.x, object.y);
    return 0;
}


///Fill the screen in black
int (fill_bckgnd)()
{
    return vg_draw_rectangle(0,0,h_res,v_res,0);
}


///Load the number xpm into an array for better performance
void (load_numbers)()
{
    for(int i = 0; i < 10; i++)
    {
        xpm_load(getNumberXPM(i),XPM_INDEXED,&numbers_sprites[i]);
    }
}

///Draw a number into x,y position
void (draw_number)(int n, int x, int y)
{
    int numer_w = 10;//, numer_h = 12;
    int nDigits = (n == 0) ? 1 : floor(log10(abs(n))) + 1;
    //vg_draw_rectangle(x,y,(numer_w+border_between_numbers)*nDigits,numer_h,0);
    for(int i = nDigits-1; i >= 0; i--)
    {
        int digit = n % 10;
        n /= 10;
        draw_sprite(numbers_sprites[digit],x+i*(numer_w+border_between_numbers),y);
    }
}

///Clean the number drawn in the x,y position
void (clear_number)(int n, int x, int y)
{
    int numer_w = 10, numer_h = 12;
    int nDigits = (n == 0) ? 1 : floor(log10(abs(n))) + 1;
    vg_draw_rectangle(x,y,(numer_w+border_between_numbers)*nDigits,numer_h,0);
}

///Clear the text drawn in the x,y position
void (clear_text)(char* str, int x, int y)
{
    int letter_h = 10;
    for(int i = 0; str[i] != '\0'; i++)
    {
        if(getLetterXPM(str[i]) == NULL)
        {
            continue;
        }
        vg_draw_rectangle(x+i*(LETTERS_WIDTH+border_between_numbers), y, LETTERS_WIDTH+border_between_numbers, letter_h, 0);
    }
}


///Draw a text in the given position
void (draw_text)(char* str, int x, int y)
{
    //int letter_w = 11;//, letter_h = 10;
    for(int i = 0; str[i] != '\0'; i++)
    {
        if(getLetterXPM(str[i]) == NULL)
        {
            continue;
        }
        //vg_draw_rectangle(x+i*(letter_w+border_between_numbers), y, letter_w+border_between_numbers, letter_h, 0);
        draw_sprite_xpm(getLetterXPM(str[i]), x+i*(LETTERS_WIDTH+border_between_numbers), y);
    }
}
