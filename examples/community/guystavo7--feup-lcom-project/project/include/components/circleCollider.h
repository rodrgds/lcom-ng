#ifndef CIRCLECOLLIDER
#define CIRCLECOLLIDER

/** @file circleCollider.h
 *  @brief Colisor circular (ex.: alcance ou projétil esferico simplificado).
 */

typedef struct CircleCollider CircleCollider;

#include <stdbool.h>

/** @brief Cria um circulo de colisao centrado em (x,y) com raio e etiqueta.
 *  @param[in] x Centro X.
 *  @param[in] y Centro Y.
 *  @param[in] radius Raio em pixels.
 *  @param[in] tag Etiqueta de camada.
 *  @return Instancia alocada.
 */
CircleCollider* circleColliderNew(int x, int y, int radius, int tag);

/** @brief Liberta o colisor.
 *  @param[in] c Instancia.
 */
void circleColliderFree(CircleCollider* c);

/** @brief Ponto dentro do circulo.
 *  @param[in] c Circulo.
 *  @param[in] x Coordenada X do ponto.
 *  @param[in] y Coordenada Y do ponto.
 *  @return true se distancia ao centro <= raio.
 */
bool circleIsCollidingWithPoint(CircleCollider* c, int x, int y);

/** @brief Move o centro do circulo.
 *  @param[in,out] c Circulo.
 *  @param[in] x Novo centro X.
 *  @param[in] y Novo centro Y.
 */
void circleColliderUpdate(CircleCollider* c, int x, int y);

/** @brief Etiqueta do colisor.
 *  @param[in] c Circulo.
 *  @return tag.
 */
int circleColliderGetTag(CircleCollider* c);

#endif
