#ifndef ANIMATED_SPRITE
#define ANIMATED_SPRITE

/** @file animatedSprite.h
 *  @brief Sequencia de frames XPM com tipos loop, one-shot ou manual.
 */

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>

#include "../../../lab5/spriteXMP.h"
#include "../../../lab5/terminal.h"

typedef struct AnimatedSprite AnimatedSprite;

/** @brief Modo de reproducao da animacao. */
enum AnimatedSpriteType {
	LOOP,     /**< Repete indefinidamente; sempre visivel. */
	ONE_SHOT, /**< Corre uma vez; visivel apenas durante animacao. */
	NORMAL    /**< Visivel; anima quando explicitamente iniciada. */
};

/** @brief Cria sprite animado a partir de um array de XPM.
 *  @param[in] frameSpeed Velocidade de troca de frames (frames/s ou factor interno).
 *  @param[in] xpm_array Array de ponteiros SpriteXPM* (frames).
 *  @param[in] n Numero de frames.
 *  @param[in] type Modo LOOP / ONE_SHOT / NORMAL.
 *  @return Instancia alocada.
 */
AnimatedSprite* animatedSpriteNew(float frameSpeed, SpriteXPM** xpm_array, int n, enum AnimatedSpriteType type);

/** @brief Liberta a estrutura e os XPM associados (conforme implementacao).
 *  @param[in] sprite Animacao.
 */
void animatedSpriteFree(AnimatedSprite* sprite);

/** @brief Liberta apenas o array de sprites, mantendo o AnimatedSprite util para troca de recursos.
 *  @param[in] sprite Animacao.
 */
void animatedSpriteFreeSpritesArrayOnly(AnimatedSprite* sprite);

/** @brief Actualiza e desenha o frame corrente no terminal.
 *  @param[in] x Posicao X de desenho.
 *  @param[in] y Posicao Y de desenho.
 *  @param[in,out] sprite Animacao.
 *  @param[in] t Terminal.
 *  @param[in] deltaTime Delta em segundos.
 *  @param[in] isFlipped Espelhar horizontalmente.
 */
void animatedSpriteUpdate(int x, int y, AnimatedSprite* sprite, Terminal* t, float deltaTime, bool isFlipped);

/** @brief Substitui o frame no indice dado.
 *  @param[in,out] sprite Animacao.
 *  @param[in] index Indice do frame.
 *  @param[in] xpm Novo sprite XPM.
 */
void animatedSpriteSetSpriteAtIndex(AnimatedSprite* sprite, int index, SpriteXPM* xpm);

/** @brief Inicia ou retoma a animacao.
 *  @param[in,out] sprite Animacao.
 */
void animatedSpriteStart(AnimatedSprite* sprite);

/** @brief Para animacoes ONE_SHOT, indica se ja terminou.
 *  @param[in] sprite Animacao.
 *  @return true se concluiu.
 */
bool animatedSpriteIsFinished(AnimatedSprite* sprite);

/** @brief Volta ao primeiro frame e estado inicial.
 *  @param[in,out] sprite Animacao.
 */
void animatedSpriteReset(AnimatedSprite* sprite);

/** @brief Largura do frame corrente.
 *  @param[in] sprite Animacao.
 *  @return Largura em pixels.
 */
int animatedSpriteGetWidth(AnimatedSprite* sprite);

/** @brief Altura do frame corrente.
 *  @param[in] sprite Animacao.
 *  @return Altura em pixels.
 */
int animatedSpriteGetHeight(AnimatedSprite* sprite);

#endif
