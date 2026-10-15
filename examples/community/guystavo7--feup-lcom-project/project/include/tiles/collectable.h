#ifndef COLLECTABLE
#define COLLECTABLE

/** @file collectable.h
 *  @brief Fruto/recurso apanhavel no mapa com hitbox e XPM.
 */

#include "../utils/vec2d.h"
#include "../components/boxCollider.h"
#include "../../../lab5/spriteXMP.h"
#include "../../../lab5/terminal.h"
#include <stdbool.h>

typedef struct Collectable Collectable;

/** @brief Cria colecionavel na posicao com offset de hitbox e sprite XPM.
 *  @param[in] position Centro ou origem do objecto.
 *  @param[in] offset Offset da caixa relativamente a position.
 *  @param[in] XPM Dados XPM do sprite.
 *  @return Instancia alocada.
 */
Collectable* collectableNew(Vec2d position, Vec2d offset, char** XPM);

/** @brief Liberta o colecionavel.
 *  @param[in] c Instancia.
 */
void collectableFree(Collectable* c);

/** @brief Activa ou desactiva (escondido / sem colisao util).
 *  @param[in,out] c Colecionavel.
 *  @param[in] isActive true para visivel/activo.
 */
void collectableSetActive(Collectable* c, bool isActive);

/** @brief Estado activo.
 *  @param[in] c Colecionavel.
 *  @return true se activo.
 */
bool collectableIsActive(Collectable* c);

/** @brief Define coordenada X.
 *  @param[in,out] c Colecionavel.
 *  @param[in] x Nova X.
 */
void collectableSetX(Collectable* c, float x);

/** @brief Define coordenada Y.
 *  @param[in,out] c Colecionavel.
 *  @param[in] y Nova Y.
 */
void collectableSetY(Collectable* c, float y);

/** @brief Define posicao completa.
 *  @param[in,out] c Colecionavel.
 *  @param[in] position Novo Vec2d.
 */
void collectableSetPostion(Collectable* c, Vec2d position);

/** @brief Posicao X.
 *  @param[in] c Colecionavel.
 *  @return X.
 */
float collectableGetX(Collectable* c);

/** @brief Posicao Y.
 *  @param[in] c Colecionavel.
 *  @return Y.
 */
float collectableGetY(Collectable* c);

/** @brief Actualiza desenho/animacao.
 *  @param[in,out] c Colecionavel.
 *  @param[in] terminal Terminal.
 */
void collectableUpdate(Collectable* c, Terminal* terminal);

/** @brief Hitbox para colisao com o jogador.
 *  @param[in] c Colecionavel.
 *  @return Ponteiro para BoxCollider.
 */
BoxCollider* collectableGetHitBox(Collectable* c);

#endif
