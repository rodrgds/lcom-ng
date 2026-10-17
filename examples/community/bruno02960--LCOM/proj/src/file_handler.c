#include "file_handler.h"
#include <stdio.h>
#include "variables.h"

/*
 * @brief Writes the score to the textfile
 */
void write_score() {
	unsigned long day = getDay();
	unsigned long month = getMonth();
	unsigned long year = getYear();
	unsigned long sec = getSec();
	unsigned long min = getMin();
	unsigned long hour = getHour();

	FILE *ficheiro;
	ficheiro = fopen("highscores.txt", "a");

	if (ficheiro == (FILE *)NULL)
		printf("\nImpossivel abrir ficheiro\n");

	else {
		fprintf(ficheiro, "%s", "Date=");
		if (day < 10)
			fprintf(ficheiro, "%u", 0);
		fprintf(ficheiro, "%u", day);
		fprintf(ficheiro, "%c", '/');
		if (day < 10)
			fprintf(ficheiro, "%u", 0);
		fprintf(ficheiro, "%u", month);
		fprintf(ficheiro, "%c", '/');
		fprintf(ficheiro, "%u", year);
		fprintf(ficheiro, "%c", ' ');
		fprintf(ficheiro, "%s", "Hour=");
		if (hour < 10)
			fprintf(ficheiro, "%u", 0);
		fprintf(ficheiro, "%u", hour);
		fprintf(ficheiro, "%c", ':');
		if (min < 10)
			fprintf(ficheiro, "%u", 0);
		fprintf(ficheiro, "%u", min);
		fprintf(ficheiro, "%c", ' ');
		fprintf(ficheiro, "%s", "Player=");
		fprintf(ficheiro, "%s", user);
		fprintf(ficheiro, "%c", ' ');
		fprintf(ficheiro, "%s", "Score=");
		fprintf(ficheiro, "%d", total_score);
		fprintf(ficheiro, "%c", '\n');
		fclose(ficheiro);
	}
}
