#include "video.h"
#include "video_macros.h"

static vbe_mode_info_t vbe_mem_info; // VBE mode info struct
static char *video_mem; // Frame buffer
static char *second_buffer; // Second frame buffer (for double buffering)
static char *triple_buffer; // Triple frame buffer (for triple buffering)
static unsigned int vram_size; // VRAM's size
static unsigned int vram_base; // VRAM's physical address

typedef enum {FIRST, SECOND, THIRD} buffer_num_t;
buffer_num_t currentBuffer = THIRD;

/**
 * @brief Function that helps go back to text mode
 */
void returnToTextMode() {
    if (vg_exit() != 0){
        panic("Error in vg_exit()\n");
    }
}

/**
 * @brief Get VRAM's X Resolution
 */
uint16_t getXResolution() {
    return vbe_mem_info.XResolution;
}


/**
 * @brief Function that gets information about a video mode.
 * @param mode video mode whose information we want to get
 */
int video_get_mode_information(uint16_t mode){
    if (vbe_get_mode_info(mode, &vbe_mem_info) != 0){
        panic("Error in vbe_get_mode_info()\n");
        returnToTextMode();
        return 1;
    }
    return 0;
}


/**
 * @brief Function that maps the video ram.
 */
int video_map_vram(){
    vram_size = video_get_vram_size();  // Size of one page
    vram_base = video_get_phys_addr();  // VRAM's physical address

    struct minix_mem_range mr;
    mr.mr_base = (phys_bytes) vram_base;
    mr.mr_limit = mr.mr_base + 3*vram_size;
    int r;

    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))){ // Allow memory mapping.
        panic("Error in video_map_vram, sys_privctl failed: %d\n", r);
        return 1;
    }

    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size); // Map memory (first buffer)
    second_buffer = vm_map_phys(SELF, (void*) (mr.mr_base + vram_size), vram_size); // Map memory (second buffer)
    triple_buffer = vm_map_phys(SELF, (void*) (mr.mr_base + (vram_size * 2)), vram_size); // Map memory (third buffer)

    if(video_mem == MAP_FAILED || second_buffer == MAP_FAILED || triple_buffer == MAP_FAILED) {
        panic("Error in video_map_vram, vm_map_phys() failed\n");
        return 1;
    }

    return 0;
}


/**
 * @brief Function that sets the desired video mode
 * @param mode video mode to be set
 */
int video_set_graphics_mode(uint16_t mode){
    reg86_t r86;
    memset(&r86, 0, sizeof(r86)); // zero the structure

    // Specify the appropriate register values
    r86.intno = VIDEO_BIOS_SERVICE; // BIOS video services
    r86.ah = VBE_CALL; // Set Video Mode function
    r86.al = VBE_SET_MODE; // mode
    r86.bx = mode | VIDEO_LINEAR_FRAMEBUF_MODE;

    // Make the BIOS call
    if(sys_int86(&r86) != OK) {
        printf("\tsys_int86() failed \n");
        return 1;
    }

    return 0;
}


/**
 * @brief Gets VRAM's physical base address
 */
unsigned int video_get_phys_addr(){
    return vbe_mem_info.PhysBasePtr;
}


/**
 * @brief Gets VRAM size
 */
unsigned int video_get_vram_size(){
    return (vbe_mem_info.XResolution * vbe_mem_info.YResolution) * ((vbe_mem_info.BitsPerPixel + 7) / 8);
}


/**
 * @brief Draws a pixel on the current back buffer
 * @param x x-coordinate of the pixel
 * @param y y-coordinate of the pixel
 * @param color Color of the pixel 
 */
int drawPixel(uint16_t x, uint16_t y, uint32_t color){
    if (color == TRANSPARENT){ 
        return 0;
    } 

    unsigned bytes_per_pixel = (vbe_mem_info.BitsPerPixel + 7) / 8;
    uint32_t bytes_pp_mask = 0;
    for (uint32_t i = 0; i < vbe_mem_info.BitsPerPixel; i++){
        bytes_pp_mask |= BIT(i);
    }

    color &= bytes_pp_mask;
    if (x >= vbe_mem_info.XResolution ||  y >= vbe_mem_info.YResolution) return 0; 

    unsigned index = (vbe_mem_info.XResolution * y + x) * bytes_per_pixel;

    switch (currentBuffer) {
    case FIRST:
        if (memcpy(&video_mem[index], &color, bytes_per_pixel) == NULL) return 1;
        break;
    case SECOND:
        if (memcpy(&second_buffer[index], &color, bytes_per_pixel) == NULL) return 1;
        break;
    case THIRD:
        if (memcpy(&triple_buffer[index], &color, bytes_per_pixel) == NULL) return 1;
        break;
    default:
        break;
    }
    return 0;
}

/**
 * @brief Draws a pixmap on the current back buffer
 * @param x x-coordinate of the starting pixel
 * @param y y-coordinate of the starting pixel
 * @param map Pixmap
 * @param pixmap_and_metadata Pixmap's metadata
 */
int vg_draw_pixmap(uint16_t x, uint16_t y, uint8_t* map, xpm_image_t pixmap_and_metadata){
    int width = pixmap_and_metadata.width;
    int height = pixmap_and_metadata.height;

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            if (drawPixel(j + x, i + y, map[j + i * width]) != 0){
                vg_exit();
                return 1;
            }
        }
    }
    return 0;
}


/**
 * @brief Clears the screen on the current back buffer
 */
void vg_clear_screen(){

    switch (currentBuffer) {
    case FIRST:
        memset(video_mem,0,vbe_mem_info.XResolution * vbe_mem_info.YResolution * ((vbe_mem_info.BitsPerPixel + 7) / 8));
        break;
    case SECOND:
        memset(second_buffer,0,vbe_mem_info.XResolution * vbe_mem_info.YResolution * ((vbe_mem_info.BitsPerPixel + 7) / 8));
        break;
    case THIRD:
        memset(triple_buffer,0,vbe_mem_info.XResolution * vbe_mem_info.YResolution * ((vbe_mem_info.BitsPerPixel + 7) / 8));
        break;
    default:
        break;
    }
}


/**
 * @brief Makes a BIOS call to set the new display start (Page flip) 
 */
int vg_page_flip(){
    reg86_t r86;
    memset(&r86, 0, sizeof(r86));	// zero the structure

    unsigned int v_res = 0;  

    switch (currentBuffer) {
        case FIRST:
            v_res = 0;
            break;
        case SECOND:
            v_res = vbe_mem_info.YResolution;
            break;
        case THIRD:
            v_res = vbe_mem_info.YResolution * 2;
            break;
        default:
            break;
    }

    // Specify the appropriate register values
    r86.intno = VIDEO_BIOS_SERVICE; // BIOS video services
    r86.ah = 0x4f; 
    r86.al = 0x07; // Set/Get display start
    r86.bh = 0x00; // Reserved and must be 00h
    r86.bl = 0x80; // Set Display CRTC Start during Vertical Retrace
    r86.cx = 0;  // Starting pixel
    r86.dx = v_res; // Starting scan line

    // Make the BIOS call
    if(sys_int86(&r86) != OK) {
        printf("\tsys_int86() failed \n");
        return 1;
    }

    switch (currentBuffer) {
        case FIRST:
            currentBuffer = THIRD;
            break;
        case SECOND:
            currentBuffer = FIRST;
            break;
        case THIRD:
            currentBuffer = SECOND;
            break;
        default:
            break;
    }

    return 0;
}


/**
 * @brief Get a buffer from a sprite
 */
void getBufferFromSprite(uint16_t height, uint16_t width, uint16_t x, uint16_t y, uint32_t *colors, char **background_buffer) {
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){

            unsigned color = colors[j + i * width];

            if (color == TRANSPARENT){ 
                continue;
            }

            uint32_t bytes_pp_mask = 0;
            for (uint32_t k = 0; k < vbe_mem_info.BitsPerPixel; k++){
                bytes_pp_mask |= BIT(k);
            }

            color &= bytes_pp_mask;

            memcpy(&(*background_buffer)[(j + i * vbe_mem_info.XResolution) * getBytesPerPixel()], &color, (vbe_mem_info.BitsPerPixel + 7) / 8);
        }
    }
}

/**
 * @brief Get the number of bytes in a pixel
 */
unsigned getBytesPerPixel() {
    return (vbe_mem_info.BitsPerPixel + 7) / 8;
}

/**
 * @brief Set the background with triple buffering
 */
int setBackgroundFromBuffer(char* background_buffer) {
    switch (currentBuffer) {
    case FIRST:
        if (memcpy(video_mem, background_buffer, video_get_vram_size()) == NULL) return 1;
        break;
    case SECOND:
        if (memcpy(second_buffer, background_buffer, video_get_vram_size()) == NULL) return 1;
        break;
    case THIRD:
        if (memcpy(triple_buffer, background_buffer, video_get_vram_size()) == NULL) return 1;
        break;
    default:
        break;
    }
    return 0;
}
