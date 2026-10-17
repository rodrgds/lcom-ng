#include "views/typing_test_view.h"
#include "texts/phrases.h"

#define CHAR_WIDTH 23
#define LINE_HEIGHT 50
#define MAX_LINE_CHARS 42
#define START_X 50
#define START_Y 200

void draw_text(const int* shuffled_indices, int phrase_index) {
    draw_rectangle_background(START_X, START_Y, MAX_LINE_CHARS * CHAR_WIDTH, LINE_HEIGHT * 3);
    uint16_t x = START_X, y = START_Y;

    for (int i = 0; i < 3; i++) {
        x = START_X;

        int idx = shuffled_indices[phrase_index + i];
        if (phrases[idx] == NULL) break;

        for (int j = 0; phrases[idx][j] != '\0'; j++) {
            if (phrases[idx][j] != '_') {
                draw_image_to_both((xpm_map_t)get_xpm_from_char(phrases[idx][j]), x, y);
            }
            x += CHAR_WIDTH;
        }

        y += LINE_HEIGHT;
    }
}

void draw_typed_chars(const char* typed_chars, int typed_count, const int* shuffled_indices, int phrase_index) {
    int offset = 0;

    // Calculate offset by summing lengths of all hidden phrases
    for (int i = 0; i < phrase_index; i++) {
        int idx = shuffled_indices[i];
        if (phrases[idx] == NULL) break;
        offset += strlen(phrases[idx]);
    }

    int char_global_index = offset;  // Start reading typed_chars from this index

    for (int i = 0; i < 3; i++) {
        uint16_t x = START_X;
        uint16_t y = START_Y + i * LINE_HEIGHT;

        int idx = shuffled_indices[phrase_index + i];
        if (phrases[idx] == NULL) break;

        int j = 0; // draw all chars, except the last one
        for (; phrases[idx][j+1] != '\0'; j++) {
            if (char_global_index >= typed_count) continue;

            char typed = typed_chars[char_global_index];
            char expected = phrases[idx][j];

            if (typed == expected) {
                if (typed != '_') draw_image_to_both((xpm_map_t)get_bright_xpm_from_char(typed), x, y);
            } else {
                draw_image_to_both((xpm_map_t)get_error_xpm_from_char(expected), x, y);
            }

            x += CHAR_WIDTH;
            char_global_index++;
        }
        if (char_global_index == typed_count) { // <=> if not at the beginning of the line
            // draw background rectangle to delete previous text cursor
            draw_rectangle_background(x-CHAR_WIDTH-1, y, 1, LINE_HEIGHT);
            // draw text cursor
            draw_image((xpm_map_t)text_cursor_xpm, x-1, y);
            return;
        }
        
        // draw last char
        if (char_global_index < typed_count) {
            char typed = typed_chars[char_global_index];
            char expected = phrases[idx][j];

            if (typed == expected) {
                if (typed != '_') draw_image_to_both((xpm_map_t)get_bright_xpm_from_char(typed), x, y);
            } else {
                draw_image_to_both((xpm_map_t)get_error_xpm_from_char(expected), x, y);
            }
            char_global_index++;
        }
    }
}
