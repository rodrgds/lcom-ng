#include <lcom/lcf.h>
#include "player_menu_view.h"

extern int player_menu_selected_option;

void draw_player_menu_frame() {
    video_draw_xpm(background, 0, 0, drawing_frame_buffer);
    video_draw_xpm(logo, 194, 46, drawing_frame_buffer);

    for (int i = 0; i < 4; i++) {
        if (i == player_menu_selected_option) {
            video_draw_xpm(button_hovered, 268, 284 + 62 * i, drawing_frame_buffer); // highlight selected option
        } else {
            video_draw_xpm(button, 268, 284 + 62 * i, drawing_frame_buffer);  // normal option
        }
    }
    
    video_draw_centered_text(284 + 20 + 62 * 0, 0x292929, "1 PLAYER", drawing_frame_buffer, 2);
    video_draw_centered_text(284 + 20 + 62 * 1, 0x292929, "2 PLAYERS", drawing_frame_buffer, 2);
    video_draw_centered_text(284 + 20 + 62 * 2, 0x292929, "INSTRUCTIONS", drawing_frame_buffer, 2);
    video_draw_centered_text(284 + 20 + 62 * 3, 0x292929, "EXIT GAME", drawing_frame_buffer, 2);

    video_draw_xpm(cursor, cursor_x, cursor_y, drawing_frame_buffer);
}
