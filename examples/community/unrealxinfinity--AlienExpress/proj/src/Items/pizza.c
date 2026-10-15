#include "pizza.h"


void draw_pizza(int index){
    if(!pizza[index].is_dead && !pizza[index].is_in_inventory){
        draw(pizza[index], pizza[index]);
    }
}

void use_pizza(int index){
    if(player_lives+inventory[index].idle_time > max_health){
        player_lives = max_health;
    }
    else{
        player_lives += inventory[index].idle_time;
    }
    inventory[index].is_dead = true;
    inventory[index].is_in_inventory = false;
    for(int i = 0; i < (int) (sizeof(pizza) / sizeof(pizza[0])); i++){
        if(pizza[i].is_in_inventory){
            pizza[i].is_in_inventory = false;
            pizza[i].is_dead = true;
            food_size++;
            break;
        }
    }
}

void get_pizza(int index){
    for(int z = 0; z < (int) (sizeof(pizza) / sizeof(pizza[0])); z++){
        if(!pizza[z].is_in_inventory && pizza[z].is_dead){
            pizza[z].is_in_inventory = true;
            pizza[z].is_dead = false;
            pizza[z].idle_time = 20;
            pizza[z].type = PIZZA;
            inventory[index].colors_32 = pizza[z].colors_32;
            inventory[index].width = pizza[z].width;
            inventory[index].height = pizza[z].height;
            inventory[index].type = PIZZA;
            inventory[index].idle_time = 20;
            break;
        }
    }
}
