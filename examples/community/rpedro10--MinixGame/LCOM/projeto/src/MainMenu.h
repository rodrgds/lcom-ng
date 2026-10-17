#ifndef _MAINMENU_H
#define _MAINMENU_H



typedef struct
{
	int x; /*!< x coordinate of the rectangle */
	int y; /*!< y coordinate of the rectangle */
	int width; /*!< width coordinate of the rectangle */
	int height; /*!< height coordinate of the rectangle */
} Rectangle;

/*! Main menu actions */
typedef enum
{
	PLAY_GAME, EXIT_GAME
} MainMenuAction;

typedef struct
{
} MainMenuState;

Rectangle* newRectangle(int x, int y, int size);



#endif /* _MAINMENU_H */
