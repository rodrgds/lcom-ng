#include "graphics.h"

extern uint8_t scancd;

uint8_t* frame_buff;

unsigned int frame_sz;

vbe_mode_info_t mode_inf;

int (set_graphics)(uint16_t submode){
    reg86_t reg86;

    memset(&reg86,0, sizeof(reg86));
    reg86.intno=0x10;
    reg86.ah=0x4F;
    reg86.al=0x02;

    reg86.bx=BIT(14)| submode;
    if(sys_int86(&reg86)!=0){
        printf("Error in graphic setting\n");
        return 1;

    }
    return 0;
}

int (set_frame_buffer)(uint16_t mode){
    memset(&mode_inf, 0, sizeof(mode_inf));

    if(vbe_get_mode_info(mode, &mode_inf)){ 
        return 1;
    }

    unsigned int bytes_per_pix = (mode_inf.BitsPerPixel + 7)/8;
    frame_sz = bytes_per_pix * mode_inf.XResolution * mode_inf.YResolution;

    struct minix_mem_range physical_ads;
    physical_ads.mr_base = mode_inf.PhysBasePtr;
    physical_ads.mr_limit = physical_ads.mr_base + frame_sz; // Reserve space for three buffers

    if(sys_privctl(SELF, SYS_PRIV_ADD_MEM, &physical_ads)){
        printf("ERROR in physical mem allocation\n");
        return 1;
    }

    frame_buff = vm_map_phys(SELF, (void*) physical_ads.mr_base, frame_sz);
    if(frame_buff == NULL){
        printf("ERROR in virtual mem allocation\n");
        return 1;
    }
    return 0;
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color){
    if (x >= mode_inf.XResolution || y >= mode_inf.YResolution) return 1;

    unsigned byte_per_pixel = (mode_inf.BitsPerPixel + 7)/8;
    unsigned int indx = byte_per_pixel * (mode_inf.XResolution * y + x);
    uint8_t *pixel = frame_buff + indx;
    memcpy(pixel, &color, byte_per_pixel);

    return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    for(unsigned i = 0; i < len; i++){
        if(vg_draw_pixel(x + i, y, color) != 0){
            return 1;
        }
    }
    return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
    for(unsigned i = 0; i < height; i++){
        if(vg_draw_hline(x, y + i, width, color) != 0){
            return 1;
        }
    }
    return 0;
}

int (normalize_color)(uint32_t color, uint32_t *new_color){
    if(mode_inf.BitsPerPixel == 32){
        *new_color = color;
    }
    else{
        *new_color = color & (BIT(mode_inf.BitsPerPixel) - 1);
    }
    return 0;
}

int (waiting_ESC)(){
    int ipc_status;
    message msg;
    uint8_t kb_mask;

    if(keyboard_subscribe(&kb_mask) != 0) return 1;

    while(scancd != 0x81){
        if(driver_receive(ANY, &msg, &ipc_status) != 0){
            printf("failure in driver_receive");
            continue;
        }
        if(is_ipc_notify(ipc_status)){
            switch(_ENDPOINT_P(msg.m_source)){
                case HARDWARE:
                    if(msg.m_notify.interrupts & kb_mask){ kbc_ih();
                    break;}
                default:
                    break;
            }
        }
    }
    if(keyboard_unsubscribe() != 0) return 1;
    return 0;
}

uint32_t (R)(uint32_t first){
    uint32_t res;
    res = (first >> mode_inf.RedFieldPosition) & ((1 << mode_inf.RedMaskSize) - 1);
    return res;
}

uint32_t (G)(uint32_t first){
    uint32_t res;
    res = (first >> mode_inf.GreenFieldPosition) & ((1 << mode_inf.GreenMaskSize) - 1);
    return res;
}

uint32_t (B)(uint32_t first){
    uint32_t res;
    res = (first >> mode_inf.BlueFieldPosition) & ((1 << mode_inf.BlueMaskSize) - 1);
    return res;
}

uint32_t (Red)(unsigned j, uint8_t step, uint32_t first){
    return (R(first) + j * step) % (1 << mode_inf.RedMaskSize);
}

uint32_t (Green)(unsigned i, uint8_t step, uint32_t first){
    return (G(first) + i * step) % (1 << mode_inf.GreenMaskSize);
}

uint32_t (Blue)(unsigned j, unsigned i, uint8_t step, uint32_t first){
    return (B(first) + (i + j) * step) % (1 << mode_inf.BlueMaskSize);
}

uint32_t (direct_mode)(uint32_t R, uint32_t G, uint32_t B){
    return (R << mode_inf.RedFieldPosition) | (G << mode_inf.GreenFieldPosition) | (B << mode_inf.BlueFieldPosition);
}

uint32_t (indexed_mode)(uint16_t colum, uint16_t row, uint16_t step, uint32_t first, uint8_t n){
    return (first + (row * n + colum) * step) % (1 << mode_inf.BitsPerPixel);
}

int (print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y){
    xpm_image_t img;
    uint8_t *clrs = xpm_load(xpm, XPM_INDEXED, &img);

    for(int height = 0; height < img.height; height++){
        for(int width = 0; width < img.width; width++){
            if(vg_draw_pixel(x + width, y + height, *clrs) != 0) return 1;
            clrs++;
        }
    }
    return 0;
}
