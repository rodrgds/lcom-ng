#define LCOM_COMPAT_IMPLEMENTATION
#include <lcom/compat.h>
#include <lcom/vbe_modes.h>
#include <stdarg.h>
#include <sys/stat.h>
#include "output_log.h"

#define WEAK __attribute__((weak))
#define HOOKS 32
#define BIOS_VIDEO 0x10
#define LOW_BASE 0x10000u
#define LOW_SIZE 0x80000u

_Static_assert(sizeof(vbe_mode_info_t) == 256, "VBE mode info must retain its BIOS layout");
struct irq_hook { bool used, enabled, subscribed; int policy; lcom_irq_t irq; };
static struct irq_hook hooks[HOOKS];
struct queued_event { lcom_event_t event; struct queued_event *next; };
static struct queued_event *event_head, *event_tail;
/* The returned base represents physical address zero, as lm_init promises. */
static unsigned char low_memory[LOW_BASE + LOW_SIZE];
#define BIOS_DATA_BASE (LOW_BASE + LOW_SIZE - 4096u)
#define ALLOC_SIZE (LOW_SIZE - 4096u)
struct low_block { size_t start, size; struct low_block *next; };
static struct low_block *low_blocks;
static uint8_t *fallback_vram;
static lcom_vbe_mode_info_t fallback_mode;
static bool graphics_active;
static bool compat_initialized;
static uint16_t display_x, display_y;
WEAK const char *lcom_compat_project_root = NULL;

/* Match a terminal's line buffering before any student stdio runs. */
static void __attribute__((constructor)) student_console_init(void) {
  setvbuf(stdout, NULL, _IOLBF, 0);
}

int lcom_compat_init(void) {
  if (lcom_init() != LCOM_OK) return EIO;
  if (compat_initialized) return OK;
  const char *option = getenv("LCOM_COMPAT_MOUSE_REPORTING");
  if (option && *option && strcmp(option, "0") && strcmp(option, "1")) return EINVAL;
  if (option && !strcmp(option, "1")) {
    /* This explicit option reproduces an already-enabled VM mouse. Course
     * Minix starts with stream reporting disabled, which remains our default. */
    uint8_t status, ack;
    if (lcom_port_read8(0x64, &status) != LCOM_OK) return EIO;
    if (status & (BIT(0) | BIT(1))) return EBUSY;
    if (lcom_port_write8(0x64, 0xd4) != LCOM_OK || lcom_port_write8(0x60, 0xf4) != LCOM_OK) return EIO;
    if (lcom_port_read8(0x64, &status) != LCOM_OK || (status & (BIT(0) | BIT(5))) != (BIT(0) | BIT(5))) return EIO;
    if (lcom_port_read8(0x60, &ack) != LCOM_OK || ack != 0xfa) return EIO;
  }
  compat_initialized = true; return OK;
}
static int ready(void) { return lcom_compat_init(); }

int sys_inb(int port, uint32_t *value) {
  if (!value || port < 0 || port > UINT16_MAX) return EINVAL;
  uint8_t byte;
  if (ready() || lcom_port_read8((uint16_t)port, &byte) != LCOM_OK) return EIO;
  *value = byte;
  return OK;
}
int lcom_compat_sys_inb_ulong(int port, unsigned long *value) {
  if (!value) return EINVAL;
  uint32_t byte; int result = sys_inb(port, &byte);
  if (!result) *value = byte;
  return result;
}
int sys_outb(int port, uint32_t value) {
  if (port < 0 || port > UINT16_MAX || value > UINT8_MAX) return EINVAL;
  return !ready() && lcom_port_write8((uint16_t)port, (uint8_t)value) == LCOM_OK ? OK : EIO;
}
void sef_startup(void) { (void)ready(); }
unsigned int sys_hz(void) { return 60; }
WEAK int mouse_enable_data_reporting(void) {
  int initialization = ready();
  if (initialization) return initialization;
  uint32_t status;
  if (sys_inb(0x64, &status) != OK) return EIO;
  if (status & (BIT(0) | BIT(1))) return EBUSY;
  if (sys_outb(0x64, 0xd4) != OK || sys_outb(0x60, 0xf4) != OK) return EIO;
  for (unsigned attempt = 0; attempt < 10; ++attempt) {
    if (sys_inb(0x64, &status) != OK) return EIO;
    if (status & BIT(0)) {
      if (!(status & BIT(5)) || (status & (BIT(6) | BIT(7)))) return EIO;
      uint32_t reply;
      return sys_inb(0x60, &reply) == OK && reply == 0xfa ? OK : EIO;
    }
    if (micro_delay(1000) != OK) return EIO;
  }
  return ETIMEDOUT;
}
uint8_t minix_get_dflt_kbc_cmd_byte(void) { return 0x03; }
int micro_delay(unsigned int micros) { return tickdelay(micros_to_ticks(micros)); }
uint32_t micros_to_ticks(uint32_t micros) {
  uint64_t ticks = micros / 1000000u * 60u + (micros % 1000000u * 60u + 999999u) / 1000000u;
  return (uint32_t)ticks;
}
void panic(const char *fmt, ...) {
  va_list ap; va_start(ap, fmt); vfprintf(stderr, fmt, ap); va_end(ap);
  fputc('\n', stderr); lcf_cleanup(); exit(EXIT_FAILURE);
}

int sys_irqsetpolicy(int irq, int policy, int *hook_id) {
  if (!hook_id || irq < 0 || irq > 15 || *hook_id < 0 || *hook_id >= HOOKS ||
      (policy & ~(IRQ_REENABLE | IRQ_EXCLUSIVE))) return EINVAL;
  for (int i = 0; i < HOOKS; ++i) if (hooks[i].used && hooks[i].irq.irq == irq) return EBUSY;
  int slot = *hook_id;
  if (hooks[slot].used) {
    for (slot = 0; slot < HOOKS && hooks[slot].used; ++slot) {}
    if (slot == HOOKS) return ENOSPC;
  }
  lcom_irq_t runtime_irq;
  if (ready() || lcom_irq_subscribe((uint8_t)irq, (uint32_t)policy, &runtime_irq) != LCOM_OK) return EIO;
  hooks[slot] = (struct irq_hook){true, true, true, policy, runtime_irq};
  *hook_id = slot;
  return OK;
}
static struct irq_hook *get_hook(int *id) {
  return id && *id >= 0 && *id < HOOKS && hooks[*id].used ? &hooks[*id] : NULL;
}
int sys_irqrmpolicy(int *hook_id) {
  struct irq_hook *hook = get_hook(hook_id);
  if (!hook) return EINVAL;
  if (hook->subscribed && lcom_irq_unsubscribe(&hook->irq) != LCOM_OK) return EIO;
  memset(hook, 0, sizeof(*hook)); return OK;
}
int sys_irqenable(int *hook_id) {
  struct irq_hook *hook = get_hook(hook_id); if (!hook) return EINVAL;
  if (!hook->subscribed) {
    lcom_irq_t subscription;
    if (lcom_irq_subscribe(hook->irq.irq, (uint32_t)hook->policy, &subscription) != LCOM_OK) return EIO;
    hook->irq = subscription; hook->subscribed = true;
  }
  hook->enabled = true; return OK;
}
int sys_irqdisable(int *hook_id) {
  struct irq_hook *hook = get_hook(hook_id); if (!hook) return EINVAL;
  if (hook->subscribed && lcom_irq_unsubscribe(&hook->irq) != LCOM_OK) return EIO;
  hook->subscribed = hook->enabled = false; return OK;
}
static int notification_mask(uint32_t runtime_mask, uint32_t *out) {
  uint32_t mask = 0;
  for (int i = 0; i < HOOKS; ++i) {
    struct irq_hook *hook = &hooks[i];
    if (hook->used && hook->enabled && (runtime_mask & hook->irq.mask)) {
      mask |= BIT(i);
      if (!(hook->policy & IRQ_REENABLE)) {
        if (lcom_irq_unsubscribe(&hook->irq) != LCOM_OK) return EIO;
        hook->subscribed = hook->enabled = false;
      }
    }
  }
  *out = mask; return OK;
}
static int next_event(lcom_event_t *event) {
  if (event_head) {
    struct queued_event *head = event_head;
    *event = head->event; event_head = head->next;
    if (!event_head) event_tail = NULL;
    free(head); return OK;
  }
  return lcom_event_wait(event) == LCOM_OK ? OK : EIO;
}
int driver_receive(endpoint_t source, message *msg, int *status) {
  if ((source != ANY && source != HARDWARE) || !msg || !status) return EINVAL;
  if (ready()) return EIO;
  if (graphics_active && lcom_vbe_present() != LCOM_OK) return EIO;
  for (;;) {
    lcom_event_t event;
    if (next_event(&event)) return EIO;
    uint32_t mask;
    if (notification_mask(event.irq_mask, &mask)) return EIO;
    if (!mask) continue;
    memset(msg, 0, sizeof(*msg)); msg->m_source = HARDWARE; msg->m_type = NOTIFY_MESSAGE;
    msg->m_notify.interrupts = mask; *status = IPC_STATUS_NOTIFY;
    return OK;
  }
}
int tickdelay(clock_t ticks) {
  if (ticks < 0) return EINVAL;
  if (!ticks) return OK;
  if (ready()) return EIO;
  struct queued_event *node = malloc(sizeof(*node));
  if (!node) return ENOMEM;
  if (lcom_delay_ticks((uint64_t)ticks, &node->event) != LCOM_OK) { free(node); return EIO; }
  if (node->event.irq_mask) {
    node->next = NULL;
    if (event_tail) event_tail->next = node; else event_head = node;
    event_tail = node;
  } else free(node);
  return OK;
}

int sys_enable_iop(endpoint_t endpoint) { return endpoint == SELF ? ready() : ENOSYS; }
int sys_privctl(endpoint_t endpoint, int request, void *arg) {
  if (endpoint != SELF || request != SYS_PRIV_ADD_MEM) return ENOSYS;
  struct minix_mem_range *range = arg;
  if (!range || range->mr_limit <= range->mr_base) return EINVAL;
  return ready(); /* The runtime validates the physical range when it is mapped. */
}
void *vm_map_phys(endpoint_t endpoint, void *phys, size_t len) {
  void *result = NULL;
  if (endpoint != SELF || ready() || lcom_phys_map((uintptr_t)phys, len, &result) != LCOM_OK) {
    errno = EINVAL; return MAP_FAILED;
  }
  return result;
}
int vm_unmap_phys(endpoint_t endpoint, void *ptr, size_t len) {
  if (endpoint != SELF) return EINVAL;
  return lcom_phys_unmap(ptr, len) == LCOM_OK ? OK : EINVAL;
}
void *lm_init(bool enable) { return enable ? low_memory : NULL; }
void *lm_alloc(size_t size, mmap_t *mapping) {
  if (!size || !mapping || size > ALLOC_SIZE) return NULL;
  size_t at = 0;
  struct low_block **link = &low_blocks;
  while (*link) {
    if ((*link)->start >= at && size <= (*link)->start - at) break;
    at = ((*link)->start + (*link)->size + 15u) & ~(size_t)15u;
    link = &(*link)->next;
  }
  if (at > ALLOC_SIZE || size > ALLOC_SIZE - at) return NULL;
  struct low_block *block = malloc(sizeof(*block)); if (!block) return NULL;
  *block = (struct low_block){at, size, *link}; *link = block;
  *mapping = (mmap_t){.virt = low_memory + LOW_BASE + at, .phys = LOW_BASE + at, .size = size};
  memset(mapping->virtual, 0, size); return mapping->virtual;
}
bool lm_free(const mmap_t *mapping) {
  if (!mapping) return false;
  struct low_block **link = &low_blocks;
  while (*link && low_memory + LOW_BASE + (*link)->start != mapping->virtual) link = &(*link)->next;
  if (!*link) return false;
  struct low_block *block = *link; *link = block->next; free(block);
  return true;
}
static void *bios_buffer(reg86_t *regs, size_t size) {
  uintptr_t phys = ((uintptr_t)regs->es << 4) + regs->di;
  for (struct low_block *block = low_blocks; block; block = block->next) {
    uintptr_t base = LOW_BASE + block->start;
    if (phys >= base && phys - base <= block->size && size <= block->size - (phys - base))
      return low_memory + base + (phys - base);
  }
  return NULL;
}
int vbe_get_mode_info(uint16_t mode, vbe_mode_info_t *info) {
  lcom_vbe_mode_info_t native;
  if (!info) return EINVAL;
  if (ready() || lcom_vbe_get_mode_info(mode & 0x3fffu, &native) != LCOM_OK) return EINVAL;
  memset(info, 0, sizeof(*info));
  info->ModeAttributes = 0x99; info->XResolution = native.width; info->YResolution = native.height;
  info->NumberOfPlanes = 1; info->BitsPerPixel = native.bpp;
  info->BytesPerScanLine = info->LinBytesPerScanLine = (uint16_t)native.pitch;
  info->PhysBasePtr = (uint32_t)native.framebuffer_phys;
  info->MemoryModel = native.bpp == 8 ? 4 : 6;
  info->NumberOfImagePages = info->LinNumberOfImagePages = info->BnkNumberOfImagePages = 1;
  if (native.bpp > 8) {
    info->RedMaskSize = native.bpp == 15 || native.bpp == 16 ? 5 : 8;
    info->GreenMaskSize = native.bpp == 16 ? 6 : info->RedMaskSize;
    info->BlueMaskSize = info->RedMaskSize;
    info->GreenFieldPosition = info->BlueMaskSize;
    info->RedFieldPosition = info->GreenFieldPosition + info->GreenMaskSize;
    if (native.bpp == 32 || native.bpp == 15) {
      info->RsvdMaskSize = native.bpp == 32 ? 8 : 1;
      info->RsvdFieldPosition = native.bpp == 32 ? 24 : 15;
    }
    info->LinRedMaskSize = info->RedMaskSize; info->LinRedFieldPosition = info->RedFieldPosition;
    info->LinGreenMaskSize = info->GreenMaskSize; info->LinGreenFieldPosition = info->GreenFieldPosition;
    info->LinBlueMaskSize = info->BlueMaskSize; info->LinBlueFieldPosition = info->BlueFieldPosition;
    info->LinRsvdMaskSize = info->RsvdMaskSize; info->LinRsvdFieldPosition = info->RsvdFieldPosition;
  }
  return OK;
}
static void bios_u16(uint8_t *data, size_t offset, uint16_t value) {
  data[offset] = (uint8_t)value; data[offset + 1] = (uint8_t)(value >> 8);
}
static void bios_u32(uint8_t *data, size_t offset, uint32_t value) {
  bios_u16(data, offset, (uint16_t)value); bios_u16(data, offset + 2, (uint16_t)(value >> 16));
}
static uint32_t bios_far_pointer(uint32_t physical) {
  return ((physical >> 4) << 16) | (physical & 0xfu);
}
static int vbe_controller(reg86_t *regs) {
  uint8_t *data = bios_buffer(regs, 512);
  if (!data) { regs->ax = 0x014f; return EINVAL; }
  if (ready()) { regs->ax = 0x014f; return EIO; }
  memset(data, 0, 512); memcpy(data, "VESA", 4);
  bios_u16(data, 4, 0x0300);
  /* Keep ROM-style strings and the terminated mode list outside allocations,
   * accessible using lm_init() plus the converted BIOS far pointer. */
  uint8_t *rom = low_memory + BIOS_DATA_BASE; memset(rom, 0, 4096);
#define LCOM_VBE_MODE_NUMBER(code, w, h, depth) code,
  const uint16_t candidates[] = {LCOM_VBE_GRAPHICS_MODES(LCOM_VBE_MODE_NUMBER)};
#undef LCOM_VBE_MODE_NUMBER
  _Static_assert(sizeof(candidates) + sizeof(uint16_t) <= 64,
                 "VBE mode list must fit before the BIOS OEM string");
  size_t count = 0;
  for (size_t i = 0; i < sizeof(candidates) / sizeof(candidates[0]); ++i) {
    lcom_vbe_mode_info_t mode;
    if (lcom_vbe_get_mode_info(candidates[i], &mode) == LCOM_OK) bios_u16(rom, 2 * count++, candidates[i]);
  }
  bios_u16(rom, 2 * count, 0xffff);
  strcpy((char *)rom + 64, "lcom-ng virtual VBE");
  strcpy((char *)rom + 128, "lcom-ng");
  strcpy((char *)rom + 160, "LCOM graphics runtime");
  strcpy((char *)rom + 224, "1.0");
  bios_u32(data, 6, bios_far_pointer(BIOS_DATA_BASE + 64));
  bios_u32(data, 14, bios_far_pointer(BIOS_DATA_BASE));
  bios_u16(data, 18, 256); /* 16 MiB, in 64 KiB units. */
  bios_u16(data, 20, 0x0100);
  bios_u32(data, 22, bios_far_pointer(BIOS_DATA_BASE + 128));
  bios_u32(data, 26, bios_far_pointer(BIOS_DATA_BASE + 160));
  bios_u32(data, 30, bios_far_pointer(BIOS_DATA_BASE + 224));
  regs->ax = 0x004f; return OK;
}
int sys_int86(reg86_t *regs) {
  if (!regs || regs->intno != BIOS_VIDEO) return ENOSYS;
  if (regs->ax == 0x0003) {
    int result = ready() || lcom_vbe_set_mode(3) != LCOM_OK ? EIO : OK;
    if (!result) graphics_active = false;
    return result;
  }
  if (regs->ax == 0x4f00) return vbe_controller(regs);
  if (regs->ax == 0x4f01) {
    vbe_mode_info_t *info = bios_buffer(regs, sizeof(*info));
    if (!info) { regs->ax = 0x014f; return EINVAL; }
    int result = vbe_get_mode_info(regs->cx, info);
    regs->ax = result ? 0x014f : 0x004f; return result;
  }
  if (regs->ax == 0x4f02) {
    int result = ready() || lcom_vbe_set_mode(regs->bx & 0x3fff) != LCOM_OK ? EINVAL : OK;
    if (!result) { graphics_active = true; display_x = display_y = 0; }
    regs->ax = result ? 0x014f : 0x004f; return result;
  }
  if (regs->ax == 0x4f07) {
    if (regs->bl == 1) {
      regs->cx = display_x; regs->dx = display_y; regs->ax = 0x004f; return OK;
    }
    if (regs->bl != 0 && regs->bl != 0x80) { regs->ax = 0x014f; return EINVAL; }
    int result = ready() || lcom_vbe_set_display_start(regs->cx, regs->dx) != LCOM_OK ? EINVAL : OK;
    if (!result) { display_x = regs->cx; display_y = regs->dx; }
    regs->ax = result ? 0x014f : 0x004f; return result;
  }
  regs->ax = 0x014f; return ENOSYS;
}
int vg_exit(void) { reg86_t regs = {0}; regs.intno = BIOS_VIDEO; regs.ax = 3; return sys_int86(&regs); }
WEAK void *vg_init(uint16_t mode) {
  if (fallback_vram) { lcom_phys_unmap(fallback_vram, fallback_mode.framebuffer_size); fallback_vram = NULL; }
  if (ready() || lcom_vbe_get_mode_info(mode, &fallback_mode) != LCOM_OK || lcom_vbe_set_mode(mode) != LCOM_OK) return NULL;
  void *ptr;
  if (lcom_phys_map(fallback_mode.framebuffer_phys, fallback_mode.framebuffer_size, &ptr) != LCOM_OK) return NULL;
  graphics_active = true; fallback_vram = ptr; return ptr;
}
WEAK int vg_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  if (!fallback_vram || y >= fallback_mode.height || x > fallback_mode.width || len > fallback_mode.width - x) return EINVAL;
  uint8_t *row = fallback_vram + (size_t)y * fallback_mode.pitch + (size_t)x * fallback_mode.bytes_per_pixel;
  for (unsigned i = 0; i < len; ++i)
    for (unsigned b = 0; b < fallback_mode.bytes_per_pixel; ++b) row[(size_t)i * fallback_mode.bytes_per_pixel + b] = (uint8_t)(color >> (8 * b));
  return OK;
}
WEAK int vg_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  if (!fallback_vram || y > fallback_mode.height || height > fallback_mode.height - y) return EINVAL;
  for (unsigned i = 0; i < height; ++i) { int result = vg_draw_hline(x, (uint16_t)(y + i), width, color); if (result) return result; }
  return OK;
}
WEAK int util_get_LSB(uint16_t value, uint8_t *lsb) { if (!lsb) return EINVAL; *lsb = (uint8_t)value; return OK; }
WEAK int util_get_MSB(uint16_t value, uint8_t *msb) { if (!msb) return EINVAL; *msb = (uint8_t)(value >> 8); return OK; }
WEAK int util_sys_inb(int port, uint8_t *value) {
  uint32_t data; if (!value) return EINVAL;
  int result = sys_inb(port, &data); if (!result) *value = (uint8_t)data; return result;
}
WEAK int timer_print_config(uint8_t timer, enum timer_status_field field, union timer_status_field_val value) {
  if (timer > 2 || field < tsf_all || field > tsf_base) return EINVAL;
  unsigned data = field == tsf_initial ? (unsigned)value.in_mode : field == tsf_base ? value.bcd : field == tsf_mode ? value.count_mode : value.byte;
  printf("Timer %u field %u: %u\n", timer, field, data); return OK;
}
uint32_t timer_print_elapsed_time(void) { static uint32_t elapsed; printf("Elapsed time: %u seconds\n", ++elapsed); return elapsed; }
int kbd_print_scancode(bool make, uint8_t size, uint8_t *bytes) {
  if (!bytes || !size || size > 2) return EINVAL;
  printf("%s code:", make ? "Make" : "Break");
  for (unsigned i = 0; i < size; ++i) printf(" 0x%02X", bytes[i]);
  putchar('\n'); return OK;
}
int kbd_print_no_sysinb(uint32_t count) { printf("sys_inb calls: %u\n", count); return OK; }
int mouse_print_packet(struct packet *p) {
  if (!p) return EINVAL;
  printf("B1=0x%02X B2=0x%02X B3=0x%02X LB=%u MB=%u RB=%u XOV=%u YOV=%u X=%d Y=%d\n",
    p->bytes[0], p->bytes[1], p->bytes[2], p->lb, p->mb, p->rb, p->x_ov, p->y_ov, p->delta_x, p->delta_y);
  return OK;
}
struct mouse_ev *mouse_detect_event(struct packet *p) {
  static bool lb, rb, mb; static struct mouse_ev event;
  if (!p) return NULL;
  event.delta_x = p->delta_x; event.delta_y = p->delta_y;
  if (p->lb != lb) event.type = p->lb ? LB_PRESSED : LB_RELEASED;
  else if (p->rb != rb) event.type = p->rb ? RB_PRESSED : RB_RELEASED;
  else if (p->mb != mb) event.type = BUTTON_EV;
  else event.type = MOUSE_MOV;
  lb = p->lb; rb = p->rb; mb = p->mb;
  return &event;
}
bool lcf_set_language(const char *language) {
  if (language && (!strcmp(language, "EN-US") || !strcmp(language, "PT-PT"))) return true;
  errno = EINVAL; return false;
}
bool lcf_trace_calls(const char *path) {
  if (!path) { errno = EINVAL; return false; }
  fprintf(stderr, "lcom-ng: LCF call tracing is unavailable; use runtime --trace instead of %s\n", path);
  errno = ENOSYS; return false;
}
int lcf_cleanup(void) {
  if (graphics_active) (void)vg_exit();
  if (fallback_vram) { (void)lcom_phys_unmap(fallback_vram, fallback_mode.framebuffer_size); fallback_vram = NULL; }
  for (int i = 0; i < HOOKS; ++i) if (hooks[i].used) { if (hooks[i].subscribed) (void)lcom_irq_unsubscribe(&hooks[i].irq); memset(&hooks[i], 0, sizeof(hooks[i])); }
  while (event_head) { struct queued_event *next = event_head->next; free(event_head); event_head = next; }
  event_tail = NULL;
  while (low_blocks) { struct low_block *next = low_blocks->next; free(low_blocks); low_blocks = next; }
  int logging_error = lcom_compat_output_log_stop();
  fflush(stdout); fflush(stderr);
  compat_initialized = false;
  lcom_exit();
  return logging_error;
}

/* Exporters turn explicit lcom.json aliases into per-process environment
 * entries, so the same resolver serves desktop, native Android and WASM. */
static char *aliased_path(const char *path, const char *root) {
  const char *setting = getenv("LCOM_COMPAT_PATH_ALIAS_COUNT");
  if (!setting) return NULL;
  char *end;
  unsigned long count = strtoul(setting, &end, 10);
  if (*end || count > 64) return NULL;
  const char *destination = NULL;
  size_t longest = 0;
  for (unsigned long index = 0; index < count; ++index) {
    char variable[64];
    snprintf(variable, sizeof(variable), "LCOM_COMPAT_PATH_ALIAS_%lu_FROM", index);
    const char *origin = getenv(variable);
    if (!origin || origin[0] != '/') continue;
    size_t length = strlen(origin);
    if (length <= longest || strncmp(path, origin, length) || (path[length] && path[length] != '/')) continue;
    snprintf(variable, sizeof(variable), "LCOM_COMPAT_PATH_ALIAS_%lu_TO", index);
    const char *target = getenv(variable);
    if (!target || !*target || target[0] == '/') continue;
    destination = target;
    longest = length;
  }
  if (!destination) return NULL;
  if (!strcmp(destination, ".")) destination = "";
  size_t root_size = strlen(root), target_size = strlen(destination), tail_size = strlen(path + longest);
  if (root_size > SIZE_MAX - target_size - tail_size - 2) { errno = ENAMETOOLONG; return NULL; }
  char *result = malloc(root_size + target_size + tail_size + 2);
  if (!result) return NULL;
  snprintf(result, root_size + target_size + tail_size + 2, "%s%s%s%s", root,
           target_size ? "/" : "", destination, path + longest);
  return result;
}

/* Return an owned fallback name, or NULL without changing errno when no
 * project mapping applies. Callers preserve normal filesystem behavior first. */
static char *project_path(const char *path) {
  if (path[0] != '/') return NULL;
  const char *root = getenv("LCOM_COMPAT_PROJECT_ROOT");
  if (!root || !*root) root = lcom_compat_project_root;
  if (!root) return NULL;
  char *alias = aliased_path(path, root);
  if (alias || errno) return alias;
  const char *relative = NULL;
  const char *prefixes[] = {"/home/lcom/labs/", "/home/lcom/"};
  for (size_t i = 0; i < sizeof(prefixes) / sizeof(prefixes[0]); ++i) {
    size_t prefix_size = strlen(prefixes[i]);
    if (!strncmp(path, prefixes[i], prefix_size)) { relative = path + prefix_size; break; }
  }
  if (!relative) return NULL;
  size_t root_len = strlen(root), relative_len = strlen(relative);
  if (root_len > SIZE_MAX - relative_len - 2) { errno = ENAMETOOLONG; return NULL; }
  char *rebased = malloc(root_len + relative_len + 2);
  if (rebased) snprintf(rebased, root_len + relative_len + 2, "%s/%s", root, relative);
  return rebased;
}

FILE *lcom_compat_fopen(const char *path, const char *mode) {
  if (!path || !mode) { errno = EINVAL; return NULL; }
  FILE *file = fopen(path, mode);
  if (file || errno != ENOENT) return file;
  errno = 0;
  char *rebased = project_path(path);
  if (!rebased) { if (!errno) errno = ENOENT; return NULL; }
  file = fopen(rebased, mode); free(rebased); return file;
}

int lcom_compat_remove(const char *path) {
  if (!path) { errno = EINVAL; return -1; }
  int result = remove(path);
  if (result == 0 || errno != ENOENT) return result;
  errno = 0;
  char *rebased = project_path(path);
  if (!rebased) { if (!errno) errno = ENOENT; return -1; }
  result = remove(rebased); free(rebased); return result;
}

/* rename may fail because either operand is absent. Preserve each existing
 * filesystem entry, including dangling symlinks, and map only missing names. */
static int missing_project_path(const char *path, char **replacement) {
  struct stat info;
  *replacement = NULL;
  if (lstat(path, &info) == 0 || errno != ENOENT) return 0;
  errno = 0;
  *replacement = project_path(path);
  return *replacement || !errno ? 0 : -1;
}

int lcom_compat_rename(const char *old_path, const char *new_path) {
  if (!old_path || !new_path) { errno = EINVAL; return -1; }
  int result = rename(old_path, new_path);
  if (result == 0 || errno != ENOENT) return result;
  char *old_rebased = NULL, *new_rebased = NULL;
  if (missing_project_path(old_path, &old_rebased) ||
      missing_project_path(new_path, &new_rebased)) {
    free(old_rebased); free(new_rebased); return -1;
  }
  if (old_rebased || new_rebased)
    result = rename(old_rebased ? old_rebased : old_path,
                    new_rebased ? new_rebased : new_path);
  else errno = ENOENT;
  free(old_rebased); free(new_rebased); return result;
}

FILE *lcom_compat_freopen(const char *path, const char *mode, FILE *stream) {
  if (!mode || !stream) { errno = EINVAL; return NULL; }
  if (path && !strcmp(path, "/dev/console")) {
    if ((stream == stdout || stream == stderr) &&
        (!strcmp(mode, "w") || !strcmp(mode, "wb") || !strcmp(mode, "a") || !strcmp(mode, "ab"))) {
      if (fflush(stream)) return NULL;
      clearerr(stream);
      return stream;
    }
    errno = ENOSYS;
    fprintf(stderr, "lcom-ng: /dev/console supports stdout/stderr output; use the virtual keyboard for input.\n");
    return NULL;
  }
  return freopen(path, mode, stream);
}

int lcf_cleanup_fast(void) { return lcf_cleanup(); }

unsigned int lcom_compat_sleep(unsigned int seconds) {
  uint64_t ticks = (uint64_t)seconds * 60u;
  if (ticks > LONG_MAX) { errno = EOVERFLOW; return seconds; }
  int result = tickdelay((clock_t)ticks);
  if (result) { errno = result; return seconds; }
  return 0;
}
int lcom_compat_usleep(unsigned int micros) {
  if (micros >= 1000000u) { errno = EINVAL; return -1; }
  int result = tickdelay((clock_t)micros_to_ticks(micros));
  if (result) { errno = result; return -1; }
  return 0;
}
int vg_display_vbe_contr_info(struct vg_vbe_contr_info *info) {
  if (!info || !info->OEMString || !info->OEMVendorNamePtr || !info->OEMProductNamePtr || !info->OEMProductRevPtr) return EINVAL;
  printf("VBE %.4s version %02x.%02x, %u KB\n%s\n%s %s %s\n", info->VBESignature,
    info->VBEVersion[0], info->VBEVersion[1], info->TotalMemory, info->OEMString,
    info->OEMVendorNamePtr, info->OEMProductNamePtr, info->OEMProductRevPtr);
  return OK;
}
