#ifndef LCOM_COMPAT_LIBXPM_H
#define LCOM_COMPAT_LIBXPM_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef const char *xpm_string_t;
typedef const char *const xpm_row_t;
typedef xpm_row_t *const xpm_map_t;

#define TRANSPARENCY_COLOR_1_5_5_5 0x8000
#define TRANSPARENCY_COLOR_8_8_8_8 0xFF000000
#define CHROMA_KEY_GREEN_888 0x00b140
#define CHROMA_KEY_GREEN_565 0x0588

enum xpm_image_type {
  XPM_INDEXED,
  XPM_1_5_5_5,
  XPM_5_6_5,
  XPM_8_8_8,
  XPM_8_8_8_8,
  XPM_GRAY_1_5_5_5,
  XPM_GRAY_5_6_5,
  XPM_GRAY_8_8_8,
  XPM_GRAY_8_8_8_8,
  INVALID_XPM
};

typedef struct {
  enum xpm_image_type type;
  uint16_t width;
  uint16_t height;
  size_t size;
  union { uint8_t *data; uint8_t *bytes; };
} xpm_image_t;

/* Load an in-memory XPM array. The caller supplies every row advertised by
 * the header. Rows use either the legacy "width height colors" indexed format
 * or the XPM "width height colors characters-per-pixel" format.
 *
 * Pixels are little-endian BGR, RGB555, or RGB565. LCF alpha means transparency:
 * opaque pixels have alpha zero, None has bit 15 or alpha byte 255. Formats
 * without alpha use the green chroma keys above. Gray formats use integer
 * BT.601 luma, (299*R + 587*G + 114*B)/1000. XPM_INDEXED requires numeric
 * palette entries. Converting legacy indices to RGB uses the VGA 16 colors.
 * On failure, img is cleared and its type becomes INVALID_XPM. */
uint8_t *xpm_load(xpm_map_t map, enum xpm_image_type type, xpm_image_t *img);

/* Free a successful load and clear its metadata. False for absent data. */
bool xpm_free_data(xpm_image_t *img);
uint32_t xpm_transparency_color(enum xpm_image_type type);

#endif
