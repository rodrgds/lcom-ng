#include "mouseView.h"

int drawMouse(Mouse m) {
    if (draw_Sprite(m.sprite, m.pos.x, m.pos.y) != 0) return 1;
    return 0;
}
