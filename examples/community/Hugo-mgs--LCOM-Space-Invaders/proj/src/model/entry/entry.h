#pragma once

#include <lcom/lcf.h>
#include "../../sprites/sprite.h"

typedef struct {
    int x, y;
    bool selected;
    Sprite *sprite;
} Entry;

/**
 * Creates a new Entry object.
 * 
 * @param x The x-coordinate of the entry.
 * @param y The y-coordinate of the entry.
 * @param selected The initial selection state of the entry.
 * @param sprite Pointer to the sprite associated with the entry.
 * @return Pointer to the newly created Entry, or NULL if memory allocation fails.
 */
Entry *createEntry(int x, int y, bool selected, Sprite *sprite);
 
/**
 * @brief Destroys an Entry object.
 * 
 * @param entry Pointer to the Entry to be destroyed.
 */
void destroyEntry(Entry *entry);
 
/**
 * Draws the specified Entry on the screen.
 * 
 * @param entry Pointer to the Entry to be drawn.
 * @return 0 on success, 1 if the entry or its sprite is NULL.
 */
int drawEntry(Entry *entry);
 
/**
 * Toggles the selection state of a specified Entry and updates its sprite.
 * 
 * @param entry Pointer to the Entry to be toggled.
 * @param sprite Pointer to the normal sprite.
 * @param highlighted_sprite Pointer to the highlighted sprite.
 */
void toggleEntry(Entry *entry, Sprite *sprite, Sprite *highlighted_sprite);
