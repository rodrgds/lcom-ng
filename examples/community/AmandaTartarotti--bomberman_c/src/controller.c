#include "../inc/controller.h"
#include "../inc/pixmap.h"
#include "../inc/draw.h"
#include "../inc/data_structures.h"

extern char map[15][20];
extern char menu[4];
uint8_t timer_mask;
uint8_t keyboard_mask;
uint8_t mouse_mask;
uint8_t rtc_mask;
int timed = 0;
extern int explosion_time;


int cur_entry = 0;
extern uint8_t scancode;
extern state_game game_state;
extern GamePos arrow_position;
extern GamePos hero_position;
extern GamePos bomb_position;
extern GamePos unlimited_bomb_position[10];
extern GamePos monster_position[5];
extern GamePos upgraded_monster_position[1];
extern GamePos monster_position[5];
extern GamePos upgraded_monster_position[1];
extern GamePos life_position;
extern int countMonster;
extern int countUpMonster;
extern int countLife;  
extern bool unlimited_bombs;
extern int helper_unlimited;
bool exitGame = false;


extern state_bomb bomb_state;
extern int count_unlimited_bombs;
extern state_bomb unlimited_bombs_state[10];
int timing_bombs[10] = {0,0,0,0,0,0,0,0,0,0};
extern uint8_t using_db;
bool change_vram_mode = false;


int setup(){
    uint16_t vram_mode = 0x115;
    if(change_vram_mode) vram_mode = 0x14C; 
    if(map_vram(vram_mode)) return 1;
    if(set_mode(vram_mode)) return 1;
    if(alloc_double_buffer(vram_mode)) return 1;
    game_state = MENU;
    if(start_menu()) return 1;
    if(mouse_enable_data_reporting() != 0) return 1;
    if(rtc_subscribe(&rtc_mask)) return 1;
    if(kbd_subscribe_int(&keyboard_mask)) return 1;
    if(mouse_subscribe_int(&mouse_mask)) return 1;
    if(timer_subscribe_int(&timer_mask)) return 1;
    

    return 0;
}

//extern Sprite *name;

int setdown(){
    //destroy_sprite(name);

    destroy_sprites();
    if(vg_exit()) return 1;
    if(rtc_unsubscribe()) return 1;
    if(kbd_unsubscribe_int()) return 1;
    if(timer_unsubscribe_int()) return 1;
    if(mouse_unsubscribe_int()) return 1;
    if(mouse_disable_stream()) return 1;
    return 0;
}

bool breakCondition(){
    if(exitGame) return false;
    return true;
}

void increase_life(){
    if(countLife == 2) return;
    if (countLife < 3) countLife++;
}

void decrease_life(){
    countLife--;}

extern vbe_mode_info_t vbe;

void arrow_update(int16_t delta_x, int16_t delta_y){

    int16_t new_posX = arrow_position.x + delta_x;
    int16_t new_posY = arrow_position.y - delta_y;

    if(new_posX < (vbe.XResolution - 20) && new_posX > 0)
        arrow_position.x = new_posX;
    if(new_posY < (vbe.YResolution - 20) && new_posY > 0)
        arrow_position.y = new_posY;
    using_db = TRUE;
}

int mouse_response(int direction){
    switch (direction)
    {
    case 1: 
        if(map[hero_position.y][hero_position.x + 1] == ' '){
            map[hero_position.y][hero_position.x] = ' ';
            map[hero_position.y][hero_position.x + 1] = 'S';
            hero_position.x += 1;
        }
        
        break;
    case 2:
        if(map[hero_position.y][hero_position.x - 1] == ' '){
            map[hero_position.y][hero_position.x] = ' ';
            map[hero_position.y][hero_position.x - 1] = 'S';
            hero_position.x -= 1;
        }
        
        break;
    case 3: 
        if(map[hero_position.y+1][hero_position.x] == ' '){
            map[hero_position.y][hero_position.x] = ' ';
            map[hero_position.y + 1][hero_position.x] = 'S';
            hero_position.y += 1;
        }
        
        break;
    case 4: 
        if(map[hero_position.y-1][hero_position.x] == ' '){
            map[hero_position.y][hero_position.x] = ' ';
            map[hero_position.y -1][hero_position.x] = 'S';
            hero_position.y -= 1;
        }

        break;
    case 5:
        if(unlimited_bombs && count_unlimited_bombs < 10){
            unlimited_bombs_state[count_unlimited_bombs] = ONE_BOMB;

        if(map[hero_position.y][hero_position.x + 1] == ' '){
            unlimited_bomb_position[count_unlimited_bombs].y = hero_position.y;
            unlimited_bomb_position[count_unlimited_bombs].x = hero_position.x + 1;
            map[unlimited_bomb_position[count_unlimited_bombs].y][unlimited_bomb_position[count_unlimited_bombs].x] = 'B';
        } 

        else if(map[hero_position.y][hero_position.x - 1] == ' '){
            unlimited_bomb_position[count_unlimited_bombs].y = hero_position.y;
            unlimited_bomb_position[count_unlimited_bombs].x = hero_position.x - 1;
            map[unlimited_bomb_position[count_unlimited_bombs].y][unlimited_bomb_position[count_unlimited_bombs].x] = 'B';
        }

        else if(map[hero_position.y + 1][hero_position.x] == ' '){
            unlimited_bomb_position[count_unlimited_bombs].y = hero_position.y + 1;
            unlimited_bomb_position[count_unlimited_bombs].x = hero_position.x;
            map[unlimited_bomb_position[count_unlimited_bombs].y][unlimited_bomb_position[count_unlimited_bombs].x] = 'B';
        }

        else if(map[hero_position.y - 1][hero_position.x] == ' '){
            unlimited_bomb_position[count_unlimited_bombs]
            .y = hero_position.y - 1;
            unlimited_bomb_position[count_unlimited_bombs].x = hero_position.x;
            map[unlimited_bomb_position[count_unlimited_bombs]
            .y][unlimited_bomb_position[count_unlimited_bombs].x] = 'B';
        } else break;
        timing_bombs[count_unlimited_bombs] = timed;
        count_unlimited_bombs++;
        }
        else if(bomb_state == NO_BOMB){
        explosion_time = timed + 3;
        bomb_state = ONE_BOMB;

        if(map[hero_position.y][hero_position.x + 1] == ' '){
            bomb_position.y = hero_position.y;
            bomb_position.x = hero_position.x + 1;
            map[bomb_position.y][bomb_position.x] = 'B';
        } 

        else if(map[hero_position.y][hero_position.x - 1] == ' '){
            bomb_position.y = hero_position.y;
            bomb_position.x = hero_position.x - 1;
            map[bomb_position.y][bomb_position.x] = 'B';
        }

        else if(map[hero_position.y + 1][hero_position.x] == ' '){
            bomb_position.y = hero_position.y + 1;
            bomb_position.x = hero_position.x;
            map[bomb_position.y][bomb_position.x] = 'B';
        }

        else if(map[hero_position.y - 1][hero_position.x] == ' '){
            bomb_position.y = hero_position.y - 1;
            bomb_position.x = hero_position.x;
            map[bomb_position.y][bomb_position.x] = 'B';
        } else break;
    }
    break;
    
    default:
        break;
    }

    using_db = TRUE;
    if(graphic_update()) return 1;

    return 0;
}

int gamepad_response(){
    if(game_state == GAME)
    {
        switch (scancode)
        {
        case 0xCD:
            if(map[hero_position.y][hero_position.x + 1] == ' '){
                map[hero_position.y][hero_position.x] = ' ';
                map[hero_position.y][hero_position.x + 1] = 'S';
                hero_position.x += 1;
            }

            break;
        case 0xCB:
            if(map[hero_position.y][hero_position.x - 1] == ' '){
                map[hero_position.y][hero_position.x] = ' ';
                map[hero_position.y][hero_position.x - 1] = 'S';
                hero_position.x -= 1;
            }

            break;
        case 0xD0:
            if(map[hero_position.y+1][hero_position.x] == ' '){
                map[hero_position.y][hero_position.x] = ' ';
                map[hero_position.y + 1][hero_position.x] = 'S';
                hero_position.y += 1;
            }

            break;
        case 0xC8:
            if(map[hero_position.y-1][hero_position.x] == ' '){
                map[hero_position.y][hero_position.x] = ' ';
                map[hero_position.y -1][hero_position.x] = 'S';
                hero_position.y -= 1;
            }
            break;
        case 0x39:
            if(unlimited_bombs && count_unlimited_bombs < 10){
                unlimited_bombs_state[count_unlimited_bombs] = ONE_BOMB;

            if(map[hero_position.y][hero_position.x + 1] == ' '){
                unlimited_bomb_position[count_unlimited_bombs].y = hero_position.y;
                unlimited_bomb_position[count_unlimited_bombs].x = hero_position.x + 1;
                map[unlimited_bomb_position[count_unlimited_bombs].y][unlimited_bomb_position[count_unlimited_bombs].x] = 'B';
            } 

            else if(map[hero_position.y][hero_position.x - 1] == ' '){
                unlimited_bomb_position[count_unlimited_bombs].y = hero_position.y;
                unlimited_bomb_position[count_unlimited_bombs].x = hero_position.x - 1;
                map[unlimited_bomb_position[count_unlimited_bombs].y][unlimited_bomb_position[count_unlimited_bombs].x] = 'B';
            }

            else if(map[hero_position.y + 1][hero_position.x] == ' '){
                unlimited_bomb_position[count_unlimited_bombs].y = hero_position.y + 1;
                unlimited_bomb_position[count_unlimited_bombs].x = hero_position.x;
                map[unlimited_bomb_position[count_unlimited_bombs].y][unlimited_bomb_position[count_unlimited_bombs].x] = 'B';
            }

            else if(map[hero_position.y - 1][hero_position.x] == ' '){
                unlimited_bomb_position[count_unlimited_bombs]
                .y = hero_position.y - 1;
                unlimited_bomb_position[count_unlimited_bombs].x = hero_position.x;
                map[unlimited_bomb_position[count_unlimited_bombs]
                .y][unlimited_bomb_position[count_unlimited_bombs].x] = 'B';
            } else break;
            timing_bombs[count_unlimited_bombs] = timed;
            count_unlimited_bombs++;
            }
            else if(bomb_state == NO_BOMB){
            explosion_time = timed + 3;
            bomb_state = ONE_BOMB;

            if(map[hero_position.y][hero_position.x + 1] == ' '){
                bomb_position.y = hero_position.y;
                bomb_position.x = hero_position.x + 1;
                map[bomb_position.y][bomb_position.x] = 'B';
            } 

            else if(map[hero_position.y][hero_position.x - 1] == ' '){
                bomb_position.y = hero_position.y;
                bomb_position.x = hero_position.x - 1;
                map[bomb_position.y][bomb_position.x] = 'B';
            }

            else if(map[hero_position.y + 1][hero_position.x] == ' '){
                bomb_position.y = hero_position.y + 1;
                bomb_position.x = hero_position.x;
                map[bomb_position.y][bomb_position.x] = 'B';
            }

            else if(map[hero_position.y - 1][hero_position.x] == ' '){
                bomb_position.y = hero_position.y - 1;
                bomb_position.x = hero_position.x;
                map[bomb_position.y][bomb_position.x] = 'B';
            } else break;
            }
        break;
        case 0x81:
            game_state = MENU;
            break;
        default:
            break;
        }
        }
    else if(game_state == MENU){
        switch (scancode)
        {
        case 0xD0:
            if(cur_entry < 3){
                menu[cur_entry] = ' ';
                cur_entry++;
                menu[cur_entry] = 'x';
            }
            if (cur_entry == 3){
                cur_entry = 0;
                menu[cur_entry] = ' ';
                menu[cur_entry] = 'x';
            }
            break;
        case 0xC8:
            if(cur_entry > 0){
                menu[cur_entry] = ' ';
                cur_entry--;
                menu[cur_entry] = 'x';
            }
            break;
        case 0x39:
        case 0x9C:
            if(cur_entry == 0){
                if(erase_screen()) return 1;
                if(start_game()) return 1;
                using_db = true;
            }
            else if(cur_entry == 1){
                game_state = INSTRUCTIONS;
            }
            else if(cur_entry == 2){
                exitGame = true;
            }
            break;
        case 0x81:
            exitGame = true;

            default:
            break;
            }
        }
    else if(game_state == GAMEOVER){
        switch (scancode)
        {
            case 0x81:
            case 0x39:
            case 0x9C:
            game_state = MENU;
            break;
        default:
            break;
        }
    }
    else if(game_state == WINNER){
        switch (scancode)
        {
            
        case 0x81:
        case 0x39:
        case 0x9C:
            game_state = MENU;
            break;
        default:
            break;
        }

    } 
    else if(game_state == INSTRUCTIONS){
        switch (scancode)
        {
        case 0x81:
        case 0x39:
        case 0x9C:
            game_state = MENU;
            break;
        
        default:
            break;
        }
    }
    

    using_db = TRUE;
    if(graphic_update()) return 1;

    return 0;
}

int move_monsters_red(){
    for(int i = 0; i < countUpMonster; i++){
        GamePos this_monster = upgraded_monster_position[i];
      
        int pop_position = (rand() % 4) + 1;
        switch(pop_position){
            case 1:
            if(map[this_monster.y][this_monster.x + 1] == ' '){
                map[this_monster.y][this_monster.x] = ' ';
                map[this_monster.y][this_monster.x + 1] = 'U';
                this_monster.x += 1;
            }
            if(map[this_monster.y][this_monster.x + 1] == 'S'){
                if(life_position.x >= 1 && life_position.x <= 3){
                if(map[life_position.y][life_position.x] == 'L'){
                    if(life_position.x < 3)life_position.x += 1; 
                    decrease_life();
                    map[life_position.y][life_position.x] = 'L';}
                    else{map[life_position.y][life_position.x] = 'L';}
                }
            }
            break;
        case 2:
            if(map[this_monster.y][this_monster.x - 1] == ' '){
                map[this_monster.y][this_monster.x] = ' ';
                map[this_monster.y][this_monster.x - 1] = 'U';}
            if(map[this_monster.y][this_monster.x - 1] == 'S'){
                if(life_position.x >= 1 && life_position.x <= 3){
                if(map[life_position.y][life_position.x] == 'L'){
                    if(life_position.x < 3)life_position.x += 1; 
                    decrease_life();
                    map[life_position.y][life_position.x] = 'L';}
                    else{map[life_position.y][life_position.x] = 'L';}
                }
            }
            break;
        case 3:
            if(map[this_monster.y+1][this_monster.x] == ' '){
                map[this_monster.y][this_monster.x] = ' ';
                map[this_monster.y + 1][this_monster.x] = 'U';
                this_monster.y += 1;
            }
            if(map[this_monster.y+1][this_monster.x] == 'S'){
                if(life_position.x >= 1 && life_position.x <= 3){
                if(map[life_position.y][life_position.x] == 'L'){
                    if(life_position.x < 3)life_position.x += 1; 
                    decrease_life();
                    map[life_position.y][life_position.x] = 'L';}
                    else{map[life_position.y][life_position.x] = 'L';}
                }
            }
            break;
        case 4:
            if(map[this_monster.y-1][this_monster.x] == ' '){
                map[this_monster.y][this_monster.x] = ' ';
                map[this_monster.y -1][this_monster.x] = 'U';
                this_monster.y -= 1;
            }

            if(map[this_monster.y-1][this_monster.x] == 'S'){
                if(life_position.x >= 1 && life_position.x <= 3){
                if(map[life_position.y][life_position.x] == 'L'){
                    if(life_position.x < 3)life_position.x += 1; 
                    decrease_life();
                    map[life_position.y][life_position.x] = 'L';}
                    else{map[life_position.y][life_position.x] = 'L';}
                }
            }
            break;
        }

    }
    using_db = TRUE;
    if(graphic_update()) return 1;

    return 0;
}

int hero_actions(){
    if(helper_unlimited == timed - 15){
        unlimited_bombs = false;
    }

    if(map[hero_position.y][hero_position.x + 1] == 'F'){
        unlimited_bombs = true;
        helper_unlimited = timed;
        map[hero_position.y][hero_position.x + 1] = ' ';
    }
    if(map[hero_position.y][hero_position.x - 1] == 'F'){
        unlimited_bombs = true;
        helper_unlimited = timed;
         map[hero_position.y][hero_position.x - 1] = ' ';
            
        }
    if(map[hero_position.y+1][hero_position.x] == 'F'){
        unlimited_bombs = true;
        helper_unlimited = timed;
         map[hero_position.y+1][hero_position.x] = ' ';
    }
    if(map[hero_position.y-1][hero_position.x] == 'F'){
        
        unlimited_bombs = true;
        helper_unlimited = timed;
         
        map[hero_position.y-1][hero_position.x] = ' ';
    }
          
    if(map[hero_position.y][hero_position.x + 1] == 'H'){
        
        if(map[life_position.y][life_position.x] == 'L'){
            map[life_position.y][life_position.x] = 'H';
            if(life_position.x > 1)life_position.x -= 1;
            increase_life();          
            
        }
        map[hero_position.y][hero_position.x + 1] = ' ';
         
        
    }
    if(map[hero_position.y][hero_position.x - 1] == 'H'){
        if(map[life_position.y][life_position.x] == 'L'){
            map[life_position.y][life_position.x] = 'H';
            if(life_position.x > 1)life_position.x -= 1;
            increase_life();
         }
         map[hero_position.y][hero_position.x - 1] = ' ';
            
        }
    if(map[hero_position.y+1][hero_position.x] == 'H'){
        if(map[life_position.y][life_position.x] == 'L'){
            map[life_position.y][life_position.x] = 'H';
            if(life_position.x > 1)life_position.x -= 1;
            increase_life();  
        }
         map[hero_position.y+1][hero_position.x] = ' ';
    }
    if(map[hero_position.y-1][hero_position.x] == 'H'){
        
        if(map[life_position.y][life_position.x] == 'L'){
            map[life_position.y][life_position.x] = 'H';
            if(life_position.x > 1)life_position.x -= 1;
            increase_life();
         }
         
        map[hero_position.y-1][hero_position.x] = ' ';
    }
     
    using_db = TRUE;
    if(graphic_update()) return 1;

    return 0;
}

int move_monsters_green(){
    for(int i = 0; i < countMonster; i++){
        GamePos this_monster = monster_position[i];
      
        int pop_position = (rand() % 4) + 1;
        switch(pop_position){
            case 1:
            if(map[this_monster.y][this_monster.x + 1] == ' '){
                map[this_monster.y][this_monster.x] = ' ';
                map[this_monster.y][this_monster.x + 1] = 'M';
                this_monster.x += 1;
            }
            if(map[this_monster.y][this_monster.x + 1] == 'S'){
                if(life_position.x >= 1 && life_position.x <= 3){

                if(map[life_position.y][life_position.x] == 'L'){
                    if(life_position.x < 3) life_position.x += 1;  
                    decrease_life();
                    map[life_position.y][life_position.x] = 'L';}
                    else{map[life_position.y][life_position.x] = 'L';}
                }
            }
            break;
        case 2:
            if(map[this_monster.y][this_monster.x - 1] == ' '){
                map[this_monster.y][this_monster.x] = ' ';
                map[this_monster.y][this_monster.x - 1] = 'M';
                this_monster.x -= 1;
            }
            if(map[this_monster.y][this_monster.x - 1] == 'S'){
                if(life_position.x >= 1 && life_position.x <= 3){
                if(map[life_position.y][life_position.x] == 'L'){
                    if(life_position.x < 3)life_position.x += 1; 
                    decrease_life();
                    map[life_position.y][life_position.x] = 'L';}
                    else{map[life_position.y][life_position.x] = 'L';}
            }}
            break;
        case 3:
            if(map[this_monster.y+1][this_monster.x] == ' '){
                map[this_monster.y][this_monster.x] = ' ';
                map[this_monster.y + 1][this_monster.x] = 'M';
                this_monster.y += 1;
            }
            
            if(map[this_monster.y+1][this_monster.x] == 'S'){
            if(life_position.x >= 1 && life_position.x <= 3){    
                if(map[life_position.y][life_position.x] == 'L'){
                    if(life_position.x < 3)life_position.x += 1; 
                    decrease_life();
                    map[life_position.y][life_position.x] = 'L';}
                    else{map[life_position.y][life_position.x] = 'L';}
            }}
            break;
        case 4:
            if(map[this_monster.y-1][this_monster.x] == ' '){
                map[this_monster.y][this_monster.x] = ' ';
                map[this_monster.y -1][this_monster.x] = 'M';
                this_monster.y -= 1;
            }
            if(map[this_monster.y-1][this_monster.x] == 'S'){
              if(life_position.x >= 1 && life_position.x <= 3){  
                if(map[life_position.y][life_position.x] == 'L'){
                    if(life_position.x < 3)life_position.x += 1;
                    decrease_life();
                    map[life_position.y][life_position.x] = 'L';}
                    else{map[life_position.y][life_position.x] = 'L';}
            }}
            break;
        }

    }
    using_db = TRUE;
    if(graphic_update()) return 1;

    return 0;
}

void get_upgrade(GamePos position){
    int pop_position = (rand() % 10) + 1;
    switch (pop_position)
    {
    case 2:
        map[position.y][position.x] = 'H';
        break;
    case 3:
        map[position.y][position.x] = 'F';
        break;
    //OTHER UPGRADES 3-10 HERE//
    
    default:
        map[position.y][position.x] = 'E';
    break;
    }
}
int check_bomb(){

    GamePos upgrade_position;
    
    switch (bomb_state)
    {
    case NO_BOMB:
        break;

    case ONE_BOMB:
        if(explosion_time == timed)
            bomb_state = EXPLODE;
        break;

    case EXPLODE:
    
        map[bomb_position.y][bomb_position.x] = ' ';
        map[bomb_position.y][bomb_position.x] = 'E';
        
        for(int i=-2; i <= 2; i++){
             if(map[bomb_position.y+i][bomb_position.x] == '#') map[bomb_position.y+i][bomb_position.x] = '#';
        else if(map[bomb_position.y+i][bomb_position.x] == 'S' ){ 
            if(life_position.x >= 1 && life_position.x <= 3){  
                map[bomb_position.y+i][bomb_position.x] = 'S';
            if(map[life_position.y][life_position.x] == 'L'){
                if(life_position.x < 3)life_position.x += 1;  
                decrease_life();
                map[life_position.y][life_position.x] = 'L';}
            else{map[life_position.y][life_position.x] = 'L';}}}
        else if(map[bomb_position.y+i][bomb_position.x] == 'M') {
            countMonster--;
            map[bomb_position.y+i][bomb_position.x] = 'E';
        }
        else if(map[bomb_position.y+i][bomb_position.x] == 'U'){
            countUpMonster--;
            map[bomb_position.y+i][bomb_position.x] = 'E';
        }
        else if(map[bomb_position.y+i][bomb_position.x] == '%') {
            upgrade_position.x = bomb_position.x;
            upgrade_position.y = bomb_position.y+i;
            get_upgrade(upgrade_position);
        }
        else if(map[bomb_position.y+i][bomb_position.x] == '-') map[bomb_position.y+i][bomb_position.x] = '-';
        else if(map[bomb_position.y+i][bomb_position.x] == 'L') map[bomb_position.y+i][bomb_position.x] = 'L';
        else if(map[bomb_position.y+i][bomb_position.x] == 'H') map[bomb_position.y+i][bomb_position.x] = 'H';
        else if(map[bomb_position.y+i][bomb_position.x] == 'F') map[bomb_position.y+i][bomb_position.x] = 'F';
        else map[bomb_position.y+i][bomb_position.x] = 'E';
        
            if(map[bomb_position.y][bomb_position.x + i] == '#') map[bomb_position.y][bomb_position.x + i] = '#';
        else if(map[bomb_position.y][bomb_position.x + i] == 'S'){ 
            if(life_position.x >= 1 && life_position.x <= 3){
                map[bomb_position.y][bomb_position.x + i] = 'S';
            if(map[life_position.y][life_position.x] == 'L'){
                if(life_position.x < 3)life_position.x += 1; 
                decrease_life();
                map[life_position.y][life_position.x] = 'L';}
            else{map[life_position.y][life_position.x] = 'L';}}
        }
        else if(map[bomb_position.y][bomb_position.x + i] == 'M') {
            countMonster--;
            map[bomb_position.y][bomb_position.x + i] = 'E';
        }
        else if(map[bomb_position.y][bomb_position.x + i] == 'U'){
            countUpMonster--;
            map[bomb_position.y][bomb_position.x + i] = 'E';
        }
        else if(map[bomb_position.y][bomb_position.x + i] == '%') {
            upgrade_position.x = bomb_position.x + i;
            upgrade_position.y = bomb_position.y;
            get_upgrade(upgrade_position);
        }
        else if(map[bomb_position.y][bomb_position.x + i] == '-') map[bomb_position.y][bomb_position.x + i] = '-';
        else if(map[bomb_position.y][bomb_position.x + i] == 'L') map[bomb_position.y][bomb_position.x + i] = 'L';
        else if(map[bomb_position.y][bomb_position.x + i] == 'H') map[bomb_position.y][bomb_position.x + i] = 'H';
        else if(map[bomb_position.y][bomb_position.x + i] == 'F') map[bomb_position.y][bomb_position.x + i] = 'F';
        else map[bomb_position.y][bomb_position.x + i] = 'E';
        }
        bomb_state = CLEAN;
        using_db = TRUE;
        if(graphic_update()) return 1;
        break;
    case CLEAN:
        map[bomb_position.y][bomb_position.x] = ' ';
        
        for(int i=-2; i <= 2; i++){
            if(map[bomb_position.y+i][bomb_position.x] == '#') map[bomb_position.y+i][bomb_position.x] = '#';
            else if(map[bomb_position.y+i][bomb_position.x] == 'S') map[bomb_position.y+i][bomb_position.x] = 'S';
            else if(map[bomb_position.y+i][bomb_position.x] == '-') map[bomb_position.y+i][bomb_position.x] = '-';
            else if(map[bomb_position.y+i][bomb_position.x] == 'H') map[bomb_position.y+i][bomb_position.x] = 'H';
            else if(map[bomb_position.y+i][bomb_position.x] == 'F') map[bomb_position.y+i][bomb_position.x] = 'F';
            else if(map[bomb_position.y+i][bomb_position.x] == 'L') map[bomb_position.y+i][bomb_position.x] = 'L';  
            else map[bomb_position.y+i][bomb_position.x] = ' ';

            if(map[bomb_position.y][bomb_position.x+i] == '#') map[bomb_position.y][bomb_position.x+i] = '#';
            else if(map[bomb_position.y][bomb_position.x+i] == 'S') map[bomb_position.y][bomb_position.x+i] = 'S';
            else if(map[bomb_position.y][bomb_position.x+i] == '-') map[bomb_position.y][bomb_position.x+i] = '-';
            else if(map[bomb_position.y][bomb_position.x+i] == 'H') map[bomb_position.y][bomb_position.x+i] = 'H';
            else if(map[bomb_position.y][bomb_position.x+i] == 'F') map[bomb_position.y][bomb_position.x+i] = 'F';
            else if(map[bomb_position.y][bomb_position.x+i] == 'L') map[bomb_position.y][bomb_position.x+i] = 'L';
        
            else map[bomb_position.y][bomb_position.x+i] = ' ';

        }
        bomb_position.y = 0; bomb_position.x = 0; explosion_time = 0;
        bomb_state = NO_BOMB;
        using_db = TRUE;
        if(graphic_update()) return 1;
    break;

    default:
        break;
    }
   
   return 0;
}



int check_unlimited_bombs(){


    GamePos upgrade_position;
    
    for(int j = 1; j < 10; j++){
    switch (unlimited_bombs_state[j])
    {
    case NO_BOMB:
        break;

    case ONE_BOMB:
        if(timing_bombs[j] == timed - 3)    
        unlimited_bombs_state[j] = EXPLODE;
        break;

    case EXPLODE:
    
        map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x] = ' ';
        map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x] = 'E';
        
        for(int i=-2; i <= 2; i++){
             if(map[unlimited_bomb_position[j].y+i][unlimited_bomb_position[j].x] == '#') map[unlimited_bomb_position[j].y+i][unlimited_bomb_position[j].x] = '#';
        else if(map[unlimited_bomb_position[j].y+i][unlimited_bomb_position[j].x] == 'S'){ 
            map[unlimited_bomb_position[j].y+i][unlimited_bomb_position[j].x] = 'S';
            if(map[life_position.y][life_position.x] == 'L'){
            if(life_position.x < 3)life_position.x += 1; 
            decrease_life();
            map[life_position.y][life_position.x] = 'L';}
            else{map[life_position.y][life_position.x] = 'L';}}
        else if(map[unlimited_bomb_position[j].y+i][unlimited_bomb_position[j].x] == 'M') {
            countMonster--;
            map[unlimited_bomb_position[j].y+i][unlimited_bomb_position[j].x] = 'E';
        }
        else if(map[unlimited_bomb_position[j].y+i][unlimited_bomb_position[j].x] == 'U'){
            countUpMonster--;
            map[unlimited_bomb_position[j].y+i][unlimited_bomb_position[j].x] = 'E';
        }
        else if(map[unlimited_bomb_position[j].y+i][unlimited_bomb_position[j].x] == '%') {
            upgrade_position.x = unlimited_bomb_position[j].x;
            upgrade_position.y = unlimited_bomb_position[j].y+i;
            get_upgrade(upgrade_position);
        }
        else if(map[unlimited_bomb_position[j].y+i][unlimited_bomb_position[j].x] == '-') map[unlimited_bomb_position[j].y+i][unlimited_bomb_position[j].x] = '-';
        else if(map[unlimited_bomb_position[j].y+i][unlimited_bomb_position[j].x] == 'L') map[unlimited_bomb_position[j].y+i][unlimited_bomb_position[j].x] = 'L';
        else if(map[unlimited_bomb_position[j].y+i][unlimited_bomb_position[j].x] == 'H') map[unlimited_bomb_position[j].y+i][unlimited_bomb_position[j].x] = 'H';
        else if(map[unlimited_bomb_position[j].y+i][unlimited_bomb_position[j].x] == 'F') map[unlimited_bomb_position[j].y+i][unlimited_bomb_position[j].x] = 'F';

        else map[unlimited_bomb_position[j].y+i][unlimited_bomb_position[j].x] = 'E';
        
            if(map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x + i] == '#') map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x + i] = '#';
        else if(map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x + i] == 'S'){ 
            map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x + i] = 'S';
            if(map[life_position.y][life_position.x] == 'L'){
            if(life_position.x < 3)life_position.x += 1; 
            decrease_life();
            map[life_position.y][life_position.x] = 'L';}
            else{map[life_position.y][life_position.x] = 'L';}}
        else if(map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x + i] == 'M') {
            countMonster--;
            map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x + i] = 'E';
        }
        else if(map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x + i] == 'U'){
            countUpMonster--;
            map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x + i] = 'E';
        }
        else if(map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x + i] == '%') {
            upgrade_position.x = unlimited_bomb_position[j].x + i;
            upgrade_position.y = unlimited_bomb_position[j].y;
            get_upgrade(upgrade_position);
        }
        else if(map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x + i] == '-') map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x + i] = '-';
        else if(map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x + i] == 'L') map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x + i] = 'L';
        else if(map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x + i] == 'H') map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x + i] = 'H';
        else if(map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x + i] == 'F') map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x + i] = 'F';

        else map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x + i] = 'E';
        }
        unlimited_bombs_state[j] = CLEAN;
        using_db = TRUE;
        if(graphic_update()) return 1;
        break;
    case CLEAN:
    if(timing_bombs[j] != timed - 6) break;

        map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x] = ' ';
        
        for(int i=-2; i <= 2; i++){
            if(map[unlimited_bomb_position[j].y+i][unlimited_bomb_position[j].x] == '#') map[unlimited_bomb_position[j].y+i][unlimited_bomb_position[j].x] = '#';
            else if(map[unlimited_bomb_position[j].y+i][unlimited_bomb_position[j].x] == 'S') map[unlimited_bomb_position[j].y+i][unlimited_bomb_position[j].x] = 'S';
            else if(map[unlimited_bomb_position[j].y+i][unlimited_bomb_position[j].x] == '-') map[unlimited_bomb_position[j].y+i][unlimited_bomb_position[j].x] = '-';
            else if(map[unlimited_bomb_position[j].y+i][unlimited_bomb_position[j].x] == 'H') map[unlimited_bomb_position[j].y+i][unlimited_bomb_position[j].x] = 'H';
            else if(map[unlimited_bomb_position[j].y+i][unlimited_bomb_position[j].x] == 'F') map[unlimited_bomb_position[j].y+i][unlimited_bomb_position[j].x] = 'F';

            else if(map[unlimited_bomb_position[j].y+i][unlimited_bomb_position[j].x] == 'L') map[unlimited_bomb_position[j].y+i][unlimited_bomb_position[j].x] = 'L';  
            else map[unlimited_bomb_position[j].y+i][unlimited_bomb_position[j].x] = ' ';

            if(map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x+i] == '#') map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x+i] = '#';
            else if(map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x+i] == 'S') map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x+i] = 'S';
            else if(map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x+i] == '-') map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x+i] = '-';
            else if(map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x+i] == 'H') map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x+i] = 'H';
            else if(map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x+i] == 'F') map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x+i] = 'F';

            else if(map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x+i] == 'L') map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x+i] = 'L';
        
            else map[unlimited_bomb_position[j].y][unlimited_bomb_position[j].x+i] = ' ';

        }
        
        //unlimited_bomb_position[j].y = 0; unlimited_bomb_position[j].x = 0; 
        unlimited_bombs_state[j] = NO_BOMB;
        count_unlimited_bombs--;
        using_db = TRUE;
        if(graphic_update()) return 1;
    break;

    default:
        break;
    }
    }

    return 0;
}



void check_game(){
    switch (game_state)
    {
    case GAME:
        if (countMonster == 0 && countUpMonster == 0) game_state = WINNER;
        if (countLife < 1) game_state = GAMEOVER;
        break;
    default:
        break;
    }
}

