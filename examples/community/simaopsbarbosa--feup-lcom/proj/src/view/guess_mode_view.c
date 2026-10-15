#include "guess_mode_view.h"

extern PlayerMode playerMode;

void draw_guess_mode_frame(int elapsed_seconds, int target_seconds) {
    if (elapsed_seconds < 3) {
        video_draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, 0x860707, drawing_frame_buffer);

        char buffer[16];
        sprintf(buffer, "%d", 3 - elapsed_seconds);
        video_draw_centered_text(300, 0x000000, buffer, drawing_frame_buffer, 7);
        video_draw_centered_text(295, 0xFFFFFF, buffer, drawing_frame_buffer, 7);
    } else {
        video_draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, 0x009a12, drawing_frame_buffer);
    }

    video_draw_centered_text(75, 0x000000, "GUESS MODE", drawing_frame_buffer, 4);
    video_draw_centered_text(71, 0xFFFFFF, "GUESS MODE", drawing_frame_buffer, 4);

    char message[64];
    sprintf(message, "GUESS WHEN %d SECONDS HAVE PASSED", target_seconds);
    video_draw_centered_text(123, 0x000000, message, drawing_frame_buffer, 2);
    video_draw_centered_text(120, 0xFFFFFF, message, drawing_frame_buffer, 2);

    if (playerMode == ONE_PLAYER) {
        video_draw_centered_text(195, 0x000000, "PRESS [SPACE]", drawing_frame_buffer, 2);
        video_draw_centered_text(192, 0xFFFFFF, "PRESS [SPACE]", drawing_frame_buffer, 2);
    } else {
        video_draw_centered_text(175, 0x000000, "PLAYER 1 PRESS [SPACE]", drawing_frame_buffer, 2);
        video_draw_centered_text(172, 0xFFFFFF, "PLAYER 1 PRESS [SPACE]", drawing_frame_buffer, 2);
        video_draw_centered_text(195, 0x000000, "PLAYER 2 PRESS [LMB]", drawing_frame_buffer, 2);
        video_draw_centered_text(192, 0xFFFFFF, "PLAYER 2 PRESS [LMB]", drawing_frame_buffer, 2);
    }
    video_draw_centered_text(220, 0x000000, "TO STOP THE TIMER", drawing_frame_buffer, 2);
    video_draw_centered_text(217, 0xFFFFFF, "TO STOP THE TIMER", drawing_frame_buffer, 2);
}
