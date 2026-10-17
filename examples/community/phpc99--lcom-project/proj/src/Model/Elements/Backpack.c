#include "Backpack.h"

/**
 * @brief Creates and initializes a new Backpack.
 *
 * Initializes all slots to `ITEM_NONE` and sets their quantities to zero.
 *
 * @param initial_capacity Maximum quantity allowed per slot.
 * @return Pointer to the new Backpack, or NULL if allocation fails.
 */

Backpack *new_Backpack(size_t initial_capacity) {
  Backpack *b = malloc(sizeof(*b));
  if (!b)
    return NULL;

  b->selected = 0;
  b->capacity = initial_capacity;
  for (int i = 0; i < BACKPACK_SIZE; i++) {
    b->slots[i] = ITEM_NONE;
    b->quantity[i] = 0;
  }
  return b;
}

/**
 * @brief Frees memory associated with a Backpack.
 *
 * @param b Pointer to the Backpack to delete.
 */

void delete_Backpack(Backpack *b) {
  free(b);
}

/**
 * @brief Adds an item to the backpack.
 *
 * Handles special logic for trees, adding both wood and sapplings based on tree stage.
 * Otherwise, adds a single unit of the corresponding item type.
 *
 * @param b Pointer to the Backpack.
 * @param element Pointer to the Element to add.
 */

void  Backpack_add(Backpack *b, Element *element) {
  ElementType element_type = element->type;
  ItemType type = elementType_to_itemType(element_type);

  if (type == ITEM_NONE)
    return;

  if (element_type == ELEMENT_TREE) {
    bool wood_added = false;
    int16_t rest = 0;

    for (int i = 0; i < BACKPACK_SIZE; i++) {
      if (b->slots[i] == ITEM_WOOD && b->quantity[i] < b->capacity) {
        int wood_to_add = 0;

        if (element->data.tree.growth_stage == TREE_SAPPLING) {
          wood_to_add = 0;
        }
        else if (element->data.tree.growth_stage == TREE_MEDIUM) {
          wood_to_add = 1;
        }
        else if (element->data.tree.growth_stage == TREE_FINAL_STAGE) {
          wood_to_add = 2;
        }

        if (b->quantity[i] + wood_to_add <= b->capacity) {
          b->quantity[i] += wood_to_add;
          wood_added = true;
          break;
        }
        else if (b->quantity[i] + wood_to_add > b->capacity) {
          rest = wood_to_add - (b->capacity - b->quantity[i]);
          b->quantity[i] = b->capacity;
          wood_added = true;
          break;
        }
      }
    }

    if (!wood_added || rest > 0) {
      for (int i = 0; i < BACKPACK_SIZE; i++) {

        if (b->slots[i] == ITEM_NONE) {
          if (rest > 0) {
            b->slots[i] = ITEM_WOOD;
            b->quantity[i] = rest;
            wood_added = true;
            break;
          }
          else if (!wood_added) {
            if (element->data.tree.growth_stage == TREE_FINAL_STAGE) {
              b->slots[i] = ITEM_WOOD;
              b->quantity[i] = 2;
              wood_added = true;
            }
            else if (element->data.tree.growth_stage == TREE_MEDIUM) {
              b->slots[i] = ITEM_WOOD;
              b->quantity[i] = 1;
              wood_added = true;
            }
          }

          break;
        }
      }
    }

    bool sappling_added = false;
    int16_t sappling_rest = 0;
    for (int i = 0; i < BACKPACK_SIZE; i++) {
      if (b->slots[i] == ITEM_SAPPLING && b->quantity[i] < b->capacity) {
        int sappling_to_add = 0;
        if (element->data.tree.growth_stage == TREE_FINAL_STAGE) {
          sappling_to_add = 2;
        }
        else {
          sappling_to_add = 1;
        }

        if (b->quantity[i] + sappling_to_add <= b->capacity) {
          b->quantity[i] += sappling_to_add;
          sappling_added = true;
          break;
        }
        else if (b->quantity[i] + sappling_to_add > b->capacity) {
          sappling_rest = sappling_to_add - (b->capacity - b->quantity[i]);
          b->quantity[i] = b->capacity;
          sappling_added = true;
          break;
        }
      }
    }

    if (!sappling_added || sappling_rest > 0) {
      for (int i = 0; i < BACKPACK_SIZE; i++) {
        if (b->slots[i] == ITEM_NONE) {
          b->slots[i] = ITEM_SAPPLING;
          if (sappling_rest > 0) {
            b->quantity[i] = sappling_rest;
            sappling_added = true;
            break;
          }
          else if (!sappling_added) {
            if (element->data.tree.growth_stage == TREE_FINAL_STAGE) {
              b->quantity[i] = 2;
              sappling_added = true;
            }
            else {
              b->quantity[i] = 1;
              sappling_added = true;
            }
            return;
          }
        }
      }
    }

    return;
  }
  for (int i = 0; i < BACKPACK_SIZE; i++) {
    if (b->slots[i] == type && b->quantity[i] < b->capacity) {
      b->quantity[i]++;
      return;
    }
  }

  for (int i = 0; i < BACKPACK_SIZE; i++) {
    if (b->slots[i] == ITEM_NONE) {
      b->slots[i] = type;
      b->quantity[i] = 1;
      return;
    }
  }
}

/**
 * @brief Removes one unit of the specified item type.
 *
 * If quantity reaches 0, the slot is cleared.
 *
 * @param b Pointer to the Backpack.
 * @param type Type of item to remove.
 * @return True if item was successfully removed, false otherwise.
 */

bool Backpack_remove(Backpack *b, ItemType type) {
  for (int i = 0; i < BACKPACK_SIZE; i++) {
    if (b->slots[i] == type && b->quantity[i] > 0) {
      b->quantity[i]--;
      if (b->quantity[i] == 0)
        b->slots[i] = ITEM_NONE;
      return true;
    }
  }
  return false;
}

/**
 * @brief Changes the currently selected inventory slot.
 *
 * @param b Pointer to the Backpack.
 * @param slot Inventory slot index (1-based).
 */

void backpack_select(Backpack *b, int slot) {
  if (slot >= 0 && slot <= BACKPACK_SIZE)
    b->selected = slot - 1;
}

/**
 * @brief Uses one unit from the currently selected slot.
 *
 * @param b Pointer to the Backpack.
 * @return True if an item was used, false if the slot was empty.
 */

bool backpack_use_selected(Backpack *b) {
  int sel = b->selected;
  if (b->slots[sel] == ITEM_NONE || b->quantity[sel] == 0)
    return false;

  b->quantity[sel]--;
  if (b->quantity[sel] == 0)
    b->slots[sel] = ITEM_NONE;
  return true;
}

/**
 * @brief Converts an ElementType to the corresponding ItemType.
 *
 * @param etype The element type to convert.
 * @return The corresponding item type.
 */

ItemType elementType_to_itemType(ElementType etype) {
  switch (etype) {
    case ELEMENT_EARTH: return ITEM_EARTH;
    case ELEMENT_TREE: return ITEM_WOOD;
    case ELEMENT_WOOD: return ITEM_WOOD;
    default: return ITEM_NONE;
  }
}

/**
 * @brief Converts an ItemType to the corresponding ElementType.
 *
 * @param itype The item type to convert.
 * @return The corresponding element type.
 */

ElementType itemType_to_elementType(ItemType itype) {
  switch (itype) {
    case ITEM_EARTH: return ELEMENT_EARTH;
    case ITEM_WOOD: return ELEMENT_WOOD;
    case ITEM_SAPPLING: return ELEMENT_TREE;
    default: return ELEMENT_NONE;
  }
}
