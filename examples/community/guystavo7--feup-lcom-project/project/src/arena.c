#include "../include/arena.h"
/** @file arena.c
 *  @brief Implementacao da arena: mapa, ondas, frutos, torres, menus de upgrade/construcao e colisoes.
 */
#include "../include/buildings/basicLongTower.h"
#include "../include/buildings/basicTower.h"
#include "../include/buildings/eletricTower.h"
#include "../include/buildings/fastTower.h"
#include "../include/buildings/genericTower.h"
#include "../include/buildings/mortarTower.h"
#include "../include/buildings/piercingTower.h"

#include "../include/button.h"
#include "../include/components/boxCollider.h"
#include "../include/data.h"
#include "../include/fruit.h"
#include "../include/player/player.h"
#include "../include/tiles/collectable.h"
#include "../include/tiles/tile.h"
#include "../include/utils/cursorHandler.h"

#include "../include/ui/buildMenu.h"
#include "../include/ui/healthBar.h"
#include "../include/ui/statsBar.h"

#include "../resources/blue/tower/blueTower.h"
#include "../resources/fruits/smallFruit.h"
#include "../resources/icons/icons.h"
#include "../resources/tiles/tiles.h"
#include "../resources/upgradeMenu/upgradeMenu.h"

#include "../../lab5/spriteXMP.h"
#include "../../lab5/terminal.h"

#include "../include/monster/monsterGen.h"
#include "../include/monster/waves.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum {
    ARENA_CONSTRUCT_NORMAL = 0,
    ARENA_CONSTRUCT_PICK_TOWER,
    ARENA_CONSTRUCT_PLACE_TOWER,
} ArenaConstructPhase;

typedef enum {
    ARENA_UPGRADE_IDLE = 0,
    ARENA_UPGRADE_MENU_OPEN,
} ArenaUpgradePhase;

typedef struct {
    Collectable* collectable;
    FruitKind kind;
    float timeToLive;
} ArenaFruit;

#define ARENA_WAVE_MAX_SPAWNS 64

typedef enum {
    ARENA_WAVE_KIND_PAWN = 0,
    ARENA_WAVE_KIND_ALIEN = 1,
    ARENA_WAVE_KIND_SPEAR = 2,
} ArenaWaveKind;

struct Arena {
    int* collidersN;
    BoxCollider*** colliders;

    int tilesN;
    Tile** tiles;

    int fruitsN;
    ArenaFruit* fruits;

    int towersN;
    GenericTower** towers;

    int width;
    int height;
    int tileSize;
 
    Vector* pathList;
    Vector* monsterGenList; 

    Player* player;
    Cursor* cursor;

    int HeroHP;

    HealthBar* healthBar;
    StatsBar* statsBar;
    BuildMenu* basicTowerBuildMenu;
    BuildMenu* longTowerBuildMenu;

    ArenaConstructPhase constructPhase;
    ArenaUpgradePhase upgradePhase;

    int upgradeTargetIndex;

    SpriteXPM* upgradeScrollLeftBasic;
    SpriteXPM* upgradeScrollRightBasic;
    BoxCollider* upgradeScrollLeftBasicHitbox;
    BoxCollider* upgradeScrollRightBasicHitbox;
    Button* upgradeHammerLeftBasic;
    Button* upgradeHammerRightBasic;

    SpriteXPM* upgradeScrollLeftLong;
    SpriteXPM* upgradeScrollRightLong;
    Button* upgradeHammerLeftLong;
    Button* upgradeHammerRightLong;

    TowerKind towerType;

    float carrotSpawnCooldown;

    int level;
    float match_time_sec;
    int prepared_wave_index;
    unsigned wave_token_total;
    unsigned wave_token_cursor;
    int wave_tokens[ARENA_WAVE_MAX_SPAWNS];
    float wave_spawn_time_abs[ARENA_WAVE_MAX_SPAWNS];
    unsigned wave_gen_count;
    bool boss_spawned;
    MonsterGen* boss_gen;

    ArenaOutcome outcome;
    bool ignore_boss_win;
};

/** @brief Selecciona o array XPM pequeno consoante o tipo de fruto.
 *  @param[in] kind Tipo de fruto.
 *  @return Ponteiro para dados XPM (static do recurso).
 */
static char** arena_fruit_xpm_for_kind(FruitKind kind) {
    switch (kind) {
        case FRUIT_BANANA:
            return smallBanana;
        case FRUIT_ABACATE:
            return smallAbacate;
        case FRUIT_CEREJA:
            return smallCereja;
        case FRUIT_CENOURA:
            return smallCenoura;
        default:
            return smallBanana;
    }
}

/** @brief Actualiza o array de colliders COLLECTABLE_TAG com base nos frutos activos.
 *  @param[in,out] arena Arena.
 */
static void arena_fruits_sync_colliders(Arena* arena) {
    int n = arena->fruitsN;
    arena->collidersN[COLLECTABLE_TAG] = n;
    if (n == 0) {
        free(arena->colliders[COLLECTABLE_TAG]);
        arena->colliders[COLLECTABLE_TAG] = NULL;
        return;
    }
    BoxCollider** next = realloc(arena->colliders[COLLECTABLE_TAG], sizeof(BoxCollider*) * (size_t)n);
    if (next == NULL) return;
    arena->colliders[COLLECTABLE_TAG] = next;
    for (int i = 0; i < n; i++) {
        next[i] = collectableGetHitBox(arena->fruits[i].collectable);
    }
}

/** @brief Adiciona um fruto colecionavel com TTL e sincroniza colliders.
 *  @param[in,out] arena Arena.
 *  @param[in] position Posicao de spawn.
 *  @param[in] kind Variante de fruto.
 *  @param[in] ttl_sec Segundos ate expirar.
 */
static void arena_fruit_push(Arena* arena, Vec2d position, FruitKind kind, float ttl_sec) {
    int idx = arena->fruitsN;
    arena->fruitsN++;
    ArenaFruit* grown = realloc(arena->fruits, sizeof(ArenaFruit) * (size_t)arena->fruitsN);
    if (grown == NULL) {
        arena->fruitsN--;
        return;
    }
    arena->fruits = grown;
    Collectable* c = collectableNew(position, EMPTY_VEC2D, arena_fruit_xpm_for_kind(kind));
    arena->fruits[idx].collectable = c;
    arena->fruits[idx].kind = kind;
    arena->fruits[idx].timeToLive = ttl_sec;
    arena_fruits_sync_colliders(arena);
}

/** @brief Sorteia tipo de fruto largado por monstro segundo tabela de probabilidades.
 *  @return FruitKind sorteado.
 */
static FruitKind arena_roll_monster_drop_fruit_kind(void) {
    int r = rand() % 100;
    if (r < FRUIT_MONSTER_ROLL_BANANA_BELOW) return FRUIT_BANANA;
    if (r < FRUIT_MONSTER_ROLL_ABACATE_BELOW) return FRUIT_ABACATE;
    return FRUIT_CEREJA;
}

void arenaDeathFruit(Arena* arena, Vec2d position) {
    if (arena == NULL) return;
    if ((rand() % 100) >= FRUIT_MONSTER_DROP_PERCENT) return;
    arena_fruit_push(arena, position, arena_roll_monster_drop_fruit_kind(), FRUIT_TIME_ON_SCREEN_SEC);
}

/** @brief Verifica se uma caixa de teste pode receber cenoura (sem path, lago ou torre).
 *  @param[in] arena Arena.
 *  @param[in] probe Caixa de teste.
 *  @return true se posicao valida.
 */
static bool arena_carrot_probe_valid(Arena* arena, BoxCollider* probe) {
    BoxCollider* hits[PLACEMENT_QUERY_MAX];
    if (arenaDetectCollisions(arena, probe, PATH_TAG, hits) > 0) return false;
    if (arenaDetectCollisions(arena, probe, LAKE_TAG, hits) > 0) return false;
    if (arena->collidersN[TOWER_TAG] > 0 && arena->colliders[TOWER_TAG] != NULL) {
        if (arenaDetectCollisions(arena, probe, TOWER_TAG, hits) > 0) return false;
    }
    return true;
}

/** @brief Tenta colocar uma cenoura numa posicao aleatoria valida.
 *  @param[in,out] arena Arena.
 */
static void arena_carrot_spawn_try(Arena* arena) {
    int spanW = MAP_TILES_X * TILE_SIZE - (int)FRUIT_SIZE.x;
    int spanH = MAP_TILES_Y * TILE_SIZE - (int)FRUIT_SIZE.y;
    if (spanW <= 0 || spanH <= 0) return;
    for (int k = 0; k < FRUIT_CARROT_SAMPLE_TRIES; k++) {
        Vec2d pos = {(float)(rand() % spanW), (float)(rand() % spanH)};
        BoxCollider* probe = boxColliderNew(pos, EMPTY_VEC2D, FRUIT_SIZE, 0);
        bool ok = arena_carrot_probe_valid(arena, probe);
        boxColliderFree(probe);
        if (ok) {
            arena_fruit_push(arena, pos, FRUIT_CENOURA, FRUIT_TIME_ON_SCREEN_SEC);
            return;
        }
    }
}

/** @brief Decrementa cooldown global e dispara tentativa de spawn de cenoura.
 *  @param[in,out] arena Arena.
 *  @param[in] deltaTime Delta em segundos.
 */
static void arena_carrot_spawn_tick(Arena* arena, float deltaTime) {
    if (arena == NULL) return;
    arena->carrotSpawnCooldown -= deltaTime;
    if (arena->carrotSpawnCooldown > 0.0f) return;
    arena->carrotSpawnCooldown = FRUIT_CARROT_SPAWN_INTERVAL_SEC;
    arena_carrot_spawn_try(arena);
}

/** @brief Remove fruto por indice e compacta o array.
 *  @param[in,out] arena Arena.
 *  @param[in] index Indice valido em fruits.
 */
static void arena_fruit_remove_at(Arena* arena, int index) {
    if (index < 0 || index >= arena->fruitsN) return;
    collectableFree(arena->fruits[index].collectable);
    int last = arena->fruitsN - 1;
    if (index != last) arena->fruits[index] = arena->fruits[last];
    arena->fruitsN--;
    if (arena->fruitsN == 0) {
        free(arena->fruits);
        arena->fruits = NULL;
    } else {
        ArenaFruit* shrunk = realloc(arena->fruits, sizeof(ArenaFruit) * (size_t)arena->fruitsN);
        if (shrunk != NULL) arena->fruits = shrunk;
    }
    arena_fruits_sync_colliders(arena);
}

/** @brief Decrementa TTL dos frutos e remove os expirados ou inactivos.
 *  @param[in,out] arena Arena.
 *  @param[in] deltaTime Delta em segundos.
 */
static void arena_fruits_tick_time_to_live(Arena* arena, float deltaTime) {
    for (int i = 0; i < arena->fruitsN;) {
        Collectable* c = arena->fruits[i].collectable;
        if (!collectableIsActive(c)) {
            arena_fruit_remove_at(arena, i);
            continue;
        }
        arena->fruits[i].timeToLive -= deltaTime;
        if (arena->fruits[i].timeToLive <= 0.0f) {
            arena_fruit_remove_at(arena, i);
            continue;
        }
        i++;
    }
}

/** @brief Credita recurso na stats bar quando o jogador apanha um fruto.
 *  @param[in,out] sb Barra de estatisticas.
 *  @param[in] kind Tipo de fruto apanhado.
 */
static void arena_pickup_add_fruit_to_stats_bar(StatsBar* sb, FruitKind kind) {
    switch (kind) {
        case FRUIT_BANANA:
            statsBarAddBanana(sb, 1);
            break;
        case FRUIT_ABACATE:
            statsBarAddAvocado(sb, 1);
            break;
        case FRUIT_CEREJA:
            statsBarAddCherry(sb, 1);
            break;
        case FRUIT_CENOURA:
            statsBarAddCarrot(sb, 1);
            break;
        default:
            break;
    }
}

/** @brief Contexto global temporario para callbacks de botoes de upgrade (padrao legado). */
static Arena* s_arena_upgrade_button_ctx;

/** @brief Fecha UI de upgrade e repoe botoes.
 *  @param[in,out] arena Arena.
 */
static void arenaCloseUpgradeMenu(Arena* arena) {
    arena->upgradePhase = ARENA_UPGRADE_IDLE;
    arena->upgradeTargetIndex = -1;
    buttonReset(arena->upgradeHammerLeftBasic);
    buttonReset(arena->upgradeHammerRightBasic);
    buttonReset(arena->upgradeHammerLeftLong);
    buttonReset(arena->upgradeHammerRightLong);
}

/** @brief Verifica custo e debita recursos da stats bar se possivel.
 *  @param[in,out] arena Arena.
 *  @param[in] newKind Tipo de torre a comprar.
 *  @return true se a compra foi debitada com sucesso.
 */
static bool buyTower(Arena* arena, TowerKind newKind) {
    bool canBuy = true;
    const int* cost = TOWER_COST[newKind];
    if (statsBarGetBanana(arena->statsBar) < cost[0] || statsBarGetAvocado(arena->statsBar) < cost[1] || statsBarGetCherry(arena->statsBar) < cost[2] || statsBarGetCarrot(arena->statsBar) < cost[3]) canBuy = false;

    if (canBuy) {
        statsBarAddBanana(arena->statsBar, -cost[0]);
        statsBarAddAvocado(arena->statsBar, -cost[1]);
        statsBarAddCherry(arena->statsBar, -cost[2]);
        statsBarAddCarrot(arena->statsBar, -cost[3]);
    }
    return canBuy;
}

/** @brief Substitui a torre seleccionada por uma nova variante apos compra valida.
 *  @param[in,out] arena Arena.
 *  @param[in] newKind Novo TowerKind.
 */
static void arenaReplaceTargetTower(Arena* arena, TowerKind newKind) {
    int i = arena->upgradeTargetIndex;
    if (i < 0 || i >= arena->towersN || !buyTower(arena, newKind)) return;
    GenericTower* old = arena->towers[i];
    if (!towerIsBasic(old) && !towerIsLong(old)) return;

    Vec2d pos = towerGetPosition(old);
    BoxCollider* oldHit = towerGetHitBox(old);
    int wallN = arena->collidersN[TOWER_TAG];
    int j;
    for (j = 0; j < wallN; j++) {
        if (arena->colliders[TOWER_TAG][j] == oldHit) break;
    }
    if (j >= wallN) return;

    GenericTower* neu = NULL;
    switch (newKind) {
        case TOWER_KIND_FAST:
            neu = fastTowerNew(pos);
            break;
        case TOWER_KIND_PIERCING:
            neu = piercingTowerNew(pos);
            break;
        case TOWER_KIND_ELETRICT:
            neu = eletrictTowerNew(pos);
            break;
        case TOWER_KIND_MORTAR:
            neu = mortarTowerNew(pos);
            break;
        default:
            return;
    }

    arena->towers[i] = neu;
    arena->colliders[TOWER_TAG][j] = towerGetHitBox(neu);
    towerFree(old);
    arenaCloseUpgradeMenu(arena);
}

/** @brief Callback: confirma upgrade para torre rapida.
 *  @param[in] self Botao (nao usado).
 */
static void arenaUpgradeConfirmFastCb(Button* self) {
    (void)self;
    if (s_arena_upgrade_button_ctx) arenaReplaceTargetTower(s_arena_upgrade_button_ctx, TOWER_KIND_FAST);
}

/** @brief Callback: confirma upgrade para torre piercing.
 *  @param[in] self Botao (nao usado).
 */
static void arenaUpgradeConfirmPiercingCb(Button* self) {
    (void)self;
    if (s_arena_upgrade_button_ctx) arenaReplaceTargetTower(s_arena_upgrade_button_ctx, TOWER_KIND_PIERCING);
}

/** @brief Callback: confirma upgrade para morteiro.
 *  @param[in] self Botao (nao usado).
 */
static void arenaUpgradeConfirmMortarCb(Button* self) {
    (void)self;
    if (s_arena_upgrade_button_ctx) arenaReplaceTargetTower(s_arena_upgrade_button_ctx, TOWER_KIND_MORTAR);
}

/** @brief Callback: confirma upgrade para torre electrica.
 *  @param[in] self Botao (nao usado).
 */
static void arenaUpgradeConfirmEletrictCb(Button* self) {
    (void)self;
    if (s_arena_upgrade_button_ctx) arenaReplaceTargetTower(s_arena_upgrade_button_ctx, TOWER_KIND_ELETRICT);
}

/** @brief Abre menu de upgrade para torre basica no indice ti.
 *  @param[in,out] arena Arena.
 *  @param[in] ti Indice em towers[].
 */
static void basicTowerClick(Arena* arena, int ti) {
    arena->upgradePhase = ARENA_UPGRADE_MENU_OPEN;
    arena->upgradeTargetIndex = ti;
    buttonReset(arena->upgradeHammerLeftBasic);
    buttonReset(arena->upgradeHammerRightBasic);
    arena->towerType = TOWER_KIND_BASIC;
}

/** @brief Abre menu de upgrade para torre longa no indice ti.
 *  @param[in,out] arena Arena.
 *  @param[in] ti Indice em towers[].
 */
static void longTowerClick(Arena* arena, int ti) {
    arena->upgradePhase = ARENA_UPGRADE_MENU_OPEN;
    arena->upgradeTargetIndex = ti;
    buttonReset(arena->upgradeHammerLeftLong);
    buttonReset(arena->upgradeHammerRightLong);
    arena->towerType = TOWER_KIND_LONG;
}

/** @brief Detecta clique numa torre basica/long e abre UI de upgrade.
 *  @param[in,out] arena Arena.
 */
static void arenaTryOpenUpgradeFromTowerClick(Arena* arena) {
    if (arena->constructPhase != ARENA_CONSTRUCT_NORMAL || arena->upgradePhase != ARENA_UPGRADE_IDLE) return;
    Cursor* cur = getCursor();
    if (!cursorIsLeftButtonPressedDown(cur)) return;

    int cx = cursorGetX(cur);
    int cy = cursorGetY(cur);
    Vec2d click = {(float)cx, (float)cy};

    for (int ti = arena->towersN - 1; ti >= 0; ti--) {
        GenericTower* t = arena->towers[ti];
        if (!boxIsCollidingWithPoint(towerGetHitBox(t), click)) continue;
        if (towerIsBasic(t)) {
            basicTowerClick(arena, ti);
            return;
        } else if (towerIsLong(t)) {
            longTowerClick(arena, ti);
            return;
        }
    }
}

/** @brief Input e hover do menu de upgrade para torre basica.
 *  @param[in,out] arena Arena.
 */
static void arenaHandleUpgradeUiBasic(Arena* arena) {
    s_arena_upgrade_button_ctx = arena;
    Cursor* cur = getCursor();
    Vec2d mousePos = {(float)cursorGetX(cur), (float)cursorGetY(cur)};
    bool leftClick = cursorIsLeftButtonPressedDown(cur);
    int mx = (int)mousePos.x;
    int my = (int)mousePos.y;

    if (buttonContainsPoint(arena->upgradeHammerLeftBasic, mx, my)) {
        if (leftClick) buttonClick(arena->upgradeHammerLeftBasic);
        buttonHover(arena->upgradeHammerLeftBasic);
    } else {
        buttonReset(arena->upgradeHammerLeftBasic);
    }

    if (buttonContainsPoint(arena->upgradeHammerRightBasic, mx, my)) {
        if (leftClick) buttonClick(arena->upgradeHammerRightBasic);
        buttonHover(arena->upgradeHammerRightBasic);
    } else {
        buttonReset(arena->upgradeHammerRightBasic);
    }

    if (leftClick) {
        if (buttonContainsPoint(arena->upgradeHammerLeftBasic, mx, my) || buttonContainsPoint(arena->upgradeHammerRightBasic, mx, my)) {
            s_arena_upgrade_button_ctx = NULL;
            return;
        }
        if (!boxIsCollidingWithPoint(arena->upgradeScrollLeftBasicHitbox, mousePos) && !boxIsCollidingWithPoint(arena->upgradeScrollRightBasicHitbox, mousePos))
            arenaCloseUpgradeMenu(arena);
    }

    s_arena_upgrade_button_ctx = NULL;
}

/** @brief Input e hover do menu de upgrade para torre longa.
 *  @param[in,out] arena Arena.
 */
static void arenaHandleUpgradeUiLong(Arena* arena) {
    s_arena_upgrade_button_ctx = arena;
    Cursor* cur = getCursor();
    Vec2d mousePos = {(float)cursorGetX(cur), (float)cursorGetY(cur)};
    bool leftClick = cursorIsLeftButtonPressedDown(cur);
    int mx = (int)mousePos.x;
    int my = (int)mousePos.y;

    if (buttonContainsPoint(arena->upgradeHammerLeftLong, mx, my)) {
        if (leftClick) buttonClick(arena->upgradeHammerLeftLong);
        buttonHover(arena->upgradeHammerLeftLong);
    } else {
        buttonReset(arena->upgradeHammerLeftLong);
    }

    if (buttonContainsPoint(arena->upgradeHammerRightLong, mx, my)) {
        if (leftClick) buttonClick(arena->upgradeHammerRightLong);
        buttonHover(arena->upgradeHammerRightLong);
    } else {
        buttonReset(arena->upgradeHammerRightLong);
    }

    if (leftClick) {
        if (buttonContainsPoint(arena->upgradeHammerLeftLong, mx, my) || buttonContainsPoint(arena->upgradeHammerRightLong, mx, my)) {
            s_arena_upgrade_button_ctx = NULL;
            return;
        }
        if (!boxIsCollidingWithPoint(arena->upgradeScrollLeftBasicHitbox, mousePos) && !boxIsCollidingWithPoint(arena->upgradeScrollRightBasicHitbox, mousePos)) arenaCloseUpgradeMenu(arena);
    }

    s_arena_upgrade_button_ctx = NULL;
}

/** @brief Desenha sprites e botoes do menu de upgrade basico.
 *  @param[in] arena Arena.
 *  @param[in] terminal Terminal.
 *  @param[in] deltaTime Delta para animacao dos botoes.
 */
static void arenaDrawUpgradeMenuBasic(Arena* arena, Terminal* terminal, float deltaTime) {
    drawSpriteXPM(arena->upgradeScrollLeftBasic, terminal, UPGRADE_SCROLL_LEFT_X, UPGRADE_SCROLL_ORIGIN_Y, false);
    drawSpriteXPM(arena->upgradeScrollRightBasic, terminal, UPGRADE_SCROLL_RIGHT_X, UPGRADE_SCROLL_ORIGIN_Y, false);
    buttonDraw(arena->upgradeHammerLeftBasic, terminal, deltaTime, false);
    buttonDraw(arena->upgradeHammerRightBasic, terminal, deltaTime, false);
}

/** @brief Desenha menu de upgrade para torre longa.
 *  @param[in] arena Arena.
 *  @param[in] terminal Terminal.
 *  @param[in] deltaTime Delta para animacao.
 */
static void arenaDrawUpgradeMenuLong(Arena* arena, Terminal* terminal, float deltaTime) {
    drawSpriteXPM(arena->upgradeScrollLeftLong, terminal, UPGRADE_SCROLL_LEFT_X, UPGRADE_SCROLL_ORIGIN_Y, false);
    drawSpriteXPM(arena->upgradeScrollRightLong, terminal, UPGRADE_SCROLL_RIGHT_X, UPGRADE_SCROLL_ORIGIN_Y, false);
    buttonDraw(arena->upgradeHammerLeftLong, terminal, deltaTime, false);
    buttonDraw(arena->upgradeHammerRightLong, terminal, deltaTime, false);
}

/** @brief Le CSV de tiles, cria Tile* e colliders de paredes/bordas.
 *  @param[in,out] arena Arena.
 *  @param[in] filename Caminho do ficheiro walls/tiles.
 *  @param[in] tileSize Tamanho em pixels de cada celula.
 */
static void createMap(Arena* arena, const char* filename, int tileSize) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open map file: %s\n", filename);
        return;
    }

    char lineBuffer[1024];

    arena->tiles = malloc(sizeof(Tile*) * (MAP_TILES_X * MAP_TILES_Y + 4));
    arena->width = MAP_TILES_X;
    arena->height = MAP_TILES_Y;
    arena->tileSize = tileSize;

    int** map = malloc(arena->height * sizeof(int*));
    for (int i = 0; i < arena->height; i++) {
        map[i] = malloc(arena->width * sizeof(int));

        if (fgets(lineBuffer, sizeof(lineBuffer), file) == NULL) {
            break;
        }

        char* ptr = lineBuffer;
        for (int j = 0; j < arena->width; j++) {
            map[i][j] = atoi(ptr);
            while (*ptr != '\0' && *ptr != '\n' && *ptr != '\r' && *ptr != ',') ptr++;
            if (*ptr == ',') ptr++;
        }
    }

    fclose(file);
    int i = 0;

    for (int y = 0; y < arena->height; y++) {
        for (int x = 0; x < arena->width; x++) {
            int id = map[y][x];
            if (id <= 0) continue;

            char** sprite = tiles[id - 1];
            arena->tiles[i] = tileNew(sprite, id, x * tileSize, y * tileSize, tileSize, tileSize);
            i++;
        }
    }

    arena->tiles[i] = tileNew(NULL, -1, 0, -10, MAP_TILES_X * TILE_SIZE, 10);
    arena->tiles[i + 1] = tileNew(NULL, -1, 0, MAP_TILES_Y * TILE_SIZE, MAP_TILES_X * TILE_SIZE, 10);
    arena->tiles[i + 2] = tileNew(NULL, -1, -10, 0, 10, MAP_TILES_Y * TILE_SIZE);
    arena->tiles[i + 3] = tileNew(NULL, -1, MAP_TILES_X * TILE_SIZE, 0, 10, MAP_TILES_Y * TILE_SIZE);
    i += 4;
    arena->tilesN = i;

    arena->collidersN[WALLS_TAG] = arena->tilesN;
    arena->colliders[WALLS_TAG] = malloc(sizeof(BoxCollider*) * (i + 4));
    for (int i = 0; i < arena->height; i++) free(map[i]);
    for (int j = 0; j < arena->tilesN; j++) arena->colliders[WALLS_TAG][j] = tileBoxCollider(arena->tiles[j]);

    free(map);
}

/** @brief Le ficheiro de path e cria colliders PATH_TAG para colocacao de torres.
 *  @param[in,out] arena Arena.
 *  @param[in] filename CSV do path.
 */
static void createMonsterPathColliders(Arena* arena, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open map file: %s\n", filename);
        return;
    }

    char lineBuffer[1024];
    int** map = malloc(arena->height * sizeof(int*));
    int num = 0;

    for (int i = 0; i < arena->height; i++) {
        map[i] = malloc(arena->width * sizeof(int));

        if (fgets(lineBuffer, sizeof(lineBuffer), file) == NULL) {
            break;
        }

        char* ptr = lineBuffer;
        for (int j = 0; j < arena->width; j++) {
            map[i][j] = atoi(ptr);
            if (map[i][j] > 0) num++;

            while (*ptr != '\0' && *ptr != '\n' && *ptr != '\r' && *ptr != ',') ptr++;
            if (*ptr == ',') ptr++;
        }
    }

    fclose(file);
    arena->collidersN[PATH_TAG] = num;
    arena->colliders[PATH_TAG] = malloc(sizeof(BoxCollider*) * num);
    int i = 0;

    for (int y = 0; y < arena->height; y++) {
        for (int x = 0; x < arena->width; x++) {
            int id = map[y][x];
            if (id <= 0) continue;

            Vec2d position = {x * TILE_SIZE, y * TILE_SIZE};
            Vec2d size = {TILE_SIZE, TILE_SIZE};
            arena->colliders[PATH_TAG][i] = boxColliderNew(position, EMPTY_VEC2D, size, PATH_TAG);
            i++;
        }
    }

    for (int i = 0; i < arena->height; i++) free(map[i]);
    free(map);
}

/** @brief Le ficheiro de agua e cria colliders LAKE_TAG.
 *  @param[in,out] arena Arena.
 *  @param[in] filename CSV de agua.
 */
static void createLakeColliders(Arena* arena, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open map file: %s\n", filename);
        return;
    }

    char lineBuffer[1024];
    int** map = malloc(arena->height * sizeof(int*));
    int num = 0;

    for (int i = 0; i < arena->height; i++) {
        map[i] = malloc(arena->width * sizeof(int));

        if (fgets(lineBuffer, sizeof(lineBuffer), file) == NULL) {
            break;
        }

        char* ptr = lineBuffer;
        for (int j = 0; j < arena->width; j++) {
            map[i][j] = atoi(ptr);
            if (map[i][j] > 0) num++;

            while (*ptr != '\0' && *ptr != '\n' && *ptr != '\r' && *ptr != ',') ptr++;
            if (*ptr == ',') ptr++;
        }
    }

    fclose(file);
    arena->collidersN[LAKE_TAG] = num;
    arena->colliders[LAKE_TAG] = malloc(sizeof(BoxCollider*) * num);
    int idx = 0;

    for (int y = 0; y < arena->height; y++) {
        for (int x = 0; x < arena->width; x++) {
            int id = map[y][x];
            if (id <= 0) continue;

            Vec2d position = {x * TILE_SIZE, y * TILE_SIZE};
            Vec2d size = {TILE_SIZE, TILE_SIZE};
            arena->colliders[LAKE_TAG][idx] = boxColliderNew(position, EMPTY_VEC2D, size, LAKE_TAG);
            idx++;
        }
    }

    for (int i = 0; i < arena->height; i++) free(map[i]);
    free(map);
}

/** @brief Instancia o jogador e regista o collider em PLAYER_TAG.
 *  @param[in,out] arena Arena.
 */
static void createPlayer(Arena* arena) {
    arena->player = playerNew(PLAYER_POS);

    arena->colliders[PLAYER_TAG] = malloc(sizeof(BoxCollider*));
    arena->colliders[PLAYER_TAG][0] = playerModelGetHitBox(playerGetPlayerModel(arena->player));
    arena->collidersN[PLAYER_TAG] = 1;
}

/** @brief Cria sprites e botoes do menu de upgrade (basico e longo).
 *  @param[in,out] res Arena onde se guarda a UI.
 */
static void createUpgradeUI(Arena* res) {
    res->upgradeScrollLeftBasic = newSpriteXPM((xpm_map_t)upgradeMenuXPM[UPGRADE_MENU_XPM_FAST], XPM_8_8_8, 0, 0);
    res->upgradeScrollRightBasic = newSpriteXPM((xpm_map_t)upgradeMenuXPM[UPGRADE_MENU_XPM_PIERCING], XPM_8_8_8, 0, 0);
    res->upgradeScrollLeftBasicHitbox = boxColliderNew((Vec2d){(float)UPGRADE_SCROLL_LEFT_X, (float)UPGRADE_SCROLL_ORIGIN_Y}, EMPTY_VEC2D, (Vec2d){(float)UPGRADE_SCROLL_W, (float)UPGRADE_SCROLL_H}, UI_TAG);
    res->upgradeScrollRightBasicHitbox = boxColliderNew((Vec2d){(float)UPGRADE_SCROLL_RIGHT_X, (float)UPGRADE_SCROLL_ORIGIN_Y}, EMPTY_VEC2D, (Vec2d){(float)UPGRADE_SCROLL_W, (float)UPGRADE_SCROLL_H}, UI_TAG);

    SpriteXPM** hammerSpritesLeft = malloc(sizeof(SpriteXPM*) * UPGRADE_HAMMER_FRAMES);
    hammerSpritesLeft[0] = newSpriteXPM((xpm_map_t)iconsXPM[UPGRADE_HAMMER_NORMAL], XPM_8_8_8, 0, 0);
    hammerSpritesLeft[1] = newSpriteXPM((xpm_map_t)iconsXPM[UPGRADE_HAMMER_PRESSED], XPM_8_8_8, 0, 0);
    res->upgradeHammerLeftBasic = buttonNew(UPGRADE_HAMMER_LEFT_X, UPGRADE_HAMMER_LEFT_Y, UPGRADE_HAMMER_W, UPGRADE_HAMMER_H, animatedSpriteNew((float)UPGRADE_HAMMER_FRAME_SPEED, hammerSpritesLeft, UPGRADE_HAMMER_FRAMES, NORMAL), arenaUpgradeConfirmFastCb, basicButtonHoverFn);

    SpriteXPM** hammerSpritesRight = malloc(sizeof(SpriteXPM*) * UPGRADE_HAMMER_FRAMES);
    hammerSpritesRight[0] = newSpriteXPM((xpm_map_t)iconsXPM[UPGRADE_HAMMER_NORMAL], XPM_8_8_8, 0, 0);
    hammerSpritesRight[1] = newSpriteXPM((xpm_map_t)iconsXPM[UPGRADE_HAMMER_PRESSED], XPM_8_8_8, 0, 0);
    res->upgradeHammerRightBasic = buttonNew(UPGRADE_HAMMER_RIGHT_X, UPGRADE_HAMMER_RIGHT_Y, UPGRADE_HAMMER_W, UPGRADE_HAMMER_H, animatedSpriteNew((float)UPGRADE_HAMMER_FRAME_SPEED, hammerSpritesRight, UPGRADE_HAMMER_FRAMES, NORMAL), arenaUpgradeConfirmPiercingCb, basicButtonHoverFn);

    // Long
    res->upgradeScrollLeftLong = newSpriteXPM((xpm_map_t)upgradeMenuXPM[UPGRADE_MENU_XPM_MORTAR], XPM_8_8_8, 0, 0);
    res->upgradeScrollRightLong = newSpriteXPM((xpm_map_t)upgradeMenuXPM[UPGRADE_MENU_XPM_ELECTRICT], XPM_8_8_8, 0, 0);

    hammerSpritesLeft = malloc(sizeof(SpriteXPM*) * UPGRADE_HAMMER_FRAMES);
    hammerSpritesLeft[0] = newSpriteXPM((xpm_map_t)iconsXPM[UPGRADE_HAMMER_NORMAL], XPM_8_8_8, 0, 0);
    hammerSpritesLeft[1] = newSpriteXPM((xpm_map_t)iconsXPM[UPGRADE_HAMMER_PRESSED], XPM_8_8_8, 0, 0);
    res->upgradeHammerLeftLong = buttonNew(UPGRADE_HAMMER_LEFT_X, UPGRADE_HAMMER_LEFT_Y, UPGRADE_HAMMER_W, UPGRADE_HAMMER_H, animatedSpriteNew((float)UPGRADE_HAMMER_FRAME_SPEED, hammerSpritesLeft, UPGRADE_HAMMER_FRAMES, NORMAL), arenaUpgradeConfirmMortarCb, basicButtonHoverFn);

    hammerSpritesRight = malloc(sizeof(SpriteXPM*) * UPGRADE_HAMMER_FRAMES);
    hammerSpritesRight[0] = newSpriteXPM((xpm_map_t)iconsXPM[UPGRADE_HAMMER_NORMAL], XPM_8_8_8, 0, 0);
    hammerSpritesRight[1] = newSpriteXPM((xpm_map_t)iconsXPM[UPGRADE_HAMMER_PRESSED], XPM_8_8_8, 0, 0);
    res->upgradeHammerRightLong = buttonNew(UPGRADE_HAMMER_RIGHT_X, UPGRADE_HAMMER_RIGHT_Y, UPGRADE_HAMMER_W, UPGRADE_HAMMER_H, animatedSpriteNew((float)UPGRADE_HAMMER_FRAME_SPEED, hammerSpritesRight, UPGRADE_HAMMER_FRAMES, NORMAL), arenaUpgradeConfirmEletrictCb, basicButtonHoverFn);
}

/** @brief Baralha array de inteiros (Fisher-Yates simplificado).
 *  @param[in,out] arr Array.
 *  @param[in] n Comprimento.
 */
static void arena_shuffle_ints(int* arr, unsigned n) {
    for (unsigned i = n; i > 1u; i--) {
        unsigned j = (unsigned)rand() % i;
        int t = arr[j];
        arr[j] = arr[i - 1u];
        arr[i - 1u] = t;
    }
}

/** @brief Preenche wave_tokens e tempos de spawn para a onda wave_index.
 *  @param[in,out] arena Arena.
 *  @param[in] wave_index Indice da onda (minuto ou indice logico).
 */
static void arena_fill_wave_tokens(Arena* arena, int wave_index) {
    WaveComposition wc = {0u, 0u, 0u};
    waves_get_composition(arena->level, (unsigned)wave_index, &wc);
    unsigned n = wc.pawn + wc.alien + wc.spear;
    if (n == 0u) {
        arena->wave_token_total = 0u;
        arena->wave_token_cursor = 0u;
        return;
    }
    if (n > ARENA_WAVE_MAX_SPAWNS) n = ARENA_WAVE_MAX_SPAWNS;

    unsigned i = 0;
    for (unsigned k = 0; k < wc.pawn && i < n; k++) arena->wave_tokens[i++] = (int)ARENA_WAVE_KIND_PAWN;
    for (unsigned k = 0; k < wc.alien && i < n; k++) arena->wave_tokens[i++] = (int)ARENA_WAVE_KIND_ALIEN;
    for (unsigned k = 0; k < wc.spear && i < n; k++) arena->wave_tokens[i++] = (int)ARENA_WAVE_KIND_SPEAR;

    arena_shuffle_ints(arena->wave_tokens, n);

    float minute_start = (float)wave_index * 60.0f;
    for (unsigned k = 0; k < n; k++) {
        float jitter = ((float)(rand() % 17) - 8.0f) / 200.0f;
        float slot = ((float)k + 0.5f) * 60.0f / (float)n;
        arena->wave_spawn_time_abs[k] = minute_start + slot + jitter;
        if (arena->wave_spawn_time_abs[k] < minute_start) arena->wave_spawn_time_abs[k] = minute_start;
        if (arena->wave_spawn_time_abs[k] >= minute_start + 60.0f) arena->wave_spawn_time_abs[k] = minute_start + 59.5f;
    }

    arena->wave_token_total = n;
    arena->wave_token_cursor = 0u;
}

/** @brief Numero de paths paralelos activos para distribuir spawns.
 *  @param[in] arena Arena.
 *  @return Numero de paths (>=1).
 */
static unsigned arena_paths_count(const Arena* arena) {
    if (arena->wave_gen_count < 3u) return 1u;
    return arena->wave_gen_count / 3u;
}

/** @brief Dispara monsterGenSpawnOne para cada token cuja hora de spawn ja passou.
 *  @param[in,out] arena Arena.
 */
static void arena_emit_wave_spawns(Arena* arena) {
    unsigned n_paths = arena_paths_count(arena);
    if (n_paths == 0u) n_paths = 1u;

    while (arena->wave_token_cursor < arena->wave_token_total &&
           arena->wave_spawn_time_abs[arena->wave_token_cursor] <= arena->match_time_sec) {
        int kind = arena->wave_tokens[arena->wave_token_cursor];
        if (kind < 0 || kind > 2) kind = 0;
        unsigned path_i = (unsigned)rand() % n_paths;
        unsigned gen_idx = path_i * 3u + (unsigned)kind;
        if (gen_idx >= arena->wave_gen_count) gen_idx = arena->wave_gen_count - 1u;
        MonsterGen* mg = *(MonsterGen**)vectorGet(arena->monsterGenList, gen_idx);
        monsterGenSpawnOne(mg);
        arena->wave_token_cursor++;
    }
}

/** @brief Invoca spawn do boss quando o tempo e o nivel o permitem.
 *  @param[in,out] arena Arena.
 */
static void arena_try_spawn_boss(Arena* arena) {
    if(!arena->ignore_boss_win){
    if (!waves_level_has_boss(arena->level)) return;
    if (arena->match_time_sec < 420.0f) return;
    arenaSpawnBoss(arena);
    }
}

/** @brief Actualiza relogio de partida, ondas e condicoes de vitoria antecipada (nivel 0).
 *  @param[in,out] arena Arena.
 *  @param[in] delta_t Delta em segundos.
 */
static void arena_wave_tick(Arena* arena, float delta_t) {
    if (arena->outcome != ARENA_OUTCOME_PLAYING) return;

    arena->match_time_sec += delta_t;

    if (arena->level == 0 && arena->match_time_sec >= 420.0f) {
        arena->outcome = ARENA_OUTCOME_WIN;
        arena->wave_token_total = 0u;
        arena->wave_token_cursor = 0u;
        return;
    }

    arena_try_spawn_boss(arena);

    int cw = (int)(arena->match_time_sec / 60.0f);
    if (cw < 0) cw = 0;

    arena_emit_wave_spawns(arena);

    if (cw != arena->prepared_wave_index) {
        bool boss_level = waves_level_has_boss(arena->level);
        if (boss_level && cw >= 7) {
            arena->wave_token_total = 0u;
            arena->wave_token_cursor = 0u;
        } else if (arena->level == 0 && cw >= 7) {
            arena->wave_token_total = 0u;
            arena->wave_token_cursor = 0u;
        } else if ((unsigned)cw < waves_normal_wave_count()) {
            arena_fill_wave_tokens(arena, cw);
        } else {
            arena->wave_token_total = 0u;
            arena->wave_token_cursor = 0u;
        }
        arena->prepared_wave_index = cw;
        arena_emit_wave_spawns(arena);
    }
}

/** @brief Marca vitoria se o boss foi derrotado.
 *  @param[in,out] arena Arena.
 */
static void arena_check_boss_win(Arena* arena) {
    if (arena->outcome != ARENA_OUTCOME_PLAYING) return;
    if (!waves_level_has_boss(arena->level)) return;
    if (!arena->boss_spawned || arena->boss_gen == NULL) return;
    if (monsterGenBossDefeated(arena->boss_gen)) arena->outcome = ARENA_OUTCOME_WIN;
}

Arena* arenaNew(Terminal* terminal, const char* tilesFile, const char* pathFile, const char* waterFile, char** background, Vector* paths, Vector* gens, int bananaStart, int level) {
    Arena* res = malloc(sizeof(Arena));
    res->collidersN = malloc(sizeof(int) * TAGS_NUM);
    res->colliders = malloc(sizeof(BoxCollider*) * TAGS_NUM);
    for (int t = 0; t < TAGS_NUM; t++) {
        res->collidersN[t] = 0;
        res->colliders[t] = NULL;
    }
     
    if(level==2){
        res->ignore_boss_win=true;
    }
    else{
        res->ignore_boss_win=false;
    }
	res->pathList = paths;
	res->monsterGenList = gens;

    res->fruits = NULL;
    res->fruitsN = 0;
    res->carrotSpawnCooldown = FRUIT_CARROT_SPAWN_INTERVAL_SEC * 0.5f;
 
    /*
    res->collidersN[MONSTER_TAG] = 1;
    res->colliders[MONSTER_TAG] = malloc(sizeof(BoxCollider*) * res->collidersN[MONSTER_TAG]);
    res->colliders[MONSTER_TAG][0] = monsterGetBoxCollider(res->monster_test);
    */ 
    res->collidersN[MONSTER_TAG] = 0;
    res->colliders[MONSTER_TAG] = malloc(sizeof(BoxCollider*) * res->collidersN[MONSTER_TAG]);
 

    createMap(res, tilesFile, TILE_SIZE);
    createMonsterPathColliders(res, pathFile);
    createLakeColliders(res, waterFile);
    createPlayer(res);
    res->towersN = 0;
    res->towers = malloc(sizeof(GenericTower*) * res->towersN);

    res->constructPhase = ARENA_CONSTRUCT_NORMAL;
    res->upgradePhase = ARENA_UPGRADE_IDLE;
    res->upgradeTargetIndex = -1;
    createUpgradeUI(res);

    SpriteXPM* backgroundSprite = newSpriteXPM((xpm_map_t)background, XPM_8_8_8, 0, 0);
    generateEmptyBuffer(backgroundSprite, terminal, 0, 0, false);

    for (int i = 0; i < res->tilesN; i++) {
        Tile* t = res->tiles[i];
        if (t->sprite != NULL) {
            drawSpriteXPMToEmptyBuffer(t->sprite, terminal, (int)t->pos.x, (int)t->pos.y, false);
            freeSpriteXPM(t->sprite);
            t->sprite = NULL;
        }
    }

    res->HeroHP = 20;
    res->healthBar = healthBarNew(res->HeroHP);
    res->statsBar = statsBarNew();
    res->basicTowerBuildMenu = buildMenuNew(TOWER_KIND_BASIC);
    res->longTowerBuildMenu = buildMenuNew(TOWER_KIND_LONG);

    res->level = level;
    res->match_time_sec = 0.0f;
    res->prepared_wave_index = -1;
    res->wave_token_total = 0u;
    res->wave_token_cursor = 0u;
    res->wave_gen_count = (unsigned)vectorGetSize(gens);
    res->boss_spawned = false;
    res->boss_gen = NULL;
    res->outcome = ARENA_OUTCOME_PLAYING;

    statsBarSetBanana(res->statsBar, bananaStart);
    statsBarSetAvocado(res->statsBar, 0);
    statsBarSetCherry(res->statsBar, 0);
    statsBarSetCarrot(res->statsBar, 0); 

    return res;
}

bool arenaCanPlaceTower(Arena* arena, Vec2d position) {
    BoxCollider* preview = boxColliderNew(position, BASIC_TOWER_HIT_BOX_OFFSET, BASIC_TOWER_SIZE, TOWER_TAG);
    BoxCollider* hits[PLACEMENT_QUERY_MAX];

    if (arenaDetectCollisions(arena, preview, WALLS_TAG, hits) > 0) {
        boxColliderFree(preview);
        return false;
    }
    if (arenaDetectCollisions(arena, preview, TOWER_TAG, hits) > 0) {
        boxColliderFree(preview);
        return false;
    }
    if (arenaDetectCollisions(arena, preview, PLAYER_TAG, hits) > 0) {
        boxColliderFree(preview);
        return false;
    }
    if (arenaDetectCollisions(arena, preview, PATH_TAG, hits) > 0) {
        boxColliderFree(preview);
        return false;
    }
    if (arenaDetectCollisions(arena, preview, LAKE_TAG, hits) > 0) {
        boxColliderFree(preview);
        return false;
    }

    boxColliderFree(preview);
    return true;
}

void arenaBuildTower(Arena* arena, Vec2d position) {
    if (!arenaCanPlaceTower(arena, position)) return;

    arena->towersN++;
    arena->towers = realloc(arena->towers, sizeof(GenericTower*) * arena->towersN);

    if (arena->towerType == TOWER_KIND_BASIC) {
        arena->towers[arena->towersN - 1] = basicTowerNew(position);
    } else {
        arena->towers[arena->towersN - 1] = basicLongTowerNew(position);
    }

    arena->collidersN[TOWER_TAG]++;
    arena->colliders[TOWER_TAG] = realloc(arena->colliders[TOWER_TAG], sizeof(BoxCollider*) * arena->collidersN[TOWER_TAG]);
    arena->colliders[TOWER_TAG][arena->collidersN[TOWER_TAG] - 1] = towerGetHitBox(arena->towers[arena->towersN - 1]);
}

void arenaFree(Arena* arena) {
    for (int i = 0; i < arena->collidersN[PATH_TAG]; i++) boxColliderFree(arena->colliders[PATH_TAG][i]);
    free(arena->colliders[PATH_TAG]);

    for (int i = 0; i < arena->collidersN[LAKE_TAG]; i++) boxColliderFree(arena->colliders[LAKE_TAG][i]);
    free(arena->colliders[LAKE_TAG]);

    for (int i = 0; i < arena->towersN; i++) towerFree(arena->towers[i]);
    free(arena->towers);

    freeSpriteXPM(arena->upgradeScrollLeftBasic);
    freeSpriteXPM(arena->upgradeScrollRightBasic);
    boxColliderFree(arena->upgradeScrollLeftBasicHitbox);
    boxColliderFree(arena->upgradeScrollRightBasicHitbox);
    buttonFree(arena->upgradeHammerLeftBasic);
    buttonFree(arena->upgradeHammerRightBasic);

    freeSpriteXPM(arena->upgradeScrollLeftLong);
    freeSpriteXPM(arena->upgradeScrollRightLong);
    buttonFree(arena->upgradeHammerLeftLong);
    buttonFree(arena->upgradeHammerRightLong);

    buildMenuFree(arena->basicTowerBuildMenu);
    buildMenuFree(arena->longTowerBuildMenu);

    for (int i = 0; i < arena->tilesN; i++) tileFree(arena->tiles[i]);
    for (int i = 0; i < arena->fruitsN; i++) collectableFree(arena->fruits[i].collectable);
    free(arena->fruits);
 
    vectorFree(arena->monsterGenList);
    vectorFree(arena->pathList); 

    free(arena->colliders[WALLS_TAG]);
    free(arena->colliders[TOWER_TAG]);
    free(arena->colliders[PLAYER_TAG]);
    free(arena->colliders[COLLECTABLE_TAG]);
    free(arena->tiles);

    playerFree(arena->player);

    free(arena->colliders);
    free(arena->collidersN);
    healthBarFree(arena->healthBar);
    statsBarFree(arena->statsBar);

    free(arena);
}

int arenaDetectCollisions(Arena* arena, BoxCollider* box, int tag, BoxCollider** colliders) {
    if (tag < 0 || tag >= TAGS_NUM) return 0;
    int n = arena->collidersN[tag];
    BoxCollider** c = arena->colliders[tag];
    if (c == NULL || n == 0) return 0;

    int j = 0;
    for (int i = 0; i < n; i++) {
        BoxCollider* collider = c[i];
        if (collider == NULL) continue;
        if (boxIsCollidingWithBox(box, collider)) {
            colliders[j] = collider;
            j++;
            if (j >= PLACEMENT_QUERY_MAX) break;
        }
    }

    return j;
}

/** @brief Detecta colisao jogador-fruto, credita stats bar e remove o fruto.
 *  @param[in,out] arena Arena.
 */
static void arenaCollectableHandler(Arena* arena) {
    BoxCollider* hitBox = playerModelGetHitBox(playerGetPlayerModel(arena->player));
    BoxCollider* collisions[PLACEMENT_QUERY_MAX];
    int n = arenaDetectCollisions(arena, hitBox, COLLECTABLE_TAG, collisions);
    for (int k = 0; k < n; k++) {
        int found = -1;
        for (int i = 0; i < arena->fruitsN; i++) {
            if (collectableGetHitBox(arena->fruits[i].collectable) == collisions[k]) {
                found = i;
                break;
            }
        }
        if (found < 0) break;
        arena_pickup_add_fruit_to_stats_bar(arena->statsBar, arena->fruits[found].kind);
        arena_fruit_remove_at(arena, found);
    }
}

/** @brief Fluxo de construcao de torres, menus de upgrade e modo de cursor.
 *  @param[in,out] arena Arena.
 *  @param[in] terminal Terminal.
 */
static void arenaHandleConstruction(Arena* arena, Terminal* terminal) {
    Cursor* cur = getCursor();
    Vec2d mousePos = {cursorGetX(cur), cursorGetY(cur)};

    if (arena->upgradePhase == ARENA_UPGRADE_MENU_OPEN) {
        if (arena->towerType == TOWER_KIND_BASIC) {
            arenaHandleUpgradeUiBasic(arena);
        } else {
            arenaHandleUpgradeUiLong(arena);
        }

        return;
    }

    if (arena->constructPhase == ARENA_CONSTRUCT_NORMAL) {
        cursorSetConstructionMode(cur, false);
        if (cursorIsRightButtonPressedDown(cur)) {
            arena->constructPhase = ARENA_CONSTRUCT_PICK_TOWER;
            return;
        }

        arenaTryOpenUpgradeFromTowerClick(arena);
        return;
    }

    if (arena->constructPhase == ARENA_CONSTRUCT_PICK_TOWER) {
        cursorSetConstructionMode(cur, false);
        if (cursorIsRightButtonPressedDown(cur)) {
            arena->constructPhase = ARENA_CONSTRUCT_NORMAL;
            return;
        }

        if (cursorIsLeftButtonPressed(cur)) {
            if (!boxIsCollidingWithPoint(arena->upgradeScrollLeftBasicHitbox, mousePos) && !boxIsCollidingWithPoint(arena->upgradeScrollRightBasicHitbox, mousePos)) {
                arena->constructPhase = ARENA_CONSTRUCT_NORMAL;
                return;
            }

            if (buildMenuIsPressed(arena->basicTowerBuildMenu) && buyTower(arena, TOWER_KIND_BASIC)) {
                arena->towerType = TOWER_KIND_BASIC;
                buildMenuReset(arena->basicTowerBuildMenu);
                arena->constructPhase = ARENA_CONSTRUCT_PLACE_TOWER;

            } else if (buildMenuIsPressed(arena->longTowerBuildMenu) && buyTower(arena, TOWER_KIND_LONG)) {
                arena->towerType = TOWER_KIND_LONG;
                buildMenuReset(arena->longTowerBuildMenu);
                arena->constructPhase = ARENA_CONSTRUCT_PLACE_TOWER;
            }

            if (cursorIsRightButtonPressedDown(getCursor())) arena->constructPhase = ARENA_CONSTRUCT_NORMAL;
        }
        return;
    }

    if (arena->constructPhase == ARENA_CONSTRUCT_PLACE_TOWER) {
        cursorSetConstructionMode(cur, true);
        Vec2d pos = {(float)cursorGetX(cur), (float)cursorGetY(cur)};
        bool ok = arenaCanPlaceTower(arena, pos);
        cursorSetPlacementHoverValid(cur, ok);

        if (cursorIsRightButtonPressedDown(cur)) {
            arena->constructPhase = ARENA_CONSTRUCT_NORMAL;
            cursorSetConstructionMode(cur, false);
            return;
        }
        if (cursorIsLeftButtonPressedDown(cur) && ok) {
            arenaBuildTower(arena, pos);
            cursorSetConstructionMode(cur, false);
            arena->constructPhase = ARENA_CONSTRUCT_NORMAL;
        }
    }
}

BoxCollider* arenaFindFirstEnemy(Arena* arena, BoxCollider* range) {
    float dist = (float)INT32_MAX;
    BoxCollider* resMonsterCollider = NULL;
    for (unsigned int i = 0; i < vectorGetSize(arena->monsterGenList); i++) {

        MonsterGen* mg = *(MonsterGen**)vectorGet(arena->monsterGenList, i);

        int iteratorIndex=0;
        int monsterIterator = monsterGenGetNextActiveMonsterIndex(mg, iteratorIndex);
        while (monsterIterator != -1) {
            Monster* monster = monsterGenGetMonster(mg, monsterIterator);
            BoxCollider* monsterCollider = monsterGetBoxCollider(monster);
            Vec2d position = {boxColliderGetX(monsterCollider), boxColliderGetY(monsterCollider)};
            Vec2d size = {boxColliderGetWidth(monsterCollider), boxColliderGetHeight(monsterCollider)};
            Vec2d center = {position.x + size.x / 2, position.y + size.y / 2};

            if (boxIsCollidingWithPoint(range, center)) {
                Vec2d range_position = {boxColliderGetX(range), boxColliderGetY(range)};
                Vec2d range_size = {boxColliderGetWidth(range), boxColliderGetHeight(range)};
                Vec2d range_center = {range_position.x + range_size.x / 2, range_position.y + range_size.y / 2};

                float newDist = vec2dDistance2Between(&center, &range_center);
                if (newDist < dist) {
                    dist = newDist;
                    resMonsterCollider = monsterCollider;
                }
                break;
            }
            iteratorIndex++;
            monsterIterator = monsterGenGetNextActiveMonsterIndex(mg, iteratorIndex);
        }
    }
    return resMonsterCollider;
}

void arenaHandleMonsterDamage(Arena* arena) {
    for (int t = 0; t < arena->towersN; t++) {
        GenericTower* tower = arena->towers[t];
        TowerKind kind = towerGetKind(tower);
        int projCount = towerGetProjectilesCount(tower);
        for (int i = 0; i < projCount; i++) {
            Projectile* arrow = towerGetProjectile(tower, i);
            if (!projectileIsActive(arrow)) continue;

            BoxCollider* projCollider = projectileGetCollider(arrow);

            for (unsigned int j = 0; j < vectorGetSize(arena->monsterGenList); j++) {
                MonsterGen* mg = *(MonsterGen**)vectorGet(arena->monsterGenList, j);
                for (unsigned int si = 0;; si++) {
                    if (!projectileIsActive(arrow)) break;

                    int mi = monsterGenGetNextActiveMonsterIndex(mg, si);
                    if (mi < 0) break;

                    Monster* monster = monsterGenGetMonster(mg, (unsigned int)mi);
                    if (boxIsCollidingWithBox(projCollider, monsterGetBoxCollider(monster))) {
                        if (!projectileHasHit(arrow, monsterGetBoxCollider(monster))) {
                            if (projectileGetType(arrow) == PROJ_SKY_FALL) {
                                monsterSumHP(monster, -SKY_FALL_DAMAGE);
                            } else if (projectileGetType(arrow) == PROJ_NEAR) {
                                monsterSumHP(monster, -NEAR_DAMAGE);
                            } else if (kind == TOWER_KIND_PIERCING) {
                                monsterSumHP(monster, -PIERCING_DAMAGE);
                            } else if (kind == TOWER_KIND_FAST) {
                                monsterSumHP(monster, -FAST_DAMAGE);
                            } else {
                                monsterSumHP(monster, -NORMAL_DAMAGE);
                            }
                            projectileAddPierce(arrow);
                            projectileAddHit(arrow, monsterGetBoxCollider(monster));
                        }
                    }
                }
            }
        }
    }
}

void arenaUpdate(Arena* arena, Terminal* terminal, float deltaTime) {
    arena_wave_tick(arena, deltaTime);

    arena_fruits_tick_time_to_live(arena, deltaTime);
    arena_carrot_spawn_tick(arena, deltaTime);
    for (int i = 0; i < arena->fruitsN; i++) collectableUpdate(arena->fruits[i].collectable, terminal);
    for (int i = 0; i < arena->tilesN; i++) tileUpdate(arena->tiles[i], terminal);
    for (int i = arena->towersN - 1; i >= 0; i--) towerUpdate(arena->towers[i], terminal, arena, deltaTime);

    arenaHandleMonsterDamage(arena);
 
    for (unsigned int i = 0; i < vectorGetSize(arena->monsterGenList); i++) {
        MonsterGen* mg = *(MonsterGen**)vectorGet(arena->monsterGenList, i);
        unsigned int leaked = monsterGenGetNumberOfInstantFinishedMonsters(mg);
        if (mg != arena->boss_gen) arena->HeroHP -= (int)leaked;
        monsterGenUpdate(mg, arena, terminal, deltaTime);
        if (arena->boss_gen == mg && monsterGenGetNumberOfInstantFinishedMonsters(mg) > 0u) {
            arena->HeroHP = 0;
            if (arena->outcome == ARENA_OUTCOME_PLAYING) arena->outcome = ARENA_OUTCOME_LOSE;
        }
    }

    if(!arena->ignore_boss_win){

    arena_check_boss_win(arena);
    }

    arenaCollectableHandler(arena);
    arenaHandleConstruction(arena, terminal);

    playerUpdate(arena->player, terminal, arena, deltaTime);
    for (int i = arena->towersN - 1; i >= 0; i--) towerDrawUpperHalf(arena->towers[i], terminal, arena, deltaTime);

    if (arena->upgradePhase == ARENA_UPGRADE_MENU_OPEN) {
        if (arena->towerType == TOWER_KIND_BASIC) {
            arenaDrawUpgradeMenuBasic(arena, terminal, deltaTime);
        } else {
            arenaDrawUpgradeMenuLong(arena, terminal, deltaTime);
        }

    } else if (arena->constructPhase == ARENA_CONSTRUCT_PICK_TOWER) {
        buildMenuUpdate(arena->basicTowerBuildMenu, terminal, arena, deltaTime);
        buildMenuUpdate(arena->longTowerBuildMenu, terminal, arena, deltaTime);
    }

    if (arena->HeroHP <= 0 && arena->outcome == ARENA_OUTCOME_PLAYING) {
        arena->HeroHP = 0;
        arena->outcome = ARENA_OUTCOME_LOSE;
    }

    healthBarSetHealth(arena->healthBar, arena->HeroHP);
    healthBarUpdate(arena->healthBar, terminal, arena);
    statsBarUpdate(arena->statsBar, terminal, arena);

    cursorUpdate(getCursor(), terminal, deltaTime);
}

ArenaOutcome arenaGetOutcome(Arena* arena) {
    return arena->outcome;
}

bool arenaIsGameOver(Arena* arena, int* health) {
    if (health != NULL) *health = arena->HeroHP;
    return arena->outcome != ARENA_OUTCOME_PLAYING;
}

void arenaSpawnBoss(Arena* arena) {
    if (!arena || !arena->pathList || vectorGetSize(arena->pathList) == 0) return;
    if (arena->boss_spawned) return;
    Path* path = *(Path**)vectorGet(arena->pathList, 0);
    MonsterGen* mg = monsterGenNew(path, bossMonsterCreator, singleSpawnGenTimeFunc);
    vectorPushBack(arena->monsterGenList, (char*)&mg);
    arena->boss_spawned = true;
    arena->boss_gen = mg;
}
