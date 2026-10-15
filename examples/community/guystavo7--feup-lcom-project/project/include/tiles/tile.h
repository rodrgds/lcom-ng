#ifndef TILE
#define TILE

/** @file tile.h
 *  @brief Celula do mapa: sprite, identificador, posicao e colisor opcional.
 */

#include "../../../lab5/spriteXMP.h"
#include "../../../lab5/terminal.h"
#include "../components/boxCollider.h"
#include "../utils/vec2d.h"

#include <stdbool.h>

typedef struct Tile {
    SpriteXPM* sprite;   /**< Imagem XPM do tile. */
    int id;            /**< Identificador logico do tipo de tile. */
    Vec2d pos;         /**< Posicao no mapa. */
    Vec2d size;        /**< Dimensoes do tile. */
    BoxCollider* collider; /**< Colisor para paredes/obstaculos ou NULL. */
} Tile;

/** @brief Cria um tile com sprite XPM bruto, id e geometria.
 *  @param[in] sprite Dados XPM (array de strings).
 *  @param[in] name Identificador numerico do tile.
 *  @param[in] x Posicao X.
 *  @param[in] y Posicao Y.
 *  @param[in] width Largura.
 *  @param[in] height Altura.
 *  @return Tile alocado.
 */
Tile* tileNew(char** sprite, int name, int x, int y, int width, int height);

/** @brief Liberta tile e collider associado.
 *  @param[in] tile Tile.
 */
void tileFree(Tile* tile);

/** @brief Desenha o tile no terminal.
 *  @param[in] tile Tile.
 *  @param[in] terminal Terminal.
 */
void tileUpdate(Tile* tile, Terminal* terminal);

/** @brief Acesso ao collider do tile (pode ser NULL).
 *  @param[in] tile Tile.
 *  @return BoxCollider ou NULL.
 */
BoxCollider* tileBoxCollider(Tile* tile);

#endif
