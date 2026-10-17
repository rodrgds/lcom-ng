#ifndef GAME_H
#define GAME_H

#include <lcom/lcf.h>

#include "devices/keyboard.h"
#include "devices/mouse.h"
#include "devices/rtc.h"
#include "devices/graphics.h"
#include "devices/serialPort.h"
#include "sprite.h"


#include "images/mazes/maze1.xpm"
#include "images/mazes/mazeDay1.xpm"
#include "images/mazes/mazeDark1.xpm"

#include "images/mazes/maze2.xpm"
#include "images/mazes/mazeDay2.xpm"
#include "images/mazes/mazeDark2.xpm"

#include "images/mazes/maze3.xpm"
#include "images/mazes/mazeDay3.xpm"
#include "images/mazes/mazeDark3.xpm"

#include "images/mazeElements/button_red.xpm"
#include "images/mazeElements/button_red_down.xpm"
#include "images/mazeElements/button_green.xpm"
#include "images/mazeElements/button_green_down.xpm"
#include "images/mazeElements/door_red.xpm"
#include "images/mazeElements/door_green.xpm"

#include "images/assets/menu.xpm"
#include "images/assets/win.xpm"
#include "images/assets/game_over.xpm"
#include "images/assets/waiting.xpm"
#include "images/assets/wait2.xpm"

#include "images/levels/level1.xpm"
#include "images/levels/level2.xpm"
#include "images/levels/level3.xpm"
#include "images/levels/hover_level1.xpm"
#include "images/levels/hover_level2.xpm"
#include "images/levels/hover_level3.xpm"

#include "images/player1/right1.xpm"
#include "images/player1/right2.xpm"
#include "images/player1/left1.xpm"
#include "images/player1/left2.xpm"
#include "images/player1/up1.xpm"
#include "images/player1/up2.xpm"
#include "images/player1/down1.xpm"
#include "images/player1/down2.xpm"

#include "images/player2/right1second.xpm"
#include "images/player2/right2second.xpm"
#include "images/player2/left1second.xpm"
#include "images/player2/left2second.xpm"
#include "images/player2/up1second.xpm"
#include "images/player2/up2second.xpm"
#include "images/player2/down1second.xpm"
#include "images/player2/down2second.xpm"

#include "images/lifeBar/life1.xpm"
#include "images/lifeBar/life2.xpm"
#include "images/lifeBar/life3.xpm"
#include "images/lifeBar/life4.xpm"
#include "images/lifeBar/life5.xpm"

#include "images/energy/energy1.xpm"
#include "images/energy/energy2.xpm"
#include "images/energy/energy3.xpm"
#include "images/energy/energy4.xpm"
#include "images/energy/energy5.xpm"

#include "images/arrows/arrow1.xpm"
#include "images/arrows/arrow2.xpm"
#include "images/arrows/arrow3.xpm"
#include "images/arrows/arrow4.xpm"
#include "images/arrows/arrow5.xpm"
#include "images/arrows/arrow6.xpm"

#include "images/numbers/num0.xpm"
#include "images/numbers/num1.xpm"
#include "images/numbers/num2.xpm"
#include "images/numbers/num3.xpm"
#include "images/numbers/num4.xpm"
#include "images/numbers/num5.xpm"
#include "images/numbers/num6.xpm"
#include "images/numbers/num7.xpm"
#include "images/numbers/num8.xpm"
#include "images/numbers/num9.xpm"
#include "images/numbers/dot.xpm"

#include "images/smallNumbers/small0.xpm"
#include "images/smallNumbers/small1.xpm" 
#include "images/smallNumbers/small2.xpm"
#include "images/smallNumbers/small3.xpm"
#include "images/smallNumbers/small4.xpm"
#include "images/smallNumbers/small5.xpm"
#include "images/smallNumbers/small6.xpm"
#include "images/smallNumbers/small7.xpm"
#include "images/smallNumbers/small8.xpm"
#include "images/smallNumbers/small9.xpm"
#include "images/smallNumbers/divisor.xpm" 

#include "images/assets/hand.xpm"
#include "images/assets/title.xpm"
#include "images/assets/start_button.xpm"
#include "images/assets/hover_start.xpm"
#include "images/assets/quit_button.xpm"
#include "images/assets/hover_quit.xpm"


typedef enum {
    MENU,
    LEVELS,
    GAME,
    MULTI,
    WIN,
    LOSE,
    EXIT
} GameState;

#define TIMER_MINUTES 4
#define MOUSE_LIMIT_X 785
#define MOUSE_LIMIT_Y 575


void (draw_menu)();
void (update_menu)();
void (update_menu_levels)();
bool (collision)(Sprite * sp1, Sprite * sp2);

void (draw_menu_levels)();
void (load_level)();

void (update_game)();
xpm_map_t (get_next_sprite_player1)(uint8_t scancode); 
xpm_map_t (get_next_sprite_player2)(uint8_t scancode); 
void (handle_ingame_scancode)(uint8_t scancode, Sprite *player);
void (handle_ingame_scancode_multi)(uint8_t scancode, Sprite *player2);
void (handle_mouse_movement)(Sprite * cursor);
void (update_life_bar)(int total_seconds);
void (update_arrow_sprite)(int total_seconds);
void (update_energy_sprite)(int total_seconds);

void (draw_number)(Sprite *num_sprite, int x, int y);
void (draw_time)(int total_seconds, int x, int y);
void (draw_time_small)(uint8_t hours, uint8_t minutes, int x, int y);

void (draw_waiting)();
void (draw_win)(int total_seconds);
void (draw_lost)();


#endif

