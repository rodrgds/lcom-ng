#include "selectorMine.h"

SMine createSMine() {
    SMine m;
    m.sprite = generate_Sprite((xpm_map_t) BigMine);
    m.pos.x = INIT_POS_X;
    m.pos.y = INIT_POS_Y;
    return m;
}

void setSMinePosition(SMine* mine, uint16_t x, uint16_t y) {
   mine->pos.x = x;
   mine->pos.y = y;
}

void destroySMine( SMine* mine) {
    free(mine->sprite);
}
