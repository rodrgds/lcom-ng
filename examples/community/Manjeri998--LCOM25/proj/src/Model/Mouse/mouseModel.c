#include "mouseModel.h"

Mouse createMouse(uint16_t x, uint16_t y) {
    Mouse m;
    m.sprite = generate_Sprite((xpm_map_t) MouseXPM);
    m.pos.x = x;
    m.pos.y = y;
    return m;
}

void destroyMouse(Mouse m) {
    freeSprite(m.sprite);
}
