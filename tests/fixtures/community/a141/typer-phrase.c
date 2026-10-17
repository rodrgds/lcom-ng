#include "game.h"
#include <assert.h>
static FILE *fixture_open(const char *path,const char *mode){FILE *f=tmpfile();assert(f);fputs("alpha beta\n",f);rewind(f);return f;}
static int fixture_rand(void){return 0;}
#undef fopen
#define fopen fixture_open
#define rand fixture_rand
#include GAME_SOURCE
#undef fopen
#undef rand
Button button_create(int x,int y,char *text){return (Button){0};}
int16_t video_get_h_res(void){return 800;}
int main(void){Game game={0};game_create(&game);assert(strcmp(game.text,"alpha beta")==0);assert(game.text_size==10);return 0;}
