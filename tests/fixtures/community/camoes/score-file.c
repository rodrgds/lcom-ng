#include "highscores.h"
#undef fopen
FILE *__wrap_lcom_compat_fopen(const char *path,const char *mode){if(strcmp(path,"/home/lcom/labs/proj/src/highscores.txt"))abort();return fopen("/fixture.txt",mode);}
int main(void){
 Game g={0};highScore rows[5]={0};g.highscores=rows;abc=calloc(nLetters,sizeof(letter));
 for(unsigned i=0;i<26;i++)abc[i].letter='a'+i;
 for(unsigned i=0;i<5;i++){rows[i].name.nameAr=calloc(11,1);rows[i].name.nameXpm=calloc(10,sizeof(letter));}
 if(get_highscores(&g)||g.NUMBEROFSCORES!=5)abort();
 const int speeds[]={6,6,4,4,3};const char*names[]={"maria","camoes","player","eca","joao"};
 for(unsigned i=0;i<5;i++)if(rows[i].speed!=speeds[i]||strcmp(rows[i].name.nameAr,names[i])){fprintf(stderr,"Original saved-file row%u misaligned: speed%d, name%s\n",i,rows[i].speed,rows[i].name.nameAr);return 1;}
 puts("Original five-record file parsed across a name line longer than its declared display length");return 0;
}
