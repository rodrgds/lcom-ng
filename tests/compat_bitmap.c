#include <minix/drivers.h>
#ifndef bit_isset
#error Minix drivers.h must expose its canonical bitmap dependency
#endif
#include <minix/bitmap.h>
#include <assert.h>
#include <stdint.h>

int main(void) {
  uint8_t packet = 0;
  bit_set(packet, 3);
  bit_set(packet, 0);
  assert(packet == 9 && bit_isset(packet, 3) == 8);
  bit_unset(packet, 0);
  assert(packet == 8 && !bit_isset(packet, 0));
  bit_fill(packet);
  assert(packet == UINT8_MAX);
  bit_empty(packet);
  assert(packet == 0);

  bitchunk_t map[BITMAP_CHUNKS(65)] = {0};
  _Static_assert(sizeof(bitchunk_t) == 4, "Minix bitmap chunk is32bits");
  _Static_assert(BITMAP_CHUNKS(65) == 3, "65bits need3Minix chunks");
  SET_BIT(map, 31);
  SET_BIT(map, 32);
  SET_BIT(map, 64);
  assert(map[0] == UINT32_C(0x80000000));
  assert(map[1] == 1 && map[2] == 1);
  assert(GET_BIT(map, 31) == UINT32_C(0x80000000));
  UNSET_BIT(map, 32);
  assert(map[1] == 0 && GET_BIT(map, 64) == 1);
  return 0;
}
