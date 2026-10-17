#include "sentences.h"
static void *owned[128];
static unsigned count, released;
void *checked_malloc(size_t size) {
  void *p=calloc(1,size+1024);
  if(!p||count==128)abort();
  owned[count++]=p;return p;
}
void checked_free(void *p) {
  for(unsigned i=0;i<count;++i)if(p&&owned[i]==p){owned[i]=NULL;++released;free(p);return;}
  fprintf(stderr,"Sentence cleanup attempted an unowned row after %u valid frees\n",released);exit(1);
}
int main(void) {
  sentences=allocateTwoDimenArrayOnHeapUsingMalloc(nSentences,nChars);
  abc=checked_malloc(sizeof(letter)*nLetters);
  numbers=checked_malloc(sizeof(number)*12);
  destroySentencesAndLetters();
  if(released!=nSentences+3)return 1;
  for(unsigned i=0;i<count;++i)if(owned[i])return 1;
  puts("Original sentence matrix cleanup released exactly its owned rows");return 0;
}
