#pragma once

/** @file monsterView.h
 *  @brief Desenho do monstro: construcao de animacao e update por frame no terminal.
 */

#include "monsterModel.h"
#include "../components/animatedSprite.h"
#include "../../lab5/terminal.h"

typedef struct monsterView MonsterView;

/** @brief Cria vista associada ao modelo; animationBuilder gera o AnimatedSprite inicial.
 *  @param[in] model Modelo do monstro.
 *  @param[in] animationBuilder Callback que devolve sprite animado.
 *  @return Vista alocada.
 */
MonsterView* monsterViewNew(MonsterModel* model, AnimatedSprite* (*animationBuilder)());

/** @brief Liberta a vista (sprites conforme implementacao).
 *  @param[in] view Vista.
 */
void monsterViewFree(MonsterView* view);

/** @brief Actualiza animacao e desenha no terminal.
 *  @param[in,out] view Vista.
 *  @param[in] terminal Terminal.
 *  @param[in] delta_t Delta em segundos.
 */
void monsterViewUpdate(MonsterView* view, Terminal* terminal, float delta_t);
