#include "game_over_view.h"

extern int game_over_menu_selected_option;
extern GameMode gameMode;
extern GameMode previousGameMode;
extern Winner winner;
extern PlayerMode playerMode;
extern TimeDiff timeDiff;

void draw_game_over_frame(){
    video_draw_xpm(background, 0, 0, drawing_frame_buffer);
    video_draw_xpm(logo, 194, 46, drawing_frame_buffer);

    if (playerMode == TWO_PLAYERS) {
        if (winner == NO_ONE) {
            video_draw_centered_text(233, 0xFF0000, "NO ONE WON", drawing_frame_buffer, 3);
            video_draw_centered_text(230, 0xFFFFFF, "NO ONE WON", drawing_frame_buffer, 3);
        } else if (winner == SOMEONE) {
            char message[64];
            if (abs(timeDiff.player1) == abs(timeDiff.player2)) {
                sprintf(message, "IT'S A TIE");
            } else {
                sprintf(message, "PLAYER %d WON", abs(timeDiff.player1) > abs(timeDiff.player2) ? 2 : 1);
            }
            video_draw_centered_text(233, 0xFF0000, message, drawing_frame_buffer, 3);
            video_draw_centered_text(230, 0xFFFFFF, message, drawing_frame_buffer, 3);

            char player1_time_diff[64];
            char player2_time_diff[64];
            sprintf(player1_time_diff, "PLAYER 1: %dms", timeDiff.player1);
            sprintf(player2_time_diff, "PLAYER 2: %dms", timeDiff.player2);
            video_draw_centered_text(320, 0xFFFFFF, player1_time_diff, drawing_frame_buffer, 2);
            video_draw_centered_text(340, 0xFFFFFF, player2_time_diff, drawing_frame_buffer, 2);

        }
    } else if (playerMode == ONE_PLAYER) {
        if (winner == NO_ONE) {
            video_draw_centered_text(233, 0xFF0000, "YOU LOST", drawing_frame_buffer, 3);
            video_draw_centered_text(230, 0xFFFFFF, "YOU LOST", drawing_frame_buffer, 3);
        } else if (winner == SOMEONE) {
            video_draw_centered_text(233, 0xFF0000, "RESULTS", drawing_frame_buffer, 3);
            video_draw_centered_text(230, 0xFFFFFF, "RESULTS", drawing_frame_buffer, 3);

            char player_time_diff[64];
            sprintf(player_time_diff, "REACTION TIME: %dms", timeDiff.player1);
            video_draw_centered_text(320, 0xFFFFFF, player_time_diff, drawing_frame_buffer, 2);
        }
    }
    
    for (int i = 0; i < 2; i++) {
        if (i == game_over_menu_selected_option) {
            video_draw_xpm(button_hovered, 268, 284 + 62 * i + 62 * 2, drawing_frame_buffer); // highlight selected option
        } else {
            video_draw_xpm(button, 268, 284 + 62 * i + 62 * 2, drawing_frame_buffer);  // normal option
        }
    }

    video_draw_centered_text(284 + 20 + 62 * 2, 0x292929, "PLAY AGAIN", drawing_frame_buffer, 2);
    video_draw_centered_text(284 + 20 + 62 * 3, 0x292929, "BACK TO MENU", drawing_frame_buffer, 2);

    video_draw_xpm(cursor, cursor_x, cursor_y, drawing_frame_buffer);
}
