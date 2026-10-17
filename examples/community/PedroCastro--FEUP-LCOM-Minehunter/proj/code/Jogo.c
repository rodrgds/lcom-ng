#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ModPortaSerie.h"
#include "ModGrafico.h"
#include "Jogo.h"
#include "ModRato.h"
#include "ModTeclado.h"
#include "ModTimer.h"
#include "bitmap.h"

void print_table(Mine** table){
	int coordx = 32, coordy = 186;
	int i,j;
	Bitmap* Bomba = loadBitmap(PATH"Bomba.bmp");
	Bitmap* Vazio = loadBitmap(PATH"Vazio.bmp");
	Bitmap* E1 = loadBitmap(PATH"Quadrado1.bmp");
	Bitmap* E2 = loadBitmap(PATH"Quadrado2.bmp");
	Bitmap* E3 = loadBitmap(PATH"Quadrado3.bmp");
	Bitmap* E4 = loadBitmap(PATH"Quadrado4.bmp");
	Bitmap* E5 = loadBitmap(PATH"Quadrado5.bmp");
	Bitmap* E6 = loadBitmap(PATH"Quadrado6.bmp");
	Bitmap* E7 = loadBitmap(PATH"Quadrado7.bmp");
	Bitmap* E8 = loadBitmap(PATH"Quadrado8.bmp");


	for(i=0;i<30;i++)
	{
		for(j=0;j<16;j++)
		{
			switch(table[i][j].valor)
			{
			case 0:
				drawBitmap(Vazio,coordx + i*32,coordy + j*32,ALIGN_LEFT,buffer);
				break;
			case 1:
				drawBitmap(E1,coordx + i*32,coordy + j*32,ALIGN_LEFT,buffer);
				break;
			case 2:
				drawBitmap(E2,coordx + i*32,coordy + j*32,ALIGN_LEFT,buffer);
				break;
			case 3:
				drawBitmap(E3,coordx + i*32,coordy + j*32,ALIGN_LEFT,buffer);
				break;
			case 4:
				drawBitmap(E4,coordx + i*32,coordy + j*32,ALIGN_LEFT,buffer);
				break;
			case 5:
				drawBitmap(E5,coordx + i*32,coordy + j*32,ALIGN_LEFT,buffer);
				break;
			case 6:
				drawBitmap(E6,coordx + i*32,coordy + j*32,ALIGN_LEFT,buffer);
				break;
			case 7:
				drawBitmap(E7,coordx + i*32,coordy + j*32,ALIGN_LEFT,buffer);
				break;
			case 8:
				drawBitmap(E8,coordx + i*32,coordy + j*32,ALIGN_LEFT,buffer);
				break;
			case -1:
				drawBitmap(Bomba,coordx + i*32,coordy + j*32,ALIGN_LEFT,buffer);
				break;
			}

		}
	}
	deleteBitmap(E1);
	deleteBitmap(E2);
	deleteBitmap(E3);
	deleteBitmap(E4);
	deleteBitmap(E5);
	deleteBitmap(E6);
	deleteBitmap(E7);
	deleteBitmap(E8);
	deleteBitmap(Bomba);
	deleteBitmap(Vazio);
}

int jogo_single_player(int difficulty,int irq_set_timer,int irq_set_keyboard,int irq_set_mouse) {
	Bitmap* bitmap_table;
	Bitmap* fundo_jogo;
	Bitmap* smiley;
	Bitmap* smile;
	Bitmap* smileyO;
	bitmap_table = loadBitmap(PATH"Tabela_Expert.bmp");
	fundo_jogo = loadBitmap(PATH"Fundo_Jogo.bmp");
	smiley = loadBitmap(PATH"Smile.bmp");
	smileyO = loadBitmap(PATH"SmileyO.bmp");
	smile = smiley;
	int contador = 0,breaker = 1,two_bytes = 0, mouse_byte; /// mouse e ciclo while
	int ipc_status, loops = 0;///cenas das interrupcoes
	message msg;
	int paused = 0;
	int filled = 0,por_carregar,repeat = 0,tempo = 0,cronometro_parado = 1;///jogo
	if(difficulty == 2)
		por_carregar = 380;
	else if (difficulty == 1)
		por_carregar = 0; /// alterar
	else por_carregar = 0; // alterar
	Mine** table = create_table(difficulty);
	drawBitmap(fundo_jogo,0,0,ALIGN_LEFT,buffer);
	drawBitmap(bitmap_table,0,0,ALIGN_LEFT,buffer);
	deleteBitmap(fundo_jogo);
	deleteBitmap(bitmap_table);
	while (breaker) {
		if (driver_receive(ANY, &msg, &ipc_status) != 0) {
			printf("driver_receive failed with: %d");
			continue;
		}
		//printf("msg.notifyArg: 0x%X\n", msg.NOTIFY_ARG);
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_timer) {
					if(global_counter % 1 == 0)
					{
						drawBitmap(smile,486,68,ALIGN_LEFT,buffer);
						update_screen(0,0,0);
					}
					timer_int_handler();
					if(cronometro_parado)
					{
						if(difficulty == 2 && por_carregar != 380)
						{
							cronometro_parado = 0;
							global_counter = 0;
						}
						///acrescentar outras dificuldades
					}

					if (global_counter == 60) {
						global_counter = 0;
						if(cronometro_parado == 0)
						{
							tempo++;
							if(tempo >= 1000)
								tempo = 999;
							update_cronometro(tempo);
						}
					}
				}
				if (msg.NOTIFY_ARG & irq_set_keyboard)
				{
					kbd_int_handler();
					if(scan_code==TWO_BYTE_CODE){two_bytes = 1;}
					else if(scan_code>>7){
						if(two_bytes) ;
						else ;
						if (scan_code ==BREAK_CODE_ESC)
							breaker = 0;
					}


					else
					{
						if (two_bytes){
							two_bytes=0;
						}
						else ;
					}

				}
				if (msg.NOTIFY_ARG & irq_set_mouse)
				{
					rato = getRato();
					mouse_byte = mouse_int_handler();
					if (contador == 0) {
						if (!first_byte(mouse_byte)) {
							contador = 0;
							continue;
						}
					}
					rato->packets[contador] = mouse_byte;
					if (contador == 2) {
						contador = 0;
						{
							updateMouse();

							if(rato->packets[0]&BIT(0) && rato->leftButtonDown == 0)
							{
								smile = smileyO;
								rato->leftButtonDown = 1;
								rato->leftButtonReleased = 0;
								if(rato->x >=486 && rato->x<=538 && rato->y >=68 && rato->y <= 120)
								{
									repeat = 1;
									breaker = 0;
								}
								else if(click_screen(&table,rato->x,rato->y,difficulty,&filled,&por_carregar,1) == -1)
								{
									post_game_state(difficulty,-1,irq_set_timer,irq_set_keyboard,irq_set_mouse,table);
									breaker = 0;
								}else if(por_carregar == 0)
								{
									post_game_state(difficulty,1,irq_set_timer,irq_set_keyboard,irq_set_mouse,table);
									breaker = 0;
								}
							}
							else if(!(rato->packets[0]&BIT(0)))
							{
								smile = smiley;
								rato->leftButtonDown = 0;
								rato->leftButtonReleased = 1;
							}
							if(rato->packets[0]&BIT(1) && rato->rightButtonDown == 0)
							{
								rato->rightButtonDown = 1;
								rato->rightButtonReleased = 0;
								right_click_screen(&table,rato->x,rato->y,difficulty);
							}
							else if(!(rato->packets[0]&BIT(1)))
							{
								rato->rightButtonDown = 0;
								rato->rightButtonReleased = 1;
							}

						}
						continue;
					}
					contador++;

				}
				break;
			}
		}

	}
	deleteBitmap(smiley);
	deleteBitmap(smileyO);
	free(table);
	if(repeat)
		jogo_single_player(difficulty,irq_set_timer,irq_set_keyboard,irq_set_mouse);
}

Mine** fill_table(Mine** table,int difficulty,int k_mouse, int j_mouse,int single,char *seed,int use)
{
	if(difficulty == 2){
		int i;
		time_t t;
		if(use == 0){
			t = time(NULL);
			*seed = t;
			t = *seed;
		}
		else{
			t = *seed;
		}
		srand((unsigned) t);
		for(i = 0; i < NUM_MINES_EXPERT + (!single);i++)
		{

			int j = rand() % HEIGHT_EXPERT;
			int k = rand() % WIDTH_EXPERT;
			if(table[k][j].valor == -1)
			{i--;}
			else if(table[k][j].valor >= 8)
			{i--;}
			else if(((k == k_mouse || k == k_mouse + 1 || k == k_mouse - 1)&&(j == j_mouse || j ==j_mouse -1 || j == j_mouse + 1)) && (single==1))
			{
				i--;
			}
			else{
				printf("%d\n",i);
				table[k][j].valor = -1;
				if(k != 0) // parede lateral esquerda /// certo!!
					if(table[k-1][j].valor != -1)
						table[k-1][j].valor += 1;
				if(k != WIDTH_EXPERT - 1) /// parede laterral direita/// certo!!
					if(table[k+1][j].valor != -1)
						table[k+1][j].valor += 1;
				if(j != 0) /// parede vertical de cima /// certo!!
					if(table[k][j - 1].valor != -1)
						table[k][j - 1].valor += 1;
				if(j != HEIGHT_EXPERT - 1)///parede vertical de baixo/// certo!!
					if(table[k][j+1].valor != -1)
						table[k][j + 1].valor += 1;
				if(k != 0 && j != 0) ///canto superior esquerdo ///certo
					if(table[k - 1][j - 1].valor != -1)
						table[k - 1][j - 1].valor += 1;
				if(k != WIDTH_EXPERT - 1 && j != HEIGHT_EXPERT - 1)  /// canto inferior direito /// certo
					if(table[k + 1][j + 1].valor != -1)
						table[k + 1][j + 1].valor += 1;
				if(k != WIDTH_EXPERT - 1 && j != 0)  /// canto superior direito
					if(table[k + 1][j - 1].valor != -1)
						table[k + 1][j - 1].valor += 1;
				if(k != 0 && j != HEIGHT_EXPERT - 1)/// canto inferior esquerdo
					if(table[k - 1][j + 1].valor != -1)
						table[k - 1][j + 1].valor += 1;
			}
		}
	}
		return table;

}

Mine** create_table(int difficulty){
	if(difficulty == 2) ///expert
	{
		int i;
		Mine** table;
		table = malloc(WIDTH_EXPERT *sizeof(Mine *));
		for (i = 0; i < WIDTH_EXPERT; i++)
			table[i] = malloc( HEIGHT_EXPERT *sizeof(Mine));
		int j;
		for(i = 0;i < WIDTH_EXPERT;i++)
			for(j = 0; j < HEIGHT_EXPERT;j++)
			{
				table[i][j].valor = 0;
				table[i][j].carregado = 0;
			}
		return table;
	}
}

int click_screen(Mine*** table, int x, int y, int difficulty, int *filled,int *por_carregar,int single){
	Bitmap* quadrado;
	int valor = - 2;
	if(difficulty == 2){
		if(x-32 > 0 && x < 992)
		{
			if(y-186>0 && y < 698)
			{
				if((*filled) == 0)
				{
					char lixo;
					*(table) = fill_table((*table),difficulty,(x-32)/32,(y-186)/32,single,&lixo,0);
					*(filled)  = 1;
				}
				if((*table)[(x-32)/32][(y-186)/32].carregado == 0){
					(*table)[(x-32)/32][(y-186)/32].carregado = 1;
					(*por_carregar) -= 1;
					valor = (*table)[(x-32)/32][(y-186)/32].valor;
					if(valor == 0)
						click_vazio(table, ((x-32)/32), ((y-186)/32), difficulty,por_carregar,single);
					if(valor == 0)
						quadrado = loadBitmap(PATH"Vazio.bmp");
					else if(valor == -1)
						quadrado = loadBitmap(PATH"Bomba.bmp");
					else if(valor == 1)
					{
						quadrado = loadBitmap(PATH"Quadrado1.bmp");
					}
					else if(valor == 2)
					{
						quadrado = loadBitmap(PATH"Quadrado2.bmp");
					}
					else if(valor == 3)
					{
						quadrado = loadBitmap(PATH"Quadrado3.bmp");
					}
					else if(valor == 4)
					{
						quadrado = loadBitmap(PATH"Quadrado4.bmp");
					}
					else if(valor == 5)
					{
						quadrado = loadBitmap(PATH"Quadrado5.bmp");
					}
					else if(valor == 6)
					{
						quadrado = loadBitmap(PATH"Quadrado6.bmp");
					}
					else if(valor == 7)
					{
						quadrado = loadBitmap(PATH"Quadrado7.bmp");
					}
					else
					{
						quadrado = loadBitmap(PATH"Quadrado8.bmp");
					}
					drawBitmap(quadrado,(((x-32)/32)*32)+32,(((y-186)/32)*32)+186,ALIGN_LEFT,buffer);
					deleteBitmap(quadrado);
				}
			}
		}
	}
	return valor;
}

int right_click_screen(Mine*** table, int x, int y, int difficulty){
	Bitmap* quadrado;
	if(difficulty == 2){
		if(x-32 > 0 && x < 992)
		{
			if(y-186>0 && y < 698)
			{
				if((*table)[(x-32)/32][(y-186)/32].carregado == 0){
					(*table)[(x-32)/32][(y-186)/32].carregado = 2;
					quadrado = loadBitmap(PATH"Bandeira.bmp");
					drawBitmap(quadrado,(((x-32)/32)*32)+32,(((y-186)/32)*32)+186,ALIGN_LEFT,buffer);
				}
				else if((*table)[(x-32)/32][(y-186)/32].carregado == 2)
				{
					(*table)[(x-32)/32][(y-186)/32].carregado = 0;
					quadrado = loadBitmap(PATH"Quadrado_Tapado.bmp");
					drawBitmap(quadrado,(((x-32)/32)*32)+32,(((y-186)/32)*32)+186,ALIGN_LEFT,buffer);
				}

			}
		}
	}

}

void click_vazio(Mine*** table, int k, int j, int difficulty,int *por_carregar,int single)
{
	int filled = 1;
	if(difficulty == 2){
		if(k != 0)
			if((*table)[k-1][j].valor != -1){
				if((*table)[k-1][j].carregado == 0){
					click_screen(table, (k-1)*32+32+2, (j)*32 + 186+2, difficulty,&filled,por_carregar, single);
				}
			}
		if(k != WIDTH_EXPERT - 1)
			if((*table)[k+1][j].valor != -1){
				if((*table)[k+1][j].carregado == 0){
					click_screen(table, (k+1)*32+32+2, (j)*32 + 186+2, difficulty,&filled,por_carregar, single);
				}
			}
		if(j != 0)
			if((*table)[k][j - 1].valor != -1){
				if((*table)[k][j-1].carregado == 0){
					click_screen(table, (k)*32+32+2, (j-1)*32 + 186+2, difficulty,&filled,por_carregar, single);
				}
			}
		if(j != HEIGHT_EXPERT - 1)
			if((*table)[k][j+1].valor != -1){
				if((*table)[k][j+1].carregado == 0){
					click_screen(table, (k)*32+32+2, (j+1)*32 + 186+2, difficulty,&filled,por_carregar, single);
				}
			}
		if(k != 0 && j != 0)
			if((*table)[k - 1][j - 1].valor != -1){
				if((*table)[k-1][j-1].carregado == 0){
					click_screen(table, (k-1)*32+32+2, (j-1)*32 + 186+2, difficulty,&filled,por_carregar, single);
				}
			}
		if(k != WIDTH_EXPERT - 1 && j != HEIGHT_EXPERT - 1)
			if((*table)[k + 1][j + 1].valor != -1){
				if((*table)[k+1][j+1].carregado == 0){
					click_screen(table, (k+1)*32+32+2, (j+1)*32 + 186+2, difficulty,&filled,por_carregar, single);
				}
			}
		if(k != WIDTH_EXPERT - 1 && j != 0)
			if((*table)[k + 1][j - 1].valor != -1){
				if((*table)[k+1][j-1].carregado == 0){
					click_screen(table, (k+1)*32+32+2, (j-1)*32 + 186+2, difficulty,&filled,por_carregar, single);
				}
			}
		if(k != 0 && j != HEIGHT_EXPERT - 1)
			if((*table)[k - 1][j + 1].valor != -1){
				if((*table)[k-1][j+1].carregado == 0){
					click_screen(table, (k-1)*32+32+2, (j+1)*32 + 186+2, difficulty,&filled,por_carregar, single);
				}
			}
	}
}

void create_interrupts(int *irq_set_timer,int *irq_set_keyboard,int *irq_set_mouse){
	*irq_set_timer = timer_subscribe_int();
	*irq_set_mouse = mouse_subscribe_int();
	*irq_set_keyboard = kbd_subscribe_int();
	int value;
	sys_irqsetpolicy(COM1_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &value);
	enable_packets();
}

void stop_interrupts(){
	unsigned long stat,lixo;
	sys_inb(STAT_REG ,&stat); //lê status-reg para variavel
	while ((stat & OBF)){
		sys_inb(STAT_REG ,&stat);
		sys_inb(OUT_BUF, &lixo); //le out_buff para variavel
	}
	mouse_unsubscribe_int();
	kbd_unsubscribe_int();
	timer_unsubscribe_int();
}

int post_game_state(int difficulty,int win,int irq_set_timer,int irq_set_keyboard,int irq_set_mouse,Mine** table){
	int contador = 0,breaker = 1,two_bytes = 0, mouse_byte;
	int ipc_status, loops = 0;
	message msg;
	Bitmap* fundo;
	Bitmap* back_button;
	back_button = loadBitmap(PATH"Back_Button.bmp");
	if(win == -1)
	{
		fundo = loadBitmap(PATH"You_Lost_Single.bmp");
	}
	else{
		fundo = loadBitmap(PATH"Fundo_Won.bmp");
	}
	drawBitmap(fundo,0,0,ALIGN_LEFT,buffer);
	drawBitmap(back_button,0,0,ALIGN_LEFT,buffer);
	if (win == -1) print_table(table);
	deleteBitmap(fundo);
	deleteBitmap(back_button);
	while (breaker) {
		if (driver_receive(ANY, &msg, &ipc_status) != 0) {
			printf("driver_receive failed with: %d");
			continue;
		}
		//printf("msg.notifyArg: 0x%X\n", msg.NOTIFY_ARG);
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_timer) {
					timer_int_handler();
					if(global_counter % 1 == 0)
					{
						update_screen(0,0,0);
					}

					if (global_counter == 60) {
						global_counter = 0;
					}
				}
				if (msg.NOTIFY_ARG & irq_set_keyboard)
				{
					kbd_int_handler();
					if(scan_code==TWO_BYTE_CODE){two_bytes = 1;}
					else if(scan_code>>7){
						if(two_bytes) ;
						else ;
						if (scan_code==BREAK_CODE_ESC)
							breaker = 0;
					}
					else
					{
						if (two_bytes){
							two_bytes=0;
						}
						else ;
					}
				}
				if (msg.NOTIFY_ARG & irq_set_mouse)
				{

					rato = getRato();
					mouse_byte = mouse_int_handler();
					if (contador == 0) {
						if (!first_byte(mouse_byte)) {
							contador = 0;
							continue;
						}
					}
					rato->packets[contador] = mouse_byte;
					if (contador == 2) {
						contador = 0;
						{
							updateMouse();

							if(rato->packets[0]&BIT(0) && rato->leftButtonDown == 0)
							{
								rato->leftButtonDown = 1;
								rato->leftButtonReleased = 0;
								if(rato->x >=0 && rato->x < 50 && rato->y >=0 && rato->y < 50)
									breaker = 0;
							}
							else if(!(rato->packets[0]&BIT(0)))
							{
								rato->leftButtonDown = 0;
								rato->leftButtonReleased = 1;
							}
							if(rato->packets[0]&BIT(1) && rato->rightButtonDown == 0)
							{
								rato->rightButtonDown = 1;
								rato->rightButtonReleased = 0;
							}
							else if(!(rato->packets[0]&BIT(1)))
							{
								rato->rightButtonDown = 0;
								rato->rightButtonReleased = 1;
							}

						}
						continue;
					}
					contador++;

				}
			}
		}
	}
}

int jogo_multi_player(int difficulty,int irq_set_timer,int irq_set_keyboard,int irq_set_mouse) {
	Bitmap* bitmap_table;
	Bitmap* fundo_jogo;
	Bitmap* Player1_table;
	Bitmap* Player2_table;
	Player1_table = loadBitmap(PATH"Player1_table.bmp");
	Player2_table = loadBitmap(PATH"Player2_table.bmp");
	bitmap_table = loadBitmap(PATH"Tabela_Expert.bmp");
	fundo_jogo = loadBitmap(PATH"Fundo_Jogo.bmp");
	int contador = 0,breaker = 1,two_bytes = 0, mouse_byte; /// mouse e ciclo while
	int ipc_status, loops = 0;///cenas das interrupcoes
	message msg;
	int filled = 1,por_carregar,cronometro = 30,jogador = 1,pontuacao1 = 0,pontuacao2 = 0,bombas_por_carregar,cronometro_parado = 1;///jogo
	if(difficulty == 2)
	{
		por_carregar = 380;
		bombas_por_carregar = 100;
	}
	else if (difficulty == 1)
		bombas_por_carregar = 0; /// alterar
	else bombas_por_carregar = 0; // alterar
	Mine** table = create_table(difficulty);
	char lixo;
	table = fill_table(table,difficulty,0,0,0,&lixo,0);
	drawBitmap(fundo_jogo,0,0,ALIGN_LEFT,buffer);
	drawBitmap(bitmap_table,0,0,ALIGN_LEFT,buffer);
	drawBitmap(Player1_table,32,50,ALIGN_LEFT,buffer);
	drawBitmap(Player2_table,880,50,ALIGN_LEFT,buffer);
	deleteBitmap(fundo_jogo);
	deleteBitmap(bitmap_table);
	while (breaker) {
		if (driver_receive(ANY, &msg, &ipc_status) != 0) {
			printf("driver_receive failed with: %d");
			continue;
		}
		//printf("msg.notifyArg: 0x%X\n", msg.NOTIFY_ARG);
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_timer) {
					timer_int_handler();
					if(global_counter % 1 == 0)
					{
						update_screen(jogador,0,0);
					}
					if(cronometro_parado)
					{
						if(difficulty == 2 && por_carregar != 380)
						{
							cronometro_parado = 0;
							cronometro = 31;
						}
						else
						{
							if(difficulty == 2 && por_carregar == 380)
							{
								cronometro = 31;
							}
						}
						///acrescentar outras dificuldades
					}

					if (global_counter == 60) {
						cronometro--;
						global_counter = 0;

							update_multi_cronometro(cronometro,jogador,pontuacao1,pontuacao2);

					}
					if(cronometro == 0 && cronometro_parado == 0)
					{
						if(jogador == 1)
							jogador = 2;
						else jogador = 1;
						cronometro = 30;
					}
				}
				if (msg.NOTIFY_ARG & irq_set_keyboard)
				{
					kbd_int_handler();
					if(scan_code==TWO_BYTE_CODE){two_bytes = 1;}
					else if(scan_code>>7){
						if(two_bytes) ;
						else ;
						if (scan_code==BREAK_CODE_ESC)
							breaker = 0;
					}
					else
					{
						if (two_bytes){
							two_bytes=0;
						}
						else ;
					}
				}
				if (msg.NOTIFY_ARG & irq_set_mouse)
				{
					rato = getRato();
					mouse_byte = mouse_int_handler();
					if (contador == 0) {
						if (!first_byte(mouse_byte)) {
							contador = 0;
							continue;
						}
					}
					rato->packets[contador] = mouse_byte;
					if (contador == 2) {
						contador = 0;
						{
							updateMouse();

							if(rato->packets[0]&BIT(0) && rato->leftButtonDown == 0)
							{
								rato->leftButtonDown = 1;
								rato->leftButtonReleased = 0;
								int carregado = click_screen(&table,rato->x,rato->y,difficulty,&filled,&por_carregar,1);
								if(carregado != -1 && (rato->x > 32 && rato->x < 992 && rato->y > 186 && rato->y < 698) && carregado != -2)
								{
									if(jogador == 1)
										jogador = 2;
									else jogador = 1;
									cronometro = 30;
								}else if(carregado == -1)
								{
									bombas_por_carregar--;
									if(jogador == 1)
										pontuacao1++;
									else pontuacao2++;
									cronometro = 30;
								}
								if(pontuacao1 >= 51)
								{
									post_game_state_multi(difficulty,irq_set_timer,irq_set_keyboard,irq_set_mouse,1);
									breaker = 0;
								}
								else if(pontuacao2 >= 51){
									post_game_state_multi(difficulty,irq_set_timer,irq_set_keyboard,irq_set_mouse,2);
									breaker = 0;
								}

							}
							else if(!(rato->packets[0]&BIT(0)))
							{
								rato->leftButtonDown = 0;
								rato->leftButtonReleased = 1;
							}
							/*if(rato->packets[0]&BIT(1) && rato->rightButtonDown == 0)
							{
								rato->rightButtonDown = 1;
								rato->rightButtonReleased = 0;
								right_click_screen(&table,rato->x,rato->y,difficulty);
							}
							else if(!(rato->packets[0]&BIT(1)))
							{
								rato->rightButtonDown = 0;
								rato->rightButtonReleased = 1;
							}*/

						}
						continue;
					}
					contador++;

				}
				break;
			}
		}

	}
	free(table);
	deleteBitmap(Player1_table);
	deleteBitmap(Player2_table);
}

int post_game_state_multi(int difficulty,int irq_set_timer,int irq_set_keyboard,int irq_set_mouse,int jogador){
	int contador = 0,breaker = 1,two_bytes = 0, mouse_byte;
	int ipc_status, loops = 0;
	message msg;
	Bitmap* fundo;
	Bitmap* back_button;
	back_button = loadBitmap(PATH"Back_Button.bmp");
	if(jogador == 1)
	{
		fundo = loadBitmap(PATH"Player1_Won.bmp");
	}
	else{
		fundo = loadBitmap(PATH"Player2_Won.bmp");
	}
	drawBitmap(fundo,0,0,ALIGN_LEFT,buffer);
	drawBitmap(back_button,0,0,ALIGN_LEFT,buffer);
	deleteBitmap(fundo);
	deleteBitmap(back_button);
	while (breaker) {
		if (driver_receive(ANY, &msg, &ipc_status) != 0) {
			printf("driver_receive failed with: %d");
			continue;
		}
		//printf("msg.notifyArg: 0x%X\n", msg.NOTIFY_ARG);
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_timer) {
					timer_int_handler();
					if(global_counter % 1 == 0)
					{
						update_screen(0,0,0);
					}

					if (global_counter == 60) {
						global_counter = 0;
					}
				}
				if (msg.NOTIFY_ARG & irq_set_keyboard)
				{
					kbd_int_handler();
					if(scan_code==TWO_BYTE_CODE){two_bytes = 1;}
					else if(scan_code>>7){
						if(two_bytes) ;
						else ;
						if (scan_code==BREAK_CODE_ESC)
							breaker = 0;
					}
					else
					{
						if (two_bytes){
							two_bytes=0;
						}
						else ;
					}
				}
				if (msg.NOTIFY_ARG & irq_set_mouse)
				{

					rato = getRato();
					mouse_byte = mouse_int_handler();
					if (contador == 0) {
						if (!first_byte(mouse_byte)) {
							contador = 0;
							continue;
						}
					}
					rato->packets[contador] = mouse_byte;
					if (contador == 2) {
						contador = 0;
						{
							updateMouse();

							if(rato->packets[0]&BIT(0) && rato->leftButtonDown == 0)
							{
								rato->leftButtonDown = 1;
								rato->leftButtonReleased = 0;
								if(rato->x >=0 && rato->x < 50 && rato->y >=0 && rato->y < 50)
									breaker = 0;
							}
							else if(!(rato->packets[0]&BIT(0)))
							{
								rato->leftButtonDown = 0;
								rato->leftButtonReleased = 1;
							}
							if(rato->packets[0]&BIT(1) && rato->rightButtonDown == 0)
							{
								rato->rightButtonDown = 1;
								rato->rightButtonReleased = 0;
							}
							else if(!(rato->packets[0]&BIT(1)))
							{
								rato->rightButtonDown = 0;
								rato->rightButtonReleased = 1;
							}

						}
						continue;
					}
					contador++;

				}
			}
		}
	}
}

void update_cronometro(int tempo){
	int centenas = (tempo / 100) % 10;
	int dezenas = (tempo / 10) % 10;
	int unidades = tempo % 10;
	switch(centenas){
	case 0:
		drawBitmap(Cron_0,850,100,ALIGN_LEFT,buffer);
		break;
	case 1:
		drawBitmap(Cron_1,850,100,ALIGN_LEFT,buffer);
		break;
	case 2:
		drawBitmap(Cron_2,850,100,ALIGN_LEFT,buffer);
		break;
	case 3:
		drawBitmap(Cron_3,850,100,ALIGN_LEFT,buffer);
		break;
	case 4:
		drawBitmap(Cron_4,850,100,ALIGN_LEFT,buffer);
		break;
	case 5:
		drawBitmap(Cron_5,850,100,ALIGN_LEFT,buffer);
		break;
	case 6:
		drawBitmap(Cron_6,850,100,ALIGN_LEFT,buffer);
		break;
	case 7:
		drawBitmap(Cron_7,850,100,ALIGN_LEFT,buffer);
		break;
	case 8:
		drawBitmap(Cron_8,850,100,ALIGN_LEFT,buffer);
		break;
	case 9:
		drawBitmap(Cron_9,850,100,ALIGN_LEFT,buffer);
		break;
	}
	switch(dezenas)
	{
	case 0:
		drawBitmap(Cron_0,866,100,ALIGN_LEFT,buffer);
		break;
	case 1:
		drawBitmap(Cron_1,866,100,ALIGN_LEFT,buffer);
		break;
	case 2:
		drawBitmap(Cron_2,866,100,ALIGN_LEFT,buffer);
		break;
	case 3:
		drawBitmap(Cron_3,866,100,ALIGN_LEFT,buffer);
		break;
	case 4:
		drawBitmap(Cron_4,866,100,ALIGN_LEFT,buffer);
		break;
	case 5:
		drawBitmap(Cron_5,866,100,ALIGN_LEFT,buffer);
		break;
	case 6:
		drawBitmap(Cron_6,866,100,ALIGN_LEFT,buffer);
		break;
	case 7:
		drawBitmap(Cron_7,866,100,ALIGN_LEFT,buffer);
		break;
	case 8:
		drawBitmap(Cron_8,866,100,ALIGN_LEFT,buffer);
		break;
	case 9:
		drawBitmap(Cron_9,866,100,ALIGN_LEFT,buffer);
		break;
	}
	switch(unidades)
	{
	case 0:
		drawBitmap(Cron_0,882,100,ALIGN_LEFT,buffer);
		break;
	case 1:
		drawBitmap(Cron_1,882,100,ALIGN_LEFT,buffer);
		break;
	case 2:
		drawBitmap(Cron_2,882,100,ALIGN_LEFT,buffer);
		break;
	case 3:
		drawBitmap(Cron_3,882,100,ALIGN_LEFT,buffer);
		break;
	case 4:
		drawBitmap(Cron_4,882,100,ALIGN_LEFT,buffer);
		break;
	case 5:
		drawBitmap(Cron_5,882,100,ALIGN_LEFT,buffer);
		break;
	case 6:
		drawBitmap(Cron_6,882,100,ALIGN_LEFT,buffer);
		break;
	case 7:
		drawBitmap(Cron_7,882,100,ALIGN_LEFT,buffer);
		break;
	case 8:
		drawBitmap(Cron_8,882,100,ALIGN_LEFT,buffer);
		break;
	case 9:
		drawBitmap(Cron_9,882,100,ALIGN_LEFT,buffer);
		break;
	}
}

void delete_cronometro(){
	deleteBitmap(Cron_0);
	deleteBitmap(Cron_1);
	deleteBitmap(Cron_2);
	deleteBitmap(Cron_3);
	deleteBitmap(Cron_4);
	deleteBitmap(Cron_5);
	deleteBitmap(Cron_6);
	deleteBitmap(Cron_7);
	deleteBitmap(Cron_8);
	deleteBitmap(Cron_9);

}

void load_cronometro(){
	Cron_0 = loadBitmap(PATH"Cron_0.bmp");
	Cron_1 = loadBitmap(PATH"Cron_1.bmp");
	Cron_2 = loadBitmap(PATH"Cron_2.bmp");
	Cron_3 = loadBitmap(PATH"Cron_3.bmp");
	Cron_4 = loadBitmap(PATH"Cron_4.bmp");
	Cron_5 = loadBitmap(PATH"Cron_5.bmp");
	Cron_6 = loadBitmap(PATH"Cron_6.bmp");
	Cron_7 = loadBitmap(PATH"Cron_7.bmp");
	Cron_8 = loadBitmap(PATH"Cron_8.bmp");
	Cron_9 = loadBitmap(PATH"Cron_9.bmp");
}

void update_multi_cronometro(int tempo, int jogador,int jogador1,int jogador2){
	int dezenas = (tempo / 10) % 10;
	int unidades = tempo % 10;

	if(jogador == 1)
	{
		switch(dezenas)
			{
			case 0:
				drawBitmap(Cron_0,32,104,ALIGN_LEFT,buffer);
				break;
			case 1:
				drawBitmap(Cron_1,32,104,ALIGN_LEFT,buffer);
				break;
			case 2:
				drawBitmap(Cron_2,32,104,ALIGN_LEFT,buffer);
				break;
			case 3:
				drawBitmap(Cron_3,32,104,ALIGN_LEFT,buffer);
				break;
			}
		switch(unidades)
		{
		case 0:
			drawBitmap(Cron_0,48,104,ALIGN_LEFT,buffer);
			break;
		case 1:
			drawBitmap(Cron_1,48,104,ALIGN_LEFT,buffer);
			break;
		case 2:
			drawBitmap(Cron_2,48,104,ALIGN_LEFT,buffer);
			break;
		case 3:
			drawBitmap(Cron_3,48,104,ALIGN_LEFT,buffer);
			break;
		case 4:
			drawBitmap(Cron_4,48,104,ALIGN_LEFT,buffer);
			break;
		case 5:
			drawBitmap(Cron_5,48,104,ALIGN_LEFT,buffer);
			break;
		case 6:
			drawBitmap(Cron_6,48,104,ALIGN_LEFT,buffer);
			break;
		case 7:
			drawBitmap(Cron_7,48,104,ALIGN_LEFT,buffer);
			break;
		case 8:
			drawBitmap(Cron_8,48,104,ALIGN_LEFT,buffer);
			break;
		case 9:
			drawBitmap(Cron_9,48,104,ALIGN_LEFT,buffer);
			break;
		}
		drawBitmap(Cron_9,960,104,ALIGN_LEFT,buffer);
		drawBitmap(Cron_9,976,104,ALIGN_LEFT,buffer);
	}else{
		switch(dezenas)
		{
		case 0:
			drawBitmap(Cron_0,960,104,ALIGN_LEFT,buffer);
			break;
		case 1:
			drawBitmap(Cron_1,960,104,ALIGN_LEFT,buffer);
			break;
		case 2:
			drawBitmap(Cron_2,960,104,ALIGN_LEFT,buffer);
			break;
		case 3:
			drawBitmap(Cron_3,960,104,ALIGN_LEFT,buffer);
			break;
		}
		switch(unidades)
		{
		case 0:
			drawBitmap(Cron_0,976,104,ALIGN_LEFT,buffer);
			break;
		case 1:
			drawBitmap(Cron_1,976,104,ALIGN_LEFT,buffer);
			break;
		case 2:
			drawBitmap(Cron_2,976,104,ALIGN_LEFT,buffer);
			break;
		case 3:
			drawBitmap(Cron_3,976,104,ALIGN_LEFT,buffer);
			break;
		case 4:
			drawBitmap(Cron_4,976,104,ALIGN_LEFT,buffer);
			break;
		case 5:
			drawBitmap(Cron_5,976,104,ALIGN_LEFT,buffer);
			break;
		case 6:
			drawBitmap(Cron_6,976,104,ALIGN_LEFT,buffer);
			break;
		case 7:
			drawBitmap(Cron_7,976,104,ALIGN_LEFT,buffer);
			break;
		case 8:
			drawBitmap(Cron_8,976,104,ALIGN_LEFT,buffer);
			break;
		case 9:
			drawBitmap(Cron_9,976,104,ALIGN_LEFT,buffer);
			break;
		}
		drawBitmap(Cron_9,32,104,ALIGN_LEFT,buffer);
		drawBitmap(Cron_9,48,104,ALIGN_LEFT,buffer);
	}
	dezenas = (jogador1 / 10) % 10;
	unidades = jogador1 % 10;
	switch(dezenas)
	{
	case 0:
		drawBitmap(Cron_0,112,104,ALIGN_LEFT,buffer);
		break;
	case 1:
		drawBitmap(Cron_1,112,104,ALIGN_LEFT,buffer);
		break;
	case 2:
		drawBitmap(Cron_2,112,104,ALIGN_LEFT,buffer);
		break;
	case 3:
		drawBitmap(Cron_3,112,104,ALIGN_LEFT,buffer);
		break;
	case 4:
		drawBitmap(Cron_4,112,104,ALIGN_LEFT,buffer);
		break;
	case 5:
		drawBitmap(Cron_5,112,104,ALIGN_LEFT,buffer);
		break;
	}
	switch(unidades)
	{
	case 0:
		drawBitmap(Cron_0,128,104,ALIGN_LEFT,buffer);
		break;
	case 1:
		drawBitmap(Cron_1,128,104,ALIGN_LEFT,buffer);
		break;
	case 2:
		drawBitmap(Cron_2,128,104,ALIGN_LEFT,buffer);
		break;
	case 3:
		drawBitmap(Cron_3,128,104,ALIGN_LEFT,buffer);
		break;
	case 4:
		drawBitmap(Cron_4,128,104,ALIGN_LEFT,buffer);
		break;
	case 5:
		drawBitmap(Cron_5,128,104,ALIGN_LEFT,buffer);
		break;
	case 6:
		drawBitmap(Cron_6,128,104,ALIGN_LEFT,buffer);
		break;
	case 7:
		drawBitmap(Cron_7,128,104,ALIGN_LEFT,buffer);
		break;
	case 8:
		drawBitmap(Cron_8,128,104,ALIGN_LEFT,buffer);
		break;
	case 9:
		drawBitmap(Cron_9,128,104,ALIGN_LEFT,buffer);
		break;
	}
	dezenas = (jogador2 / 10) % 10;
	unidades = jogador2 % 10;
	switch(dezenas)
	{
	case 0:
		drawBitmap(Cron_0,880,104,ALIGN_LEFT,buffer);
		break;
	case 1:
		drawBitmap(Cron_1,880,104,ALIGN_LEFT,buffer);
		break;
	case 2:
		drawBitmap(Cron_2,880,104,ALIGN_LEFT,buffer);
		break;
	case 3:
		drawBitmap(Cron_3,880,104,ALIGN_LEFT,buffer);
		break;
	case 4:
		drawBitmap(Cron_4,880,104,ALIGN_LEFT,buffer);
		break;
	case 5:
		drawBitmap(Cron_5,880,104,ALIGN_LEFT,buffer);
		break;
	}
	switch(unidades)
	{
	case 0:
		drawBitmap(Cron_0,896,104,ALIGN_LEFT,buffer);
		break;
	case 1:
		drawBitmap(Cron_1,896,104,ALIGN_LEFT,buffer);
		break;
	case 2:
		drawBitmap(Cron_2,896,104,ALIGN_LEFT,buffer);
		break;
	case 3:
		drawBitmap(Cron_3,896,104,ALIGN_LEFT,buffer);
		break;
	case 4:
		drawBitmap(Cron_4,896,104,ALIGN_LEFT,buffer);
		break;
	case 5:
		drawBitmap(Cron_5,896,104,ALIGN_LEFT,buffer);
		break;
	case 6:
		drawBitmap(Cron_6,896,104,ALIGN_LEFT,buffer);
		break;
	case 7:
		drawBitmap(Cron_7,896,104,ALIGN_LEFT,buffer);
		break;
	case 8:
		drawBitmap(Cron_8,896,104,ALIGN_LEFT,buffer);
		break;
	case 9:
		drawBitmap(Cron_9,896,104,ALIGN_LEFT,buffer);
		break;
	}
}

int jogo_multi_player_porta(int difficulty,int irq_set_timer,int irq_set_keyboard,int irq_set_mouse, int host) {
	Bitmap* bitmap_table;
	Bitmap* fundo_jogo;
	Bitmap* Player1_table;
	Bitmap* Player2_table;
	Player1_table = loadBitmap(PATH"Player1_table.bmp");
	Player2_table = loadBitmap(PATH"Player2_table.bmp");
	bitmap_table = loadBitmap(PATH"Tabela_Expert.bmp");
	fundo_jogo = loadBitmap(PATH"Fundo_Jogo.bmp");
	int contador = 0,breaker = 1,two_bytes = 0, mouse_byte; /// mouse e ciclo while
	int ipc_status, loops = 0;///cenas das interrupcoes
	message msg;
	int filled = 1,por_carregar,cronometro = 30,jogador,pontuacao1 = 0,pontuacao2 = 0,bombas_por_carregar,cronometro_parado = 1,pont_max = 51;///jogo
	if(difficulty == 2)
	{
		por_carregar = 380;
		bombas_por_carregar = 100;
	}
	else if (difficulty == 1)
		bombas_por_carregar = 0; /// alterar
	else bombas_por_carregar = 0; // alterar
	int turn = 1;
	if(host == 1)
		jogador = 1;
	else jogador = 2;
	Mine** table = create_table(difficulty);
	unsigned short addr = COM1_ADDR;
	breaker = connection_state(&table,difficulty,irq_set_timer, irq_set_keyboard, irq_set_mouse, host);
	drawBitmap(fundo_jogo,0,0,ALIGN_LEFT,buffer);
	drawBitmap(bitmap_table,0,0,ALIGN_LEFT,buffer);
	drawBitmap(Player1_table,32,50,ALIGN_LEFT,buffer);
	drawBitmap(Player2_table,880,50,ALIGN_LEFT,buffer);
	deleteBitmap(fundo_jogo);
	deleteBitmap(bitmap_table);
	while (breaker) {
		if (driver_receive(ANY, &msg, &ipc_status) != 0) {
			printf("driver_receive failed with: %d");
			continue;
		}
		//printf("msg.notifyArg: 0x%X\n", msg.NOTIFY_ARG);
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_timer) {
					timer_int_handler();
					if(global_counter % 1 == 0)
					{
						update_screen(jogador,turn,0);
					}
					if(turn != jogador)
					{
						char resposta;
						if(getCharOne(addr,&resposta) != 1)
						{
								unsigned char x1,x2,y1,y2;
								int x,y;
								x1 = resposta;
								getChar(addr,&resposta);
								x2 = resposta;
								x = (x2 << 8) | x1;
								if(x1 == 'e' && x2 == 'e')
								{
									end_scree_multi_porta(irq_set_timer,irq_set_keyboard,irq_set_mouse,0,jogador);
									breaker = 0;
									goto skip;
								}
								getChar(addr,&resposta);
								y1 = resposta;
								getChar(addr,&resposta);
								y2 = resposta;
								y =  (y2 << 8) | y1;
								//////
								printf("x = %d y = %d ",x,y);
								int carregado = click_screen(&table,x,y,difficulty,&filled,&por_carregar,1);
								if(carregado != -1 && (rato->x > 32 && rato->x < 992 && rato->y > 186 && rato->y < 698) && carregado != -2)
								{
									turn = jogador;
									cronometro = 30;
								}
								else if(carregado == -1)
								{
									bombas_por_carregar--;
									if(turn == 1)
										pontuacao1++;
									else pontuacao2++;
									if(pontuacao1 >= pont_max || pontuacao2 >= pont_max)
									{
										end_scree_multi_porta(irq_set_timer,irq_set_keyboard,irq_set_mouse,turn,jogador);
										breaker = 0;
										goto skip;
									}
									cronometro = 30;
								}
								skip:
								;
						}
					}
					else{
						char resposta;
						if(getCharOne(addr,&resposta) != 1)
						{
							if(resposta == 'e')
							{
								getCharOne(addr,&resposta);
								if(resposta == 'e')
								{
									end_scree_multi_porta(irq_set_timer,irq_set_keyboard,irq_set_mouse,0,jogador);
									breaker = 0;
								}
							}
						}
					}
					if(cronometro_parado)
					{
						if(difficulty == 2 && por_carregar != 380)
						{
							cronometro_parado = 0;
							cronometro = 30;
						}
						///acrescentar outras dificuldades
					}
					else
					{
						if(difficulty == 2 && por_carregar == 380)
						{
							cronometro = 30;						}
					}
					if (global_counter == 60) {
						cronometro--;
						global_counter = 0;

						update_multi_cronometro(cronometro,turn,pontuacao1,pontuacao2);

					}
					if(cronometro == 0 && cronometro_parado == 0)
					{
						if(turn == 1)
							turn = 2;
						else turn = 1;
						cronometro = 30;
					}
				}
				if (msg.NOTIFY_ARG & irq_set_keyboard)
				{
					kbd_int_handler();
					if(scan_code==TWO_BYTE_CODE){two_bytes = 1;}
					else if(scan_code>>7){
						if(two_bytes) ;
						else ;
						if (scan_code==BREAK_CODE_ESC)
						{
							sendChar(addr,'e');
							sendChar(addr,'e');
							end_scree_multi_porta(irq_set_timer,irq_set_keyboard,irq_set_mouse,0,jogador);
							breaker = 0;
						}
					}
					else
					{
						if (two_bytes){
							two_bytes=0;
						}
						else ;
					}
				}
				if (msg.NOTIFY_ARG & irq_set_mouse)
				{
					rato = getRato();
					mouse_byte = mouse_int_handler();
					if (contador == 0) {
						if (!first_byte(mouse_byte)) {
							contador = 0;
							continue;
						}
					}
					rato->packets[contador] = mouse_byte;
					if (contador == 2) {
						contador = 0;
						{
							updateMouse();

							if(rato->packets[0]&BIT(0) && rato->leftButtonDown == 0)
							{
								rato->leftButtonDown = 1;
								rato->leftButtonReleased = 0;
								if (turn == jogador){
									int carregado = click_screen(&table,rato->x,rato->y,difficulty,&filled,&por_carregar,1);
									unsigned char x1,x2,y1,y2;
									x1 = rato->x & 0xFF;
									x2 = rato->x >> 8;
									y1 = rato->y & 0xFF;
									y2 = rato->y >> 8;
									sendChar(addr,x1);
									sendChar(addr,x2);
									sendChar(addr,y1);
									sendChar(addr,y2);
									if(carregado != -1 && (rato->x > 32 && rato->x < 992 && rato->y > 186 && rato->y < 698) && carregado != -2)
									{
										if(turn == 1)
											turn = 2;
										else turn = 1;
										cronometro = 30;
									}
									else if(carregado == -1)
									{
										bombas_por_carregar--;
										if(turn == 1)
											pontuacao1++;
										else pontuacao2++;
										if(pontuacao1 >= pont_max || pontuacao2 >= pont_max)
										{
											end_scree_multi_porta(irq_set_timer,irq_set_keyboard,irq_set_mouse,turn,jogador);
											breaker = 0;
											goto skip;
										}
										cronometro = 30;
									}
								}
							}
							else if(!(rato->packets[0]&BIT(0)))
							{
								rato->leftButtonDown = 0;
								rato->leftButtonReleased = 1;
							}
							/*if(rato->packets[0]&BIT(1) && rato->rightButtonDown == 0)
								{
									rato->rightButtonDown = 1;
									rato->rightButtonReleased = 0;
									right_click_screen(&table,rato->x,rato->y,difficulty);
								}
								else if(!(rato->packets[0]&BIT(1)))
								{
									rato->rightButtonDown = 0;
									rato->rightButtonReleased = 1;
								}*/

						}
						continue;
					}
					contador++;

				}
				break;
			}
		}

	}
	deleteBitmap(Player1_table);
	deleteBitmap(Player2_table);
	free(table);
}

void draw_connection_state(){
	Bitmap* fundo;
	Bitmap* back_button;
	fundo = loadBitmap(PATH"Connecting.bmp");
	back_button = loadBitmap(PATH"Back_Button.bmp");
	drawBitmap(fundo,0,0,ALIGN_LEFT,buffer);
	drawBitmap(back_button,0,0,ALIGN_LEFT,buffer);
	deleteBitmap(fundo);
	deleteBitmap(back_button);
}

int connection_state(Mine*** table,int difficulty,int irq_set_timer,int irq_set_keyboard,int irq_set_mouse, int host)
{
	int sent = 0;
	global_counter = 0;
	int contador = 0,breaker = 1,two_bytes = 0, mouse_byte; /// mouse e ciclo while
	int ipc_status, loops = 0;///cenas das interrupcoes
	int connected = 0;
	message msg;
	unsigned short addr = COM1_ADDR;
	draw_connection_state();
	while (breaker) {
		if (driver_receive(ANY, &msg, &ipc_status) != 0) {
			printf("driver_receive failed with: %d");
			continue;
		}
		//printf("msg.notifyArg: 0x%X\n", msg.NOTIFY_ARG);
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_timer) {
					timer_int_handler();
					if(global_counter % 1 == 0)
					{
						update_screen(0,0,0);
					}
					////
					char resposta;
					if(host == 1 && connected == 0)
					{
						if(getCharOne(addr,&resposta) != 1)
						{
							if(resposta == 'c'){
								sendChar(addr,'s');
								connected = 1;
							}
						}
					}
					else if(host == 0 && connected == 0)
					{
						if(sent == 0)
						{
							sendChar(addr,'c');
							sent = 1;
						}

						if(getCharOne(addr,&resposta) != 1)
						{
							if(resposta == 's'){
								connected = 1;
							}
						}

					}
					if(connected == 1){
						breaker = 0;
					}


					////
					if (global_counter == 60) {
						global_counter = 0;
					}
				}
				if (msg.NOTIFY_ARG & irq_set_keyboard)
				{
					kbd_int_handler();
					if(scan_code==TWO_BYTE_CODE){two_bytes = 1;}
					else if(scan_code>>7){
						if(two_bytes) ;
						else ;
						if (scan_code==BREAK_CODE_ESC){
							char seed;
							getCharOne(addr,&seed);
							return 0;
						}
					}
					else
					{
						if (two_bytes){
							two_bytes=0;
						}
						else ;
					}
				}
				if (msg.NOTIFY_ARG & irq_set_mouse)
				{
					rato = getRato();
					mouse_byte = mouse_int_handler();
					if (contador == 0) {
						if (!first_byte(mouse_byte)) {
							contador = 0;
							continue;
						}
					}
					rato->packets[contador] = mouse_byte;
					if (contador == 2) {
						contador = 0;
						{
							updateMouse();

							if(rato->packets[0]&BIT(0) && rato->leftButtonDown == 0)
							{
								rato->leftButtonDown = 1;
								rato->leftButtonReleased = 0;
								if(rato->x >=0 && rato->x < 50 && rato->y >=0 && rato->y < 50)
									return 0;
							}
							else if(!(rato->packets[0]&BIT(0)))
							{
								rato->leftButtonDown = 0;
								rato->leftButtonReleased = 1;
							}
							if(rato->packets[0]&BIT(1) && rato->rightButtonDown == 0)
							{
								rato->rightButtonDown = 1;
								rato->rightButtonReleased = 0;
							}
							else if(!(rato->packets[0]&BIT(1)))
							{
								rato->rightButtonDown = 0;
								rato->rightButtonReleased = 1;
							}

						}
						continue;
					}
					contador++;

				}
				break;
			}
		}
	}
	char seed;
	if(host == 0)
	{

		*table = fill_table(*table,difficulty,0,0,0,&seed,0);
		sendChar(addr,seed);
	}
	else{
		getChar(addr,&seed);
		*table = fill_table(*table,difficulty,0,0,0,&seed,1);
	}
	while(getCharOne(addr,&seed) != 1){};
	return 1;
}

void draw_end_scree_multi_porta(int turn,int jogador){
	Bitmap* fundo;
	Bitmap* back_button;
	if(turn == 0)
		fundo = loadBitmap(PATH"Disconnected.bmp");
	else if(turn == jogador){
		fundo = loadBitmap(PATH"Fundo_Won.bmp");
	}
	else{
		fundo = loadBitmap(PATH"Fundo_Lost.bmp");
	}
	back_button = loadBitmap(PATH"Back_Button.bmp");
	drawBitmap(fundo,0,0,ALIGN_LEFT,buffer);
	drawBitmap(back_button,0,0,ALIGN_LEFT,buffer);
	deleteBitmap(fundo);
	deleteBitmap(back_button);
}

void end_scree_multi_porta(int irq_set_timer,int irq_set_keyboard,int irq_set_mouse,int turn,int jogador){
	printf ("entra\n");
		global_counter = 0;
		int contador = 0,breaker = 1,two_bytes = 0, mouse_byte; /// mouse e ciclo while
		int ipc_status, loops = 0;///cenas das interrupcoes
		int connected = 0;
		message msg;
		unsigned short addr = COM1_ADDR;
		draw_end_scree_multi_porta(turn,jogador);
		printf ("entra\n");
		while (breaker) {
			if (driver_receive(ANY, &msg, &ipc_status) != 0) {
				printf("driver_receive failed with: %d");
				continue;
			}
			//printf("msg.notifyArg: 0x%X\n", msg.NOTIFY_ARG);
			if (is_ipc_notify(ipc_status)) {
				switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE:
					if (msg.NOTIFY_ARG & irq_set_timer) {
						timer_int_handler();
						if(global_counter % 1 == 0)
						{
							update_screen(0,0,0);
						}
						////


						////
						if (global_counter == 60) {
							global_counter = 0;
						}
					}
					if (msg.NOTIFY_ARG & irq_set_keyboard)
					{
						kbd_int_handler();
						if(scan_code==TWO_BYTE_CODE){two_bytes = 1;}
						else if(scan_code>>7){
							if(two_bytes) ;
							else ;
							if (scan_code==BREAK_CODE_ESC)
								return;
						}
						else
						{
							if (two_bytes){
								two_bytes=0;
							}
							else ;
						}
					}
					if (msg.NOTIFY_ARG & irq_set_mouse)
					{
						rato = getRato();
						mouse_byte = mouse_int_handler();
						if (contador == 0) {
							if (!first_byte(mouse_byte)) {
								contador = 0;
								continue;
							}
						}
						rato->packets[contador] = mouse_byte;
						if (contador == 2) {
							contador = 0;
							{
								updateMouse();

								if(rato->packets[0]&BIT(0) && rato->leftButtonDown == 0)
								{
									rato->leftButtonDown = 1;
									rato->leftButtonReleased = 0;
									if(rato->x >=0 && rato->x < 50 && rato->y >=0 && rato->y < 50)
										return;
								}
								else if(!(rato->packets[0]&BIT(0)))
								{
									rato->leftButtonDown = 0;
									rato->leftButtonReleased = 1;
								}
								if(rato->packets[0]&BIT(1) && rato->rightButtonDown == 0)
								{
									rato->rightButtonDown = 1;
									rato->rightButtonReleased = 0;
								}
								else if(!(rato->packets[0]&BIT(1)))
								{
									rato->rightButtonDown = 0;
									rato->rightButtonReleased = 1;
								}

							}
							continue;
						}
						contador++;

					}
					break;
				}
			}
		}

}
