#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
char *itoa(int,int);
static unsigned char *allocation;
static size_t capacity;
static int deny;
void *checked_malloc(size_t size) {
  if (deny) return NULL;
  if (allocation) abort();
  allocation = malloc(size+64); capacity=size;
  memset(allocation,0xa5,size+64);
  return allocation;
}
static int check(int value,int base,const char *expected) {
  char *converted=itoa(value,base);
  if ((void*)converted!=allocation) return 1;
  for (size_t i=0;i<64;++i) if (allocation[capacity+i]!=0xa5) {
    fprintf(stderr,"conversion(%d,%d) wrote past its %zu-byte allocation\n",value,base,capacity);return 2;
  }
  if (strcmp(converted,expected)) {fprintf(stderr,"expected %s got %s\n",expected,converted);return 3;}
  free(allocation);allocation=NULL;return 0;
}
int main(void) {
  if(check(0,10,"0")||check(9,10,"9")||check(10,10,"10")||
     check(1234567890,10,"1234567890")||check(INT_MAX,10,"2147483647")||
     check(INT_MIN,10,"-2147483648")||check(INT_MIN,16,"80000000")||
     check(-9,10,"-9")||check(-9,16,"9")||check(31,2,"11111")||
     check(INT_MAX,2,"1111111111111111111111111111111")||check(31,32,"V")||
     check(5,1,"")||check(5,33,""))return 1;
  deny=1;if(itoa(10,10)!=NULL)return 1;
  puts("Original numeric conversion bounds and integer extremes passed");return 0;
}
