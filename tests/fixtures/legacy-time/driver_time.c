#include <minix/drivers.h>

_Static_assert(sizeof(time((time_t *)0)) == sizeof(time_t), "driver time returns the libc time_t");
int check_driver_time(void) {
  struct { uint64_t before; time_t value; uint64_t after; } result = {
    UINT64_C(0x1122334455667788), (time_t)-1, UINT64_C(0x8877665544332211)
  };
  time_t now = time(&result.value);
  return now < (time_t)1000000000 || now != result.value ||
         result.before != UINT64_C(0x1122334455667788) ||
         result.after != UINT64_C(0x8877665544332211) || time(NULL) < now;
}
