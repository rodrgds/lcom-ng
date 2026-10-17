
#include "Leaderboard.h"
#include "video_card.h"
#include "words.h"
#include "sprite.h"

extern Sprite* spriteRecordDia;
extern Sprite* spriteLeaderboard;
const char* filename_leaderboard = "/home/lcom/labs/proj/Leaderboard.txt";
Player players[3]; // players da leaderboard
extern char* topPlayerName;



void askForName(uint8_t* initial_address, char* topPlayerName) {
  draw_backgroud(spriteRecordDia, initial_address);
  displayWordWithColor(330, 690, BLACK, topPlayerName, initial_address);
}


// copiado de http://source-code-share.blogspot.com/2014/07/implementation-of-java-stringsplit.html
int split (char *str, char c, char ***arr) {
    int count = 1;
    int token_len = 1;
    int i = 0;
    char *p;
    char *t;

    p = str;
    while (*p != '\0')
    {
        if (*p == c)
            count++;
        p++;
    }

    *arr = (char**) malloc(sizeof(char*) * count);
    if (*arr == NULL)
        exit(1);

    p = str;
    while (*p != '\0')
    {
        if (*p == c)
        {
            (*arr)[i] = (char*) malloc( sizeof(char) * token_len );
            if ((*arr)[i] == NULL)
                exit(1);

            token_len = 0;
            i++;
        }
        p++;
        token_len++;
    }
    (*arr)[i] = (char*) malloc( sizeof(char) * token_len );
    if ((*arr)[i] == NULL)
        exit(1);

    i = 0;
    p = str;
    t = ((*arr)[i]);
    while (*p != '\0')
    {
        if (*p != c && *p != '\0')
        {
            *t = *p;
            t++;
        }
        else
        {
            *t = '\0';
            i++;
            t = ((*arr)[i]);
        }
        p++;
    }

    return count;
}


void read_leaderboard_file() {
  FILE *file;

  file = fopen(filename_leaderboard, "r");
  if (file == NULL) {
      printf("Could not open file %s",filename_leaderboard);
      return;
  }
  char* input = (char *) calloc(256, sizeof(char));
  int line = 0;
  char** array = NULL;

  while(fgets(input, 256 * sizeof(char), file)) {

      split(input, '-', &array);

      players[line].name = array[0];
      players[line].score = (uint8_t) atoi(array[1]);
      
      line++;
  }

  free(input);
  free(array);
  fclose(file);
}

bool new_record_handler(int score, char* name) {
    read_leaderboard_file();
    if (score >= players[2].score) {
        if (score >= players[1].score) {
            if (score >= players[0].score) {
                // 1º
                players[2].name = players[1].name;
                players[2].score = players[1].score;
                players[1].name = players[0].name;
                players[1].score = players[0].score;
                players[0].name = name;
                players[0].score = score;
                
                return true;
            }
            else {
                // 2º
                players[2].name = players[1].name;
                players[2].score = players[1].score;
                players[1].name = name;
                players[1].score = score;
                return true;
            }
        }
        else {
                // 3º
                players[2].name = name;
                players[2].score = score;
                return true;
        }
    }
    else {
        return false;
    }
}

bool is_new_record(int score) {
    read_leaderboard_file();
    if (score >= players[2].score) {
        return true;
    }
    else {
        return false;
    }
}

void write_leaderboard_file() {
    FILE *file;

    file = fopen(filename_leaderboard, "w");
    if (file == NULL) {
        printf("Could not open file %s",filename_leaderboard);
        return;
    }
    for (int i = 0; i < 3; i++) {
        fprintf(file, "%s-%d\n", players[i].name, players[i].score);
    }
    fclose(file);
}


void display_leaderboard(uint8_t* initial_address) {
    read_leaderboard_file();
    draw_backgroud(spriteLeaderboard, initial_address);

    char* name = (char *) calloc(256, sizeof(char));
    uint8_t score = 0;
    int x0 = 305;
    int y0 = 290;
    for (int i = 0; i < 3; i++) { // numero de linhas é 3
        name = (char*) players[i].name;
        score = (uint8_t) players[i].score;
        displayWordWithColor(x0, y0, BLACK, name, initial_address);
        displayNumber(930, y0, (unsigned int) score, initial_address);
        y0 += 200;
    }
    free(name);
}
