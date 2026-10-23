#ifndef TEST_LCF_H
#define TEST_LCF_H

/* Host declarations for the LCF/graphics boundary; gameplay uses its real code. */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHROMA_KEY_GREEN_888 0x00b140

typedef const char *const *xpm_map_t;
enum xpm_image_type { XPM_8_8_8 };
typedef struct {
    uint16_t width, height;
} xpm_image_t;

struct vg_vbe_contr_info;
uint8_t *xpm_load(xpm_map_t xpm, enum xpm_image_type type, xpm_image_t *image);

#endif
