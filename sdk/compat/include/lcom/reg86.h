#ifndef LCOM_NG_MINIX_REG86_H
#define LCOM_NG_MINIX_REG86_H
/* Minix 3.1's nested register views share storage. Keep this separate tag
 * from the LCF register type so mixed-generation drivers retain their ABI. */
struct reg86u { union { reg86_t l, w, b; } u; };
int lcom_compat_legacy_int86(struct reg86u *registers);
#endif
