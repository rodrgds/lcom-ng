#ifndef PVZ_PLATFORM_H
#define PVZ_PLATFORM_H

#include <lcom/lcom.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef const char *xpm_row_t;
typedef const xpm_row_t *xpm_map_t;

typedef struct {
    uint16_t width;
    uint16_t height;
} xpm_image_t;

enum xpm_image_type {
    XPM_8_8_8 = 1,
    XPM_INDEXED = 2
};

struct packet {
    uint8_t bytes[3];
    bool rb;
    bool mb;
    bool lb;
    int16_t delta_x;
    int16_t delta_y;
    bool x_ov;
    bool y_ov;
};

uint32_t xpm_transparency_color(enum xpm_image_type type);
uint8_t *xpm_load(xpm_map_t xpm, enum xpm_image_type type, xpm_image_t *img);

static inline int sys_outb(uint16_t port, uint32_t value) {
    return lcom_port_write8(port, (uint8_t)value) == LCOM_OK ? 0 : 1;
}

static inline int sys_inb(uint16_t port, uint32_t *value) {
    uint8_t byte = 0;
    if (value == NULL || lcom_port_read8(port, &byte) != LCOM_OK) return 1;
    *value = byte;
    return 0;
}

static inline unsigned micros_to_ticks(unsigned micros) {
    return micros;
}

static inline void tickdelay(unsigned ticks) {
    (void)ticks;
}

#endif
