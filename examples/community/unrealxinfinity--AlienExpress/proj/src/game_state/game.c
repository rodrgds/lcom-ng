#include "game.h"


uint8_t timer_bit_no;
uint8_t kbd_bit_no;
uint8_t mouse_bit_no;
uint8_t rtc_bit_no;
message msg;
game_info_t game_info = {MENU, START};

//setters
void set_state(game_state_t state){
    if(state == LEVEL1){
        if(game_info.state == GAME_OVER || game_info.state == MENU){
            eliminate_entities();
            counter_timer = 0;
        }
    }
    game_info.state = state;
    game_info.action = AFK;
    player.direction = STILL;
}

//handlers
int keyboard_ih(uint32_t scancode){
    if(game_info.state == MENU){
        game_info.action = keyboard_ih_menu(scancode);
        if(game_info.action != AFK)set_selection_menu(AFK);
    }
    else if(game_info.state == LEVEL1){
        game_info.action = keyboard_ih_level(scancode);
    }
    else if(game_info.state == PAUSE){
        game_info.action = keyboard_ih_pause(scancode);
        if(game_info.action != AFK)set_selection_pause(AFK);
    }
    else if(game_info.state == GAME_OVER){
        game_info.action = keyboard_ih_game_over(scancode);
        if(game_info.action != AFK)set_selection_game_over(AFK);
    }
    return 0;
}
int mouse_proj_ih(){
    mouse.x = mouse_x;
    mouse.y = mouse_y;
    if(game_info.state == MENU){
        game_info.action = mouse_ih_menu();
        if(game_info.action != AFK)set_selection_menu(AFK);
    }
    else if(game_info.state == LEVEL1){
        game_info.action = mouse_ih_level();
    }
    else if(game_info.state == PAUSE){
        game_info.action = mouse_ih_pause();
        if(game_info.action != AFK)set_selection_pause(AFK);
    }
    else if(game_info.state == GAME_OVER){
        game_info.action = mouse_ih_game_over();
        if(game_info.action != AFK)set_selection_game_over(AFK);
    }
    return 0;
}
bool control_state(){
    switch(game_info.state){
        case MENU:
            if(game_info.action == START){
                reset_movement();
                set_state(LEVEL1);
                init_level();
                init_anim_img();
            }
            else if(game_info.action == EXIT){
                return true;
            }
            break;
        case LEVEL1:
            if(game_info.action == EXIT){
                set_state(PAUSE);
                reset_movement();
            }
            break;
        case PAUSE:
            if(game_info.action == EXIT){
                set_state(MENU);
            }
            else if(game_info.action == START){
                set_state(LEVEL1);
                
            }
            break;
        case GAME_OVER:
            if(game_info.action == START){
                reset_movement();
                set_state(LEVEL1);
                init_level();
                init_anim_img();
            }
            else if(game_info.action == EXIT){
                set_state(MENU);
            }
        default:
            return false;
    }
    return false;
}

int draw_state(){
    switch(game_info.state){
        case MENU:
            draw_menu();
            break;
        case LEVEL1:
            if((counter_timer%60) == 0){
                    spawn();
            }
            draw_level();
            if(player_lives <= 0)set_state(GAME_OVER);
            break;
        case PAUSE:
            draw_afk_level();
            draw_pause();
            break;
        case GAME_OVER:
            draw_level();
            draw_game_over();
            
            break;
        default:
            return 1;
    }
    if(mouse_hover)draw(mouse_animation.animation[1], mouse);
    else draw(mouse_animation.animation[0], mouse);
    return 0;
}


//game_loop
int initialize(){
    memset(&game_info, 0, sizeof(game_info));
    if(init_graphics(DIRECT_COLOR_8_8_8_8)) return 1;
    Allocate();
    drawBorder();
    drawTiles();
    set_state(MENU);

    if(mouse_write(STREAM_MODE)) return 1;
    if(mouse_write(DATA_REPORT))return 1;
    if(timer_set_frequency_proj(0, 30)) return 1;
    init_mouse();
    initialize_game();
    timer_bit_no = timer_subscribe();
    if(timer_bit_no == 0xFF) return 1;
    kbd_bit_no = keyboard_subscribe();
    if(kbd_bit_no == 0xFF) return 1;
    mouse_bit_no = mouse_subscribe();
    if(mouse_bit_no == 0xFF)return 1;
    rtc_bit_no = rtc_subscribe();
    if(rtc_bit_no == 0xFF)return 1;
    update(true);
    return 0;
}

int game_loop(){
  int ipc_status;
  while(true) {
		if ((driver_receive(ANY, &msg, &ipc_status)) != 0) { 
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
                if (msg.m_notify.interrupts & timer_bit_no){
                    if(game_info.state == LEVEL1)timer_int_handler_proj();
                    memset(frame_buffer, 0, frame_size);
                    if(game_info.state != MENU)pass_map();
                    
                    draw_state();
                   
                    pass_to_vm_buffer();
                }
                if (msg.m_notify.interrupts & rtc_bit_no){
                    rtc_ih();
                }
				if(msg.m_notify.interrupts & kbd_bit_no) {
                    uint32_t command;
					uint32_t scancode = kbc_read_output(OUT_BUFFER, &command);
                    keyboard_ih(scancode);
                    
					if(control_state()){
                        return 0;
                    }
				}
                if(msg.m_notify.interrupts & mouse_bit_no){
                    mouse_control();
                    mouse_proj_ih();
                    if(control_state()){
                        return 0;
                    }
                }
                
				break;

			default:
				break;
			}
		}
	}
  return 0;
}



int terminate(){
    if(mouse_unsubscribe())return 1;
    if(rtc_unsubscribe())return 1;
    update(false);
    if(keyboard_unsubscribe()) return 1;
    if(mouse_write(0xF5))return 1;
    if(timer_unsubscribe()) return 1;
    keyboard_restore();  
    if(vg_exit()) return 1;
    free(frame_buffer);
    free(prev_buffer);
    free(map);
    return 0;
}

