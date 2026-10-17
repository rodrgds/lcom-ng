#include "graphics.h"
#include <lcom/lcf.h>
#include <machine/int86.h>
static phys_bytes phys;
static char *video_mem;
static char *double_buffer;
static uint8_t current_page; // 0 or 1 - Used for page flipping
static unsigned x_res;
static unsigned y_res;
static unsigned bitspp; //Bits per pixel
static unsigned bytespp;
bool usePageFlipping = true;
struct minix_mem_range mr;
unsigned int vram_base; /* VRAM's physical addresss */
unsigned int vram_size; /* VRAM's size, but you can use
				    the frame-buffer size, instead */

void drawBackground() {
  for (unsigned y = 0; y < y_res * bytespp; y++) {
    for (unsigned x = 0; x < x_res * bytespp; x++) {
      vg_draw_pixel(x, y, 0xFFFFFF);
    }
  }
}

int activateGraphicsMode(uint16_t mode) {
  struct reg86 r;
  vbe_mode_info_t vbeInfo;
  memset(&r, 0, sizeof(r));
  //Get info of mode
  if (vbe_get_mode_info(mode, &vbeInfo) != OK) {
    printf("Can't get info of selected mode(%d)", mode);
    return 1;
  }
  x_res = vbeInfo.XResolution;
  y_res = vbeInfo.YResolution;
  bitspp = vbeInfo.BitsPerPixel;
  bytespp = bitspp / 8;
  phys = vbeInfo.PhysBasePtr;

  //printf("Mode = %d\n", mode);
  r.ax = VBE_FUNCTION_SET_VBE;
  r.bx = BIT(14) | mode;
  r.intno = BIOS_VIDEO_CARD;
  if (sys_int86(&r) != OK) {
    printf("Error activating graphics Mode\n");
    return 1;
  }

  /* Allow memory mapping */
  vram_base = phys;
  vram_size = y_res * x_res * bytespp * 2;
  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;
  unsigned ans;
  if (OK != (ans = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", ans);

  /* Map memory */
  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);
  double_buffer = (char *) malloc(x_res * y_res * bytespp);
  current_page = 0;
  drawBackground();
  return 0;
}

int activateTextMode() {
  return vg_exit();
}

int vg_draw_pixel(uint16_t x, uint16_t y, uint32_t color) {

  if (x < x_res && y < y_res) {
    char *vram = double_buffer + (y * x_res + x) * bytespp;
    int nBytes = bytespp;
    while (nBytes-- > 0) {
      *(vram++) = color & 0xFF;
      color = color >> 8;
    }
    return 0;
  }
  else {
    return 1;
  }
}

uint32_t draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
  if (x < x_res && y < y_res) {
    char *vram = double_buffer + (y * x_res + x) * bytespp;
    uint32_t colorDrawn;
    memcpy(&colorDrawn, vram, bytespp);
    vg_draw_pixel(x, y, color);
    printf("%x\n", colorDrawn);
    return colorDrawn;
  }
  else {
    return 0xFFFFFF;
  }
}
int is_out_of_bounds(int x, int y, int tolerance) {
  return !((-tolerance < x) && (x < (int) x_res + tolerance)) || !((-tolerance < y) && (y < (int) y_res + tolerance));
}

int drawSprite(Sprite *sprite) {
  int collided = 0;
  int x = sprite->x;
  int y = sprite->y;
  uint32_t transparentColor = 0xFFFFFF;
  for (int iy = 0; iy < sprite->image->height; iy++) {
    for (int ix = 0; ix < sprite->image->width; ix++) {
      unsigned position = (ix + iy * sprite->image->width) * bytespp;
      uint32_t color;
      memcpy(&color, &sprite->image->bytes[position], bytespp);
      if (transparentColor != color) {
        if (sprite->rotation == 0) {
          //Rotation disabled
          vg_draw_pixel(x + ix, y + iy, color);
        }
        else {
          int rotatedX, rotatedY;
          getRotationCoordinate(sprite, x + ix, y + iy, &rotatedX, &rotatedY);
          vg_draw_pixel(rotatedX, rotatedY, color);
          /*if(draw_pixel(rotatedX, rotatedY, color) != 0xFFFFFF){
              collided = 1;
              printf("Collision detected \n");
            }
          */
        }
      }
    }
  }
  return collided;
}

int drawPlayer(Player *player) {
  drawSprite(player->sprite);
  drawGun(player->gun);
  return 0;
}

int drawEnemy(Enemy *enemy) {
  drawSprite(enemy->sprite);
  drawGun(enemy->gun);
  return 0;
}

int drawGun(Gun *gun) {
  drawSprite(gun->sprite);
  return 0;
}

int drawBullet(Bullet *bullet) {
  drawSprite(bullet->sprite);
  return 0;
}

int drawString(char *str, int x, int y) {
  uint32_t transparentColor = 0xFFFFFF;
  for (int i = 0; str[i] != '\0'; i++) {
    if (str[i] != ' ') {
      letter_xpm letter = getLetterXpm(str[i]);
      for (int iy = 0; iy < letter.height; iy++) {
        for (int ix = 0; ix < letter.width; ix++) {
          int position = ((ix + letter.startX) + iy * letter.letters->width) * bytespp;
          uint32_t color;
          memcpy(&color, &letter.letters->bytes[position], bytespp);
          if (color != transparentColor) {
            vg_draw_pixel(x + ix, y + iy, color);
          }
        }
      }
    }
    x += 14;
  }
  /*
  xpm_string xpmStr = get_string_xpm(str);
  xpm_char *c = xpmStr.first;
  while (c != NULL) {
    drawChar(c, x, y);
    c = c->next;
    x += c->xpm.width;
  }
  */
  return 0;
}

/*
int drawChar(xpm_char *c, int x, int y) {
  uint32_t transparentColor = 0xFFFFFF;
  for (int iy = 0; iy < c->xpm.height; iy++) {
    for (int ix = 0; ix < c->xpm.width; ix++) {
      int position = ((ix + c->xpm.startX) + iy * c->xpm.letters->width) * bytespp;
      uint32_t color;
      memcpy(&color, &c->xpm.letters->bytes[position], bytespp);
      if (color != transparentColor) {
        vg_draw_pixel(x + ix, y + iy, color);
      }
    }
  }
  return 0;
}
*/

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (unsigned ix = 0; ix < len; ix++) {
    if (vg_draw_pixel(x + ix, y, color) != OK) {
      return 1;
    }
  }
  return 0;
}

int vg_draw_vline(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (unsigned iy = 0; iy < len; iy++) {
    if (vg_draw_pixel(x, y + iy, color) != OK) {
      return 1;
    }
  }
  return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  vg_draw_hline(x, y, width, color);
  vg_draw_hline(x, y + height, width, color);
  vg_draw_vline(x, y, height, color);
  vg_draw_vline(x + width, y, height, color);

  return 0;
}

void applyChanges() {
  if (!usePageFlipping) {
    memcpy(video_mem, double_buffer, y_res * x_res * bytespp);
  }
  else {

    struct reg86 r;
    memset(&r, 0, sizeof(r));
    r.intno = BIOS_VIDEO_CARD;
    r.ax = VBE_FUNCTION_SETGET_DISPLAY_START;
    r.dx = current_page * y_res;

    if (sys_int86(&r) != OK) {
      printf("Error page Flipping\n");
      return;
    }

    current_page ^= 1;
    double_buffer = video_mem + y_res * x_res * bytespp * current_page;
  }
  drawBackground();
}

void getResolution(int *xres, int *yres) {
  *xres = x_res;
  *yres = y_res;
}
