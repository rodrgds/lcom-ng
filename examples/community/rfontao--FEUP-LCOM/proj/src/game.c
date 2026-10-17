#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "game.h"

extern uint16_t hres;
extern uint16_t vres;
extern int bitsPerPixel;


int cameraX = 0;
int cameraY = 0;

extern void *video_mem;
extern void *video_buffer;

extern int kbc_hook_id;
extern uint8_t scancode;
extern int ih_error;
extern int mouse_ih_error;

extern int timer0_hook_id;
extern int timer_int_count;

extern uint32_t mouse_x;
extern uint32_t mouse_y;
extern int mouse_hook_id;

extern int rtc_hook_id;

int ui_x = user_interface_x;
int ui_y = user_interface_y;

unsigned int round_time = FIXED_ROUND_TIME;
unsigned int round_number = 1;

extern Bullet* bullets[];
extern CollisionCell collision_matrix[YRES][XRES];

extern Enemy* enemies[];
extern Sprite* enemyBars[];
extern int enemy_hp;
extern unsigned int enemy_count;

bool menu = true;
bool game_over = false;
bool round_passed = false;

int score = 0;
int high_score = 0;

extern Queue* serial_port_send_queue;
extern Queue* serial_port_receive_queue;
extern int serial_port_hook_id;


bool WPressed = false;
bool SPressed = false;
bool APressed = false;
bool DPressed = false;
bool LBPressed = false;
bool RBPressed = false;
bool esc_pressed = false;
extern bool mouseComplete;
extern bool kbdComplete;
extern bool receivingMouse;
int16_t delta_x = 0;
int16_t delta_y = 0;

unsigned int current_player = 0;
bool multi = false;

int game_loop(uint16_t mode){

	multi = false;
    	int ipc_status;
	message msg;
	int r;
	// Set to true when a scancode has two bytes
	bool two_bytes = false;
	// Used for the kbd_print_scancode function
	bool make;
	uint8_t size;
	uint8_t bytes[2];
	uint8_t irq_kbd = BIT(kbc_hook_id);
	uint8_t irq_timer = BIT(timer0_hook_id);

	uint8_t aux_timer0 = (uint8_t) timer0_hook_id;

	int counter = 1;

	bool sign_y;
  	bool sign_x;

  	struct packet pp;

	uint32_t mouse_irq_set = BIT(mouse_hook_id);

	uint32_t rtc_irq_set = BIT(rtc_hook_id);


	int gun_cooldown = CAR_GUN_COOLDOWN;

	int enemy_cooldown = ENEMY_SPAWN_COOLDOWN;

	round_time = FIXED_ROUND_TIME;
	

	if (vg_init(mode) == NULL) {
		vg_exit();
		printf("Init failed");
		return 1;
	}
  
	// Sprite *back = loadSprite(background);
	// Sprite *backM = loadSprite(backMenu);
	// Sprite *backMTest = loadSprite(backMTest_xpm);
	Sprite *backTest = loadSprite(backgroundFinal);

	Sprite *carr = loadSprite(carro1);

	Car * car = (Car*)malloc(sizeof(Car));

        if(!car){
                printf("Couldn't allocate memory for car\n");
                return 1;
        }

	car->sprite = carr;
	car->hp = CAR_HP;
	car->id = 0;

        Sprite *objj = loadSprite(obj);

	Objective *object = (Objective*)malloc(sizeof(Objective));

        if(!object){
                printf("Couldn't allocate memory for objective\n");
                return 1;
        }
        
	object->sprite = objj;
	object->hp = OBJECTIVE_HP; 

	Sprite *car1 = loadSprite(carro1);
	Sprite *car2 = loadSprite(carro2);
	Sprite *car3 = loadSprite(carro3);
	Sprite *car4 = loadSprite(carro4);
	Sprite *car5 = loadSprite(carro5);
	Sprite *car6 = loadSprite(carro6);
	Sprite *car7 = loadSprite(carro7);
	Sprite *car8 = loadSprite(carro8);
	Sprite *car12 = loadSprite(carro12);
	Sprite *car23 = loadSprite(carro23);
	Sprite *car34 = loadSprite(carro34);
	Sprite *car45 = loadSprite(carro45);
	Sprite *car56 = loadSprite(carro56);
	Sprite *car67 = loadSprite(carro67);
	Sprite *car78 = loadSprite(carro78);
	Sprite *car81 = loadSprite(carro81);

	Sprite *arma = loadSprite(arma1);
	Sprite *arm1 = loadSprite(arma1);
	Sprite *arm2 = loadSprite(arma2);
	Sprite *arm3 = loadSprite(arma3);
	Sprite *arm4 = loadSprite(arma4);
	Sprite *arm5 = loadSprite(arma5);
	Sprite *arm6 = loadSprite(arma6);
	Sprite *arm7 = loadSprite(arma7);
	Sprite *arm8 = loadSprite(arma8);
	Sprite *arm12 = loadSprite(arma12);
	Sprite *arm23 = loadSprite(arma23);
	Sprite *arm34 = loadSprite(arma34);
	Sprite *arm45 = loadSprite(arma45);
	Sprite *arm56 = loadSprite(arma56);
	Sprite *arm67 = loadSprite(arma67);
	Sprite *arm78 = loadSprite(arma78);
	Sprite *arm81 = loadSprite(arma81);

	Sprite *enem1 = loadSprite(enemy1);
	Sprite *enem2 = loadSprite(enemy2);
	Sprite *enem3 = loadSprite(enemy3);
	Sprite *enem4 = loadSprite(enemy4);
	Sprite *enem5 = loadSprite(enemy5);
	Sprite *enem6 = loadSprite(enemy6);
	Sprite *enem7 = loadSprite(enemy7);
	Sprite *enem8 = loadSprite(enemy8);
	Sprite *enem12 = loadSprite(enemy12);
	Sprite *enem23 = loadSprite(enemy23);
	Sprite *enem34 = loadSprite(enemy34);
	Sprite *enem45 = loadSprite(enemy45);
	Sprite *enem56 = loadSprite(enemy56);
	Sprite *enem67 = loadSprite(enemy67);
	Sprite *enem78 = loadSprite(enemy78);
	Sprite *enem81 = loadSprite(enemy81);

	Sprite *d0 = loadSprite(digit0);
	Sprite *d1 = loadSprite(digit1);
	Sprite *d2 = loadSprite(digit2);
	Sprite *d3 = loadSprite(digit3);
	Sprite *d4 = loadSprite(digit4);
	Sprite *d5 = loadSprite(digit5);
	Sprite *d6 = loadSprite(digit6);
	Sprite *d7 = loadSprite(digit7);
	Sprite *d8 = loadSprite(digit8);
	Sprite *d9 = loadSprite(digit9);
	Sprite *dt = loadSprite(dots);

	Sprite * digits[] = { d0, d1, d2, d3, d4, d5, d6, d7, d8, d9, dt};


	Sprite *carHpBar = loadSprite(carBar);
	Sprite *carHp = loadSprite(carHpFill);
	Sprite *objHpBar = loadSprite(objectiveBar);
	Sprite *objHp = loadSprite(objectiveHpFill);

	Sprite *startBack = loadSprite(startMenu);
	Sprite *gameOver = loadSprite(gameOverScreen);

	Sprite *round = loadSprite(roundSp);

	Sprite *scoreS = loadSprite(scoreSp);
	Sprite *highS = loadSprite(highSp);

  
	Sprite * carAnim[] = {car1, car12, car2, car23, car3, car34, car4, car45, car5, car56, car6,
	  car67, car7, car78, car8, car81};

	Sprite * armaAnim[] = {arm1, arm12, arm2, arm23, arm3, arm34, arm4, arm45, arm5, arm56, arm6, arm67, arm7, arm78, arm8, arm81};

	Sprite * enemyAnim[] = {enem1, enem12, enem2, enem23, enem3, enem34, enem4, enem45, enem5, enem56, enem6, enem67, enem7, enem78, enem8, enem81};

	Sprite *cursor = loadSprite(crosshair);

	Sprite *startButton = loadSprite(start1);
	Sprite *startButton1 = loadSprite(start1);
	Sprite *startButton2 = loadSprite(start2);
	Sprite *titleSp = loadSprite(title);

	makeBulletsNull(bullets);

	drawBackground(backTest);


	drawSprite(object->sprite, objective_x, objective_y);
	drawCollidible(object->sprite, OBJECTIVEGROUP, 0, car, object);
	drawSprite(car->sprite,hres/2,vres/2);
	drawCollidible(car->sprite,CARGROUP,car->id, car, object);
	drawSprite(arma, car->sprite->x + car->sprite->width/2, car->sprite->y + car->sprite->height/2);
	drawSprite(cursor,mouse_x,mouse_y);




	// Subscribing the interruptions

	if (mouse_send_cmd(MOUSE_ENABLE_CMD)){
		return 1;
        }

	if (mouse_subscribe_int(&mouse_hook_id)){
                printf("Failed to subscribe mouse interrupts\n");
		return 1;
        }
  
	if(timer_subscribe_int(&aux_timer0)){
                printf("Failed to subscribe timer interrupts\n");
		return 1;
        }

	timer0_hook_id = (int)aux_timer0;

	if (kbc_subscribe_int(&kbc_hook_id)){
                printf("Failed to subscribe kbc interrupts");
		return 1;
        }
	

	if(rtc_subscribe_int(&rtc_hook_id)){
                printf("Failed to sunscribe rtc interrupts\n");
                return 1;
        }

	rtc_ih();

		

	while (scancode != ESC_BREAKCODE) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* Received notication */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* Hardware interrupt notification */
				if (msg.m_notify.interrupts & irq_timer) {
		
					
					displayBuffer(video_buffer,video_mem,cameraX,cameraY);
					
					updateCameraCoords(car->sprite,&cameraX,&cameraY);
					cleanCollisionMatrix();
					if(car->hp <= 0 || object->hp <= 0 || round_time == 0)
					{
						game_over = true;
					}
					if(car->hp > 0 && object->hp > 0 && round_time == 0)
					{
						game_over = false;
						round_passed = true;
					}

					if(gun_cooldown > 0)
						gun_cooldown--;
					if(enemy_cooldown > 0)
						enemy_cooldown--;

					if(score < 0)
					{
						score = 0;
					}
					else if(score > 9999)
					{
						score = 9999;
					}

					if(!menu && !game_over && !round_passed){
						
						if (WPressed)
						{
							car->sprite->yspeed -= 1;
							if(car->sprite->yspeed < -CAR_MAX_SPEED)
								car->sprite->yspeed = -CAR_MAX_SPEED;
							
						}
						else
						{
							if(car->sprite->yspeed < 0)
							{
								car->sprite->yspeed += 1;
							}
						}
						
						if(SPressed)
						{
							car->sprite->yspeed += 1;
							if(car->sprite->yspeed > CAR_MAX_SPEED)
								car->sprite->yspeed = CAR_MAX_SPEED;
						}
						else
						{
							if(car->sprite->yspeed > 0)
							{
								car->sprite->yspeed -= 1;
							}
						}
						arma->yspeed = car->sprite->yspeed;
						

						if(APressed)
						{
							car->sprite->xspeed -= 1;
							arma->xspeed -= 1;
							if(car->sprite->xspeed < -CAR_MAX_SPEED)
								car->sprite->xspeed = -CAR_MAX_SPEED;
						}
						else
						{
							if(car->sprite->xspeed < 0)
							{
								car->sprite->xspeed += 1;
							}
						}

						if(DPressed)
						{
							

							car->sprite->xspeed += 1;
							arma->xspeed += 1;
							if(car->sprite->xspeed > CAR_MAX_SPEED)
								car->sprite->xspeed = CAR_MAX_SPEED;
						}
						else
						{
							if(car->sprite->xspeed > 0)
							{
								car->sprite->xspeed -= 1;
							}
						}
						arma->xspeed = car->sprite->xspeed;
					
						if(WPressed || SPressed || APressed || DPressed)
						{
							
							anim_Car(car, carAnim);
						}
						
						if(enemy_cooldown == 0)
						{
							spawnEnemy();
							enemy_cooldown = 120;
						}
						

						move_Car(car, arma);
						anim_Arma(arma, armaAnim);
						
						drawBackground(backTest);
						drawSprite(object->sprite, 900, 80);
						drawCollidible(object->sprite, OBJECTIVEGROUP, 0, car, object);

						drawEnemies(car, object);

						drawSprite(car->sprite, car->sprite->x, car->sprite->y);
						drawCollidible(car->sprite,CARGROUP, car->id, car, object);
						

						carHp->width = car->hp;
						objHp->width = object->hp;
						drawSprite(carHp, ui_x + 35, ui_y + 8);
						drawSprite(carHpBar, ui_x, ui_y);
						drawSprite(objHp, ui_x + 780 + 38, ui_y + 8);
						drawSprite(objHpBar, ui_x + 780, ui_y - 7);
						
						drawSprite(arma, car->sprite->x + car->sprite->width/2 - arma->width/2, car->sprite->y + car->sprite->height/2 - arma->height/2);
						

						displayTimeAndScore(digits);

						drawSprite(digits[round_number], ui_x + 145, ui_y + 635);
						drawSprite(round, ui_x, ui_y + 635);
						drawSprite(scoreS, ui_x + hres - 129 - 100, ui_y + 635 - 50);
						
						drawSprite(cursor, mouse_x, mouse_y);

						if(LBPressed){
							if(gun_cooldown == 0)
							{
								shoot(bullets,car->sprite->x + car->sprite->width/2 - 10,car->sprite->y + car->sprite->height/2 - 10 ,mouse_x,mouse_y, ALLYBULLETGROUP);

								gun_cooldown = CAR_GUN_COOLDOWN;
							}
						}
						drawBullets(bullets, car, object);
						updateEnemies(car, object, enemyAnim);
						updateBullets(bullets);
					}
					else if(game_over)
					{
						if(score > high_score)
						{
							high_score = score;
						}
						drawBackground(backTest);
						drawSprite(gameOver, cameraX + hres/2 - gameOver->width/2, cameraY + 250);
						
						drawSprite(highS, cameraX + hres/2 - (highS->width + scoreS->width + 20)/2, cameraY + 380);
						drawSprite(scoreS, highS->x + highS->width + 20, cameraY + 380);
						drawSprite(digits[10], scoreS->x + scoreS->width + 5, cameraY + 380);

						unsigned int score_mil = high_score / 1000;
						unsigned int score_cen = (high_score - 1000 * score_mil) / 100;
						unsigned int score_dec = (high_score - 1000 * score_mil - 100 * score_cen) / 10;
						unsigned int score_un = (high_score - 1000 * score_mil - 100 * score_cen - 10 * score_dec);
						
						drawSprite(digits[score_mil], cameraX + hres/2 - 27*2, cameraY + 430);
						drawSprite(digits[score_cen], cameraX + hres/2 - 27, cameraY + 430);
						drawSprite(digits[score_dec], cameraX + hres/2, cameraY + 430);
						drawSprite(digits[score_un], cameraX + hres/2 + 27,cameraY + 430);

						drawSprite(startButton, cameraX + 412, cameraY + 500);
						drawSprite(cursor, mouse_x, mouse_y);

						if(mouse_x <= (uint32_t)(startButton->x + startButton->width) && mouse_x >= (uint32_t)startButton->x && mouse_y >= (uint32_t)startButton->y && mouse_y <= (uint32_t)(startButton->y + startButton->height) )
						{
							if(LBPressed) {
								game_over = false;
								ui_x = user_interface_x;
								ui_y = user_interface_y;
								cameraX = 0;
								cameraY = 0;
								round_time = FIXED_ROUND_TIME;
								round_number = 1;
								score = 0;
								car->hp = CAR_HP;
								object->hp = OBJECTIVE_HP;
								for(size_t i = 0; i < enemy_count; i++)
								{
									free(enemies[i]);
									enemies[i] = NULL;
								}
								makeBulletsNull(bullets);

								drawBackground(backTest);
								drawSprite(object->sprite, 900, 80);
								drawCollidible(object->sprite, OBJECTIVEGROUP, 0, car, object);
								drawSprite(car->sprite,hres/2,vres/2);
								drawCollidible(car->sprite,CARGROUP,car->id, car, object);
								drawSprite(arma, car->sprite->x + car->sprite->width/2, car->sprite->y + car->sprite->height/2);
								drawSprite(cursor,mouse_x,mouse_y);
							}
							else
							{
								startButton->map = startButton2->map;
							}
							
						}
						else
						{
							startButton->map = startButton1->map;
						}

					}
					else if(round_passed)
					{
						
						
						score += 500;
						round_number++;
						round_time = FIXED_ROUND_TIME;
						car->hp = 100;
						object->hp = 100;
						for(size_t i = 0; i < enemy_count; i++)
						{
							free(enemies[i]);
							enemies[i] = NULL;
						}
						makeBulletsNull(bullets);

						drawBackground(backTest);

						ui_x = user_interface_x;
						ui_y = user_interface_y;
						cameraX = 0;
						cameraY = 0;
						enemy_count += 2;
						enemy_hp += 20;

						drawSprite(object->sprite, 900, 80);
						drawCollidible(object->sprite, OBJECTIVEGROUP, 0, car, object);
						drawSprite(car->sprite,hres/2,vres/2);
						drawCollidible(car->sprite,CARGROUP,car->id, car, object);
						drawSprite(arma, car->sprite->x + car->sprite->width/2, car->sprite->y + car->sprite->height/2);
						drawSprite(cursor,mouse_x,mouse_y);
						round_passed = false;
						
					}
					else
					{
						
						drawBackground(startBack);
						drawSprite(titleSp, 70, 300);
						drawSprite(startButton, 412, 430);	
						drawSprite(cursor, mouse_x, mouse_y);
						if(mouse_x <= (uint32_t)(startButton->x + startButton->width) && mouse_x >= (uint32_t)startButton->x && mouse_y >= (uint32_t)startButton->y && mouse_y <= (uint32_t)(startButton->y + startButton->height) )
						{
							if(LBPressed)
								menu = false;
							else
							{
								startButton->map = startButton2->map;
							}
							
						}
						else
						{
							startButton->map = startButton1->map;
						}
					}
					
					
				}
				
				if (msg.m_notify.interrupts & irq_kbd) {
					// Code has 2 bytes
					if (two_bytes) {
						
						kbc_ih();

						// Interrupt handler error handling
						if (ih_error) {
							ih_error = 0;
							continue;
						}

						// Check if its makecode or breakcode(Breakcode has MSB as 1)
						if (scancode & BIT(7))
							make = false;
						else
							make = true;

						size = 2;
						bytes[0] = 0xE0;
						bytes[1] = scancode;

						// Resetting the flag for the next while loop
						two_bytes = false;
					} else {

						kbc_ih();

						// Reading of the status of the keyboard
						if (ih_error) {
							ih_error = 0;
							continue;
						}

						// If the the msb is 0xE0 we set the flag of two_bytes to true and
						// we skip to the next iteration
						if (scancode == 0xE0) {
							two_bytes = true;
							continue;
						} else
							two_bytes = false;

						if (scancode & BIT(7))
						make = false;
						else
						make = true;
						size = 1;
						bytes[0] = scancode;
						
						if (scancode == 0x11)
							WPressed = true;
						else if (scancode == 0x91)
							WPressed = false;
						else if (scancode == 0x1F)
							SPressed = true;
						else if (scancode == 0x9F)
							SPressed = false;
						else if(scancode == 0x1E)
							APressed = true;
						else if(scancode == 0x9E)
							APressed = false;
						else if(scancode == 0x20)
							DPressed = true;
						else if(scancode == 0xA0)
							DPressed = false;	
					}
				}
                                if (msg.m_notify.interrupts & mouse_irq_set) 
				{
					mouse_ih();
					if (mouse_ih_error)
						continue;
				
					if (counter == 1) {
						
						if ((scancode & BIT(3)) == 0)
							continue;

						pp.bytes[0] = (uint16_t) scancode;
						counter++;
						if (scancode & MOUSE_X_DELTA_SIGN)
							sign_x = true;
						else
							sign_x = false;

						if (scancode & MOUSE_Y_DELTA_SIGN)
							sign_y = true;
						else
							sign_y = false;
						
					}
					else if (counter == 2) {

						pp.bytes[1] = scancode;
						counter++;
						
					}
					else if (counter == 3) {

						pp.bytes[2] = scancode;

						pp.delta_x = sign_x;
						pp.delta_y = sign_y;
						pp.lb = pp.bytes[0] & MOUSE_LB_PRESSED;
						pp.rb = pp.bytes[0] & MOUSE_RB_PRESSED;
						pp.mb = pp.bytes[0] & MOUSE_MB_PRESSED;
						pp.delta_x = ninebit16bit(sign_x, pp.bytes[1]);
						pp.delta_y = ninebit16bit(sign_y, pp.bytes[2]);
						pp.x_ov = pp.bytes[0] & MOUSE_X_OVF;
						pp.y_ov = pp.bytes[0] & MOUSE_Y_OVF;
						
						updateMousePosition( pp.delta_x, pp.delta_y);
						if(pp.lb)
							LBPressed = true;
						else 
							LBPressed = false;
						counter = 1;

					}
                                }                    
				if (msg.m_notify.interrupts & rtc_irq_set) {
                                        rtc_ih(); 
                                }
                                break;
			default:
				break; /*No other interruptions expected*/
			}
		} else {}
	}

	
	// Unsubcribing the interruptions
	if(timer_unsubscribe_int()){
                printf("Failed to unsubscribe mouse interrupts\n");
		return 1;
        }

	if(kbc_unsubscribe_int()){
                printf("Failed to unsubscribe kbc interrupts\n");
		return 1;
        }

	if(mouse_unsubscribe_int()){
                printf("Failed to unsubscribe mouse interrupts\n");
		return 1;
        }

	if(mouse_send_cmd(MOUSE_DISABLE_CMD)){
    		return 1;
        }

	if(rtc_unsubscribe_int())
	{
		printf("Failed to unsubscribe RTC interrupts\n");
		return 1;
	}


        //Freeing all the sprites
        for(size_t i = 0; i < 16; ++i){
                destroy_sprite(carAnim[i]);
                destroy_sprite(armaAnim[i]);
                destroy_sprite(enemyAnim[i]);
        }

        free(backTest);

        destroy_sprite(car->sprite);
        free(car);

        destroy_sprite(object->sprite);
        free(object);

	for(size_t k = 0; k < MAX_ENEMY_COUNT; k++)
	{
		free(enemies[k]);
		destroy_sprite(enemyBars[k]);
	}

	for(size_t j = 0; j < 11; j++)
	{
		destroy_sprite(digits[j]);
	}


        destroy_sprite(cursor);

        destroy_sprite(startButton);
        destroy_sprite(startButton1);
        destroy_sprite(startButton2);

        makeBulletsNull(bullets);

	vg_exit();

        return 0;
}

int game_loop_multi(uint16_t mode, unsigned int player){

	multi = true;
    	int ipc_status;
	message msg;
	int r;
	// Set to true when a scancode has two bytes
	bool two_bytes = false;
	// Used for the kbd_print_scancode function
	bool make;
	uint8_t size;
	uint8_t bytes[2];
	uint8_t irq_kbd = BIT(kbc_hook_id);
	uint8_t irq_timer = BIT(timer0_hook_id);

	uint8_t aux_timer0 = (uint8_t) timer0_hook_id;

	int counter = 1;

	bool sign_y;
  	bool sign_x;

  	struct packet pp;

	uint32_t mouse_irq_set = BIT(mouse_hook_id);

	uint32_t rtc_irq_set = BIT(rtc_hook_id);

	//uint32_t serial_port_irq_set = BIT(serial_port_hook_id);

	current_player = player;


	int gun_cooldown = CAR_GUN_COOLDOWN;

	int enemy_cooldown = ENEMY_SPAWN_COOLDOWN;

	round_time = FIXED_ROUND_TIME;


	if (vg_init(mode) == NULL) {
		vg_exit();
		printf("Init failed");
		return 1;
	}
  
	// Sprite *back = loadSprite(background);
	// Sprite *backM = loadSprite(backMenu);
	// Sprite *backMTest = loadSprite(backMTest_xpm);
	Sprite *backTest = loadSprite(backgroundFinal);

	Sprite *carr = loadSprite(carro1);

	Car * car = (Car*)malloc(sizeof(Car));

        if(!car){
                printf("Couldn't allocate memory for car\n");
                return 1;
        }

	car->sprite = carr;
	car->hp = CAR_HP;
	car->id = 0;

        Sprite *objj = loadSprite(obj);

	Objective *object = (Objective*)malloc(sizeof(Objective));

        if(!object){
                printf("Couldn't allocate memory for objective\n");
                return 1;
        }
        
	object->sprite = objj;
	object->hp = OBJECTIVE_HP; 

	Sprite *car1 = loadSprite(carro1);
	Sprite *car2 = loadSprite(carro2);
	Sprite *car3 = loadSprite(carro3);
	Sprite *car4 = loadSprite(carro4);
	Sprite *car5 = loadSprite(carro5);
	Sprite *car6 = loadSprite(carro6);
	Sprite *car7 = loadSprite(carro7);
	Sprite *car8 = loadSprite(carro8);
	Sprite *car12 = loadSprite(carro12);
	Sprite *car23 = loadSprite(carro23);
	Sprite *car34 = loadSprite(carro34);
	Sprite *car45 = loadSprite(carro45);
	Sprite *car56 = loadSprite(carro56);
	Sprite *car67 = loadSprite(carro67);
	Sprite *car78 = loadSprite(carro78);
	Sprite *car81 = loadSprite(carro81);

	Sprite *arma = loadSprite(arma1);
	Sprite *arm1 = loadSprite(arma1);
	Sprite *arm2 = loadSprite(arma2);
	Sprite *arm3 = loadSprite(arma3);
	Sprite *arm4 = loadSprite(arma4);
	Sprite *arm5 = loadSprite(arma5);
	Sprite *arm6 = loadSprite(arma6);
	Sprite *arm7 = loadSprite(arma7);
	Sprite *arm8 = loadSprite(arma8);
	Sprite *arm12 = loadSprite(arma12);
	Sprite *arm23 = loadSprite(arma23);
	Sprite *arm34 = loadSprite(arma34);
	Sprite *arm45 = loadSprite(arma45);
	Sprite *arm56 = loadSprite(arma56);
	Sprite *arm67 = loadSprite(arma67);
	Sprite *arm78 = loadSprite(arma78);
	Sprite *arm81 = loadSprite(arma81);

	Sprite *enem1 = loadSprite(enemy1);
	Sprite *enem2 = loadSprite(enemy2);
	Sprite *enem3 = loadSprite(enemy3);
	Sprite *enem4 = loadSprite(enemy4);
	Sprite *enem5 = loadSprite(enemy5);
	Sprite *enem6 = loadSprite(enemy6);
	Sprite *enem7 = loadSprite(enemy7);
	Sprite *enem8 = loadSprite(enemy8);
	Sprite *enem12 = loadSprite(enemy12);
	Sprite *enem23 = loadSprite(enemy23);
	Sprite *enem34 = loadSprite(enemy34);
	Sprite *enem45 = loadSprite(enemy45);
	Sprite *enem56 = loadSprite(enemy56);
	Sprite *enem67 = loadSprite(enemy67);
	Sprite *enem78 = loadSprite(enemy78);
	Sprite *enem81 = loadSprite(enemy81);

	Sprite *d0 = loadSprite(digit0);
	Sprite *d1 = loadSprite(digit1);
	Sprite *d2 = loadSprite(digit2);
	Sprite *d3 = loadSprite(digit3);
	Sprite *d4 = loadSprite(digit4);
	Sprite *d5 = loadSprite(digit5);
	Sprite *d6 = loadSprite(digit6);
	Sprite *d7 = loadSprite(digit7);
	Sprite *d8 = loadSprite(digit8);
	Sprite *d9 = loadSprite(digit9);
	Sprite *dt = loadSprite(dots);

	Sprite * digits[] = { d0, d1, d2, d3, d4, d5, d6, d7, d8, d9, dt};


	Sprite *carHpBar = loadSprite(carBar);
	Sprite *carHp = loadSprite(carHpFill);
	Sprite *objHpBar = loadSprite(objectiveBar);
	Sprite *objHp = loadSprite(objectiveHpFill);

	Sprite *gameOver = loadSprite(gameOverScreen);

	Sprite *round = loadSprite(roundSp);

	Sprite *scoreS = loadSprite(scoreSp);
	Sprite *highS = loadSprite(highSp);

  
	Sprite * carAnim[] = {car1, car12, car2, car23, car3, car34, car4, car45, car5, car56, car6,
	  car67, car7, car78, car8, car81};

	Sprite * armaAnim[] = {arm1, arm12, arm2, arm23, arm3, arm34, arm4, arm45, arm5, arm56, arm6, arm67, arm7, arm78, arm8, arm81};

	Sprite * enemyAnim[] = {enem1, enem12, enem2, enem23, enem3, enem34, enem4, enem45, enem5, enem56, enem6, enem67, enem7, enem78, enem8, enem81};

	Sprite *cursor = loadSprite(crosshair);

	Sprite *startButton = loadSprite(start1);
	Sprite *startButton1 = loadSprite(start1);
	Sprite *startButton2 = loadSprite(start2);
	Sprite *titleSp = loadSprite(title);
	Sprite *startBack = loadSprite(startMenu);
	Sprite *multB = loadSprite(multButton);
	Sprite *multB2 = loadSprite(multButton2);


	makeBulletsNull(bullets);

	drawBackground(backTest);

	drawSprite(object->sprite, 900, 80);
	drawCollidible(object->sprite, OBJECTIVEGROUP, 0, car, object);
	drawSprite(car->sprite,hres/2,vres/2);
	drawCollidible(car->sprite,CARGROUP,car->id, car, object);
	drawSprite(arma, car->sprite->x + car->sprite->width/2, car->sprite->y + car->sprite->height/2);
	drawSprite(cursor,mouse_x,mouse_y);


	

	// Subscribing the interruptions

	if (mouse_send_cmd(MOUSE_ENABLE_CMD)){
		return 1;
        }

	if (mouse_subscribe_int(&mouse_hook_id)){
                printf("Failed to subscribe mouse interrupts\n");
		return 1;
        }
  
	if(timer_subscribe_int(&aux_timer0)){
                printf("Failed to subscribe timer interrupts\n");
		return 1;
        }

	timer0_hook_id = (int)aux_timer0;

	if(player == 1)
	{
		if (kbc_subscribe_int(&kbc_hook_id)){
			printf("Failed to subscribe kbc interrupts");
			return 1;
		}
	}
	
	if(serial_port_subscribe_int(&serial_port_hook_id))
	{
		printf("Failed to subscribe UART interrupts");
		return 1;
	}
	
	srand(857835);

	if(rtc_subscribe_int(&rtc_hook_id)){
                printf("Failed to sunscribe rtc interrupts\n");
                return 1;
        }

	rtc_ih();

		

	while (!esc_pressed && !RBPressed) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* Received notication */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* Hardware interrupt notification */
				if (msg.m_notify.interrupts & irq_timer) {
					//sprite->yspeed += 1;		
					
					displayBuffer(video_buffer,video_mem,cameraX,cameraY);
					//memcpy(video_buffer, video_mem, hres*vres*bitsPerPixel/8);
					updateCameraCoords(car->sprite,&cameraX,&cameraY);
					cleanCollisionMatrix();
					if(car->hp <= 0 || object->hp <= 0 || round_time == 0)
					{
						game_over = true;
					}
					if(car->hp > 0 && object->hp > 0 && round_time == 0)
					{
						game_over = false;
						round_passed = true;
					}

					if(gun_cooldown > 0)
						gun_cooldown--;
					if(enemy_cooldown > 0)
						enemy_cooldown--;

					if(score < 0)
					{
						score = 0;
					}
					else if(score > 9999)
					{
						score = 9999;
					}

					if(!menu && !game_over && !round_passed){
						
						if (WPressed)
						{
							car->sprite->yspeed -= 1;
							if(car->sprite->yspeed < -8)
								car->sprite->yspeed = -8;
							
						}
						else
						{
							if(car->sprite->yspeed < 0)
							{
								car->sprite->yspeed += 1;
							}
						}
						
						if(SPressed)
						{
							car->sprite->yspeed += 1;
							if(car->sprite->yspeed > 8)
								car->sprite->yspeed = 8;
						}
						else
						{
							if(car->sprite->yspeed > 0)
							{
								car->sprite->yspeed -= 1;
							}
						}
						arma->yspeed = car->sprite->yspeed;
						

						if(APressed)
						{
							car->sprite->xspeed -= 1;
							arma->xspeed -= 1;
							if(car->sprite->xspeed < -8)
								car->sprite->xspeed = -8;
						}
						else
						{
							if(car->sprite->xspeed < 0)
							{
								car->sprite->xspeed += 1;
							}
						}

						if(DPressed)
						{
							

							car->sprite->xspeed += 1;
							arma->xspeed += 1;
							if(car->sprite->xspeed > 8)
								car->sprite->xspeed = 8;
						}
						else
						{
							if(car->sprite->xspeed > 0)
							{
								car->sprite->xspeed -= 1;
							}
						}
						arma->xspeed = car->sprite->xspeed;
						
						
						/*
						if((!SPressed && !WPressed) || (SPressed && WPressed))
						{
							sprite->yspeed = 0;
							
						}
						if((!APressed && !DPressed) || (APressed && DPressed))
						{
							sprite->xspeed = 0;
							
						}
						*/
					
						if(WPressed || SPressed || APressed || DPressed)
						{
							
							anim_Car(car, carAnim);
						}
						
						if(enemy_cooldown == 0)
						{
							spawnEnemy();
							enemy_cooldown = 120;
						}
						

						move_Car(car, arma);
						anim_Arma(arma, armaAnim);
						// drawBackground(back);
						drawBackground(backTest);
						drawSprite(object->sprite, 900, 80);
						drawCollidible(object->sprite, OBJECTIVEGROUP, 0, car, object);

						drawEnemies(car, object);

						drawSprite(car->sprite, car->sprite->x, car->sprite->y);
						drawCollidible(car->sprite,CARGROUP, car->id, car, object);
						
						

						//carHp->width = car->hp;
						carHp->width = car->hp;
						objHp->width = object->hp;
						drawSprite(carHp, ui_x + 35, ui_y + 8);
						drawSprite(carHpBar, ui_x, ui_y);
						drawSprite(objHp, ui_x + 780 + 38, ui_y + 8);
						drawSprite(objHpBar, ui_x + 780, ui_y - 7);
						
						drawSprite(arma, car->sprite->x + car->sprite->width/2 - arma->width/2, car->sprite->y + car->sprite->height/2 - arma->height/2);
						

						displayTimeAndScore(digits);

						drawSprite(digits[round_number], ui_x + 145, ui_y + 635);
						drawSprite(round, ui_x, ui_y + 635);
						drawSprite(scoreS, ui_x + hres - 129 - 100, ui_y + 635 - 50);
						
						drawSprite(cursor, mouse_x, mouse_y);

						if(LBPressed){
							if(gun_cooldown == 0)
							{
								shoot(bullets,car->sprite->x + car->sprite->width/2 - 10,car->sprite->y + car->sprite->height/2 - 10 ,mouse_x,mouse_y, ALLYBULLETGROUP);

								gun_cooldown = 20;
							}
							
							//shoot(bullets,500,500,mouse_x,mouse_y);
						}
						drawBullets(bullets, car, object);
						updateEnemies(car, object, enemyAnim);
						updateBullets(bullets);
						
						

					}
					else if(game_over)
					{
						if(score > high_score)
						{
							high_score = score;
						}
						drawBackground(backTest);
						drawSprite(gameOver, cameraX + hres/2 - gameOver->width/2, cameraY + 250);
						
						drawSprite(highS, cameraX + hres/2 - (highS->width + scoreS->width + 20)/2, cameraY + 380);
						drawSprite(scoreS, highS->x + highS->width + 20, cameraY + 380);
						drawSprite(digits[10], scoreS->x + scoreS->width + 5, cameraY + 380);

						unsigned int score_mil = high_score / 1000;
						unsigned int score_cen = (high_score - 1000 * score_mil) / 100;
						unsigned int score_dec = (high_score - 1000 * score_mil - 100 * score_cen) / 10;
						unsigned int score_un = (high_score - 1000 * score_mil - 100 * score_cen - 10 * score_dec);
						
						drawSprite(digits[score_mil], cameraX + hres/2 - 27*2, cameraY + 430);
						drawSprite(digits[score_cen], cameraX + hres/2 - 27, cameraY + 430);
						drawSprite(digits[score_dec], cameraX + hres/2, cameraY + 430);
						drawSprite(digits[score_un], cameraX + hres/2 + 27,cameraY + 430);

						drawSprite(startButton, cameraX + 412, cameraY + 500);
						drawSprite(cursor, mouse_x, mouse_y);

						if(mouse_x <= (uint32_t)(startButton->x + startButton->width) && mouse_x >= (uint32_t)startButton->x && mouse_y >= (uint32_t)startButton->y && mouse_y <= (uint32_t)(startButton->y + startButton->height) )
						{
							if(LBPressed) {
								game_over = false;
								ui_x = user_interface_x;
								ui_y = user_interface_y;
								cameraX = 0;
								cameraY = 0;
								round_time = FIXED_ROUND_TIME;
								round_number = 1;
								score = 0;
								car->hp = CAR_HP;
								object->hp = OBJECTIVE_HP;
								for(size_t i = 0; i < MAX_ENEMY_COUNT; i++)
								{
									free(enemies[i]);
									enemies[i] = NULL;
								}
								makeBulletsNull(bullets);

								drawBackground(backTest);
								drawSprite(object->sprite, 900, 80);
								drawCollidible(object->sprite, OBJECTIVEGROUP, 0, car, object);
								drawSprite(car->sprite,hres/2,vres/2);
								drawCollidible(car->sprite,CARGROUP,car->id, car, object);
								drawSprite(arma, car->sprite->x + car->sprite->width/2, car->sprite->y + car->sprite->height/2);
								drawSprite(cursor,mouse_x,mouse_y);
							}
							else
							{
								startButton->map = startButton2->map;
							}
							
						}
						else
						{
							startButton->map = startButton1->map;
						}

					}
					else if(round_passed)
					{
						score += 500;
						round_number++;
						round_time = FIXED_ROUND_TIME;
						car->hp = 100;
						object->hp = 100;
						for(size_t i = 0; i < MAX_ENEMY_COUNT; i++)
						{
							free(enemies[i]);
							enemies[i] = NULL;
						}
						makeBulletsNull(bullets);

						drawBackground(backTest);

						ui_x = user_interface_x;
						ui_y = user_interface_y;
						cameraX = 0;
						cameraY = 0;


						drawSprite(object->sprite, 900, 80);
						drawCollidible(object->sprite, OBJECTIVEGROUP, 0, car, object);
						drawSprite(car->sprite,hres/2,vres/2);
						drawCollidible(car->sprite,CARGROUP,car->id, car, object);
						drawSprite(arma, car->sprite->x + car->sprite->width/2, car->sprite->y + car->sprite->height/2);
						drawSprite(cursor,mouse_x,mouse_y);
						round_passed = false;
						
					}
					else
					{
						
						drawBackground(startBack);
						drawSprite(titleSp, 70, 300);
						drawSprite(multB, 412, 430);	
						drawSprite(cursor, mouse_x, mouse_y);
						if(mouse_x <= (uint32_t)(multB->x + multB->width) && mouse_x >= (uint32_t)multB->x && mouse_y >= (uint32_t)multB->y && mouse_y <= (uint32_t)(multB->y + multB->height) )
						{
							if(LBPressed)
								menu = false;
							else
							{
								multB->map = multB2->map;
							}
							
						}
						else
						{
							multB->map = multB->map;
						}
					}
					serial_port_rec();
					
				}
				if(player == 1)
				{
					if (msg.m_notify.interrupts & irq_kbd) {
						
							
							// Code has 2 bytes
							if (two_bytes) {
								
								kbc_ih();

								// Interrupt handler error handling
								if (ih_error) {
									ih_error = 0;
									continue;
								}

								serial_port_write(scancode);
								serial_port_send();
								// Check if its makecode or breakcode(Breakcode has MSB as 1)
								if (scancode & BIT(7))
									make = false;
								else
									make = true;

								size = 2;
								bytes[0] = 0xE0;
								bytes[1] = scancode;
								serial_port_write('K');
								serial_port_write(bytes[0]);
								serial_port_write(bytes[1]);
								serial_port_send();
								// Resetting the flag for the next while loop
								two_bytes = false;
							} else {
								

								kbc_ih();

								// Reading of the status of the keyboard
								if (ih_error) {
									ih_error = 0;
									continue;
								}
								
								
								
								// If the the msb is 0xE0 we set the flag of two_bytes to true and
								// we skip to the next iteration
								if (scancode == 0xE0) {
									two_bytes = true;
									continue;
								} else
									two_bytes = false;

								if (scancode & BIT(7))
								make = false;
								else
								make = true;
								size = 1;
								bytes[0] = scancode;

								serial_port_write('K');
								serial_port_write(bytes[0]);
								serial_port_send();

								if (scancode == 0x11)
									WPressed = true;
								else if (scancode == 0x91)
									WPressed = false;
								else if (scancode == 0x1F)
									SPressed = true;
								else if (scancode == 0x9F)
									SPressed = false;
								else if(scancode == 0x1E)
									APressed = true;
								else if(scancode == 0x9E)
									APressed = false;
								else if(scancode == 0x20)
									DPressed = true;
								else if(scancode == 0xA0)
									DPressed = false;
								else if(scancode == 0x01)
									esc_pressed = true;	
							}
							
					}
				}
                                if (msg.m_notify.interrupts & mouse_irq_set) 
				{
					mouse_ih();
					if (mouse_ih_error)
						continue;
					if(player == 2)
					{
						if (counter == 1) {
							
							if ((scancode & BIT(3)) == 0)
								continue;

							pp.bytes[0] = (uint16_t) scancode;
							counter++;
							if (scancode & MOUSE_X_DELTA_SIGN)
								sign_x = true;
							else
								sign_x = false;

							if (scancode & MOUSE_Y_DELTA_SIGN)
								sign_y = true;
							else
								sign_y = false;
							
							
						}
						else if (counter == 2) {

							pp.bytes[1] = scancode;
							counter++;
						}
						else if (counter == 3) {

							pp.bytes[2] = scancode;

							pp.delta_x = sign_x;
							pp.delta_y = sign_y;
							pp.lb = pp.bytes[0] & MOUSE_LB_PRESSED;
							pp.rb = pp.bytes[0] & MOUSE_RB_PRESSED;
							pp.mb = pp.bytes[0] & MOUSE_MB_PRESSED;
							pp.delta_x = ninebit16bit(sign_x, pp.bytes[1]);
							pp.delta_y = ninebit16bit(sign_y, pp.bytes[2]);
							pp.x_ov = pp.bytes[0] & MOUSE_X_OVF;
							pp.y_ov = pp.bytes[0] & MOUSE_Y_OVF;
							
							updateMousePosition( pp.delta_x, pp.delta_y);
							if(pp.lb)
								LBPressed = true;
							else 
								LBPressed = false;
							if(pp.rb)
								RBPressed = true;
							counter = 1;
							
							serial_port_write('M');
							serial_port_write(pp.bytes[0]);
							serial_port_write(pp.bytes[1]);
							serial_port_write(pp.bytes[2]);
							serial_port_send();
						}

						
						
					}
               			}               
				if (msg.m_notify.interrupts & rtc_irq_set) {
                                        rtc_ih(); 
                	}
                    		break;
			default:
				break; /*No other interruptions expected*/
			}
		} else {}

	}

	vg_exit();
	// Unsubcribing the interruptions
	if(timer_unsubscribe_int()){
                printf("Failed to unsubscribe mouse interrupts\n");
		return 1;
        }
	if(player == 1)
	{
		if(kbc_unsubscribe_int()){
			printf("Failed to unsubscribe kbc interrupts\n");
			return 1;
		}
	}

	if(mouse_unsubscribe_int()){
                printf("Failed to unsubscribe mouse interrupts\n");
		return 1;
        }

	if(mouse_send_cmd(MOUSE_DISABLE_CMD)){
    		return 1;
        }

	if(serial_port_unsubscribe_int())
	{
		printf("Failed to unsubscribe UART interrupts\n");
		return 1;
	}

	if(rtc_unsubscribe_int())
	{
		printf("Failed to unsubscribe RTC interrupts\n");
		return 1;
	}


        //Freeing all the sprites
        for(size_t i = 0; i < 16; ++i){
                destroy_sprite(carAnim[i]);
                destroy_sprite(armaAnim[i]);
                destroy_sprite(enemyAnim[i]);
        }

        free(backTest);

        destroy_sprite(car->sprite);
        free(car);

        destroy_sprite(object->sprite);
        free(object);

	for(size_t k = 0; k < MAX_ENEMY_COUNT; k++)
	{
		free(enemies[k]);
		destroy_sprite(enemyBars[k]);
	}

	for(size_t j = 0; j < 11; j++)
	{
		destroy_sprite(digits[j]);
	}


        destroy_sprite(cursor);

        destroy_sprite(startButton);
        destroy_sprite(startButton1);
        destroy_sprite(startButton2);

        makeBulletsNull(bullets);

	

        return 0;
}
