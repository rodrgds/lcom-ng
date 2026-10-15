#include "graphics.h"
#include "../../game/menu.h"
#include "../keyboard/keyboard.h"
#include <lcom/lcf.h>

extern uint8_t scancode;

// Variáveis globais acessíveis pelas funções de desenho
vbe_mode_info_t video_info;
uint8_t *main_buffer;
uint8_t *back_buffer = NULL;
uint16_t h_res;
uint16_t v_res;
uint8_t bytes_per_pixel;

int end_loop_ESC() {
  // loop como os usados antes

  int ipc_status;
  int r;
  uint8_t irq_set;
  message msg;

  if (keyboard_subscribe_int(&irq_set) != 0)
    return 1;

  while (scancode != ESC_BREAKCODE) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) {
            keyboard_ih();
          }
          break;
      }
    }
  }

  if (keyboard_unsubscribe_int() != 0)
    return 1;

  return 0;
}

int vg_init_graphic(uint16_t mode) {
  reg86_t reg86;

  memset(&reg86, 0, sizeof(reg86));

  reg86.ax = SET_VBE_MODE;
  reg86.bx = mode | BIT(14); // modo passado no registo bx com bit 14 ativo
  reg86.intno = 0x10;

  if (sys_int86(&reg86) != 0) {
    return 1;
  }

  return 0;
}

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  if (x >= h_res || y >= v_res)
    return 1; // ver se n esta fora do ecrã

  // calcular o index do pixel
  uint32_t index = (h_res * y + x) * bytes_per_pixel;

  // desenha o pixel na memoria na pos "index" de acordo com o numero de bytes que serao copiados (bytes_per_pixel)
  memcpy(&back_buffer[index], &color, bytes_per_pixel);

  return 0;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {

  // para cada pixel da linha, desenhar o proprio pixel
  for (unsigned i = 0; i < len; i++) {
    if (vg_draw_pixel(x + i, y, color) != 0)
      return 1;
  }

  return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {

  // desenhar o retangulo linha por linha
  for (unsigned i = 0; i < height; i++) {

    if (vg_draw_hline(x, y + i, width, color) != 0)
      return 1;
  }

  return 0;
}

int map_main_buffer(uint16_t mode) {

  // vai obter informação sobre o modo grafico
  memset(&video_info, 0, sizeof(video_info));
  if (vbe_get_mode_info(mode, &video_info) != 0) {
    return 1;
  }

  // faz as contas de quantos bytes vai guardar cada pixel
  bytes_per_pixel = (video_info.BitsPerPixel + 7) / 8;
  h_res = video_info.XResolution;
  v_res = video_info.YResolution;

  // e depois o espaçe necessario para guardar tudo isso
  uint32_t vram_size = h_res * v_res * bytes_per_pixel;
  ;

  struct minix_mem_range memory_range;
  memory_range.mr_base = video_info.PhysBasePtr;
  memory_range.mr_limit = memory_range.mr_base + vram_size;

  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &memory_range) != 0)
    return 1;

  main_buffer = vm_map_phys(SELF, (void *) memory_range.mr_base, vram_size); // mapear a virtual memory)

  if (main_buffer == NULL)
    return 1;

  return 0;
}

int normalize_color(uint32_t color, uint32_t *new_color) {

  // cor normal
  if (video_info.BitsPerPixel == 32) {
    *new_color = color;
  }

  // temos de aplicar uma mascara para garantir que a cor não exceda o numero de bits suportado
  else {

    uint32_t mask = BIT(video_info.BitsPerPixel); // ativa um bit na possição n (usada pelo modo escolhido)
    mask -= 1;                                    // subtraimos 1 para ativar apenas os n bits que vamos usar (n bits menos significativos)

    *new_color = color & mask;
  }

  return 0;
}

int calculate_size(RectangleSize *rectangle, uint8_t no_rect, uint16_t x, uint16_t y) {
  if (no_rect == 0)
    return 1; // para evitar dividir por zero

  rectangle->width = x / no_rect;
  rectangle->height = y / no_rect;

  return 0;
}

uint32_t generate_color(uint8_t row, uint8_t col, uint8_t no_rect, uint32_t first, uint8_t step) {

  if (video_info.MemoryModel == DIRECT_COLOR) {

    // caso seja modo direto temos de calcular componentes RGB
    RGBColor rgb;

    // calculo igual ao dado no lab
    rgb.r = (R(first) + col * step) % (1 << video_info.RedMaskSize);
    rgb.g = (G(first) + row * step) % (1 << video_info.GreenMaskSize);
    rgb.b = (B(first) + (col + row) * step) % (1 << video_info.BlueMaskSize);

    // organizar o rgb
    return pack_rgb(&rgb);
  }

  else {
    // modo indexado, calculo igual ao dado no lab
    return (first + (row * no_rect + col) * step) % (1 << video_info.BitsPerPixel);
  }
}

uint32_t pack_rgb(RGBColor *rgb) {

  // basicamente, vamos deslocar os bits para a sua posiçao correta de acordo com o "FieldPosition" da cor
  uint32_t packed_red = rgb->r << video_info.RedFieldPosition;
  uint32_t packed_green = rgb->g << video_info.GreenFieldPosition;
  uint32_t packed_blue = rgb->b << video_info.BlueFieldPosition;

  // depois de os bits estarem deslocados podemos simplesmente combina-los todos para formar o rbg final
  uint32_t final_rgb = packed_red | packed_green | packed_blue;

  return final_rgb;
}

int draw_pattern(RectangleSize *rectangle, uint8_t no_rect, uint32_t first, uint8_t step) {

  // para toda a tela, ir desenhando o padrao
  for (uint8_t row = 0; row < no_rect; row++) {
    for (uint8_t col = 0; col < no_rect; col++) {

      // gerar a cor usada para o respetivo retangulo
      uint32_t color = generate_color(row, col, no_rect, first, step);

      // calcular o inicio do retangulo (SE)
      uint16_t x = col * rectangle->width;
      uint16_t y = row * rectangle->height;

      // desenhar o retangulo na pos certa com a cor certa
      if (vg_draw_rectangle(x, y, rectangle->width, rectangle->height, color) != 0) {
        return 1;
      }
    }
  }

  return 0;
}

uint32_t R(uint32_t color) {

  // temos de criar uma mascara de tamanho n para guardar os bits vermelhos
  uint32_t red_mask = (1 << video_info.RedMaskSize) - 1;

  // colocar os bits vermelhos na posição certa
  uint32_t shifted_color = color >> video_info.RedFieldPosition;

  // ao fazer o "and", apenas os bits vermelhos ficam ativos
  uint32_t red_component = shifted_color & red_mask;

  return red_component;
}

uint32_t G(uint32_t color) {

  // mesma logica de cima
  uint32_t green_mask = (1 << video_info.GreenMaskSize) - 1;
  uint32_t shifted_color = color >> video_info.GreenFieldPosition;
  uint32_t green_component = shifted_color & green_mask;

  return green_component;
}

uint32_t B(uint32_t color) {

  uint32_t blue_mask = (1 << video_info.BlueMaskSize) - 1;

  // como os "bits azuis" estao nos bits menos significativos, não precisa deslocar, apenas fazer o "and" com a mascara
  uint32_t blue_component = color & blue_mask;

  return blue_component;
}

int draw_xpm(uint8_t *pixmap, xpm_image_t *img, uint16_t x, uint16_t y) {

  for (int row = 0; row < img->height; row++) {
    for (int col = 0; col < img->width; col++) {

      // primeiro desenhar o pixel
      if (vg_draw_pixel(x + col, y + row, *pixmap) != 0)
        return 1;

      // depos avançar
      pixmap++;
    }
  }

  return 0;
}

int calculate_move_amount(int speed, int target_pos, int current_pos) {
  int remaining_mov = target_pos - current_pos;

  if (speed > 0) {
    if (remaining_mov < speed) {
      return remaining_mov;
    }
    else {
      return speed;
    }
  }
  else {
    return 1;
  }
}

void graphics_exit(void) {
  reg86_t reg86;
  memset(&reg86, 0, sizeof(reg86));
  reg86.ah = 0x00; // BIOS video mode 0x03: 80x25 text mode
  reg86.al = 0x03;
  reg86.intno = 0x10; // BIOS video interrupt

  if (sys_int86(&reg86) != 0) {
    printf("video erro");
  } // Call BIOS interrupt to set video mode
}

void clear_screen() {
  memset(back_buffer, 0, h_res * v_res * bytes_per_pixel);
}

uint32_t vg_get_pixel(uint16_t x, uint16_t y) {
  if (x >= h_res || y >= v_res)
    return 0;

  uint32_t index = (h_res * y + x) * bytes_per_pixel;

  uint32_t color;
  memcpy(&color, &back_buffer[index], bytes_per_pixel);

  return color;
}

int init_double_buffer() {
  if (main_buffer == NULL)
    return 1;

  back_buffer = malloc(h_res * v_res * bytes_per_pixel);

  if (back_buffer == NULL) {
    return 1;
  }

  return 0;
}

void clear_back_buffer() {
  memset(back_buffer, 0, h_res * v_res * bytes_per_pixel);
}

void swap_buffers() {
  memcpy(main_buffer, back_buffer, h_res * v_res * bytes_per_pixel);
}

void free_double_buffer() {
  if (back_buffer != NULL) {

    free(back_buffer);
    back_buffer = NULL;
  }
}
