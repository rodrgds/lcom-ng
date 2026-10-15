#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#include "../../../src/game/water/water.h"

static void test_water_creation(void) {
  Water *water = water_create(500);

  assert(water != NULL);
  assert(water_get_level(water) == 500);

  water_destroy(water);
}

static void test_water_null_safety(void) {
  assert(water_get_level(NULL) == 0);
  water_destroy(NULL); 
}

int main(void) {
  test_water_creation();
  test_water_null_safety();

  return 0;
}
