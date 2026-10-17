#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "macros.h"
#include "gameObjects.h"
#include "stateMachine.h"

GameObject createGameObject(char* label, double x, double y, double vx, double vy, char* bmpPath, short transparentColor){
  GameObject g;
  g.label = label;
  g.x = x;
  g.y = y;
  g.vx = vx;
  g.vy = vy;
  Bitmap* bmp = loadBitmap(bmpPath);
  g.bmp = bmp;
  g.bmpPath = bmpPath;
  g.transparentColor = transparentColor;

  g.sizeX = bmp->bitmapInfoHeader.width;
  g.sizeY = bmp->bitmapInfoHeader.height;
  return g;
}

ObjLayer createObjLayer(){
    ObjLayer array;
    array.gameObjects = malloc(0);
    array.size = 0;
    return array;
}

ObjLayer* ptr_createObjLayer(){
    ObjLayer* array = malloc(sizeof(ObjLayer));
    array->gameObjects = malloc(sizeof(GameObject));
    array->size = 0;
    return array;
}

void createActiveObjectsList(ObjLayer* activeObjects, unsigned int size){
    unsigned int i;
    for(i = 0; i < size; i++){
        (activeObjects)[i] = createObjLayer();
    }
}

GameObject* zIndexPush(ObjLayer* array, unsigned int zIndex, GameObject g){
    if(zIndex <= Z_INDEX_MAX && zIndex >= 0){
        //[DEBUG] printf("Length: %d\n", array[zIndex].size);
        array[zIndex].gameObjects = realloc(array[zIndex].gameObjects, sizeof(GameObject)*(array[zIndex].size + 1));
        g.zIndex = zIndex;
        array[zIndex].gameObjects[array[zIndex].size] = g;
        //[DEBUG]printf("Adding object. X value: %f\n", array[zIndex].gameObjects[array[zIndex].size].x);
        array[zIndex].size++;
        return &array[zIndex].gameObjects[array[zIndex].size - 1];
    }
    return NULL;
}

bool zIndexRemoveByIndex(ObjLayer* activeObjects, unsigned int zIndex, unsigned int objIndex){
    if(zIndex <= Z_INDEX_MAX && zIndex >= 0){
        GameObject* newArray = malloc((activeObjects[zIndex].size - 1)*sizeof(GameObject));
        if(objIndex != 0) memcpy(newArray, activeObjects[zIndex].gameObjects, sizeof(GameObject) * objIndex);
        memcpy(&newArray[objIndex], &activeObjects[zIndex].gameObjects[objIndex+1], sizeof(GameObject)*(activeObjects[zIndex].size - (objIndex+1)));
        freeGameObject(&activeObjects[zIndex].gameObjects[objIndex]);
        free(activeObjects[zIndex].gameObjects);
        activeObjects[zIndex].gameObjects = newArray;
        activeObjects[zIndex].size--;
        return true;
    }
    return false;
}

bool zIndexRemove(ObjLayer* activeObjects, GameObject* g){
    unsigned int zIndex = g->zIndex;
    if(zIndex <= Z_INDEX_MAX && zIndex >= 0){
        //Check if object exists
        int objIndex;
        bool objExistsInZIndex = false;
        for(objIndex = 0; objIndex < activeObjects[zIndex].size; objIndex++){
          if(&activeObjects[zIndex].gameObjects[objIndex] == g){
            objExistsInZIndex = true;
            break;
          }
        }
        if(!objExistsInZIndex) return false;

        //Remove object
        GameObject* newArray = malloc((activeObjects[zIndex].size - 1)*sizeof(GameObject));
        if(objIndex != 0) memcpy(newArray, activeObjects[zIndex].gameObjects, sizeof(GameObject) * objIndex);
        memcpy(&newArray[objIndex], &activeObjects[zIndex].gameObjects[objIndex+1], sizeof(GameObject)*(activeObjects[zIndex].size - (objIndex+1)));
        freeGameObject(&activeObjects[zIndex].gameObjects[objIndex]);
        free(activeObjects[zIndex].gameObjects);
        activeObjects[zIndex].gameObjects = newArray;
        activeObjects[zIndex].size--;
        return true;
    }
    return false;
}

void drawObject(GameObject* g){
  if (!g->bmp) printf("BMP IS NULL\n");
  drawBitmap(g->bmp, g->x, g->y, g->transparentColor);
}

void objLayer_free(ObjLayer* objects) {
  int i;
  for(i = 0; i <= Z_INDEX_MAX; i++) {
      gameObjects_free(objects[i].gameObjects);
  }
  free(objects);
}

void gameObjects_free(GameObject* gameObjects) {
  int i;
  for(i = 0; i < sizeof(gameObjects)/sizeof(GameObject); i++){
    freeGameObject(&gameObjects[i]);
  }
  free(gameObjects);
}

void freeGameObject(GameObject* g) {
  deleteBitmap(g->bmp);
}

GameObject* getObjectFromLabel(ObjLayer* objects, char* label){
  int z, i;
  for(z = 0; z <= Z_INDEX_MAX; z++)
  for(i = 0; i < objects[z].size; i++){
    if(strcmp(objects[z].gameObjects[i].label, label) == 0){
      return &(objects[z].gameObjects[i]);
    }
  }
  return NULL;
}

//[DEBUG]
void printObjectArray(ObjLayer* activeObjects){
    printf("----------------------\n---PRINTING LAYERS---\n---------------------\n");
    unsigned int i,z;
    for(z = 0; z <= Z_INDEX_MAX; z++){
        ObjLayer layerObjs = activeObjects[z];
        printf("----------------\nLAYER %d\n", z);
        for(i = 0; i < layerObjs.size; i++){
            printf("GameObject%d: x=%f, y=%f\n", i, layerObjs.gameObjects[i].x, layerObjs.gameObjects[i].y);
        }
        printf("Layer %d has %d objects\n", z, layerObjs.size);
    }
}
