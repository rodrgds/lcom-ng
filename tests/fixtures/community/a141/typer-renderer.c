#include "game.h"
#include <stdint.h>
static void *guarded_malloc(size_t n);
static void guarded_free(void *p);
#define malloc guarded_malloc
#define free guarded_free
#include LC_SOURCE
#undef malloc
#undef free
#include <assert.h>
#include <string.h>
#include <limits.h>
static int elapsed=60;
static unsigned char *allocation;static size_t allocation_size;static int freed;
static void *guarded_malloc(size_t n){assert(!allocation);allocation=malloc(n+32);allocation_size=n;memset(allocation+n,0xD7,32);return allocation;}
static void guarded_free(void *p){assert(p==allocation);for(size_t i=0;i<32;i++)assert(allocation[allocation_size+i]==0xD7);freed=1;free(p);allocation=NULL;}
static char rendered[80];
static char letters[80];static int count;
int rtc_get_time_elapsed(void){return elapsed;}
int draw_sentence(char *s,int x,int y,uint32_t color){(void)x;(void)y;(void)color;strcpy(rendered,s);return 0;}
int draw_char(char letter,int x,int y,uint32_t color){(void)x;(void)y;(void)color;letters[count++]=letter;return 0;}
int vg_draw_rectangle(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint32_t color){return 0;}
int16_t video_get_h_res(void){return 1024;}
int get_sentence_width(char *sentence){assert(sentence);return strlen(sentence)*20;}
int main(int argc,char **argv){
 Game game={0};
 if(argc>1 && strcmp(argv[1],"text")==0){
  game.text="hello world";game.text_size=11;base_y=TEXT_Y_START;
  draw_text(&game);assert(count==11);assert(memcmp(letters,"hello world",11)==0);
  if(allocation){for(size_t i=0;i<32;i++)assert(allocation[allocation_size+i]==0xD7);}
  assert(freed);
 }else{
  game.player_position=300;draw_wpm(&game);assert(game.wpm==60);assert(strcmp(rendered,"60 wpm")==0);
  game.player_position=INT_MAX;elapsed=60;draw_wpm(&game);assert(game.wpm==429496729);assert(strcmp(rendered,"429496729 wpm")==0);
 }
 return 0;
}
