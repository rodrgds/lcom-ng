#include "../../include/ui/menu_draw.h"

#include "../../include/macros/menu.h"
#include "../../include/ui/ui.h"

#define MENU_BUTTON_COUNT 3

typedef struct {
    MenuOption option;
    button_t button;
} MenuButtonSpec;

static button_t make_menu_button(uint16_t x, uint16_t y, const char *label) {
    button_t button;
    button.x = x;
    button.y = y;
    button.width = MENU_BUTTON_W;
    button.height = MENU_BUTTON_H;
    button.color = MENU_BUTTON_COLOR;
    button.hover_color = MENU_BUTTON_HOVER_COLOR;
    button.label = label;
    button.hovered = false;
    return button;
}

static uint16_t button_text_x(const button_t *button, const Font *font) {
    uint16_t width = font_text_width(font, button->label);

    if (width >= button->width) return button->x;
    return button->x + (button->width - width) / 2;
}

static uint16_t button_text_y(const button_t *button, const Font *font) {
    if (font->char_width >= button->height) return button->y;
    return button->y + (button->height - font->char_width) / 2;
}

static int draw_button_with_label(const button_t *button, const Font *font) {
    if (ui_draw_button(button) != 0) return 1;
    return ui_draw_text(font,
                        button->label,
                        button_text_x(button, font),
                        button_text_y(button, font));
}

static MenuButtonSpec menu_button_at(uint8_t index) {
    static const MenuOption options[] = {
        MENU_OPTION_SINGLEPLAYER,
        MENU_OPTION_SEED,
        MENU_OPTION_EXIT
    };
    static const char *labels[] = {
        "singleplayer",
        "seed",
        "exit"
    };

    MenuButtonSpec spec;
    spec.option = options[index];
    spec.button = make_menu_button(MENU_BUTTON_X,
                                   MENU_BUTTON_Y + index * (MENU_BUTTON_H + MENU_BUTTON_GAP),
                                   labels[index]);
    return spec;
}

MenuOption ui_menu_hit_test(int x, int y) {
    for (uint8_t i = 0; i < MENU_BUTTON_COUNT; i++) {
        MenuButtonSpec spec = menu_button_at(i);
        if (ui_button_contains(&spec.button, x, y)) {
            return spec.option;
        }
    }
    return MENU_OPTION_NONE;
}

int ui_menu_draw(const menu_t *menu, const Assets *assets, const StatusBarInfo *status) {
    if (menu == NULL || assets == NULL) return 1;

    if (ui_draw_background(&assets->background_sprite) != 0) return 1;
    if (ui_draw_sprite(&assets->name_sprite, 50, 100) != 0) return 1;
    anim_draw(&assets->mascot_anim, MENU_MASCOT_X, MENU_MASCOT_Y);

    for (uint8_t i = 0; i < MENU_BUTTON_COUNT; i++) {
        MenuButtonSpec spec = menu_button_at(i);
        spec.button.hovered = spec.option == menu->hovered_option;

        char dynamic_label[32];
        if (spec.option == MENU_OPTION_SEED) {
            if (menu->entering_seed) {
                snprintf(dynamic_label, sizeof(dynamic_label), "seed %s", menu->seed_str);
            } else if (menu->seed_val > 0) {
                snprintf(dynamic_label, sizeof(dynamic_label), "seed %u", (unsigned)menu->seed_val);
            } else {
                snprintf(dynamic_label, sizeof(dynamic_label), "set seed");
            }
            spec.button.label = dynamic_label;
        }

        if (draw_button_with_label(&spec.button, &assets->game_font) != 0) {
            return 1;
        }
    }

    if (status != NULL && status->date != NULL && status->time != NULL) {
        if (ui_status_bar_draw(assets, status) != 0) return 1;
    }

    return 0;
}
