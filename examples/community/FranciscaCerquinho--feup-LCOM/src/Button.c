#include "Button.h"

Button *newButton(char *bitmap)
{
	Button *auxButton;
	auxButton=(Button *)malloc(sizeof(Button));
	auxButton->image=loadBitmap(bitmap);
	return auxButton;
}


void DestroyButton(Button *auxButton)
{
	free(auxButton);
}


