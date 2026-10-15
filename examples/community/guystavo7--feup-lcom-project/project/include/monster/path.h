#pragma once

/** @file path.h
 *  @brief Polyline partilhada: subscritores com indice de segmento proprio; direccao ao proximo ponto.
 */

#include "../data_structures/vector.h"
#include "./../utils/vec2d.h"

typedef struct path Path;

/** @brief Cria path a partir de vector de floats (x,y) em pares; data_size do vector deve ser 4 bytes.
 *  @param[in] point_change_distance Distancia minima ao ponto para avancar (> 0).
 *  @param[in] points Vector com numero par de floats (>=2) representando vertices.
 *  @return Path alocado ou NULL em erro.
 */
Path* pathNew(float point_change_distance, Vector* points);

/** @brief Liberta o path e recursos associados.
 *  @param[in] p Path.
 */
void pathFree(Path* p);

/** @brief Regista um subscritor (ponteiro opaco) que seguira este path com estado proprio.
 *  @param[in,out] p Path.
 *  @param[in] pointer Identificador do seguidor (chave interna).
 */
void pathSubscribe(Path* p, void* pointer);

/** @brief Avanca o subscritor para o proximo waypoint.
 *  @param[in,out] p Path.
 *  @param[in] pointer Seguidor.
 */
void pathGoToNextPoint(Path* p, void* pointer);

/** @brief Escreve o waypoint corrente do subscritor em curPoint (estrutura ja inicializada).
 *  @param[in] p Path.
 *  @param[in] pointer Seguidor.
 *  @param[out] curPoint Posicao do ponto actual.
 */
void pathGetCurPoint(Path* p, void* pointer, Vec2d* curPoint);

/** @brief Verdadeiro se pos esta dentro da margem point_change_distance do ponto corrente.
 *  @param[in] p Path.
 *  @param[in] pointer Seguidor.
 *  @param[in] pos Posicao a testar.
 *  @return true se suficientemente proximo para mudar de ponto.
 */
bool pathCheckInPointChangeDistance(Path* p, void* pointer, Vec2d* pos);

/** @brief Calcula vector direccao normalizado do seguidor em pos em direccao ao ponto corrente.
 *  @param[in] p Path.
 *  @param[in] pointer Seguidor.
 *  @param[in] pos Posicao actual do monstro.
 *  @param[out] new_dir Direccao resultante (pre-inicializada).
 */
void pathGetDirectionToCurPoint(Path* p, void* pointer, Vec2d* pos, Vec2d* new_dir);

/** @brief Actualiza pos e new_dir seguindo o path (integracao ao longo do segmento).
 *  @param[in,out] p Path.
 *  @param[in] pointer Seguidor.
 *  @param[in,out] pos Posicao actualizada.
 *  @param[out] new_dir Nova direccao de movimento.
 */
void pathFollowingDir(Path* p, void* pointer, Vec2d* pos, Vec2d* new_dir);

/** @brief Numero de vertices do path.
 *  @param[in] path Path.
 *  @return Contagem de pontos.
 */
unsigned int pathGetPointsSize(Path* path);

/** @brief Copia o vertice i para point.
 *  @param[in] p Path.
 *  @param[in] i Indice do vertice.
 *  @param[out] point Saida (x,y).
 */
void pathGetPoint(Path* p, unsigned int i, Vec2d* point);

/** @brief Alias/contagem de pontos (conforme implementacao).
 *  @param[in] path Path.
 *  @return Numero de pontos.
 */
unsigned int pathGetNumOfPoints(Path* path);

/** @brief Indice do waypoint corrente para o subscritor.
 *  @param[in] p Path.
 *  @param[in] pointer Seguidor.
 *  @return Indice actual.
 */
unsigned int pathGetCurPointIndex(Path* p, void* pointer);

/** @brief Forca o indice do waypoint do subscritor (ex.: reset ao spawn).
 *  @param[in,out] p Path.
 *  @param[in] pointer Seguidor.
 *  @param[in] index Novo indice.
 */
void pathSetCurPointIndex(Path* p, void* pointer, unsigned int index);
