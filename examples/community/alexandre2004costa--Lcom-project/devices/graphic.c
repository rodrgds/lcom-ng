#include "graphic.h"
#include <stdio.h>
#include <math.h>

/** @defgroup graphic graphic
 * @{
 */

/// Indicates which buffer is in use
int bufferInUse = 1;

/// Pointer to the first video buffer
static uint8_t *video_buff; 

/// Pointer to the second video buffer
static uint8_t *video_buff2;

/// Structure to hold VBE mode information
vbe_mode_info_t modeInfos;

/// Number of bytes per pixel
static uint8_t byts_per_pixel;

/**
 * @brief Changes the graphic mode
 * @param mode Graphic mode to be set
 * @return 0 on success, 1 otherwise
 */
int (change_graphic_mode)(uint16_t mode){
    reg86_t r;
    memset(&r, 0, sizeof(r));
    r.intno = 0x10;
    r.ah = 0x4f;
    r.al = 0x02;
    r.bx = mode | BIT(14); 
    if(sys_int86(&r) != 0) {
        printf("graphic fail");
        return 1;
    }
    return 0;
}

/**
 * @brief Maps the video memory
 * @param mode Graphic mode for which memory is mapped
 * @return 0 on success, 1 otherwise
 */
int (map_memory)(uint16_t mode){
    if (vbe_get_mode_info(mode, &modeInfos) != 0) return 1;    
    struct minix_mem_range mr;
    struct minix_mem_range mr2;
    byts_per_pixel = (modeInfos.BitsPerPixel + 7) / 8;
    unsigned int vram_base = modeInfos.PhysBasePtr;
    unsigned int vram_size =  byts_per_pixel *  modeInfos.XResolution * modeInfos.YResolution;  
    int r;
    mr.mr_base = (phys_bytes) vram_base;    
    mr.mr_limit = mr.mr_base + 2 * vram_size;  
    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))){
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
        return 1;
    } 
    mr2.mr_base = (phys_bytes)(modeInfos.PhysBasePtr + vram_size); 
    video_buff = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
    video_buff2 = vm_map_phys(SELF, (void *)mr2.mr_base, vram_size);
    if(video_buff == MAP_FAILED || video_buff2 == MAP_FAILED){
        panic("couldn't map video memory");
        return 1;
    }
    return 0; 
}

/**
 * @brief Sets a pixel color at given coordinates
 * @param x X coordinate of the pixel
 * @param y Y coordinate of the pixel
 * @param color Color to set the pixel to
 * @return 0 on success, 1 otherwise
 */
int (pixel_color)(uint16_t x, uint16_t y, uint32_t color){
    if (x < 0 || y < 0 || x > modeInfos.XResolution || y > modeInfos.YResolution) return 0;
    uint32_t index = (modeInfos.XResolution * y + x) * byts_per_pixel;
    if (color == 0x123456) return 0;
    if (bufferInUse == 1) memcpy(&video_buff2[index], &color, byts_per_pixel);
    else memcpy(&video_buff[index], &color, byts_per_pixel);
    return 0;
}

/**
 * @brief Fills the background with a given color
 * @param cor Pointer to the color array
 */
void (fillBackground)(uint32_t *cor){
    if (bufferInUse == 1) memcpy(video_buff2, cor, byts_per_pixel * modeInfos.XResolution * modeInfos.YResolution);
    else memcpy(video_buff, cor, byts_per_pixel * modeInfos.XResolution * modeInfos.YResolution);
}

/**
 * @brief Rotates the buffer
 * @return 0 on success, 1 otherwise
 */
int (rotateBuffer)(){
    reg86_t command;
    memset(&command, 0, sizeof(command));
    // No need to call Vbe to get the display start, because bufferInUse already controls that
    if (bufferInUse == 1){ // First Buffer is active
         command.dx = modeInfos.YResolution;
         bufferInUse = 2;
    }else{
        command.dx = 0;
        bufferInUse = 1;
    }
    // Command to change the display start, so to change the used buffer
    command.intno = 0x10;
    command.ax = 0x4f07;
    command.bx = 0x00;
    if (sys_int86(&command) != 0) return 1;
    return 0;
}

/**
 * @brief Frees the allocated video buffers
 */
void (freeVideoBuffers)(){
    size_t size = (size_t)byts_per_pixel * modeInfos.XResolution * modeInfos.YResolution;
    vm_unmap_phys(SELF, video_buff, size);
    vm_unmap_phys(SELF, video_buff2, size);
}

/**@}*/
