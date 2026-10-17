/*********************************************************
 Definition of a ship that have the type of ship,
 the health and the positions in the matrix of the game.
 **********************************************************/
/* Attributes */
struct ship {
	char type;
	int sizeShip;
	int health;
	int positions[5][3];
};

/* Functions */
int ship_build(struct ship *pShip, char type, int positions[5][3]);
int ship_hit(struct ship *pShip, int x, int y);
int ship_destroyed(struct ship *pShip);
void ship_print(struct ship *pShip);

/*********************************************************
 Definition of a game that has a player, the ships
 **********************************************************/
/* Attributes */
struct game {
	struct ship ships[5];
	char map[10][10];
	char mapOpponent[10][10];
};
/* Functions */
int game_build(struct game *pGame);
int game_attack(int x, int y);
int game_defend();
int game_isOver(struct game *pGame);
void game_printMap(struct game *pGame);
void game_printOpMap(struct game *pGame);
void game_hit(struct game *pGame, int x, int y);
