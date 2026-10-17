#include "graphic.h"

#include <stdint.h>
#include <stdio.h>
#include <lcom/lcf.h>

int set_graphic_mode(uint16_t submode) {
    reg86_t reg86;                      // Initialize register structure
    memset(&reg86, 0, sizeof(reg86));   // Set all fields to 0
    reg86.intno = 0x10;                 // BIOS video interrupt
    reg86.ah = 0x4F;                    // VBE function
    reg86.al = 0x02;                    // Set mode
    reg86.bx = submode | BIT(14);       // Enable linear framebuffer (bit 14)

    if (sys_int86(&reg86) != 0) {   // Call interrupt
        printf("Failed to set graphics mode\n");
        return 1;
    }

    return 0;
}

// Exit graphics mode, return to text mode
int set_text_mode() {
    return vg_exit(); 
}

// Clear mode_info
int set_frame_buffer(uint16_t mode) {
    memset(&mode_info, 0, sizeof(mode_info)); 
  
    // Get VBE mode info
    if (vbe_get_mode_info(mode, &mode_info) != 0) {
      printf("Erro ao obter informações do modo gráfico.\n");
      return 1;
    }
    // Round up bytes per pixel
    unsigned int bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8; 
  
    frame_size = mode_info.XResolution * mode_info.YResolution * bytes_per_pixel;
  
    struct minix_mem_range mem;

    // Start address of video memory
    mem.mr_base = mode_info.PhysBasePtr;
    // End address of video memory
    mem.mr_limit = mem.mr_base + frame_size;
    
     // Allocate RAM for off-screen buffer (double buffering)
    shadow_buffer = malloc(frame_size); 
    if (shadow_buffer == NULL) {
      printf("Erro ao alocar shadow buffer.\n");
      return 1;
    }
    // Request permission to access video memory
    if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mem) != 0) { 
      printf("Erro ao alocar memória física.\n");
      return 1;
    }
  
    // Map physical video memory to virtual address space
    frame_buffer = vm_map_phys(SELF, (void*) mem.mr_base, frame_size);
    if (frame_buffer == NULL) {
      printf("Erro ao mapear memória virtual.\n");
      return 1;
    }
  
    return 0;
}

int normalize_color(uint32_t color, uint32_t *nova_cor) {
    if (mode_info.BitsPerPixel == 32) { 
      // If using 32-bit color mode, use the color directly
      *nova_cor = color;
    } else {
      // Otherwise, mask the color to fit the number of supported bits
      *nova_cor = color & (BIT(mode_info.BitsPerPixel) - 1); 
    }
    return 0;
}

int my_vg_draw_rectangle(uint16_t x, uint16_t y, uint16_t largura, uint16_t altura, uint32_t cor) {
    for (unsigned i = 0; i < altura; i++) {
        if (my_vg_draw_hline(x, y + i, largura, cor)) {    // Desenha uma linha horizontal em cada linha do retângulo
            vg_exit();
            return 1;
        }
    }
    return 0;
}

int my_vg_draw_hline(uint16_t x, uint16_t y, uint16_t comprimento, uint32_t cor) {
    for (unsigned i = 0; i < comprimento; i++) {
      if (my_vg_draw_pixel(x + i, y, cor)) return 1;    // Desenha um pixel por vez ao longo da linha
    }
    return 0;
}
  
int my_vg_draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
    if (x >= mode_info.XResolution || y >= mode_info.YResolution) return 1; //Coordenadas dentro dos limites
    unsigned bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;            //Numero de bytes por pixel

    unsigned int index = (mode_info.XResolution * y + x) * bytes_per_pixel; //Calcula a posição do pixel

    for (unsigned i = 0; i < bytes_per_pixel; i++) {                        //Copia tudo pro buffer temporario em RAM
      shadow_buffer[index + i] = (color >> (8 * i)) & 0xFF;
    }

    return 0;
}


uint32_t (direct_mode)(uint32_t R, uint32_t G, uint32_t B) {
  uint32_t pixel = 0;

  pixel |= (R << mode_info.RedFieldPosition);
  pixel |= (G << mode_info.GreenFieldPosition);
  pixel |= (B << mode_info.BlueFieldPosition);

  return pixel;
}

uint32_t (indexed_mode)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t n) {
  uint32_t index = row * n + col;                                           // Índice linear do retângulo na grelha
  uint32_t color = (first + index * step) % (1 << mode_info.BitsPerPixel);  // Coloca a cor nos bits disponíveis

  return color;
}

uint32_t get_red_component(unsigned col, uint8_t step, uint32_t first) {
  uint32_t r_first = (first >> mode_info.RedFieldPosition) & ((1 << mode_info.RedMaskSize) - 1);
  return (r_first + col * step) % (1 << mode_info.RedMaskSize);
}

uint32_t get_green_component(unsigned row, uint8_t step, uint32_t first) {
  uint32_t g_first = (first >> mode_info.GreenFieldPosition) & ((1 << mode_info.GreenMaskSize) - 1);
  return (g_first + row * step) % (1 << mode_info.GreenMaskSize);
}

uint32_t get_blue_component(unsigned col, unsigned row, uint8_t step, uint32_t first) {
  uint32_t b_first = (first >> mode_info.BlueFieldPosition) & ((1 << mode_info.BlueMaskSize) - 1);
  return (b_first + (row + col) * step) % (1 << mode_info.BlueMaskSize);
}

int (draw_xpm_image)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  xpm_image_t img;
  
  // Carrega a imagem XPM no formato indexado
  uint8_t *colors = xpm_load(xpm, XPM_INDEXED, &img);
  if (colors == NULL) return 1;

  // Desenha píxel a píxel a imagem
  for (int row = 0; row < img.height; row++) {
    for (int col = 0; col < img.width; col++) {
      if (my_vg_draw_pixel(x + col, y + row, *colors) != 0) return 1;
      colors++;
    }
  }

  return 0;
}

void swap_buffers() {
    memcpy(frame_buffer, shadow_buffer, mode_info.XResolution * mode_info.YResolution * ((mode_info.BitsPerPixel + 7) / 8));
}
