#ifndef HEALTH_BAR
#define HEALTH_BAR

/** @file healthBar.h
 *  @brief Barra de vida do jogador na HUD.
 */

#include "../../../lab5/spriteXMP.h"
#include "../../../lab5/terminal.h"
#include "../arena.h"

typedef struct HealthBar HealthBar;

/** @brief Cria barra com vida inicial.
 *  @param[in] health Vida inicial a mostrar.
 *  @return HealthBar alocada.
 */
HealthBar* healthBarNew(int health);

/** @brief Liberta a barra de vida.
 *  @param[in] healthBar Instancia.
 */
void healthBarFree(HealthBar* healthBar);

/** @brief Actualiza desenho e valor a partir da arena.
 *  @param[in,out] healthBar Barra.
 *  @param[in] terminal Terminal.
 *  @param[in,out] arena Arena (fonte da vida).
 */
void healthBarUpdate(HealthBar* healthBar, Terminal* terminal, Arena* arena);

/** @brief Vida actual mostrada.
 *  @param[in] healthBar Barra.
 *  @return HP.
 */
int healthBarGetHealth(HealthBar* healthBar);

/** @brief Define vida explicitamente.
 *  @param[in,out] healthBar Barra.
 *  @param[in] health Novo valor.
 */
void healthBarSetHealth(HealthBar* healthBar, int health);

/** @brief Soma delta a vida (cura ou dano visual).
 *  @param[in,out] healthBar Barra.
 *  @param[in] health Variacao.
 */
void healthBarAddHealth(HealthBar* healthBar, int health);

#endif
