#ifndef __SLASH_COMBO_H
#define __SLASH_COMBO_H

#include <stdint.h>

/**
 * @file slash_combo.h
 * @brief Declares quick-slash combo scoring and popup helpers.
 */

/** @brief Minimum number of quick cuts required to trigger a combo bonus. */
#define SLASH_COMBO_MIN_CUTS 3

/** @brief Maximum ticks between cuts that still count toward one combo. */
#define SLASH_COMBO_WINDOW_TICKS 25

/** @brief Duration of the combo popup, in gameplay ticks. */
#define SLASH_COMBO_POPUP_TICKS 60

/**
 * @brief Runtime state for one quick-slash combo burst.
 *
 * The active combo is resolved once after its time window closes. For example,
 * four one-point cuts inside the window award four base points immediately and
 * one four-point bonus when the combo finishes.
 */
typedef struct {
  /** @brief Number of cuts currently chained inside the combo window. */
  uint8_t count;

  /** @brief Last gameplay tick in which the combo chain received a cut. */
  uint32_t last_cut_tick;

  /** @brief Sum of base points earned by the current combo chain. */
  uint32_t base_points;

  /** @brief Horizontal impact coordinate for the current combo chain. */
  int impact_x;

  /** @brief Vertical impact coordinate for the current combo chain. */
  int impact_y;

  /** @brief Remaining ticks for drawing the combo popup. */
  uint8_t popup_ticks;

  /** @brief Cut count shown in the combo popup. */
  uint8_t popup_count;

  /** @brief Horizontal impact coordinate used to place the combo popup. */
  int popup_x;

  /** @brief Vertical impact coordinate used to place the combo popup. */
  int popup_y;
} slash_combo_t;

/**
 * @brief Reset both the active combo and visible popup state.
 *
 * @param combo Combo state to reset.
 */
void slash_combo_reset(slash_combo_t *combo);

/**
 * @brief Register one cut into the active combo burst.
 *
 * If a previous burst expired before this cut, that burst is resolved first and
 * its one-shot bonus is returned.
 *
 * @param combo Combo state to mutate.
 * @param current_tick Current gameplay/session tick.
 * @param score_value Base score awarded by the cut.
 * @param impact_x Horizontal impact coordinate for popup placement.
 * @param impact_y Vertical impact coordinate for popup placement.
 * @return Bonus points awarded by any burst resolved before this cut.
 */
uint32_t slash_combo_register_cut(slash_combo_t *combo,
                                  uint32_t current_tick,
                                  uint32_t score_value,
                                  int impact_x,
                                  int impact_y);

/**
 * @brief Advance combo timing and resolve expired bursts.
 *
 * @param combo Combo state to mutate.
 * @param current_tick Current gameplay/session tick.
 * @return Bonus points awarded when an expired combo is resolved.
 */
uint32_t slash_combo_update(slash_combo_t *combo, uint32_t current_tick);

#endif /* __SLASH_COMBO_H */
