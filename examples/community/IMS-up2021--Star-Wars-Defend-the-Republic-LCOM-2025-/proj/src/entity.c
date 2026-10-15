/**
 * @file entity.c
 * @brief Implements functionality related to game entities, specifically the mouse cursor.
 */

#include "entity.h"
#include "gameState.h"
#include <math.h>
#include <lcom/lcf.h>
#include "controllers/video/graphics.h"
#include "xpms/mouse_cursor.xpm"
#include "xpms/button_cursor.xpm" 
#include "controllers/keyboardMouse/mouse.h"

Cursor *cursor;
Position mouse_pos = {260, 140};

uint16_t x_max = 1274;
uint16_t y_max = 988;

struct packet mouse_packet;

/**
 * @brief Creates a new cursor object.
 * Allocates memory for a Cursor struct, loads its XPM sprite, and initializes its position.
 * @param pos_x The initial x-coordinate for the cursor.
 * @param pos_y The initial y-coordinate for the cursor.
 * @param xpm The XPM map (sprite) for the cursor.
 * @return A pointer to the newly created Cursor, or NULL on failure (memory allocation or XPM loading).
 */
Cursor *create_cursor(unsigned int pos_x, unsigned int pos_y, xpm_map_t default_xpm) {
    Cursor *cursor = (Cursor *)malloc(sizeof(Cursor));
    if (cursor == NULL) {
        printf("Error allocating memory for cursor\n");
        return NULL;
    }

    // Inicializar ponteiros para NULL para segurança
    cursor->sprite = NULL;
    cursor->button_sprite = NULL;

    // Carregar o sprite padrão
    cursor->sprite = xpm_load(default_xpm, XPM_5_6_5, &cursor->img);
    if (cursor->sprite == NULL) {
        printf("Error loading default cursor XPM\n");
        free(cursor);
        return NULL;
    }

    // Carregar o sprite de botão
    cursor->button_sprite = xpm_load((xpm_map_t)button_cursor_xpm, XPM_5_6_5, &cursor->button_img);
    if (cursor->button_sprite == NULL) {
        printf("Error loading button cursor XPM\n");
        free(cursor->sprite); // Libertar o que já foi carregado
        free(cursor);
        return NULL;
    }

    // Inicializar propriedades
    cursor->pos_x = pos_x;
    cursor->pos_y = pos_y;
    // As dimensões devem ser as do sprite padrão inicialmente
    cursor->width = cursor->img.width;
    cursor->height = cursor->img.height;

    return cursor;
}

/**
 * @brief Initializes the mouse cursor.
 * Creates the cursor using `create_cursor` with predefined settings.
 * Sets the global `x_max` and `y_max` based on `mode_info`.
 * @return True if the cursor was successfully initialized, false otherwise.
 */
bool init_cursor(void) {
    cursor = create_cursor(mouse_pos.x, mouse_pos.y, (xpm_map_t)mouse_cursor_xpm);

    if (cursor == NULL) {
        printf("Error creating cursor\n");
        return false;
    }
    return true;
}

/**
 * @brief Checks if the cursor is currently over any active button based on the game state.
 * @return True if the cursor is over a button, false otherwise.
 */
bool is_cursor_on_any_button() {
    // Usa a posição do cursor global
    unsigned int x = cursor->pos_x;
    unsigned int y = cursor->pos_y;
    
    switch (state) {
        case MAIN_MENU:
            if (x >= PLAY_BTN_X1 && x <= PLAY_BTN_X2 && y >= PLAY_BTN_Y1 && y <= PLAY_BTN_Y2) return true;
            if (x >= INSTR_BTN_X1 && x <= INSTR_BTN_X2 && y >= INSTR_BTN_Y1 && y <= INSTR_BTN_Y2) return true;
            if (x >= EXIT_BTN_X1 && x <= EXIT_BTN_X2 && y >= EXIT_BTN_Y1 && y <= EXIT_BTN_Y2) return true;
            break;
        case PLAYING:
            if (x >= BTN1_X1 && x <= BTN1_X2 && y >= BTN1_Y1 && y <= BTN1_Y2) return true;
            if (x >= BTN2_X1 && x <= BTN2_X2 && y >= BTN2_Y1 && y <= BTN2_Y2) return true;
            if (x >= BTN3_X1 && x <= BTN3_X2 && y >= BTN3_Y1 && y <= BTN3_Y2) return true;
            if (x >= BTN4_X1 && x <= BTN4_X2 && y >= BTN4_Y1 && y <= BTN4_Y2) return true;
            if (x >= BTN5_X1 && x <= BTN5_X2 && y >= BTN5_Y1 && y <= BTN5_Y2) return true;
            if (x >= PAUSE_BTN_X1 && x <= PAUSE_BTN_X2 && y >= PAUSE_BTN_Y1 && y <= PAUSE_BTN_Y2) return true;
            if (x >= RESUME_BTN_X1 && x <= RESUME_BTN_X2 && y >= RESUME_BTN_Y1 && y <= RESUME_BTN_Y2) return true;
            if (x >= FAST_BTN_X1 && x <= FAST_BTN_X2 && y >= FAST_BTN_Y1 && y <= FAST_BTN_Y2) return true;
            break;
        case INSTRUCTIONS:
            if (x >= BACK_BTN_X1 && x <= BACK_BTN_X2 && y >= BACK_BTN_Y1 && y <= BACK_BTN_Y2) return true;
            break;
        default:
            return false;
    }
    return false;
}

/**
 * @brief Draws the cursor, selecting the appropriate sprite based on its position.
 * @param c Pointer to the Cursor object to draw.
 * @return 0 on success, 1 if the cursor or its sprite is NULL.
 */
int draw_cursor(Cursor *c) {
    if (c == NULL) return 1;

    uint8_t *active_sprite_data;
    xpm_image_t active_img;

    // Decide qual sprite e qual imagem usar
    if (is_cursor_on_any_button()) {
        active_sprite_data = c->button_sprite;
        active_img = c->button_img;
    } else {
        active_sprite_data = c->sprite;
        active_img = c->img;
    }

    // Verifica se os dados do sprite ativo são válidos
    if (active_sprite_data == NULL) return 1;

    // Extrai os dados dos pixeis da imagem ativa
    uint16_t *pixels = (uint16_t *)active_img.bytes;
    unsigned int width = active_img.width;
    unsigned int height = active_img.height;

    for (uint16_t y = 0; y < height; y++) {
        for (uint16_t x = 0; x < width; x++) {
            uint16_t color = pixels[y * width + x];
            if (color == TRANSPARENT_COLOR) continue;
            vg_draw_pixel(c->pos_x + x, c->pos_y + y, color);
        }
    }

    return 0;
}

/**
 * @brief Updates the mouse cursor's location based on delta movements.
 * Ensures the cursor stays within the screen boundaries defined by `x_max` and `y_max`.
 * @param delta_x The change in x-coordinate.
 * @param delta_y The change in y-coordinate (note: typically inverted from mouse hardware).
 */
void (update_mouse_location)(int16_t delta_x, int16_t delta_y){
    int32_t temp_x = cursor->pos_x;
    int32_t temp_y = cursor->pos_y;

    temp_x += delta_x;
    temp_y -= delta_y;

    if (temp_x < 0) temp_x = 0;
    if (temp_x > x_max) temp_x = x_max;

    if (temp_y < 0) temp_y = 0;
    if (temp_y > y_max) temp_y = y_max;

    cursor->pos_x = (unsigned int)temp_x;
    cursor->pos_y = (unsigned int)temp_y;

}
