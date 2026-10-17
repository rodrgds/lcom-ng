#include "leaderboard.h"

/* read scores file */
int read_file(GameScore scores[], int size)
{
	FILE *scores_to_read = fopen("/home/lcom/labs/proj/leaderboard_scores.txt","r");

	if(scores_to_read == NULL)
		return 1;

	char line[100] = {'\0'};

	for(int lines=0; lines < size; lines++)
	{
		fgets(line, 100, scores_to_read);

		unsigned int i = 0;

		char score_temp[20] = {'\0'};

		for(;i < 100; i++)
		{
			if (line[i] == ' ')
			{
				i++;
				break;
			}

			else score_temp[i] = line[i];
		}

		char *endptr;
		scores[lines].score = strtol(score_temp, &endptr, 10);

		char name_temp[20] = {'\0'};
		unsigned int j = 0;

		for(;i < 100; i++, j++)
		{
			if (line[i] == ' ' && (line[i+1] == '0' || line[i+1] == '1' || line[i+1] == '2' || line[i+1] == '3' || line[i+1] == '4' || line[i+1] == '5' || line[i+1] == '6' || line[i+1] == '7' || line[i+1] == '8' || line[i+1] == '9'))
			{
				break;
			}

			else name_temp[j] = line[i];
		}
		strcpy(scores[lines].name,name_temp);


		char date_temp[25] = {'\0'};
		i++;
		j=0;
		for(;i < 100; i++, j++)
		{
			if (line[i] == '\n')
				break;
			
			else{
				date_temp[j] = line[i];
			} 
		}

		strcpy(scores[lines].date,date_temp);

	}

	fclose(scores_to_read);

	return 0;
}

int write_file(GameScore scores[], int size)
{
	FILE *scores_to_write = fopen("/home/lcom/labs/proj/leaderboard_scores.txt","w");

	if(scores_to_write == NULL)
		return 1;

	for(int i=0; i < size; i++)
	{
		fprintf(scores_to_write, "%lu %s %s\n", scores[i].score, scores[i].name, scores[i].date);
	}

	fclose(scores_to_write);
	return 0;
}

int insert_on_leaderboard(GameScore scores[], int size, GameScore gs){
    for(int counter=0; counter<size; counter++){
        if (gs.score>=scores[counter].score){
            for (int counter1=size-2; counter1>=counter;counter1--){
                scores[counter1+1]=scores[counter1];
            }
            scores[counter]=gs;
            break;
        }
    }
    return 0;
}
