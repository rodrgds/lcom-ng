#include <lcom/lcf.h>
#include "files.h"

int read_highscores(Highscore highscores[]){
    FILE* file;
    int hour,  minute, day,month,year;
    file=fopen("/home/lcom/labs/proj/src/highscores.txt","r");
    if(file!=NULL){
       for(int i=0;i<5;i++){
           if(feof(file)){
               highscores[i].score=0;
               break;
           }

           fscanf(file,"%d\t",&hour);
           fscanf(file,"%d\t",&minute);
           fscanf(file,"%d\t",&day);
           fscanf(file,"%d\t",&month);
           fscanf(file,"%d\t",&year);
           fscanf(file,"%d\n",&highscores[i].score);
           
           highscores[i].d.hour=hour;
           highscores[i].d.minute=minute;
           highscores[i].d.month=month;
           highscores[i].d.day=day;
           highscores[i].d.year=year;

       }
    }
    else return 1;
    fclose(file);
    return 0;
}


int write_highscores(Highscore highscores[]){
    
    
    FILE* file;
    file=fopen("/home/lcom/labs/proj/src/highscores.txt","w");
    if(file!=NULL){
      for(int i=0;i<5;i++){
           fprintf(file,"%d\t%d\t%d\t%d\t%d\t%d\n",highscores[i].d.hour,highscores[i].d.minute,highscores[i].d.day,highscores[i].d.month,highscores[i].d.year,highscores[i].score);

       }
    }
    else return 1;
    fclose(file);
    return 0;
}

