#ifndef ARENA
#define ARENA

/** @file arena.h
 *  @brief Arena de jogo: mapa, entidades, colisoes, torres, ondas e condicao de vitoria/derrota.
 */

#include "./components/boxCollider.h"
#include "./components/projectile.h"
#include "../../lab5/terminal.h"
#include "data_structures/vector.h"
#include "utils/vec2d.h"

#include <stdbool.h>

typedef struct Arena Arena;

/** @brief Resultado da partida na arena. */
typedef enum {
    ARENA_OUTCOME_PLAYING = 0, /**< Partida a decorrer. */
    ARENA_OUTCOME_WIN,         /**< Jogador venceu. */
    ARENA_OUTCOME_LOSE,        /**< Jogador perdeu. */
} ArenaOutcome;

 
/** @brief Constroi uma arena a partir de ficheiros de mapa e listas de caminhos/spawns.
 *  @param[in] terminal Contexto grafico para desenho.
 *  @param[in] tilesFile CSV ou ficheiro de paredes/chao.
 *  @param[in] pathFile Ficheiro do grafo/caminho para monstros.
 *  @param[in] waterFile Ficheiro de agua/lagos.
 *  @param[in] background XPM de fundo (array de strings XPM).
 *  @param[in] paths Vector de dados de caminho (formato interno).
 *  @param[in] gens Vector de pontos de spawn.
 *  @param[in] bananaStart Saldo inicial de bananas (moeda).
 *  @param[in] level Indice/nivel da fase.
 *  @return Ponteiro para Arena alocada ou NULL em erro.
 */
Arena* arenaNew(Terminal* terminal, const char* tilesFile, const char* pathFile, const char* waterFile, char** background, Vector* paths, Vector* gens, int bananaStart, int level);

/** @brief Liberta toda a memoria associada a arena.
 *  @param[in] arena Instancia a libertar.
 */
 
void arenaFree(Arena* arena);

/** @brief Detecta colisoes do collider box com etiqueta tag contra outros colliders na arena.
 *  @param[in] arena Arena activa.
 *  @param[in] box Caixa a testar.
 *  @param[in] tag Filtro de etiqueta (ex.: PLAYER_TAG).
 *  @param[out] colliders Array pre-alocado onde se escrevem ponteiros para os que colidiram.
 *  @return Numero de colisoes encontradas (quantos slots de colliders foram preenchidos).
 */
int arenaDetectCollisions(Arena* arena, BoxCollider* box, int tag, BoxCollider** colliders);

/** @brief Encontra o primeiro inimigo cuja caixa intersecta a area range.
 *  @param[in] arena Arena.
 *  @param[in] range Regiao de procura (ex.: alcance de torre).
 *  @return Collider do inimigo ou NULL se nenhum.
 */
BoxCollider* arenaFindFirstEnemy(Arena* arena, BoxCollider* range);

/** @brief Passo de simulacao: monstros, torres, projéteis, UI de jogo, frutos, etc.
 *  @param[in,out] arena Estado da arena.
 *  @param[in] terminal Terminal para desenho.
 *  @param[in] deltaTime Delta em segundos.
 */
void arenaUpdate(Arena* arena, Terminal* terminal, float deltaTime);

/** @brief Tenta construir uma torre na celula/tile correspondente a position.
 *  @param[in,out] arena Arena.
 *  @param[in] position Posicao em coordenadas de mundo ou grelha (conforme implementacao).
 */
void arenaBuildTower(Arena* arena, Vec2d position);

/** @brief Indica se e permitido colocar torre na posicao (sem sobrepor paths invalidos, etc.).
 *  @param[in] arena Arena.
 *  @param[in] position Posicao candidata.
 *  @return true se o jogador pode construir ai.
 */
bool arenaCanPlaceTower(Arena* arena, Vec2d position);

/** @brief Trata morte de monstro na posicao (drop de fruto, efeitos).
 *  @param[in,out] arena Arena.
 *  @param[in] position Ponto onde o monstro morreu.
 */
void arenaDeathFruit(Arena* arena, Vec2d position);

/** @brief Estado win/lose/playing agregado.
 *  @param[in] arena Arena.
 *  @return Valor ArenaOutcome.
 */
ArenaOutcome arenaGetOutcome(Arena* arena);

/** @brief Consulta se a partida terminou por vida a zero (ou equivalente).
 *  @param[in] arena Arena.
 *  @param[out] health Se nao NULL, recebe vida restante.
 *  @return true se game over.
 */
bool arenaIsGameOver(Arena* arena, int* health);

/** @brief Dispara spawn do boss (fim de ondas ou evento especial).
 *  @param[in,out] arena Arena.
 */
void arenaSpawnBoss(Arena* arena);

#endif
