#include <stdlib.h>
#include <stdio.h>
#include <lcom/lcf.h>
#include "Score.h"

Score score(int val){
  Score s;
  //65 == ASCII CODE FOR: A
  s.letters[0] = 65;
  s.letters[1] = 65;
  s.letters[2] = 65;
  s.value = val;
  return s;
}

Score maxScore(Score *s1, Score *s2){
  if (s1->value < s2->value) return *s2;
  else return *s1;
}

void upLetterZero(Score *s1){
  if (s1->letters[0] < 90) s1->letters[0]++;
  else s1->letters[0] = 65;
}

void downLetterZero(Score *s1){
  if (s1->letters[0] > 65) s1->letters[0]--;
  else s1->letters[0] = 90;
}

void upLetterOne(Score *s1){
  if (s1->letters[1] < 90) s1->letters[1]++;
  else s1->letters[1] = 65;
}

void downLetterOne(Score *s1){
  if (s1->letters[1] > 65) s1->letters[1]--;
  else s1->letters[1] = 90;
}

void upLetterTwo(Score *s1){
  if (s1->letters[2] < 90) s1->letters[2]++;
  else s1->letters[2] = 65;
}

void downLetterTwo(Score *s1){
  if (s1->letters[2] > 65) s1->letters[2]--;
  else s1->letters[2] = 90;
}

void setScore(Score *s1, char l1, char l2, char l3, int val){
  s1->letters[0] = l1;
  s1->letters[1] = l2;
  s1->letters[2] = l3;
  s1->value = val;
}

void copy(Score * dest, Score * src){
  dest->letters[0] = src->letters[0];
  dest->letters[1] = src->letters[1];
  dest->letters[2] = src->letters[2];
  dest->value = src->value;
}

void readHighScoresFromFile(){
  FILE *fp;
  if ((fp = fopen("/home/lcom/labs/proj/src/highscores.txt", "r")) == NULL){
    puts("ERROR OPENING HIGHSCORES FILE");
    Score s;
    s.letters[0] = 65;
    s.letters[1] = 65;
    s.letters[2] = 65;
    s.value = 0;
    s.data = NewDate();
    highscores[0] = s;
    highscores[1] = s;
    highscores[2] = s;
    return;
  }
  char buff[255];
  int i = 0;
  while(i < 3){
    fgets(buff, 255, fp);
    Score s;
    setScore(&s, buff[0], buff[1], buff[2], 0);
    fgets(buff, 255, fp);
    s.value = atoi(buff);
    fgets(buff, 255, fp);
    Date d;
    stringToDate(&d, buff);
    s.data = d;
    highscores[i] = s;
    i++;
  }
}

void writeHighScoresToFile(){
  FILE *fp;
  if ((fp = fopen("/home/lcom/labs/proj/src/highscores.txt", "w")) == NULL){
    puts("ERROR OPENING HIGHSCORES FILE");
    return;
  }
  for(int i = 0; i < 3; i++){
    char datas[13];
    dateToString(&highscores[i].data, datas);
    fprintf(fp, "%s\n", highscores[i].letters);
    fprintf(fp, "%d\n", highscores[i].value);
    fprintf(fp, "%s\n", datas);
  }
}

bool newHS(Score * s1){
  if (s1->value > highscores[2].value) return true;
  return false;
}

void updateHighScores(Score *s1){
  if (s1->value > highscores[2].value){
    if (s1->value > highscores[1].value){
      if (s1->value > highscores[0].value){
        highscores[2] = highscores[1];
        highscores[1] = highscores[0];
        highscores[0] = *s1;
      }
      else{
        highscores[2] = highscores[1];
        highscores[1] = *s1;
      }
    }

    else highscores[2] = *s1;
  }
}
