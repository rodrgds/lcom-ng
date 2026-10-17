#include "game/wordlist.h"

#include "vector/vector.h"
#include "utils.h"

#include "game/layout.h"

static vector *wordlist;

int (game_wordlist_init)(char *path) {

  FILE *file = fopen("/home/lcom/labs/proj/src/wordlist.txt", "r");
  if (file == NULL) {
    return GAME_ELOAD;
  }

  wordlist = vec_create(sizeof(char*));

  char buffer[COLS + 1];
  while (fscanf(file, "%s", buffer) > 0) {
    void *word;
    util_copy_elem(&word, buffer, COLS + 1);

    if (word == NULL) {
      continue;
    }
    
    vec_push(wordlist, &word);
  }

  fclose(file);

  return OKAY;
}

int (game_wordlist_cleanup)() {
  for (size_t i = 0; i < wordlist->len; i++) {
    char *word = *(char**) vec_at(wordlist, i);
    free(word);
  }

  vec_destroy(wordlist);
  return OKAY;
}

bool (game_wordlist_contains)(char* word) {
  size_t l = 0;
  size_t r = wordlist->len;

  while (l < r) {
    size_t mid = (l + r) / 2;

    char *mid_word = *(char**) vec_at(wordlist, mid);
    int comp = strcmp(mid_word, word);

    if (comp == 0) {
      return true;
    } else if (comp > 0) {
      r = mid;
    } else if (comp < 0) {
      l = mid + 1;
    }
  }

  return false;
}

char *(game_wordlist_select_random)() {
  size_t num_words = wordlist->len;

  size_t select = rand() % num_words;
  return *(char**) vec_at(wordlist, select);
}
