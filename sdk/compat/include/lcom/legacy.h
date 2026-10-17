#ifndef LCOM_NG_MINIX_LEGACY_H
#define LCOM_NG_MINIX_LEGACY_H

/* Pre-LCF projects retain their own lmlib and memory-range declarations. */
struct mem_range { phys_bytes mr_base, mr_limit; };
#ifdef LCOM_COMPAT_LEGACY_LM_STATUS
int lcom_compat_legacy_lm_status(void);
#else
void *lcom_compat_legacy_lm_init(void);
#endif
#ifndef LCOM_COMPAT_IMPLEMENTATION
#define lm_free lcom_compat_legacy_lm_free
#ifdef LCOM_COMPAT_LEGACY_LM_STATUS
#define lm_init lcom_compat_legacy_lm_status
#else
#define lm_init lcom_compat_legacy_lm_init
#endif
#endif
#endif
