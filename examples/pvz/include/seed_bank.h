#ifndef _SEED_BANK_H_
#define _SEED_BANK_H_

#include <stdbool.h>
#include "plant.h"

PlantType get_seed_slot_type(int slot);
int get_seed_slot_x(int slot);
int get_seed_slot_for_type(PlantType type);
bool get_seed_from_position(int x, int y, PlantType *type);

/* Aplica um deck personalizado ao seed bank */
void set_seed_slots(const PlantType *types, int count);

#endif /* _SEED_BANK_H_ */
