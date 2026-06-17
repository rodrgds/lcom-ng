#ifndef NINJIX_PLATFORM_H
#define NINJIX_PLATFORM_H

#include <lcom/lcom.h>

#include <stdbool.h>
#include <stdint.h>

#ifndef BIT
#define BIT(n) (1U << (n))
#endif

#ifndef OK
#define OK 0
#endif

typedef const char *xpm_row_t;
typedef const xpm_row_t *xpm_map_t;

struct packet {
  uint8_t bytes[3];
  bool lb;
  bool rb;
  bool mb;
  bool x_ov;
  bool y_ov;
  int16_t delta_x;
  int16_t delta_y;
};

enum {
  XPM_8_8_8_8 = 1
};

static inline uint32_t xpm_transparency_color(int type) {
  (void)type;
  return 0;
}

#endif
