#include <lcom/lcf.h>
#include <machine/int86.h> // /usr/src/include/arch/i386
#include <stdio.h>
#include "i8042.h"
#include "game.h"
#include "graphics.h"
#include "keyboard.h"
#include "menu.h"
#include "file.h"


int shotX, shotY, alien_shotX, alien_shotY;
extern int h_res, v_res;

game_st game_state = NAME;

extern bool explosion;
extern bool ship_explosion;
extern int counterExplosion;
extern int ship_counterExplosion;
extern bool allowed_to_fire;
extern bool protected;
Asteroid* rock;
Asteroid* rock2;


Jogo* inicio(){
	Jogo* jogo = (Jogo*) malloc(sizeof(Jogo));

	jogo->background_map =  background_xpm;
	jogo->background_pic = xpm_load(jogo->background_map, XPM_5_6_5, &jogo->background_info);

	jogo->ship_map =  rocket2_xpm;
	jogo->ship_pic = xpm_load(jogo->ship_map, XPM_5_6_5, &jogo->ship_info);

	jogo->shield_map =  shield_xpm;
	jogo->shield_pic = xpm_load(jogo->shield_map, XPM_5_6_5, &jogo->shield_info);

	jogo->asteroid_map =  asteroid_xpm;
	jogo->asteroid_pic = xpm_load(jogo->asteroid_map, XPM_5_6_5, &jogo->asteroid_info);

	jogo->SCORE_map =  SCORE_xpm;
	jogo->SCORE_pic = xpm_load(jogo->SCORE_map, XPM_5_6_5, &jogo->SCORE_info);

	jogo->shot_map =  shot_xpm;
	jogo->shot_pic = xpm_load(jogo->shot_map, XPM_5_6_5, &jogo->shot_info);

	jogo->shot2_map =  shot_inverso_xpm;
	jogo->shot2_pic = xpm_load(jogo->shot2_map, XPM_5_6_5, &jogo->shot2_info);

	jogo->alien_map =  alien_xpm;
	jogo->alien_pic = xpm_load(jogo->alien_map, XPM_5_6_5, &jogo->alien_info);

	jogo->bang_map =  explosion_xpm;
	jogo->bang_pic = xpm_load(jogo->bang_map, XPM_5_6_5, &jogo->bang_info);

	jogo->ship_explosion_map =  explosion100_xpm;
	jogo->ship_explosion_pic = xpm_load(jogo->ship_explosion_map, XPM_5_6_5, &jogo->ship_explosion_info);

	jogo->highscores_map =  highscores_xpm;
	jogo->highscores_pic = xpm_load(jogo->highscores_map, XPM_5_6_5, &jogo->highscores_info);

	jogo->name_map =  name_xpm;
	jogo->name_pic = xpm_load(jogo->name_map, XPM_5_6_5, &jogo->name_info);

	jogo->menu_map =  menu_xpm;
	jogo->menu_pic = xpm_load(jogo->menu_map, XPM_5_6_5, &jogo->menu_info);

	jogo->menuS_map =  menustart_xpm;
	jogo->menuS_pic = xpm_load(jogo->menuS_map, XPM_5_6_5, &jogo->menuS_info);

	jogo->menuE_map =  menuexit_xpm;
	jogo->menuE_pic = xpm_load(jogo->menuE_map, XPM_5_6_5, &jogo->menuE_info);

	jogo->menuH_map =  menuscore_xpm;
	jogo->menuH_pic = xpm_load(jogo->menuH_map, XPM_5_6_5, &jogo->menuH_info);

	jogo->pause_map = pause_xpm;
	jogo->pause_pic = xpm_load(jogo->pause_map, XPM_5_6_5, &jogo->pause_info);

	jogo->gameOver_map =  gameover_xpm;
	jogo->gameOver_pic = xpm_load(jogo->gameOver_map, XPM_5_6_5, &jogo->gameOver_info);

	jogo->gameWon_map =  won_xpm;
	jogo->gameWon_pic = xpm_load(jogo->gameWon_map, XPM_5_6_5, &jogo->gameWon_info);

	jogo->instructions_map =  instructions_xpm;
	jogo->instructions_pic = xpm_load(jogo->instructions_map, XPM_5_6_5, &jogo->instructions_info);

	jogo->rato_map =  rato_xpm;
	jogo->rato_pic = xpm_load(jogo->rato_map, XPM_5_6_5, &jogo->rato_info);

	jogo->num0_map = n0_xpm;
	jogo->num0_pic = xpm_load(jogo->num0_map, XPM_5_6_5, &jogo->num0_info);

	jogo->num1_map = n1_xpm;
	jogo->num1_pic = xpm_load(jogo->num1_map, XPM_5_6_5, &jogo->num1_info);

	jogo->num2_map = n2_xpm;
	jogo->num2_pic = xpm_load(jogo->num2_map, XPM_5_6_5, &jogo->num2_info);

	jogo->num3_map = n3_xpm;
	jogo->num3_pic = xpm_load(jogo->num3_map, XPM_5_6_5, &jogo->num3_info);

	jogo->num4_map = n4_xpm;
	jogo->num4_pic = xpm_load(jogo->num4_map, XPM_5_6_5, &jogo->num4_info);

	jogo->num5_map = n5_xpm;
	jogo->num5_pic = xpm_load(jogo->num5_map, XPM_5_6_5, &jogo->num5_info);

	jogo->num6_map = n6_xpm;
	jogo->num6_pic = xpm_load(jogo->num6_map, XPM_5_6_5, &jogo->num6_info);

	jogo->num7_map = n7_xpm;
	jogo->num7_pic = xpm_load(jogo->num7_map, XPM_5_6_5, &jogo->num7_info);

	jogo->num8_map = n8_xpm;
	jogo->num8_pic = xpm_load(jogo->num8_map, XPM_5_6_5, &jogo->num8_info);

	jogo->num9_map = n9_xpm;
	jogo->num9_pic = xpm_load(jogo->num9_map, XPM_5_6_5, &jogo->num9_info);

	jogo->lA_map = A_xpm;
	jogo->lA_pic = xpm_load(jogo->lA_map, XPM_5_6_5, &jogo->lA_info);

	jogo->lB_map = B_xpm;
	jogo->lB_pic = xpm_load(jogo->lB_map, XPM_5_6_5, &jogo->lB_info);

	jogo->lC_map = C_xpm;
	jogo->lC_pic = xpm_load(jogo->lC_map, XPM_5_6_5, &jogo->lC_info);

	jogo->lD_map = D_xpm;
	jogo->lD_pic = xpm_load(jogo->lD_map, XPM_5_6_5, &jogo->lD_info);

	jogo->lE_map = E_xpm;
	jogo->lE_pic = xpm_load(jogo->lE_map, XPM_5_6_5, &jogo->lE_info);

	jogo->lF_map = F_xpm;
	jogo->lF_pic = xpm_load(jogo->lF_map, XPM_5_6_5, &jogo->lF_info);

	jogo->lG_map = G_xpm;
	jogo->lG_pic = xpm_load(jogo->lG_map, XPM_5_6_5, &jogo->lG_info);

	jogo->lH_map = H_xpm;
	jogo->lH_pic = xpm_load(jogo->lH_map, XPM_5_6_5, &jogo->lH_info);

	jogo->lI_map = I_xpm;
	jogo->lI_pic = xpm_load(jogo->lI_map, XPM_5_6_5, &jogo->lI_info);

	jogo->lJ_map = J_xpm;
	jogo->lJ_pic = xpm_load(jogo->lJ_map, XPM_5_6_5, &jogo->lJ_info);

	jogo->lK_map = K_xpm;
	jogo->lK_pic = xpm_load(jogo->lK_map, XPM_5_6_5, &jogo->lK_info);

	jogo->lL_map = L_xpm;
	jogo->lL_pic = xpm_load(jogo->lL_map, XPM_5_6_5, &jogo->lL_info);

	jogo->lM_map = M_xpm;
	jogo->lM_pic = xpm_load(jogo->lM_map, XPM_5_6_5, &jogo->lM_info);

	jogo->lN_map = N_xpm;
	jogo->lN_pic = xpm_load(jogo->lN_map, XPM_5_6_5, &jogo->lN_info);

	jogo->lO_map = O_xpm;
	jogo->lO_pic = xpm_load(jogo->lO_map, XPM_5_6_5, &jogo->lO_info);

	jogo->lP_map = P_xpm;
	jogo->lP_pic = xpm_load(jogo->lP_map, XPM_5_6_5, &jogo->lP_info);

	jogo->lQ_map = Q_xpm;
	jogo->lQ_pic = xpm_load(jogo->lQ_map, XPM_5_6_5, &jogo->lQ_info);

	jogo->lR_map = R_xpm;
	jogo->lR_pic = xpm_load(jogo->lR_map, XPM_5_6_5, &jogo->lR_info);

	jogo->lS_map = S_xpm;
	jogo->lS_pic = xpm_load(jogo->lS_map, XPM_5_6_5, &jogo->lS_info);

	jogo->lT_map = T_xpm;
	jogo->lT_pic = xpm_load(jogo->lT_map, XPM_5_6_5, &jogo->lT_info);

	jogo->lU_map = U_xpm;
	jogo->lU_pic = xpm_load(jogo->lU_map, XPM_5_6_5, &jogo->lU_info);

	jogo->lV_map = V_xpm;
	jogo->lV_pic = xpm_load(jogo->lV_map, XPM_5_6_5, &jogo->lV_info);

	jogo->lW_map = W_xpm;
	jogo->lW_pic = xpm_load(jogo->lW_map, XPM_5_6_5, &jogo->lW_info);

	jogo->lX_map = X_xpm;
	jogo->lX_pic = xpm_load(jogo->lX_map, XPM_5_6_5, &jogo->lX_info);

	jogo->lY_map = Y_xpm;
	jogo->lY_pic = xpm_load(jogo->lY_map, XPM_5_6_5, &jogo->lY_info);

	jogo->lZ_map = Z_xpm;
	jogo->lZ_pic = xpm_load(jogo->lZ_map, XPM_5_6_5, &jogo->lZ_info);

	jogo->barra_map = barra_xpm;
	jogo->barra_pic = xpm_load(jogo->barra_map, XPM_5_6_5, &jogo->barra_info);

	return jogo;
}

void fim(Jogo* jogo){
	free(jogo);
}

Player* playerInit(Jogo* jogo){
	Player* player = (Player*) malloc(sizeof(Player));

	player->x = h_res/2 - (jogo->ship_info.width)/2;
	player->y = v_res-(jogo->ship_info.height)*2;
	player->speed = 20;
	player->shot = 0;
	player->lives = 200;
	player->score = 20;
	player->alive=true;

	rock = (Asteroid*) asteroidInit(player, -1);
	rock2 = (Asteroid*) asteroidInit(player, 900);
	
	return player;
}

void reset_player(Jogo* jogo, Player* player){
	player->x = h_res/2 - (jogo->ship_info.width)/2;
	player->y = v_res-(jogo->ship_info.height)*2;
	player->speed = 20;
	player->shot = 0;
	player->lives = 200;
	player->score = 20;
	player->alive=true;
	ship_explosion = false;
	
	rock = (Asteroid*) asteroidInit(player, -1);
	rock2 = (Asteroid*) asteroidInit(player, 900);
}

void move_ship(Jogo* jogo, Player* player){

if(player->alive == false)
	return;

	if (byte == LEFT_ARROW)
	{
		if (player->x + jogo->ship_info.width > h_res-10)
		{
			player->x = h_res - jogo->ship_info.width;
		}
		else{
			player->x += player->speed;
		}
	}
	else if (byte == RIGHT_ARROW)
	{
		if (player->x - player->speed < 0)
		{
			player->x = 0;
		}
		else{
			player->x -= player->speed;
		}
	}
	else if (byte == KEY_P)
	{
		
		if (game_state == GAME)
		{
			game_state = PAUSE;

		}
		else if (game_state == PAUSE)
		{
			game_state = GAME;
		}
	}
}

void playerDelete(Player* player){
	free(player);
	}

Alien* alienInit(Jogo* jogo){
	Alien* alien = (Alien*) malloc(sizeof(Alien));

	alien->x = h_res/2 - (jogo->alien_info.width)/2;
	alien->y = 60;
	alien->shot = 0;
	alien->speed = 0;
	alien->lives = 6;
	alien->alive = true;
	alien->right = true;
	alien->left = false;

	return alien;
}

void reset_alien(Jogo* jogo, Alien* alien, int level, int lives, int speed){
	alien->x = h_res/2 - (jogo->alien_info.width)/2;
	alien->y = 60;
	alien->shot = 0;
	alien->speed = speed;
	alien->lives = lives;
	alien->alive = true;
	alien->right = true;
	alien->left = false;
	alien->level = level;
	explosion = false;
}

void alienDelete(Alien* alien){
	free(alien);
}


/////////////////////////////////////////////////////////////////////////////////////
void drawJogo(Jogo* jogo, Player* player, Alien* alien){
	
	draw_background(jogo, player);

	move_alien(jogo, alien);											//Altera posiçao do alien;


	if (alien->alive)
		{
			vg_draw_xpm(jogo->alien_pic, &jogo->alien_info, alien->x, alien->y);
		}
		

	if(player->alive)											// Desenha 
	{ 
		if(protected)
		{
			printf("protected\n");
			vg_draw_xpm(jogo->shield_pic, &jogo->shield_info, player->x, player->y);
		}
		else
			vg_draw_xpm(jogo->ship_pic, &jogo->ship_info, player->x, player->y);
	}

	if(!alien->alive)
		{
			level_transition(jogo,alien,player);				//Transicao entre niveis quando necessarrio
		}

	alien_fire(jogo,alien,player);								//Provoca disparo do alien



	player_fire(jogo,alien,player);									//provoca disparo do Jogador





	if(explosion)
	{
		vg_draw_xpm(jogo->bang_pic, &jogo->bang_info, alien->x, alien->y);
	
	}

	if(ship_explosion)
	{
			vg_draw_xpm(jogo->ship_explosion_pic, &jogo->ship_explosion_info, player->x, player->y);
			alien->shot = 0;
			
	}

	
	
	
}



//////////////////////////////////////////////////////////////////////////////////////

void alien_shot_init(Jogo* jogo, Alien* alien){
	if(alien->shot){
		alien_shotX = alien->x;
		alien_shotY = alien->y + (jogo->alien_info.height);

	}
}

void kbd_read(){
  byte = kb_scan_byte(false);

  if (byte == ESC_BREAK)
  {
      is_over = true;
      make = false;      
      return;
  }
 if (byte == TWO_BYTE_SCAN)
 {
    size = 2;
    return;
  }
  else{
  if (byte & 0x80)
  {
      make = false;
      return;
   }
   else{
    make = true;
   }

 }
 
   return;
}



void check_player_fire(Jogo* jogo,Player* player)
{
	if (allowed_to_fire && !(player->shot))
	{
		player->shot = 1;
		shotX = player->x + (jogo->ship_info.width)/2 - (jogo->shot_info.width)/2;
		shotY = player->y;
	}

	allowed_to_fire = false;
}



void move_alien(Jogo* jogo, Alien* alien)
{
	if(alien->alive)
	{
		if(alien->right)
		{
			if (alien->x + jogo->alien_info.width > h_res-10)
			{
				alien->x = h_res - jogo->alien_info.width;
				alien->right = false;
				alien->left = true;
			}
			else{
				alien->x += alien->speed;
			}

		}

		else if(alien->left)
		{
			
			if (alien->x - alien->speed < 0)
			{
				alien->x = 0;
				alien->left = false;
				alien->right = true;
			}
			else{
				alien->x -= alien->speed;
			}


		}
	}

}





void alien_fire(Jogo* jogo, Alien* alien, Player* player)
{
	int impact;
	switch(alien->level)
	{
		case 1: 
		impact = 10;
		break;

		case 2:
		impact = 20;
		break;

		case 3:
		impact = 30;
		break;
	}
	if(alien->shot && alien->alive)
	{		
		if (alien_shotY + 70 <= v_res && !ship_explosion)
		{
			alien_shotY += 20;

			vg_draw_xpm(jogo->shot2_pic, &jogo->shot2_info, alien_shotX, alien_shotY);
			
		}

		if(alien_shotY +70 > v_res)
			{
				alien->shot=0;
				return;
			}


		if (check_colision(jogo->ship_pic,player->x,player->y,jogo->ship_info.width,jogo->ship_info.height))
		{	
				
		
				if(!protected)
				{
					
					ship_explosion = true;
					if(player->score - impact <= 0)
					{
						player-> score = 0;
					}
					else
					{
					player->score -= impact;
					}
				}

				protected = false;
				
		
			if(player->score == 0)
				{
					player->alive = false;
					game_state = GAME_OVER;
				}



			alien->shot =0;
		}
	}


}




void player_fire(Jogo* jogo, Alien* alien, Player* player)
{

	if (player->shot && player->alive)
	{
		
		
		if (shotY - 20 >= 0)
		{
				shotY -= 20;
				vg_draw_xpm(jogo->shot_pic, &jogo->shot_info, shotX, shotY);
				
		}
		else
		{
				player->shot = 0;
				if(player->score -5 <= 0)
				{
					player->score = 0;
				}
				else
				{
					player->score -=5;
				}
		}
		

			if (check_colision(jogo->alien_pic,alien->x,alien->y,jogo->alien_info.width,jogo->alien_info.height))
			{	printf("lives %d\n", alien->lives );
				alien->lives--;
				if(alien->lives == 0)
					{
						alien->alive = false;
						player->score+= 50;
						
						
					}

				player->score += 20;
				player->shot = 0;
				explosion = true;
				
			}
		


		}
		
}

Asteroid* asteroidInit(Player* player, int x){
	Asteroid* rock = (Asteroid*) malloc(sizeof(Asteroid));
	rock->x =x;
	rock->y= 10;
	rock->speedX = (player->x - rock->x)/ 100;
	rock->speedY = (player->y - rock->y)/ 100;
	return rock;

}

void level_transition(Jogo* jogo, Alien* alien, Player* player)
{
	
	if((rock->y + rock->speedY) < (v_res- jogo->bang_info.height)  && (rock->x + rock->speedX) < (h_res  - jogo->bang_info.width))
	{
			rock->x += rock->speedX;
			rock->y += rock->speedY;
			rock2->x += rock2->speedX;
			rock2->y += rock2->speedY;
			vg_draw_xpm(jogo->asteroid_pic, &jogo->asteroid_info, rock->x, rock->y);
			vg_draw_xpm(jogo->asteroid_pic, &jogo->asteroid_info, rock2->x, rock2->y);

			if (check_colision(jogo->ship_pic,player->x,player->y,jogo->ship_info.width,jogo->ship_info.height))
		{
			if(!protected)
				{
					
					ship_explosion = true;
					if(player->score - 10 <= 0)
					{
						player-> score = 0;
					}
					else
					{
					player->score -= 10;
					}
				}

				protected = false;
				
		
			if(player->score == 0)
				{
					player->alive = false;
					game_state = GAME_OVER;
				}
			if(alien->level == 1)
			{
				reset_alien(jogo, alien,2,5, 5);
				rock->x =-1;
				rock->y= 10;
				rock2->x = 900;
				rock2->y = 10;
				rock->speedX = (player->x - rock->x)/ 100;
				rock->speedY = (player->y - rock->y)/ 100;
				rock2->speedX = (player->x - rock2->x)/ 100;
				rock2->speedY = (player->y - rock2->y)/ 100;

			}
			else if(alien->level == 2)
			{
				
				reset_alien(jogo, alien,3, 5, 7);
				rock->x =-1;
				rock->y= 10;
				rock2->x = 900;
				rock2->y = 10;
				rock->speedX = (player->x - rock->x)/ 100;
				rock->speedY = (player->y - rock->y)/ 100;
				rock2->speedX = (player->x - rock2->x)/ 100;
				rock2->speedY = (player->y - rock2->y)/ 100;
			}
			else if(alien->level == 3)
			{
				game_state = WON;
			}
		}
			
	}
	else
	{
		if(alien->level == 1)
		{
			reset_alien(jogo, alien,2,5, 5);
			rock->x =-1;
			rock->y= 10;
			rock2->x = 900;
			rock2->y = 10;
			rock->speedX = (player->x - rock->x)/ 100;
			rock->speedY = (player->y - rock->y)/ 100;

		}
		else if(alien->level == 2)
		{
		
			reset_alien(jogo, alien,3, 5, 7);
			rock->x =-1;
			rock->y= 10;
			rock2->x = 900;
			rock2->y = 10;
			rock->speedX = (player->x - rock->x)/ 100;
			rock->speedY = (player->y - rock->y)/ 100;
		}
		else if(alien->level == 3)
		{
			game_state = WON;
		}
	}
	
	
}






void draw_background(Jogo * jogo,Player* player)
{
vg_draw_xpm(jogo->background_pic, &jogo->background_info, 0, 0); // Desenha o background;
vg_draw_xpm(jogo->SCORE_pic, &jogo->SCORE_info, 10, 10); // Desenha o background;
show_score(jogo,player);
}



void show_score(Jogo* jogo, Player* player)
{
int unidades = (player->score % 10);
display_number(jogo,210, 10, unidades);
int dezenas = (player->score % 100)/10;
display_number(jogo,160, 10, dezenas);
int centenas = (player->score / 100);
display_number(jogo,110, 10, centenas);

}




void display_number(Jogo* jogo,int x, int y, int number)
{
	switch(number)
	{

	case 1:
		vg_draw_xpm(jogo->num1_pic, &jogo->num1_info, x,y);
		break;

	case 2:
		vg_draw_xpm(jogo->num2_pic, &jogo->num2_info, x,y);
		break;

	case 3:
		vg_draw_xpm(jogo->num3_pic, &jogo->num3_info, x,y);
		break;

	case 4:
		vg_draw_xpm(jogo->num4_pic, &jogo->num4_info, x,y);
		break;

	case 5:
		vg_draw_xpm(jogo->num5_pic, &jogo->num5_info, x,y);
		break;

	case 6:
		vg_draw_xpm(jogo->num6_pic, &jogo->num6_info, x,y);
		break;

	case 7:
		vg_draw_xpm(jogo->num7_pic, &jogo->num7_info, x,y);
		break;

	case 8:
		vg_draw_xpm(jogo->num8_pic, &jogo->num8_info, x,y);
		break;

	case 9:
		vg_draw_xpm(jogo->num9_pic, &jogo->num9_info, x,y);
		break;

	case 0:
		vg_draw_xpm(jogo->num0_pic, &jogo->num0_info, x,y);
		break;


	}
}

void display_char_number(Jogo* jogo, char number, int x, int y)
{
	switch(number)
	{

	case '1':
		vg_draw_xpm(jogo->num1_pic, &jogo->num1_info, x,y);
		break;

	case '2':
		vg_draw_xpm(jogo->num2_pic, &jogo->num2_info, x,y);
		break;

	case '3':
		vg_draw_xpm(jogo->num3_pic, &jogo->num3_info, x,y);
		break;

	case '4':
		vg_draw_xpm(jogo->num4_pic, &jogo->num4_info, x,y);
		break;

	case '5':
		vg_draw_xpm(jogo->num5_pic, &jogo->num5_info, x,y);
		break;

	case '6':
		vg_draw_xpm(jogo->num6_pic, &jogo->num6_info, x,y);
		break;

	case '7':
		vg_draw_xpm(jogo->num7_pic, &jogo->num7_info, x,y);
		break;

	case '8':
		vg_draw_xpm(jogo->num8_pic, &jogo->num8_info, x,y);
		break;

	case '9':
		vg_draw_xpm(jogo->num9_pic, &jogo->num9_info, x,y);
		break;

	case '0':
		vg_draw_xpm(jogo->num0_pic, &jogo->num0_info, x,y);
		break;
	case '/':
		vg_draw_xpm(jogo->barra_pic, &jogo->barra_info, x,y);
		break;
	default:
		break;
	}
}

void drawMenu(Jogo* jogo, Mouse* mouse){
	if (mouse->play)
	{
		vg_draw_xpm(jogo->menuS_pic, &jogo->menuS_info, 0, 0);
	}
	else if (mouse->score)
	{
		vg_draw_xpm(jogo->menuH_pic, &jogo->menuH_info, 0, 0);
	}
	else if (mouse->exit)
	{
		vg_draw_xpm(jogo->menuE_pic, &jogo->menuE_info, 0, 0);
	}
	else{
		vg_draw_xpm(jogo->menu_pic, &jogo->menu_info, 0, 0);
	}
	vg_draw_xpm(jogo->rato_pic, &jogo->rato_info, mouse->x, mouse->y);
	
}

void drawPause(Jogo* jogo){
	vg_draw_xpm(jogo->pause_pic, &jogo->pause_info, 0, 0);
}

void drawGameOver(Jogo* jogo){
	vg_draw_xpm(jogo->gameOver_pic, &jogo->gameOver_info, 0, 0);
}

void drawWon(Jogo* jogo){
	vg_draw_xpm(jogo->gameWon_pic, &jogo->gameWon_info, 0, 0);
}

void drawInstructions(Jogo* jogo){
	vg_draw_xpm(jogo->instructions_pic, &jogo->instructions_info, 0, 0);
}

void menu_kb_ih(){
	switch(byte){
		case ESC_BREAK:
			game_state = MAIN_MENU;
			break;
		case KEY_H:
			game_state = INSTRUCTIONS;
			break;
		default:
			break;
	}
}


void show_letter_byte(char* name, int i)
{
	switch(byte)
	{
		case 0x1e:   //A
		name[i] = 'A';
			break;

		case 0x30:  
		name[i] = 'B';
		break;

		case 0x2e:  
		name[i] = 'C';
		break;

		case 0x20:  
		name[i] = 'D';
		break;

		case 0x12:  
		name[i] = 'E';
		break;

		case 0x21:  
		name[i] = 'F';
			break;

		case 0x22:  
		name[i] = 'G';
			break;

		case 0x23:  
		name[i] = 'H';
			break;

		case 0x17:	
		name[i] = 'I';
			break;  

		case 0x24:	
		name[i] = 'J';
			break;

		case 0x25:	
		name[i] = 'K';
			break;

		case 0x26:	
		name[i] = 'L';
		break;

		case 0x32:	
		name[i] = 'M';
		break;

		case 0x31:	
		name[i] = 'N';
		break;	

		case 0x18:	
		name[i] = 'O';
		break;  

		case 0x19:	
		name[i] = 'P';
		break;

		case 0x10:	
		name[i] = 'Q';
		break;

		case 0x13:	
		name[i] = 'R';
		break;

		case 0x1F:	
		name[i] = 'S';
		break;

		case 0x14:	
		name[i] = 'T';
		break;	

		case 0x16:	
		name[i] = 'U';
		break;  

		case 0x11:	
		name[i] = 'W';
		break;

		case 0x2F:	
		name[i] = 'V';
		break;

		case 0x2D:	
		name[i] = 'X';
		break;

		case 0x15:	
		name[i] = 'Y';
		break;

		case 0x2C:	
		name[i] = 'Z';
		break;
		default:
		break;	
	}

}

void show_letter_file(Jogo* jogo, char letter, int x, int y)
{
	switch(letter)
	{
		case 'A':   
		vg_draw_xpm(jogo->lA_pic, &jogo->lA_info, x, y);
		break;

		case 'B':  //B
		vg_draw_xpm(jogo->lB_pic, &jogo->lB_info, x, y);
		break;

		case 'C':  //C
		vg_draw_xpm(jogo->lC_pic, &jogo->lC_info, x, y);
		break;

		case 'D':  //D
		vg_draw_xpm(jogo->lD_pic, &jogo->lD_info, x, y);
		break;

		case 'E':  //E
		vg_draw_xpm(jogo->lE_pic, &jogo->lE_info, x, y);
		break;

		case 'F':  //F
		vg_draw_xpm(jogo->lF_pic, &jogo->lF_info, x, y);
			break;

		case 'G':  //G
		vg_draw_xpm(jogo->lG_pic, &jogo->lG_info, x, y);
			break;

		case 'H':  //H
		vg_draw_xpm(jogo->lH_pic, &jogo->lH_info, x, y);
			break;

		case 'I':	//I
		vg_draw_xpm(jogo->lI_pic, &jogo->lI_info, x, y);
			break;  

		case 'J':	//J	
		vg_draw_xpm(jogo->lJ_pic, &jogo->lJ_info, x, y);
			break;

		case 'K':	//K
		vg_draw_xpm(jogo->lK_pic, &jogo->lK_info, x, y);
			break;

		case 'L':	//L
		vg_draw_xpm(jogo->lL_pic, &jogo->lL_info, x, y);
		break;

		case 'M':	//M
		vg_draw_xpm(jogo->lM_pic, &jogo->lM_info, x, y);
		break;

		case 'N':	//N
		vg_draw_xpm(jogo->lN_pic, &jogo->lN_info, x, y);
		break;	

		case 'O':	//O
		vg_draw_xpm(jogo->lO_pic, &jogo->lO_info, x, y);
		break;  

		case 'P':	//P
		vg_draw_xpm(jogo->lP_pic, &jogo->lP_info, x, y);
		break;

		case 'Q':	//Q
		vg_draw_xpm(jogo->lQ_pic, &jogo->lQ_info, x, y);
		break;

		case 'R':	//R
		vg_draw_xpm(jogo->lR_pic, &jogo->lR_info, x, y);
		break;

		case 'S':	//S
		vg_draw_xpm(jogo->lS_pic, &jogo->lS_info, x, y);
		break;

		case 'T':	//T
		vg_draw_xpm(jogo->lT_pic, &jogo->lT_info, x, y);
		break;	

		case 'U':	//U
		vg_draw_xpm(jogo->lU_pic, &jogo->lU_info, x, y);
		break;  

		case 'W':	//W	
		vg_draw_xpm(jogo->lW_pic, &jogo->lW_info, x, y);
		break;

		case 'V':	//V
		vg_draw_xpm(jogo->lV_pic, &jogo->lV_info, x, y);
		break;

		case 'X':	//X
		vg_draw_xpm(jogo->lX_pic, &jogo->lX_info, x, y);
		break;

		case 'Y':	//Y
		vg_draw_xpm(jogo->lY_pic, &jogo->lY_info, x, y);
		break;

		case 'Z':	//Z
		vg_draw_xpm(jogo->lZ_pic, &jogo->lZ_info, x, y);
		break;	
		default:
			break;
	}

}

void display_score(Jogo* jogo, Users users){
	Users l = users;

	if(l == NULL || l->next == NULL) return;

	int counter = 0;
	int y;
	l = l->next;

	while(l != NULL && counter < 3){
		if (counter == 0)
		{
			y = (v_res/2)-73;
		}
		else if (counter == 1)
		{
			y = (v_res/2)+57;
		}
		else if (counter == 2)
		{
			y = (v_res/2)+200;
		}
		show_letter_file(jogo,l->name[0],50,y);
		show_letter_file(jogo,l->name[1],100,y);
		show_letter_file(jogo,l->name[2],150,y);

		display_char_number(jogo,l->score[0],200,y);
		display_char_number(jogo,l->score[1],250,y);
		display_char_number(jogo,l->score[2],300,y);

		display_char_number(jogo,l->date[0],400,y);
		display_char_number(jogo,l->date[1],450,y);
		display_char_number(jogo,l->date[2],500,y);
		display_char_number(jogo,l->date[3],550,y);
		display_char_number(jogo,l->date[4],600,y);
		display_char_number(jogo,l->date[5],650,y);
		display_char_number(jogo,l->date[6],700,y);
		display_char_number(jogo,l->date[7],750,y);

		counter++;
		l = l->next;
	}
	
}






