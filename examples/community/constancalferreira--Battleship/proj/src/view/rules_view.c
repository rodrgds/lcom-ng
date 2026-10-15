#include "rules_view.h"

extern Sprite* rules_content;  
extern Sprite* rules_text;
extern Sprite* go_back;


void draw_rules(uint8_t *buffer) {
    fill_screen(LILAC, buffer);
    
    uint16_t option_x = (mode_info.XResolution - rules->width) / 2;
    uint16_t option_y = 30; 
    draw_sprite(rules, option_x, option_y, buffer);
    
    uint16_t text_x = (mode_info.XResolution - rules_text->width) / 2;
    uint16_t text_y = option_y + rules->height + 20;
    draw_sprite(rules_text, text_x, text_y, buffer);


    draw_sprite(go_back, 600, 540, buffer);
}

