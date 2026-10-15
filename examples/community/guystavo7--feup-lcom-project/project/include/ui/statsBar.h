#ifndef STATS_BAR
#define STATS_BAR

/** @file statsBar.h
 *  @brief Barra de recursos (banana, abacate, cereja, cenoura): icones e contagens na HUD.
 */

#include "../../../lab5/spriteXMP.h"
#include "../../../lab5/terminal.h"
#include "../arena.h"

typedef struct StatsBar StatsBar;

/** @brief Constroi a barra de estatisticas com sprites da HUD.
 *  @return StatsBar alocada.
 */
StatsBar* statsBarNew();

/** @brief Liberta a barra.
 *  @param[in] statsBar Instancia (parametro legacy healthBar na assinatura original).
 */
void statsBarFree(StatsBar* statsBar);

/** @brief Desenha e sincroniza valores com a arena.
 *  @param[in,out] statsBar Barra.
 *  @param[in] terminal Terminal.
 *  @param[in,out] arena Arena (fonte de moedas/recursos).
 */
void statsBarUpdate(StatsBar* statsBar, Terminal* terminal, Arena* arena);

/** @brief Getter bananas.
 *  @param[in] statsBar Barra.
 *  @return Contagem.
 */
int statsBarGetBanana(StatsBar* statsBar);

/** @brief Getter abacates.
 *  @param[in] statsBar Barra.
 *  @return Contagem.
 */
int statsBarGetAvocado(StatsBar* statsBar);

/** @brief Getter cerejas.
 *  @param[in] statsBar Barra.
 *  @return Contagem.
 */
int statsBarGetCherry(StatsBar* statsBar);

/** @brief Getter cenouras.
 *  @param[in] statsBar Barra.
 *  @return Contagem.
 */
int statsBarGetCarrot(StatsBar* statsBar);

/** @brief Define contagem de bananas.
 *  @param[in,out] statsBar Barra.
 *  @param[in] n Novo valor.
 */
void statsBarSetBanana(StatsBar* statsBar, int n);

/** @brief Define abacates.
 *  @param[in,out] statsBar Barra.
 *  @param[in] n Valor.
 */
void statsBarSetAvocado(StatsBar* statsBar, int n);

/** @brief Define cerejas.
 *  @param[in,out] statsBar Barra.
 *  @param[in] n Valor.
 */
void statsBarSetCherry(StatsBar* statsBar, int n);

/** @brief Define cenouras.
 *  @param[in,out] statsBar Barra.
 *  @param[in] n Valor.
 */
void statsBarSetCarrot(StatsBar* statsBar, int n);

/** @brief Incrementa bananas.
 *  @param[in,out] statsBar Barra.
 *  @param[in] n Delta (pode ser negativo se implementacao permitir).
 */
void statsBarAddBanana(StatsBar* statsBar, int n);

/** @brief Incrementa abacates.
 *  @param[in,out] statsBar Barra.
 *  @param[in] n Delta.
 */
void statsBarAddAvocado(StatsBar* statsBar, int n);

/** @brief Incrementa cerejas.
 *  @param[in,out] statsBar Barra.
 *  @param[in] n Delta.
 */
void statsBarAddCherry(StatsBar* statsBar, int n);

/** @brief Incrementa cenouras.
 *  @param[in,out] statsBar Barra.
 *  @param[in] n Delta.
 */
void statsBarAddCarrot(StatsBar* statsBar, int n);

#endif
