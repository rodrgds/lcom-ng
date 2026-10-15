#include "MinionViewer.h"

#define MINION_ANIM_TIME 25

MinionViewer *createMinionViewer(xpm_row_t *stop_xpm, xpm_row_t *atk_xpm) {
    MinionViewer *viewer = malloc(sizeof(MinionViewer));
    if (!viewer) return NULL;

    Sprite **idle_frames = malloc(2 * sizeof(Sprite *));
    if (!idle_frames) { 
        free(viewer);
        return NULL;
    }

    idle_frames[0] = spriteCreate(stop_xpm);
    idle_frames[1] = spriteCreate(stop_xpm);

    if (!idle_frames[0] || !idle_frames[1]) {
        if (idle_frames[0]) spriteDestroy(idle_frames[0]);
        if (idle_frames[1]) spriteDestroy(idle_frames[1]);
        free(idle_frames); free(viewer); return NULL;
    }
    viewer->idle = createAnimSprite(MINION_ANIM_TIME, 2, 0, 0, idle_frames);
    if (!viewer->idle) {
        spriteDestroy(idle_frames[0]); spriteDestroy(idle_frames[1]);
        free(idle_frames); free(viewer); return NULL;
    }

    Sprite **atk_frames = malloc(2 * sizeof(Sprite *));
    if (!atk_frames) { 
        destroyAnimSprite(viewer->idle);
        free(viewer);
        return NULL;
    }

    atk_frames[0] = spriteCreate(stop_xpm);
    atk_frames[1] = spriteCreate(atk_xpm);

    if (!atk_frames[0] || !atk_frames[1]) {
        if (atk_frames[0]) spriteDestroy(atk_frames[0]);
        if (atk_frames[1]) spriteDestroy(atk_frames[1]);
        free(atk_frames);
        destroyAnimSprite(viewer->idle);
        free(viewer);
        return NULL;
    }

    viewer->atk = createAnimSprite(MINION_ANIM_TIME, 2, 0, 0, atk_frames);
    if (!viewer->atk) {
        spriteDestroy(atk_frames[0]); spriteDestroy(atk_frames[1]);
        free(atk_frames);
        destroyAnimSprite(viewer->idle);
        free(viewer);
        return NULL;
    }

    viewer->hpBar = createHpBarViewer();
    if (!viewer->hpBar) {
        destroyAnimSprite(viewer->atk);
        destroyAnimSprite(viewer->idle);
        free(viewer);
        return NULL;
    }

    return viewer;
}

void destroyMinionViewer(MinionViewer *viewer) {
    if (!viewer) return;
    if (viewer->idle) destroyAnimSprite(viewer->idle);
    if (viewer->atk) destroyAnimSprite(viewer->atk);
    destroyHpBarViewer(viewer->hpBar);
    free(viewer);
}

void minionViewerUpdate(MinionViewer *viewer) {
    if (!viewer) return;
    updateAnimSprite(viewer->idle);
    updateAnimSprite(viewer->atk);
}

/**
 * @brief Draws a single minion with its HP bar at its current world position.
 * Selects the attack or idle animation clip based on the minion's state.
 * @param viewer MinionViewer supplying the animation clips and HP bar.
 * @param m Minion to draw.
 * @return 0 on success, non-zero on error.
 */
static int drawMinion(MinionViewer *viewer, Minion *m) {
    AnimSprite *anim = (m->state == MINION_ATTACKING) ? viewer->atk : viewer->idle;
    int mx = position_px(m->pos);
    int my = position_py(m->pos);
    int sprite_w = getWidth(getCurrentSprite(anim));
    int draw_x = (m->team == TEAM_RED) ? mx - (sprite_w - MINION_W) : mx;
    setAnimSpritePosition(anim, draw_x, my);
    hpBarViewerDraw(viewer->hpBar, mx, my, MINION_W, m->hp, MINION_HP);
    return drawAnimSprite(anim);
}

int minionViewerDraw(MinionViewer *viewer, Minions *minions, Team team) {
    MinionWave *waves = (team == TEAM_BLUE) ? minions->blue_waves : minions->red_waves;
    for (int w = 0; w < MAX_WAVES; w++) {
        if (!waves[w].active) continue;
        for (int i = 0; i < MINION_COUNT; i++) {
            Minion *m = &waves[w].minions[i];
            if (m->state == MINION_DEAD) continue;
            if (drawMinion(viewer, m) != 0) return 1;
        }
    }
    return 0;
}
