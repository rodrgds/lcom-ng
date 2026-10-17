/* BSD integer names are available through Minix stdlib before LCF headers. */
#include <stdlib.h>
_Static_assert(sizeof(u_int8_t) == 1, "Minix unsigned8 ABI");
_Static_assert(sizeof(u_int16_t) == 2, "Minix unsigned16 ABI");
_Static_assert(sizeof(u_int32_t) == 4, "Minix unsigned32 ABI");
_Static_assert(sizeof(u_int64_t) == 8, "Minix unsigned64 ABI");
#include <minix/devio.h>
_Static_assert(sizeof(((pvb_pair_t *)0)->value) == 1, "byte I/O pair");
_Static_assert(sizeof(((pvw_pair_t *)0)->value) == 2, "word I/O pair");
_Static_assert(sizeof(((pvl_pair_t *)0)->value) == 4, "long I/O pair");
#include <lcom/lcf.h>

/* Older student C callbacks often omit unused arguments in their definition.
 * The original native ABI accepts the extra argument from this typed caller. */
static int old_callback() { return 42; }
static int (*volatile callback)(void *) = old_callback;

int (proj_main_loop)() {
  printf("legacy callback:%d\n", callback(NULL));
  return 0;
}

int main(int argc, char **argv) {
  return lcf_start(argc, argv);
}
