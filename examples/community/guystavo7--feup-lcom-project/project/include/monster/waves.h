#pragma once

/** @file waves.h
 *  @brief Configuracao de ondas por nivel: duracao, composicao de tipos e boss.
 */

#include <stdbool.h>

/** @brief Contagem por tipo de monstro numa onda. */
typedef struct {
    unsigned pawn;   /**< Numero de pawns. */
    unsigned alien;  /**< Numero de aliens. */
    unsigned spear;  /**< Numero de lancers/spear. */
} WaveComposition;

/** @brief Duracao total das ondas normalis para o nivel (segundos).
 *  @param[in] level Indice do nivel (1-based ou 0-based conforme jogo).
 *  @return Segundos configurados.
 */
unsigned waves_match_duration_sec(int level);

/** @brief Quantos paths activos o nivel usa (paralelismo de spawns).
 *  @param[in] level Nivel.
 *  @return Numero de paths.
 */
unsigned waves_paths_for_level(int level);

/** @brief Numero de geradores de monstros por path.
 *  @return Quantidade fixa ou por configuracao global.
 */
unsigned waves_monster_gens_per_path(void);

/** @brief Quantas ondas "normais" existem antes do boss ou fim.
 *  @return Contagem de ondas.
 */
unsigned waves_normal_wave_count(void);

/** @brief Indica se o nivel inclui fase de boss.
 *  @param[in] level Nivel.
 *  @return true se ha boss.
 */
bool waves_level_has_boss(int level);

/** @brief Preenche out com a composicao da onda wave_index do nivel.
 *  @param[in] level Nivel.
 *  @param[in] wave_index Indice da onda (0 .. n-1).
 *  @param[out] out Estrutura preenchida.
 */
void waves_get_composition(int level, unsigned wave_index, WaveComposition* out);
