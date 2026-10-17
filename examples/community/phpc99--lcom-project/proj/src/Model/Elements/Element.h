#ifndef _ELEMENT_H_
#define _ELEMENT_H_

#include "src/include.h"
#include "src/Model/Position.h"
#include "src/Model/Elements/Type.h"

/**
 * @file Element.h
 * @brief Declares the Element structure and related types for map objects.
 *
 * Objects placed on the map such as terrain blocks or trees.
 * May have additional properties.
 */

/**
 * @enum ElementType
 * @brief Represents the type of an element on the map.
 */

typedef enum {
    ELEMENT_EARTH,
    ELEMENT_TREE,
    ELEMENT_WOOD,
    ELEMENT_BEDROCK,
    ELEMENT_NONE,
} ElementType;

/**
 * @enum TreeGrowthStage
 * @brief Represents the current growth stage of a tree.
 */

typedef enum {
    TREE_SAPPLING,
    TREE_MEDIUM,
    TREE_FINAL_STAGE,
} TreeGrowthStage;

/**
 * @struct TreeData
 * @brief Contains data specific to tree elements.
 *
 * Used to track the age and growth stage of a tree.
 */

typedef struct {
    TreeGrowthStage growth_stage;
    int16_t tree_age;
} TreeData;

/**
 * @struct Element
 * @brief Represents an object on the game map.
 *
 * Contains basic attributes like position and size, plus optional data
 * depending on the element type (e.g., tree data).
 */

typedef struct {
    ElementType type;
    Position position;
    int16_t width, height;
    bool block_up;
    union {
        TreeData tree;
    } data;
} Element;

/**
 * @brief Creates and initializes a new Element.
 *
 * @param type The type of the element (e.g., earth, tree).
 * @param position Position of the element.
 * @param width Width of the element in pixels.
 * @param height Height of the element in pixels.
 * @param block_up Whether the element blocks movement above it.
 * @return Pointer to a newly allocated Element, or NULL on failure.
 */

Element *new_Element(ElementType type, Position position, int16_t width, int16_t height, bool block_up);

/**
 * @brief Frees memory allocated for an Element.
 *
 * @param e Pointer to the Element to delete.
 */

void delete_Element(Element *e);

#endif
