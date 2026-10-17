#include <minix/drivers.h>
#include <machine/int86.h>

/* These names belong to older student modules, not imported time APIs. */
static unsigned time;
static unsigned clock;
int check_driver_time(void);
int check_explicit_before(void);
int check_explicit_after(void);

int main(void) {
  struct reg86u original_register_type = {0};
  time = 7;
  clock = 9;
  if (original_register_type.u.w.ax || time != 7 || clock != 9) return 1;
  if (check_driver_time() || check_explicit_before() || check_explicit_after()) return 2;
  printf("legacy time:%u pointer-write namespace function-address passed\n", (unsigned)(sizeof(time_t) * CHAR_BIT));
  return 0;
}
