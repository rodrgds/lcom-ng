#ifndef DODGEM_H_
#define DODGEM_H_

#include "RTC.h"
#include "bitmap.h"

/**
 * @brief Saves the coordinates of rectangles and direction (in case of a game object)
 */
typedef struct{
	int xi;
	int xf;
	int yi;
	int yf;
	int direction;
}rectangle;

/**
 * @brief Containes all Bitmaps, irq's, FPS and rectangles objects
 */
typedef struct{
	Bitmap *MenuImage;
	Bitmap* GameField;
	Bitmap* PlaySquare;
	Bitmap* EnemyBL;
	Bitmap* EnemyBR;
	Bitmap* EnemyTL;
	Bitmap* EnemyTR;
	Bitmap* Cursor;
	Bitmap* CursorLeft;
	Bitmap* CursorRight;
	Bitmap* CursorLR;
	Bitmap* CursorLRM;
	Bitmap* CursorMiddle;
	Bitmap* NumbersWhite;
	Bitmap* NumbersBlack;
	Bitmap* ScoreBackground;
	Bitmap* EnergyBar;
	Bitmap* Border;
	Bitmap* PlayInv;
	Bitmap* alphabet;
	Bitmap* submitScreen;
	Bitmap* space;
	Bitmap* MenuHighscore;
	Bitmap* HighscoreList;
	Bitmap* Help;
	Bitmap* Separadores;
	rectangle * PlayOption;
	rectangle * HSOption;
	rectangle * ExitOption;
	rectangle * gameMenuOption;
	rectangle * submitScore;
	rectangle *submitOK;
	rectangle *submitCANCEL;
	rectangle *HighscoreExit;
	rectangle *HighscoreNOBORDER;
	rectangle *HighscoreBORDER;
	rectangle *HighscoreListExit;



	rectangle * TL;
	rectangle * TR;
	rectangle * BL;
	rectangle * BR;

	rectangle * MainSquare;

	int FPS;
	int irq_set_mouse;
	int irq_set_keyboard;
	int irq_set_time;
}DODGEM;

/**
 * @brief Contains the value of powers being used
 *
 * invencibilidade and stopMovement are 0 when inactive and 1 when active
 * vel = 8 when normal and 4 when objects were slowed
 * PowerBeginTime saves the time (counter) of the activation of last power
 * Energy contains the evergy that the player has
 */
typedef struct{
	int invencibilidade;
	int stopMovement;
	int vel;
	long PowerBeginTime; //Quando se ativa um poder guarda aqui o valor do counter de quando foi iniciado
	int Energy;
}POWER;

/**
 * @brief Contains times of scores
 */
typedef struct{
	int best_segundos;
	int best_centesimas;
	int actual_segundos;
	double actual_centesimas;

	int segundosAnteriores;
}SCORE;

/**
 * @brief Has the information about a player (name and score)
 */
typedef struct{
	char nickname[12];
	int segundos;
	int centesimas;
	DATA data;
}PLAYER;

extern DODGEM * game;
extern POWER * Poderes;
extern SCORE *Scores;

/**
 * @brief Update the coordinates of the option rectangles
 */
void StartOptions();

/**
 * @brief load all bitmaps, does all subscribes
 */
void start_DODGEM();

/**
 * @brief alloc memory for game object's
 */
void start_Objects();

/**
 * @brief Delete all bitmaps
 */
void exit_DODGEM();

/**
 * @brief Main menu of DODG'EM
 *
 * @return the option chosen (1- PLAY, 2-HIGHSCORES, 3-EXIT)
 */
int mainMenu(); //return the option chosen

/**
 * @brief see if the player clicked any main menu option
 *
 * @return the option clicked: 0(None), 1(Play), 2(HS), 3(Exit)
 */
int checkMenuOption(); //see if the user clicked any option from menu. 0(None), 1(Play), 2(HS), 3(Exit)

/**
 * @brief Play Menu
 *
 * Contains the menu of the game. If the player click the main square the game starts,
 * if he clicks submit it goes to the submit menu, '?' - HelpMenu and "Menu" goes to main menu
 *
 * @return the option chosen: 1(EXIT), 2(Submit), 3(Play), 4(Help)
 */
int gameMenu(); //menu de jogo

/**
 * @brief HIGHSCORE menu
 *
 * @return the option clicked: 1(Exit), 2(NoBorder HIGHSCORES), 3(Border HIGHSCORES)
 */
int highscoreMenu(); // menu de pontuações

/**
 * @brief Help Menu
 *
 * @return the option clicked
 */
int HelpMenu(); //menu de ajuda

/**
 * @brief sai do jogo
 *
 * @return 3
 */
int exitMenu(); //sair do jogo

/**
 * @brief check the option of the game menu
 *
 * @return the option chosen: 1(EXIT), 2(Submit), 3(Play), 4(Help)
 */
int checkGameOption(); //verificar a opção de jogo escolhida (juntamente com os powers)

/**
 * @brief Start Game (the game itself)
 */
int PlayGame(); //função que permite jogar o jogo

/**
 * @brief Updates the coordinates of an object
 *
 * @param Objeto The object that needs to be updated
 */
void UpdateObjPosition(rectangle * Objeto); //atualiza a posicao dos objetos

/**
 * @brief Calls UpdateObjPosition(rectangle * Objeto) for all objects
 */
void UpdateAllObjects(); // faz o update dos objetos todos

/**
 * @brief Draw all objects
 */
void drawAllObjects(); // desenha todos os objetos

/**
 * @brief Resets the coordinates of all objects
 */
void ResetObjects(); //reset nas coordenadas dos objetos

/**
 * @brief Check if the player colided with an object or the game area
 *
 * @param Inven - 0 or 1 depending if the player has the power immortality activated or not
 *
 * @return 0 if he didn't colide or 1 if he did
 */
int CheckPLayerColision(int Inven); //verifica se o jogador colidiu (0 - Não, 1 - Sim)

/**
 * @brief Check if the player colided with an object
 *
 * @param Objeto - The object it will compare with
 *
 * @return 0 if he didnt and 1 if he did
 */
int CheckColisionObj(rectangle * Objeto); // (0 Não, 1 Sim)

/**
 * @brief check if the player clicked the mouse left button
 *
 * @return 0 if he didn't, 1 if he did
 */
int checkClick(); //verifica se clicou no botao do rato

/**
 * @brief Update all powers states
 */
void UpdatePowers(); //faz update aos poderes

/**
 * @brief Reset the powers value
 */
void StartGamePowers(); //inicia os valores dos poderes (inicio do jogo)

/**
 * @brief Draw the energy bar and updates the energy (regen)
 */
void UpdateEnergy(); // faz update da energia

/**
 * @brief Update the best Score
 */
void updateScores();//esta funcao compara e faz o update do bestscore

/**
 * @brief Read the scores file
 *
 * @return 1 if it does an error 0 otherwise
 */
int loadScores();// faz update para o jogo de todos os scores retorna 1 se der erro retorna 0 se der

/**
 * @brief adds to the Score array the best score
 *
 * @param p - a player information
 */
void addScore(PLAYER p); //esta função adiciona ao array o best score da struct SCORE

/**
 * @brief saves the scores to the scores file
 */
void saveScores(); // esta função guarda no txt todos os recordes do jogo

/**
 * @brief the submit highscore menu
 */
int submitHighscoreMenu(); // faz aparecer o menu para fazer submit do score

/**
 * @brief Checks the option of the submit menu chosen
 *
 * @return Cancel(2) or OK(1)
 */
int checkSubmitOption(); // ve a opcao de submit escolhida cancelas ou aceitar

/**
 * @brief DIsplay the list of highscores
 *
 * @param option - Border or without Border
 */
int MenuHighscoreList(int option); // de acordo com a opção escolhida faz display dos scores

/**
 * @brief Checks wich option the user has chosen (Highscore menu)
 *
 * @return 1 (Exit), 2(NoBorder Highscores), 3(Border Highscores)
 */
int checkHighscoreOption(); // vê a qual a opção de score a mostrar ao utilizador

/**
 * @brief check if the user clicked the exit button on the highscore list menu
 *
 * @return 1(Exit) or 0.
 */
int HighScoreListExit(); // faz exit no menu das listagens dos highscores

#endif
