#include "Champion.h"

/**
 * @brief Full definition of the opaque Champion type.
 */
struct Champion {
    ChampionState state;  /**< Current action state (idle, moving, attacking). */
    Position respawnPos;  /**< Fixed world position the champion returns to on respawn. */
    Position pos;         /**< Current world position. */
    Direction dir;        /**< Current facing direction. */
    Team team;            /**< Team this champion belongs to. */
    int hp;               /**< Current hp points. */
    int max_hp;           /**< Maximum hp points. */
    float speed;          /**< Movement speed in pixels per tick. */
    int attack_dmg;       /**< Damage dealt per attack. */
    int respawn_timer;    /**< Countdown in seconds until respawn; ALIVE when not dead. */
    int width;            /**< Sprite width in pixels. */
    int height;           /**< Sprite height in pixels. */
    int points;           /**< Accumulated kill/score points. */
};

Champion *createChampion(Position respawnPos, Direction dir, int width, int height) {
    Champion *champion = malloc(sizeof(Champion));

    if (champion == NULL) {
        return NULL;
    }

    champion->dir = dir;
    champion->respawnPos = respawnPos;
    champion->pos = respawnPos;
    champion->hp = CHAMPION_HP;
    champion->max_hp = CHAMPION_HP;
    champion->attack_dmg = CHAMPION_ATTACK_DMG;
    champion->speed = CHAMPION_SPEED;
    champion->respawn_timer = ALIVE;
    champion->state = CHAMPION_IDLE;
    champion->width = width;
    champion->height = height;
    champion->points = 0;

    return champion;
}

void destroyChampion(Champion *champion) {
    free(champion);
}

void championRespawn(Champion *champion) {
    champion->hp = champion->max_hp;
    champion->pos = champion->respawnPos;
    champion->state = CHAMPION_IDLE;
}

Position championGetPos(Champion *champion) {
    return champion->pos;
}

Direction championGetDir(Champion *champion) {
    return champion->dir;
}

ChampionState championGetState(Champion *champion) {
    return champion->state;
}

int championGetHp(Champion *champion) {
    return champion->hp;
}

int championGetMaxHp(Champion *champion) {
    return champion->max_hp;
}

float championGetSpeed(Champion *champion) {
    return champion->speed;
}

int championGetAttackDmg(Champion *champion) {
    return champion->attack_dmg;
}

int championGetRespawnTimer(Champion *champion) {
    return champion->respawn_timer;
}

Team championGetTeam(Champion *champion) {
    return champion->team;
}

int championGetWidth(Champion *champion) {
    return champion->width;
}

int championGetHeight(Champion *champion) {
    return champion->height;
}

void championSetState(Champion *champion, ChampionState state) {
    champion->state = state;
}

void championSetDir(Champion *champion, Direction dir) {
    champion->dir = dir;
}

void championSetPos(Champion *champion, Position pos) {
    champion->pos = pos;
}

void championSetHp(Champion *champion, int hp) {
    champion->hp = hp;
}

void championSetRespawnTimer(Champion *champion, int timer) {
    champion->respawn_timer = timer;
}

void championClearPosition(Champion *champion) {
    champion->pos.x = champion->respawnPos.x;
    champion->pos.y = champion->respawnPos.y;
}

void championAddHp(Champion *champion, int delta) {
    champion->hp += delta;
}

void championAddMaxHp(Champion *champion, int delta) {
    champion->max_hp += delta;
}

void championAddSpeed(Champion *champion, int delta) {
    champion->speed += delta;
}

void championAddAttackDmg(Champion *champion, int delta) {
    champion->attack_dmg += delta;
}

Box championGetCollisionBox(Champion *champion) {
    int cw = (int)(champion->width * COLLISION_PROPORTION);
    int ch = (int)(champion->height * COLLISION_PROPORTION);
    return (Box){
        position_px(champion->pos) + (champion->width - cw) / 2,
        position_py(champion->pos) + (champion->height - ch) / 2,
        cw,
        ch
    };
}

Box championGetAttackBox(Champion *champion) {
    int x = position_px(champion->pos);
    int y = position_py(champion->pos);
    int w = champion->width;
    int h = champion->height;
    int ox = (int)(ATTACK_RANGE_OFFSET * w);
    int oy = (int)(ATTACK_RANGE_OFFSET * h);

    switch (champion->dir) {
        case NORTH:
            return (Box){ x, y - oy, w, h / 2 + oy };
        case SOUTH:
            return (Box){ x, y + h / 2, w, h / 2 + oy };
        case EAST:
            return (Box){ x + w / 2, y, w / 2 + ox, h };
        case WEST:
            return (Box){ x - ox, y, w / 2 + ox, h };
        case NORTHEAST:
            return (Box){ x + w / 2, y - oy, w / 2 + ox, h / 2 + oy };
        case NORTHWEST:
            return (Box){ x - ox, y - oy, w / 2 + ox, h / 2 + oy };
        case SOUTHEAST:
            return (Box){ x + w / 2, y + h / 2, w / 2 + ox, h / 2 + oy };
        case SOUTHWEST:
            return (Box){ x - ox, y + h / 2, w / 2 + ox, h / 2 + oy };
        default:
            return (Box){ x, y, w, h };
    }
}

bool championAttackHits(Champion *attacker, Champion *target) {
    Box attack = championGetAttackBox(attacker);
    Box hit = (Box){
        position_px(target->pos),
        position_py(target->pos),
        target->width,
        target->height
    };
    return boxesOverlap(attack, hit);
}

int championCurrentPoints(Champion *champion) { 
    return champion->points;
}

void championUpdatePoints(Champion *champion, int delta) {
    if (!champion) return;
    champion->points += delta;
}
