#include "background.h"

static char* background = NULL;

void alloc_background()
{
    background = malloc(vmi_p.XResolution * vmi_p.YResolution*(vmi_p.BitsPerPixel/8) + 7);
} 

void free_background()
{
    free(background);
}

int draw_background(bool init)
{
    //se estivermos a iniciar o background, second_buffer tem que ser igual ao background
    if(init)
    {
        //vg_read, pinta o segundo buffer com o background
        vg_read_xpm(background_xpm,0,0);
        //char* second_buffer = get_videobuff();
        memcpy(background,video_buff,vmi_p.XResolution * vmi_p.YResolution*(vmi_p.BitsPerPixel/8));
    }
    //caso estejamos a resetar o background, apenas queremos copiar o background para o second_buffer
    else
    {
        //char* second_buffer = get_videobuff();
        memcpy(video_buff,background,vmi_p.XResolution * vmi_p.YResolution*(vmi_p.BitsPerPixel/8));
    }
    return 0;
}


void reset_to_background()
{
    vg_clear_screen();
    draw_background(false);
}
