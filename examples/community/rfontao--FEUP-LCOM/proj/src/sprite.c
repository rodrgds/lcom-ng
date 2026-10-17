#include "sprite.h"
//#include "mouse.h"

extern uint16_t hres;
extern uint16_t vres;

extern void *video_mem;
extern void *video_buffer;
extern int bitsPerPixel;

extern uint32_t mouse_x;
extern uint32_t mouse_y;

extern int ui_x;
extern int ui_y;

extern unsigned int round_time;
extern unsigned int round_number;

extern int score;

int old_car_width = 0;

Sprite * loadSprite(xpm_map_t xpm){

	enum xpm_image_type type = XPM_5_6_5;
  	xpm_image_t img;

	Sprite* sprite = (Sprite*)malloc(sizeof(Sprite));

	if(sprite == NULL){
		vg_exit();
		printf("Couldnt allocate memory for sprite");
	}

  	sprite->map = xpm_load(xpm, type, &img);

	if(sprite->map == NULL){
		vg_exit();
		printf("Failed to load sprite");
	}


	sprite->width = img.width;
	sprite->height = img.height;

	sprite->xspeed = 0;
	sprite->yspeed = 0;

	return sprite;

}


void drawSprite(Sprite * sprite, int x, int y){

	sprite->x = x;
	sprite->y = y;
	uint8_t *aux = sprite->map;

	uint16_t copyX = x;

	for (uint16_t v = 0; v < sprite->height; v++) {
		if(y >= YRES || y < 0){
			y++;
			sprite->map += sprite->width * 2;
			x = copyX;
		} else {
			for (uint16_t h = 0; h < sprite->width; h++) {

				if(x >= XRES || x < 0){
					sprite->map += (bitsPerPixel/8);
					
				} else {

					if(*(uint16_t*)(sprite->map) != xpm_transparency_color(XPM_5_6_5)){
						vg_fillPixel(x, y, sprite->map);
					}
					sprite->map += (bitsPerPixel/8);

					x++;
				}
			}

			y++;
			x = copyX;
		}
	}
	sprite->map = aux;

}

void destroy_sprite(Sprite *sprite) {
	if (sprite == NULL)
		return;
	if (sprite->map)
		free(sprite->map);

	free(sprite);
	sprite = NULL; 
}

void move_Car(Car *car, Sprite *arma){

	int old_x = car->sprite->x;
	//int old_y = car->sprite->y;

	car->sprite->x += car->sprite->xspeed;
	car->sprite->y += car->sprite->yspeed;
	arma->x += arma->xspeed;
	arma->y += arma->yspeed;

	if(car->sprite->y < 0)
		car->sprite->y = 0;
	else if(car->sprite->y + car->sprite->height > YRES)
		car->sprite->y = YRES - car->sprite->height;	

	if(car->sprite->x < 258)
	{
		if(car->sprite->y + car->sprite->height > 828 || car->sprite->y < 682)
		{
			if(old_x >= 258)
			{
				car->sprite->x = 258;
			}
			else
			{
				if(car->sprite->y + car->sprite->height > 828)
				{
					car->sprite->y = 828 - car->sprite->height;
				}
				else if(car->sprite->y < 682)
				{
					car->sprite->y = 682;
				}
			}
		}
	}
	else if(car->sprite->x + car->sprite->width > 1739)
	{
		if(car->sprite->y + car->sprite->height > 811 || car->sprite->y < 659)
		{
			if(old_x + old_car_width <= 1739)
			{
				car->sprite->x = 1739 - car->sprite->width;
			}
			else
			{
				if(car->sprite->y + car->sprite->height > 814)
				{
					car->sprite->y = 814 - car->sprite->height;
				}
				else if(car->sprite->y < 659)
				{
					car->sprite->y = 659;
				}
			}
		}
	}
	
	if(car->sprite->x < 0)
		car->sprite->x = 0;
	else if(car->sprite->x + car->sprite->width > XRES)
		car->sprite->x = XRES - car->sprite->width;
	
}

void move_sprite(Sprite *sprite)
{
	sprite->x += sprite->xspeed;
	sprite->y += sprite->yspeed;
}


void drawBackground(Sprite * background){

	memcpy(video_mem, background->map, XRES*YRES*bitsPerPixel/8);

}

void anim_Car(Car * car, Sprite * carAnim[])
{
	old_car_width = car->sprite->width;

	if(car->sprite->xspeed == 0 && car->sprite->yspeed < 0)
	{
		car->sprite->height = carAnim[0]->height;
		car->sprite->width = carAnim[0]->width;
		car->sprite->map = carAnim[0]->map;
	}
	else if(car->sprite->xspeed > 0 && car->sprite->yspeed<0 && car->sprite->xspeed < -car->sprite->yspeed)
	{
		car->sprite->height = carAnim[1]->height;
		car->sprite->width = carAnim[1]->width;
		car->sprite->map = carAnim[1]->map;
	}
	else if(car->sprite->xspeed > 0 && car->sprite->yspeed<0 && car->sprite->xspeed == -car->sprite->yspeed)
	{
		car->sprite->height = carAnim[2]->height;
		car->sprite->width = carAnim[2]->width;
		car->sprite->map = carAnim[2]->map;
	}
	else if(car->sprite->xspeed > 0 && car->sprite->yspeed<0 && car->sprite->xspeed > -car->sprite->yspeed)
	{
		car->sprite->height = carAnim[3]->height;
		car->sprite->width = carAnim[3]->width;
		car->sprite->map = carAnim[3]->map;
	}
	else if(car->sprite->xspeed > 0 && car->sprite->yspeed == 0)
	{
		car->sprite->height = carAnim[4]->height;
		car->sprite->width = carAnim[4]->width;
		car->sprite->map = carAnim[4]->map;
	}
	else if(car->sprite->xspeed > 0 && car->sprite->yspeed > 0 && car->sprite->xspeed > car->sprite->yspeed)
	{
		car->sprite->height = carAnim[5]->height;
		car->sprite->width = carAnim[5]->width;
		car->sprite->map = carAnim[5]->map;
	}
	else if(car->sprite->xspeed > 0 && car->sprite->yspeed > 0 && car->sprite->xspeed == car->sprite->yspeed)
	{
		car->sprite->height = carAnim[6]->height;
		car->sprite->width = carAnim[6]->width;
		car->sprite->map = carAnim[6]->map;
	}
	else if(car->sprite->xspeed > 0 && car->sprite->yspeed > 0 && car->sprite->xspeed < car->sprite->yspeed)
	{
		car->sprite->height = carAnim[7]->height;
		car->sprite->width = carAnim[7]->width;
		car->sprite->map = carAnim[7]->map;
	}
	else if(car->sprite->xspeed == 0 && car->sprite->yspeed > 0)
	{
		car->sprite->height = carAnim[8]->height;
		car->sprite->width = carAnim[8]->width;
		car->sprite->map = carAnim[8]->map;
	}
	else if(car->sprite->xspeed < 0 && car->sprite->yspeed > 0 && -car->sprite->xspeed < car->sprite->yspeed)
	{
		car->sprite->height = carAnim[9]->height;
		car->sprite->width = carAnim[9]->width;
		car->sprite->map = carAnim[9]->map;
	}
	else if(car->sprite->xspeed < 0 && car->sprite->yspeed > 0 && -car->sprite->xspeed == car->sprite->yspeed)
	{
		car->sprite->height = carAnim[10]->height;
		car->sprite->width = carAnim[10]->width;
		car->sprite->map = carAnim[10]->map;
	}
	else if(car->sprite->xspeed < 0 && car->sprite->yspeed > 0 && -car->sprite->xspeed > car->sprite->yspeed)
	{
		car->sprite->height = carAnim[11]->height;
		car->sprite->width = carAnim[11]->width;
		car->sprite->map = carAnim[11]->map;
	}
	else if(car->sprite->xspeed < 0 && car->sprite->yspeed == 0)
	{
		car->sprite->height = carAnim[12]->height;
		car->sprite->width = carAnim[12]->width;
		car->sprite->map = carAnim[12]->map;
	}
	else if(car->sprite->xspeed < 0 && car->sprite->yspeed < 0 && car->sprite->xspeed < car->sprite->yspeed)
	{
		car->sprite->height = carAnim[13]->height;
		car->sprite->width = carAnim[13]->width;
		car->sprite->map = carAnim[13]->map;
	}
	else if(car->sprite->xspeed < 0 && car->sprite->yspeed < 0 && car->sprite->xspeed == car->sprite->yspeed)
	{
		car->sprite->height = carAnim[14]->height;
		car->sprite->width = carAnim[14]->width;
		car->sprite->map = carAnim[14]->map;
	}
	else if(car->sprite->xspeed < 0 && car->sprite->yspeed < 0 && car->sprite->xspeed > car->sprite->yspeed)
	{
		car->sprite->height = carAnim[15]->height;
		car->sprite->width = carAnim[15]->width;
		car->sprite->map = carAnim[15]->map;
	}
	
}

void anim_Arma(Sprite * arma, Sprite * armaAnim[])
{
	int dx = mouse_x - (arma->x + arma->width/2 - 10);
	int dy = arma->y + arma->height/2 - mouse_y; //y positive variation pointing upwards
	double declive = 0.0;
	if(dx != 0)
		declive = (double)dy/(double)dx; 

	if((dx > 0 && declive > tg_72) || (dx < 0 && declive <= -tg_72) || (dx == 0 && dy > 0))
	{
		arma->map = armaAnim[0]->map;
	}
	else if(dx > 0 && declive <= tg_72 && declive > tg_54)
	{
		arma->map = armaAnim[1]->map;
	}
	else if(dx > 0 && declive <= tg_54 && declive > tg_36)
	{
		arma->map = armaAnim[2]->map;
	}
	else if(dx > 0 && declive <= tg_36 && declive > tg_18)
	{
		arma->map = armaAnim[3]->map;
	}
	else if(dx > 0 && declive <= tg_18 && declive > -tg_18)
	{
		arma->map = armaAnim[4]->map;
	}
	else if(dx > 0 && -declive >= tg_18 && -declive < tg_36)
	{
		arma->map = armaAnim[5]->map;
	}
	else if(dx > 0 && -declive >= tg_36 && -declive < tg_54)
	{
		arma->map = armaAnim[6]->map;
	}
	else if(dx > 0 && -declive >= tg_54 && -declive < tg_72)
	{
		arma->map = armaAnim[7]->map;
	}
	else if((dx > 0 && -declive >= tg_72)||(dx < 0 && declive > tg_72)||(dx == 0 && dy < 0))
	{
		arma->map = armaAnim[8]->map;
	}
	else if(dx < 0 && declive <= tg_72 && declive > tg_54)
	{
		arma->map = armaAnim[9]->map;
	}
	else if(dx < 0 && declive <= tg_54 && declive > tg_36)
	{
		arma->map = armaAnim[10]->map;
	}
	else if(dx < 0 && declive <= tg_36 && declive > tg_18)
	{
		arma->map = armaAnim[11]->map;
	}
	else if(dx < 0 && declive <= tg_18 && declive > -tg_18)
	{
		arma->map = armaAnim[12]->map;
	}
	else if(dx < 0 && -declive >= tg_18 && -declive < tg_36)
	{
		arma->map = armaAnim[13]->map;
	}
	else if(dx < 0 && -declive >= tg_36 && -declive < tg_54)
	{
		arma->map = armaAnim[14]->map;
	}
	else if(dx < 0 && -declive >= tg_54 && -declive < tg_72)
	{
		arma->map = armaAnim[15]->map;
	}
}


void updateCameraCoords(Sprite * sprite,int * x,int * y){

	if(sprite->x < hres/2){
		//mouse_x -= 0;
		*x = 0;
	} else if(sprite->x > XRES - hres/2){
		mouse_x += (XRES-hres)-(*x);
		*x = XRES - hres;
	} else {
		mouse_x += (sprite->x - hres/2)-(*x);
		*x = sprite->x - hres/2;
	}
	
	if(sprite->y < vres/2){
		//mouse_y -= 0;
		*y = 0;
	} else if(sprite->y > YRES - vres/2){
		mouse_y += (YRES-vres)-(*y);
		*y = YRES - vres;
	} else {
		mouse_y += (sprite->y - vres/2)-(*y);
		*y = sprite->y - vres/2;
	}
	ui_x = *x + 50;
	ui_y = *y + 50;

}

void displayTimeAndScore(Sprite* digits[])
{
	unsigned int minutes = round_time / 60;
	unsigned int seconds_dec = (round_time - 60 * minutes) / 10;
	unsigned int seconds_un = (round_time - 60 * minutes - 10 * seconds_dec);

	drawSprite(digits[minutes], ui_x + 481 - 50, ui_y);
	drawSprite(digits[10], ui_x + 502 - 50, ui_y);
	drawSprite(digits[seconds_dec], ui_x - 50 + 523, ui_y);
	drawSprite(digits[seconds_un], ui_x - 50 + 544, ui_y);
	
	unsigned int score_mil = score / 1000;
	unsigned int score_cen = (score - 1000 * score_mil) / 100;
	unsigned int score_dec = (score - 1000 * score_mil - 100 * score_cen) / 10;
	unsigned int score_un = (score - 1000 * score_mil - 100 * score_cen - 10 * score_dec);
	
	drawSprite(digits[score_mil], ui_x + hres - 27*4 - 100, ui_y + 635);
	drawSprite(digits[score_cen], ui_x + hres - 27*3 - 100, ui_y + 635);
	drawSprite(digits[score_dec], ui_x + hres - 27*2 - 100, ui_y + 635);
	drawSprite(digits[score_un], ui_x + hres - 27 - 100, ui_y + 635);

}
