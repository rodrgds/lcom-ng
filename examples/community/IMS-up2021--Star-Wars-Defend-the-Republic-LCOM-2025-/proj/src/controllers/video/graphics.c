#include <lcom/lcf.h>
#include "./graphics.h"
#include <math.h>

#define BIT(n) (1 << (n))

vbe_mode_info_t mode_info; 
uint8_t *video_mem; 
uint8_t *frame_buffer;
uint8_t *double_buffer = NULL;

// set minix to graphic mode
int (set_graphic_mode)(uint16_t submode){
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86));
    reg86.intno = 0x10;
    reg86.ah = 0x4F;
    reg86.al = 0x02;
    reg86.bx = submode | BIT(14);

    if (sys_int86(&reg86) != 0) {
        printf("Set graphic mode failed\n");
        return 1;
    }
    
    return 0;
}

// Set minix to text mode
int (set_text_mode)() {
    reg86_t reg86;

    memset(&reg86, 0, sizeof(reg86));

    reg86.intno = 0x10;
    reg86.ah = 0x00;
    reg86.al = 0x03;
    reg86.bx = 0x0000;

    if (sys_int86(&reg86) != 0) {
        printf("Set text mode failed\n");
        return 1;
    }

    return 0;
}

int (set_frame_buffer)(uint16_t mode){

    memset(&mode_info, 0, sizeof(mode_info));
    if(vbe_get_mode_info(mode, &mode_info)) return 1;

    uint8_t bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;
    unsigned int frame_size = mode_info.XResolution * mode_info.YResolution * bytes_per_pixel;
    
    struct minix_mem_range physic_addresses;
    physic_addresses.mr_base = mode_info.PhysBasePtr; 
    physic_addresses.mr_limit = physic_addresses.mr_base + frame_size; 
    
    if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &physic_addresses)) {
      printf("Physical memory allocation error\n");
      return 1;
    }
  
    frame_buffer = vm_map_phys(SELF, (void*) physic_addresses.mr_base, frame_size);
    if (frame_buffer == NULL) {
      printf("Virtual memory allocation error");
      return 1;
    }

    // Allocate secondary buffer in RAM
    double_buffer = malloc(frame_size);
    if (double_buffer == NULL) {
        printf("Double buffer allocation failed\n");
        return 1;
    }
  
    return 0;
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
    if (x >= mode_info.XResolution || y >= mode_info.YResolution) return 1;
    unsigned int BytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;
    unsigned int index = (mode_info.XResolution * y + x) * BytesPerPixel;
    
    memcpy(&double_buffer[index], &color, BytesPerPixel);

    return 0;
}

int (vg_swap_buffers)() {
    unsigned int bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;
    unsigned int frame_size = mode_info.XResolution * mode_info.YResolution * bytes_per_pixel;

    if (double_buffer == NULL || frame_buffer == NULL) {
        printf("Buffer not initialized\n");
        return 1;
    }

    memcpy(frame_buffer, double_buffer, frame_size);
    return 0;
}


static int get_source_rgb(uint8_t *src_pixel_ptr, uint8_t src_bytes_per_pixel, 
                          uint8_t *r_out, uint8_t *g_out, uint8_t *b_out) {
    if (src_bytes_per_pixel == 2) { // Assuming 5:6:5 format (like XPM_5_6_5)
        uint16_t color16 = *(uint16_t*)src_pixel_ptr;
        // Standard 5:6:5 (RRRRR GGGGGG BBBBB)
        // Expand 5-bit R to 8-bit: (val * 255) / 31 or val << 3 | val >> 2
        // Expand 6-bit G to 8-bit: (val * 255) / 63 or val << 2 | val >> 4
        *r_out = (uint8_t)((((color16 >> 11) & 0x1F) * 255U) / 31U);
        *g_out = (uint8_t)((((color16 >> 5)  & 0x3F) * 255U) / 63U);
        *b_out = (uint8_t)((( color16        & 0x1F) * 255U) / 31U);
        return 0;
    } else if (src_bytes_per_pixel == 4) { // Assuming XPM_8_8_8_8 (e.g., 0xAARRGGBB or 0x00RRGGBB)
        uint32_t color32_src = *(uint32_t*)src_pixel_ptr;
        // Assuming R is in bits 16-23, G in 8-15, B in 0-7
        *r_out = (color32_src >> 16) & 0xFF;
        *g_out = (color32_src >> 8)  & 0xFF;
        *b_out =  color32_src        & 0xFF;
        // Alpha ((color32_src >> 24) & 0xFF) is ignored here for simplicity
        return 0;
    } else if (src_bytes_per_pixel == 3) { // Assuming XPM_8_8_8 (e.g., R, G, B bytes in order)
        // Order can depend on XPM library. Assuming R, G, B order.
        *r_out = src_pixel_ptr[0]; // R
        *g_out = src_pixel_ptr[1]; // G
        *b_out = src_pixel_ptr[2]; // B
        return 0;
    }
    return 1; 
}


int vg_draw_scaled_pixmap(uint8_t *pixmap_data, uint16_t original_width, uint16_t original_height, uint8_t  src_bytes_per_pixel,
    uint16_t screen_target_x, uint16_t screen_target_y, uint16_t screen_target_width, uint16_t screen_target_height) {
        
    if (!pixmap_data || original_width == 0 || original_height == 0 || 
        screen_target_width == 0 || screen_target_height == 0 || src_bytes_per_pixel == 0) {
        printf("vg_draw_scaled_pixmap: Invalid parameters.\n");
        return 1;
    }

    uint8_t framebuffer_bpp = (mode_info.BitsPerPixel + 7) / 8;
    uint32_t src_pitch_bytes = original_width * src_bytes_per_pixel;

    float x_ratio = (float)original_width / screen_target_width;
    float y_ratio = (float)original_height / screen_target_height;

    for (uint16_t ty = 0; ty < screen_target_height; ++ty) {
        uint16_t current_screen_y = screen_target_y + ty;

        // Simple vertical clipping for the target row
        if (current_screen_y >= mode_info.YResolution) {
            continue; // or break if Y is guaranteed to increase
        }

        // Calculate corresponding Y in source image (Nearest Neighbor)
        uint16_t sy = (uint16_t)floorf(ty * y_ratio);
        // Clamp sy to be within source image bounds (paranoia, ratio should handle it if target isn't 0)
        if (sy >= original_height) sy = original_height - 1;

        for (uint16_t tx = 0; tx < screen_target_width; ++tx) { // Target X on the scaled image
            uint16_t current_screen_x = screen_target_x + tx;

            // Simple horizontal clipping for the target pixel
            if (current_screen_x >= mode_info.XResolution) {
                continue; // or break if X is guaranteed to increase
            }

            // Calculate corresponding X in source image (Nearest Neighbor)
            uint16_t sx = (uint16_t)floorf(tx * x_ratio);
            // Clamp sx
            if (sx >= original_width) sx = original_width - 1;

            // Get pointer to the source pixel
            uint8_t *src_pixel_ptr = pixmap_data + (sy * src_pitch_bytes) + (sx * src_bytes_per_pixel);

            // Extract R, G, B components from source (normalized to 8-bit per channel)
            uint8_t r_src, g_src, b_src;
            if (get_source_rgb(src_pixel_ptr, src_bytes_per_pixel, &r_src, &g_src, &b_src) != 0) {
                // Error in getting source color, skip this pixel or return error
                printf("vg_draw_scaled_pixmap: Error getting source RGB at (%u,%u) from src_bpp %u\n", sx, sy, src_bytes_per_pixel);
                continue; 
            }
            
            // Pointer to destination pixel in framebuffer
            uint8_t *dest_fb_pixel_ptr = double_buffer + 
                                         (current_screen_y * mode_info.XResolution * framebuffer_bpp) +
                                         (current_screen_x * framebuffer_bpp);

            // Convert and write to framebuffer based on framebuffer's BPP
            if (mode_info.BitsPerPixel == 16) {
                uint16_t color16_fb = 0;
                // Convert 8-bit r_src, g_src, b_src to the framebuffer's 16-bit format
                color16_fb |= ((uint16_t)(r_src >> (8 - mode_info.RedMaskSize)))   << mode_info.RedFieldPosition;
                color16_fb |= ((uint16_t)(g_src >> (8 - mode_info.GreenMaskSize))) << mode_info.GreenFieldPosition;
                color16_fb |= ((uint16_t)(b_src >> (8 - mode_info.BlueMaskSize)))  << mode_info.BlueFieldPosition;
                *(uint16_t*)dest_fb_pixel_ptr = color16_fb; 
            } else {
                printf("vg_draw_scaled_pixmap: Unsupported framebuffer BitsPerPixel: %d\n", mode_info.BitsPerPixel);
                return 1; // Or skip pixel
            }
        }
    }
    return 0;
}



