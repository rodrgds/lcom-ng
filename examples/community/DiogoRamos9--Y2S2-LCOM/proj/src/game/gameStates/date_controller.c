#include <lcom/lcf.h>

#include "date_controller.h"

extern Date *date;
extern Sprite *numbers[10];

void draw_date_time() {
    int x2 = 620;
    draw_sprite(numbers[date->hour / 10], x2, 500);
    x2 += numbers[date->hour / 10]->width;
    draw_sprite(numbers[date->hour % 10], x2, 500);
    x2 += numbers[date->hour % 10]->width;
    draw_sprite(special_chars[1], x2, 500);
    x2 += special_chars[1]->width;
    draw_sprite(numbers[date->min / 10], x2, 500);
    x2 += numbers[date->min / 10]->width;
    draw_sprite(numbers[date->min % 10], x2, 500);

    int x = 520;
    draw_sprite(numbers[date->day / 10], x, 550);
    x += numbers[date->day / 10]->width;
    draw_sprite(numbers[date->day % 10], x, 550);
    x += numbers[date->day % 10]->width;
    draw_sprite(special_chars[0], x, 550);
    x += special_chars[0]->width;
    draw_sprite(numbers[date->month / 10], x, 550);
    x += numbers[date->month / 10]->width;
    draw_sprite(numbers[date->month % 10], x, 550);
    x += numbers[date->month % 10]->width;
    draw_sprite(special_chars[0], x, 550);
    x += special_chars[0]->width;
    draw_sprite(numbers[date->year / 10], x, 550);
    x += numbers[date->year / 10]->width;
    draw_sprite(numbers[date->year % 10], x, 550);
    x += numbers[date->year % 10]->width;
}
