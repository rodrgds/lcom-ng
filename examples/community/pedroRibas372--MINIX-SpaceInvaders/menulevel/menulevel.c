#include "menulevel.h"
#include "../controllers/graphicsCard/graphics.h"
#include "../controllers/keyboard/keyboard.h"
#include "../controllers/keyboard/i8042.h"
#include "../xpm/chooselevel.xpm"
#include "../xpm/level1.xpm"
#include "../xpm/level2.xpm"
#include "../xpm/level3.xpm"
#include "../xpm/exitbuttons.xpm"     
#include "../global.h"
#include "../sprites/sprites.h"
#include "../menu/menu.h" 
#include <lcom/lcf.h>


LevelSelection selected_level = LEVEL1;

Sprite *chooseLevelTitle = NULL;
Sprite *level1Button = NULL;
Sprite *level2Button = NULL;
Sprite *level3Button = NULL;
Sprite *exitLevelButton = NULL; 
LevelSelection current_level = LEVEL1;

void update_level_button_states() {
    switch (current_level) {
        case LEVEL1:
            level1Button->map = xpm_load(drawLevel1pressed_xpm, XPM_INDEXED, &level1Button->img);
            level2Button->map = xpm_load(drawLevel2_xpm, XPM_INDEXED, &level2Button->img);
            level3Button->map = xpm_load(drawLevel3_xpm, XPM_INDEXED, &level3Button->img);
            exitLevelButton->map = xpm_load(drawExit_xpm, XPM_INDEXED, &exitLevelButton->img);
            break;
        case LEVEL2:
            level1Button->map = xpm_load(drawLevel1_xpm, XPM_INDEXED, &level1Button->img);
            level2Button->map = xpm_load(drawLevel2pressed_xpm, XPM_INDEXED, &level2Button->img);
            level3Button->map = xpm_load(drawLevel3_xpm, XPM_INDEXED, &level3Button->img);
            exitLevelButton->map = xpm_load(drawExit_xpm, XPM_INDEXED, &exitLevelButton->img);
            break;
        case LEVEL3:
            level1Button->map = xpm_load(drawLevel1_xpm, XPM_INDEXED, &level1Button->img);
            level2Button->map = xpm_load(drawLevel2_xpm, XPM_INDEXED, &level2Button->img);
            level3Button->map = xpm_load(drawLevel3pressed_xpm, XPM_INDEXED, &level3Button->img);
            exitLevelButton->map = xpm_load(drawExit_xpm, XPM_INDEXED, &exitLevelButton->img);
            break;
        case LEVEL_EXIT:
            level1Button->map = xpm_load(drawLevel1_xpm, XPM_INDEXED, &level1Button->img);
            level2Button->map = xpm_load(drawLevel2_xpm, XPM_INDEXED, &level2Button->img);
            level3Button->map = xpm_load(drawLevel3_xpm, XPM_INDEXED, &level3Button->img);
            exitLevelButton->map = xpm_load(drawExitpressed_xpm, XPM_INDEXED, &exitLevelButton->img);
            break;
        default:
            break;
    }
}

int draw_level_menu() {
    if (draw_xpm(chooseLevelTitle->map, chooseLevelTitle->x, chooseLevelTitle->y, chooseLevelTitle->img) != OK) return 1;
    if (draw_xpm(level1Button->map, level1Button->x, level1Button->y, level1Button->img) != OK) return 1;
    if (draw_xpm(level2Button->map, level2Button->x, level2Button->y, level2Button->img) != OK) return 1;
    if (draw_xpm(level3Button->map, level3Button->x, level3Button->y, level3Button->img) != OK) return 1;
    if (draw_xpm(exitLevelButton->map, exitLevelButton->x, exitLevelButton->y, exitLevelButton->img) != OK) return 1;
    update_screen();
    return 0;
}

int level_menu_loop() {
    bool inside = true;
    chooseLevelTitle = create_static(drawChooseLevel_xpm, 180, 100);
    level1Button = create_static(drawLevel1pressed_xpm, 320, 300);
    level2Button = create_static(drawLevel2_xpm, 320, 400);
    level3Button = create_static(drawLevel3_xpm, 320, 500);
    exitLevelButton = create_static(drawExit_xpm, 320, 600);
    if (!chooseLevelTitle || !level1Button || !level2Button || !level3Button || !exitLevelButton) return 1;

    current_level = LEVEL1;
    update_level_button_states();
    draw_level_menu();

    int ipc_status, r;
    message msg;
    uint8_t bit_no_kbc;
    uint32_t irq_set_kbc;
    bool make;

    if (kbd_subscribe_int(&bit_no_kbc) != OK) return 1;
    irq_set_kbc = BIT(bit_no_kbc);

    while (inside) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK) continue;

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_set_kbc) {
                        kbc_ih();
                        uint8_t scancode = latest_scancode[latest_scancode[0] == TWO_BYTES ? 1 : 0];
                        make = !(scancode & MAKE_CODE);

                        if (done && make) {
                            switch (scancode) {
                                case ENTER_MAKECODE:
                                    memset(double_buffer, 0, get_frame_size());
                                    update_screen();

                                    if (current_level == LEVEL_EXIT) {
                                        current_button = MENU_START;
                                        state = MENU_MODE;
                                    }
                                    else {
                                        selected_level = current_level;
                                        current_button = MENU_START;
                                        state = MENU_MODE;
                                    }
                                    inside = false;
                                    break;

                                case ESCAPE_MAKECODE:
                                    memset(double_buffer, 0, get_frame_size());
                                    update_screen();

                                    current_button = MENU_START;
                                    inside = false;
                                    state = MENU_MODE;
                                    break;

                                case SCANCODE_DOWN:
                                    current_level = (current_level + 1) % LEVEL_COUNT;
                                    update_level_button_states();
                                    draw_level_menu();
                                    break;

                                case SCANCODE_UP:
                                    current_level = (current_level + LEVEL_COUNT - 1) % LEVEL_COUNT;
                                    update_level_button_states();
                                    draw_level_menu();
                                    break;
                            }
                        }
                    }
                    break;
                default: break;
            }
        }
    }

    if (kbd_unsubscribe_int() != OK) return 1;

    return 0;
}
