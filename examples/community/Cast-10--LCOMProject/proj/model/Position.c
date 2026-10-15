#include "Position.h"

void position_move(Position *pos, int dx, int dy, float speed) {
    if (dx == 0 && dy == 0) return;

    float div = (dx != 0 && dy != 0) ? sqrtf(2.0f) : 1.0f;
    pos->x += (dx / div) * speed;
    pos->y += (dy / div) * speed;
}

int position_px(Position pos) {
    return (int)roundf(pos.x);
}

int position_py(Position pos) {
    return (int)roundf(pos.y);
}
