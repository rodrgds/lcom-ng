#include "game.h"
#include "highscores.h"
#include <assert.h>
static void *owned[128];static size_t sizes[128];static unsigned count;
void *checked_malloc(size_t n){void*p=malloc(n+64);assert(p&&count<128);memset(p,0xA5,n+64);((char*)p)[n+63]=0;owned[count]=p;sizes[count++]=n;return p;}
static size_t capacity(void*p){for(unsigned i=0;i<count;i++)if(owned[i]==p)return sizes[i];abort();}
void loadLetters(void){abc=calloc(nLetters,sizeof(letter));for(unsigned i=0;i<10;i++){abc[i].letter='A'+i;abc[i].makeCode=10+i;}}
void loadSentences(void){}
unsigned get_hres(void){return 1024;}unsigned get_vres(void){return 768;}
int graphics_mode(uint16_t mode){assert(mode==0x117);return 0;}
uint8_t*xpm_load(xpm_map_t map,enum xpm_image_type type,xpm_image_t*img){(void)map;(void)type;img->width=80;img->height=40;return NULL;}
int __wrap_get_highscores(Game*g){g->NUMBEROFSCORES=0;return 0;}
int __wrap_gameStateHandler(void){
 if(capacity(game->name->nameAr)<11){fputs("Ten-letter player name lacks terminator capacity\n",stderr);exit(1);}
 for(unsigned i=0;i<5;i++)if(capacity(game->highscores[i].name.nameAr)<11){fputs("Ten-letter saved name lacks terminator capacity\n",stderr);exit(1);}
 for(unsigned length=3;length<=10;length+=7){
  memset(game->name->nameAr,0xA5,11);game->NUMBEROFSCORES=0;game->name->nameSize=length;
  for(unsigned i=0;i<length;i++)game->name->nameXpm[i]=abc[i];
  game->result.speed=20;game->state.mode=SAVE_SCORE;game->state.start=false;game->mc.lbPressed=true;game->mc.mouseX=670;game->mc.mouseY=290;
  back_to_menu();
  if(game->name->nameAr[length]!='\0'){fputs("Saved player name is not terminated at its actual length\n",stderr);exit(1);}
  if(game->NUMBEROFSCORES!=1||game->highscores[0].name.nameSize!=length||memcmp(game->highscores[0].name.nameAr,"ABCDEFGHIJ",length)||game->highscores[0].name.nameAr[length]!='\0')abort();
 }
 puts("Actual game constructor and save route preserve3/10-letter names and terminators");return 1;
}
int main(void){assert(gameStart()==1);return 0;}
