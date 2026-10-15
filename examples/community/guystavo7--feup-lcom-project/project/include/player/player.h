#ifndef PLAYER_ENTITY_H
#define PLAYER_ENTITY_H

/** @file player.h
 *  @brief Jogador: agrega modelo (dados) e controlador (input/FSM).
 */

#include "./../utils/vec2d.h"
#include "./../arena.h"
#include "playerModel.h"
#include "playerController.h"

#include "../../lab5/terminal.h"

typedef struct Player Player;

/** @brief Cria o jogador na posicao inicial.
 *  @param[in] pos Posicao de spawn.
 *  @return Ponteiro para Player alocado.
 */
Player* playerNew(Vec2d pos);

/** @brief Liberta jogador e sub-componentes.
 *  @param[in] player Instancia.
 */
void playerFree(Player* player);

/** @brief Passo de frame: input, estado, fisica e desenho.
 *  @param[in,out] player Jogador.
 *  @param[in] terminal Terminal.
 *  @param[in] arena Arena para colisoes.
 *  @param[in] deltaTime Delta em segundos.
 */
void playerUpdate(Player* player, Terminal* terminal, Arena* arena, float deltaTime);

/** @brief Acesso ao modelo (sprites, velocidade, hitbox).
 *  @param[in] player Jogador.
 *  @return Ponteiro para PlayerModel.
 */
PlayerModel* playerGetPlayerModel(Player* player);

/** @brief Acesso ao controlador (maquina de estados / logica).
 *  @param[in] player Jogador.
 *  @return Ponteiro para PlayerController.
 */
PlayerController* playerGetPlayerController(Player* player);

#endif
