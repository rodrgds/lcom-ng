#include <lcom/lcf.h>
#include <machine/int86.h> // /usr/src/include/arch/i386
#include <stdio.h>
#include "file.h"

Users usersInit(){

	Users users = malloc(sizeof(struct lligada));

	users->name[0] = '0';
  users->name[1] = '0';
  users->name[2] = '0';
  users->score[0] = '0';
  users->score[1] = '0';
  users->score[2] = '0';
  users->date[0] = '0';
  users->date[1] = '0';
  users->date[2] = '0';
  users->date[3] = '0';
  users->date[4] = '0';
  users->date[5] = '0';
  users->date[6] = '0';
  users->date[7] = '0';

	users->next = NULL;

	return users;
}

void addUser(Users users, char* name, char* date, char* score){
	Users l = users;
	Users pt, ant;
	pt = l;
	ant = malloc(sizeof(struct lligada));

	ant->name[0]  = name[0];
	ant->name[1]  = name[1];
	ant->name[2]  = name[2];
	ant->score[0] = score[0];
	ant->score[1] = score[1];
	ant->score[2] = score[2];
	ant->date[0]  = date[0];
	ant->date[1]  = date[1];
	ant->date[2]  = '/';
	ant->date[3]  = date[3];
	ant->date[4]  = date[4];
	ant->date[5]  = '/';
	ant->date[6]  = date[6];
	ant->date[7]  = date[7];

	ant->next = NULL;

	if (l == NULL)
	{
		users = ant;
	}
	else{
		while( l->next != NULL && scoreToInt(l->next->score) > scoreToInt(score)){
			l = l->next;
		}
		ant->next = l->next;
		l->next = ant;

		users = pt;
	}
}

void freeUsers(Users users){
	if (users == NULL)
	{
		return;
	}
	Users r = users;
	while(r != NULL){
		users = r;
		r = r->next;
		free(users);
	}
}

void write_to_file(Users users, char* file){
	if (users == NULL || users->next == NULL) return;

	Users l = users->next;

	FILE *ptr;
	// printf("Insira o path para o ficheiro\n");
	// char file[128];//"/home/lcom/labs/proj/src/users.txt"ile);
	ptr = fopen(file, "w");
	
	while(l != NULL){
		int n = 0;
		while(n < 3){
			fprintf(ptr, "%c", l->name[n++]);
		}
		fprintf(ptr, "\t");
		n = 0;
		while(n < 3){
			fprintf(ptr, "%c", l->score[n++]);
		}
		fprintf(ptr, "\t");
		n = 0;
		while(n < 8){
			fprintf(ptr, "%c", l->date[n++]);
		}
		fprintf(ptr, "\n");
		l = l->next;
	}
}

void read_from_file(Users users, char* file){

	FILE *ptr;
	// printf("Insira o path para o ficheiro\n");
	// char file[128];//"/home/lcom/labs/proj/src/users.txt";
	// scanf("%123s",file);
	ptr = fopen(file, "r");

	int count = 0;
	char c = fgetc(ptr);

	while(c != EOF){
		char name[3], score[3], date[8];
		count = 0;

		while(c != '\t'){
			name[count++] = c;
			c = fgetc(ptr);
		}
		c = fgetc(ptr);
		count = 0;
		while(c != '\t'){
			score[count++] = c;
			c = fgetc(ptr);
		}
		c = fgetc(ptr);
		count = 0;
		while(c != '\n'){
			date[count++] = c;
			c = fgetc(ptr);
		}
		c = fgetc(ptr);

		addUser(users,name,date,score);
	}
}

int scoreToInt(char* score){

	int valor = score[0]*100 + score[1]*10 + score[2];

	return valor;
}

void scoreIntToString(char* score, int valor){
	int unidades = valor % 10;
	int dezenas = (valor % 100)/10;
	int centenas = (valor / 100);

	score[0] = centenas + '0';
	score[1] = dezenas + '0';
	score[2] = unidades + '0';
}

void dateIntToString(char* date, int dia, int mes, int ano){
	int dunidades = dia % 10;
	int ddezenas = (dia % 100)/10;

	int munidades = mes % 10;
	int mdezenas = (mes % 100)/10;

	int aunidades = ano % 10;
	int adezenas = (ano % 100)/10;

	date[2] = date[5] = '/';

	date[0] = ddezenas + '0';
	date[1] = dunidades + '0';

	date[3] = mdezenas + '0';
	date[4] = munidades + '0';

	date[6] = adezenas + '0';
	date[7] = aunidades + '0';
}
