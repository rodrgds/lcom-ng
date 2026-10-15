#include "react_mode_view.h"
#include <lcom/lcf.h>

extern PlayerMode playerMode;

void draw_react_mode_frame(int elapsed_seconds, int target_seconds) {

    if(elapsed_seconds < target_seconds) {
        video_draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, 0x860707, drawing_frame_buffer);
    }else {
        video_draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, 0x009a12, drawing_frame_buffer);
    }

    video_draw_centered_text(75, 0x000000, "REACT MODE", drawing_frame_buffer, 4);
    video_draw_centered_text(71, 0xFFFFFF, "REACT MODE", drawing_frame_buffer, 4);
    
    if (playerMode == ONE_PLAYER) {
        video_draw_centered_text(145, 0x000000, "PRESS [SPACE]", drawing_frame_buffer, 2);
        video_draw_centered_text(142, 0xFFFFFF, "PRESS [SPACE]", drawing_frame_buffer, 2);
    } else {
        video_draw_centered_text(125, 0x000000, "PLAYER 1 PRESS [SPACE]", drawing_frame_buffer, 2);
        video_draw_centered_text(122, 0xFFFFFF, "PLAYER 1 PRESS [SPACE]", drawing_frame_buffer, 2);
        video_draw_centered_text(145, 0x000000, "PLAYER 2 PRESS [LMB]", drawing_frame_buffer, 2);
        video_draw_centered_text(142, 0xFFFFFF, "PLAYER 2 PRESS [LMB]", drawing_frame_buffer, 2);
    }

    if(elapsed_seconds < target_seconds) {
        video_draw_centered_text(180, 0x000000, "WHEN THE SCREEN TURNS GREEN", drawing_frame_buffer, 2);
        video_draw_centered_text(177, 0xFFFFFF, "WHEN THE SCREEN TURNS GREEN", drawing_frame_buffer, 2);
    } else {
        video_draw_centered_text(180, 0x000000, "NOW!", drawing_frame_buffer, 3);
        video_draw_centered_text(175, 0xFFFFFF, "NOW!", drawing_frame_buffer, 3);
    }
}
