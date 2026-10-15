#include "../../include/monster/path.h"
/** @file path.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */

struct path {
    float pcd; // point change distance
    Vector* points;
    Vector* sub_keys;   // subscriptor keys
    Vector* sub_values; // subscriptor mvalues (map to the points)
};

Path* pathNew(float point_change_distance, Vector* points) {

    if ((vectorGetDataSize(points) != 4) || (vectorGetSize(points) % 2 != 0) || (vectorGetSize(points) == 0) || (0 > point_change_distance)) {
        printf("points parameter must be a 4-byte-data-size vector with a even number (different of 0) of floats that correspont to pairs (x,y). point_change_distance should be>0");
        fflush(stdout);
        return NULL;
    }
    Path* p = malloc(sizeof(Path));
    p->pcd = point_change_distance;

    p->sub_keys = vectorCreate(sizeof(void*), NULL);
    p->sub_values = vectorCreate(4, NULL);
    p->points = vectorCreate(4, NULL);
    for (unsigned int i = 0; i < vectorGetSize(points); i++) {
        vectorPushBack(p->points, vectorGet(points, i));
    }
    return p;
}

void pathFree(Path* p) {
    vectorFree(p->points);
    vectorFree(p->sub_keys);
    vectorFree(p->sub_values);
    free(p);
}

void pathSubscribe(Path* p, void* pointer) {
    unsigned long long int pointer_val = (unsigned long long int)pointer;
    vectorPushBack(p->sub_keys, (char*)&pointer_val);
    vectorPushBackVal(p->sub_values, 0);
}

void pathGoToNextPoint(Path* p, void* pointer) {

    int i = vectorFind(p->sub_keys, (char*)&pointer);
    if (i == -1) {
        pathSubscribe(p, pointer);
        return;
    }
    unsigned int ui = i;
    printf("cur: %d\n",i);
    fflush(stdout);
    unsigned int points_size = vectorGetSize(p->points) / 2;
    vectorSetVal(p->sub_values, ui, (vectorGetVal(p->sub_values, ui) + 1) % points_size);
}

void pathGetCurPoint(Path* p, void* pointer, Vec2d* curPoint) {

    int i = vectorFind(p->sub_keys, (char*)&pointer);
    if (i == -1) {
        pathSubscribe(p, pointer);
    } 
    
    i = vectorFind(p->sub_keys, (char*)&pointer); 
    unsigned int ui = i;
    unsigned int point_index = vectorGetVal(p->sub_values, ui) * 2;

    float ppx, ppy; // path point x ,y
    ppx = *(float*)vectorGet(p->points, point_index);
    ppy = *(float*)vectorGet(p->points, point_index + 1);
    if (curPoint == NULL) {
        printf("curPoint should be initialized");
        fflush(stdout);
    }
    vec2dSetX(curPoint, ppx);
    vec2dSetY(curPoint, ppy);
}

bool pathCheckInPointChangeDistance(Path* p, void* pointer, Vec2d* pos) {
    Vec2d* path_point = vec2dCreate(0, 0);
    pathGetCurPoint(p, pointer, path_point);

    vec2dSubVec(path_point, pos);
    float dist2 = vec2dModule2(path_point);
    vec2dFree(path_point);
    return (p->pcd) * (p->pcd) >= dist2;
}

void pathGetDirectionToCurPoint(Path* p, void* pointer, Vec2d* pos, Vec2d* new_dir) {
    if (new_dir == NULL) {
        printf("new_dir should be initialized");
        fflush(stdout);
    }
    Vec2d* path_point = vec2dCreate(0, 0);
    pathGetCurPoint(p, pointer, path_point);

    vec2dSubVec(path_point, pos);
    vec2dNormalize(path_point);

    vec2dSetX(new_dir, vec2dGetX(path_point));
    vec2dSetY(new_dir, vec2dGetY(path_point));

    vec2dFree(path_point);
}

void pathFollowingDir(Path* p, void* pointer, Vec2d* pos, Vec2d* new_dir) {
 
    fflush(stdout);
    if (new_dir == NULL || pos == NULL) {
        printf("new_dir and pos should be initialized");
        fflush(stdout);
    }

    
    if (pathCheckInPointChangeDistance(p, pointer, pos)) {
        pathGoToNextPoint(p, pointer);
    }
    pathGetDirectionToCurPoint(p, pointer, pos, new_dir);
}

unsigned int pathGetPointsSize(Path* path){
    return vectorGetSize(path->points);
}



void pathGetPoint(Path* p,unsigned int i,Vec2d* point){
    
    unsigned int point_index = i * 2;

    //float ppx, ppy; // path point x ,y
    point->x = *(float*)vectorGet(p->points, point_index);
    point->y = *(float*)vectorGet(p->points, point_index + 1);
     
}

unsigned int pathGetNumOfPoints(Path* path){
    return vectorGetSize(path->points)/2;
}

unsigned int pathGetCurPointIndex(Path* p, void* pointer) {

    int i = vectorFind(p->sub_keys, (char*)&pointer);
    if (i == -1) {
        pathSubscribe(p, pointer);
    } 
    i = vectorFind(p->sub_keys, (char*)&pointer); 
    
    return vectorGetVal(p->sub_values, i);
}

void pathSetCurPointIndex(Path* p,void* pointer,unsigned int index){
    int i = vectorFind(p->sub_keys, (char*)&pointer);
    if (i == -1) {
        pathSubscribe(p, pointer);
    } 
    i = vectorFind(p->sub_keys, (char*)&pointer); 
    
    vectorSetVal(p->sub_values, i,index);
}
