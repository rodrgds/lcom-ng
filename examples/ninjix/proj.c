#include "app.h"

#include <lcom/lcom.h>

int main(void) {
  if (lcom_init() != LCOM_OK) return 1;
  int result = app_run();
  lcom_exit();
  return result;
}
