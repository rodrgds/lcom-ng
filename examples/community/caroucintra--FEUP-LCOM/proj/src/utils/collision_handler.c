#include <lcom/lcf.h>
#include "collision_handler.h"
#include "video.h"

int (clear_sprite)(Sprite* sp, Sprite* collision_element) {
  unsigned num_bytes = get_bytesperpixel();
  uint32_t color;
  int collision_element_pos;
  uint8_t* collision_map = collision_element->map;

  for (int j = 0; j < sp->img.height; j++) {
    
    for (int i = 0; i < sp->img.width; i++) {
      collision_element_pos = ((sp->x - collision_element->x + i)  + (sp->y - collision_element->y + j) * collision_element->img.width) * num_bytes;

      color = 0;
      for (unsigned byte = 0; byte < num_bytes; byte++) {
        color += *(collision_map + collision_element_pos + byte) << byte * 8;
      }
      
      if (draw_pixel(sp->x + i, sp->y + j, color))
        return 1;
        
    }
  }
  sp->changed = true;
  return 0;
}

int (clear_sprite_up)(Sprite* sp, Sprite* collision_element, int bg_delta_y, int element_delta_y){
    unsigned num_bytes = get_bytesperpixel();
  uint32_t color;
  int collision_element_pos;
  uint8_t* collision_map = collision_element->map;

  for (int j = 0; j < element_delta_y; j++) {
    
    for (int i = 0; i < sp->img.width; i++) {
      collision_element_pos = ((sp->x - collision_element->x + i)  + (sp->y + bg_delta_y - collision_element->y + j) * collision_element->img.width) * num_bytes;

      color = 0;
      for (unsigned byte = 0; byte < num_bytes; byte++) {
        color += *(collision_map + collision_element_pos + byte) << byte * 8;
      }
      
      if (draw_pixel(sp->x + i, sp->y + bg_delta_y + j, color))
        return 1;
    }
  }
  sp->changed = true;
  return 0;
}

int (clear_sprite_down)(Sprite* sp, Sprite* collision_element, int bg_delta_y, int element_delta_y){
  unsigned num_bytes = get_bytesperpixel();
  uint32_t color;
  int collision_element_pos;
  uint8_t* collision_map = collision_element->map;

  for (int j = 0; j < element_delta_y; j++) {
    
    for (int i = 0; i < sp->img.width; i++) {
      collision_element_pos = ((sp->x - collision_element->x + i)  + (sp->y - collision_element->y + j) * collision_element->img.width) * num_bytes;

      color = 0;
      for (unsigned byte = 0; byte < num_bytes; byte++) {
        color += *(collision_map + collision_element_pos + byte) << byte * 8;
      }
      
      if (draw_pixel(sp->x + i, sp->y + j, color))
        return 1;
    }
  }
  sp->changed = true;
  return 0;
}

int (clear_sprite_left)(Sprite* sp, Sprite* collision_element, int bg_delta_x, int element_delta_x){
  unsigned num_bytes = get_bytesperpixel();
  uint32_t color;
  int collision_element_pos;
  uint8_t* collision_map = collision_element->map;

  for (int j = 0; j < sp->img.height; j++) {
    
    for (int i = 0; i < element_delta_x; i++) {
      collision_element_pos = ((sp->x + bg_delta_x - collision_element->x + i)  + (sp->y - collision_element->y + j) * collision_element->img.width) * num_bytes;

      color = 0;
      for (unsigned byte = 0; byte < num_bytes; byte++) {
        color += *(collision_map + collision_element_pos + byte) << byte * 8;
      }
      
      if (draw_pixel(sp->x + bg_delta_x + i, sp->y + j, color))
        return 1;
    }
  }
  sp->changed = true;
  return 0;
}

int (clear_sprite_right)(Sprite* sp, Sprite* collision_element, int bg_delta_x, int element_delta_x){
  unsigned num_bytes = get_bytesperpixel();
  uint32_t color;
  int collision_element_pos;
  uint8_t* collision_map = collision_element->map;

  for (int j = 0; j < sp->img.height; j++) {
    
    for (int i = 0; i < element_delta_x; i++) {
      collision_element_pos = ((sp->x  - collision_element->x + i)  + (sp->y - collision_element->y + j) * collision_element->img.width) * num_bytes;

      color = 0;
      for (unsigned byte = 0; byte < num_bytes; byte++) {
        color += *(collision_map + collision_element_pos + byte) << byte * 8;
      }
      
      if (draw_pixel(sp->x + i, sp->y + j, color))
        return 1;
    }
  }
  sp->changed = true;
  return 0;
}
int (clear_sprite_top_left)(Sprite* sp, Sprite* collision_element, int bg_delta_x, int element_delta_x, int bg_delta_y, int element_delta_y){
  unsigned num_bytes = get_bytesperpixel();
  uint32_t color;
  int collision_element_pos;
  uint8_t* collision_map = collision_element->map;

  for (int j = 0; j < element_delta_y; j++) {
    
    for (int i = 0; i < element_delta_x; i++) {
      collision_element_pos = ((sp->x + bg_delta_x - collision_element->x + i)  + (sp->y + bg_delta_y - collision_element->y + j) * collision_element->img.width) * num_bytes;

      color = 0;
      for (unsigned byte = 0; byte < num_bytes; byte++) {
        color += *(collision_map + collision_element_pos + byte) << byte * 8;
      }
      
      if (draw_pixel(sp->x + bg_delta_x + i, sp->y + bg_delta_y + j, color))
        return 1;
    }
  }
  sp->changed = true;
  return 0;
}

int (clear_sprite_top_right)(Sprite* sp, Sprite* collision_element, int bg_delta_x, int element_delta_x, int bg_delta_y, int element_delta_y){
  unsigned num_bytes = get_bytesperpixel();
  uint32_t color;
  int collision_element_pos;
  uint8_t* collision_map = collision_element->map;

  for (int j = 0; j < element_delta_y; j++) {
    
    for (int i = 0; i < element_delta_x; i++) {
      collision_element_pos = ((sp->x  - collision_element->x + i)  + (sp->y + bg_delta_y - collision_element->y + j) * collision_element->img.width) * num_bytes;

      color = 0;
      for (unsigned byte = 0; byte < num_bytes; byte++) {
        color += *(collision_map + collision_element_pos + byte) << byte * 8;
      }
      
      if (draw_pixel(sp->x + i, sp->y + bg_delta_y + j, color))
        return 1;
    }
  }
  sp->changed = true;
  return 0;
}

int (clear_sprite_bottom_left)(Sprite* sp, Sprite* collision_element, int bg_delta_x, int element_delta_x, int bg_delta_y, int element_delta_y){
  unsigned num_bytes = get_bytesperpixel();
  uint32_t color;
  int collision_element_pos;
  uint8_t* collision_map = collision_element->map;

  for (int j = 0; j < element_delta_y; j++) {
    
    for (int i = 0; i < element_delta_x; i++) {
      collision_element_pos = ((sp->x + bg_delta_x - collision_element->x + i)  + (sp->y - collision_element->y + j) * collision_element->img.width) * num_bytes;

      color = 0;
      for (unsigned byte = 0; byte < num_bytes; byte++) {
        color += *(collision_map + collision_element_pos + byte) << byte * 8;
      }
      
      if (draw_pixel(sp->x + bg_delta_x + i, sp->y + j, color))
        return 1;
    }
  }
  sp->changed = true;
  return 0;
}

int (clear_sprite_bottom_right)(Sprite* sp, Sprite* collision_element, int bg_delta_x, int element_delta_x, int bg_delta_y, int element_delta_y){
  unsigned num_bytes = get_bytesperpixel();
  uint32_t color;
  int collision_element_pos;
  uint8_t* collision_map = collision_element->map;

  for (int j = 0; j < element_delta_y; j++) {
    
    for (int i = 0; i < element_delta_x; i++) {
      collision_element_pos = ((sp->x  - collision_element->x + i)  + (sp->y  - collision_element->y + j) * collision_element->img.width) * num_bytes;

      color = 0;
      for (unsigned byte = 0; byte < num_bytes; byte++) {
        color += *(collision_map + collision_element_pos + byte) << byte * 8;
      }
      
      if (draw_pixel(sp->x + i, sp->y + j, color))
        return 1;
    }
  }
  sp->changed = true;
  return 0;
}

bool (check_collision_up)(Mouse mouse, Sprite element){
    Sprite mouse_sprite = get_mouse();
    if((mouse.y < element.y && mouse.y + mouse_sprite.img.height <= element.y + element.img.height &&
            mouse.y + mouse_sprite.img.height >= element.y) &&
            (mouse.x >= element.x && mouse.x + mouse_sprite.img.width < element.x + element.img.width))
    {
        int bg_delta_y = element.y - mouse.y;
        int element_delta_y = mouse_sprite.img.height - bg_delta_y;
        clear_sprite_up(&mouse_sprite, &element, bg_delta_y, element_delta_y);
        return true;
    }

    return false;
}

bool (check_collision_down)(Mouse mouse, Sprite element){
    Sprite mouse_sprite = get_mouse();
    if((mouse.y >= element.y && mouse.y + mouse_sprite.img.height > element.y + element.img.height &&
            mouse.y <= element.y + element.img.height) &&
            (mouse.x >= element.x && mouse.x + mouse_sprite.img.width < element.x + element.img.width))
    {
        int bg_delta_y = mouse.y + mouse_sprite.img.height - element.y - element.img.height;
        int element_delta_y = mouse_sprite.img.height - bg_delta_y;
        clear_sprite_down(&mouse_sprite, &element, bg_delta_y, element_delta_y);
        return true;
    }

    return false;
}


bool (check_collision_left)(Mouse mouse, Sprite element){
    Sprite mouse_sprite = get_mouse();
    if((mouse.y >= element.y && mouse.y + mouse_sprite.img.height < element.y + element.img.height) &&
            (mouse.x < element.x && mouse.x + mouse_sprite.img.width <= element.x + element.img.width
            && mouse.x + mouse_sprite.img.width >= element.x))
    {
        int bg_delta_x = element.x - mouse.x;
        int element_delta_x = mouse_sprite.img.width - bg_delta_x;
        clear_sprite_left(&mouse_sprite, &element, bg_delta_x, element_delta_x);
        return true;
    }
    return false;
}

bool (check_collision_right)(Mouse mouse, Sprite element){
    Sprite mouse_sprite = get_mouse();
    if((mouse.y >= element.y && mouse.y + mouse_sprite.img.height < element.y + element.img.height) &&
            (mouse.x + mouse_sprite.img.width > element.x + element.img.width && mouse.x <= element.x + element.img.width
            && mouse.x >= element.x))
    {
        int bg_delta_x = mouse.x + mouse_sprite.img.width - element.x  - element.img.width ;
        int element_delta_x = mouse_sprite.img.width - bg_delta_x;
        clear_sprite_right(&mouse_sprite, &element, bg_delta_x, element_delta_x);
        return true;
    }
    return false;
}

bool (check_collision_inside)(Mouse mouse, Sprite element){
    Sprite mouse_sprite = get_mouse();
    if((mouse.y >= element.y && mouse.y + mouse_sprite.img.height <= element.y + element.img.height) &&
       (mouse.x >= element.x && mouse.x + mouse_sprite.img.width <= element.x + element.img.width))
    {
        clear_sprite(&mouse_sprite, &element);
        return true;
    }

    return false;
}

bool (check_collision_top_left)(Mouse mouse, Sprite element){
    Sprite mouse_sprite = get_mouse();
    if((mouse.y < element.y && mouse.y + mouse_sprite.img.height <= element.y + element.img.height &&
            mouse.y + mouse_sprite.img.height >= element.y) &&
            (mouse.x < element.x && mouse.x + mouse_sprite.img.width <= element.x + element.img.width
            && mouse.x + mouse_sprite.img.width >= element.x))
    {
        int bg_delta_y = element.y - mouse.y;
        int element_delta_y = mouse_sprite.img.height - bg_delta_y;
        int bg_delta_x = element.x - mouse.x;
        int element_delta_x = mouse_sprite.img.width - bg_delta_x;
        clear_sprite_top_left(&mouse_sprite, &element, bg_delta_x, element_delta_x, bg_delta_y, element_delta_y);
        return true;
    }
    return false;
}

bool (check_collision_top_right)(Mouse mouse, Sprite element){
    Sprite mouse_sprite = get_mouse();
    if((mouse.y < element.y && mouse.y + mouse_sprite.img.height <= element.y + element.img.height &&
            mouse.y + mouse_sprite.img.height >= element.y) &&
            (mouse.x + mouse_sprite.img.width > element.x + element.img.width && mouse.x <= element.x + element.img.width
            && mouse.x >= element.x))
    {
        int bg_delta_y = element.y - mouse.y;
        int element_delta_y = mouse_sprite.img.height - bg_delta_y;
        int bg_delta_x = mouse.x + mouse_sprite.img.width - element.x  - element.img.width;
        int element_delta_x = mouse_sprite.img.width - bg_delta_x;
        clear_sprite_top_right(&mouse_sprite, &element, bg_delta_x, element_delta_x, bg_delta_y, element_delta_y);
        return true;
    }
    return false;
}

bool (check_collision_bottom_left)(Mouse mouse, Sprite element){
    Sprite mouse_sprite = get_mouse();
    if((mouse.y >= element.y && mouse.y + mouse_sprite.img.height > element.y + element.img.height &&
            mouse.y <= element.y + element.img.height) &&
        (mouse.x < element.x && mouse.x + mouse_sprite.img.width <= element.x + element.img.width
        && mouse.x + mouse_sprite.img.width >= element.x))
    {
        int bg_delta_x = element.x - mouse.x;
        int element_delta_x = mouse_sprite.img.width - bg_delta_x;
        int bg_delta_y = mouse.y + mouse_sprite.img.height - element.y - element.img.height;
        int element_delta_y = mouse_sprite.img.height - bg_delta_y;
        clear_sprite_bottom_left(&mouse_sprite, &element, bg_delta_x, element_delta_x, bg_delta_y, element_delta_y);
        return true;
    }
    return false;
}

bool (check_collision_bottom_right)(Mouse mouse, Sprite element){
    Sprite mouse_sprite = get_mouse();
    if((mouse.y >= element.y && mouse.y + mouse_sprite.img.height > element.y + element.img.height &&
            mouse.y <= element.y + element.img.height) &&
        (mouse.x + mouse_sprite.img.width > element.x + element.img.width && mouse.x <= element.x + element.img.width
        && mouse.x >= element.x))
    {
        int bg_delta_x = mouse.x + mouse_sprite.img.width - element.x  - element.img.width ;
        int element_delta_x = mouse_sprite.img.width - bg_delta_x;
        int bg_delta_y = mouse.y + mouse_sprite.img.height - element.y - element.img.height;
        int element_delta_y = mouse_sprite.img.height - bg_delta_y;
        clear_sprite_bottom_right(&mouse_sprite, &element, bg_delta_x, element_delta_x, bg_delta_y, element_delta_y);
        return true;
    }
    return false;
}

 bool check_collision_background(Mouse mouse){
    Sprite background = get_background();
    if(check_collision_inside(mouse, background)) return true;
    return false;
}


 bool check_collision_button_food(Mouse mouse){
    Sprite button_food = get_button_food();
    if(check_collision_up(mouse, button_food)) return true;
    if(check_collision_down(mouse, button_food)) return true;
    if(check_collision_left(mouse, button_food)) return true;
    if(check_collision_right(mouse, button_food)) return true;
    if(check_collision_inside(mouse, button_food)) return true;
    if(check_collision_bottom_left(mouse, button_food)) return true;
    if(check_collision_bottom_right(mouse, button_food)) return true;
    if(check_collision_top_left(mouse, button_food)) return true;
    if(check_collision_top_right(mouse, button_food)) return true;
    return false;
}


 bool check_collision_button_bath(Mouse mouse){
    Sprite button_bath = get_button_bath();
    if(check_collision_up(mouse, button_bath)) return true;
    if(check_collision_down(mouse, button_bath)) return true;
    if(check_collision_left(mouse, button_bath)) return true;
    if(check_collision_right(mouse, button_bath)) return true;
    if(check_collision_inside(mouse, button_bath)) return true;
    if(check_collision_bottom_left(mouse, button_bath)) return true;
    if(check_collision_bottom_right(mouse, button_bath)) return true;
    if(check_collision_top_left(mouse, button_bath)) return true;
    if(check_collision_top_right(mouse, button_bath)) return true;
    return false;
}

 bool check_collision_button_fun(Mouse mouse){
    Sprite button_fun = get_button_fun();
    if(check_collision_up(mouse, button_fun)) return true;
    if(check_collision_down(mouse, button_fun)) return true;
    if(check_collision_left(mouse, button_fun)) return true;
    if(check_collision_right(mouse, button_fun)) return true;
    if(check_collision_inside(mouse, button_fun)) return true;
    if(check_collision_bottom_left(mouse, button_fun)) return true;
    if(check_collision_bottom_right(mouse, button_fun)) return true;
    if(check_collision_top_left(mouse, button_fun)) return true;
    if(check_collision_top_right(mouse, button_fun)) return true;
    return false;
}

 bool check_collision_button_love(Mouse mouse){
    Sprite button_love = get_button_love();
    if(check_collision_up(mouse, button_love)) return true;
    if(check_collision_down(mouse, button_love)) return true;
    if(check_collision_left(mouse, button_love)) return true;
    if(check_collision_right(mouse, button_love)) return true;
    if(check_collision_inside(mouse, button_love)) return true;
    return false;
}

 bool check_collision_button_sleep(Mouse mouse){
    Sprite button_sleep = get_button_sleep();
    if(check_collision_up(mouse, button_sleep)) return true;
    if(check_collision_down(mouse, button_sleep)) return true;
    if(check_collision_left(mouse, button_sleep)) return true;
    if(check_collision_right(mouse, button_sleep)) return true;
    if(check_collision_inside(mouse, button_sleep)) return true;
    if(check_collision_bottom_left(mouse, button_sleep)) return true;
    if(check_collision_bottom_right(mouse, button_sleep)) return true;
    if(check_collision_top_left(mouse, button_sleep)) return true;
    if(check_collision_top_right(mouse, button_sleep)) return true;
    return false;
}

 bool check_collision_arrow(Mouse mouse){
    Sprite arrow = get_arrow();
    if(check_collision_up(mouse, arrow)) return true;
    if(check_collision_down(mouse, arrow)) return true;
    if(check_collision_left(mouse, arrow)) return true;
    if(check_collision_right(mouse, arrow)) return true;
    if(check_collision_inside(mouse, arrow)) return true;
    if(check_collision_bottom_left(mouse, arrow)) return true;
    if(check_collision_bottom_right(mouse, arrow)) return true;
    if(check_collision_top_left(mouse, arrow)) return true;
    if(check_collision_top_right(mouse, arrow)) return true;
    return false;
}

 bool check_collision_hour_unit(Mouse mouse){
    Sprite hour_unit = get_hour_unit();
    if(check_collision_up(mouse, hour_unit)) return true;
    if(check_collision_down(mouse, hour_unit)) return true;
    if(check_collision_left(mouse, hour_unit)) return true;
    if(check_collision_right(mouse, hour_unit)) return true;
    if(check_collision_inside(mouse, hour_unit)) return true;
    if(check_collision_bottom_left(mouse, hour_unit)) return true;
    if(check_collision_bottom_right(mouse, hour_unit)) return true;
    if(check_collision_top_left(mouse, hour_unit)) return true;
    if(check_collision_top_right(mouse, hour_unit)) return true;
    return false;
 }

 bool check_collision_hour_ten(Mouse mouse){
    Sprite hour_ten = get_hour_ten();
    if(check_collision_up(mouse, hour_ten)) return true;
    if(check_collision_down(mouse, hour_ten)) return true;
    if(check_collision_left(mouse, hour_ten)) return true;
    if(check_collision_right(mouse, hour_ten)) return true;
    if(check_collision_inside(mouse, hour_ten)) return true;
    if(check_collision_bottom_left(mouse, hour_ten)) return true;
    if(check_collision_bottom_right(mouse, hour_ten)) return true;
    if(check_collision_top_left(mouse, hour_ten)) return true;
    if(check_collision_top_right(mouse, hour_ten)) return true;
    return false;
}

 bool check_collision_min_unit(Mouse mouse){
    Sprite min_unit = get_min_unit();
    if(check_collision_up(mouse, min_unit)) return true;
    if(check_collision_down(mouse, min_unit)) return true;
    if(check_collision_left(mouse, min_unit)) return true;
    if(check_collision_right(mouse, min_unit)) return true;
    if(check_collision_inside(mouse, min_unit)) return true;
    if(check_collision_bottom_left(mouse, min_unit)) return true;
    if(check_collision_bottom_right(mouse, min_unit)) return true;
    if(check_collision_top_left(mouse, min_unit)) return true;
    if(check_collision_top_right(mouse, min_unit)) return true;
    return false;
}

 bool check_collision_min_ten(Mouse mouse){
    Sprite min_ten = get_min_ten();
    if(check_collision_up(mouse, min_ten)) return true;
    if(check_collision_down(mouse, min_ten)) return true;
    if(check_collision_left(mouse, min_ten)) return true;
    if(check_collision_right(mouse, min_ten)) return true;
    if(check_collision_inside(mouse, min_ten)) return true;
    if(check_collision_bottom_left(mouse, min_ten)) return true;
    if(check_collision_bottom_right(mouse, min_ten)) return true;
    if(check_collision_top_left(mouse, min_ten)) return true;
    if(check_collision_top_right(mouse, min_ten)) return true;
    return false;
}

 bool check_collision_dois_pontos(Mouse mouse){
    Sprite dois_pontos = get_dois_pontos();
    if(check_collision_up(mouse, dois_pontos)) return true;
    if(check_collision_down(mouse, dois_pontos)) return true;
    if(check_collision_left(mouse, dois_pontos)) return true;
    if(check_collision_right(mouse, dois_pontos)) return true;
    if(check_collision_inside(mouse, dois_pontos)) return true;
    if(check_collision_bottom_left(mouse, dois_pontos)) return true;
    if(check_collision_bottom_right(mouse, dois_pontos)) return true;
    if(check_collision_top_left(mouse, dois_pontos)) return true;
    if(check_collision_top_right(mouse, dois_pontos)) return true;
    return false;
}

bool check_collision_violetchi(Mouse mouse){
    Sprite violetchi = get_violetchi();
    if(check_collision_up(mouse, violetchi)) return true;
    if(check_collision_down(mouse, violetchi)) return true;
    if(check_collision_left(mouse, violetchi)) return true;
    if(check_collision_right(mouse, violetchi)) return true;
    if(check_collision_inside(mouse, violetchi)) return true;
    if(check_collision_bottom_left(mouse, violetchi)) return true;
    if(check_collision_bottom_right(mouse, violetchi)) return true;
    if(check_collision_top_left(mouse, violetchi)) return true;
    if(check_collision_top_right(mouse, violetchi)) return true;
    return false;
}

void check_any_collision(Mouse mouse){
    check_collision_background( mouse);
    check_collision_button_food( mouse);
    check_collision_button_bath( mouse);
    check_collision_button_fun( mouse);
    check_collision_button_love( mouse);
    check_collision_button_sleep( mouse);
    check_collision_arrow( mouse);
    check_collision_hour_unit( mouse);
    check_collision_hour_ten( mouse);
    check_collision_min_unit( mouse);
    check_collision_min_ten( mouse);
    check_collision_dois_pontos( mouse);
    check_collision_violetchi( mouse);
}

