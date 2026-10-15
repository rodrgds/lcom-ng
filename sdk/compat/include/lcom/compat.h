#ifndef LCOM_NG_MINIX_COMPAT_H
#define LCOM_NG_MINIX_COMPAT_H

#include <lcom/lcom.h>
#include <stdbool.h>
#include <stdio.h>
#include <lcom/paths.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>

/* The Minix umbrella exposes time helpers, but does not reserve the name
 * clock used by older student graphics modules. Programs needing struct tm
 * members should include <time.h>, as with ordinary host C code. */
struct tm;
time_t time(time_t *result);
char *ctime(const time_t *timer);
struct tm *localtime(const time_t *timer);
struct tm *gmtime(const time_t *timer);
double difftime(time_t end, time_t beginning);
time_t mktime(struct tm *date);
size_t strftime(char *buffer, size_t size, const char *format, const struct tm *date);

/* Minix 3.4.0 endpoint and notification ABI used by the FEUP image. */
#define MAX_NR_TASKS 1023
#define _ENDPOINT_GENERATION_SHIFT 15
#define _ENDPOINT_GENERATION_SIZE (1 << _ENDPOINT_GENERATION_SHIFT)
#define _ENDPOINT_SLOT_TOP (_ENDPOINT_GENERATION_SIZE - MAX_NR_TASKS)
#define ANY ((endpoint_t)(_ENDPOINT_SLOT_TOP - 1))
#define NONE ((endpoint_t)(_ENDPOINT_SLOT_TOP - 2))
#define SELF ((endpoint_t)(_ENDPOINT_SLOT_TOP - 3))
#define ASYNCM ((endpoint_t)-5)
#define IDLE ((endpoint_t)-4)
#define CLOCK ((endpoint_t)-3)
#define SYSTEM ((endpoint_t)-2)
#define KERNEL ((endpoint_t)-1)
#define HARDWARE KERNEL
#define _ENDPOINT(generation, slot) ((endpoint_t)(((generation) << _ENDPOINT_GENERATION_SHIFT) + (slot)))
#define _ENDPOINT_G(endpoint) (((endpoint) + MAX_NR_TASKS) >> _ENDPOINT_GENERATION_SHIFT)
#define _ENDPOINT_P(endpoint) ((((endpoint) + MAX_NR_TASKS) & (_ENDPOINT_GENERATION_SIZE - 1)) - MAX_NR_TASKS)
#define IRQ_REENABLE 0x001
#define IRQ_EXCLUSIVE 0x002
#define IRQ_ENABLE 3
/* Legacy Minix headers expose integer booleans alongside stdbool.h. */
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#define SYS_PRIV_ADD_MEM 6
#define NOTIFY_MESSAGE 0x1000
#define is_notify(type) ((unsigned)((type) - NOTIFY_MESSAGE) < 0x100u)
#define SEND 1
#define RECEIVE 2
#define SENDREC 3
#define NOTIFY 4
#define SENDNB 5
#define SENDA 16
#define IPC_STATUS_CALL_SHIFT 0
#define IPC_STATUS_CALL_MASK 0x3f
#define IPC_STATUS_CALL(status) (((status) >> IPC_STATUS_CALL_SHIFT) & IPC_STATUS_CALL_MASK)
#define IPC_STATUS_CALL_TO(call) (((call) & IPC_STATUS_CALL_MASK) << IPC_STATUS_CALL_SHIFT)
#define IPC_FLG_MSG_FROM_KERNEL 1
#define IPC_STATUS_FLAGS_SHIFT 16
#define IPC_STATUS_FLAGS(flags) ((flags) << IPC_STATUS_FLAGS_SHIFT)
#define IPC_STATUS_FLAGS_TEST(status, flags) (((status) >> IPC_STATUS_FLAGS_SHIFT) & (flags))
#define IPC_STATUS_NOTIFY IPC_STATUS_CALL_TO(NOTIFY)
#define is_ipc_notify(status) (IPC_STATUS_CALL(status) == NOTIFY)
#define is_ipc_asynch(status) (is_ipc_notify(status) || IPC_STATUS_CALL(status) == SENDA)
/* sys/param.h macros re-exported by the original Minix driver umbrella. */
#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif
#define PB2BASE(address) ((uint16_t)(((uintptr_t)(address) >> 4) & 0xf000))
#define PB2OFF(address) ((uint16_t)((uintptr_t)(address) & 0xffff))

#ifndef KBD_AUX_IRQ
#define KBD_AUX_IRQ 12
#endif

#ifndef KEYBOARD_IRQ
#define KEYBOARD_IRQ 1
#endif

typedef uint8_t u8_t;
typedef uint16_t u16_t;
/* Exposed by the Minix system headers used in older lab sources. */
typedef uint16_t __uint16_t;
typedef uint32_t u32_t;
typedef uint64_t u64_t;
typedef int8_t i8_t;
typedef int16_t i16_t;
typedef int32_t i32_t;
typedef int64_t i64_t;
typedef uintptr_t phys_bytes;
typedef uintptr_t vir_bytes;
typedef int endpoint_t;
typedef uint16_t port_t;
typedef struct { int m_source; int m_type; struct { uint32_t interrupts; } m_notify; } message;
/* Older Minix handouts name the same notification mask NOTIFY_ARG. */
#define NOTIFY_ARG m_notify.interrupts
struct minix_mem_range { phys_bytes mr_base, mr_limit; };
typedef struct { phys_bytes phys; union { void *virt; void *virtual; }; size_t size; } mmap_t;

/* The BIOS register aliases overlap, as they do in the Minix interface. */
typedef struct reg86 {
  union { uint32_t ef; struct { uint16_t f, _ef_hi; }; struct { uint8_t intno, _ef_b1, _ef_b2, _ef_b3; }; };
  union { uint32_t vec; struct { uint16_t off, seg; }; };
  uint16_t ds, es;
  union { uint32_t eax; struct { uint16_t ax, _ax_hi; }; struct { uint8_t al, ah; }; };
  union { uint32_t ebx; struct { uint16_t bx, _bx_hi; }; struct { uint8_t bl, bh; }; };
  union { uint32_t ecx; struct { uint16_t cx, _cx_hi; }; struct { uint8_t cl, ch; }; };
  union { uint32_t edx; struct { uint16_t dx, _dx_hi; }; struct { uint8_t dl, dh; }; };
  union { uint32_t esi; struct { uint16_t si, _si_hi; }; };
  union { uint32_t edi; struct { uint16_t di, _di_hi; }; };
  union { uint32_t ebp; struct { uint16_t bp, _bp_hi; }; };
} reg86_t;

#pragma pack(push, 1)
typedef struct {
  uint16_t ModeAttributes;
  uint8_t WinAAttributes, WinBAttributes;
  uint16_t WinGranularity, WinSize, WinASegment, WinBSegment;
  uint32_t WinFuncPtr;
  uint16_t BytesPerScanLine, XResolution, YResolution;
  uint8_t XCharSize, YCharSize, NumberOfPlanes, BitsPerPixel, NumberOfBanks;
  uint8_t MemoryModel, BankSize, NumberOfImagePages, Reserved1;
  uint8_t RedMaskSize, RedFieldPosition, GreenMaskSize, GreenFieldPosition;
  uint8_t BlueMaskSize, BlueFieldPosition, RsvdMaskSize, RsvdFieldPosition;
  uint8_t DirectColorModeInfo;
  uint32_t PhysBasePtr;
  uint8_t Reserved2[4], Reserved3[2];
  uint16_t LinBytesPerScanLine;
  uint8_t BnkNumberOfImagePages, LinNumberOfImagePages;
  uint8_t LinRedMaskSize, LinRedFieldPosition, LinGreenMaskSize, LinGreenFieldPosition;
  uint8_t LinBlueMaskSize, LinBlueFieldPosition, LinRsvdMaskSize, LinRsvdFieldPosition;
  uint32_t MaxPixelClock;
  uint8_t Reserved4[190];
} vbe_mode_info_t;
#pragma pack(pop)

typedef uint8_t BCD;
typedef struct vg_vbe_contr_info {
  char VBESignature[4];
  BCD VBEVersion[2];
  char *OEMString;
  uint16_t *VideoModeList;
  uint32_t TotalMemory;
  char *OEMVendorNamePtr, *OEMProductNamePtr, *OEMProductRevPtr;
} vg_vbe_contr_info_t;
int vg_display_vbe_contr_info(struct vg_vbe_contr_info *info);

struct packet {
  uint8_t bytes[3];
  bool rb, mb, lb;
  int16_t delta_x, delta_y;
  bool x_ov, y_ov;
};
enum mouse_ev_t { LB_PRESSED, LB_RELEASED, RB_PRESSED, RB_RELEASED, BUTTON_EV, MOUSE_MOV };
struct mouse_ev { enum mouse_ev_t type; int16_t delta_x, delta_y; };
enum timer_status_field { tsf_all, tsf_initial, tsf_mode, tsf_base };
enum timer_init { INVAL_val, LSB_only, MSB_only, MSB_after_LSB };
union timer_status_field_val { uint8_t byte; enum timer_init in_mode; uint8_t count_mode; bool bcd; };

int lcom_compat_init(void);
int sys_inb(int port, uint32_t *value);
int lcom_compat_sys_inb_ulong(int port, unsigned long *value);
int sys_outb(int port, uint32_t value);
int sys_irqsetpolicy(int irq, int policy, int *hook_id);
int sys_irqrmpolicy(int *hook_id);
int sys_irqenable(int *hook_id);
int sys_irqdisable(int *hook_id);
int driver_receive(endpoint_t source, message *msg, int *status);
int sys_privctl(endpoint_t endpoint, int request, void *arg);
int sys_enable_iop(endpoint_t endpoint);
void *vm_map_phys(endpoint_t endpoint, void *phys, size_t len);
int vm_unmap_phys(endpoint_t endpoint, void *ptr, size_t len);
int sys_int86(reg86_t *regs);
unsigned int sys_hz(void);
int tickdelay(clock_t ticks);
int micro_delay(unsigned int micros);
uint8_t minix_get_dflt_kbc_cmd_byte(void);
uint32_t micros_to_ticks(uint32_t micros);
void sef_startup(void);
void panic(const char *fmt, ...);
void *lm_init(bool enable);
void *lm_alloc(size_t size, mmap_t *mapping);
bool lm_free(const mmap_t *mapping);
int vbe_get_mode_info(uint16_t mode, vbe_mode_info_t *info);
int vg_exit(void);
void *vg_init(uint16_t mode);
int vg_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int vg_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int util_get_LSB(uint16_t value, uint8_t *lsb);
int util_get_MSB(uint16_t value, uint8_t *msb);
int util_sys_inb(int port, uint8_t *value);
int timer_print_config(uint8_t timer, enum timer_status_field field, union timer_status_field_val value);
uint32_t timer_print_elapsed_time(void);
int kbd_print_scancode(bool make, uint8_t size, uint8_t *bytes);
int kbd_print_no_sysinb(uint32_t count);
int mouse_print_packet(struct packet *packet);
struct mouse_ev *mouse_detect_event(struct packet *packet);
unsigned int lcom_compat_sleep(unsigned int seconds);
int lcom_compat_usleep(unsigned int micros);
#ifndef LCOM_COMPAT_IMPLEMENTATION
/* Minix's 32-bit unsigned long becomes 64-bit on current host machines.
 * Preserve the original value semantics without leaving stale upper bytes. */
#ifndef __cplusplus
#define sys_inb(port, value) _Generic((value), unsigned long *: lcom_compat_sys_inb_ulong, default: sys_inb)((port), (value))
#endif
#define sleep lcom_compat_sleep
#define usleep lcom_compat_usleep
#endif
bool lcf_set_language(const char *language);
bool lcf_trace_calls(const char *path);
bool lcf_log_output(const char *path);
int lcf_start(int argc, char **argv);
int lcf_start_fast(int argc, char **argv);
int lcf_cleanup(void);
int lcf_cleanup_fast(void);
int proj_main_loop(int argc, char **argv);

#endif
