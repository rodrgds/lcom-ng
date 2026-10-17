#ifndef LCOM_NG_COMPAT_MINIX_BITMAP_H
#define LCOM_NG_COMPAT_MINIX_BITMAP_H
#include <limits.h>
#include <stdint.h>

/* Minix scalar masks and 32-bit bitmap chunks. Contract references:
 * minix/include/minix/bitmap.h at 433d6423c39e34ec4b79c950597bb2d236f886be;
 * sys/sys/types.h defines bitchunk_t as uint32_t on the Minix guest. */
typedef uint32_t bitchunk_t;

#define bit_set(mask, n) ((mask) |= (UINT32_C(1) << (n)))
#define bit_unset(mask, n) ((mask) &= ~(UINT32_C(1) << (n)))
#define bit_isset(mask, n) ((mask) & (UINT32_C(1) << (n)))
#define bit_empty(mask) ((mask) = 0)
#define bit_fill(mask) ((mask) = ~0)

#define BITCHUNK_BITS (sizeof(bitchunk_t) * CHAR_BIT)
#define BITMAP_CHUNKS(nr_bits) (((nr_bits) + BITCHUNK_BITS - 1) / BITCHUNK_BITS)
#define MAP_CHUNK(map, bit) (map)[(bit) / BITCHUNK_BITS]
#define CHUNK_OFFSET(bit) ((bit) % BITCHUNK_BITS)
#define GET_BIT(map, bit) (MAP_CHUNK(map, bit) & (UINT32_C(1) << CHUNK_OFFSET(bit)))
#define SET_BIT(map, bit) (MAP_CHUNK(map, bit) |= (UINT32_C(1) << CHUNK_OFFSET(bit)))
#define UNSET_BIT(map, bit) (MAP_CHUNK(map, bit) &= ~(UINT32_C(1) << CHUNK_OFFSET(bit)))

#if defined(CONFIG_SMP) && defined(__GNUC__) && !defined(__ASSEMBLY__)
static inline void bits_fill(bitchunk_t *chunks, unsigned bits) {
  for (unsigned index = 0; index < BITMAP_CHUNKS(bits); ++index) bit_fill(chunks[index]);
}
#endif
#endif
