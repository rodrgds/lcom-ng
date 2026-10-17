#ifndef LCOM_COMPAT_ASM_H
#define LCOM_COMPAT_ASM_H
#include <stdint.h>
/* Private declarations for compiler-lowered Minix assembly, not the native API. */
#ifdef __cplusplus
extern "C" {
#endif
void lcom_compat_cpu_interrupts(int enabled);
uint8_t lcom_compat_asm_in8(uint16_t port);
void lcom_compat_asm_out8(uint16_t port, uint8_t value);
#ifdef __cplusplus
}
#endif

/* GCC x86 a/INB/OUTB denotes AL here. A wider or boolean C operand would
 * change the original instruction bits and is deliberately not accepted. */
#define LCOM_COMPAT_ASM_BYTE(value) \
  _Static_assert(__builtin_types_compatible_p(__typeof__(value), char) || \
                 __builtin_types_compatible_p(__typeof__(value), signed char) || \
                 __builtin_types_compatible_p(__typeof__(value), unsigned char), \
                 "INB/OUTB AL operand must have character type")
/* Nd is either an unsigned 8-bit immediate or the 16-bit DX register. This
 * checks types/constant range without evaluating the C port expression. */
#define LCOM_COMPAT_ASM_PORT(port) \
  _Static_assert(__builtin_classify_type(port) == 1 && \
                 (sizeof(port) == 2 || __builtin_choose_expr(__builtin_constant_p(port), \
                    (port) >= 0 && (port) <= 255, 0)), \
                 "INB/OUTB Nd port must be 16-bit or an unsigned 8-bit constant")
#endif
