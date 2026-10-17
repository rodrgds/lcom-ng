#include <lcom/lcf.h>
#include "file.h"

int (read_highscore)(int *highscore){
    FILE *fptr;
    if((fptr = fopen("/home/lcom/labs/proj/src/highscore.txt", "r")) == NULL){
        printf("Error reading highscore value\n");
        return 1;
    }
    fscanf(fptr,"%d", highscore);
    fclose(fptr);
    return 0;
}

int (write_highscore)(int highscore){
    FILE *fptr;
    if((fptr = fopen("/home/lcom/labs/proj/src/highscore.txt", "w")) == NULL){
        printf("Error reading highscore value\n");
        return 1;
    }
    fprintf(fptr, "%d", highscore);
    fclose(fptr);
    return 0;
}
