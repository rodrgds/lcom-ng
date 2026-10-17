#ifndef MACHINE_LAB_LCOM_H
#define MACHINE_LAB_LCOM_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BIT
#define BIT(n) (1u << (n))
#endif

#ifndef OK
#define OK 0
#endif

typedef enum {
  LCOM_OK = 0,
  LCOM_ERR = 1,
  LCOM_TIMEOUT = 2,
  LCOM_NO_DATA = 3,
  LCOM_UNSUPPORTED = 4
} lcom_result_t;

typedef struct {
  uint8_t irq;
  uint8_t bit_no;
  uint32_t mask;
} lcom_irq_t;

typedef struct {
  uint32_t irq_mask;
  uint64_t tick;
} lcom_event_t;

typedef struct {
  uint16_t mode;
  uint16_t width;
  uint16_t height;
  uint8_t bpp;
  uint8_t bytes_per_pixel;
  uint32_t pitch;
  uintptr_t framebuffer_phys;
  size_t framebuffer_size;
} lcom_vbe_mode_info_t;

int lcom_init(void);
void lcom_exit(void);

int lcom_printf(const char *fmt, ...);

/* Host clipboard bridge. SDL uses the system clipboard; headless runs keep a
 * private clipboard per guest. At most 4091 text bytes plus a terminator.
 * Reads fail without modifying buffer when capacity is insufficient. */
int lcom_clipboard_set_text(const char *text);
int lcom_clipboard_get_text(char *buffer, size_t capacity);

int lcom_port_read8(uint16_t port, uint8_t *value);
int lcom_port_write8(uint16_t port, uint8_t value);
int lcom_port_read16(uint16_t port, uint16_t *value);
int lcom_port_write16(uint16_t port, uint16_t value);
int lcom_port_read32(uint16_t port, uint32_t *value);
int lcom_port_write32(uint16_t port, uint32_t value);

int lcom_irq_subscribe(uint8_t irq, uint32_t flags, lcom_irq_t *out);
int lcom_irq_unsubscribe(lcom_irq_t *irq);
int lcom_event_wait(lcom_event_t *event);
/* Delay in 60 Hz system ticks, retaining coalesced device notifications. */
int lcom_delay_ticks(uint64_t ticks, lcom_event_t *event);

int lcom_phys_map(uintptr_t phys, size_t len, void **out);
int lcom_phys_unmap(void *ptr, size_t len);

int lcom_vbe_get_mode_info(uint16_t mode, lcom_vbe_mode_info_t *out);
int lcom_vbe_set_mode(uint16_t mode);
int lcom_vbe_present(void);
int lcom_vbe_set_display_start(uint16_t x, uint16_t y);
/* Indexed DAC format: operation 0 sets the nearest supported width <= requested;
 * operation 1 queries. Supported widths are 6 and 8, reset to 6 on each mode set.
 * Direct-color modes return LCOM_UNSUPPORTED; invalid requests return LCOM_ERR.
 * Failure leaves output and device state unchanged. */
int lcom_vbe_dac_format(uint8_t operation, uint8_t requested, uint8_t *actual);

typedef struct {
  uint16_t bytes;
  uint16_t pixels;
  uint16_t lines;
} lcom_vbe_scanline_info_t;
/* VBE logical scanline operations: 0 sets pixels, 1 queries, 2 sets bytes.
 * Visible geometry stays unchanged. Returned lines cover the full VRAM. */
int lcom_vbe_scanline(uint8_t operation, uint16_t value, lcom_vbe_scanline_info_t *out);

#ifdef __cplusplus
}
#endif

#endif
