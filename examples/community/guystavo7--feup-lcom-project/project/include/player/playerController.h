#ifndef PLAYER_CONTROLLER
#define PLAYER_CONTROLLER

/** @file playerController.h
 *  @brief Orquestra actualizacao do jogador (delega ao estado corrente do modelo).
 */

#include "playerModel.h"
#include "../../lab5/terminal.h"
#include "../arena.h"

typedef struct PlayerController PlayerController;

/** @brief Cria controlador associado ao modelo.
 *  @param[in] model Modelo do jogador (nao pode ser NULL).
 *  @return Controlador alocado.
 */
PlayerController* playerControllerNew(PlayerModel* model);

/** @brief Liberta o controlador.
 *  @param[in] controller Instancia.
 */
void playerControllerFree(PlayerController* controller);

/** @brief Actualiza input, transicoes de estado e movimento por frame.
 *  @param[in,out] controller Controlador.
 *  @param[in] terminal Terminal.
 *  @param[in] arena Arena.
 *  @param[in] deltaTime Delta em segundos.
 */
void playerControllerUpdate(PlayerController* controller, Terminal* terminal, Arena* arena, float deltaTime);
 
#endif
