#include "entry.h"

Entry *createEntry(int x, int y, bool selected, Sprite *sprite) {
    Entry *entry = (Entry *) malloc(sizeof(Entry));
    if (entry == NULL) {
        return NULL; // Memory allocation failed
    }
    entry->x = x;
    entry->y = y;
    entry->selected = selected;
    entry->sprite = sprite;
    return entry;
}

void destroyEntry(Entry *entry) {
    if (entry != NULL) {
        if (entry->sprite != NULL) {
            destroy_sprite(entry->sprite);
        }
        free(entry);
    }
}

int drawEntry(Entry *entry) {
    if (entry != NULL && entry->sprite != NULL) {
        draw_sprite(entry->sprite, entry->x, entry->y);
        return 0;
    }
    return 1;
}
void toggleEntry(Entry *entry, Sprite *sprite, Sprite *highlighted_sprite) {
    if (entry != NULL) {
        entry->selected = !entry->selected;
    }

    if (entry->selected) {
        entry->sprite = highlighted_sprite; 
    } else {
        entry->sprite = sprite; 
    }
}
