#include <lcom/lcf.h>
#include "highscores.h"
#include "rtc_macros.h"

int get_highscores(Game * game)
{
  FILE* fp;

  fp = fopen("/home/lcom/labs/proj/src/highscores.txt", "r");

  if (fp == NULL){
    printf("Highscore file not found\n");
    return 1;
  } 

  fscanf(fp, "%d ", &game->NUMBEROFSCORES);
  char buffer[100];

  for(unsigned int i = 0; i < game->NUMBEROFSCORES; i++) {

    highScore* highscore = &game->highscores[i];

    fscanf(fp, "%d %d %d %d %d %d %d", &highscore->date.hour, &highscore->date.minutes, &highscore->date.day, &highscore->date.month, &highscore->date.year, &highscore->speed, &highscore->name.nameSize);

    fgets(buffer, 100, fp);
    fgets(highscore->name.nameAr, highscore->name.nameSize + 1, fp);
    fgets(buffer, sizeof(buffer), fp); // consume the rest of the name line
  }

  //stores de xpm array for evry name
  for(unsigned int j = 0; j < game->NUMBEROFSCORES; j++){
    for(unsigned int l = 0; l < game->highscores[j].name.nameSize; l++){
      for(unsigned int k = 0; k < nLetters; k++){
        if(abc[k].letter == game->highscores[j].name.nameAr[l]){
          game->highscores[j].name.nameXpm[l] = abc[k];
        }
      }
    }
  }
  
  fclose(fp);
  return 0;
}

int new_highscore(Game * game,int speed)
{
  
  for(unsigned int i = 0; i < game->NUMBEROFSCORES; i++) {
    if(speed > (game->highscores[i].speed))
      return i;
  }
  return game->NUMBEROFSCORES;
}

void set_highscores(Game* game)
{
  if(game->NUMBEROFSCORES == 0){
    printf("no highscores to set\n");
    return;
  }

  FILE* fp;

  fp = fopen("/home/lcom/labs/proj/src/highscores.txt", "w");

  if (fp == NULL){
    printf("Highscore file not found\n");
    return;
  } 

  fprintf(fp, "%d", game->NUMBEROFSCORES);

  for(unsigned int i = 0; i < game->NUMBEROFSCORES; i++) {

    highScore* highscore = &game->highscores[i];

    fprintf(fp, "\n%d %d %d %d %d %d %d", highscore->date.hour, highscore->date.minutes, highscore->date.day, highscore->date.month, highscore->date.year, highscore->speed, highscore->name.nameSize);
    fprintf(fp, "\n%s", highscore->name.nameAr);
  }

  fclose(fp);
}

void add_new_highscore(Game ** game, unsigned int pos,int speed,realTime * dt)
{

  if((*game)->NUMBEROFSCORES == pos) {
    (*game)->highscores[pos].date = *dt;
    (*game)->highscores[pos].speed = speed;

    (*game)->highscores[pos].name.nameSize = (*game)->name->nameSize;
    
    strcpy((*game)->highscores[pos].name.nameAr, (*game)->name->nameAr);
    memcpy((*game)->highscores[pos].name.nameXpm,(*game)->name->nameXpm,sizeof(letter)*10);
   
    (*game)->NUMBEROFSCORES++;
  }
  else {
    for (unsigned int i=(*game)->NUMBEROFSCORES;i>pos;i--){
      if (i==5) continue;
      (*game)->highscores[i].date = (*game)->highscores[i-1].date;
      (*game)->highscores[i].speed = (*game)->highscores[i-1].speed;
      (*game)->highscores[i].name.nameSize = (*game)->highscores[i-1].name.nameSize;
      
      strcpy((*game)->highscores[i].name.nameAr,(*game)->highscores[i-1].name.nameAr);
      memcpy((*game)->highscores[i].name.nameXpm,(*game)->highscores[i-1].name.nameXpm,sizeof(letter)*10);
      
    }
    (*game)->highscores[pos].date = *dt;
    (*game)->highscores[pos].speed = speed;
    (*game)->highscores[pos].name.nameSize = (*game)->name->nameSize;
    
    strcpy((*game)->highscores[pos].name.nameAr, (*game)->name->nameAr);
    memcpy((*game)->highscores[pos].name.nameXpm,(*game)->name->nameXpm,sizeof(letter)*10);

    if ((*game)->NUMBEROFSCORES < 5) (*game)->NUMBEROFSCORES++;
  }

}
