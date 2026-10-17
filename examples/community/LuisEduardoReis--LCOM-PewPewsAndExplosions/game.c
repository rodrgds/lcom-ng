#include "game.h"
#include "time.h"
#include "util.h"
#include "stdlib.h"


const unsigned short graficsMODE = 0x115; //10F 320x200   112 640x480    115 800x600    118 1024x768    11B 1280x1024

extern rgbImage ButtonSprite[];
extern rgbImage GameOverSprite[];
extern rgbImage LogoSprite[];
extern rgbImage LevelSignSprite[];


extern listNode* entities;

// Variaveis globais do jogo

	// Altura e Largura do ecra
	int HEIGHT, WIDTH;

	//Pointer para entidade do Jogador
	entity* player;

	//Pointer para entidade da Nave Mae
	entity* MotherShip;

	//Numero de vidas do jogador
	int life = STARTING_LIVES;

	//Pontuacao actual do jogador
	int score = 0;

	//Nivel actual em que o jogador se encontra
	int level = STARTING_LEVEL; //4 levels

	


// Variaveis locais do Jogo

static int playerRespawnTimer = -1; // Temporizador para a criacao do jogador. -1 para desativar
static int playerRespawnDelay = 2*FPS;

static int s_score = 0; // "Slow Score" para ter uma animacao da pontuacao a subir

static int menu = 1; // Se o jogador esta no menu
static int GameOver = -1; // Se o jogador perdeu o jogo (>0). Tambem serve de contador para a animacao do GameOver
static int GameWon = 0; //Se o jogador ganhou o jogo
static Date ScoreTime; // Momento em que o jogador acabou o jogo actual (Para guardar nas pontuacoes)

static int enemyTimer = -1; // Temporizador para a criacao de inimigos. -1 para desativar
static int enemyDelay = FPS; //Tempo para o proximo inimigo

static int* stars; // Array com as estrelas do fundo

static int screenShake = 0; // "Tremer" do ecra

static int LevelChange = 0; //se o jogador passou de nivel

/////////


/*!
 * Inicializa os drivers, os graficos e restantes variaveis do jogo.
 * \return devolve 0 se for bem sucedido
 */
int init() {

	if (timer_set_square(0, FPS) != 0) return 1;
	printf("%d",FPS);
	mouse_enable_packets();

	vg_init(graficsMODE);

	screen_info info = vg_getScreenInfo();
	HEIGHT = info.HEIGHT;
	WIDTH = info.WIDTH;

	timer_subscribe_int();
	kbd_subscribe_int();
	mouse_subscribe_int();

	ser_disable_int(COM2_BASE, IER_RECIEVEDDATA);
	ser_disable_int(COM2_BASE, IER_TRANSMITTEREMTY);
	ser_disable_int(COM2_BASE, IER_RECIEVEDLINESTATUS);

	srand(time(NULL));

	init_Input();

	if (initBackground() != 0) {
		printf("Erro a alocar o background.\n");
		return 1;
	}
	if (init_font(filepath) != 0) {
		printf("Error opening font file.\n");
		return 1;
	}
	if (init_sprites(filepath) != 0) {
		printf("Error opening sprite file.\n");
		return 1;
	}


	return 0;
};

/*!
 * Reinicia o jogo depois do jogador ter acabado um outro jogo.
 */
void gameRestart() {
	playerRespawnTimer = -1;
	if (player != NULL) {
		player->remove = 1;
		player = NULL;
	}

	life = STARTING_LIVES;
	score = 0;
	s_score = 0;
	level = STARTING_LEVEL;

	menu = 1;
	GameOver = -1;
	GameWon = 0;

	enemyTimer = -1;
	enemyDelay = FPS;

	listNode* i;
	for(i = entities; i != NULL; i = i->next)
		if ((i->elem)->type == ENEMY || (i->elem)->type == ENEMYCRUISER || (i->elem)->type == ENEMYMOTHERSHIP)
			(i->elem)->remove = 1;

	MotherShip = NULL;
}

void gameWin() { RTC_read_Date(&ScoreTime); GameOver = 1;	GameWon = 1; }
void gameLose() { RTC_read_Date(&ScoreTime); GameOver = 1;	GameWon = 0; }
int isGameOver() {return (GameOver != -1); }

static rectangle startButton = {-96,-32,192,64};
/*!
 * Evento principal da logica do jogo.
 * Trata de:
 * 	- Receber input no menu
 * 	- Verificar quando o jogador morre e reduzir o numero de vidas
 * 	- Criar um novo jogador caso restem vidas
 * 	- Verificar se o jogo acabou (nao restam vidas)
 * 	- Mudar de nivel
 * 	- Criar inimigos
 * 	\return devolve 1 para sair do jogo
 */
int gameTick() {
	Input in = get_Input();
	//Menu
	if (menu) {
		if (in.mouse_LB_P && pointToRectangleCollision(in.mouse_x, in.mouse_y, startButton, WIDTH/2, HEIGHT/2+192)) {
			LevelChange = 5*FPS;
			playerRespawnTimer = 2*FPS;
			enemyTimer = 3*FPS;
			menu = 0;
		}
	}
	//Verificar se o jogador morreu
	if (player!=NULL && player->health <= 0) {
		life = max(life - 1, 0);

		player->remove = 1;
		player = NULL;

		//Trigger Player Respawn
		//se tiver vidas.
		if (life > 0)
			//respawnTimer (-1 quando desligado)
			playerRespawnTimer = playerRespawnDelay;
		else
			gameLose();

	} else if (player == NULL && playerRespawnTimer != -1 && !isGameOver()) {
		//Contagem para criar um jogador novo
		playerRespawnTimer = max(playerRespawnTimer-1,0);
		if (playerRespawnTimer == 0) {
			player = createEntityAt(PLAYER,WIDTH/2,HEIGHT + 64);
			player->dy = -10 * (60.0/FPS);
			playerRespawnTimer = -1;
		}
	} else {		//Niveis
		if (score <= SCORE0) {
			level = 0;
		} else if (score > SCORE0 && score <= SCORE1) {
			if (level == 0) {
				level = 1;
				LevelChange = 5 * FPS;
			}
		} else if (score > SCORE1 && score <= SCORE2) {
			if (level == 1) {
				level = 2;
				LevelChange = 5 * FPS;
			}
		} else {
			if (level == 2) {
				level = 3;
				LevelChange = 5 * FPS;
			}
		}
	}


	// Enemy Spawning
	if (enemyTimer == 0 && !isGameOver() && LevelChange <= 0) {

		switch(level){
		case 0:
		{
			//a few number of normal enemies
			entity* en1 = createEntity(ENEMY);
			en1->x = rand()%WIDTH;
			en1->y = -64;
			en1->dx = randomRange(-64,128)/128.0 * 1 * (60.0/FPS);
			en1->dy = randomRange(128,255)/255.0 * 2 * (60.0/FPS);
			break;
		}
		case 1:
		{
			//more normal enemies
			enemyDelay =  FPS / 1.5;
			entity* en1 = createEntity(ENEMY);
			en1->x = rand()%WIDTH;
			en1->y = -64;
			en1->dx = randomRange(-64,128)/128.0 * 1 * (60.0/FPS);
			en1->dy = randomRange(128,255)/255.0 * 2 * (60.0/FPS);
			break;
		}
		case 2:
		{
			//normal enemies and cruiser
			enemyDelay =  FPS/2.5;
			entity* en1 = createEntity(ENEMY);
			en1->x = rand()%WIDTH; //(WIDTH - 32) + 32;
			en1->y = -64;
			en1->dx = randomRange(-64,128)/128.0 * 1 * (60.0/FPS);
			en1->dy = randomRange(128,255)/255.0 * 2 * (60.0/FPS);
			if (rand()%32 == 0) createEntityAt(ENEMYCRUISER, rand() % WIDTH, -80);
			break;
		}
		case 3:
		{
			//mother ship
			if(MotherShip == NULL){
				MotherShip = createEntityAt(ENEMYMOTHERSHIP, WIDTH/2, -160);
			}
		break;
		}
		}

		enemyTimer = randomRange(enemyDelay, enemyDelay*2);
	} else if (enemyTimer > 0)
		enemyTimer = max(enemyTimer-1,0);


	return 0;
}

/*!
 * Desenha os graficos do jogo que nao pertencem às entidades
 * Trata de desenhar:
 * 	- A data no canto do ecra
 * 	- O menu incluindo pontuacoes e UI
 * 	- Pontuacao, Vida da nave atual, Vidas restantes, Nivel atual e nivel de calor da arma do jogador
 * 	- Menu de fim de jogo. Recebe tambem o nome do jogador para as pontuacoes e reinicia o jogo.
 * 	- Envia para a placa grafica um offset para um efeito de "abanar" o ecra
 * 	 \return devolve sempre 0
 */
int gameDraw() {
	Input in = get_Input();
	char text[256];

	Date d;
	RTC_read_Date(&d);
	sprintf(text, "Earth Time: %.2x:%.2x:%.2x - %.2x/%.2x/20%.2x", d.hours,d.minutes,d.seconds, d.day,d.month,d.year);
	drawText(text, 10, HEIGHT-18);


	// MENU
	if (menu) {
		int i = 0;

		//Logo
		drawImage(LogoSprite[0],WIDTH/2,200);

		//StartButton
		if (pointToRectangleCollision(in.mouse_x, in.mouse_y, startButton, WIDTH/2, HEIGHT/2+192)) i=1;
		drawImage(ButtonSprite[i], WIDTH/2, HEIGHT/2+192);

		//HighScores
		DrawScores(WIDTH/2, HEIGHT/2);

		//Cursor
		drawImage(ButtonSprite[2],in.mouse_x,in.mouse_y);

	} else {
	
	//Mudanca de Nivel

		if(LevelChange > 0)
		{
			switch(level){
			case 0:
				drawImage(LevelSignSprite[0],WIDTH/2, min((LevelChange*2 - 128)*(60/FPS),HEIGHT/2-128)); break;
			case 1:
				drawImage(LevelSignSprite[1],WIDTH/2, min((LevelChange*2 - 128)*(60/FPS),HEIGHT/2-128)); break;
			case 2:
				drawImage(LevelSignSprite[2],WIDTH/2, min((LevelChange*2 - 128)*(60/FPS),HEIGHT/2-128)); break;
			case 3:
				drawImage(LevelSignSprite[3],WIDTH/2, min((LevelChange*2 - 128)*(60/FPS),HEIGHT/2-128)); break;
			}
			LevelChange = max(LevelChange -1, 0); //diminui o contador
		}
	
	
		s_score = stepTo(s_score, score, 1);
		sprintf(text, "SCORE: %d", s_score);
		drawTextBlend(text, 10, 10, 0xFFFF00);

		sprintf(text, "Level: %d", level+1);
		drawText(text, 10, 25);

		sprintf(text, "Lives: %d", life);
		drawText(text, 10, 40);

		int score_next;

		switch(level){
			case 0:
				score_next = SCORE0 - score; break;
			case 1:
				score_next = SCORE1 - score; break;
			case 2:
				score_next = SCORE2 - score; break;
			default: break;
		}

		if (level != 3){
		sprintf(text, "Next level: %d", score_next);
		drawText(text, 10, 70);}

	}


	// PLAYER
	if (player != NULL) {
		//Health
		sprintf(text, "Health: %d", player->health);
		drawText(text, 10, 55);

		//Heat Bar
		int heat = ((Player*) player)->weaponHeat;
		vg_draw_rectangle(32, HEIGHT-32 - min(abs(heat),100), 32, min(abs(heat),100), 0xFF0000);
	}


	// MOTHER SHIP
	if(MotherShip != NULL){
		//Health
		sprintf(text, "Boss Health: %d", MotherShip->health);
		drawTextBlend(text, 10, 100, 0xFF0000);
	}

	// GAME OVER
	if (GameOver>0) {
		if (GameOver == 1) input_text_clear();
		GameOver++;

		int color;

		if (GameWon) color = hsvTorgb(GameOver*2.5,1,1);
		else color = 0xFFFFFF;

		drawImageBlend(GameOverSprite[GameWon],WIDTH/2, min((GameOver*2 - 128)*(60/FPS),HEIGHT/2-128), color);

		if (GameOver > 3*FPS) {

			int n;  Highscore h = getHighscore();
			color = 0xFFFF00;
			if (score > h.score) {
				color = hsvTorgb(GameOver*5,1,1);
				sprintf(text, "!!HIGHSCORE!!%n",&n);
				drawTextBlend(text, WIDTH/2 - 4*n, HEIGHT/2+16,color);
			}
			sprintf(text, "FINAL SCORE: %d%n", score,&n);
			drawTextBlend(text, WIDTH/2 - 4*n, HEIGHT/2,0xFFFF00);

			sprintf(text, "Enter your name: %s %n",in.text,&n);
			drawText(text, WIDTH/2 - 4*n, HEIGHT/2 + 64);

			if (in.pos_text != 0) {
				sprintf(text, "Press Enter to save Score and Restart%n",&n);
				drawText(text, WIDTH/2 - 4*n, HEIGHT/2 + 96);

				if (in.ENTER_P) {
					h.date = ScoreTime;
					h.score = score;
					memcpy(h.name,in.text,16);
					AddScore(h);

					//Restart Game
					gameRestart();
				}
			}
		}


	}

	//Screen Shake
	if (screenShake == 0) {
		vg_set_buffer_offset(0,0);
	} else {
		vg_set_buffer_offset(
				randomRange(-screenShake,screenShake),
				randomRange(-screenShake,screenShake));
		screenShake = 0;
	}

	return 0;
}


/*!
 * Aumenta o offset do ecra para um efeito de "abanar"
 * \param ss valor a incrementar o offset
 * \return o valor actual do offset
 */
int add_screenShake(int ss) {
	return screenShake+=ss;
}

/*!
 * Termina o jogo e o programa. Liberta a memoria alocada para as entidades e para os graficos.
 * Cancela a subscricao a interrupcoes e retorna ao modo de texto.
 * \return devolve 0 caso seja bem sucedido
 */
int stop() {
	printf("Off\n");

	entitiesFree();
	freeSprites();
	freeFont();
	free(stars);

	timer_unsubscribe_int();
	kbd_unsubscribe_int();
	mouse_unsubscribe_int();

	vg_exit();

	return 0;
};


static int numStars = 128;
static double scroll = 0;
extern rgbImage StarSprite[];

/*!
 * Aloca e inicializa um array de estrelas com valores aleatorios.
 * Cada estrela e composta de 3 ints (x,y,z) sendo que o terceiro dita a velocidade a que esta se move para um efeito de paralax.
 * \return devolve 0 se for bem sucedido
 */
int initBackground() {
	int i = 0;
	stars = (int*) malloc(3*sizeof(int)*numStars);
	if (stars == NULL) {
		perror(NULL);
		return 1;
	}

	for(i = 0; i < 3*numStars; i+=3) {
		stars[i] = rand()%WIDTH;
		stars[i+1] = rand()%HEIGHT;
		stars[i+2] = rand()%64 + 1;
	}
	return 0;
}

/*!
 * Desenha o array de estrelas. O tipo e a velocidade da estrela depende do seu valor de z.
 */
void drawBackground() {
	scroll+=75.0/FPS;
	int i, y, ind;
	for(i = 0; i < 3*numStars; i+=3) {
		y = (stars[i+1]+(((int) scroll)/stars[i+2]))%(HEIGHT+64);

		if (stars[i+2] >= 40) ind = 0;
		else if (stars[i+2] >= 20) ind = 2;
		else if (stars[i+2] == 19) ind = 6;
		else ind = 4;


		if (((int) scroll) & 0x40) ind++;
		drawImage(StarSprite[ind],stars[i],y);
	}
}
