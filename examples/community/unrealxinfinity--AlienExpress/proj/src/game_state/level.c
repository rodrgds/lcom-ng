#include "level.h"
#include <math.h>





action_t keyboard_ih_level(uint32_t scancode){
    if(scancode == MAKE_ESC){
        return EXIT;
    }
    player_controller(scancode);

    return AFK;
}
action_t mouse_ih_level(){
    bool was_hovered = false;
    for(int i = 0; i < (inventory_size*3); i++){
            if(mouse_position(inventory[i].x, inventory[i].y, inventory[i].x+inventory[i].width, inventory[i].y+inventory[i].height)){
                if(mouse_packet.lb && !mouse_click){
                    mouse_index = i;
                    mouse_click = true;
                }
                if(!mouse_packet.lb && mouse_click){
                    mouse_click = false;
                    img_t temp = inventory[mouse_index];
                    inventory[mouse_index].colors_32 = inventory[i].colors_32;
                    inventory[mouse_index].is_in_inventory = inventory[i].is_in_inventory;
                    inventory[mouse_index].type = inventory[i].type;
                    inventory[mouse_index].width = inventory[i].width;
                    inventory[mouse_index].height = inventory[i].height;
                    inventory[mouse_index].idle_time = inventory[i].idle_time;

                    inventory[i].colors_32 = temp.colors_32;
                    inventory[i].is_in_inventory = temp.is_in_inventory;
                    inventory[i].type = temp.type;
                    inventory[i].width = temp.width;
                    inventory[i].height = temp.height;
                    inventory[i].idle_time = temp.idle_time;
                    mouse_index = -1;
                }
                if(mouse_packet.rb){
                    mouse_click = false;
                    remove_from_inventory(i);
                    mouse_index = -1;
                }
                if(mouse_packet.mb && inventory[i].is_in_inventory){
                    mouse_click = false;
                    mouse_index = -1;
                    use_item(i);
                }
                was_hovered = true;
            }
    }
    for(int i = 0; i < 3; i++){
        if(mouse_position(equipment[i].x, equipment[i].y, equipment[i].x+equipment[i].width, equipment[i].y+equipment[i].height)){
            if(mouse_packet.rb){
                mouse_click = false;
                remove_from_equipment(i);
                mouse_index = -1;
            }
            was_hovered = true;
        }
    }
    if(!mouse_packet.lb && mouse_click){
        remove_from_inventory(mouse_index);
        mouse_index = -1;
        mouse_click = false;
    }
    if(!was_hovered && !mouse_click)mouse_hover = false;
    return AFK;
}







void draw_afk_level(){
    draw_teleports();
    draw_packages();
    draw_items();
    draw_bullets();
    draw_player();

    for(int i = 0; i < (int) (sizeof(enemies_lv1) / sizeof(enemies_lv1[0])); i++){
        if(!enemies_lv1[i].is_dead) draw_enemies_lvl(enemies_lv1[i]);
    }
    for(int i = 0; i < (int) (sizeof(workers) / sizeof(workers[0])); i++){
        if(!workers[i].is_dead) draw_workers(workers[i]);
    }
    if(!wolf.is_dead && !wolf.is_in_inventory) draw_wolf(wolf);
    else if(wolf.is_in_inventory && !wolf.is_dead)draw_friendly_wolf(wolf);

    draw_explosion();
    draw_level_words();
    draw_level_numbers();

    draw_health();
    draw_inventory();
    if(!selected.is_dead)draw(selected, selected);

}


void draw_level(){
    draw_teleports();
    draw_packages();
    draw_items();
    draw_bullets();
    draw_player();

    move_player();
    move_workers();
    move_wolf();
    move_bullets();
    move_enemies_lvl();

    draw_explosion();
    draw_level_words();
    draw_level_numbers();

    draw_health();
    draw_inventory();
    if(!selected.is_dead)draw(selected, selected);


}




void draw_items(){
    for(int i = 0; i < (int) (sizeof(pistols) / sizeof(pistols[0])); i++){
        draw_pistol(i);
        draw_ammo(i);
        draw_pizza(i);
        draw_grenade(i);
    }
    draw_watch();
    draw_boots();
    draw_backpack();
    draw_bone();
}
void draw_level_numbers(){
    if(player_lives > 0){
        if((counter_timer % 30) == 0) seconds++;
        if(seconds == 60){
            minutes++;
            seconds = 0;
        }
    }
    draw_timer(seconds, minutes);
}
void draw_level_words(){

    set_char(&s, 800, 20, 0);
    set_char(&c, 815, 20, 0);
    set_char(&o, 830, 20, 0);
    set_char(&r, 845, 20, 0);
    set_char(&e, 860, 20, 0);

    two_dots.x = 875;

    draw(two_dots, two_dots);

    two_dots.x = 530;

    draw(two_dots, two_dots);

    pick_draw_type();

}

