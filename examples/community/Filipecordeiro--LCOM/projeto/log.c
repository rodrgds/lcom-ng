#include "log.h"
#include <time.h>

int LogCreated = 0;

void Log(char *message){

/*  char cwd[1024];
  if(getcwd(cwd, sizeof(cwd)) != NULL)
    //printf("Current working dir: %s\n", cwd);
  else
    perror("getcwd() error");*/

  time_t now;
  struct tm *now_tm;
  int hour,minute,second;

  now = time(NULL);
  now_tm = localtime(&now);
  hour = now_tm->tm_hour;
  minute = now_tm->tm_min;
  second = now_tm->tm_sec;

  FILE *file;
  if(LogCreated == 0){
    file = fopen(LOGFILE, "w");
    LogCreated = 1;
  }
  else{
    file = fopen(LOGFILE, "a+");
  }

  if(file == NULL){
    if(LogCreated == 1) LogCreated = 0;
    perror("ERROR OPENING LOGFILE");
    return;
  }
  else{
    //fputs(message, file);
    char z_h=(char) 0, z_m=(char) 0, z_s=(char) 0;
    if(hour<10) z_h = '\0';
    if(minute<10) z_m = '\0';
    if(second<10) z_s = '\0';
    fprintf(file, "[%d%d:%d%d:%d%d] ", z_h, hour, z_m, minute, z_s, second);
    fprintf(file, "%s", message);
    fclose(file);
  }


}
