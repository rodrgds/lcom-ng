#include "../include/game/slash_combo.h"

#include <stdbool.h>
#include <string.h>

static void slash_combo_reset_active(slash_combo_t *combo) {
    if (combo == NULL) return;

    combo->count = 0;
    combo->last_cut_tick = 0;
    combo->base_points = 0;
    combo->impact_x = 0;
    combo->impact_y = 0;
}

static uint32_t slash_combo_finish(slash_combo_t *combo) {
    if (combo == NULL) return 0;

    uint32_t bonus = 0;

    if (combo->count >= SLASH_COMBO_MIN_CUTS) {
        bonus = combo->base_points;
        combo->popup_ticks = SLASH_COMBO_POPUP_TICKS;
        combo->popup_count = combo->count;
        combo->popup_x = combo->impact_x;
        combo->popup_y = combo->impact_y;
    }

    slash_combo_reset_active(combo);
    return bonus;
}

static bool slash_combo_expired(const slash_combo_t *combo, uint32_t current_tick) {
    return combo != NULL &&
           combo->count > 0 &&
           current_tick - combo->last_cut_tick > SLASH_COMBO_WINDOW_TICKS;
}

void slash_combo_reset(slash_combo_t *combo) {
    if (combo == NULL) return;

    memset(combo, 0, sizeof(*combo));
}

uint32_t slash_combo_register_cut(slash_combo_t *combo,
                                  uint32_t current_tick,
                                  uint32_t score_value,
                                  int impact_x,
                                  int impact_y) {
    if (combo == NULL) return 0;

    uint32_t bonus = 0;
    if (slash_combo_expired(combo, current_tick)) {
        bonus = slash_combo_finish(combo);
    }

    if (combo->count < UINT8_MAX) {
        combo->count++;
    }

    combo->last_cut_tick = current_tick;
    combo->base_points += score_value;
    combo->impact_x = impact_x;
    combo->impact_y = impact_y;

    return bonus;
}

uint32_t slash_combo_update(slash_combo_t *combo, uint32_t current_tick) {
    if (combo == NULL) return 0;

    if (slash_combo_expired(combo, current_tick)) {
        return slash_combo_finish(combo);
    }

    if (combo->popup_ticks > 0) {
        combo->popup_ticks--;
    }

    return 0;
}
