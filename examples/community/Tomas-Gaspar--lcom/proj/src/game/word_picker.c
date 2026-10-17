#include <lcom/lcf.h>

#include "word_picker.h"

char* get_word() {
    char** words = NULL;
    char word[MAX_WORD_LENGTH];
    int word_count = 0;
    int index;

    FILE* file = fopen("home/lcom/labs/g1/proj/src/game/word_list.txt", "r");
    
    if (file == NULL)
        file = fopen("home/lcom/labs/proj/src/game/word_list.txt", "r");

    while (fscanf(file, "%s", word) != EOF) {
        
        words = realloc(words, (word_count + 1) * sizeof(char*));

        words[word_count] = malloc((strlen(word) + 1) * sizeof(char));

        strcpy(words[word_count], word);

        word_count++;
    }

    fclose(file);

    index = rand() % word_count;

    char* result = malloc((strlen(words[index]) + 1) * sizeof(char));
    strcpy(result, words[index]);

    for (int i = 0; i < word_count; i++) {
        free(words[i]);
    }
    free(words);

    return result;
}
