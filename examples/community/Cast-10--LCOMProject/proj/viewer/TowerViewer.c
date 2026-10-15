#include "TowerViewer.h"


#define PROJECTILE_ANIM_TIME 8

/**
 * @brief Creates a two-frame AnimSprite for a tower projectile from two XPM rows.
 * @param frame1 XPM row data for the first animation frame.
 * @param frame2 XPM row data for the second animation frame.
 * @return Pointer to the new AnimSprite, or NULL on failure.
 */
static AnimSprite *createProjectileAnim(xpm_row_t *frame1, xpm_row_t *frame2) {
    Sprite **frames = malloc(2 * sizeof(Sprite *));
    if (!frames) return NULL;

    frames[0] = spriteCreate((xpm_map_t) frame1);
    frames[1] = spriteCreate((xpm_map_t) frame2);

    if (!frames[0] || !frames[1]) {
        if (frames[0]) spriteDestroy(frames[0]);
        if (frames[1]) spriteDestroy(frames[1]);
        free(frames);
        return NULL;
    }

    AnimSprite *anim = createAnimSprite(PROJECTILE_ANIM_TIME, 2, 0, 0, frames);
    if (!anim) {
        spriteDestroy(frames[0]);
        spriteDestroy(frames[1]);
        free(frames);
    }
    return anim;
}

TowerViewer *createTowerViewer(xpm_map_t xpm_blue, xpm_map_t xpm_red) {
    TowerViewer *viewer = malloc(sizeof(TowerViewer));
    if (!viewer) return NULL;

    viewer->hpBar           = NULL;
    viewer->blue            = NULL;
    viewer->red             = NULL;
    viewer->blueProjectile  = NULL;
    viewer->redProjectile   = NULL;

    viewer->hpBar = createHpBarViewer();
    viewer->blue  = spriteCreate(xpm_blue);
    viewer->red   = spriteCreate(xpm_red);
    viewer->blueProjectile = createProjectileAnim(p1_missil1_xpm, p1_missil2_xpm);
    viewer->redProjectile  = createProjectileAnim(p2_missil1_xpm, p2_missil2_xpm);

    if (!viewer->hpBar || !viewer->blue || !viewer->red ||
        !viewer->blueProjectile || !viewer->redProjectile) {
        destroyTowerViewer(viewer);
        return NULL;
    }

    return viewer;
}

void destroyTowerViewer(TowerViewer *viewer) {
    if (!viewer) return;

    if (viewer->hpBar)          destroyHpBarViewer(viewer->hpBar);
    if (viewer->blue)           spriteDestroy(viewer->blue);
    if (viewer->red)            spriteDestroy(viewer->red);
    if (viewer->blueProjectile) destroyAnimSprite(viewer->blueProjectile);
    if (viewer->redProjectile)  destroyAnimSprite(viewer->redProjectile);
    free(viewer);
}

void towerViewerDraw(TowerViewer *viewer, Towers *towers) {
    if (!viewer || !towers) return;

    Tower *t = &towers->blue;
    if (t->hp > 0) {
        spriteDraw(viewer->blue, position_px(t->pos), position_py(t->pos));
        hpBarViewerDraw(viewer->hpBar,
                        position_px(t->pos), position_py(t->pos),
                        TOWER_W, t->hp, TOWER_HP);
    }

    t = &towers->red;
    if (t->hp > 0) {
        spriteDraw(viewer->red, position_px(t->pos), position_py(t->pos));
        hpBarViewerDraw(viewer->hpBar,
                        position_px(t->pos), position_py(t->pos),
                        TOWER_W, t->hp, TOWER_HP);
    }

    if (towers->blue.projectile.active) {
        int px = position_px(towers->blue.projectile.pos);
        int py = position_py(towers->blue.projectile.pos);
        updateAnimSprite(viewer->blueProjectile);
        setAnimSpritePosition(viewer->blueProjectile, px, py);
        drawAnimSprite(viewer->blueProjectile);
    }

    if (towers->red.projectile.active) {
        int px = position_px(towers->red.projectile.pos);
        int py = position_py(towers->red.projectile.pos);
        updateAnimSprite(viewer->redProjectile);
        setAnimSpritePosition(viewer->redProjectile, px, py);
        drawAnimSprite(viewer->redProjectile);
    }
}
