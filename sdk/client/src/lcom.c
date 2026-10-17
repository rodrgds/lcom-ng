#include <lcom/lcom.h>
#include <lcom/ac97.h>
#include "lcom_protocol.h"

#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static int g_fd = -1;
static uint32_t g_next_request = 1;
struct phys_mapping {
  void *user;
  void *base;
  size_t length;
  size_t mapped_length;
  struct phys_mapping *next;
};
static struct phys_mapping *g_mappings;

static int write_all(int fd, const void *buf, size_t len) {
  const uint8_t *p = (const uint8_t *)buf;
  while (len > 0) {
    ssize_t n = write(fd, p, len);
    if (n < 0) {
      if (errno == EINTR) continue;
      return -1;
    }
    if (n == 0) return -1;
    p += (size_t)n;
    len -= (size_t)n;
  }
  return 0;
}

static int read_all(int fd, void *buf, size_t len) {
  uint8_t *p = (uint8_t *)buf;
  while (len > 0) {
    ssize_t n = read(fd, p, len);
    if (n < 0) {
      if (errno == EINTR) continue;
      return -1;
    }
    if (n == 0) return -1;
    p += (size_t)n;
    len -= (size_t)n;
  }
  return 0;
}

static int send_msg(uint16_t type, uint32_t request_id, const void *payload, uint32_t size) {
  if (g_fd < 0) return -1;
  if (size > LCOM_MAX_PAYLOAD) return -1;

  lcom_msg_header_t hdr;
  hdr.type = type;
  hdr.flags = 0;
  hdr.size = size;
  hdr.request_id = request_id;

  if (write_all(g_fd, &hdr, sizeof(hdr)) != 0) return -1;
  if (size != 0 && write_all(g_fd, payload, size) != 0) return -1;
  return 0;
}

static int recv_msg(lcom_msg_header_t *hdr, void *payload, uint32_t capacity) {
  if (g_fd < 0) return -1;
  if (read_all(g_fd, hdr, sizeof(*hdr)) != 0) return -1;
  if (hdr->size > capacity || hdr->size > LCOM_MAX_PAYLOAD) return -1;
  if (hdr->size != 0 && read_all(g_fd, payload, hdr->size) != 0) return -1;
  return 0;
}

static int request_reply(uint16_t req_type,
                         const void *req,
                         uint32_t req_size,
                         uint16_t reply_type,
                         void *reply,
                         uint32_t reply_capacity,
                         uint32_t *reply_size) {
  uint32_t request_id = g_next_request++;
  if (send_msg(req_type, request_id, req, req_size) != 0) return -1;

  for (;;) {
    uint8_t buf[LCOM_MAX_PAYLOAD];
    lcom_msg_header_t hdr;
    if (recv_msg(&hdr, buf, sizeof(buf)) != 0) return -1;
    if (hdr.request_id != request_id) continue;
    if (hdr.type != reply_type) return -1;
    if (hdr.size != reply_capacity) return -1;
    memcpy(reply, buf, hdr.size);
    if (reply_size != NULL) *reply_size = hdr.size;
    return 0;
  }
}

int lcom_init(void) {
  if (g_fd >= 0) return LCOM_OK;

  const char *fd_env = getenv("MACHINE_LAB_RUN_FD");
  if (fd_env == NULL || fd_env[0] == '\0') {
    fd_env = getenv("LCOM_RUN_FD");
  }
  if (fd_env == NULL || fd_env[0] == '\0') {
    return LCOM_ERR;
  }

  char *end = NULL;
  long fd = strtol(fd_env, &end, 10);
  if (end == fd_env || fd < 0) return LCOM_ERR;

  g_fd = (int)fd;

  lcom_hello_t hello;
  hello.version = LCOM_PROTOCOL_VERSION;
  hello.pid = (uint32_t)getpid();

  lcom_hello_reply_t reply;
  if (request_reply(LCOM_MSG_HELLO, &hello, sizeof(hello), LCOM_MSG_HELLO_REPLY,
                    &reply, sizeof(reply), NULL) != 0) {
    g_fd = -1;
    return LCOM_ERR;
  }

  return reply.status == 0 && reply.version == LCOM_PROTOCOL_VERSION ? LCOM_OK : LCOM_ERR;
}

void lcom_exit(void) {
  while (g_mappings != NULL) {
    struct phys_mapping *mapping = g_mappings;
    g_mappings = mapping->next;
    munmap(mapping->base, mapping->mapped_length);
    free(mapping);
  }
  if (g_fd >= 0) {
    send_msg(LCOM_MSG_EXIT, g_next_request++, NULL, 0);
    close(g_fd);
    g_fd = -1;
  }
}

int lcom_printf(const char *fmt, ...) {
  char stack_buf[1024];
  va_list ap;
  va_start(ap, fmt);
  int n = vsnprintf(stack_buf, sizeof(stack_buf), fmt, ap);
  va_end(ap);
  if (n < 0) return LCOM_ERR;

  if (g_fd < 0) {
    fputs(stack_buf, stdout);
    return LCOM_OK;
  }

  size_t len = (size_t)n;
  if (len >= sizeof(stack_buf)) len = sizeof(stack_buf) - 1;
  if (send_msg(LCOM_MSG_CONSOLE_WRITE, 0, stack_buf, (uint32_t)len) != 0) {
    return LCOM_ERR;
  }
  return LCOM_OK;
}

int lcom_port_read8(uint16_t port, uint8_t *value) {
  if (value == NULL) return LCOM_ERR;
  lcom_port_read8_t req;
  req.port = port;
  req.reserved = 0;

  lcom_port_read8_reply_t reply;
  if (request_reply(LCOM_MSG_PORT_READ8, &req, sizeof(req), LCOM_MSG_PORT_READ8_REPLY,
                    &reply, sizeof(reply), NULL) != 0) {
    return LCOM_ERR;
  }
  if (reply.status != 0) return LCOM_ERR;
  *value = reply.value;
  return LCOM_OK;
}

int lcom_port_write8(uint16_t port, uint8_t value) {
  lcom_port_write8_t req;
  req.port = port;
  req.value = value;
  req.reserved = 0;

  lcom_status_reply_t reply;
  if (request_reply(LCOM_MSG_PORT_WRITE8, &req, sizeof(req), LCOM_MSG_STATUS,
                    &reply, sizeof(reply), NULL) != 0) {
    return LCOM_ERR;
  }
  return reply.status == 0 ? LCOM_OK : LCOM_ERR;
}

int lcom_irq_subscribe(uint8_t irq, uint32_t flags, lcom_irq_t *out) {
  if (out == NULL) return LCOM_ERR;
  lcom_irq_subscribe_t req;
  req.irq = irq;
  memset(req.reserved, 0, sizeof(req.reserved));
  req.flags = flags;

  lcom_irq_subscribe_reply_t reply;
  if (request_reply(LCOM_MSG_IRQ_SUBSCRIBE, &req, sizeof(req),
                    LCOM_MSG_IRQ_SUBSCRIBE_REPLY, &reply, sizeof(reply), NULL) != 0) {
    return LCOM_ERR;
  }
  if (reply.status != 0) return LCOM_ERR;
  out->irq = reply.irq;
  out->bit_no = reply.bit_no;
  out->mask = reply.mask;
  return LCOM_OK;
}

int lcom_irq_unsubscribe(lcom_irq_t *irq) {
  if (irq == NULL) return LCOM_ERR;
  lcom_irq_unsubscribe_t req;
  req.irq = irq->irq;
  memset(req.reserved, 0, sizeof(req.reserved));

  lcom_status_reply_t reply;
  if (request_reply(LCOM_MSG_IRQ_UNSUBSCRIBE, &req, sizeof(req), LCOM_MSG_STATUS,
                    &reply, sizeof(reply), NULL) != 0) {
    return LCOM_ERR;
  }
  if (reply.status != 0) return LCOM_ERR;
  irq->mask = 0;
  return LCOM_OK;
}

int lcom_event_wait(lcom_event_t *event) {
  if (event == NULL) return LCOM_ERR;

  lcom_event_reply_t reply;
  if (request_reply(LCOM_MSG_EVENT_WAIT, NULL, 0, LCOM_MSG_EVENT_REPLY,
                    &reply, sizeof(reply), NULL) != 0) {
    return LCOM_ERR;
  }
  if (reply.status != 0) return LCOM_ERR;
  event->irq_mask = reply.irq_mask;
  event->tick = reply.tick;
  return LCOM_OK;
}

int lcom_delay_ticks(uint64_t ticks, lcom_event_t *event) {
  if (event == NULL) return LCOM_ERR;
  lcom_delay_ticks_t req = {ticks};
  lcom_event_reply_t reply;
  if (request_reply(LCOM_MSG_DELAY_TICKS, &req, sizeof(req), LCOM_MSG_EVENT_REPLY,
                    &reply, sizeof(reply), NULL) != 0 || reply.status != 0) return LCOM_ERR;
  event->irq_mask = reply.irq_mask;
  event->tick = reply.tick;
  return LCOM_OK;
}

int lcom_phys_map(uintptr_t phys, size_t len, void **out) {
  if (out == NULL || len == 0) return LCOM_ERR;

  lcom_phys_map_t req;
  req.phys = (uint64_t)phys;
  req.length = (uint64_t)len;

  lcom_phys_map_reply_t reply;
  if (request_reply(LCOM_MSG_PHYS_MAP, &req, sizeof(req), LCOM_MSG_PHYS_MAP_REPLY,
                    &reply, sizeof(reply), NULL) != 0) {
    return LCOM_ERR;
  }
  if (reply.status != 0 || reply.length != len ||
      memchr(reply.shm_name, '\0', sizeof(reply.shm_name)) == NULL) return LCOM_ERR;
  long page_size = sysconf(_SC_PAGESIZE);
  if (page_size <= 0) return LCOM_ERR;
  uint64_t aligned_offset = reply.offset - reply.offset % (uint64_t)page_size;
  size_t prefix = (size_t)(reply.offset - aligned_offset);
  if (len > SIZE_MAX - prefix || aligned_offset > INT64_MAX) return LCOM_ERR;
  struct phys_mapping *mapping = malloc(sizeof(*mapping));
  if (mapping == NULL) return LCOM_ERR;
  int fd = shm_open(reply.shm_name, O_RDWR, 0600);
  if (fd < 0) { free(mapping); return LCOM_ERR; }

  void *mapped = mmap(NULL, len + prefix, PROT_READ | PROT_WRITE, MAP_SHARED,
                      fd, (off_t)aligned_offset);
  close(fd);
  if (mapped == MAP_FAILED) { free(mapping); return LCOM_ERR; }

  *out = (uint8_t *)mapped + prefix;
  *mapping = (struct phys_mapping){*out, mapped, len, len + prefix, g_mappings};
  g_mappings = mapping;
  return LCOM_OK;
}

int lcom_phys_unmap(void *ptr, size_t len) {
  if (ptr == NULL || len == 0) return LCOM_ERR;
  struct phys_mapping **link = &g_mappings;
  while (*link != NULL && (*link)->user != ptr) link = &(*link)->next;
  if (*link == NULL || (*link)->length != len) return LCOM_ERR;
  struct phys_mapping *mapping = *link;
  if (munmap(mapping->base, mapping->mapped_length) != 0) return LCOM_ERR;
  *link = mapping->next;
  free(mapping);
  return LCOM_OK;
}

int lcom_vbe_get_mode_info(uint16_t mode, lcom_vbe_mode_info_t *out) {
  if (out == NULL) return LCOM_ERR;

  lcom_vbe_mode_request_t req;
  req.mode = mode;
  req.reserved = 0;

  lcom_vbe_mode_info_wire_t reply;
  if (request_reply(LCOM_MSG_VBE_GET_MODE_INFO, &req, sizeof(req),
                    LCOM_MSG_VBE_MODE_INFO_REPLY, &reply, sizeof(reply), NULL) != 0) {
    return LCOM_ERR;
  }
  if (reply.status != 0) return LCOM_ERR;

  out->mode = reply.mode;
  out->width = reply.width;
  out->height = reply.height;
  out->bpp = reply.bpp;
  out->bytes_per_pixel = reply.bytes_per_pixel;
  out->pitch = reply.pitch;
  out->framebuffer_phys = (uintptr_t)reply.framebuffer_phys;
  out->framebuffer_size = (size_t)reply.framebuffer_size;
  return LCOM_OK;
}

int lcom_vbe_set_mode(uint16_t mode) {
  lcom_vbe_mode_request_t req;
  req.mode = mode;
  req.reserved = 0;

  lcom_status_reply_t reply;
  if (request_reply(LCOM_MSG_VBE_SET_MODE, &req, sizeof(req), LCOM_MSG_STATUS,
                    &reply, sizeof(reply), NULL) != 0) {
    return LCOM_ERR;
  }
  return reply.status == 0 ? LCOM_OK : LCOM_ERR;
}

int lcom_vbe_set_display_start(uint16_t x, uint16_t y) {
  lcom_vbe_display_start_t req = {x, y};
  lcom_status_reply_t reply;
  if (request_reply(LCOM_MSG_VBE_SET_DISPLAY_START, &req, sizeof(req), LCOM_MSG_STATUS,
                    &reply, sizeof(reply), NULL) != 0) return LCOM_ERR;
  return reply.status == 0 ? LCOM_OK : LCOM_ERR;
}

int lcom_vbe_scanline(uint8_t operation, uint16_t value, lcom_vbe_scanline_info_t *out) {
  if (!out) return LCOM_ERR;
  lcom_vbe_scanline_request_t req = {value, operation, 0};
  lcom_vbe_scanline_reply_t reply;
  if (request_reply(LCOM_MSG_VBE_SCANLINE, &req, sizeof(req), LCOM_MSG_VBE_SCANLINE_REPLY,
                    &reply, sizeof(reply), NULL) != 0 || reply.status != 0) return LCOM_ERR;
  *out = (lcom_vbe_scanline_info_t){reply.bytes, reply.pixels, reply.lines};
  return LCOM_OK;
}

int lcom_vbe_dac_format(uint8_t operation, uint8_t requested, uint8_t *actual) {
  if (!actual) return LCOM_ERR;
  lcom_vbe_dac_request_t req = {operation, requested, {0, 0}};
  lcom_vbe_dac_reply_t reply;
  if (request_reply(LCOM_MSG_VBE_DAC_FORMAT, &req, sizeof(req), LCOM_MSG_VBE_DAC_FORMAT_REPLY,
                    &reply, sizeof(reply), NULL) != 0) return LCOM_ERR;
  if (reply.status != 0) return reply.status == LCOM_UNSUPPORTED ? LCOM_UNSUPPORTED : LCOM_ERR;
  *actual = reply.width;
  return LCOM_OK;
}

int lcom_vbe_present(void) {
  lcom_status_reply_t reply;
  if (request_reply(LCOM_MSG_VBE_PRESENT, NULL, 0, LCOM_MSG_STATUS,
                    &reply, sizeof(reply), NULL) != 0) {
    return LCOM_ERR;
  }
  return reply.status == 0 ? LCOM_OK : LCOM_ERR;
}

int lcom_ac97_get_buffer(lcom_ac97_buffer_info_t *out) {
  if (out == NULL) return LCOM_ERR;
  lcom_ac97_buffer_wire_t reply;
  if (request_reply(LCOM_MSG_AC97_GET_BUFFER, NULL, 0, LCOM_MSG_AC97_BUFFER_REPLY,
                    &reply, sizeof(reply), NULL) != 0) {
    return LCOM_ERR;
  }
  if (reply.status != 0) return LCOM_ERR;
  out->pcm_phys = (uintptr_t)reply.pcm_phys;
  out->pcm_bytes = (size_t)reply.pcm_bytes;
  out->sample_rate = reply.sample_rate;
  out->channels = reply.channels;
  out->bits_per_sample = reply.bits_per_sample;
  return LCOM_OK;
}

int lcom_clipboard_set_text(const char *text) {
  if (text == NULL) return LCOM_ERR;
  size_t length = strnlen(text, sizeof(((lcom_clipboard_reply_t *)0)->text));
  if (length >= sizeof(((lcom_clipboard_reply_t *)0)->text)) return LCOM_ERR;
  lcom_status_reply_t reply;
  if (request_reply(LCOM_MSG_CLIPBOARD_SET, text, (uint32_t)length + 1,
                    LCOM_MSG_STATUS, &reply, sizeof(reply), NULL) != 0) return LCOM_ERR;
  return reply.status == 0 ? LCOM_OK : LCOM_ERR;
}

int lcom_clipboard_get_text(char *buffer, size_t capacity) {
  if (buffer == NULL || capacity == 0) return LCOM_ERR;
  lcom_clipboard_reply_t reply;
  if (request_reply(LCOM_MSG_CLIPBOARD_GET, NULL, 0, LCOM_MSG_CLIPBOARD_REPLY,
                    &reply, sizeof(reply), NULL) != 0 || reply.status != 0) return LCOM_ERR;
  size_t length = strnlen(reply.text, sizeof(reply.text));
  if (length >= sizeof(reply.text) || length >= capacity) return LCOM_ERR;
  memcpy(buffer, reply.text, length + 1);
  return LCOM_OK;
}
