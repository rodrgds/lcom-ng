#include "Element.h"

/**
 * @brief Creates and initializes a new Element.
 *
 * Allocates memory for an Element and sets its type, position, size,
 * and blocking behavior. 
 *
 * @param type The type of the element (e.g., tree, earth, bedrock).
 * @param position The position of the element on the map.
 * @param width The width of the element in pixels.
 * @param height The height of the element in pixels.
 * @param block_up Whether this element blocks movement from above.
 * @return Pointer to the newly created Element, or NULL if allocation fails.
 */

Element *new_Element(ElementType type, Position position, int16_t width, int16_t height, bool block_up) {
    Element *e = malloc(sizeof(*e));
    if (!e) return NULL;
    e->type = type;
    e->position = position;
    e->width = width;
    e->height = height;
    e->block_up = block_up;

    switch (type) {
        case ELEMENT_TREE:
            e->data.tree.growth_stage = TREE_SAPPLING;
            e->data.tree.tree_age = 0;
            break;
        default:
            break;
    }
    return e;
}

/**
 * @brief Frees the memory allocated for an Element.
 *
 * @param e Pointer to the Element to delete.
 */

void delete_Element(Element *e) {
    free(e);
}
