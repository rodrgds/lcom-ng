#ifndef __plane_h
#define __plane_h



#include "Object.h"
#include "xpm/plane.xpm"
#include "xpm/planeR.xpm"
#include "classes/Score.h"

typedef struct {
    Object* planeObject;
    int state; // 0 and 1 for sprites, 2 for hit
}Plane;


typedef struct {
    Plane** plane;
    int sizePlanes;
} Planes;

Planes *create_PlanesArray(int maxPlaneNumber);

Plane* (createPlane)(Planes *b, bool left);

void (removePlane)(Planes *b, int indexToRemove);

void (PlanesMovement)(Planes *PlanesArray);

void (drawPlanes)(Planes *PlanesArray);

void (planeMovement)(Planes *PlanesArray, int index);

void (freePlanes)(Planes* planes);

#endif
