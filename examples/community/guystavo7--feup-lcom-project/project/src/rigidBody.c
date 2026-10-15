#include "../include/components/rigidBody.h"
/** @file rigidBody.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */
#include "../include/components/boxCollider.h"
#include "../include/utils/vec2d.h"
#include "../include/data.h"
#include <stdlib.h>

struct RigidBody {
    Vec2d velocity;
};

RigidBody* rigidBodyNew() {
    RigidBody* res = malloc(sizeof(RigidBody));
	res->velocity = EMPTY_VEC2D;

    return res;
}

void rigidBodyFree(RigidBody* rb) {
    free(rb);
}

static float moveX(RigidBody* rb, float x, Arena* arena, BoxCollider* hitBox, float deltaTime) {
    float width = boxColliderGetWidth(hitBox);
	float xOffset = boxColliderGetOffsetX(hitBox);
    x += rb->velocity.x * deltaTime;
    boxColliderSetX(hitBox, x);

    BoxCollider* colliders[128];
    int n = arenaDetectCollisions(arena, hitBox, WALLS_TAG, colliders);
    n += arenaDetectCollisions(arena, hitBox, TOWER_TAG, colliders + n);

    for (int i = 0; i < n; i++) {
        if (rb->velocity.x > 0 && (x + xOffset + width) > (boxColliderGetX(colliders[i]) + boxColliderGetOffsetX(colliders[i]))) {
            x = boxColliderGetX(colliders[i]) + boxColliderGetOffsetX(colliders[i]) - width - xOffset;

        } else if (rb->velocity.x < 0 && (x + xOffset) < (boxColliderGetX(colliders[i]) + boxColliderGetOffsetX(colliders[i]) + boxColliderGetWidth(colliders[i]))) {
            x = boxColliderGetX(colliders[i]) + boxColliderGetWidth(colliders[i]) + boxColliderGetOffsetX(colliders[i]) - xOffset;
        }

        rb->velocity.x = 0;
    }

    boxColliderSetX(hitBox, x);
    return x;
}

static float moveY(RigidBody* rb, float y, Arena* arena, BoxCollider* hitBox, float deltaTime) {
    float height = boxColliderGetHeight(hitBox);
	float yOffset = boxColliderGetOffsetY(hitBox);
    y += rb->velocity.y * deltaTime;
    boxColliderSetY(hitBox, y);

    BoxCollider* colliders[128];
    int n = arenaDetectCollisions(arena, hitBox, WALLS_TAG, colliders);
    n += arenaDetectCollisions(arena, hitBox, TOWER_TAG, colliders + n);

    for (int i = 0; i < n; i++) {
        if (rb->velocity.y > 0 && (y + height + yOffset) > (boxColliderGetY(colliders[i])) + boxColliderGetOffsetY(colliders[i])) {
            y = boxColliderGetY(colliders[i]) + boxColliderGetOffsetY(colliders[i]) - height - yOffset;

        } else if (rb->velocity.y < 0 && (y + yOffset) < (boxColliderGetY(colliders[i]) + boxColliderGetOffsetY(colliders[i]) + boxColliderGetHeight(colliders[i]))) {
            y = boxColliderGetY(colliders[i]) + boxColliderGetHeight(colliders[i]) + boxColliderGetOffsetY(colliders[i]) - yOffset;
        }

        rb->velocity.y = 0;
    }

    boxColliderSetY(hitBox, y);
    return y;
}

void rigidBodyUpdate(RigidBody* rb, Vec2d* pos, Arena* arena, BoxCollider* hitBox, float deltaTime) {
    pos->x = moveX(rb, pos->x, arena, hitBox, deltaTime);
    pos->y = moveY(rb, pos->y, arena, hitBox, deltaTime);
}

void rigidBodySetVelocity(RigidBody* rb, int xSpeed, int ySpeed) {
    rb->velocity.x = xSpeed;
    rb->velocity.y = ySpeed;
}
