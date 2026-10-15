#ifndef BOXCOLLIDER
#define BOXCOLLIDER

/** @file boxCollider.h
 *  @brief Caixa alinhada aos eixos (AABB) com etiqueta para colisoes por camadas.
 */

#include "../utils/vec2d.h"
#include <stdbool.h>

typedef struct BoxCollider BoxCollider;

/** @brief Cria uma caixa de colisao com posicao, offset interno, tamanho e etiqueta.
 *  @param[in] pos Canto ou origem logica da caixa (conforme implementacao).
 *  @param[in] offset Deslocamento do sprite para o rectangulo de hit.
 *  @param[in] size Largura e altura.
 *  @param[in] tag Identificador de camada (PLAYER_TAG, MONSTER_TAG, ...).
 *  @return Nova instancia alocada.
 */
BoxCollider* boxColliderNew(Vec2d pos, Vec2d offset, Vec2d size, int tag);

/** @brief Liberta a caixa.
 *  @param[in] boxCollider Instancia.
 */
void boxColliderFree(BoxCollider* boxCollider);

/** @brief Interseccao AABB entre duas caixas.
 *  @param[in] b1 Primeira caixa.
 *  @param[in] b2 Segunda caixa.
 *  @return true se sobrepostas.
 */
bool boxIsCollidingWithBox(BoxCollider* b1, BoxCollider* b2);

/** @brief Testa se um ponto esta dentro da caixa.
 *  @param[in] b Caixa.
 *  @param[in] pos Ponto em coordenadas de mundo.
 *  @return true se interior ou fronteira.
 */
bool boxIsCollidingWithPoint(BoxCollider* b, Vec2d pos);

/** @brief Etiqueta de colisao.
 *  @param[in] b Caixa.
 *  @return Valor tag.
 */
int boxColliderGetTag(BoxCollider* b);

/** @brief Largura da caixa.
 *  @param[in] b Caixa.
 *  @return Largura em pixels/unidades.
 */
float boxColliderGetWidth(BoxCollider* b);

/** @brief Altura da caixa.
 *  @param[in] b Caixa.
 *  @return Altura.
 */
float boxColliderGetHeight(BoxCollider* b);

/** @brief Define a coordenada X da origem da caixa.
 *  @param[in,out] b Caixa.
 *  @param[in] x Nova X inteira.
 */
void boxColliderSetX(BoxCollider* b, int x);

/** @brief Define a coordenada Y da origem da caixa.
 *  @param[in,out] b Caixa.
 *  @param[in] y Nova Y inteira.
 */
void boxColliderSetY(BoxCollider* b, int y);

/** @brief Define posicao completa.
 *  @param[in,out] b Caixa.
 *  @param[in] pos Novo Vec2d de posicao.
 */
void boxColliderSetPos(BoxCollider* b, Vec2d pos);

/** @brief Offset X do hitbox relativamente ao sprite.
 *  @param[in] b Caixa.
 *  @return Offset em X.
 */
float boxColliderGetOffsetX(BoxCollider* b);

/** @brief Offset Y do hitbox.
 *  @param[in] b Caixa.
 *  @return Offset em Y.
 */
float boxColliderGetOffsetY(BoxCollider* b);

/** @brief Posicao X efectiva (origem + offset, conforme implementacao).
 *  @param[in] b Caixa.
 *  @return Coordenada X.
 */
float boxColliderGetX(BoxCollider* b);

/** @brief Posicao Y efectiva.
 *  @param[in] b Caixa.
 *  @return Coordenada Y.
 */
float boxColliderGetY(BoxCollider* b);

#endif
