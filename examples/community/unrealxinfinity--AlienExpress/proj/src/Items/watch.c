#include "watch.h"

void draw_watch(){
    if(!watch.is_dead && !watch.is_in_inventory){
        draw(watch, watch);
    }
    if(get_current_index() != -1){
        if(inventory[get_current_index()].type == WATCH && inventory[get_current_index()].is_in_inventory){
            draw_rtc();
        }
    }

}

void get_watch(int index){
    watch.is_dead = false;
    watch.is_in_inventory = true;
    inventory[index].colors_32 = watch.colors_32;
    inventory[index].width = watch.width;
    inventory[index].height = watch.height;
    inventory[index].type = WATCH;
    watch.type = WATCH;
}

int draw_rtc(){
    int x = 400;
    int seconds = rtc_time[0];
    int size = 2;
    while(size>0){
        int number = seconds % 10;
        seconds = seconds /10;
        draw_number(number, x, 20, 0);
        x-=15;
        size--;
    }
    set_char(&two_dots, x, 20, 0);
    x-=15;
    int minutes = rtc_time[1];
    size = 2;
    while(size > 0){
        int number = minutes%10;
        minutes = minutes / 10;
        draw_number(number, x, 20, 0);
        x-=15;
        size--;
    }
    set_char(&two_dots, x, 20, 0);
    x-=15;
    int hour = rtc_time[2] +1;
    size = 2;
    while(size > 0){
        int number = hour % 10;
        hour = hour/10;
        draw_number(number, x, 20, 0);
        x-=15;
        size--;
    }
    return 0;
}



