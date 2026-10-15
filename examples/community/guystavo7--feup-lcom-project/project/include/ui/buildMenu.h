#ifndef BUILD_MENU
#define BUILD_MENU

/** @file buildMenu.h
 *  @brief Menu radial ou lista para escolher tipo de torre a construir.
 */

#include "../../../lab5/spriteXMP.h"
#include "../../../lab5/terminal.h"
#include "../arena.h"
#include "../buildings/genericTower.h"

typedef struct BuildMenu BuildMenu;

/** @brief Cria menu para um TowerKind (filtra opcoes ou pre-selecciona tipo).
 *  @param[in] type Tipo de torre associado ao menu.
 *  @return BuildMenu alocado.
 */
BuildMenu* buildMenuNew(TowerKind type);

/** @brief Liberta o menu.
 *  @param[in] buildMenu Instancia.
 */
void buildMenuFree(BuildMenu* buildMenu);

/** @brief Input, hover e desenho do menu de construcao.
 *  @param[in,out] buildMenu Menu.
 *  @param[in] terminal Terminal.
 *  @param[in,out] arena Arena (colocacao de torre).
 *  @param[in] deltaTime Delta em segundos.
 */
void buildMenuUpdate(BuildMenu* buildMenu, Terminal* terminal, Arena* arena, float deltaTime);

/** @brief Indica se o jogador confirmou uma escolha (clique valido).
 *  @param[in] buildMenu Menu.
 *  @return true se pressionado/confirmado.
 */
bool buildMenuIsPressed(BuildMenu* buildMenu);

/** @brief Repoe estado do menu apos fechar ou cancelar.
 *  @param[in,out] buildMenu Menu.
 */
void buildMenuReset(BuildMenu* buildMenu);

#endif
