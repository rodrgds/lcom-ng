#include "graphics.h"

int(set_graphic_mode)(uint16_t mode)
{
    reg86_t r86;

    memset(&r86, 0, sizeof(r86));

    r86.intno = VIDEO_SERVICES;
    r86.ax = VBE_SET_MODE;
    r86.bx = mode | BIT(14);

    return sys_int86(&r86);
}

int(set_text_mode)()
{
    reg86_t r86;

    memset(&r86, 0, sizeof(r86));

    r86.intno = VIDEO_SERVICES;
    r86.ah = 0x00;
    r86.al = TEXT_MODE;

    return sys_int86(&r86);
}

int(set_main_buffer)(uint16_t mode)
{
    struct minix_mem_range mr;

    memset(&mode_info, 0, sizeof(mode_info));
    if (vbe_get_mode_info(mode, &mode_info))
        return 1;

    yRes = mode_info.YResolution;
    xRes = mode_info.XResolution;
    bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;
    frame_size = bytes_per_pixel * xRes * yRes;

    mr.mr_base = mode_info.PhysBasePtr;
    mr.mr_limit = mr.mr_base + frame_size;

    if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) return 1;

    main_frame_buffer = vm_map_phys(SELF, (void *)mr.mr_base, frame_size);

    if (main_frame_buffer == NULL) return 1;
    return 0;
}

int(vg_draw_pixel)(int x, int y, uint32_t color)
{
    unsigned int index = (xRes * y + x) * bytes_per_pixel;
    if (x >= xRes || y >= yRes) return 0;
    if (memcpy(&drawing_frame_buffer[index], &color, bytes_per_pixel) == NULL) return 1;
    return 0;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color)
{
    for (int i = 0; i < len; i++)
    {
        if (vg_draw_pixel(x + i, y, color))
            return 1;
    }
    return 0;
}


int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    for (int i = 0; i < height; i++)
    {
        if (vg_draw_hline(x, y + i, width, color) )
            return 1;
    }
    return 0;
}

void(set_drawing_buffer)()
{
    drawing_frame_buffer = (uint8_t *)malloc(frame_size);
}

void(swap_buffers)()
{
    memcpy(main_frame_buffer, drawing_frame_buffer, frame_size);
}

void(clear_drawing_buffer)()
{
    memset(drawing_frame_buffer, 0, frame_size);
}

void(free_drawing_buffer)()
{
    free(drawing_frame_buffer);
}

