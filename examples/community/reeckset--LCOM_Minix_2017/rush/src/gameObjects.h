#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H


/** @defgroup gameObjects gameObjects
 * @{
 *
 * Functions to manage game objects
 */

#include <stdbool.h>
#include "bitmap.h"

typedef struct{
    double x,y,vx,vy;
    int sizeX, sizeY;
    unsigned int zIndex;
    char* label;
    Bitmap* bmp;
    char* bmpPath;
    short transparentColor;
} GameObject;

typedef struct{
    GameObject* gameObjects;
    unsigned int size;
} ObjLayer;


/**
 * @brief Creates an ObjLayer
 *
 *
 * @return pointer to created ObjLayer
 */
ObjLayer* ptr_createObjLayer();

ObjLayer createObjLayer();

/**
 * @brief Creates an GameObject
 *
 *
 * @return pointer to created GameObject
 */
GameObject createGameObject(char* label, double x, double y, double vx, double vy, char* bmpPath, short trasparentColor);

/**
 * @brief Releases the memory occupied by given GameObject
 *
 * @param g - GameObject pointer to free
 * @return None
 */
void freeGameObject(GameObject* g);

/**
 * @brief Creates an array of ObjLayers to store different GameObjects
 *
 * @param activeObjects - array pointer
 * @param size - desired size
 *
 * @return None
 */
void createActiveObjectsList(ObjLayer* activeObjects, unsigned int size);

/**
 * @brief Add a GameObject to a layer in a given ObjLayers array
 *
 * @param array - ObjLayer array
 * @param zIndex - zIndex (layer index) to add the object
 * @param g - GameObject to add
 *
 * @return pointer to the addded GameObject
 */
GameObject* zIndexPush(ObjLayer* array, unsigned int zIndex, GameObject g);

/**
 * @brief Remove a GameObject of a layer in a given ObjLayers array
 *
 * @param activeObjects - ObjLayer array
 * @param zIndex - zIndex (layer index) of which to remove the object
 * @param objIndex - GameObject index to remove
 *
 * @return true on success, false otherwise
 */
bool zIndexRemoveByIndex(ObjLayer* activeObjects, unsigned int zIndex, unsigned int objIndex);

/**
 * @brief Remove a GameObject of a layer in a given ObjLayers array
 *
 * @param activeObjects - ObjLayer array
 * @param g - GameObject pointer to remove
 *
 * @return true on success, false otherwise
 */
bool zIndexRemove(ObjLayer* activeObjects, GameObject* g);
/**
 * @brief Draw a GameObject on the screen
 *
 * @param g - GameObject pointer to draw
 *
 * @return none
 */
void drawObject(GameObject* g);

/**
 * @brief Get a GameObject from its label (returns the first element found)
 *
 * @param activeObjects - ObjLayer array
 * @param label - label to search
 *
 * @return first GameObject* found with that label or NULL if not found
 */
GameObject* getObjectFromLabel(ObjLayer* activeObjects, char* label);

void printObjectArray(ObjLayer* activeObjects); //[DEBUG]

/**
 * @brief Free the memory used by an ObjLayers array
 *
 * @param objects - ObjLayer array to free
 *
 * @return none
 */
void objLayer_free(ObjLayer* objects);

/**
 * @brief Free the memory used by a GameObject
 *
 * @param gameObjects - object to free
 *
 * @return none
 */
void gameObjects_free(GameObject* gameObjects);

/** @} end of gameObjects*/

#endif
