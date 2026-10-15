#include "BuildSprites.h"

static xpm_row_t *p1_idle_xpms[4] = {
    p1_north_stop_xpm,
    p1_south_stop_xpm,
    p1_east_stop_xpm,
    p1_west_stop_xpm,
};

static xpm_row_t *p1_move_xpms[4][MOVE_FRAMES] = {
    {p1_north_move1_xpm, p1_north_move2_xpm},
    {p1_south_move1_xpm, p1_south_move2_xpm},
    {p1_east_move1_xpm, p1_east_move2_xpm},
    {p1_west_move1_xpm, p1_west_move2_xpm},
};

static xpm_row_t *p1_attack_xpms[4][ATTACK_FRAMES] = {
    {p1_north_attack_up_xpm, p1_north_attack_mid_xpm},
    {p1_south_attack_up_xpm, p1_south_attack_mid_xpm},
    {p1_east_attack_up_xpm, p1_east_attack_mid_xpm},
    {p1_west_attack_up_xpm, p1_west_attack_mid_xpm},
};

static xpm_row_t *p2_idle_xpms[4] = {
    p2_north_stop_xpm,
    p2_south_stop_xpm,
    p2_east_stop_xpm,
    p2_west_stop_xpm,
};

static xpm_row_t *p2_move_xpms[4][MOVE_FRAMES] = {
    {p2_north_move1_xpm, p2_north_move2_xpm},
    {p2_south_move1_xpm, p2_south_move2_xpm},
    {p2_east_move1_xpm, p2_east_move2_xpm},
    {p2_west_move1_xpm, p2_west_move2_xpm},
};

static xpm_row_t *p2_attack_xpms[4][ATTACK_FRAMES] = {
    {p2_north_attack_up_xpm, p2_north_attack_mid_xpm},
    {p2_south_attack_up_xpm, p2_south_attack_mid_xpm},
    {p2_east_attack_up_xpm, p2_east_attack_mid_xpm},
    {p2_west_attack_up_xpm, p2_west_attack_mid_xpm},
};

/**
 * @brief Destroys the first count elements of an AnimSprite array and frees the array itself.
 * Used for cleanup when buildAnimations() fails partway through construction.
 * @param animations Array of AnimSprite pointers to destroy.
 * @param count Number of elements to destroy.
 */
static void destroyAnimations(AnimSprite **animations, int count) {
    for (int i = 0; i < count; i++)
        destroyAnimSprite(animations[i]);
    free(animations);
}

AnimSprite **buildAnimations(int player) {
    AnimSprite **animations = malloc(NUM_ANIMS * sizeof(AnimSprite *));
    if (animations == NULL)
        return NULL;

    int anims_created = 0;

    for (int i = 0; i < 4; i++) {
        Sprite *s = NULL;
        switch (player) {
            case 1:
                s = spriteCreate(p1_idle_xpms[i]);
                break;
            case 2:
                s = spriteCreate(p2_idle_xpms[i]);
                break;
            default:
                break;
        }

        if (s == NULL) {
            destroyAnimations(animations, anims_created);
            return NULL;
        }

        Sprite **frames = malloc(sizeof(Sprite *));
        if (frames == NULL) {
            spriteDestroy(s);
            destroyAnimations(animations, anims_created);
            return NULL;
        }

        frames[0] = s;
        animations[i] = createAnimSprite(1, 1, 350, 220, frames);

        if (animations[i] == NULL) {
            free(frames);
            spriteDestroy(s);
            destroyAnimations(animations, anims_created);
            return NULL;
        }
        anims_created++;
    }

    for (int i = 0; i < 4; i++) {
        Sprite **frames = malloc(ATTACK_FRAMES * sizeof(Sprite *));
        if (frames == NULL) {
            destroyAnimations(animations, anims_created);
            return NULL;
        }

        int frames_ok = 0;
        for (int f = 0; f < ATTACK_FRAMES; f++) {
            switch (player) {
                case 1:
                    frames[f] = spriteCreate(p1_attack_xpms[i][f]);
                    break;
                case 2:
                    frames[f] = spriteCreate(p2_attack_xpms[i][f]);
                    break;
                default:
                    break;
            }
            
            if (frames[f] == NULL) {
                for (int k = 0; k < frames_ok; k++)
                    spriteDestroy(frames[k]);
                free(frames);
                destroyAnimations(animations, anims_created);
                return NULL;
            }
            frames_ok++;
        }
        
        animations[4 + i] = createAnimSprite(ATTACK_TIME, ATTACK_FRAMES, 350, 220, frames);
        if (animations[4 + i] == NULL) {
            for (int f = 0; f < ATTACK_FRAMES; f++)
                spriteDestroy(frames[f]);
            free(frames);
            destroyAnimations(animations, anims_created);
            return NULL;
        }
        anims_created++;
    }

    for (int i = 0; i < 4; i++) {
        Sprite **frames = malloc(MOVE_FRAMES * sizeof(Sprite *));
        if (frames == NULL) {
            destroyAnimations(animations, anims_created);
            return NULL;
        }

        int frames_ok = 0;
        for (int f = 0; f < MOVE_FRAMES; f++) {
            frames[f] = spriteCreate(player == 1 ? p1_move_xpms[i][f] : p2_move_xpms[i][f]);
            if (frames[f] == NULL) {
                for (int k = 0; k < frames_ok; k++) spriteDestroy(frames[k]);
                free(frames);
                destroyAnimations(animations, anims_created);
                return NULL;
            }
            frames_ok++;
        }

        animations[8 + i] = createAnimSprite(MOVE_TIME, MOVE_FRAMES, 350, 220, frames);
        if (animations[8 + i] == NULL) {
            for (int f = 0; f < MOVE_FRAMES; f++) spriteDestroy(frames[f]);
            free(frames);
            destroyAnimations(animations, anims_created);
            return NULL;
        }
        anims_created++;
    }

    return animations;
}
