#include <lcom/lcf.h>
#include <math.h>
#include "sprite.h"
#include "game.h"

extern Sprite *number;
extern Sprite *mini_number;
extern xpm_image_t numbers[10];
extern xpm_image_t numbers_mini[11];
extern int points;
extern uint8_t day;
extern uint8_t month;
extern uint8_t year;
int highscores[3];
int highscoresdays[3];
int highscoresmonths[3];
int highscoresyears[3];

void show_points(){
    int n = points;
    number->xi = 7;
    int pos = 7;
    int n_digits = (int)floor(log(n)/log(10));
    int nums[n];
    for(int i = 0;i < n_digits;i++){
        nums[i] = n/(int)pow(10,n_digits-i);
        n -= (int)pow(10,n_digits-i)*nums[i];
    }
    for(int i = 0; i < n_digits; i++){
        number->map = (uint32_t*)numbers[nums[i]].bytes;
        number->xi = pos;
        pos += 40;
        if (nums[i] == 1) {
            number->width = numbers[1].width;
            draw_sprite(number);
            number->width = 40;
        }
        else draw_sprite(number);
    }
    int last = points - ((points/10) * 10);
    number->map =(uint32_t*)numbers[last].bytes;
    number->width  = numbers[last].width;
    number->xi = pos;
    draw_sprite(number);
}

void deal_with_points(){
    FILE *ptr;
    ptr = fopen("/home/lcom/labs/g10/proj/src/HighScores.txt","r");
    for (int i = 0; i < 3; i++){
        fscanf(ptr,"%d %d %d %d \n", &highscores[i], &highscoresdays[i], &highscoresmonths[i], &highscoresyears[i]);
    }
    fclose(ptr);
    if (points > highscores[0]){
        highscores[2] = highscores[1];
        highscoresyears[2] = highscoresyears[1];
        highscoresmonths[2] = highscoresmonths[1];
        highscoresdays[2] = highscoresdays[1];
        highscores[1] = highscores[0];
        highscoresyears[1] = highscoresyears[0];
        highscoresmonths[1] = highscoresmonths[0];
        highscoresdays[1] = highscoresdays[0];
        highscores[0] = points;
        highscoresyears[0] = year;
        highscoresmonths[0] = month;
        highscoresdays[0] = day;
    }
    else if(points > highscores[1]){
        highscores[2] = highscores[1];
        highscoresyears[2] = highscoresyears[1];
        highscoresmonths[2] = highscoresmonths[1];
        highscoresdays[2] = highscoresdays[1];
        highscores[1] = points;
        highscoresyears[1] = year;
        highscoresmonths[1] = month;
        highscoresdays[1] = day;
    }
    else if(points >highscores[2]){
        highscores[2] = points;
        highscoresyears[2] = year;
        highscoresmonths[2] = month;
        highscoresdays[2] = day;
    }
    ptr = fopen("/home/lcom/labs/g10/proj/src/HighScores.txt","w");
    for (int i = 0; i < 3; i++){
        fprintf(ptr,"%d %d %d %d \n", highscores[i], highscoresdays[i], highscoresmonths[i], highscoresyears[i]);
    }
    fclose(ptr);
}

void show_score(){
    int n = points;
    int pos = 590;
    int n_digits = (int)floor(log(n)/log(10));
    int nums[n];
    for(int i = 0;i < n_digits;i++){
        nums[i] = n/(int)pow(10,n_digits-i);
        n -= (int)pow(10,n_digits-i)*nums[i];
    }
    mini_number->yi = 230;
    for(int i = 0; i < n_digits; i++){
        mini_number->map = (uint32_t*)numbers_mini[nums[i]].bytes;
        mini_number->xi = pos;
        draw_sprite(mini_number);
        pos += 20;
    }
    int last = points - ((points/10) * 10);
    mini_number->map =(uint32_t*)numbers_mini[last].bytes;
    mini_number->width  = numbers_mini[last].width;
    mini_number->xi = pos;
    draw_sprite(mini_number);
    for (int i = 0; i < 3; i++){
        int n = highscores[i];
        int pos = 430 ;
        int n_digits = (int)floor(log(n)/log(10));
        int nums1[highscores[i]];
        for(int i = 0;i < n_digits;i++){
            nums1[i] = n/(int)pow(10,n_digits-i);
            n -= (int)pow(10,n_digits-i)*nums1[i];
        }
        mini_number->yi = 324 + i * 35;
        for(int i = 0; i < n_digits; i++){
            mini_number->map = (uint32_t*)numbers_mini[nums1[i]].bytes;
            mini_number->xi = pos;
            draw_sprite(mini_number);
            pos += 20;
        }
        last = highscores[i] - ((highscores[i]/10) * 10);
        mini_number->map =(uint32_t*)numbers_mini[last].bytes;
        mini_number->width  = numbers_mini[last].width;
        mini_number->xi = pos;
        draw_sprite(mini_number);
        n = highscoresdays[i];
        pos = 612;
        n_digits = (int)floor(log(n)/log(10));
        int nums2[highscoresdays[i]];
        for(int i = 0;i < n_digits;i++){
            nums2[i] = n/(int)pow(10,n_digits-i);
            n -= (int)pow(10,n_digits-i)*nums2[i];
        }
        mini_number->yi = 324 + i * 35;
        for(int i = 0; i < n_digits; i++){
            mini_number->map = (uint32_t*)numbers_mini[nums2[i]].bytes;
            mini_number->xi = pos;
            draw_sprite(mini_number);
            pos += 20;
        }
        last = highscoresdays[i] - ((highscoresdays[i]/10) * 10);
        mini_number->map =(uint32_t*)numbers_mini[last].bytes;
        mini_number->width  = numbers_mini[last].width;
        mini_number->xi = pos;
        draw_sprite(mini_number);
        mini_number->map =(uint32_t*)numbers_mini[10].bytes;
        mini_number->width  = numbers_mini[10].width;
        mini_number->xi = 651;
        draw_sprite(mini_number);
        n = highscoresmonths[i];
        pos = 661;
        n_digits = (int)floor(log(n)/log(10));
        int nums3[highscoresmonths[i]];
        for(int i = 0;i < n_digits;i++){
            nums3[i] = n/(int)pow(10,n_digits-i);
            n -= (int)pow(10,n_digits-i)*nums3[i];
        }
        mini_number->yi = 324 + i * 35;
        for(int i = 0; i < n_digits; i++){
            mini_number->map = (uint32_t*)numbers_mini[nums3[i]].bytes;
            mini_number->xi = pos;
            draw_sprite(mini_number);
            pos += 20;
        }
        last = highscoresmonths[i] - ((highscoresmonths[i]/10) * 10);
        mini_number->map =(uint32_t*)numbers_mini[last].bytes;
        mini_number->width  = numbers_mini[last].width;
        mini_number->xi = pos;
        draw_sprite(mini_number);
        mini_number->map =(uint32_t*)numbers_mini[10].bytes;
        mini_number->width  = numbers_mini[10].width;
        mini_number->xi = 700;
        draw_sprite(mini_number);
        n = highscoresyears[i];
        pos = 714 ;
        n_digits = (int)floor(log(n)/log(10));
        int nums4[highscoresyears[i]];
        for(int i = 0;i < n_digits;i++){
            nums4[i] = n/(int)pow(10,n_digits-i);
            n -= (int)pow(10,n_digits-i)*nums4[i];
        }
        mini_number->yi = 324 + i * 35;
        for(int i = 0; i < n_digits; i++){
            mini_number->map = (uint32_t*)numbers_mini[nums4[i]].bytes;
            mini_number->xi = pos;
            draw_sprite(mini_number);
            pos += 20;
        }
        last = highscoresyears[i] - ((highscoresyears[i]/10) * 10);
        mini_number->map =(uint32_t*)numbers_mini[last].bytes;
        mini_number->width  = numbers_mini[last].width;
        mini_number->xi = pos;
        draw_sprite(mini_number);
    }

}
