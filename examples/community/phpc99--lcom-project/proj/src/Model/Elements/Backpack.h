#ifndef _BACKPACK_H_
#define _BACKPACK_H_

#include "src/include.h"
#include "src/Model/Elements/Type.h"
#include "src/Model/Elements/Element.h"

/**
 * @file Backpack.h
 * @brief Declares the Backpack structure and related inventory management functions.
 *
 * A backpack stores items collected by the player, handling storage,
 * selection, usage, and type conversions.
 */

#define BACKPACK_SIZE 5

/**
 * @struct Backpack
 * @brief Represents the player's inventory system.
 *
 * The backpack holds a fixed number of item slots, along with the current
 * selection index and item quantities.
 */

typedef struct{
    int16_t selected;
    ItemType slots[BACKPACK_SIZE];
    size_t quantity[BACKPACK_SIZE];
    size_t capacity;
} Backpack;

/**
 * @brief Allocates and initializes a new Backpack.
 *
 * @param initial_capacity Initial number of usable slots (up to BACKPACK_SIZE).
 * @return Pointer to the new Backpack, or NULL if allocation fails.
 */

Backpack *new_Backpack(size_t initial_capacity);

/**
 * @brief Frees the memory associated with a Backpack.
 *
 * @param b Pointer to the Backpack to delete.
 */

void delete_Backpack(Backpack *b);

/**
 * @brief Adds an item to the backpack.
 *
 * If the item type already exists in a slot, its quantity is increased.
 * Otherwise, it is placed in an empty slot (if available).
 *
 * @param b Pointer to the Backpack.
 * @param element Pointer to the Element to add.
 */

void Backpack_add(Backpack *b, Element *element);

/**
 * @brief Removes one unit of the specified item type from the backpack.
 *
 * @param b Pointer to the Backpack.
 * @param item_type Type of item to remove.
 * @return True if successful, false if the item was not found or empty.
 */

bool Backpack_remove(Backpack *b, ItemType item_type);

/**
 * @brief Changes the currently selected inventory slot.
 *
 * @param b Pointer to the Backpack.
 * @param slot Index of the slot to select.
 */

void backpack_select(Backpack *b, int slot);

/**
 * @brief Uses one item from the currently selected slot.
 *
 * @param b Pointer to the Backpack.
 * @return True if the item was used, false if the slot is empty.
 */

bool backpack_use_selected(Backpack *b);

/**
 * @brief Converts an ElementType to the corresponding ItemType.
 *
 * @param etype ElementType to convert.
 * @return Corresponding ItemType.
 */

ItemType elementType_to_itemType(ElementType etype);

/**
 * @brief Converts an ItemType to the corresponding ElementType.
 *
 * @param itype ItemType to convert.
 * @return Corresponding ElementType.
 */

ElementType itemType_to_elementType(ItemType itype);

#endif

