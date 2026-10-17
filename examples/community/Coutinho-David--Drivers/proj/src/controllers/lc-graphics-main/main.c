#include <lcom/lcf.h>
#include "includes/gpu.h"
#include "includes/xpm.h"
#include "includes/bitmap.h"
#include "assets/feup_xpm.h"

int main(int argc, char *argv[])
{
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need it]
    // TODO: change the path to a valid one in your system
    lcf_trace_calls("/home/lcom/labs/proj/src/trace.txt");

    // enables to save the output of //printf function calls on a file
    // [comment this out if you don't want/need it]

    // TODO: change the path to a valid one in your system
    lcf_log_output("/home/lcom/labs/proj/src/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
        return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}

typedef struct
{
    char *name;
    uint16_t mode;
} video_mode_t;

int(draw_bitmaps)()
{
    video_mode_t modes[] = {
        {"16", 0x11A},
        {"24", 0x115},
        {"32", 0x14C}};

    for (int i = 0; i < 3; i++)
    {
        char filepath[100];
        char *name = modes[i].name;
        uint32_t mode = modes[i].mode;

        uint8_t *video_mem = vg_init(mode);
        //printf("Video memory: %p\n", video_mem);

        s//printf(filepath, "/home/lcom/labs/project/assets/ribeira_%s.bmp", name);

        sprite_t *ribeira = bitmap_load(filepath, mode);

        if (ribeira == NULL)
        {
            //printf("Error loading ribeira background\n");
            return 1;
        }

        //printf("Ribeira y: %d\n", ribeira->y);
        //printf("Ribeira x: %d\n", ribeira->x);
        //printf("Ribeira width: %d\n", ribeira->width);
        //printf("Ribeira height: %d\n", ribeira->height);

        //printf("Bits per pixel Image: %d\n", ribeira->bits_per_pixel);
        //printf("Bits per pixel Mode: %d\n", get_bits_per_pixel());

        if (ribeira->bits_per_pixel != get_bits_per_pixel())
        {
            //printf("Error: bits per pixel do not match\n");
            free(ribeira->video_map);
            free(ribeira);
            vg_exit();
            return 1;
        }

        for (int y = ribeira->y; y < ribeira->y + ribeira->height; y++)
        {
            for (int x = ribeira->x; x < ribeira->x + ribeira->width; x++)
            {
                uint8_t *video_map = (uint8_t *)video_mem + (y * get_h_res() + x) * get_bits_per_pixel() / 8;

                for (int i = 0; i < get_bits_per_pixel() / 8; i++)
                {
                    video_map[i] = ribeira->video_map[(ribeira->height - 1 - (y - ribeira->y)) * ribeira->width * get_bits_per_pixel() / 8 + (x - ribeira->x) * get_bits_per_pixel() / 8 + i];
                }
            }
        }
        sleep(5);
    }

    vg_exit();

    return 0;
}

int(draw_xpms)()
{
    video_mode_t modes[] = {
        {"16", 0x11A},
        {"24", 0x115},
        {"32", 0x14C}};

    for (int i = 0; i < 3; i++)
    {
        uint32_t mode = modes[i].mode;

        uint8_t *video_mem = vg_init(mode);
        uint8_t *pixmap = NULL;

        xpm_image_t img;

        switch (mode)
        {
        case 0x11A:
            pixmap = xpm_load(feup_xpm, XPM_5_6_5, &img);
            break;
        case 0x115:
            pixmap = xpm_load(feup_xpm, XPM_8_8_8, &img);
            break;
        case 0x14C:
            pixmap = xpm_load(feup_xpm, XPM_8_8_8_8, &img);
            break;
        default:
            //printf("Error: mode not supported\n");
            break;
        }

        if (pixmap == NULL)
        {
            //printf("Error loading xpm");
            return 1;
        }

        for (int y = 0; y < img.height; y++)
        {
            for (int x = 0; x < img.width; x++)
            {
                uint8_t *video_map = (uint8_t *)video_mem + (y * get_h_res() + x) * get_bits_per_pixel() / 8;

                for (int i = 0; i < get_bits_per_pixel() / 8; i++)
                {
                    video_map[i] = pixmap[y * img.width * get_bits_per_pixel() / 8 + x * get_bits_per_pixel() / 8 + i];
                }
            }
        }

        sleep(5);
    }

    vg_exit();

    return 0;
}

int(proj_main_loop)()
{
    draw_bitmaps();

    draw_xpms();

    return 0;
}
