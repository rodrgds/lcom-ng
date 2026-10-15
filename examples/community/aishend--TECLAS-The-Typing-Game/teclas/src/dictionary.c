#include "dictionary.h"
#include <stdlib.h>
#include <string.h>

void gen_words(Word *pool, int from, int count, int pool_size) {
    for (int i = 0; i < count; i++) {
        int idx = (from + i) % pool_size;
        pool[idx].word = english_dictionary[rand() % DICTIONARY_SIZE];
        pool[idx].word_len = strlen(pool[idx].word);
        pool[idx].typed_len = 0;
        memset(pool[idx].states, 0, sizeof(pool[0].states));
    }
}
