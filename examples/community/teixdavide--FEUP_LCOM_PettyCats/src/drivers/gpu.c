#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "gpu.h"
#include "../models/Sprite.h"

// extern Sprite characters[CHAR_COUNT];
static char *front_buffer;
static char *back_buffer;
uint8_t bypp;
vbe_mode_info_t vbe_mode_info;
uint16_t x_res, y_res;


int (enter_video_mode)(uint16_t mode) {
  reg86_t r86;
  memset(&r86, 0, sizeof(r86));

  r86.intno = INTNO;
  r86.ah = 0x4F;
  r86.al = 0x02;
  r86.bx = mode | BIT(14);

  if (sys_int86(&r86) != OK) {
    printf("enter_video_mode(): error in sys_int86()\n");
    return 1;
  }

  return 0;
}

int (exit_video_mode)() { // wrapper for vg_exit() that also frees the memory of back_buffer
  if (back_buffer != NULL) {
    free(back_buffer);
  }

  if (vg_exit() != 0) {
    printf("exit_video_mode(): error in vg_exit()\n");
    return 1;
  }

  return 0;
}

int (create_vram_buffer)(uint16_t mode) {
  if (vbe_get_mode_info(mode, &vbe_mode_info) != 0) {
    return 1;
  }

  x_res = vbe_mode_info.XResolution;
  y_res = vbe_mode_info.YResolution;

  uint8_t n_bits_per_pixel = vbe_mode_info.BitsPerPixel;

  if (n_bits_per_pixel == 15) {
    bypp = 2;
  } else {
    bypp = n_bits_per_pixel / 8;
  }

  uint32_t buffer_size = bypp * vbe_mode_info.XResolution * vbe_mode_info.YResolution;
  struct minix_mem_range buffer;
  buffer.mr_base = vbe_mode_info.PhysBasePtr;
  buffer.mr_limit = vbe_mode_info.PhysBasePtr + buffer_size;

  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &buffer) != 0) {
    return 1;
  }

  front_buffer = vm_map_phys(SELF, (void *) buffer.mr_base, buffer_size);
	back_buffer = malloc(bypp * vbe_mode_info.XResolution * vbe_mode_info.YResolution);

  if (front_buffer == NULL) {
    return 1;
  }

  return 0;
}

int (draw_pixel)(uint16_t mode, uint16_t x, uint16_t y, uint32_t color) {
  if (x < 0 || y < 0 || x > vbe_mode_info.XResolution || y > vbe_mode_info.YResolution) {
    return 0;
  }

  uint8_t bypp = 0;

  if (vbe_mode_info.BitsPerPixel == 15) {
    bypp = 2;
  } else {
    bypp = vbe_mode_info.BitsPerPixel / 8;
  }

  uint32_t buffer_index = (y * vbe_mode_info.XResolution + x) * bypp;

  if (memcpy(&back_buffer[buffer_index], &color, bypp) == NULL) {
		printf("draw_pixel(): error drawing pixel\n");
    return 1;
  }

  return 0;
}

int (draw_line)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint32_t color) {
  for (int i = 0; i < width; i++) {
    if (draw_pixel(mode, x + i, y, color) != 0) {
      return 1;
    }
  }

  return 0;
}

int (draw_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (int i = 0; i < height; i++) {
    if (draw_line(mode, x, y + i, width, color) != 0) {
      return 1;
    }
  }

  return 0;
}
 
int (set_background_color)(uint16_t mode, uint32_t color) {
  for (int x = 0; x < vbe_mode_info.XResolution; x++) {
    for (int y = 0; y < vbe_mode_info.YResolution; y++) {
      if (draw_pixel(mode, x, y, color) != 0) {
        return 1;
			}
    }
  }

  return 0;
}

int (draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  enum xpm_image_type type = XPM_INDEXED;
  xpm_image_t img;
  uint8_t *sprite = xpm_load(xpm, type, &img);

  if (sprite == NULL) {
    return 1;
  }

  for (int i = 0; i < img.height; i++) {
    for (int j = 0; j < img.width; j++) {
      if (*sprite == 255) {
        sprite++;
        continue;
      }

      if (draw_pixel(0x105, x + j, y + i, *sprite) != 0) {
        return 1;
      }

      sprite++;
    }
  }

  return 0;
}

int (draw_sprite)(Sprite sprite, uint16_t x, uint16_t y) {

  if (sprite.sprite == NULL) {
    return 1;
  }

  for (int i = 0; i < sprite.height; i++) {
    for (int j = 0; j < sprite.width; j++) {
      if (*(sprite.sprite) == 255) {
        sprite.sprite++;
        continue;
      }

      if (draw_pixel(0x105, x + j, y + i, *(sprite.sprite)) != 0) {
        return 1;
      }

      sprite.sprite++;
    }
  }

  return 0;
}


int (draw_sprite_single_color)(Sprite sprite, uint16_t x, uint16_t y, uint32_t color) {

  if (sprite.sprite == NULL) {
    return 1;
  }

  for (int i = 0; i < sprite.height; i++) {
    for (int j = 0; j < sprite.width; j++) {
      if (*(sprite.sprite) == 255) {
        sprite.sprite++;
        continue;
      }

      if (draw_pixel(0x105, x + j, y + i, color) != 0) {
        return 1;
      }

      sprite.sprite++;
    }
  }

  return 0;
}

int (get_char_sprite_idx)(char ch) {
  if (ch >= 48 && ch <= 57) {
    return ch - 47 + 26;
  }

  if (ch >= 65 && ch <= 90) {
    return ch - 64;
  }

  if (ch >= 97 && ch <= 122) {
    return ch - 96;
  }

  return 0;
}

int (draw_text)(const char *text, uint16_t x, uint16_t y, uint32_t color) {
  if (text == NULL) {
    return 1;
  }

  int length = strlen(text);
  int x_diff = 0, y_diff = 0;
  for (int i = 0; i < length; i++) {
    if (text[i] == '\n') {
      x_diff = 0;
      y_diff += character_height;
      continue;
    }

    draw_sprite_single_color(characters[get_char_sprite_idx(text[i])], x + x_diff, y + y_diff, color);
    x_diff += character_width;
  }

  return 0;
}

int (update_front_buffer)() {
	if (!memcpy(front_buffer, back_buffer, vbe_mode_info.XResolution * vbe_mode_info.YResolution * bypp)) {
		printf("update_front_buffer(): error copying memory from second buffer to main buffer\n");
		return 1;
	}

	return 0;
}
