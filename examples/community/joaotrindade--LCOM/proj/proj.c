#include <stdint.h>

#include "proj.h"
#define WAIT_TIME_S 5
#define UPPER_LIMIT 2
#define LOWER_LIMIT 568
#define N_MAX_MISSEIS 200
#define N_MAX_INIMIGOS 50
#define enemy_height 100



int spaceship_position;
int last_missile_index, last_enemy_index, total_enemies ;
int aliveEnemies = 0;
int createdEnemies = 0;
int total_enemy_positions[N_MAX_INIMIGOS]={0,0,100,200,300,400,500,100,400,500,450,300,400,120,280,400,500,0,300,100,0,500,100,400,200,300,0,500,150,350,200,0,500,200,400,300,150,0,400,150,0,220,400,260,500,0, 500, 400, 200, 0};
int enemy_positions[N_MAX_INIMIGOS];
int game_over = 0;
//int enemy_height = 100;
int pontuacao = 0;
static int hook= 0;
unsigned char scancode;




missile vetor_misseis[N_MAX_MISSEIS];
enemy vetor_inimigos[N_MAX_INIMIGOS];
player_score highscores[50];
int last_highscore = 0;
int lowestHighscore = 0;



void ordenaHighScores(player_score arrayResultados[])
{
	int i, j;
	player_score temp;
	for( i = 0 ;  i < last_highscore - 1  ; i++)
	{
		for (j = i + 1; j < last_highscore; j++)
		{
			if (arrayResultados[j].pontuacao > arrayResultados[i].pontuacao)
			{
				temp = arrayResultados[i];
				arrayResultados[i] = highscores[j];
				arrayResultados[j] = temp;
			}
		}
	}
}



/* The shipped Minix/x86 file stores a 16-byte name slot and six little-endian
 * 32-bit fields, independently of the host's long size or struct padding. */
static uint32_t score_word(const unsigned char *p)
{
    return (uint32_t)p[0] | (uint32_t)p[1]<<8 | (uint32_t)p[2]<<16 | (uint32_t)p[3]<<24;
}

void writeHighScores(player_score arrayResultados[])
{
    FILE *fp = fopen("/usr/src/drivers/proj/highscores", "wb");
    if (!fp) return;
    int count = last_highscore < 15 ? last_highscore : 15;
    int i, j;
    for (i = 0; i < count; ++i) {
        player_score *entry = &arrayResultados[i];
        unsigned char record[40] = {0};
        for (j = 0; j < 14 && entry->nome[j]; ++j) record[j] = entry->nome[j];
        int32_t fields[6] = {entry->pontuacao, entry->dia, entry->mes,
                             entry->ano, entry->hora, entry->minuto};
        for (j = 0; j < 6; ++j) {
            uint32_t value = (uint32_t)fields[j];
            int byte;
            for (byte = 0; byte < 4; ++byte) record[16 + 4*j + byte] = value >> (8*byte);
        }
        if (fwrite(record, 1, sizeof record, fp) != sizeof record) break;
    }
    fclose(fp);
}

void readHighScores(player_score arrayResultados[])
{
    player_score empty = {0};
    strcpy(empty.nome, "EMPTY");
    empty.dia = empty.mes = empty.ano = empty.hora = empty.minuto = '0';
    int i;
    for (i = 0; i < 15; ++i) arrayResultados[i] = empty;
    last_highscore = 0;
    FILE *fp = fopen("/usr/src/drivers/proj/highscores", "rb");
    if (!fp) return;
    unsigned char record[40];
    while (last_highscore < 15 && fread(record, 1, sizeof record, fp) == sizeof record) {
        player_score entry = {0};
        memcpy(entry.nome, record, sizeof entry.nome - 1);
        entry.pontuacao = (int32_t)score_word(record + 16);
        entry.dia = (int32_t)score_word(record + 20);
        entry.mes = (int32_t)score_word(record + 24);
        entry.ano = (int32_t)score_word(record + 28);
        entry.hora = (int32_t)score_word(record + 32);
        entry.minuto = (int32_t)score_word(record + 36);
        arrayResultados[last_highscore++] = entry;
        if (entry.pontuacao < lowestHighscore) lowestHighscore = entry.pontuacao;
    }
    fclose(fp);
}

void insertNome(player_score jogador, int _irq_set, int _ipc_status, message _msg)
{
	int enter_found = 0;
	char tempNome[15] = {0};
	int nextChar = 0;
	unsigned char scancode2;
	vg_fill(0x00);
	drawInterface(0, 10, 259,0);
	int letra_x, letra_y;
	letra_x = 256;
	letra_y = 341;

	printf("Pontuacao na Funcao : %d\n",jogador.pontuacao);


	while(!enter_found) {
		if(driver_receive(ANY, &_msg, &_ipc_status) != 0) {
			printf("driver_receive failed\n");
			continue;
		}
		if (is_ipc_notify(_ipc_status)) {

			switch (_ENDPOINT_P(_msg.m_source)) {
				case HARDWARE :
					if (_msg.NOTIFY_ARG & _irq_set)
					{
						//printf("entrou ciclo\n");
						scancode2 = read_scancode();
                        if (nextChar >= sizeof tempNome - 1 && scancode2 != 0x1c) continue;
						if (scancode2 == 0x1c) {
							enter_found = 1;
						}

						if (scancode2 == 0x1e)
						{

							drawLetter('A', letra_x, letra_y, 0);
							tempNome[nextChar] = 'A';
							nextChar++;
							letra_y += 25;

						}
						if (scancode2 == 0x30)
						{
							drawLetter('B', letra_x, letra_y, 0);
							tempNome[nextChar] = 'B';
							nextChar++;
							letra_y += 25;
						}
						if (scancode2 == 0x2e)
						{
							drawLetter('C', letra_x, letra_y, 0);
							tempNome[nextChar] = 'C';
							nextChar++;
							letra_y += 25;
						}
						if (scancode2 == 0x20)
						{
							drawLetter('D', letra_x, letra_y, 0);
							tempNome[nextChar] = 'D';
							nextChar++;
							letra_y += 25;
						}
						if (scancode2 == 0x12)
						{
							drawLetter('E', letra_x, letra_y, 0);
							tempNome[nextChar] = 'E';
							nextChar++;
							letra_y += 25;
						}
						if (scancode2 == 0x21)
						{
							drawLetter('F', letra_x, letra_y, 0);
							tempNome[nextChar] = 'F';
							nextChar++;
							letra_y += 25;
						}
						if (scancode2 == 0x22)
						{
							drawLetter('G', letra_x, letra_y, 0);
							tempNome[nextChar] = 'G';
							nextChar++;
							letra_y += 25;
						}
						if (scancode2 == 0x23)
						{
							drawLetter('H', letra_x, letra_y, 0);
							tempNome[nextChar] = 'H';
							nextChar++;
							letra_y += 25;
						}
						if (scancode2 == 0x17)
						{
							drawLetter('I', letra_x, letra_y, 0);
							tempNome[nextChar] = 'I';
							nextChar++;
							letra_y += 25;
						}
						if (scancode2 == 0x24)
						{
							drawLetter('J', letra_x, letra_y, 0);
							tempNome[nextChar] = 'J';
							nextChar++;
							letra_y += 25;
						}
						if (scancode2 == 0x25)
						{
							drawLetter('K', letra_x, letra_y, 0);
							tempNome[nextChar] = 'K';
							nextChar++;
							letra_y += 25;
						}
						if (scancode2 == 0x26)
						{
							drawLetter('L', letra_x, letra_y, 0);
							tempNome[nextChar] = 'L';
							nextChar++;
							letra_y += 25;
						}
						if (scancode2 == 0x32)
						{
							drawLetter('M', letra_x, letra_y, 0);
							tempNome[nextChar] = 'M';
							nextChar++;
							letra_y += 25;
						}
						if (scancode2 == 0x31)
						{
							drawLetter('N', letra_x, letra_y, 0);
							tempNome[nextChar] = 'N';
							nextChar++;
							letra_y += 25;
						}
						if (scancode2 == 0x18)
						{
							drawLetter('O', letra_x, letra_y, 0);
							tempNome[nextChar] = 'O';
							nextChar++;
							letra_y += 25;
						}
						if (scancode2 == 0x19)
						{
							drawLetter('P', letra_x, letra_y, 0);
							tempNome[nextChar] = 'P';
							nextChar++;
							letra_y += 25;
						}
						if (scancode2 == 0x10)
						{
							drawLetter('Q', letra_x, letra_y, 0);
							tempNome[nextChar] = 'Q';
							nextChar++;
							letra_y += 25;
						}
						if (scancode2 == 0x13)
						{
							drawLetter('R', letra_x, letra_y, 0);
							tempNome[nextChar] = 'R';
							nextChar++;
							letra_y += 25;
						}
						if (scancode2 == 0x1f)
						{
							drawLetter('S', letra_x, letra_y, 0);
							tempNome[nextChar] = 'S';
							nextChar++;
							letra_y += 25;
						}
						if (scancode2 == 0x14)
						{
							drawLetter('T', letra_x, letra_y, 0);
							tempNome[nextChar] = 'T';
							nextChar++;
							letra_y += 25;
						}
						if (scancode2 == 0x16)
						{
							drawLetter('U', letra_x, letra_y, 0);
							tempNome[nextChar] = 'U';
							nextChar++;
							letra_y += 25;
						}
						if (scancode2 == 0x2f)
						{
							drawLetter('V', letra_x, letra_y, 0);
							tempNome[nextChar] = 'V';
							nextChar++;
							letra_y += 25;
						}
						if (scancode2 == 0x11)
						{
							drawLetter('W', letra_x, letra_y, 0);
							tempNome[nextChar] = 'W';
							nextChar++;
							letra_y += 25;
						}
						if (scancode2 == 0x2d)
						{
							drawLetter('X', letra_x, letra_y, 0);
							tempNome[nextChar] = 'X';
							nextChar++;
							letra_y += 25;
						}
						if (scancode2 == 0x15)
						{
							drawLetter('Y', letra_x, letra_y, 0);
							tempNome[nextChar] = 'Y';
							nextChar++;
							letra_y += 25;
						}
						if (scancode2 == 0x2c)
						{
							drawLetter('Z', letra_x, letra_y, 0);
							tempNome[nextChar] = 'Z';
							nextChar++;
							letra_y += 25;
						}

						tempNome[nextChar] = '\0';
					}
			}
		}
	}
	strcpy(jogador.nome, tempNome);

	long int dia1, mes1, ano1, hora1, minuto1;
	long int *p_dia, *p_mes, *p_ano, *p_hora, *p_minuto;
	//printf("entrou apontadores\n");
	p_dia = &dia1;
	p_mes = &mes1;
	p_ano = &ano1;
	p_hora = &hora1;
	p_minuto = &minuto1;
	//printf("passou apontadores\n");
	get_data(p_dia,p_mes,p_ano,p_hora,p_minuto);
	//printf("saiu");
	printf(" DEPOIS DO GETDATA %x - %x - %x ! %x : %x",dia1,mes1,ano1,hora1,minuto1);

	jogador.ano = ano1;
	jogador.mes = mes1;
	jogador.dia = dia1;
	jogador.hora = hora1;
	jogador.minuto = minuto1;

	highscores[last_highscore] = jogador;
	last_highscore++;
	ordenaHighScores(highscores);
	writeHighScores(highscores);
}



void checkColisao(missile vetor_misseis[]){
	int i, j,k;
	int found = 0;
	// vertical + 7
	// horizonal + 50
	//printf("Last Enemy Index Inicial : %d \n",last_enemy_index );
	for(i = 1; i < last_missile_index;i++)
	{
		found = 0;
		if ( (vg_get_pixel(vetor_misseis[i].horizontalPos + 50,vetor_misseis[i].verticalPos + 7) != 0) && (found == 0) )
		{
			//printf("Detectou colisao \n",j);
			//enemy teste1;
			//teste1.horizontalPos = 850;
			//teste1.verticalPos = 150;
			//drawEnemy(teste1,0);


			//VER QUAL É O INDICE DO VECTOR A QUE CORRESPONDE A COLISAO
			for(j = 1; j < last_enemy_index; j++ )
			{
				if (( ( vetor_misseis[i].verticalPos + 7 > vetor_inimigos[j].verticalPos ) && ( vetor_misseis[i].verticalPos + 7 < vetor_inimigos[j].verticalPos + enemy_height ) ) && (found == 0))
				{
					//printf("Detectou colisao parametrizada enemyindex : %d \n",j);
					found = 1;
					drawEnemy(vetor_inimigos[j],1);
					drawMissile(vetor_misseis[i],1);

					//Eliminar inimigo atingindo do vetor
					for(k = j; k < last_enemy_index-1 ; k++)
					{
						vetor_inimigos[k] = vetor_inimigos[k+1];
						//enemy_positions[k] = enemy_positions[k+1];
					}

					//Eliminar missil que acertou do vetor
					for(k = i-1; k < last_missile_index-1 ; k++)
					{
						vetor_misseis[k] = vetor_misseis[k+1];
					}
					aliveEnemies--;
					last_missile_index--;
					last_enemy_index--;
					pontuacao+=100;
					drawPontuacao(pontuacao, 956,670,0);
					//drawNumber(1,600,600,0);
					//drawNumber(2,600,956,0);
				}
			}
			actualizaEnemy(0);


		}

	}
	//printf("Last Enemy Index Final : %d\n",last_enemy_index );
}



int actualizaMisseis(missile vetor_misseis[]){
	int i,j, removed;
	removed = 0;


	for(i = 1; i < last_missile_index;i++)
	{
		//if (vetor_misseis[i].horizontalPos < 800) vetor_misseis[i].horizontalPos+=10;
		drawMissile(vetor_misseis[i],1);
		vetor_misseis[i].horizontalPos+=10;
	}


	for(i = 1; i < last_missile_index;i++)
	{
		if (vetor_misseis[i].horizontalPos > 800)
		{
			for(j = i-1; j < last_missile_index-1 ; j++)
			{
				vetor_misseis[j] = vetor_misseis[j+1];
			}
			//removed++;
			last_missile_index--;
		}
	}
	//last_missile_index -= removed;

}

void actualizaEnemy(int move){
	// MOVE = 1 - MOVIMENTA O INIMIGO
	int i,j, removed;
	removed = 0;

	for(i = 1; i < last_enemy_index;i++)
	{
		//if (vetor_misseis[i].horizontalPos < 800) vetor_misseis[i].horizontalPos+=10;
		drawEnemy(vetor_inimigos[i],1);
		if(move == 1) vetor_inimigos[i].horizontalPos-=20;
		drawEnemy(vetor_inimigos[i],0);
		if (vetor_inimigos[i].horizontalPos < 100) game_over = 1;
	}


	for(i = 1; i < last_enemy_index;i++)
	{
		if (vetor_inimigos[i].horizontalPos < 10)
		{
			for(j = i-1; j < last_enemy_index-1 ; j++)
			{
				vetor_inimigos[j] = vetor_inimigos[j+1];
			}
			last_enemy_index--;
		}
	}
}

void createEnemy(){
	enemy temp;
	int i;
	//printf("Ciclo: \n");
	//for(i = 1; i < 5; i++) printf(" %d ",enemy_positions[i]);
	//printf("\n");
	temp.verticalPos = enemy_positions[createdEnemies+1];
	temp.horizontalPos = 700;
	//printf("entrou : %d", temp.verticalPos );

	vetor_inimigos[last_enemy_index] = temp;
	drawEnemy(temp,0);
	last_enemy_index++;
	createdEnemies++;


}

void drawGUI()
{
	vg_draw_line(0,668,1023,668,0x36);
}

int jogo(int _irq_set, int _ipc_status, message _msg){
	spaceship_position = 334;


	int ipc_status, irq_set, esc_found;

	int time_count,refresh_count,enemy_count,enemy_refresh;
	int missil_i;

	total_enemies = N_MAX_INIMIGOS;
	last_missile_index = 1;
	last_enemy_index = 1;
	message msg;
	esc_found = 0;
	refresh_count = 0;
	enemy_count = 0;
	enemy_refresh = 0;
	pontuacao = 0;
	game_over = 0;
	createdEnemies = 0 ;
	aliveEnemies = 0;

	readHighScores(highscores);
	player_score jogador1;

	int i;
	for(i = 0; i < N_MAX_INIMIGOS; i++ )
	{
		enemy_positions[i] = total_enemy_positions[i];
	}

	//sef_startup();

	missile a1;

	//printf("entrou\n &d     %d", a1.horizontalPos, a1.verticalPos);


	//drawMissile(a1);


	//vg_init(0x105);
	vg_fill(0x00);
	drawGUI();
	drawPontuacao(pontuacao, 956, 670, 0);
	drawMainShip(spaceship_position,0);

	//drawMainShip(spaceship_position);
	//irq_set = kbc_subscribe_int();

	while(!esc_found && !game_over) {
			if(driver_receive(ANY, &_msg, &_ipc_status) != 0) {
				printf("driver_receive failed\n");
				continue;
			}
			if (is_ipc_notify(_ipc_status)) {

				switch (_ENDPOINT_P(_msg.m_source)) {
					case HARDWARE :
						if (_msg.NOTIFY_ARG & _irq_set)
						{
							//printf("entrou ciclo\n");
							scancode = read_scancode();
							if (scancode == 0x81) {
								esc_found = 1;
							}
							if (scancode == 0x48) // FOUND SETA CIMA
							{
								if (spaceship_position - 20 > UPPER_LIMIT)
								{
									drawMainShip(spaceship_position,1);
									spaceship_position-= 10;
									drawMainShip(spaceship_position,0);
								}
								else
								{
									drawMainShip(spaceship_position,1);
									spaceship_position = 0;
									drawMainShip(spaceship_position,0);
								}


							}
							if (scancode == 0x50) // FOUND SETA BAIXO
							{
								if (spaceship_position + 20 < LOWER_LIMIT)
								{
									drawMainShip(spaceship_position,1);
									spaceship_position+=10;
									drawMainShip(spaceship_position,0);

								}
								else
								{
									drawMainShip(spaceship_position,1);
									spaceship_position = LOWER_LIMIT;
									drawMainShip(spaceship_position,0);

								}

							}

							if (scancode == 0x39) // FOUND B.ESPACOS
							{
								a1.horizontalPos = 150;
								a1.verticalPos = spaceship_position + 50;
								vetor_misseis[last_missile_index] = a1;
								last_missile_index++;
								drawMissile(a1,0);
							}
						}
						if (_msg.NOTIFY_ARG & 0x4)
						{
							printf("entrou no timer\n");
							refresh_count++;
							enemy_count++;
							enemy_refresh++;
							if (refresh_count == 2)
							{
								//last_missile_index = actualizaMisseis(vetor_misseis,last_missile_index);
								//a1.horizontalPos = 250;
								//a1.verticalPos = 250;

								actualizaMisseis(vetor_misseis);
								checkColisao(vetor_misseis);
								//if ( last_enemy_index>0 ) actualizaEnemy();
								if (last_missile_index > 1)
								for(missil_i = 1; missil_i < last_missile_index; missil_i++)
								{
									drawMissile(vetor_misseis[missil_i],0);
								}
								refresh_count = 0;
							}
							if(enemy_count == 100)
							{

								if ( createdEnemies  < total_enemies )
								{
										createEnemy();
										aliveEnemies ++;

								}
								else
								{
									if (aliveEnemies == 0) game_over = 1;

								}
								enemy_count = 0;

							}
							if(enemy_refresh == 50)
							{
								actualizaEnemy(1);
								enemy_refresh = 0;
							}
						}

				}
		}

	}
	/*if (game_over == 1)
	{
		jogador1.pontuacao = pontuacao;
		insertNome(jogador1, _irq_set, _ipc_status, _msg);
	}*/

	jogador1.pontuacao = pontuacao;
	insertNome(jogador1, _irq_set, _ipc_status, _msg);

	//printf("Saiu do Ciclo\n");

	//printf("Fez unsubscribe ao timer\n");
	//kbc_unsubscribe_int();
	//printf("Fez unsubscribe ao KBC\n");
	//sleep(2);
	//vg_exit();





	//vg_init(0x105);
	//drawMainShip(spaceship_position);
	//sleep(15);
	//vg_exit(); /* Return to text mode */
	//printf("Saiu\n");
	return 0;
}
