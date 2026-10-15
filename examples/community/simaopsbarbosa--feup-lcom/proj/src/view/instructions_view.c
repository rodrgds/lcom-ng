#include "instructions_view.h"

extern bool instructions_button_hovered;

void draw_instructions_frame() {
    video_draw_xpm(background, 0, 0, drawing_frame_buffer);
    video_draw_xpm(logo, 194, 46, drawing_frame_buffer);

    video_draw_centered_text(240, 0x970000, "GUESS MODE", drawing_frame_buffer, 3);
    video_draw_centered_text(236, 0xFFFFFF, "GUESS MODE", drawing_frame_buffer, 3);

    video_draw_centered_text(280, 0xFFFFFF, "GUESS WHEN X SECONDS HAVE PASSED", drawing_frame_buffer, 2);
    video_draw_centered_text(300, 0xFFFFFF, "TIME STARTS AFTER BACKGROUND TURNS GREEN", drawing_frame_buffer, 2);
    video_draw_centered_text(320, 0xFFFFFF, "PRESS SPECIFIED KEY TO STOP THE TIMER", drawing_frame_buffer, 2);
    
    video_draw_centered_text(370, 0x970000, "REACT MODE", drawing_frame_buffer, 3);
    video_draw_centered_text(366, 0xFFFFFF, "REACT MODE", drawing_frame_buffer, 3);

    video_draw_centered_text(410, 0xFFFFFF, "REACT AS FAST AS YOU CAN!", drawing_frame_buffer, 2);
    video_draw_centered_text(430, 0xFFFFFF, "PRESS SPECIFIED KEY WHEN SCREEN TURNS GREEN", drawing_frame_buffer, 2);

    if (instructions_button_hovered) {
        video_draw_xpm(button_hovered, 268, 500, drawing_frame_buffer);
    } else {
        video_draw_xpm(button, 268, 500, drawing_frame_buffer);
    }
    video_draw_text((mode_info.XResolution - (strlen("GO BACK") * 8 * 2)) / 2, 520, 0x292929, "GO BACK", drawing_frame_buffer, 2);

    video_draw_xpm(cursor, cursor_x, cursor_y, drawing_frame_buffer);
}
