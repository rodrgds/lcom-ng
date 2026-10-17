#include "graphics.h"

#define NUM_BUFFERS 2

static int current_view_buffer = 0;
static int current_draw_buffer = 1;

int initialize_memory(uint16_t graphics_mode)
{
        memset(&mode_info, 0, sizeof(mode_info));
        if (vbe_get_mode_info(graphics_mode, &mode_info))
                return 1;

        pixel_byte_count = (mode_info.BitsPerPixel + 7) / 8;
        total_frame_buffer_size = mode_info.XResolution * mode_info.YResolution * pixel_byte_count;

        struct minix_mem_range mem_range;
        mem_range.mr_base = mode_info.PhysBasePtr;
        mem_range.mr_limit = mem_range.mr_base + (total_frame_buffer_size * NUM_BUFFERS);

        if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mem_range))
        {
                printf("Physical memory allocation error\n");
                return 1;
        }

        frame_buffer = vm_map_phys(SELF, (void *)mem_range.mr_base, total_frame_buffer_size * NUM_BUFFERS);
        if (frame_buffer == NULL)
        {
                printf("Virtual memory allocation error\n");
                return 1;
        }

        return 0;
}

int switch_vbe_mode(uint16_t graphics_mode)
{
        reg86_t r;
        memset(&r, 0, sizeof(r));

        r.intno = BIOS_SETTINGS;
        r.ax = SET_VBE_MODE;
        r.bx = graphics_mode | LINEAR_MODE;

        if (sys_int86(&r) != 0)
        {
                printf("set_vbe_mode: sys_int86() failed \n");
                return 1;
        }

        return 0;
}

int change_graphics_mode(uint16_t graphics_mode)
{
        if (initialize_memory(graphics_mode))
        {
                return 1;
        }

        if (switch_vbe_mode(graphics_mode))
        {
                return 1;
        }

        current_view_buffer = 0;
        current_draw_buffer = 1;

        return 0;
}

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color)
{
        if (x >= mode_info.XResolution)
                return 1;
        if (y >= mode_info.YResolution)
                return 2;

        unsigned int buffer_offset = current_draw_buffer * total_frame_buffer_size;
        unsigned int index = buffer_offset + (mode_info.XResolution * y + x) * pixel_byte_count;

        if (memcpy(&frame_buffer[index], &color, pixel_byte_count) == NULL)
                return 1;

        return 0;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color)
{
        for (unsigned i = 0; i < len; i++)
                if (vg_draw_pixel(x + i, y, color) != 0)
                        return 1;
        return 0;
}

int(vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color)
{
        for (unsigned i = 0; i < len; i++)
                if (vg_draw_pixel(x, y + i, color) != 0)
                        return 1;
        return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
        for (unsigned i = 0; i < height; i++)
                if (vg_draw_hline(x, y + i, width, color) != 0)
                {
                        vg_exit();
                        return 1;
                }
        return 0;
}

void clearBuffer()
{
        unsigned int buffer_offset = current_draw_buffer * total_frame_buffer_size;
        memset(&frame_buffer[buffer_offset], 0xFFFFFF, total_frame_buffer_size);
}

void set_display_buffer(int buffer_index)
{
        reg86_t r;
        memset(&r, 0, sizeof(r));
        r.ax = 0x4F07;                                 // VBE function 07h: Set/Get Display Start
        r.bx = 0x0080;                                 // Wait for Vertical Retrace during set (prevents tearing)
        r.cx = 0;                                      // X offset
        r.dx = (buffer_index * mode_info.YResolution); // Y offset determines the buffer
        r.intno = 0x10;
        sys_int86(&r);
}

void swap_buffers()
{
        set_display_buffer(current_draw_buffer);

        current_view_buffer = current_draw_buffer;
        current_draw_buffer = (current_draw_buffer + 1) % NUM_BUFFERS;
}
