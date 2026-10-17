#include <minix/drivers.h>
#include "snake.h"

const int FPS = 60; //frames per second

Snake* initSnake()
{
	Snake* snake = malloc(sizeof(Snake));
	
	snake->score = 0; 
	snake->size = INIT_SIZE;
	
	snake->snakePosition = malloc(INIT_SIZE * sizeof(snake->snakePosition));
	snake->snakePosition[0] = getPoint(0,0);
	snake->snakePosition[1] = getPoint(1,0);
	snake->snakePosition[2] = getPoint(2,0);

	snake->direction = RIGHT;

	return snake;
}

static void freeBody(Point** body, unsigned int size)
{
	unsigned int i;
	for (i = 0; i < size; i++) free(body[i]);
	free(body);
}

void die(FILE* fp, Snake* snake, Fruit* fruit)
{
	freeBody(snake->snakePosition, snake->size);
	snake->size = INIT_SIZE;
	writeScore(fp, snake->score); 
	
	snake->snakePosition = malloc(INIT_SIZE * sizeof(snake->snakePosition));
	snake->snakePosition[0] = getPoint(0,0);
	snake->snakePosition[1] = getPoint(1,0);
	snake->snakePosition[2] = getPoint(2,0);

	snake->direction = RIGHT;
	snake->score = 0;
	
	updatepositionF(fruit);
	
	return;
}

void moveSnake(FILE* fp, Snake* snake, Fruit* fruit, Fruit* specialF)
{
	int value=0; 
	
	// Calculate new head position
	Point* headPosition = snake->snakePosition[snake->size - 1];

	// Save old body
	Point** oldBody = malloc(snake->size * sizeof(*oldBody));

	unsigned int i = 0;
	for(i = 0; i < snake->size; i++)
	{
		oldBody[i] = getPoint(snake->snakePosition[i]->x, snake->snakePosition[i]->y);
	}

	// Move head
	switch(snake->direction)
	{
	case LEFT:
		headPosition->x -= 1;
		break;

	case RIGHT:
		headPosition->x += 1;
		break;

	case UP:
		headPosition->y -= 1;
		break;

	case DOWN:
		headPosition->y += 1;
		break;

	default:
		printf("Error\n");
		break;
	}

	// Check if goes against boarders
	if(headPosition->x < 0 || headPosition->x >= 25 || headPosition->y < 0 || headPosition->y >= 25)
	{
		// Dead
		freeBody(oldBody, snake->size);
		die(fp, snake, fruit);
		return;
	}
	// Check if goes against herself
	int b;
	Point* bodyPosition;
	for(b=0; b < snake->size - 1; b++)
	{
		bodyPosition = snake->snakePosition[b];
		
		if (comparePoints(headPosition, bodyPosition))
		{
			 freeBody(oldBody, snake->size);
			 die(fp, snake, fruit);	
			return ; 		
		}
	}	

	// Check if food eaten
	if(comparePoints(headPosition, fruit->fruitPosition))
	{
		// Food eaten
		value = eatFruit(fruit);
		snake->score += value ; 
		
		// Save head
		Point* head = getPoint(headPosition->x, headPosition->y);

		// Size increases
		Point** previousBody = snake->snakePosition;
		snake->size += 1;
		snake->snakePosition = malloc(snake->size * sizeof(snake->snakePosition));

		// Regenerate body
		unsigned int i;
		for(i = 0; i < (snake->size - 1); i++)
		{
			snake->snakePosition[i] = oldBody[i];
		}
		snake->snakePosition[snake->size - 1] = head;
		freeBody(previousBody, snake->size - 1);
	}
	// Check if special food eaten
	else if(comparePoints(headPosition, specialF->fruitPosition))
	{
		// Food eaten
		value = eatFruit(specialF);
		snake->score += value ; 
		
		// Save head
		Point* head = getPoint(headPosition->x, headPosition->y);

		// Size increases
		Point** previousBody = snake->snakePosition;
		snake->size += 1;
		snake->snakePosition = malloc(snake->size * sizeof(snake->snakePosition));

		// Regenerate body
		unsigned int i;
		for(i = 0; i < (snake->size - 1); i++)
		{
			snake->snakePosition[i] = oldBody[i];
		}
		snake->snakePosition[snake->size - 1] = head;
		freeBody(previousBody, snake->size - 1);
	}
	else
	{
		// Regenerate body
		unsigned int i;
		for(i = 1; i < snake->size; i++)
		{
			free(snake->snakePosition[i - 1]);
			snake->snakePosition[i - 1] = oldBody[i];
		}
		free(oldBody[0]);
	}
	free(oldBody);
}

void printSnakePosition(Snake* snake)
{

	unsigned int i = 0;
	for(i = 0; i < snake->size; i++)
	{
		printf("X: %d AND Y: %d\n", snake->snakePosition[i]->x, snake->snakePosition[i]->y);
	}

}

void updateDirection(Snake* snake, unsigned short key)
{
	switch(key)
	{
	case SNAKE_A:

		snake->direction = LEFT;
		break;

	case SNAKE_D:

		snake->direction = RIGHT;
		break;

	case SNAKE_W:

		snake->direction = UP;
		break;

	case SNAKE_S:

		snake->direction = DOWN;
		break;

	default:
		// DO NOTHING
		break;
	}
}
