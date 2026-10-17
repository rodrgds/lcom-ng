#include "game.h"
#include GAME_OVER_SOURCE
#include <assert.h>
#include <limits.h>
static char speed[40],accuracy[40];static unsigned calls;
int16_t video_get_h_res(void){return 800;}
int get_sentence_width(char*text){assert(text);return strlen(text)*30;}
int draw_sentence(char*text,int x,int y,uint32_t color){(void)x;(void)y;(void)color;if(calls==1)strcpy(speed,text);if(calls==3)strcpy(accuracy,text);calls++;return 0;}
int main(int argc,char**argv){Game game={0};game.time_elapsed=60;game.text_size=43;game.player_position=300;
 if(argc>1&&!strcmp(argv[1],"wpm")){game.player_position=INT_MAX;game.typo_count=1;}
 game_over_draw(&game);assert(calls==4);assert(!strcmp(speed,game.player_position==INT_MAX?"429496729":"60"));assert(!strcmp(accuracy,game.typo_count?"97%":"100%"));return 0;}
