#include "menu_viewer.h"
#include "viewer.h"
#include "../model/menu/menu.h"
#include "../drivers/video_card/gpu.h"
#include "viewer.h"
#include <stdlib.h>
#include <stdio.h>
#include "../drivers/rtc/rtc.h"

extern rtc_timestamp_t real_time;

int draw_title(){
  
  for (int i = 0; i < menu->title.num_words; i++) {

    if(draw_word(alphabet, menu->title.x, menu->title.y + (i * 90), menu->title.words[i]))
       return 1;
  } 

  return 0;
}

int draw_buttons(){

  if(draw_sprite(menu->playButton.sprite, menu->playButton.x, menu->playButton.y)){
       return 1;
  }

  if(draw_sprite(menu->exitButton.sprite, menu->exitButton.x, menu->exitButton.y)){
       return 1;
  }

  return 0;
}

int draw_date(){
  
    if(drawDate(&real_time)){
        return 1;
    }
    return 0;
}

int draw_menu(){

  if(draw_sprite(menu->background, 0, 0)) return 1;

  if (draw_title()) return 1;

  if (draw_buttons()) return 1;

  return 0;
}


int menu_refresh() {

    if(erase_cursor()) return 1;

    if(erase_date()) return 1;

    if(draw_date()) return 1;
  
    if (draw_cursor()) return 1;

    if (render_screen()) return 1;
    
  
    return 0;
}



int (menu_screenshot)(){

  if (clean_screen()) return 1;

  if(draw_menu()) return 1;

  if (copy_buffer_to_screenshot())
    return 1;
  
  return 0;
}

