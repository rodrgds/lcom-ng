#ifndef LEVEL_CONFIG_H
#define LEVEL_CONFIG_H

#include "enemy.h"
#include "config.h"
#include <stdbool.h>

#define LEVEL_MAX_SPAWNS 64
// menor inteiro possivel para não entrar em conflito com nenhum valor de derição
#define LEVEL_USE_DEFAULT INT32_MIN

// Rails do level 1 - normalized positions
#define L1_RAIL_A_X 55
#define L1_RAIL_A_TOP 50
#define L1_RAIL_A_BOT 550
#define L1_RAIL_B_X  220
#define L1_RAIL_B_TOP 50
#define L1_RAIL_B_BOT 550
#define L1_RAIL_C_X  440
#define L1_RAIL_C_TOP 50
#define L1_RAIL_C_BOT 550
#define L1_RAIL_D_X  605
#define L1_RAIL_D_TOP 50
#define L1_RAIL_D_BOT 550

// Rails do level 2 - normalized positions
#define L2_RAIL_A_X 255
#define L2_RAIL_A_TOP 50
#define L2_RAIL_A_BOT 550
#define L2_RAIL_B_X 340
#define L2_RAIL_B_TOP 50
#define L2_RAIL_B_BOT 550
#define L2_RAIL_C_X 425
#define L2_RAIL_C_TOP 50
#define L2_RAIL_C_BOT 550

// Cantos dos paralyzers level 2 - normalized positions
#define L2_PL_TL_X 80
#define L2_PL_TL_Y 140
#define L2_PL_TR_X 590
#define L2_PL_TR_Y 140
#define L2_PL_BL_X 80
#define L2_PL_BL_Y 340
#define L2_PL_BR_X 590
#define L2_PL_BR_Y 340

// Posicoes dos shooters level 3 - PLACEHOLDER
#define L3_S1_X 130
#define L3_S1_Y 100
#define L3_S2_X 270
#define L3_S2_Y 100
#define L3_S3_X 430
#define L3_S3_Y 100
#define L3_S4_X 580
#define L3_S4_Y 100
#define L3_S5_X 130
#define L3_S5_Y 400
#define L3_S6_X 270
#define L3_S6_Y 400
#define L3_S7_X 430
#define L3_S7_Y 400
#define L3_S8_X 580
#define L3_S8_Y 400



// Rails do level 4 - normalized positions
#define L4_RAIL_A_X 275
#define L4_RAIL_A_TOP 50
#define L4_RAIL_A_BOT 550
#define L4_RAIL_B_X 415
#define L4_RAIL_B_TOP 50
#define L4_RAIL_B_BOT 550


// Cantos dos paralyzers level 4 - normalized positions
#define L4_PL_TL_X 105 
#define L4_PL_TL_Y 100 
#define L4_PL_TR_X 570
#define L4_PL_TR_Y 100
#define L4_PL_BL_X 105
#define L4_PL_BL_Y 380
#define L4_PL_BR_X 570
#define L4_PL_BR_Y 380

// Posicoes dos shooters level 4 - PLACEHOLDER
#define L4_S1_X 200
#define L4_S1_Y 140
#define L4_S2_X 350
#define L4_S2_Y 80
#define L4_S3_X 500
#define L4_S3_Y 140
#define L4_S4_X 200
#define L4_S4_Y 340
#define L4_S5_X 350
#define L4_S5_Y 400
#define L4_S6_X 500
#define L4_S6_Y 340


typedef enum {
    SPAWN_AT_LEVEL_START,
    SPAWN_AT_TIME, //  ticks após o início do nível
    SPAWN_AFTER_WAVE // wave_id que tem de estar completa
} SpawnTriggerType;

typedef struct {
    SpawnTriggerType type;
    int value; // ticks for time triggers, wave id for wave triggers
} SpawnTrigger;

// os campos de stats com LEVEL_USE_DEFAULT herdam os defaults do tipo.
typedef struct {
    int x, y;
    EnemyType type;
    SpawnTrigger trigger;
    int wave_id;
    EnemyStats stats; // fields with LEVEL_USE_DEFAULT mantem os defaults de EnemyType
} EnemySpawnData;

typedef struct {
    const EnemySpawnData *spawns;
    int count;
} LevelData;

typedef struct {
    const LevelData *data;
    int elapsed_ticks;
    bool spawned[LEVEL_MAX_SPAWNS];
} LevelRuntime;

// Chamada pelo level_update antes de aplicar overrides por spawn
void level_default_stats(EnemyType type, EnemyStats *out);

const LevelData *level_get_data(int level);

// Inicia o nível, preparando os spawns e resetando o timer interno do runtime
void level_start(LevelRuntime *runtime, Enemy enemies[], int max, int level);

// Atualiza o estado do nível, verifica se algum spawn deve ser ativado e atualizando o timer interno do runtime
void level_update(LevelRuntime *runtime, Enemy enemies[], int max);

// true se ainda houver spawns pendentes (não ativados) no nível
bool level_has_pending_spawns(const LevelRuntime *runtime);

// true se o nível foi completado quando nao ha spawns pendentes e nao ha inimigos ativos
bool level_is_complete(const LevelRuntime *runtime, Enemy enemies[], int max);

#endif // LEVEL_CONFIG_H
