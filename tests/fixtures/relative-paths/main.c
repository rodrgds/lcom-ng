#include <lcom/lcf.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
static int resource(const char *path) {
  FILE *file=fopen(path,"r");if(!file)return 0;
  char value[64]={0};size_t length=fread(value,1,sizeof(value)-1,file);fclose(file);
  return length==24 && !strcmp(value,"relative alias resource\n");
}
int main(void) {
  if(!resource("home/lcom/relative-sdk/resource.txt"))return 21;
  if(!resource("assets/resource.txt"))return 22;
  FILE *file=fopen("home/lcom/relative-sdk2/resource.txt","r");
  if(file || errno!=ENOENT)return 23;
  file=fopen("home/lcom/relative-sdk/pending.txt","w");if(!file)return 24;
  if(fputs("relative alias resource\n",file)<0 || fclose(file))return 25;
  if(rename("home/lcom/relative-sdk/pending.txt","home/lcom/relative-sdk/saved.txt"))return 26;
  if(!resource("home/lcom/relative-sdk/saved.txt"))return 27;
  if(remove("home/lcom/relative-sdk/saved.txt"))return 28;
  printf("RELATIVE_PATHS_OK\n");return 0;
}
