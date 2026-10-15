#include "../include/utils/vec2d.h"
/** @file vec2d.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */

Vec2d* vec2dCreate(float x, float y) {
    Vec2d* v = malloc(sizeof(Vec2d));
    v->x = x;
    v->y = y;

    return v;
}

Vec2d* vec2dCreateWithVec2d(Vec2d* v2) {
    Vec2d* v = malloc(sizeof(Vec2d));
    vec2dSetWithVec2d(v, v2);
    return v;
}

void vec2dFree(Vec2d* v) {
    free(v);
}

float vec2dGetX(Vec2d* v) {
    return v->x;
}
float vec2dGetY(Vec2d* v) {
    return v->y;
}

void vec2dSetX(Vec2d* v, float x) {
    v->x = x;
}

void vec2dSetY(Vec2d* v, float y) {
    v->y = y;
}

void vec2dSetWithVec2d(Vec2d* v, Vec2d* v2) {
    v->x = v2->x;
    v->y = v2->y;
}

float vec2dModule2(Vec2d* v) {
    float m = v->x * v->x + v->y * v->y;
    return m;
}

float vec2dModule(Vec2d* v) {
    float m = sqrt(v->x * v->x + v->y * v->y);
    return m;
}

float vec2dDistance2Between(Vec2d* v1,Vec2d* v2){
    Vec2d v3={v1->x-v2->x,v1->y-v2->y};
    return vec2dModule2(&v3);
}

void vec2dNormalize(Vec2d* v) {
    if (v->x == 0.0f && v->y == 0.0f) {
        return;
    }
    float m = vec2dModule(v);
    v->x /= m;
    v->y /= m;
}

void vec2dMultiply(Vec2d* v, float delta) {
    v->x *= delta;
    v->y *= delta;
}

void vec2dAddVec(Vec2d* v, Vec2d* v2) {
    v->x += v2->x;
    v->y += v2->y;
}

void vec2dSubVec(Vec2d* v, Vec2d* v2) {
    v->x -= v2->x;
    v->y -= v2->y;
}

