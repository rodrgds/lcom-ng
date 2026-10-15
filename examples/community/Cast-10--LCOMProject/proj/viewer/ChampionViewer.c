#include "ChampionViewer.h"

ChampionViewer *createChampionViewer(AnimSprite **animations, int numAnimations) {
    if (animations == NULL || numAnimations <= 0) {
        return NULL;
    }

    ChampionViewer *viewer = malloc(sizeof(ChampionViewer));

    if (viewer == NULL) {
        return NULL;
    }

    viewer->animations = animations;
    viewer->numAnimations = numAnimations;
    viewer->currentAnimation = 0;
    viewer->hpBar = createHpBarViewer();
    if (!viewer->hpBar) {
        free(viewer);
        return NULL;
    }

    return viewer;
}

void destroyChampionViewer(ChampionViewer *viewer) {
    if (viewer == NULL) {
        return;
    }

    for (int i = 0; i < viewer->numAnimations; i++) {
        destroyAnimSprite(viewer->animations[i]);
    }

    destroyHpBarViewer(viewer->hpBar);
    free(viewer);
}

/**
 * @brief Maps diagonal directions to their nearest cardinal equivalent.
 *
 * Since only 4-direction sprites exist, NORTHEAST/NORTHWEST collapse to NORTH
 * and SOUTHEAST/SOUTHWEST collapse to SOUTH. All cardinal directions pass through unchanged.
 *
 * @param dir The direction to map.
 * @return The nearest supported cardinal Direction.
 */
static Direction mapDir(Direction dir) {
    switch (dir) {
        case NORTHEAST: return NORTH;
        case NORTHWEST: return NORTH;
        case SOUTHEAST: return SOUTH;
        case SOUTHWEST: return SOUTH;
        default: return dir;
    }
}

bool championViewerUpdate(ChampionViewer *viewer, Champion *champion) {
    if (viewer == NULL || champion == NULL) {
        return false;
    }

    int targetAnimation;
    ChampionState state = championGetState(champion);
    Direction dir = mapDir(championGetDir(champion));

    switch (state) {
        case CHAMPION_MOVING:
            targetAnimation = (int)dir + 8;
            break;
        case CHAMPION_ATTACKING:
            if (viewer->currentAnimation >= 4 && viewer->currentAnimation < 8) {
                targetAnimation = viewer->currentAnimation;
            } else {
                targetAnimation = (int)dir + 4;
            }
            break;
        default:
            targetAnimation = (int)dir;
            break;
    }

    if (viewer->currentAnimation != targetAnimation) {
        viewer->currentAnimation = targetAnimation;
        animSpriteReset(viewer->animations[targetAnimation]);
    }

    updateAnimSprite(viewer->animations[viewer->currentAnimation]);

    return animSpriteHasLooped(viewer->animations[viewer->currentAnimation]);
}

int championDraw(ChampionViewer *viewer, Champion *champion) {
    if (viewer == NULL || champion == NULL || championGetHp(champion) <= 0) {
        return 0;
    }

    Position pos = championGetPos(champion);

    setAnimSpritePosition(
        viewer->animations[viewer->currentAnimation],
        position_px(pos),
        position_py(pos)
    );

    hpBarViewerDraw(viewer->hpBar,
                    position_px(pos), position_py(pos),
                    championGetWidth(champion),
                    championGetHp(champion),
                    championGetMaxHp(champion));

    return drawAnimSprite(viewer->animations[viewer->currentAnimation]);
}
