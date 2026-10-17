#include "highscores.h"
#include <math.h>
#define X_PLAYER 50
#define Y_NOME 25
#define Y_PONTUACAO 520
#define Y_DATA 620
#define SMALL_CHAR_WIDTH 22





void print_data(char dia[], char mes[], char ano[], char hora[], char minuto[], int x_pos, int y_pos)
{

	int last_y_pos = y_pos;
	int i, valor_temp;;
	printf("entrou print data\n");

	//drawNumber(atoi(dia[0]), x_pos, last_y_pos, 0, 1 );


	//DIA
	valor_temp = atoi(dia);
	if (valor_temp < 10)
	{
		drawNumber(0, x_pos, last_y_pos,0, 1);
		last_y_pos +=SMALL_CHAR_WIDTH;
		drawNumber(valor_temp, x_pos, last_y_pos,0, 1);
		last_y_pos +=SMALL_CHAR_WIDTH;
	}
	else
	{
		drawNumber(valor_temp/10, x_pos, last_y_pos,0, 1);
		last_y_pos +=SMALL_CHAR_WIDTH;
		drawNumber(valor_temp%10, x_pos, last_y_pos,0, 1);
		last_y_pos +=SMALL_CHAR_WIDTH;
	}


	// TODO: PRINT SEPERATOR
	last_y_pos +=SMALL_CHAR_WIDTH;



	//MES
	valor_temp = atoi(mes);
	if (valor_temp < 10)
	{
		drawNumber(0, x_pos, last_y_pos,0, 1);
		last_y_pos +=SMALL_CHAR_WIDTH;
		drawNumber(valor_temp, x_pos, last_y_pos,0, 1);
		last_y_pos +=SMALL_CHAR_WIDTH;
	}
	else
	{
		drawNumber(valor_temp/10, x_pos, last_y_pos,0, 1);
		last_y_pos +=SMALL_CHAR_WIDTH;
		drawNumber(valor_temp%10, x_pos, last_y_pos,0, 1);
		last_y_pos +=SMALL_CHAR_WIDTH;
	}

	// TODO: PRINT SEPERATOR
	last_y_pos +=SMALL_CHAR_WIDTH;



	//ANO
	valor_temp = atoi(ano);
	if (valor_temp < 10)
	{
		drawNumber(0, x_pos, last_y_pos,0, 1);
		last_y_pos +=SMALL_CHAR_WIDTH;
		drawNumber(valor_temp, x_pos, last_y_pos,0, 1);
		last_y_pos +=SMALL_CHAR_WIDTH;
	}
	else
	{
		drawNumber(valor_temp/10, x_pos, last_y_pos,0, 1);
		last_y_pos +=SMALL_CHAR_WIDTH;
		drawNumber(valor_temp%10, x_pos, last_y_pos,0, 1);
		last_y_pos +=SMALL_CHAR_WIDTH;
	}

	// TODO: PRINT SEPERATOR
	last_y_pos +=SMALL_CHAR_WIDTH;



	//hora
	valor_temp = atoi(hora);
	if (valor_temp < 10)
	{
		drawNumber(0, x_pos, last_y_pos,0, 1);
		last_y_pos +=SMALL_CHAR_WIDTH;
		drawNumber(valor_temp, x_pos, last_y_pos,0, 1);
		last_y_pos +=SMALL_CHAR_WIDTH;
	}
	else
	{
		drawNumber(valor_temp/10, x_pos, last_y_pos,0, 1);
		last_y_pos +=SMALL_CHAR_WIDTH;
		drawNumber(valor_temp%10, x_pos, last_y_pos,0, 1);
		last_y_pos +=SMALL_CHAR_WIDTH;
	}

	// TODO: PRINT SEPERATOR
	last_y_pos +=SMALL_CHAR_WIDTH;



	//minutos
	valor_temp = atoi(minuto);
	if (valor_temp < 10)
	{
		drawNumber(0, x_pos, last_y_pos,0, 1);
		last_y_pos +=SMALL_CHAR_WIDTH;
		drawNumber(valor_temp, x_pos, last_y_pos,0, 1);
		last_y_pos +=SMALL_CHAR_WIDTH;
	}
	else
	{
		drawNumber(valor_temp/10, x_pos, last_y_pos,0, 1);
		last_y_pos +=SMALL_CHAR_WIDTH;
		drawNumber(valor_temp%10, x_pos, last_y_pos,0, 1);
		last_y_pos +=SMALL_CHAR_WIDTH;
	}





	printf("Print_Data: %c  - resultado do atoi: %d\n", dia[0],atoi(dia));


}

void print_scores(int _irq_set, int _ipc_status, message _msg)
{
	int i,j;
	int exit = 0;
	char temp_nome[15];
	char dia[5], mes[5],ano[5],hora[5],minuto[5];
	unsigned char scancode;
	player_score resultados[50], temp;
	readHighScores(resultados);
	
	vg_fill(0x00);

	//drawPontuacao(222,0, 0, 1);
	//drawNumber((int*)resultados[0].hora, 500, 700, 0, 1);

	for(i= 0; i < 13; i++)
	{
		temp = resultados[i];
		strcpy(temp_nome, temp.nome);
		j = 0;


		while ( (temp_nome[j] != '\0') && (j < 15) )
		{
			drawLetter(temp_nome[j], (i+1)*X_PLAYER, (j+1)*Y_NOME, 0);
			j++;
		}

		drawPontuacao(temp.pontuacao,Y_PONTUACAO, (i+1)*X_PLAYER, 1);
		printf("chegou aqui\n");
		printf("Em hexa: %x\n",temp.dia);
		sprintf (ano, "%x",temp.ano);
		sprintf (mes, "%x",temp.mes);
		sprintf (dia, "%x",temp.dia);
		sprintf (hora, "%x",temp.hora);
		sprintf (minuto, "%x",temp.minuto);

		print_data(dia,mes,ano,hora,minuto, (i+1)*X_PLAYER,Y_DATA); //Y_DATA, (i+1)*X_PLAYER );

	}


	while(exit != 1) {
			if(driver_receive(ANY, &_msg, &_ipc_status) != 0) {
				printf("driver_receive failed\n");
				continue;
			}
			if (is_ipc_notify(_ipc_status)) {

				switch (_ENDPOINT_P(_msg.m_source)) {
					case HARDWARE :
						if (_msg.NOTIFY_ARG & _irq_set)
						{
							scancode = read_scancode();
							if (scancode == 0x81) exit = 1;
						}
				}
			}
	}

}
