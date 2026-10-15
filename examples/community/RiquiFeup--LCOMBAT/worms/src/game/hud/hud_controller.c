#include "hud_controller.h"

#include <stddef.h>

#include "hud.h"
#include "../../config.h"
#include "../worm/worm_controller.h"

bool hud_controller_handle_click(GameContext *context, int x, int y) {
    if (context == NULL) return false;

    if (y >= HUD_HOTBAR_Y && y <= HUD_HOTBAR_Y + HUD_HOTBAR_SLOT_H) {
        for (int i = 0; i < HUD_HOTBAR_SLOT_COUNT; i++) {
            int sx = HUD_HOTBAR_X + i * (HUD_HOTBAR_SLOT_W + HUD_HOTBAR_GAP);
            if (x >= sx && x <= sx + HUD_HOTBAR_SLOT_W) {
                Worm *active = game_context_get_active_worm(context);
                if (active != NULL) {
                    WeaponType selected = hud_get_hotbar_weapon(i);
                    if (selected == WEAPON_BAZOOKA) {
                        worm_controller_apply_command(active, WORM_COMMAND_EQUIP_BAZOOKA);
                    } else if (selected == WEAPON_GRENADE) {
                        worm_controller_apply_command(active, WORM_COMMAND_EQUIP_GRENADE);
                    }
                }
                return true;
            }
        }
    }

    return false;
}
