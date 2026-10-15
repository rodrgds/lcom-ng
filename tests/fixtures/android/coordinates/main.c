#include <lcom/lcom.h>
#include "coordinates.h"

int main(void) {
  // Original games such as Skess keep a 2 MB drawing surface on the C stack.
  volatile uint8_t stack_frame[2 * 1024 * 1024];
  for (size_t page = 0; page < sizeof(stack_frame); page += 4096) stack_frame[page] = 42;
  if (stack_frame[0] != 42 || stack_frame[sizeof(stack_frame)-4096] != 42) return 10;
  if (lcom_init()) return 1;
  lcom_printf("fixture row:%d\n", fixture_rows[1]);
  int result = fixture_rows[1] == 200 ? 0 : 9;
  lcom_exit();
  return result;
}
