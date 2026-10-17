#include "fruit.h"

Fruit* initFruit()
{
	srand(time(NULL));
	int x= rand()%25; 
	int y= rand()%25;
	Fruit* fruit = malloc(sizeof(Fruit));
	fruit->fruitPosition = getPoint(x, y);
	fruit->fruitCoords = (Point*)pointToCoord(fruit->fruitPosition);
	fruit->timer = 0; 
	fruit->value = 5; 
	fruit->special=1; 
	return fruit; 
}

int eatFruit(Fruit* fruit)
{
	srand(time(NULL));
	if(fruit->special==0)
	{
		fruit->timer=rand()%10; 
		fruit->duration= 50; 
		updatepositionF(fruit); 
    	return fruit->value;
	}
	else
	{
		updatepositionF(fruit); 
    		return fruit->value;	
   	}
} 

void updatepositionF(Fruit* fruit)
{
	srand(time(NULL)); 
	int x= rand()%25; 
	int y= rand()%25;
	fruit->fruitPosition = getPoint(x, y);
	fruit->fruitCoords = (Point*)pointToCoord(fruit->fruitPosition);
	return; 

}

Fruit* specialFruit()
{
	srand(time(NULL));
	int x= rand()%25; 
	int y= rand()%25;
	Fruit* fruit = malloc(sizeof(Fruit));
	fruit->fruitPosition = getPoint(x, y);
	fruit->fruitCoords = (Point*)pointToCoord(fruit->fruitPosition);
	fruit->timer=rand()%10;
	fruit->duration = 50;  
	fruit->value = 15; 
	fruit->special=0; 
	
	return fruit; 
}
