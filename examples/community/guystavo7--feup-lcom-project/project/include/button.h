#ifndef BUTTON
#define BUTTON

/** @file button.h
 *  @brief Botao UI com sprite animado, hitbox e callbacks de clique/hover.
 */

#include "../../lab5/spriteXMP.h"
#include "../../lab5/terminal.h"
#include "components/boxCollider.h"
#include "components/animatedSprite.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct Button Button;

/** @brief Callback quando o botao e clicado. */
typedef void (*ButtonClickFn)(Button* self);
/** @brief Callback quando o rato entra/sai do hover (conforme implementacao). */
typedef void (*ButtonHoverFn)(Button* self);

/** @brief Cria um botao com area, sprite e callbacks.
 *  @param[in] x Posicao X do canto superior esquerdo (pixels).
 *  @param[in] y Posicao Y.
 *  @param[in] width Largura da area clicavel.
 *  @param[in] height Altura da area clicavel.
 *  @param[in] sprite Animacao associada ao botao.
 *  @param[in] onClick Funcao de clique ou NULL.
 *  @param[in] onHover Funcao de hover ou NULL.
 *  @return Ponteiro para Button alocado.
 */
Button* buttonNew(int x, int y, int width, int height, AnimatedSprite* sprite, ButtonClickFn onClick, ButtonHoverFn onHover);

/** @brief Liberta o botao (e recursos que possua exclusivamente).
 *  @param[in] b Botao.
 */
void buttonFree(Button* b);

/** @brief Desenha o botao e actualiza animacao.
 *  @param[in] b Botao.
 *  @param[in] t Terminal.
 *  @param[in] deltaTime Delta em segundos.
 *  @param[in] isFlipped Espelhar sprite horizontalmente se true.
 */
void buttonDraw(Button* b, Terminal* t, float deltaTime, bool isFlipped);

/** @brief Notifica clique (chama onClick se definido).
 *  @param[in,out] button Botao.
 */
void buttonClick(Button* button);

/** @brief Notifica hover.
 *  @param[in,out] button Botao.
 */
void buttonHover(Button* button);

/** @brief Repoe estado visual/logico do botao (ex.: soltar pressao).
 *  @param[in,out] button Botao.
 */
void buttonReset(Button* button);

/** @brief Acesso ao collider do botao (colisoes com rato).
 *  @param[in] button Botao.
 *  @return Ponteiro para BoxCollider interno.
 */
BoxCollider* buttonGetBoxCollider(Button* button);

/** @brief Implementacao por omissao de hover (pode animar ou mudar estado).
 *  @param[in,out] button Botao.
 */
void basicButtonHoverFn(Button* button);

/** @brief Indica se a animacao one-shot do botao terminou.
 *  @param[in] button Botao.
 *  @return true se animacao concluida.
 */
bool buttonIsFinished(Button* button);

/** @brief Testa se o ponto (px,py) esta dentro da area do botao.
 *  @param[in] b Botao.
 *  @param[in] px Coordenada x do rato.
 *  @param[in] py Coordenada y do rato.
 *  @return true se dentro.
 */
bool buttonContainsPoint(Button* b, int px, int py);

/** @brief Indica se o botao esta visualmente/logicamente pressionado.
 *  @param[in] b Botao.
 *  @return true se pressionado.
 */
bool buttonIsPressed(Button* b);

#endif
