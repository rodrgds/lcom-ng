#ifndef LCOM_NG_MEMORY_MAP_H
#define LCOM_NG_MEMORY_MAP_H

/* Shared physical address owners. Host virtual pointers are unrelated. */
#define LCOM_MINIX_LOW_BASE 0x10000u
#define LCOM_MINIX_LOW_SIZE 0x80000u
#define LCOM_AC97_PCM_PHYS 0xD0000000u
#define LCOM_AC97_PCM_BYTES (2u * 1024u * 1024u)
#define LCOM_VBE_DEFAULT_PHYS 0xE0000000u
#define LCOM_VGA_TEXT_PHYS 0xB8000u
#define LCOM_VGA_TEXT_BYTES 0x8000u

#endif
