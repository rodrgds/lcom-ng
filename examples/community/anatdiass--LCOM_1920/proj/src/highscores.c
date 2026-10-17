#include <lcom/lcf.h>

#include "highscores.h"
#include"xpmHandler.h"
#include "i8042.h"

Record records[3];
int nrRecords=0;

int getMaxTemp(int seconds[], int size){
    if(size <= 0) return 0;
    int max = seconds[0];

    for(int i=0;i<size;i++){
        if(seconds[i]>max)
            max=seconds[i];
    }
    return max;
}

void getSecondsArray(int res[]){

    for(int i=0;i<nrRecords;i++){
        char second[sizeof records[0].seconds];
        int aux;

        strcpy(second,records[i].seconds);

        sscanf(second, "%d", &aux);
        res[i] = aux;
    }
}


void addRecord(char nome[], int seg){

    int secs[3]; getSecondsArray(secs);

    //current max time
    int max = getMaxTemp(secs,nrRecords);

    if(nrRecords<3){
        Record r;
        strcpy(r.name, nome);
        sprintf(r.seconds, "%d", seg);

        records[nrRecords] = r;
        nrRecords++;
    }

    else{
        for(int i=0;i<nrRecords;i++){
            int segundos;
            sscanf(records[i].seconds, "%d", &segundos);
            //replace record with current max time
            if(segundos==max){
                strcpy(records[i].name, nome);
                sprintf(records[i].seconds, "%d", seg);
            }
        }
    }
}

bool verifyIsRecord(int seconds){
    int secs[3]; //array with all times
    getSecondsArray(secs);

    int maxSeconds = getMaxTemp(secs, nrRecords);

    //Verify when the table is full max time and compares with highscore time
    if(nrRecords>=3 && seconds>maxSeconds)
        return false;

    return true;
}

void printRecords(){
    for(int i=0;i<nrRecords;i++){
        if(i==0){
            printWord(records[i].name, 55, 254);
            printWord(records[i].seconds, 577, 254);
        }
        else if(i==1){
            printWord(records[i].name, 55, 329);
            printWord(records[i].seconds, 577, 329);
        }
        else if(i==2){
            printWord(records[i].name, 55, 410);
            printWord(records[i].seconds, 577, 410);
        }
    }
}

void saveRecords(){
    FILE *f = fopen("/home/lcom/proj/src/highscores.txt", "w");

    if(f!=NULL){
        for(int i=0; i<nrRecords; i++){
            fprintf(f, "%s", records[i].name);
            fprintf(f, "%c", '\n');
            fprintf(f, "%s", records[i].seconds); 
            fprintf(f, "%c", '\n');
		}
        fclose(f);
    }
}

void loadRecords(){
    FILE *f = fopen("/home/lcom/proj/src/highscores.txt", "r");
    
    int i=0;
    if(f!=NULL){
        while(!feof(f)){
            fscanf(f, "%s", records[i].name);
            fscanf(f, "%s", records[i].seconds);
            nrRecords++;
            i++;    
        }
    }
}

void addRecordName(char name[], int nameSize, uint8_t scancode){
    if(nameSize < 6) {
		if(scancode == A_KEY){
			name[nameSize] = 'A';
			nameSize++;
		}
		else if(scancode == B_KEY){
			name[nameSize] = 'B';
			nameSize++;
		}
		else if(scancode == C_KEY){
			name[nameSize] = 'C';
			nameSize++;
		}
		else if(scancode == D_KEY){
			name[nameSize] = 'D';
			nameSize++;
		}
		else if(scancode == E_KEY){
			name[nameSize] = 'E';
			nameSize++;
		}
		else if(scancode == F_KEY){
			name[nameSize] = 'F';
			nameSize++;
		}
		else if(scancode == G_KEY){
			name[nameSize] = 'G';
			nameSize++;
		}
		else if(scancode == H_KEY){
			name[nameSize] = 'H';
			nameSize++;
		}
		else if(scancode == I_KEY){
			name[nameSize] = 'I';
			nameSize++;
		}
		else if(scancode == J_KEY){
			name[nameSize] = 'J';
			nameSize++;
		}
		else if(scancode == K_KEY){
			name[nameSize] = 'K';
			nameSize++;
		}
		else if(scancode == L_KEY){
			name[nameSize] = 'L';
			nameSize++;
		}
		else if(scancode == M_KEY){
			name[nameSize] = 'M';
			nameSize++;
		}
		else if(scancode == N_KEY){
			name[nameSize] = 'N';
			nameSize++;
		}
		else if(scancode == O_KEY){
			name[nameSize] = 'O';
			nameSize++;
		}
		else if(scancode == P_KEY){
			name[nameSize] = 'P';
			nameSize++;
		}
		else if(scancode == Q_KEY){
			name[nameSize] = 'Q';
			nameSize++;
		}
		else if(scancode == R_KEY){
			name[nameSize] = 'R';
			nameSize++;
		}
		else if(scancode == S_KEY){
			name[nameSize] = 'S';
			nameSize++;
		}
		else if(scancode == T_KEY){
			name[nameSize] = 'T';
			nameSize++;
		}
		else if(scancode == U_KEY){
			name[nameSize] = 'U';
			nameSize++;
		}
		else if(scancode == V_KEY){
			name[nameSize] = 'V';
			nameSize++;
		}
		else if(scancode == W_KEY){
			name[nameSize] = 'W';
			nameSize++;
		}
		else if(scancode == X_KEY){
			name[nameSize] = 'X';
			nameSize++;
		}
		else if(scancode == Y_KEY){
			name[nameSize] = 'Y';
			nameSize++;
		}
		else if(scancode == Z_KEY){
			name[nameSize] = 'Z';
			nameSize++;
		}
		else if(scancode == ZERO_KEY){
			name[nameSize] = '0';
			nameSize++;
		}
		else if(scancode == ONE_KEY){
			name[nameSize] = '1';
			nameSize++;
		}
		else if(scancode == TWO_KEY){
			name[nameSize] = '2';
			nameSize++;
		}
		else if(scancode == THREE_KEY){
			name[nameSize] = '3';
			nameSize++;
		}
		else if(scancode == FOUR_KEY){
			name[nameSize] = '4';
			nameSize++;
		}
		else if(scancode == FIVE_KEY){
			name[nameSize] = '5';
			nameSize++;
		}
		else if(scancode == SIX_KEY){
			name[nameSize] = '6';
			nameSize++;
		}
		else if(scancode == SEVEN_KEY){
			name[nameSize] = '7';
			nameSize++;
		}
		else if(scancode == EIGHT_KEY){
			name[nameSize] = '8';
			nameSize++;
		}
		else if(scancode == NINE_KEY){
			name[nameSize] = '9';
			nameSize++;
		}
        else if(scancode == SPACE_KEY){
            name[nameSize] = ' ';
            nameSize++;
        }
		else if(scancode == BACKSPACE_KEY && nameSize > 0)
			nameSize--;
	}
	else {
		if(scancode == BACKSPACE_KEY){
			nameSize --;
		}
	}
}
