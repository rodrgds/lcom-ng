#include <time.h>
#include <minix/drivers.h>

int check_explicit_before(void) {
  time_t (*read_time)(time_t *) = time;
  clock_t (*read_cpu_clock)(void) = clock;
  time_t written = (time_t)-1;
  time_t now = read_time(&written);
  return now < (time_t)1000000000 || now != written || read_cpu_clock() == (clock_t)-1;
}
