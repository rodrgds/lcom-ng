#include <minix/syslib.h>
#include <minix/drivers.h>
#include <time.h>

#include "TUI.h"
#include "video_txt.h"
unsigned char packet[3];
static int cor_linhas=RED;
static int cor_texto=GREEN;
static int cor_rato=GREY;
int jogo=1;


int pontoMedio(int x, int y)
{
	int a, temp;
	if(x<y)
	{
		temp=x;
		x=y;
		y=temp;
	}
	a=(x-y)/2;
	return a;
}

int desenhaLinha(int x, int y, int fin, int ori)
{
	int i;
	if(ori==0)
	{
		for(i=y; i<fin; i++)
		{
			vt_print_char(0xB3, i, x, cor_linhas);
		}
	}
	if(ori==1)
	{
		for(i=x; i<fin; i++)
		{
			vt_print_char(0xC4, y, i, cor_linhas);
		}
	}
}

int desenhaBotao(botao bot)
{
	vt_draw_frame(bot.comp, bot.alt, bot.y, bot.x, cor_linhas);
	vt_print_string(bot.name, (bot.y+1), (bot.x+1), cor_texto);
}

void set_pos_rato(Rato * r, short newx, short newy)
{
	r->x = r->x + (newx/10); /*define o x*/
	r->y = r->y - (newy/24); /*define o y*/
	if(r->x<0)
	{r->x=0;} /*limita o x ao ecra*/
	if(r->x>79/* PREENCHER valor maximo de casinhas x*/)
	{r->x=79;} /*limita o x ao ecra*/
	if(r->y<0)
	{r->y=0;} /*limita o y ao ecra*/
	if(r->y>24/* PREENCHER valor maximo de casinhas y*/)
	{r->y=24;} /*limita o y ao ecra*/
}

void desenhar_rato(Rato * r, int x, int y)
{
	vt_print_char(0xDB, y, x, cor_rato);/*verificar se isto funciona com os hex e etc*/
}

void MenuPrincipal(Rato * r)
{
	botao Jogo_Amigavel;
	botao Jogo_Rapido;
	botao Opcoes;
	botao Sair;
	/*Rato r;*/
	short x,y;
	int i=0;
	int ipc_status;
	unsigned long irq_mouse;
	message msg;
	int count =0;
	vt_fill(00, 00);
	r->x=0;
	r->y=0;
	r->lb=0;
	r->rb=0;

	/*-----------------------*/
	Jogo_Amigavel.x = 2;
	Jogo_Amigavel.y = 4;
	Jogo_Amigavel.comp = 15;
	Jogo_Amigavel.alt = 3;
	Jogo_Amigavel.name = "Jogo Amigavel";

	/*-----------------------*/
	Jogo_Rapido.x = 2;
	Jogo_Rapido.y = 7;
	Jogo_Rapido.comp = 15;
	Jogo_Rapido.alt = 3;
	Jogo_Rapido.name = "Jogo Rapido";

	/*-----------------------*/
	Opcoes.x = 2;
	Opcoes.y = 10;
	Opcoes.comp = 15;
	Opcoes.alt = 3;
	Opcoes.name = "Opcoes";

	/*-----------------------*/
	Sair.x = 2;
	Sair.y = 21;
	Sair.comp = 15;
	Sair.alt = 3;
	Sair.name = "Sair";

	desenhaBotao(Jogo_Amigavel);
	desenhaBotao(Jogo_Rapido);
	desenhaBotao(Opcoes);
	desenhaBotao(Sair);

	while(jogo) { 			/*while(de hoje a 8 == pra semana) */
		/* Get a request message. */

		if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			printf("driver_receive failed");
			continue;
		}
		if (is_ipc_notify(ipc_status)) { 		/* received notification */
			if (_ENDPOINT_P(msg.m_source) == HARDWARE) {
				/* hardware interrupt notification */
				if (msg.NOTIFY_ARG & r->irq) { /* subscribed interrupt */
					switch (count) {
					case 3:
						count = 0;
					case 0:
						packet[0] = mouse_read();
						if (packet[0] == (unsigned char)(-1)) {
							break;
						}
						if (packet[0] & BIT(3)) {
							/*	printf(" B1=0x%X", packet[0]);*/
							count++;
						}
						break;
					case 1:
						packet[1] = mouse_read();
						/*printf(" B2=0x%X", packet[1]);*/
						count++;
						break;
					case 2:
						packet[2] = mouse_read();
						x=( (packet[0] & MOUSE_XSIGN ? -((packet[1] ^ 0xFF) + 1) : packet[1]));
						y=((packet[0] & MOUSE_YSIGN ? -((packet[2] ^ 0xFF) + 1) : packet[2]));
						r->lb = (packet[0] & BIT(0));
						r->rb = (packet[0] & BIT(1));

						/*printf("X: %c%d\n", (packet[0] & MOUSE_XSIGN ? '-' : 0x00), (packet[0] & MOUSE_XSIGN ? (packet[1] ^ 0xFF) + 1 : packet[1]));
						printf("Y: %c%d\n", (packet[0] & MOUSE_YSIGN ? '-' : 0x00), (packet[0] & MOUSE_YSIGN ? (packet[2] ^ 0xFF) + 1 : packet[2]));
						printf("x:%d\ny:%d\n\n\n",r->x,r->y);*/
						count++;
						set_pos_rato(r, x, y);
						if(((r->x>=Jogo_Amigavel.x) && (r->x<(Jogo_Amigavel.x+Jogo_Amigavel.comp))) &&
								((r->y>=Jogo_Amigavel.y) && (r->y<(Jogo_Amigavel.y+Jogo_Amigavel.alt))) && (r->lb==1))
						{
							EscEquipas(r);
						}
						if(((r->x>=Jogo_Rapido.x) && (r->x<(Jogo_Rapido.x+Jogo_Rapido.comp))) &&
								((r->y>=Jogo_Rapido.y) && (r->y<(Jogo_Rapido.y+Jogo_Rapido.alt))) && (r->lb==1))
						{
							JogoRapido(r);
						}
						if(((r->x>=Opcoes.x) && (r->x<(Opcoes.x+Opcoes.comp))) &&
								((r->y>=Opcoes.y) && (r->y<(Opcoes.y+Opcoes.alt))) && (r->lb==1))
						{
							speaker_ctrl(1);
							speaker_timer_ctrl(1);
							_Opcoes(r);
							speaker_ctrl(0);
							speaker_timer_ctrl(0);
						}
						if(((r->x>=Sair.x) && (r->x<(Sair.x+Sair.comp))) &&
								((r->y>=Sair.y) && (r->y<(Sair.y+Sair.alt))) && (r->lb==1))
						{
							_Sair();
						}

						vt_fill(00, 00);
						desenhaBotao(Jogo_Amigavel);
						desenhaBotao(Jogo_Rapido);
						desenhaBotao(Opcoes);
						desenhaBotao(Sair);
						desenhar_rato(r, r->x, r->y);
						break;
					}
				}

			}
		}
	}
	mouse_unsubscribe_int();
}

void JogoAmigavel(Rato * r)
{
	EscEquipas(r);
}

void JogoRapido(Rato * r)
{
	criaEquipasTacticas();
	equipa1=equipa3;
	equipa2=equipa4;
	PreJogo(r);
}

void _Opcoes(Rato * r)
{
	botao Alterar_Texto;
	botao Alterar_Linhas;
	botao Alterar_Dificuldade;
	botao Alterar_Rato;
	botao Som;
	botao Voltar;
	short x,y;
	int i=0;
	int ipc_status;
	unsigned long irq_mouse;
	message msg;
	int j;
	int count =0;
	j=0;
	vt_fill(00, 00);
	r->x=0;
	r->y=0;
	r->lb=0;
	r->rb=0;


	/*-----------------------*/
	Alterar_Texto.x = 2;
	Alterar_Texto.y = 4;
	Alterar_Texto.comp = 24;
	Alterar_Texto.alt = 3;
	Alterar_Texto.name = "Alterar Cor do Texto";

	/*-----------------------*/
	Alterar_Linhas.x = 2;
	Alterar_Linhas.y = 7;
	Alterar_Linhas.comp = 24;
	Alterar_Linhas.alt = 3;
	Alterar_Linhas.name = "Alterar Cor das Linhas";

	/*-----------------------*/
	Alterar_Rato.x = 2;
	Alterar_Rato.y = 10;
	Alterar_Rato.comp = 24;
	Alterar_Rato.alt = 3;
	Alterar_Rato.name = "Alterar Cor do Rato";
	/*-----------------------*/
	Alterar_Dificuldade.x = 2;
	Alterar_Dificuldade.y = 13;
	Alterar_Dificuldade.comp = 24;
	Alterar_Dificuldade.alt = 3;
	Alterar_Dificuldade.name = "Alterar Dificuldade";

	/*-----------------------*/
	Som.x = 2;
	Som.y = 16;
	Som.comp = 24;
	Som.alt = 3;
	Som.name = "Som";

	/*-----------------------*/
	Voltar.x = 2;
	Voltar.y = 21;
	Voltar.comp = 17;
	Voltar.alt = 3;
	Voltar.name = "Voltar";

	desenhaBotao(Alterar_Texto);
	desenhaBotao(Alterar_Linhas);
	desenhaBotao(Alterar_Rato);
	desenhaBotao(Alterar_Dificuldade);
	desenhaBotao(Som);
	desenhaBotao(Voltar);
	while(jogo) { 			/*while(de hoje a 8 == pra semana) */
		/* Get a request message. */

		if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			printf("driver_receive failed");
			continue;
		}
		if (is_ipc_notify(ipc_status)) { 		/* received notification */
			if (_ENDPOINT_P(msg.m_source) == HARDWARE) {
				/* hardware interrupt notification */
				if (msg.NOTIFY_ARG & r->irq) { /* subscribed interrupt */
					switch (count) {
					case 3:
						count = 0;
					case 0:
						packet[0] = mouse_read();
						if (packet[0] == (unsigned char)(-1)) {
							break;
						}
						if (packet[0] & BIT(3)) {
							/*	printf(" B1=0x%X", packet[0]);*/
							count++;
						}
						break;
					case 1:
						packet[1] = mouse_read();
						/*printf(" B2=0x%X", packet[1]);*/
						count++;
						break;
					case 2:
						packet[2] = mouse_read();
						x=( (packet[0] & MOUSE_XSIGN ? -((packet[1] ^ 0xFF) + 1) : packet[1]));
						y=((packet[0] & MOUSE_YSIGN ? -((packet[2] ^ 0xFF) + 1) : packet[2]));
						r->lb = (packet[0] & BIT(0));
						r->rb = (packet[0] & BIT(1));

						/*printf("X: %c%d\n", (packet[0] & MOUSE_XSIGN ? '-' : 0x00), (packet[0] & MOUSE_XSIGN ? (packet[1] ^ 0xFF) + 1 : packet[1]));
							printf("Y: %c%d\n", (packet[0] & MOUSE_YSIGN ? '-' : 0x00), (packet[0] & MOUSE_YSIGN ? (packet[2] ^ 0xFF) + 1 : packet[2]));
							printf("x:%d\ny:%d\n\n\n",r->x,r->y);*/
						count++;
						set_pos_rato(r, x, y);
						if(((r->x>=Alterar_Texto.x) && (r->x<(Alterar_Texto.x+Alterar_Texto.comp))) &&
								((r->y>=Alterar_Texto.y) && (r->y<(Alterar_Texto.y+Alterar_Texto.alt))) && (r->lb==1))
						{
							AltCorLetras(r);
						}
						if(((r->x>=Alterar_Linhas.x) && (r->x<(Alterar_Linhas.x+Alterar_Linhas.comp))) &&
								((r->y>=Alterar_Linhas.y) && (r->y<(Alterar_Linhas.y+Alterar_Linhas.alt))) && (r->lb==1))
						{
							AltCorLinhas(r);
						}
						if(((r->x>=Alterar_Rato.x) && (r->x<(Alterar_Rato.x+Alterar_Rato.comp))) &&
								((r->y>=Alterar_Rato.y) && (r->y<(Alterar_Rato.y+Alterar_Rato.alt))) && (r->lb==1))
						{
							AltCorRato(r);
						}
						if(((r->x>=Alterar_Dificuldade.x) && (r->x<(Alterar_Dificuldade.x+Alterar_Dificuldade.comp))) &&
								((r->y>=Alterar_Dificuldade.y) && (r->y<(Alterar_Dificuldade.y+Alterar_Dificuldade.alt))) && (r->lb==1))
						{
							_Dificuldade(r);
						}
						if(((r->x>=Voltar.x) && (r->x<(Voltar.x+Voltar.comp))) &&
								((r->y>=Voltar.y) && (r->y<(Voltar.y+Voltar.alt))) && (r->lb==1))
						{
							MenuPrincipal(r);
						}


						vt_fill(00, 00);
						desenhaBotao(Alterar_Texto);
						desenhaBotao(Alterar_Linhas);
						desenhaBotao(Alterar_Rato);
						desenhaBotao(Alterar_Dificuldade);
						desenhaBotao(Som);
						desenhaBotao(Voltar);
						desenhar_rato(r, r->x, r->y);
						break;
					}
				}

			}
		}
	}
	mouse_unsubscribe_int();
}

void AltCorLetras(Rato * r)
{
	char *subtitulo;
	botao Cinzento;
	botao Branco;
	botao Amarelo;
	botao Roxo;
	botao Vermelho;
	botao Azul_Claro;
	botao Verde;
	botao Azul;
	botao Voltar;
	short x,y;
	int i=0;
	int ipc_status;
	unsigned long irq_mouse;
	message msg;
	int j, temp;
	int count =0;
	j=0;
	temp=cor_texto;
	subtitulo="Escolha a nova cor das letras:";
	vt_fill(00, 00);

	r->x=0;
	r->y=0;
	r->lb=0;
	r->rb=0;

	/*-----------------------*/
	Cinzento.x = 2;
	Cinzento.y = 6;
	Cinzento.comp = 17;
	Cinzento.alt = 3;
	Cinzento.name = "Cinzento";
	/*-----------------------*/
	Branco.x = 2;
	Branco.y = 9;
	Branco.comp = 17;
	Branco.alt = 3;
	Branco.name = "Branco";
	/*-----------------------*/
	Amarelo.x = 2;
	Amarelo.y = 12;
	Amarelo.comp = 17;
	Amarelo.alt = 3;
	Amarelo.name = "Amarelo";
	/*-----------------------*/
	Roxo.x = 2;
	Roxo.y = 15;
	Roxo.comp = 17;
	Roxo.alt = 3;
	Roxo.name = "Roxo";
	/*-----------------------*/
	Vermelho.x = 20;
	Vermelho.y = 6;
	Vermelho.comp = 17;
	Vermelho.alt = 3;
	Vermelho.name = "Vermelho";
	/*-----------------------*/
	Azul_Claro.x = 20;
	Azul_Claro.y = 9;
	Azul_Claro.comp = 17;
	Azul_Claro.alt = 3;
	Azul_Claro.name = "Azul Claro";
	/*-----------------------*/
	Verde.x = 20;
	Verde.y = 12;
	Verde.comp = 17;
	Verde.alt = 3;
	Verde.name = "Verde";
	/*-----------------------*/
	Azul.x = 20;
	Azul.y = 15;
	Azul.comp = 17;
	Azul.alt = 3;
	Azul.name = "Azul";
	/*-----------------------*/
	Voltar.x = 2;
	Voltar.y = 21;
	Voltar.comp = 17;
	Voltar.alt = 3;
	Voltar.name = "Voltar";

	vt_print_string(subtitulo, 04, 02, cor_texto);
	cor_texto=GREY;
	desenhaBotao(Cinzento);
	cor_texto=WHITE;
	desenhaBotao(Branco);
	cor_texto=YELLOW;
	desenhaBotao(Amarelo);
	cor_texto=PURPLE;
	desenhaBotao(Roxo);
	cor_texto=RED;
	desenhaBotao(Vermelho);
	cor_texto=LIGHT_BLUE;
	desenhaBotao(Azul_Claro);
	cor_texto=GREEN;
	desenhaBotao(Verde);
	cor_texto=BLUE;
	desenhaBotao(Azul);
	cor_texto= temp;
	desenhaBotao(Voltar);
	while(jogo) { 			/*while(de hoje a 8 == pra semana) */
		/* Get a request message. */

		if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			printf("driver_receive failed");
			continue;
		}
		if (is_ipc_notify(ipc_status)) { 		/* received notification */
			if (_ENDPOINT_P(msg.m_source) == HARDWARE) {
				/* hardware interrupt notification */
				if (msg.NOTIFY_ARG & r->irq) { /* subscribed interrupt */
					switch (count) {
					case 3:
						count = 0;
					case 0:
						packet[0] = mouse_read();
						if (packet[0] == (unsigned char)(-1)) {
							break;
						}
						if (packet[0] & BIT(3)) {
							/*	printf(" B1=0x%X", packet[0]);*/
							count++;
						}
						break;
					case 1:
						packet[1] = mouse_read();
						/*printf(" B2=0x%X", packet[1]);*/
						count++;
						break;
					case 2:
						packet[2] = mouse_read();
						x=( (packet[0] & MOUSE_XSIGN ? -((packet[1] ^ 0xFF) + 1) : packet[1]));
						y=((packet[0] & MOUSE_YSIGN ? -((packet[2] ^ 0xFF) + 1) : packet[2]));
						r->lb = (packet[0] & BIT(0));
						r->rb = (packet[0] & BIT(1));

						/*printf("X: %c%d\n", (packet[0] & MOUSE_XSIGN ? '-' : 0x00), (packet[0] & MOUSE_XSIGN ? (packet[1] ^ 0xFF) + 1 : packet[1]));
								printf("Y: %c%d\n", (packet[0] & MOUSE_YSIGN ? '-' : 0x00), (packet[0] & MOUSE_YSIGN ? (packet[2] ^ 0xFF) + 1 : packet[2]));
								printf("x:%d\ny:%d\n\n\n",r->x,r->y);*/
						count++;
						set_pos_rato(r, x, y);
						if(((r->x>=Cinzento.x) && (r->x<(Cinzento.x+Cinzento.comp))) &&
								((r->y>=Cinzento.y) && (r->y<(Cinzento.y+Cinzento.alt))) && (r->lb==1))
						{
							cor_texto=GREY;
							_Opcoes(r);
						}
						if(((r->x>=Branco.x) && (r->x<(Branco.x+Branco.comp))) &&
								((r->y>=Branco.y) && (r->y<(Branco.y+Branco.alt))) && (r->lb==1))
						{
							cor_texto=WHITE;
							_Opcoes(r);
						}
						if(((r->x>=Amarelo.x) && (r->x<(Amarelo.x+Amarelo.comp))) &&
								((r->y>=Amarelo.y) && (r->y<(Amarelo.y+Amarelo.alt))) && (r->lb==1))
						{
							cor_texto=YELLOW;
							_Opcoes(r);
						}
						if(((r->x>=Roxo.x) && (r->x<(Roxo.x+Roxo.comp))) &&
								((r->y>=Roxo.y) && (r->y<(Roxo.y+Roxo.alt))) && (r->lb==1))
						{
							cor_texto=PURPLE;
							_Opcoes(r);
						}
						if(((r->x>=Vermelho.x) && (r->x<(Vermelho.x+Vermelho.comp))) &&
								((r->y>=Vermelho.y) && (r->y<(Vermelho.y+Vermelho.alt))) && (r->lb==1))
						{
							cor_texto=RED;
							_Opcoes(r);
						}
						if(((r->x>=Azul_Claro.x) && (r->x<(Azul_Claro.x+Azul_Claro.comp))) &&
								((r->y>=Azul_Claro.y) && (r->y<(Azul_Claro.y+Azul_Claro.alt))) && (r->lb==1))
						{
							cor_texto=LIGHT_BLUE;
							_Opcoes(r);
						}
						if(((r->x>=Verde.x) && (r->x<=(Verde.x+Verde.comp))) &&
								((r->y>=Verde.y) && (r->y<=(Verde.y+Verde.alt))) && (r->lb==1))
						{
							cor_texto=GREEN;
							_Opcoes(r);
						}
						if(((r->x>=Azul.x) && (r->x<(Azul.x+Azul.comp))) &&
								((r->y>=Azul.y) && (r->y<(Azul.y+Azul.alt))) && (r->lb==1))
						{
							cor_texto=BLUE;
							_Opcoes(r);
						}
						if(((r->x>=Voltar.x) && (r->x<(Voltar.x+Voltar.comp))) &&
								((r->y>=Voltar.y) && (r->y<(Voltar.y+Voltar.alt))) && (r->lb==1))
						{
							cor_texto=temp;
							_Opcoes(r);
						}


						vt_fill(00, 00);
						vt_print_string(subtitulo, 04, 02, cor_texto);
						cor_texto=GREY;
						desenhaBotao(Cinzento);
						cor_texto=WHITE;
						desenhaBotao(Branco);
						cor_texto=YELLOW;
						desenhaBotao(Amarelo);
						cor_texto=PURPLE;
						desenhaBotao(Roxo);
						cor_texto=RED;
						desenhaBotao(Vermelho);
						cor_texto=LIGHT_BLUE;
						desenhaBotao(Azul_Claro);
						cor_texto=GREEN;
						desenhaBotao(Verde);
						cor_texto=BLUE;
						desenhaBotao(Azul);
						cor_texto= temp;
						desenhaBotao(Voltar);
						desenhar_rato(r, r->x, r->y);
						break;
					}
				}

			}
		}
	}
	mouse_unsubscribe_int();
}

void AltCorRato(Rato * r)
{
	char *subtitulo;
	botao Cinzento;
	botao Branco;
	botao Amarelo;
	botao Roxo;
	botao Vermelho;
	botao Azul_Claro;
	botao Verde;
	botao Azul;
	botao Voltar;
	short x,y;
	int i=0;
	int ipc_status;
	unsigned long irq_mouse;
	message msg;
	int j, temp;
	int count =0;
	j=0;
	temp=cor_rato;
	subtitulo="Escolha a nova cor do rato:";
	vt_fill(00, 00);

	r->x=0;
	r->y=0;
	r->lb=0;
	r->rb=0;

	/*-----------------------*/
	Cinzento.x = 2;
	Cinzento.y = 6;
	Cinzento.comp = 17;
	Cinzento.alt = 3;
	Cinzento.name = "Cinzento";
	/*-----------------------*/
	Branco.x = 2;
	Branco.y = 9;
	Branco.comp = 17;
	Branco.alt = 3;
	Branco.name = "Branco";
	/*-----------------------*/
	Amarelo.x = 2;
	Amarelo.y = 12;
	Amarelo.comp = 17;
	Amarelo.alt = 3;
	Amarelo.name = "Amarelo";
	/*-----------------------*/
	Roxo.x = 2;
	Roxo.y = 15;
	Roxo.comp = 17;
	Roxo.alt = 3;
	Roxo.name = "Roxo";
	/*-----------------------*/
	Vermelho.x = 20;
	Vermelho.y = 6;
	Vermelho.comp = 17;
	Vermelho.alt = 3;
	Vermelho.name = "Vermelho";
	/*-----------------------*/
	Azul_Claro.x = 20;
	Azul_Claro.y = 9;
	Azul_Claro.comp = 17;
	Azul_Claro.alt = 3;
	Azul_Claro.name = "Azul Claro";
	/*-----------------------*/
	Verde.x = 20;
	Verde.y = 12;
	Verde.comp = 17;
	Verde.alt = 3;
	Verde.name = "Verde";
	/*-----------------------*/
	Azul.x = 20;
	Azul.y = 15;
	Azul.comp = 17;
	Azul.alt = 3;
	Azul.name = "Azul";
	/*-----------------------*/
	Voltar.x = 2;
	Voltar.y = 21;
	Voltar.comp = 17;
	Voltar.alt = 3;
	Voltar.name = "Voltar";


	vt_print_string(subtitulo, 04, 02, cor_texto);
	desenhaBotao(Cinzento);
	desenhaBotao(Branco);
	desenhaBotao(Amarelo);
	desenhaBotao(Roxo);
	desenhaBotao(Vermelho);
	desenhaBotao(Azul_Claro);
	desenhaBotao(Verde);
	desenhaBotao(Azul);
	desenhaBotao(Voltar);
	while(jogo) { 			/*while(de hoje a 8 == pra semana) */
		/* Get a request message. */

		if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			printf("driver_receive failed");
			continue;
		}
		if (is_ipc_notify(ipc_status)) { 		/* received notification */
			if (_ENDPOINT_P(msg.m_source) == HARDWARE) {
				/* hardware interrupt notification */
				if (msg.NOTIFY_ARG & r->irq) { /* subscribed interrupt */
					switch (count) {
					case 3:
						count = 0;
					case 0:
						packet[0] = mouse_read();
						if (packet[0] == (unsigned char)(-1)) {
							break;
						}
						if (packet[0] & BIT(3)) {
							/*	printf(" B1=0x%X", packet[0]);*/
							count++;
						}
						break;
					case 1:
						packet[1] = mouse_read();
						/*printf(" B2=0x%X", packet[1]);*/
						count++;
						break;
					case 2:
						packet[2] = mouse_read();
						x=( (packet[0] & MOUSE_XSIGN ? -((packet[1] ^ 0xFF) + 1) : packet[1]));
						y=((packet[0] & MOUSE_YSIGN ? -((packet[2] ^ 0xFF) + 1) : packet[2]));
						r->lb = (packet[0] & BIT(0));
						r->rb = (packet[0] & BIT(1));

						/*printf("X: %c%d\n", (packet[0] & MOUSE_XSIGN ? '-' : 0x00), (packet[0] & MOUSE_XSIGN ? (packet[1] ^ 0xFF) + 1 : packet[1]));
									printf("Y: %c%d\n", (packet[0] & MOUSE_YSIGN ? '-' : 0x00), (packet[0] & MOUSE_YSIGN ? (packet[2] ^ 0xFF) + 1 : packet[2]));
									printf("x:%d\ny:%d\n\n\n",r->x,r->y);*/
						count++;
						set_pos_rato(r, x, y);
						if(((r->x>=Cinzento.x) && (r->x<(Cinzento.x+Cinzento.comp))) &&
								((r->y>=Cinzento.y) && (r->y<(Cinzento.y+Cinzento.alt))) && (r->lb==1))
						{
							cor_rato=GREY;
							_Opcoes(r);
						}
						if(((r->x>=Branco.x) && (r->x<(Branco.x+Branco.comp))) &&
								((r->y>=Branco.y) && (r->y<(Branco.y+Branco.alt))) && (r->lb==1))
						{
							cor_rato=WHITE;
							_Opcoes(r);
						}
						if(((r->x>=Amarelo.x) && (r->x<(Amarelo.x+Amarelo.comp))) &&
								((r->y>=Amarelo.y) && (r->y<(Amarelo.y+Amarelo.alt))) && (r->lb==1))
						{
							cor_rato=YELLOW;
							_Opcoes(r);
						}
						if(((r->x>=Roxo.x) && (r->x<(Roxo.x+Roxo.comp))) &&
								((r->y>=Roxo.y) && (r->y<(Roxo.y+Roxo.alt))) && (r->lb==1))
						{
							cor_rato=PURPLE;
							_Opcoes(r);
						}
						if(((r->x>=Vermelho.x) && (r->x<(Vermelho.x+Vermelho.comp))) &&
								((r->y>=Vermelho.y) && (r->y<(Vermelho.y+Vermelho.alt))) && (r->lb==1))
						{
							cor_rato=RED;
							_Opcoes(r);
						}
						if(((r->x>=Azul_Claro.x) && (r->x<(Azul_Claro.x+Azul_Claro.comp))) &&
								((r->y>=Azul_Claro.y) && (r->y<(Azul_Claro.y+Azul_Claro.alt))) && (r->lb==1))
						{
							cor_rato=LIGHT_BLUE;
							_Opcoes(r);
						}
						if(((r->x>=Verde.x) && (r->x<(Verde.x+Verde.comp))) &&
								((r->y>=Verde.y) && (r->y<(Verde.y+Verde.alt))) && (r->lb==1))
						{
							cor_rato=GREEN;
							_Opcoes(r);
						}
						if(((r->x>=Azul.x) && (r->x<(Azul.x+Azul.comp))) &&
								((r->y>=Azul.y) && (r->y<(Azul.y+Azul.alt))) && (r->lb==1))
						{
							cor_rato=BLUE;
							_Opcoes(r);
						}
						if(((r->x>=Voltar.x) && (r->x<(Voltar.x+Voltar.comp))) &&
								((r->y>=Voltar.y) && (r->y<(Voltar.y+Voltar.alt))) && (r->lb==1))
						{
							cor_rato=temp;
							_Opcoes(r);
						}


						vt_fill(00, 00);
						vt_print_string(subtitulo, 04, 02, cor_texto);
						desenhaBotao(Cinzento);
						desenhaBotao(Branco);
						desenhaBotao(Amarelo);
						desenhaBotao(Roxo);
						desenhaBotao(Vermelho);
						desenhaBotao(Azul_Claro);
						desenhaBotao(Verde);
						desenhaBotao(Azul);
						desenhaBotao(Voltar);
						desenhar_rato(r, r->x, r->y);
						break;
					}
				}

			}
		}
	}
	mouse_unsubscribe_int();
}

void AltCorLinhas(Rato * r)
{
	char *subtitulo;
	botao Cinzento;
	botao Branco;
	botao Amarelo;
	botao Roxo;
	botao Vermelho;
	botao Azul_Claro;
	botao Verde;
	botao Azul;
	botao Voltar;
	short x,y;
	int i=0;
	int ipc_status;
	unsigned long irq_mouse;
	message msg;
	int j, temp;
	int count =0;
	j=0;
	temp=cor_linhas;
	subtitulo="Escolha a nova cor das linhas:";
	vt_fill(00, 00);

	r->x=0;
	r->y=0;
	r->lb=0;
	r->rb=0;

	/*-----------------------*/
	Cinzento.x = 2;
	Cinzento.y = 6;
	Cinzento.comp = 17;
	Cinzento.alt = 3;
	Cinzento.name = "Cinzento";
	/*-----------------------*/
	Branco.x = 2;
	Branco.y = 9;
	Branco.comp = 17;
	Branco.alt = 3;
	Branco.name = "Branco";
	/*-----------------------*/
	Amarelo.x = 2;
	Amarelo.y = 12;
	Amarelo.comp = 17;
	Amarelo.alt = 3;
	Amarelo.name = "Amarelo";
	/*-----------------------*/
	Roxo.x = 2;
	Roxo.y = 15;
	Roxo.comp = 17;
	Roxo.alt = 3;
	Roxo.name = "Roxo";
	/*-----------------------*/
	Vermelho.x = 20;
	Vermelho.y = 6;
	Vermelho.comp = 17;
	Vermelho.alt = 3;
	Vermelho.name = "Vermelho";
	/*-----------------------*/
	Azul_Claro.x = 20;
	Azul_Claro.y = 9;
	Azul_Claro.comp = 17;
	Azul_Claro.alt = 3;
	Azul_Claro.name = "Azul Claro";
	/*-----------------------*/
	Verde.x = 20;
	Verde.y = 12;
	Verde.comp = 17;
	Verde.alt = 3;
	Verde.name = "Verde";
	/*-----------------------*/
	Azul.x = 20;
	Azul.y = 15;
	Azul.comp = 17;
	Azul.alt = 3;
	Azul.name = "Azul";
	/*-----------------------*/
	Voltar.x = 2;
	Voltar.y = 21;
	Voltar.comp = 17;
	Voltar.alt = 3;
	Voltar.name = "Voltar";


	vt_print_string(subtitulo, 04, 02, cor_texto);
	cor_linhas=GREY;
	desenhaBotao(Cinzento);
	cor_linhas=WHITE;
	desenhaBotao(Branco);
	cor_linhas=YELLOW;
	desenhaBotao(Amarelo);
	cor_linhas=PURPLE;
	desenhaBotao(Roxo);
	cor_linhas=RED;
	desenhaBotao(Vermelho);
	cor_linhas=LIGHT_BLUE;
	desenhaBotao(Azul_Claro);
	cor_linhas=GREEN;
	desenhaBotao(Verde);
	cor_linhas=BLUE;
	desenhaBotao(Azul);
	cor_linhas= temp;
	desenhaBotao(Voltar);
	while(jogo) { 			/*while(de hoje a 8 == pra semana) */
		/* Get a request message. */

		if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			printf("driver_receive failed");
			continue;
		}
		if (is_ipc_notify(ipc_status)) { 		/* received notification */
			if (_ENDPOINT_P(msg.m_source) == HARDWARE) {
				/* hardware interrupt notification */
				if (msg.NOTIFY_ARG & r->irq) { /* subscribed interrupt */
					switch (count) {
					case 3:
						count = 0;
					case 0:
						packet[0] = mouse_read();
						if (packet[0] == (unsigned char)(-1)) {
							break;
						}
						if (packet[0] & BIT(3)) {
							/*	printf(" B1=0x%X", packet[0]);*/
							count++;
						}
						break;
					case 1:
						packet[1] = mouse_read();
						/*printf(" B2=0x%X", packet[1]);*/
						count++;
						break;
					case 2:
						packet[2] = mouse_read();
						x=( (packet[0] & MOUSE_XSIGN ? -((packet[1] ^ 0xFF) + 1) : packet[1]));
						y=((packet[0] & MOUSE_YSIGN ? -((packet[2] ^ 0xFF) + 1) : packet[2]));
						r->lb = (packet[0] & BIT(0));
						r->rb = (packet[0] & BIT(1));

						/*printf("X: %c%d\n", (packet[0] & MOUSE_XSIGN ? '-' : 0x00), (packet[0] & MOUSE_XSIGN ? (packet[1] ^ 0xFF) + 1 : packet[1]));
										printf("Y: %c%d\n", (packet[0] & MOUSE_YSIGN ? '-' : 0x00), (packet[0] & MOUSE_YSIGN ? (packet[2] ^ 0xFF) + 1 : packet[2]));
										printf("x:%d\ny:%d\n\n\n",r->x,r->y);*/
						count++;
						set_pos_rato(r, x, y);
						if(((r->x>=Cinzento.x) && (r->x<(Cinzento.x+Cinzento.comp))) &&
								((r->y>=Cinzento.y) && (r->y<(Cinzento.y+Cinzento.alt))) && (r->lb==1))
						{
							cor_linhas=GREY;
							_Opcoes(r);
						}
						if(((r->x>=Branco.x) && (r->x<(Branco.x+Branco.comp))) &&
								((r->y>=Branco.y) && (r->y<(Branco.y+Branco.alt))) && (r->lb==1))
						{
							cor_linhas=WHITE;
							_Opcoes(r);
						}
						if(((r->x>=Amarelo.x) && (r->x<(Amarelo.x+Amarelo.comp))) &&
								((r->y>=Amarelo.y) && (r->y<(Amarelo.y+Amarelo.alt))) && (r->lb==1))
						{
							cor_linhas=YELLOW;
							_Opcoes(r);
						}
						if(((r->x>=Roxo.x) && (r->x<(Roxo.x+Roxo.comp))) &&
								((r->y>=Roxo.y) && (r->y<(Roxo.y+Roxo.alt))) && (r->lb==1))
						{
							cor_linhas=PURPLE;
							_Opcoes(r);
						}
						if(((r->x>=Vermelho.x) && (r->x<(Vermelho.x+Vermelho.comp))) &&
								((r->y>=Vermelho.y) && (r->y<(Vermelho.y+Vermelho.alt))) && (r->lb==1))
						{
							cor_linhas=RED;
							_Opcoes(r);
						}
						if(((r->x>=Azul_Claro.x) && (r->x<(Azul_Claro.x+Azul_Claro.comp))) &&
								((r->y>=Azul_Claro.y) && (r->y<(Azul_Claro.y+Azul_Claro.alt))) && (r->lb==1))
						{
							cor_linhas=LIGHT_BLUE;
							_Opcoes(r);
						}
						if(((r->x>=Verde.x) && (r->x<(Verde.x+Verde.comp))) &&
								((r->y>=Verde.y) && (r->y<(Verde.y+Verde.alt))) && (r->lb==1))
						{
							cor_linhas=GREEN;
							_Opcoes(r);
						}
						if(((r->x>=Azul.x) && (r->x<(Azul.x+Azul.comp))) &&
								((r->y>=Azul.y) && (r->y<(Azul.y+Azul.alt))) && (r->lb==1))
						{
							cor_linhas=BLUE;
							_Opcoes(r);
						}
						if(((r->x>=Voltar.x) && (r->x<(Voltar.x+Voltar.comp))) &&
								((r->y>=Voltar.y) && (r->y<(Voltar.y+Voltar.alt))) && (r->lb==1))
						{
							cor_linhas=temp;
							_Opcoes(r);
						}


						vt_fill(00, 00);
						vt_print_string(subtitulo, 04, 02, cor_texto);
						cor_linhas=GREY;
						desenhaBotao(Cinzento);
						cor_linhas=WHITE;
						desenhaBotao(Branco);
						cor_linhas=YELLOW;
						desenhaBotao(Amarelo);
						cor_linhas=PURPLE;
						desenhaBotao(Roxo);
						cor_linhas=RED;
						desenhaBotao(Vermelho);
						cor_linhas=LIGHT_BLUE;
						desenhaBotao(Azul_Claro);
						cor_linhas=GREEN;
						desenhaBotao(Verde);
						cor_linhas=BLUE;
						desenhaBotao(Azul);
						cor_linhas= temp;
						desenhaBotao(Voltar);
						desenhar_rato(r, r->x, r->y);
						break;
					}
				}

			}
		}
	}
	mouse_unsubscribe_int();
}

void _Dificuldade(Rato * r)
{
	char *subtitulo;
	botao Facil;
	botao Normal;
	botao Dificil;
	botao Voltar;
	short x,y;
	int i=0;
	int ipc_status;
	unsigned long irq_mouse;
	message msg;
	int j, temp;
	int count =0;
	j=0;
	subtitulo="Escolha a dificuldade do jogo:";
	vt_fill(00, 00);

	r->x=0;
	r->y=0;
	r->lb=0;
	r->rb=0;

	/*-----------------------*/
	Facil.x = 2;
	Facil.y = 6;
	Facil.comp = 17;
	Facil.alt = 3;
	Facil.name = "Facil";
	/*-----------------------*/
	Normal.x = 2;
	Normal.y = 9;
	Normal.comp = 17;
	Normal.alt = 3;
	Normal.name = "Normal";
	/*-----------------------*/
	Dificil.x = 2;
	Dificil.y = 12;
	Dificil.comp = 17;
	Dificil.alt = 3;
	Dificil.name = "Dificil";
	/*-----------------------*/
	Voltar.x = 2;
	Voltar.y = 21;
	Voltar.comp = 17;
	Voltar.alt = 3;
	Voltar.name = "Voltar";


	vt_print_string(subtitulo, 04, 02, cor_texto);
	desenhaBotao(Facil);
	desenhaBotao(Normal);
	desenhaBotao(Dificil);
	desenhaBotao(Voltar);
	while(jogo) { 			/*while(de hoje a 8 == pra semana) */
		/* Get a request message. */

		if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			printf("driver_receive failed");
			continue;
		}
		if (is_ipc_notify(ipc_status)) { 		/* received notification */
			if (_ENDPOINT_P(msg.m_source) == HARDWARE) {
				/* hardware interrupt notification */
				if (msg.NOTIFY_ARG & r->irq) { /* subscribed interrupt */
					switch (count) {
					case 3:
						count = 0;
					case 0:
						packet[0] = mouse_read();
						if (packet[0] == (unsigned char)(-1)) {
							break;
						}
						if (packet[0] & BIT(3)) {
							/*	printf(" B1=0x%X", packet[0]);*/
							count++;
						}
						break;
					case 1:
						packet[1] = mouse_read();
						/*printf(" B2=0x%X", packet[1]);*/
						count++;
						break;
					case 2:
						packet[2] = mouse_read();
						x=( (packet[0] & MOUSE_XSIGN ? -((packet[1] ^ 0xFF) + 1) : packet[1]));
						y=((packet[0] & MOUSE_YSIGN ? -((packet[2] ^ 0xFF) + 1) : packet[2]));
						r->lb = (packet[0] & BIT(0));
						r->rb = (packet[0] & BIT(1));

						/*printf("X: %c%d\n", (packet[0] & MOUSE_XSIGN ? '-' : 0x00), (packet[0] & MOUSE_XSIGN ? (packet[1] ^ 0xFF) + 1 : packet[1]));
											printf("Y: %c%d\n", (packet[0] & MOUSE_YSIGN ? '-' : 0x00), (packet[0] & MOUSE_YSIGN ? (packet[2] ^ 0xFF) + 1 : packet[2]));
											printf("x:%d\ny:%d\n\n\n",r->x,r->y);*/
						count++;
						set_pos_rato(r, x, y);
						if(((r->x>=Facil.x) && (r->x<(Facil.x+Facil.comp))) &&
								((r->y>=Facil.y) && (r->y<(Facil.y+Facil.alt))) && (r->lb==1))
						{
							AlteraDificuldade(3);
							_Opcoes(r);
						}
						if(((r->x>=Normal.x) && (r->x<(Normal.x+Normal.comp))) &&
								((r->y>=Normal.y) && (r->y<(Normal.y+Normal.alt))) && (r->lb==1))
						{
							AlteraDificuldade(2.5);
							_Opcoes(r);
						}
						if(((r->x>=Dificil.x) && (r->x<(Dificil.x+Dificil.comp))) &&
								((r->y>=Dificil.y) && (r->y<(Dificil.y+Dificil.alt))) && (r->lb==1))
						{
							AlteraDificuldade(2);
							_Opcoes(r);
						}


						if(((r->x>=Voltar.x) && (r->x<(Voltar.x+Voltar.comp))) &&
								((r->y>=Voltar.y) && (r->y<(Voltar.y+Voltar.alt))) && (r->lb==1))
						{
							_Opcoes(r);
						}


						vt_fill(00, 00);
						vt_print_string(subtitulo, 04, 02, cor_texto);
						desenhaBotao(Facil);
						desenhaBotao(Normal);
						desenhaBotao(Dificil);
						desenhaBotao(Voltar);
						desenhar_rato(r, r->x, r->y);
						break;
					}
				}

			}
		}
	}
	mouse_unsubscribe_int();
}

void _Sair()
{
	jogo=0;
}

void EscEquipas(Rato * r)
{
	char *subtitulo1;
	char *subtitulo2;
	botao RealMadrid1;
	botao RealMadrid2;
	botao Barcelona1;
	botao Barcelona2;
	botao Voltar;
	short x,y;
	int i=0;
	int esc=0;
	int ipc_status;
	unsigned long irq_mouse;
	message msg;
	int count =0;
	subtitulo1="Escolha a sua equipa:";
	subtitulo2="Escolha a equipa adversaria:";
	vt_fill(00, 00);
	r->x=0;
	r->y=0;
	r->lb=0;
	r->rb=0;

	/*-----------------------*/
	RealMadrid1.x = 2;
	RealMadrid1.y = 6;
	RealMadrid1.comp = 15;
	RealMadrid1.alt = 3;
	RealMadrid1.name = "Real Madrid";
	/*-----------------------*/
	Barcelona1.x = 2;
	Barcelona1.y = 9;
	Barcelona1.comp = 15;
	Barcelona1.alt = 3;
	Barcelona1.name = "Barcelona";
	/*-----------------------*/
	RealMadrid2.x = 44;
	RealMadrid2.y = 6;
	RealMadrid2.comp = 15;
	RealMadrid2.alt = 3;
	RealMadrid2.name = "Real Madrid";
	/*-----------------------*/
	Barcelona2.x = 44;
	Barcelona2.y = 9;
	Barcelona2.comp = 15;
	Barcelona2.alt = 3;
	Barcelona2.name = "Barcelona";

	/*-----------------------*/
	Voltar.x = 2;
	Voltar.y = 21;
	Voltar.comp = 17;
	Voltar.alt = 3;
	Voltar.name = "Voltar";

	criaEquipasTacticas();
	vt_print_string(subtitulo1, 04, 02, cor_texto);
	vt_print_string(subtitulo2, 04, 44, cor_texto);
	desenhaLinha(40, 04, 10, 0);
	desenhaBotao(RealMadrid1);
	desenhaBotao(Barcelona1);
	desenhaBotao(RealMadrid2);
	desenhaBotao(Barcelona2);
	desenhaBotao(Voltar);

	while(jogo)
	{ 			/*while(de hoje a 8 == pra semana) */
		/* Get a request message. */

		if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			printf("driver_receive failed");
			continue;
		}
		if (is_ipc_notify(ipc_status)) { 		/* received notification */
			if (_ENDPOINT_P(msg.m_source) == HARDWARE) {
				/* hardware interrupt notification */
				if (msg.NOTIFY_ARG & r->irq) { /* subscribed interrupt */
					switch (count) {
					case 3:
						count = 0;
					case 0:
						packet[0] = mouse_read();
						if (packet[0] == (unsigned char)(-1)) {
							break;
						}
						if (packet[0] & BIT(3)) {
							/*	printf(" B1=0x%X", packet[0]);*/
							count++;
						}
						break;
					case 1:
						packet[1] = mouse_read();
						/*printf(" B2=0x%X", packet[1]);*/
						count++;
						break;
					case 2:
						packet[2] = mouse_read();
						x=( (packet[0] & MOUSE_XSIGN ? -((packet[1] ^ 0xFF) + 1) : packet[1]));
						y=((packet[0] & MOUSE_YSIGN ? -((packet[2] ^ 0xFF) + 1) : packet[2]));
						r->lb = (packet[0] & BIT(0));
						r->rb = (packet[0] & BIT(1));

						/*printf("X: %c%d\n", (packet[0] & MOUSE_XSIGN ? '-' : 0x00), (packet[0] & MOUSE_XSIGN ? (packet[1] ^ 0xFF) + 1 : packet[1]));
						printf("Y: %c%d\n", (packet[0] & MOUSE_YSIGN ? '-' : 0x00), (packet[0] & MOUSE_YSIGN ? (packet[2] ^ 0xFF) + 1 : packet[2]));
						printf("x:%d\ny:%d\n\n\n",r->x,r->y);*/
						count++;
						set_pos_rato(r, x, y);
						if(((r->x>=RealMadrid1.x) && (r->x<(RealMadrid1.x+RealMadrid1.comp))) &&
								((r->y>=RealMadrid1.y) && (r->y<(RealMadrid1.y+RealMadrid1.alt))) && (r->lb==1))
						{
							setEquipa(1, 3);
							esc++;
							if(esc==2)
							{
								PreJogo(r);
							}
						}
						if(((r->x>=Barcelona1.x) && (r->x<(Barcelona1.x+Barcelona1.comp))) &&
								((r->y>=Barcelona1.y) && (r->y<(Barcelona1.y+Barcelona1.alt))) && (r->lb==1))
						{
							setEquipa(1, 4);
							esc++;
							if(esc==2)
							{
								PreJogo(r);
							}
						}
						if(((r->x>=RealMadrid2.x) && (r->x<(RealMadrid2.x+RealMadrid2.comp))) &&
								((r->y>=RealMadrid2.y) && (r->y<(RealMadrid2.y+RealMadrid2.alt))) && (r->lb==1))
						{
							setEquipa(2, 3);
							esc++;
							if(esc==2)
							{
								PreJogo(r);
							}
						}
						if(((r->x>=Barcelona2.x) && (r->x<(Barcelona2.x+Barcelona2.comp))) &&
								((r->y>=Barcelona2.y) && (r->y<(Barcelona2.y+Barcelona2.alt))) && (r->lb==1))
						{
							setEquipa(2, 4);
							esc++;
							if(esc==2)
							{
								PreJogo(r);
							}
						}
						if(((r->x>=Voltar.x) && (r->x<(Voltar.x+Voltar.comp))) &&
								((r->y>=Voltar.y) && (r->y<(Voltar.y+Voltar.alt))) && (r->lb==1))
						{
							MenuPrincipal(r);
						}

						vt_fill(00, 00);
						vt_print_string(subtitulo1, 04, 02, cor_texto);
						vt_print_string(subtitulo2, 04, 44, cor_texto);
						desenhaLinha(40, 04, 24, 0);
						if(esc==0)
						{
							desenhaBotao(RealMadrid1);
							desenhaBotao(Barcelona1);
							desenhaBotao(RealMadrid2);
							desenhaBotao(Barcelona2);
						}
						if(esc==1)
						{
							if(getNumEquipa(equipa1)==getNumEquipa(equipa3))
							{
								desenhaBotao(RealMadrid1);
								desenhaBotao(Barcelona2);
							}
							if(getNumEquipa(equipa1)==getNumEquipa(equipa4))
							{
								desenhaBotao(Barcelona1);
								desenhaBotao(RealMadrid2);
							}
							if(getNumEquipa(equipa2)==getNumEquipa(equipa3))
							{
								desenhaBotao(RealMadrid2);
								desenhaBotao(Barcelona1);
							}
							if(getNumEquipa(equipa2)==getNumEquipa(equipa4))
							{
								desenhaBotao(Barcelona2);
								desenhaBotao(RealMadrid1);
							}
						}
						desenhaBotao(Voltar);
						desenhar_rato(r, r->x, r->y);
					}
				}

			}
		}
	}
	mouse_unsubscribe_int();
}

void PreJogo(Rato * r)
{
	botao Jogar;
	botao Formacao;
	botao Plantel;
	botao Voltar;
	/*Rato r;*/
	short x,y;
	int i=0;
	int ipc_status;
	unsigned long irq_mouse;
	message msg;
	int count =0;
	vt_fill(00, 00);
	r->x=0;
	r->y=0;
	r->lb=0;
	r->rb=0;

	/*-----------------------*/
	Jogar.x = 2;
	Jogar.y = 4;
	Jogar.comp = 17;
	Jogar.alt = 3;
	Jogar.name = "JOGAR!!!";

	/*-----------------------*/
	Plantel.x = 2;
	Plantel.y = 7;
	Plantel.comp = 17;
	Plantel.alt = 3;
	Plantel.name = "Editar Plantel";

	/*-----------------------*/
	Formacao.x = 2;
	Formacao.y = 10;
	Formacao.comp = 17;
	Formacao.alt = 3;
	Formacao.name = "Editar Formacao";

	/*-----------------------*/
	Voltar.x = 2;
	Voltar.y = 21;
	Voltar.comp = 17;
	Voltar.alt = 3;
	Voltar.name = "Voltar";

	desenhaBotao(Jogar);
	desenhaBotao(Plantel);
	desenhaBotao(Formacao);
	desenhaBotao(Voltar);

	while(jogo) { 			/*while(de hoje a 8 == pra semana) */
		/* Get a request message. */

		if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			printf("driver_receive failed");
			continue;
		}
		if (is_ipc_notify(ipc_status)) { 		/* received notification */
			if (_ENDPOINT_P(msg.m_source) == HARDWARE) {
				/* hardware interrupt notification */
				if (msg.NOTIFY_ARG & r->irq) { /* subscribed interrupt */
					switch (count) {
					case 3:
						count = 0;
					case 0:
						packet[0] = mouse_read();
						if (packet[0] == (unsigned char)(-1)) {
							break;
						}
						if (packet[0] & BIT(3)) {
							/*	printf(" B1=0x%X", packet[0]);*/
							count++;
						}
						break;
					case 1:
						packet[1] = mouse_read();
						/*printf(" B2=0x%X", packet[1]);*/
						count++;
						break;
					case 2:
						packet[2] = mouse_read();
						x=( (packet[0] & MOUSE_XSIGN ? -((packet[1] ^ 0xFF) + 1) : packet[1]));
						y=((packet[0] & MOUSE_YSIGN ? -((packet[2] ^ 0xFF) + 1) : packet[2]));
						r->lb = (packet[0] & BIT(0));
						r->rb = (packet[0] & BIT(1));

						/*printf("X: %c%d\n", (packet[0] & MOUSE_XSIGN ? '-' : 0x00), (packet[0] & MOUSE_XSIGN ? (packet[1] ^ 0xFF) + 1 : packet[1]));
							printf("Y: %c%d\n", (packet[0] & MOUSE_YSIGN ? '-' : 0x00), (packet[0] & MOUSE_YSIGN ? (packet[2] ^ 0xFF) + 1 : packet[2]));
							printf("x:%d\ny:%d\n\n\n",r->x,r->y);*/
						count++;
						set_pos_rato(r, x, y);
						if(((r->x>=Jogar.x) && (r->x<(Jogar.x+Jogar.comp))) &&
								((r->y>=Jogar.y) && (r->y<(Jogar.y+Jogar.alt))) && (r->lb==1))
						{
							Mostra(r);
						}
						if(((r->x>=Plantel.x) && (r->x<(Plantel.x+Plantel.comp))) &&
								((r->y>=Plantel.y) && (r->y<(Plantel.y+Plantel.alt))) && (r->lb==1))
						{
							EscPlantel(r);
						}
						if(((r->x>=Formacao.x) && (r->x<(Formacao.x+Formacao.comp))) &&
								((r->y>=Formacao.y) && (r->y<(Formacao.y+Formacao.alt))) && (r->lb==1))
						{
							EscFormacao(r);
						}
						if(((r->x>=Voltar.x) && (r->x<(Voltar.x+Voltar.comp))) &&
								((r->y>=Voltar.y) && (r->y<(Voltar.y+Voltar.alt))) && (r->lb==1))
						{
							MenuPrincipal(r);
						}

						vt_fill(00, 00);
						desenhaBotao(Jogar);
						desenhaBotao(Plantel);
						desenhaBotao(Formacao);
						desenhaBotao(Voltar);
						desenhar_rato(r, r->x, r->y);
						break;
					}
				}

			}
		}
	}
	mouse_unsubscribe_int();
}

void CampoFutebol()
{
	int i, j;
	vt_fill(00,00);
	vt_draw_frame(25, 25, 0, 40, WHITE);
	for(i=41; i<64; i++)
	{
		for(j=1;j<24;j++)
		{
			vt_print_char(0xDB, j, i, GREEN);
		}
	}
}

void Mostra(Rato * r)
{
	botao Equipa1;
	botao Equipa2;
	botao Voltar;
	short x,y;
	int i=0;
	int ipc_status;
	unsigned long irq_mouse;
	message msg;
	int j;
	int count =0;
	j=0;
	vt_fill(00, 00);
	r->x=0;
	r->y=0;
	r->lb=0;
	r->rb=0;

	Equipa1.x = 2;
	Equipa1.y = 04;
	Equipa1.comp = 17;
	Equipa1.alt = 3;
	if(getNumEquipa(equipa1)==getNumEquipa(equipa3))
	{
		Equipa1.name = "Real Madrid";
	}
	if(getNumEquipa(equipa1)==getNumEquipa(equipa4))
	{
		Equipa1.name = "Barcelona";
	}
	/*-----------------------*/
	Equipa2.x = 28;
	Equipa2.y = 04;
	Equipa2.comp = 17;
	Equipa2.alt = 3;
	if(getNumEquipa(equipa2)==getNumEquipa(equipa3))
	{
		Equipa2.name = "Real Madrid";
	}
	if(getNumEquipa(equipa2)==getNumEquipa(equipa4))
	{
		Equipa2.name = "Barcelona";
	}
	/*-----------------------*/
	Voltar.x = 2;
	Voltar.y = 21;
	Voltar.comp = 17;
	Voltar.alt = 3;
	Voltar.name = "Voltar";

	setResultado(equipa1, equipa2);
	vt_print_int(G1, 05, 20, 04);
	vt_print_int(G2, 05, 24, 04);
	desenhaBotao(Equipa1);
	desenhaBotao(Equipa2);
	desenhaBotao(Voltar);
	while(jogo) { 			/*while(de hoje a 8 == pra semana) */
		/* Get a request message. */

		if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			printf("driver_receive failed");
			continue;
		}
		if (is_ipc_notify(ipc_status)) { 		/* received notification */
			if (_ENDPOINT_P(msg.m_source) == HARDWARE) {
				/* hardware interrupt notification */
				if (msg.NOTIFY_ARG & r->irq) { /* subscribed interrupt */
					switch (count) {
					case 3:
						count = 0;
					case 0:
						packet[0] = mouse_read();
						if (packet[0] == (unsigned char)(-1)) {
							break;
						}
						if (packet[0] & BIT(3)) {
							/*	printf(" B1=0x%X", packet[0]);*/
							count++;
						}
						break;
					case 1:
						packet[1] = mouse_read();
						/*printf(" B2=0x%X", packet[1]);*/
						count++;
						break;
					case 2:
						packet[2] = mouse_read();
						x=( (packet[0] & MOUSE_XSIGN ? -((packet[1] ^ 0xFF) + 1) : packet[1]));
						y=((packet[0] & MOUSE_YSIGN ? -((packet[2] ^ 0xFF) + 1) : packet[2]));
						r->lb = (packet[0] & BIT(0));
						r->rb = (packet[0] & BIT(1));

						/*printf("X: %c%d\n", (packet[0] & MOUSE_XSIGN ? '-' : 0x00), (packet[0] & MOUSE_XSIGN ? (packet[1] ^ 0xFF) + 1 : packet[1]));
								printf("Y: %c%d\n", (packet[0] & MOUSE_YSIGN ? '-' : 0x00), (packet[0] & MOUSE_YSIGN ? (packet[2] ^ 0xFF) + 1 : packet[2]));
								printf("x:%d\ny:%d\n\n\n",r->x,r->y);*/
						count++;
						set_pos_rato(r, x, y);

						if(((r->x>=Voltar.x) && (r->x<(Voltar.x+Voltar.comp))) &&
								((r->y>=Voltar.y) && (r->y<(Voltar.y+Voltar.alt))) && (r->lb==1))
						{
							MenuPrincipal(r);
						}


						vt_fill(00, 00);
						vt_print_int(G1, 05, 20, 04);
						vt_print_int(G2, 05, 24, 04);
						desenhaBotao(Equipa1);
						desenhaBotao(Equipa2);
						desenhaBotao(Voltar);
						desenhar_rato(r, r->x, r->y);
						break;
					}
				}

			}
		}
	}
	mouse_unsubscribe_int();
}

void desenhaPlantel(Equipa e)
{
	int i, w, j, n, ove;
	char *pos, *name;
	for (i=2; i<25; i++)
	{
		if(i<17)
		{
			desenhaLinha(2, i, 53, 1);
		}
		desenhaLinha(2, i, 27, 1);
		i++;
	}
	desenhaLinha(2, 2, 25, 0);
	desenhaLinha(6, 2, 25, 0);
	desenhaLinha(24, 2, 25, 0);
	desenhaLinha(27, 2, 25, 0);

	desenhaLinha(31, 2, 17, 0);
	desenhaLinha(49, 2, 17, 0);
	desenhaLinha(52, 2, 17, 0);
	/*--------------------*/
	/*n=3;
	for(j=1; j<=11; j++)
	{
		if (getPosJogador(equipa1, j)==1)
		{
			pos="GR";
		}
		if (getPosJogador(equipa1, j)==2)
		{
			pos="DEF";
		}
		if (getPosJogador(equipa1, j)==3)
		{
			pos="MED";
		}
		if (getPosJogador(equipa1, j)==4)
		{
			pos="ATA";
		}
	 *name = getNameJogador(equipa1, j);
		ove = getOvJogador(equipa1, j);
		vt_print_string(pos, n, 3, cor_texto);
		vt_print_string(name, n, 7, cor_texto);
		vt_print_int(ove, n, 25, cor_texto);
		n++;
		n++;
	}*/
	n=3;
	for(j=1; j<12; j++)
	{
		if (getPosJogador(equipa1, j)==1)
		{
			pos = "GR";
		}
		if (getPosJogador(equipa1, j)==2)
		{
			pos = "DEF";
		}
		if (getPosJogador(equipa1, j)==3)
		{
			pos = "MED";
		}
		if (getPosJogador(equipa1, j)==4)
		{
			pos = "AVA";
		}
		vt_print_string(pos, n, 3, cor_texto);
		ove = getOvJogador(equipa1, j);
		vt_print_int(ove, n, 25, cor_texto);
		vt_print_string(pos, n, 3, cor_texto);
		n++;
		n++;
	}
	n=3;
	for(j=12; j<19; j++)
	{
		if (getPosJogador(equipa1, j)==1)
		{
			pos = "GR";
		}
		if (getPosJogador(equipa1, j)==2)
		{
			pos = "DEF";
		}
		if (getPosJogador(equipa1, j)==3)
		{
			pos = "MED";
		}
		if (getPosJogador(equipa1, j)==4)
		{
			pos = "AVA";
		}
		vt_print_string(pos, n, 28, cor_texto);
		ove = getOvJogador(equipa1, j);
		vt_print_int(ove, n, 50, cor_texto);
		n++;
		n++;
	}
}

void EscPlantel(Rato * r)
{
	char *subtitulo1;
	botao Voltar;
	short x,y;
	int i=0;
	int t=0;
	int ipc_status;
	unsigned long irq_mouse;
	message msg;
	int j, temp;
	int count =0;
	j=0;
	temp=cor_texto;
	subtitulo1="Escolha o seu 11 inicial:";
	vt_fill(00, 00);

	r->x=0;
	r->y=0;
	r->lb=0;
	r->rb=0;

	/*-----------------------*/
	Voltar.x = 42;
	Voltar.y = 21;
	Voltar.comp = 17;
	Voltar.alt = 3;
	Voltar.name = "Voltar";

	vt_print_string(subtitulo1, 00, 02, cor_texto);
	desenhaPlantel(equipa1);
	desenhaBotao(Voltar);
	while(jogo) { 			/*while(de hoje a 8 == pra semana) */
		/* Get a request message. */

		if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			printf("driver_receive failed");
			continue;
		}
		if (is_ipc_notify(ipc_status)) { 		/* received notification */
			if (_ENDPOINT_P(msg.m_source) == HARDWARE) {
				/* hardware interrupt notification */
				if (msg.NOTIFY_ARG & r->irq) { /* subscribed interrupt */
					switch (count) {
					case 3:
						count = 0;
					case 0:
						packet[0] = mouse_read();
						if (packet[0] == (unsigned char)(-1)) {
							break;
						}
						if (packet[0] & BIT(3)) {
							/*	printf(" B1=0x%X", packet[0]);*/
							count++;
						}
						break;
					case 1:
						packet[1] = mouse_read();
						/*printf(" B2=0x%X", packet[1]);*/
						count++;
						break;
					case 2:
						packet[2] = mouse_read();
						x=( (packet[0] & MOUSE_XSIGN ? -((packet[1] ^ 0xFF) + 1) : packet[1]));
						y=((packet[0] & MOUSE_YSIGN ? -((packet[2] ^ 0xFF) + 1) : packet[2]));
						r->lb = (packet[0] & BIT(0));
						r->rb = (packet[0] & BIT(1));

						/*printf("X: %c%d\n", (packet[0] & MOUSE_XSIGN ? '-' : 0x00), (packet[0] & MOUSE_XSIGN ? (packet[1] ^ 0xFF) + 1 : packet[1]));
										printf("Y: %c%d\n", (packet[0] & MOUSE_YSIGN ? '-' : 0x00), (packet[0] & MOUSE_YSIGN ? (packet[2] ^ 0xFF) + 1 : packet[2]));
										printf("x:%d\ny:%d\n\n\n",r->x,r->y);*/
						count++;
						set_pos_rato(r, x, y);
						if(((r->x>=2) && (r->x<27)) &&
								((r->y>=2) && (r->y<4)) && (r->lb==1))
						{
							if (t==0)
							{
								t=1;
							}
							else
							{
								Substituicao(equipa1, 1, t);
							}
						}
						if(((r->x>=2) && (r->x<27)) &&
								((r->y>=4) && (r->y<6)) && (r->lb==1))
						{
							if (t==0)
							{
								t=2;
							}
							else
							{
								Substituicao(equipa1, 2, t);
							}
						}
						if(((r->x>=2) && (r->x<27)) &&
								((r->y>=6) && (r->y<8)) && (r->lb==1))
						{
							if (t==0)
							{
								t=3;
							}
							else
							{
								Substituicao(equipa1, 3, t);
							}
						}
						if(((r->x>=2) && (r->x<27)) &&
								((r->y>=8) && (r->y<10)) && (r->lb==1))
						{
							if (t==0)
							{
								t=4;
							}
							else
							{
								Substituicao(equipa1, 4, t);
							}
						}
						if(((r->x>=2) && (r->x<27)) &&
								((r->y>=10) && (r->y<12)) && (r->lb==1))
						{
							if (t==0)
							{
								t=5;
							}
							else
							{
								Substituicao(equipa1, 5, t);
							}
						}
						if(((r->x>=2) && (r->x<27)) &&
								((r->y>=12) && (r->y<14)) && (r->lb==1))
						{
							if (t==0)
							{
								t=6;
							}
							else
							{
								Substituicao(equipa1, 6, t);
							}
						}
						if(((r->x>=2) && (r->x<27)) &&
								((r->y>=14) && (r->y<16)) && (r->lb==1))
						{
							if (t==0)
							{
								t=7;
							}
							else
							{
								Substituicao(equipa1, 7, t);
							}
						}
						if(((r->x>=2) && (r->x<27)) &&
								((r->y>=16) && (r->y<18)) && (r->lb==1))
						{
							if (t==0)
							{
								t=8;
							}
							else
							{
								Substituicao(equipa1, 8, t);
							}
						}
						if(((r->x>=2) && (r->x<27)) &&
								((r->y>=18) && (r->y<20)) && (r->lb==1))
						{
							if (t==0)
							{
								t=9;
							}
							else
							{
								Substituicao(equipa1, 9, t);
							}
						}
						if(((r->x>=2) && (r->x<27)) &&
								((r->y>=20) && (r->y<22)) && (r->lb==1))
						{
							if (t==0)
							{
								t=10;
							}
							else
							{
								Substituicao(equipa1, 10, t);
							}
						}
						if(((r->x>=2) && (r->x<27)) &&
								((r->y>=22) && (r->y<24)) && (r->lb==1))
						{
							if (t==0)
							{
								t=11;
							}
							else
							{
								Substituicao(equipa1, 11, t);
							}
						}
						/*----------------------------------*/
						if(((r->x>=28) && (r->x<52)) &&
								((r->y>=2) && (r->y<4)) && (r->lb==1))
						{
							if (t==0)
							{
								t=12;
							}
							else
							{
								Substituicao(equipa1, t, 12);
							}
						}
						if(((r->x>=28) && (r->x<52)) &&
								((r->y>=4) && (r->y<6)) && (r->lb==1))
						{
							if (t==0)
							{
								t=13;
							}
							else
							{
								Substituicao(equipa1, t, 13);
							}
						}
						if(((r->x>=28) && (r->x<52)) &&
								((r->y>=6) && (r->y<8)) && (r->lb==1))
						{
							if (t==0)
							{
								t=14;
							}
							else
							{
								Substituicao(equipa1, t, 14);
							}
						}
						if(((r->x>=28) && (r->x<52)) &&
								((r->y>=8) && (r->y<10)) && (r->lb==1))
						{
							if (t==0)
							{
								t=15;
							}
							else
							{
								Substituicao(equipa1, t, 15);
							}
						}
						if(((r->x>=28) && (r->x<52)) &&
								((r->y>=10) && (r->y<12)) && (r->lb==1))
						{
							if (t==0)
							{
								t=16;
							}
							else
							{
								Substituicao(equipa1, t, 16);
							}
						}
						if(((r->x>=28) && (r->x<52)) &&
								((r->y>=12) && (r->y<14)) && (r->lb==1))
						{
							if (t==0)
							{
								t=17;
							}
							else
							{
								Substituicao(equipa1, t, 17);
							}
						}
						if(((r->x>=28) && (r->x<52)) &&
								((r->y>=14) && (r->y<16)) && (r->lb==1))
						{
							if (t==0)
							{
								t=18;
							}
							else
							{
								Substituicao(equipa1, t, 18);
							}
						}

						if(((r->x>=Voltar.x) && (r->x<(Voltar.x+Voltar.comp))) &&
								((r->y>=Voltar.y) && (r->y<(Voltar.y+Voltar.alt))) && (r->lb==1))
						{
							PreJogo(r);
						}
						vt_fill(00, 00);
						vt_print_string(subtitulo1, 00, 02, cor_texto);
						desenhaPlantel(equipa1);
						desenhaBotao(Voltar);
						desenhar_rato(r, r->x, r->y);
						break;
					}
				}

			}
		}
	}
	mouse_unsubscribe_int();
}

void EscFormacao(Rato * r)
{
	char *subtitulo1;
	char *subtitulo2;
	botao t433t;
	botao t451t;
	botao t442t;
	botao t541t;
	botao t532t;
	botao t352t;
	botao t343t;
	botao Voltar;
	short x,y;
	int i=0;
	int ipc_status;
	unsigned long irq_mouse;
	message msg;
	int j, temp;
	int count =0;
	j=0;
	temp=cor_texto;
	subtitulo1="Escolha a tactica da sua equipa:";
	subtitulo2="Tactica actual: ";
	vt_fill(00, 00);

	r->x=0;
	r->y=0;
	r->lb=0;
	r->rb=0;

	/*-----------------------*/
	t433t.x = 2;
	t433t.y = 6;
	t433t.comp = 17;
	t433t.alt = 3;
	t433t.name = "4-3-3";
	/*-----------------------*/
	t451t.x = 2;
	t451t.y = 9;
	t451t.comp = 17;
	t451t.alt = 3;
	t451t.name = "4-5-1";
	/*-----------------------*/
	t442t.x = 2;
	t442t.y = 12;
	t442t.comp = 17;
	t442t.alt = 3;
	t442t.name = "4-4-2";
	/*-----------------------*/
	t541t.x = 2;
	t541t.y = 15;
	t541t.comp = 17;
	t541t.alt = 3;
	t541t.name = "5-4-1";
	/*-----------------------*/
	t532t.x = 20;
	t532t.y = 6;
	t532t.comp = 17;
	t532t.alt = 3;
	t532t.name = "5-3-2";
	/*-----------------------*/
	t352t.x = 20;
	t352t.y = 9;
	t352t.comp = 17;
	t352t.alt = 3;
	t352t.name = "3-5-2";
	/*-----------------------*/
	t343t.x = 20;
	t343t.y = 12;
	t343t.comp = 17;
	t343t.alt = 3;
	t343t.name = "3-4-3";
	/*-----------------------*/
	Voltar.x = 2;
	Voltar.y = 21;
	Voltar.comp = 17;
	Voltar.alt = 3;
	Voltar.name = "Voltar";

	vt_print_string(subtitulo1, 04, 02, cor_texto);
	vt_print_string(subtitulo2, 04, 40, cor_texto);
	vt_print_string(equipa1.tac.nome, 04, 57, cor_texto);
	desenhaBotao(t433t);
	desenhaBotao(t451t);
	desenhaBotao(t442t);
	desenhaBotao(t541t);
	desenhaBotao(t532t);
	desenhaBotao(t352t);
	desenhaBotao(t343t);
	desenhaBotao(Voltar);
	while(jogo) { 			/*while(de hoje a 8 == pra semana) */
		/* Get a request message. */

		if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			printf("driver_receive failed");
			continue;
		}
		if (is_ipc_notify(ipc_status)) { 		/* received notification */
			if (_ENDPOINT_P(msg.m_source) == HARDWARE) {
				/* hardware interrupt notification */
				if (msg.NOTIFY_ARG & r->irq) { /* subscribed interrupt */
					switch (count) {
					case 3:
						count = 0;
					case 0:
						packet[0] = mouse_read();
						if (packet[0] == (unsigned char)(-1)) {
							break;
						}
						if (packet[0] & BIT(3)) {
							/*	printf(" B1=0x%X", packet[0]);*/
							count++;
						}
						break;
					case 1:
						packet[1] = mouse_read();
						/*printf(" B2=0x%X", packet[1]);*/
						count++;
						break;
					case 2:
						packet[2] = mouse_read();
						x=( (packet[0] & MOUSE_XSIGN ? -((packet[1] ^ 0xFF) + 1) : packet[1]));
						y=((packet[0] & MOUSE_YSIGN ? -((packet[2] ^ 0xFF) + 1) : packet[2]));
						r->lb = (packet[0] & BIT(0));
						r->rb = (packet[0] & BIT(1));

						/*printf("X: %c%d\n", (packet[0] & MOUSE_XSIGN ? '-' : 0x00), (packet[0] & MOUSE_XSIGN ? (packet[1] ^ 0xFF) + 1 : packet[1]));
									printf("Y: %c%d\n", (packet[0] & MOUSE_YSIGN ? '-' : 0x00), (packet[0] & MOUSE_YSIGN ? (packet[2] ^ 0xFF) + 1 : packet[2]));
									printf("x:%d\ny:%d\n\n\n",r->x,r->y);*/
						count++;
						set_pos_rato(r, x, y);
						if(((r->x>=t433t.x) && (r->x<(t433t.x+t433t.comp))) &&
								((r->y>=t433t.y) && (r->y<(t433t.y+t433t.alt))) && (r->lb==1))
						{
							equipa1.tac=tacticas.tac1;
							PreJogo(r);
						}
						if(((r->x>=t451t.x) && (r->x<(t451t.x+t451t.comp))) &&
								((r->y>=t451t.y) && (r->y<(t451t.y+t451t.alt))) && (r->lb==1))
						{
							equipa1.tac=tacticas.tac2;
							PreJogo(r);
						}
						if(((r->x>=t442t.x) && (r->x<(t442t.x+t442t.comp))) &&
								((r->y>=t442t.y) && (r->y<(t442t.y+t442t.alt))) && (r->lb==1))
						{
							equipa1.tac=tacticas.tac3;
							PreJogo(r);
						}
						if(((r->x>=t541t.x) && (r->x<(t541t.x+t541t.comp))) &&
								((r->y>=t541t.y) && (r->y<(t541t.y+t541t.alt))) && (r->lb==1))
						{
							equipa1.tac=tacticas.tac4;
							PreJogo(r);
						}
						if(((r->x>=t532t.x) && (r->x<(t532t.x+t532t.comp))) &&
								((r->y>=t532t.y) && (r->y<(t532t.y+t532t.alt))) && (r->lb==1))
						{
							equipa1.tac=tacticas.tac5;
							PreJogo(r);
						}
						if(((r->x>=t352t.x) && (r->x<(t352t.x+t352t.comp))) &&
								((r->y>=t352t.y) && (r->y<(t352t.y+t352t.alt))) && (r->lb==1))
						{
							equipa1.tac=tacticas.tac6;
							PreJogo(r);
						}
						if(((r->x>=t343t.x) && (r->x<=(t343t.x+t343t.comp))) &&
								((r->y>=t343t.y) && (r->y<=(t343t.y+t343t.alt))) && (r->lb==1))
						{
							equipa1.tac=tacticas.tac7;
							PreJogo(r);
						}
						if(((r->x>=Voltar.x) && (r->x<(Voltar.x+Voltar.comp))) &&
								((r->y>=Voltar.y) && (r->y<(Voltar.y+Voltar.alt))) && (r->lb==1))
						{
							PreJogo(r);
						}


						vt_fill(00, 00);
						vt_print_string(subtitulo1, 04, 02, cor_texto);
						vt_print_string(subtitulo2, 04, 40, cor_texto);
						vt_print_string(equipa1.tac.nome, 04, 57, cor_texto);
						desenhaBotao(t433t);
						desenhaBotao(t451t);
						desenhaBotao(t442t);
						desenhaBotao(t541t);
						desenhaBotao(t532t);
						desenhaBotao(t352t);
						desenhaBotao(t343t);
						desenhaBotao(Voltar);
						desenhar_rato(r, r->x, r->y);
						break;
					}
				}

			}
		}
	}
	mouse_unsubscribe_int();
}
