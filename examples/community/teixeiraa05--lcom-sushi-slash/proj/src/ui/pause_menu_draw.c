#include "../../include/ui/pause_menu_draw.h"

#include "../../include/macros/menu.h"
#include "../../include/ui/ui.h"

typedef struct {
    PauseOption option;
    button_t button;
} PauseButtonSpec;

static button_t make_pause_button(uint16_t x, uint16_t y, const char *label) {
    button_t button;
    button.x = x;
    button.y = y;
    button.width = PAUSE_BUTTON_W;
    button.height = PAUSE_BUTTON_H;
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

static PauseButtonSpec pause_button_at(uint8_t index) {
    static const PauseOption options[] = {
        PAUSE_OPTION_RESUME,
        PAUSE_OPTION_MAIN_MENU,
        PAUSE_OPTION_EXIT
    };
    static const char *labels[] = {
        "resume",
        "main menu",
        "exit"
    };

    PauseButtonSpec spec;
    spec.option = options[index];
    spec.button = make_pause_button(PAUSE_BUTTON_X,
                                    PAUSE_BUTTON_Y + index * (PAUSE_BUTTON_H + PAUSE_BUTTON_GAP),
                                    labels[index]);
    return spec;
}

PauseOption ui_pause_menu_hit_test(int x, int y) {
    for (uint8_t i = 0; i < PAUSE_BUTTON_COUNT; i++) {
        PauseButtonSpec spec = pause_button_at(i);
        if (ui_button_contains(&spec.button, x, y)) {
            return spec.option;
        }
    }
    return PAUSE_OPTION_NONE;
}

int ui_pause_menu_draw(const pause_menu_t *menu, const Assets *assets, const StatusBarInfo *status) {
    if (menu == NULL || assets == NULL) return 1;

    if (ui_draw_rectangle(PAUSE_PANEL_X, PAUSE_PANEL_Y,
                          PAUSE_PANEL_W, PAUSE_PANEL_H,
                          0x1A1A1A) != 0) {
        return 1;
    }
    if (font_draw_scaled_text(&assets->game_font,
                              "paused",
                              CENTER_X(font_scaled_text_width(&assets->game_font, "paused", PAUSE_TITLE_SCALE)),
                              PAUSE_TITLE_Y,
                              PAUSE_TITLE_SCALE) != 0) {
        return 1;
    }

    anim_draw(&assets->mascot_anim, PAUSE_MASCOT_X, PAUSE_MASCOT_Y);

    for (uint8_t i = 0; i < PAUSE_BUTTON_COUNT; i++) {
        PauseButtonSpec spec = pause_button_at(i);
        spec.button.hovered = spec.option == menu->hovered_option;

        if (draw_button_with_label(&spec.button, &assets->game_font) != 0) {
            return 1;
        }
    }

    if (status != NULL && status->date != NULL && status->time != NULL) {
        if (ui_status_bar_draw(assets, status) != 0) return 1;
    }

    return 0;
}
