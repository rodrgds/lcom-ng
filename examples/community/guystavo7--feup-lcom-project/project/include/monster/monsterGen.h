#pragma once

/** @file monsterGen.h
 *  @brief Gerador de monstros ao longo do tempo: fila, timers e funcoes de pacing por onda.
 */

#include "monster.h"
#include "pawnMonster.h"
#include "alienMonster.h"
#include "../utils/timer.h"
#include "spearManMonster.h"
#include "bossMonster.h"

typedef struct monsterGen MonsterGen;

/** @brief Cria monstro "default" no path (tipo generico de teste).
 *  @param[in] path Path partilhado.
 *  @return Monster alocado.
 */
Monster* defaultMonsterCreator(Path* path);

/** @brief Fabrica pawn.
 *  @param[in] path Path.
 *  @return Novo monstro pawn.
 */
Monster* pawnMonsterCreator(Path* path);

/** @brief Fabrica alien.
 *  @param[in] path Path.
 *  @return Novo alien.
 */
Monster* alienMonsterCreator(Path* path);

/** @brief Fabrica spear man / lancer.
 *  @param[in] path Path.
 *  @return Novo monstro.
 */
Monster* spearManMonsterCreator(Path* path);

/** @brief Fabrica boss.
 *  @param[in] path Path.
 *  @return Novo boss.
 */
Monster* bossMonsterCreator(Path* path);

/** @brief Politica de spawn: exemplo progressivo ate 50 unidades no timer.
 *  @param[in] totalTimeElapsed Tempo total desde inicio do gerador.
 *  @param[in] genTimer Timer interno do gerador.
 *  @return Numero de monstros a libertar neste passo (convencao do jogo).
 */
int progressiveTimeFunction50(float totalTimeElapsed, Timer* genTimer);

/** @brief Variante de pacing 100 (parte 1).
 *  @param[in] totalTimeElapsed Tempo acumulado.
 *  @param[in] genTimer Timer do gerador.
 *  @return Quantidade ou codigo de spawn.
 */
int progressiveTimeFunction100_1(float totalTimeElapsed, Timer* genTimer);

/** @brief Variante de pacing 100 (parte 2).
 *  @param[in] totalTimeElapsed Tempo acumulado.
 *  @param[in] genTimer Timer do gerador.
 *  @return Quantidade ou codigo de spawn.
 */
int progressiveTimeFunction100_2(float totalTimeElapsed, Timer* genTimer);

/** @brief Variante de pacing 200 (parte 1).
 *  @param[in] totalTimeElapsed Tempo acumulado.
 *  @param[in] genTimer Timer do gerador.
 *  @return Quantidade ou codigo de spawn.
 */
int progressiveTimeFunction200_1(float totalTimeElapsed, Timer* genTimer);

/** @brief Variante de pacing 200 (parte 2).
 *  @param[in] totalTimeElapsed Tempo acumulado.
 *  @param[in] genTimer Timer do gerador.
 *  @return Quantidade ou codigo de spawn.
 */
int progressiveTimeFunction200_2(float totalTimeElapsed, Timer* genTimer);

/** @brief Um unico spawn ao fim de ~450s ou condicao equivalente.
 *  @param[in] totalTimeElapsed Tempo acumulado.
 *  @param[in] genTimer Timer do gerador.
 *  @return Codigo de spawn.
 */
int singleAppear450(float totalTimeElapsed, Timer* genTimer);

/** @brief Funcao de tempo por omissao do gerador.
 *  @param[in] delta Parametro de tempo (delta ou total conforme chamada).
 *  @param[in] timer Timer associado.
 *  @return Decisao de spawn.
 */
int defaultGenTimeFunc(float delta, Timer* timer);

/** @brief Spawn lento constante.
 *  @param[in] totalTimeElapsed Tempo total.
 *  @param[in] genTimer Timer.
 *  @return Decisao de spawn.
 */
int constantSlowGenTimeFunc(float totalTimeElapsed, Timer* genTimer);

/** @brief Spawn lento constante (variante 1).
 *  @param[in] totalTimeElapsed Tempo total.
 *  @param[in] genTimer Timer.
 *  @return Decisao de spawn.
 */
int constantSlowGenTimeFunc_1(float totalTimeElapsed, Timer* genTimer);

/** @brief Spawn lento constante (variante 2).
 *  @param[in] totalTimeElapsed Tempo total.
 *  @param[in] genTimer Timer.
 *  @return Decisao de spawn.
 */
int constantSlowGenTimeFunc_2(float totalTimeElapsed, Timer* genTimer);

/** @brief Ondas sem auto-spawn (jogador ou script dispara).
 *  @param[in] totalTimeElapsed Tempo total.
 *  @param[in] genTimer Timer.
 *  @return Sem spawn automatico (0 ou flag).
 */
int waveNoAutoSpawnGenTimeFunc(float totalTimeElapsed, Timer* genTimer);

/** @brief Um unico spawn automatico na linha de tempo.
 *  @param[in] totalTimeElapsed Tempo total.
 *  @param[in] genTimer Timer.
 *  @return Codigo de spawn unico.
 */
int singleSpawnGenTimeFunc(float totalTimeElapsed, Timer* genTimer);

/** @brief Libertador para usar com vector de monstros (vectorFree callback).
 *  @param[in] m Ponteiro para Monster a libertar.
 */
void freeMonsterList(void* m);

/** @brief Cria gerador associado a um path, fabrica de monstros e funcao de tempo entre spawns.
 *  @param[in] path Path onde nascem os monstros.
 *  @param[in] monsterCreator Funcao que cria cada instancia.
 *  @param[in] genTimeFunc Funcao que decide quando quantos spawnar.
 *  @return MonsterGen alocado.
 */
MonsterGen* monsterGenNew(Path* path, Monster* (*monsterCreator)(Path*), int (*genTimeFunc)(float, Timer*));

/** @brief Liberta gerador e monstros pendentes.
 *  @param[in] mg Gerador.
 */
void monsterGenFree(MonsterGen* mg);

/** @brief Adiciona entrada de spawn a fila interna (conforme implementacao).
 *  @param[in,out] mg Gerador.
 */
void monsterGenAddMonster(MonsterGen* mg);

/** @brief Forca spawn imediato de uma unidade se condicoes o permitirem.
 *  @param[in,out] mg Gerador.
 */
void monsterGenSpawnOne(MonsterGen* mg);

/** @brief Actualiza timers, spawns e actualizacao dos monstros vivos.
 *  @param[in,out] mg Gerador.
 *  @param[in,out] arena Arena.
 *  @param[in] terminal Terminal para desenho.
 *  @param[in] delta_t Delta em segundos.
 */
void monsterGenUpdate(MonsterGen* mg, Arena* arena, Terminal* terminal, float delta_t);

/** @brief Indice do monstro activo na ordem de sortedIndex (ordenacao interna).
 *  @param[in] mg Gerador.
 *  @param[in] sortedIndex Posicao na lista ordenada.
 *  @return Indice no vector bruto de monstros.
 */
int monsterGenGetNextActiveMonsterIndex(MonsterGen* mg, unsigned int sortedIndex);

/** @brief Acesso ao monstro por indice no vector.
 *  @param[in] mg Gerador.
 *  @param[in] index Indice.
 *  @return Ponteiro Monster ou NULL.
 */
Monster* monsterGenGetMonster(MonsterGen* mg, unsigned int index);

/** @brief Quantos monstros ja terminaram o path instantaneamente (metrica de jogo).
 *  @param[in] mg Gerador.
 *  @return Contagem.
 */
unsigned int monsterGenGetNumberOfInstantFinishedMonsters(MonsterGen* mg);

/** @brief Para o gerador de boss: indica se o boss foi derrotado.
 *  @param[in] bossGen Gerador do boss.
 *  @return true se boss morto/vencido.
 */
bool monsterGenBossDefeated(MonsterGen* bossGen);
