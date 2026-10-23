#include "include/level_config.h"
#include "include/config.h"
#include "include/enemy.h"
#include <stddef.h>

#define ARRAY_LEN(a) ((int)(sizeof(a) / sizeof((a)[0])))

#define DEFAULT_STATS {LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT}
#define STATS(speed, health, direction, max_distance, fire_rate) \
    {speed, health, direction, LEVEL_USE_DEFAULT, max_distance, LEVEL_USE_DEFAULT, fire_rate}

#define AT_START       { SPAWN_AT_LEVEL_START, 0 }
#define AT_TIME(ticks) { SPAWN_AT_TIME, ticks }
#define AFTER_WAVE(w)  { SPAWN_AFTER_WAVE, w }

// =========================================================
// LEVEL 1 -> apenas ENEMY_WALKER (analog)
// 6 waves, 2 walkers por wave, padroes alternados
// 4 rails verticais
// Walker: 2 HP, speed=2
// =========================================================

#define WALKER_H 96
#define WALKER_ACROSS (SCREEN_H + 200)

static const EnemySpawnData level1_spawns[] = {

  // Wave 1 (esquerdos, ambos descem)
  {L1_RAIL_A_X, L1_RAIL_A_TOP - WALKER_H, ENEMY_WALKER, AT_START, 1, STATS(2, 2,  1, WALKER_ACROSS, LEVEL_USE_DEFAULT)},
  {L1_RAIL_B_X, L1_RAIL_B_TOP - WALKER_H, ENEMY_WALKER, AT_START, 1, STATS(2, 2, 1, WALKER_ACROSS, LEVEL_USE_DEFAULT)},

  // Wave 2 (direitos, ambos sobem)
  {L1_RAIL_C_X, L1_RAIL_C_BOT, ENEMY_WALKER, AFTER_WAVE(1), 2, STATS(2, 2, -1, WALKER_ACROSS, LEVEL_USE_DEFAULT)},
  {L1_RAIL_D_X, L1_RAIL_D_BOT, ENEMY_WALKER, AFTER_WAVE(1), 2, STATS(2, 2, -1, WALKER_ACROSS, LEVEL_USE_DEFAULT)},

  // Wave 3 (centrais, ambos descem)
  {L1_RAIL_B_X, L1_RAIL_B_TOP - WALKER_H, ENEMY_WALKER, AFTER_WAVE(2), 3, STATS(2, 2,  1, WALKER_ACROSS, LEVEL_USE_DEFAULT)},
  {L1_RAIL_C_X, L1_RAIL_C_TOP - WALKER_H, ENEMY_WALKER, AFTER_WAVE(2), 3, STATS(2, 2,  1, WALKER_ACROSS, LEVEL_USE_DEFAULT)},

  // Wave 4 (extremos, ambos sobem)
  {L1_RAIL_A_X, L1_RAIL_A_BOT, ENEMY_WALKER, AFTER_WAVE(3), 4, STATS(2, 2, -1, WALKER_ACROSS, LEVEL_USE_DEFAULT)},
  {L1_RAIL_D_X, L1_RAIL_D_BOT, ENEMY_WALKER, AFTER_WAVE(3), 4, STATS(2, 2, -1, WALKER_ACROSS, LEVEL_USE_DEFAULT)},

  // Wave 5 (cruzado: esq extremo desce, dir central sobe)
  {L1_RAIL_A_X, L1_RAIL_A_TOP - WALKER_H, ENEMY_WALKER, AFTER_WAVE(4), 5, STATS(2, 2,  1, WALKER_ACROSS, LEVEL_USE_DEFAULT)},
  {L1_RAIL_C_X, L1_RAIL_C_BOT, ENEMY_WALKER, AFTER_WAVE(4), 5, STATS(2, 2, -1, WALKER_ACROSS, LEVEL_USE_DEFAULT)},

  // Wave 6 (cruzado: centro esq sobe, dir extremo desce)
  {L1_RAIL_B_X, L1_RAIL_B_BOT, ENEMY_WALKER, AFTER_WAVE(5), 6, STATS(2, 2, -1, WALKER_ACROSS, LEVEL_USE_DEFAULT)},
  {L1_RAIL_D_X, L1_RAIL_D_TOP - WALKER_H, ENEMY_WALKER, AFTER_WAVE(5), 6, STATS(2, 2,  1, WALKER_ACROSS, LEVEL_USE_DEFAULT)},
};

// =========================================================
// LEVEL 2 -> ENEMY_WALKER (analog) + ENEMY_PARALYZER (alarm)
// 4 waves, padroes alternados
// Paralyzer: 3 HP, parado, paralisa o player ~3s (180 ticks)
// Walker: 2 HP, speed=2
// =========================================================

static const EnemySpawnData level2_spawns[] = {

    // Wave 1 -> 2 walkers + 2 paralyzers 
    {L2_RAIL_A_X, L2_RAIL_A_TOP - WALKER_H, ENEMY_WALKER, AT_START, 1, STATS(2, 2,  1, WALKER_ACROSS, LEVEL_USE_DEFAULT)},
    {L2_RAIL_C_X, L2_RAIL_C_TOP - WALKER_H, ENEMY_WALKER, AT_START, 1, STATS(2, 2,  1, WALKER_ACROSS, LEVEL_USE_DEFAULT)},
    {L2_PL_TL_X,  L2_PL_TL_Y, ENEMY_PARALYZER, AT_START, 1, STATS(LEVEL_USE_DEFAULT, 3, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 180)},
    {L2_PL_BR_X,  L2_PL_BR_Y, ENEMY_PARALYZER, AT_START, 1, STATS(LEVEL_USE_DEFAULT, 3, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 180)},

    // Wave 2 -> 3 walkers + 4 paralyzers 
    {L2_RAIL_A_X, L2_RAIL_A_TOP - WALKER_H, ENEMY_WALKER, AFTER_WAVE(1), 4, STATS(2, 2,  1, WALKER_ACROSS, LEVEL_USE_DEFAULT)},
    {L2_RAIL_B_X, L2_RAIL_C_BOT, ENEMY_WALKER, AFTER_WAVE(1), 4, STATS(2, 2, -1, WALKER_ACROSS, LEVEL_USE_DEFAULT)},
    {L2_PL_TL_X,  L2_PL_TL_Y,   ENEMY_PARALYZER, AFTER_WAVE(1), 2, STATS(LEVEL_USE_DEFAULT, 3, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 200)},
    {L2_PL_TR_X,  L2_PL_TR_Y,   ENEMY_PARALYZER, AFTER_WAVE(1), 2, STATS(LEVEL_USE_DEFAULT, 3, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 220)},
    {L2_PL_BL_X,  L2_PL_BL_Y,   ENEMY_PARALYZER, AFTER_WAVE(1), 2, STATS(LEVEL_USE_DEFAULT, 3, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 200)},
    {L2_PL_BR_X,  L2_PL_BR_Y,   ENEMY_PARALYZER, AFTER_WAVE(1), 2, STATS(LEVEL_USE_DEFAULT, 3, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 210)},

    // Wave 3 -> 2 walkers + 2 paralyzers
    {L2_RAIL_A_X, L2_RAIL_A_TOP - WALKER_H, ENEMY_WALKER, AFTER_WAVE(2), 3, STATS(2, 2,  1, WALKER_ACROSS, LEVEL_USE_DEFAULT)},
    {L2_RAIL_C_X, L2_RAIL_C_TOP - WALKER_H, ENEMY_WALKER, AFTER_WAVE(2), 3, STATS(2, 2,  1, WALKER_ACROSS, LEVEL_USE_DEFAULT)},
    {L2_PL_TR_X,  L2_PL_TR_Y, ENEMY_PARALYZER, AFTER_WAVE(2), 3, STATS(LEVEL_USE_DEFAULT, 3, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 160)},
    {L2_PL_BL_X,  L2_PL_BL_Y, ENEMY_PARALYZER, AFTER_WAVE(2), 3, STATS(LEVEL_USE_DEFAULT, 3, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 160)},

    // Wave 4 -> 3 walkers cruzados + 4 paralyzers 
    {L2_RAIL_A_X, L2_RAIL_A_TOP - WALKER_H, ENEMY_WALKER, AFTER_WAVE(3), 4, STATS(2, 2,  1, WALKER_ACROSS, LEVEL_USE_DEFAULT)},
    {L2_RAIL_C_X, L2_RAIL_C_BOT, ENEMY_WALKER, AFTER_WAVE(3), 4, STATS(2, 2, 1, WALKER_ACROSS, LEVEL_USE_DEFAULT)},
    {L2_RAIL_B_X, L2_RAIL_B_TOP - WALKER_H, ENEMY_WALKER, AFTER_WAVE(3), 4, STATS(2, 1,  -1, WALKER_ACROSS, LEVEL_USE_DEFAULT)},
    {L2_PL_TL_X,  L2_PL_TL_Y, ENEMY_PARALYZER, AFTER_WAVE(3), 4, STATS(LEVEL_USE_DEFAULT, 3, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 140)},
    {L2_PL_TR_X,  L2_PL_TR_Y, ENEMY_PARALYZER, AFTER_WAVE(3), 4, STATS(LEVEL_USE_DEFAULT, 3, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 150)},
    {L2_PL_BL_X,  L2_PL_BL_Y, ENEMY_PARALYZER, AFTER_WAVE(3), 4, STATS(LEVEL_USE_DEFAULT, 3, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 140)},
    {L2_PL_BR_X,  L2_PL_BR_Y, ENEMY_PARALYZER, AFTER_WAVE(3), 4, STATS(LEVEL_USE_DEFAULT, 3, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 145)},
};

// =========================================================
// LEVEL 3 -> apenas ENEMY_SHOTTER (digital)
// 4 waves, entre 4 e 8 shooters por wave
// Shotter: 4 HP, parado, dispara,
// 8 posicoes possiveis: S1..S8 (4 em cima, 4 em baixo)
// =========================================================

static const EnemySpawnData level3_spawns[] = {

    // Wave 1 -> 4 shooters no topo
    {L3_S1_X, L3_S1_Y, ENEMY_SHOTTER, AT_START, 1, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 280)},
    {L3_S2_X, L3_S2_Y, ENEMY_SHOTTER, AT_START, 1, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 300)},
    {L3_S3_X, L3_S3_Y, ENEMY_SHOTTER, AT_START, 1, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 260)},
    {L3_S4_X, L3_S4_Y, ENEMY_SHOTTER, AT_START, 1, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 290)},

    // Wave 2 -> 4 shooters na base
    {L3_S5_X, L3_S5_Y, ENEMY_SHOTTER, AFTER_WAVE(1), 2, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 250)},
    {L3_S6_X, L3_S6_Y, ENEMY_SHOTTER, AFTER_WAVE(1), 2, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 270)},
    {L3_S7_X, L3_S7_Y, ENEMY_SHOTTER, AFTER_WAVE(1), 2, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 260)},
    {L3_S8_X, L3_S8_Y, ENEMY_SHOTTER, AFTER_WAVE(1), 2, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 240)},

    // Wave 3 -> 6 shooters topo e base intercalados 
    {L3_S1_X, L3_S1_Y, ENEMY_SHOTTER, AFTER_WAVE(2), 3, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 220)},
    {L3_S3_X, L3_S3_Y, ENEMY_SHOTTER, AFTER_WAVE(2), 3, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 230)},
    {L3_S4_X, L3_S4_Y, ENEMY_SHOTTER, AFTER_WAVE(2), 3, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 215)},
    {L3_S5_X, L3_S5_Y, ENEMY_SHOTTER, AFTER_WAVE(2), 3, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 210)},
    {L3_S6_X, L3_S6_Y, ENEMY_SHOTTER, AFTER_WAVE(2), 3, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 220)},
    {L3_S7_X, L3_S7_Y, ENEMY_SHOTTER, AFTER_WAVE(2), 3, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 200)},

    // Wave 4 -> 8 shooters todos, mais rapidos (wave final)
    {L3_S1_X, L3_S1_Y, ENEMY_SHOTTER, AFTER_WAVE(3), 4, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 180)},
    {L3_S2_X, L3_S2_Y, ENEMY_SHOTTER, AFTER_WAVE(3), 4, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 190)},
    {L3_S3_X, L3_S3_Y, ENEMY_SHOTTER, AFTER_WAVE(3), 4, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 175)},
    {L3_S4_X, L3_S4_Y, ENEMY_SHOTTER, AFTER_WAVE(3), 4, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 185)},
    {L3_S5_X, L3_S5_Y, ENEMY_SHOTTER, AFTER_WAVE(3), 4, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 180)},
    {L3_S6_X, L3_S6_Y, ENEMY_SHOTTER, AFTER_WAVE(3), 4, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 170)},
    {L3_S7_X, L3_S7_Y, ENEMY_SHOTTER, AFTER_WAVE(3), 4, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 185)},
    {L3_S8_X, L3_S8_Y, ENEMY_SHOTTER, AFTER_WAVE(3), 4, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 175)},
};

// =========================================================
// LEVEL 4 -> os 3 tipos em conjunto
// 4 waves, sempre:
//   2 walkers nos rails (max 2 simultaneos), speed=3, 3 HP
//   2-4 paralyzers em cantos opostos
//   2-6 shooters em posicoes diferentes dos paralyzers
// =========================================================

static const EnemySpawnData level4_spawns[] = {

    // Wave 1 -> 2 walkers + 2 paralyzers + 3 shooters 
    {L4_RAIL_A_X, L4_RAIL_A_TOP - WALKER_H, ENEMY_WALKER, AT_START, 1, STATS(3, 1,  1, WALKER_ACROSS, LEVEL_USE_DEFAULT)},
    {L4_RAIL_B_X, L4_RAIL_B_TOP - WALKER_H, ENEMY_WALKER, AT_START, 1, STATS(3, 1,  1, WALKER_ACROSS, LEVEL_USE_DEFAULT)},
    {L4_PL_TL_X, L4_PL_TL_Y, ENEMY_PARALYZER, AT_START, 1, STATS(LEVEL_USE_DEFAULT, 3, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 160)},
    {L4_PL_BR_X, L4_PL_BR_Y, ENEMY_PARALYZER, AT_START, 1, STATS(LEVEL_USE_DEFAULT, 3, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 160)},
    {L4_S1_X, L4_S1_Y, ENEMY_SHOTTER, AT_START, 1, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 200)},
    {L4_S2_X, L4_S2_Y, ENEMY_SHOTTER, AT_START, 1, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 210)},
    {L4_S3_X, L4_S3_Y, ENEMY_SHOTTER, AT_START, 1, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 220)},

    // Wave 2 -> 4 paralyzers + 3 shooters
    {L4_RAIL_A_X, L4_RAIL_A_BOT,  ENEMY_WALKER, AFTER_WAVE(1), 3, STATS(2, 3, 1, WALKER_ACROSS, LEVEL_USE_DEFAULT)},
    {L4_PL_TL_X, L4_PL_TL_Y, ENEMY_PARALYZER, AFTER_WAVE(1), 2, STATS(LEVEL_USE_DEFAULT, 3, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 140)},
    {L4_PL_TR_X, L4_PL_TR_Y, ENEMY_PARALYZER, AFTER_WAVE(1), 2, STATS(LEVEL_USE_DEFAULT, 3, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 150)},
    {L4_PL_BL_X, L4_PL_BL_Y, ENEMY_PARALYZER, AFTER_WAVE(1), 2, STATS(LEVEL_USE_DEFAULT, 3, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 140)},
    {L4_PL_BR_X, L4_PL_BR_Y, ENEMY_PARALYZER, AFTER_WAVE(1), 2, STATS(LEVEL_USE_DEFAULT, 3, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 145)},
    {L4_S4_X, L4_S4_Y, ENEMY_SHOTTER, AFTER_WAVE(1), 2, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 190)},
    {L4_S5_X, L4_S5_Y, ENEMY_SHOTTER, AFTER_WAVE(1), 2, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 170)},
    {L4_S6_X, L4_S6_Y, ENEMY_SHOTTER, AFTER_WAVE(1), 2, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 175)},

    // Wave 3 -> 2 walkers + 2 paralyzers  + 6 shooters 
    {L4_RAIL_B_X, L4_RAIL_B_BOT, ENEMY_WALKER, AFTER_WAVE(2), 3, STATS(2, 3, -1, WALKER_ACROSS, LEVEL_USE_DEFAULT)},
    {L4_PL_TR_X,  L4_PL_TR_Y, ENEMY_PARALYZER, AFTER_WAVE(2), 3, STATS(LEVEL_USE_DEFAULT, 3, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 130)},
    {L4_PL_BL_X, L4_PL_BL_Y, ENEMY_PARALYZER, AFTER_WAVE(2), 3, STATS(LEVEL_USE_DEFAULT, 3, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 130)},
    {L4_S1_X,  L4_S1_Y,  ENEMY_SHOTTER, AFTER_WAVE(2), 3, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 160)},
    {L4_S2_X, L4_S2_Y, ENEMY_SHOTTER, AFTER_WAVE(2), 3, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 155)},
    {L4_S3_X, L4_S3_Y, ENEMY_SHOTTER, AFTER_WAVE(2), 3, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 165)},
    {L4_S4_X, L4_S4_Y, ENEMY_SHOTTER, AFTER_WAVE(2), 3, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 160)},
    {L4_S5_X, L4_S5_Y, ENEMY_SHOTTER, AFTER_WAVE(2), 3, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 150)},
    {L4_S6_X, L4_S6_Y, ENEMY_SHOTTER, AFTER_WAVE(2), 3, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 155)},

    // Wave 4 -> wave final: 2 walkers + 4 paralyzers + 6 shooters mais rapidos
    {L4_RAIL_A_X, L4_RAIL_A_TOP - WALKER_H, ENEMY_WALKER, AFTER_WAVE(3), 4, STATS(1, 3,  -1, WALKER_ACROSS, LEVEL_USE_DEFAULT)},
    {L4_RAIL_B_X, L4_RAIL_B_BOT, ENEMY_WALKER,    AFTER_WAVE(3), 4, STATS(1, 3, 1, WALKER_ACROSS, LEVEL_USE_DEFAULT)},
    {L4_PL_TL_X, L4_PL_TL_Y, ENEMY_PARALYZER, AFTER_WAVE(3), 4, STATS(LEVEL_USE_DEFAULT, 3, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 110)},
    {L4_PL_TR_X, L4_PL_TR_Y, ENEMY_PARALYZER, AFTER_WAVE(3), 4, STATS(LEVEL_USE_DEFAULT, 3, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 120)},
    {L4_PL_BL_X, L4_PL_BL_Y, ENEMY_PARALYZER, AFTER_WAVE(3), 4, STATS(LEVEL_USE_DEFAULT, 3, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 110)},
    {L4_PL_BR_X,  L4_PL_BR_Y, ENEMY_PARALYZER, AFTER_WAVE(3), 4, STATS(LEVEL_USE_DEFAULT, 3, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 115)},
    {L4_S1_X, L4_S1_Y, ENEMY_SHOTTER, AFTER_WAVE(3), 4, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 130)},
    {L4_S2_X, L4_S2_Y, ENEMY_SHOTTER, AFTER_WAVE(3), 4, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 125)},
    {L4_S3_X, L4_S3_Y, ENEMY_SHOTTER,  AFTER_WAVE(3), 4, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 135)},
    {L4_S4_X, L4_S4_Y, ENEMY_SHOTTER, AFTER_WAVE(3), 4, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 130)},
    {L4_S5_X, L4_S5_Y, ENEMY_SHOTTER, AFTER_WAVE(3), 4, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 120)},
    {L4_S6_X, L4_S6_Y, ENEMY_SHOTTER, AFTER_WAVE(3), 4, STATS(LEVEL_USE_DEFAULT, 4, LEVEL_USE_DEFAULT, LEVEL_USE_DEFAULT, 125)},
};

static const LevelData levels[] = {
    {NULL, 0},
    {level1_spawns, ARRAY_LEN(level1_spawns)},
    {level2_spawns, ARRAY_LEN(level2_spawns)},
    {level3_spawns, ARRAY_LEN(level3_spawns)},
    {level4_spawns, ARRAY_LEN(level4_spawns)},
};

void level_default_stats(EnemyType type, EnemyStats *out) {
    if (out == NULL) return;
    out->startY = 0;
    out->fire_cooldown = 0;

    switch (type) {
        case ENEMY_WALKER:
            out->speed = 2;
            out->health = 2;
            out->direction = 1;
            out->max_distance = 220;
            out->fire_rate = 0;
            break;
        case ENEMY_SHOTTER:
            out->speed = 0;
            out->health = 4;
            out->direction = 0;
            out->max_distance = 0;
            out->fire_rate = 280;
            break;
        case ENEMY_PARALYZER:
            out->speed = 0;
            out->health = 3;
            out->direction = 0;
            out->max_distance = 0;
            out->fire_rate = 480;
            break;
    }
}

static void level_apply_stat_overrides(EnemyStats *stats, const EnemyStats *overrides) {
    if (stats == NULL || overrides == NULL) return;
    if (overrides->speed != LEVEL_USE_DEFAULT) stats->speed = overrides->speed;
    if (overrides->health != LEVEL_USE_DEFAULT) stats->health = overrides->health;
    if (overrides->direction != LEVEL_USE_DEFAULT) stats->direction = overrides->direction;
    if (overrides->max_distance != LEVEL_USE_DEFAULT) stats->max_distance = overrides->max_distance;
    if (overrides->fire_rate != LEVEL_USE_DEFAULT) stats->fire_rate = overrides->fire_rate;
}

static bool level_wave_has_pending_spawns(const LevelRuntime *runtime, int wave_id) {
    if (runtime == NULL || runtime->data == NULL) return false;
    int count = runtime->data->count;
    if (count > LEVEL_MAX_SPAWNS) count = LEVEL_MAX_SPAWNS;
    for (int i = 0; i < count; i++) {
        if (runtime->data->spawns[i].wave_id == wave_id && !runtime->spawned[i])
            return true;
    }
    return false;
}

static bool level_wave_is_complete(const LevelRuntime *runtime, Enemy enemies[], int max, int wave_id) {
   return !level_wave_has_pending_spawns(runtime, wave_id) &&
           !enemy_wave_has_active(enemies, max, wave_id);
}

static bool level_spawn_is_ready(const LevelRuntime *runtime, Enemy enemies[], int max, const EnemySpawnData *spawn) {
    if (runtime == NULL || spawn == NULL) return false;
    switch (spawn->trigger.type) {
        case SPAWN_AT_LEVEL_START: return true;
        case SPAWN_AT_TIME: return runtime->elapsed_ticks >= spawn->trigger.value;
        case SPAWN_AFTER_WAVE: return level_wave_is_complete(runtime, enemies, max, spawn->trigger.value);
    }
    return false;
}

const LevelData *level_get_data(int level) {
    if (level < 1 || level >= (int)ARRAY_LEN(levels)) return NULL;
    return &levels[level];
}

void level_start(LevelRuntime *runtime, Enemy enemies[], int max, int level) {
    if (runtime == NULL) return;
    runtime->data = level_get_data(level);
    runtime->elapsed_ticks = 0;
    for (int i = 0; i < LEVEL_MAX_SPAWNS; i++)
        runtime->spawned[i] = false;
    init_enemies(enemies, max);
}

void level_update(LevelRuntime *runtime, Enemy enemies[], int max) {
    if (runtime == NULL || runtime->data == NULL) return;
    int count = runtime->data->count;
    if (count > LEVEL_MAX_SPAWNS) count = LEVEL_MAX_SPAWNS;
    for (int i = 0; i < count; i++) {
        const EnemySpawnData *spawn = &runtime->data->spawns[i];
        if (runtime->spawned[i]) continue;
        if (!level_spawn_is_ready(runtime, enemies, max, spawn)) continue;
        int slot = enemy_find_free_slot(enemies, max);
        if (slot < 0) break;
        EnemyStats stats;
        level_default_stats(spawn->type, &stats);
        level_apply_stat_overrides(&stats, &spawn->stats);
        enemy_spawn(&enemies[slot], spawn->x, spawn->y, spawn->type, spawn->wave_id, &stats);
        runtime->spawned[i] = true;
    }
    runtime->elapsed_ticks++;
}

bool level_has_pending_spawns(const LevelRuntime *runtime) {
    if (runtime == NULL || runtime->data == NULL) return false;
    int count = runtime->data->count;
    if (count > LEVEL_MAX_SPAWNS) count = LEVEL_MAX_SPAWNS;
    for (int i = 0; i < count; i++)
        if (!runtime->spawned[i]) return true;
    return false;
}

bool level_is_complete(const LevelRuntime *runtime, Enemy enemies[], int max) {
    if (level_has_pending_spawns(runtime)) return false;
    for (int i = 0; i < max; i++)
        if (enemies[i].is_active) return false;
    return true;
}
