// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

// Any header files included below this line should have been created by you
#include "kbd.h"
#include "i8042.h"
#include "i8254.h"
#include "mouse.h"
#include "ps2.h"
#include "vga.h"
#include "game.h"
#include "bmp.h"
#include "rtc.h"

#include "img/floor.xpm"
#include "img/souto.xpm"
#include "img/zombie.xpm"
#include "img/mouse.xpm"
#include "img/boost.xpm"

#include <sys/types.h> 
#include <unistd.h>
#include <string.h>

bool printZombie = false, printFloor = false, printBoost = false;

xpm_image_t zombiexpm, floorxpm, boostxpm;

void writeHorCenter(char string[], int pos_y){
	unsigned int len = strlen(string);
	if(len % 2){
		for(unsigned int i = 0; i < len; i++){
			//printf("Char: %c (%d) at pos X: %d Y: %d\n", string[i], string[i] - 'A',((getHRes()/2)-((len/2)*30)) + 30*i, pos_y);
			if(string[i]>= 'A' && string[i]<= 'Z'){
				drawNewXPM(alphabet.letters[string[i] - 'A'], ((getHRes()/2)-((len/2)*30)) + 30*i, pos_y);
			}
			else if(string[i]>= 'a' && string[i]<= 'z'){
				drawNewXPM(alphabet.letters[string[i] - 'a'], ((getHRes()/2)-((len/2)*30)) + 30*i, pos_y);
			}
			else if(string[i]>= '0' && string[i]<= '9'){
				drawNewXPM(alphabet.numbers[string[i] - '0'], ((getHRes()/2)-((len/2)*30)) + 30*i, pos_y);
			}
			else if(string[i]== '.'){
				drawNewXPM(alphabet.dotxpm, ((getHRes()/2)-((len/2)*30)) + 30*i, pos_y);
			}
			else if(string[i]== ':'){
				drawNewXPM(alphabet.colonxpm, ((getHRes()/2)-((len/2)*30)) + 30*i, pos_y);
			}
			else if(string[i]== '/'){
				drawNewXPM(alphabet.dashxpm, ((getHRes()/2)-((len/2)*30)) + 30*i, pos_y);
			}
			else if(string[i]== '-'){
				drawNewXPM(alphabet.slashxpm, ((getHRes()/2)-((len/2)*30)) + 30*i, pos_y);
			}
		}
	}
	else{
		for(unsigned int i = 0; i < len; i++){
			//printf("Char: %c (%d) at pos X: %d Y: %d\n", string[i], string[i] - 'A',((getHRes()/2)-(((len-1)/2)*30)-15) + 30*i, pos_y);
			if(string[i]>= 'A' && string[i]<= 'Z'){
				drawNewXPM(alphabet.letters[string[i] - 'A'], ((getHRes()/2)-(((len-1)/2)*30)-15) + 30*i, pos_y);
			}
			else if(string[i]>= 'a' && string[i]<= 'z'){
				drawNewXPM(alphabet.letters[string[i] - 'a'], ((getHRes()/2)-(((len-1)/2)*30)-15) + 30*i, pos_y);
			}
			else if(string[i]>= '0' && string[i]<= '9'){
				drawNewXPM(alphabet.numbers[string[i] - '0'], ((getHRes()/2)-(((len-1)/2)*30)-15) + 30*i, pos_y);
			}
			else if(string[i]== '.'){
				drawNewXPM(alphabet.dotxpm, ((getHRes()/2)-(((len-1)/2)*30)-15) + 30*i, pos_y);
			}
			else if(string[i]== ':'){
				drawNewXPM(alphabet.colonxpm, ((getHRes()/2)-(((len-1)/2)*30)-15) + 30*i, pos_y);
			}
			else if(string[i]== '/'){
				drawNewXPM(alphabet.dashxpm, ((getHRes()/2)-(((len-1)/2)*30)-15) + 30*i, pos_y);
			}
			else if(string[i]== '-'){
				drawNewXPM(alphabet.slashxpm, ((getHRes()/2)-(((len-1)/2)*30)-15) + 30*i, pos_y);
			}
		}
	}
}

void printPoints(){
	if(printAlphabet == true){
		drawNewXPM(alphabet.letters['P'-'A'], getHRes() - (13*30), 30);
		drawNewXPM(alphabet.letters['O'-'A'], getHRes() - (12*30), 30);
		drawNewXPM(alphabet.letters['I'-'A'], getHRes() - (11*30), 30);
		drawNewXPM(alphabet.letters['N'-'A'], getHRes() - (10*30), 30);
		drawNewXPM(alphabet.letters['T'-'A'], getHRes() - (9*30), 30);
		drawNewXPM(alphabet.letters['S'-'A'], getHRes() - (8*30), 30);
		drawNewXPM(alphabet.colonxpm, getHRes() - (7*30), 30);
		drawNewXPM(alphabet.numbers[((souto->score)/100)%10], getHRes() - (5*30), 30);
		drawNewXPM(alphabet.numbers[((souto->score)/10)%10], getHRes() - (4*30), 30);
		drawNewXPM(alphabet.numbers[(souto->score)%10], getHRes() - (3*30), 30);
		
	}
}

void drawOptions(){
	if(printButtons){
		for(unsigned int i = 1; i < 6; i++){
			switch(i){
				case 1:
					drawNewXPM(buttonsXpm.size_1_xpm, getHRes()/2 - 100, i*getVRes()/6 - 15);
					break;
				case 2:
					drawNewXPM(buttonsXpm.size_2_xpm, getHRes()/2 - 100, i*getVRes()/6 - 15);
					break;
				case 3:
					drawNewXPM(buttonsXpm.size_3_xpm, getHRes()/2 - 100, i*getVRes()/6 - 15);
					break;
				case 4:
					drawNewXPM(buttonsXpm.size_4_xpm, getHRes()/2 - 100, i*getVRes()/6 - 15);
					break;
				case 5:
					drawNewXPM(buttonsXpm.size_5_xpm, getHRes()/2 - 100, i*getVRes()/6 - 15);
					break;
				default:
					break;
			}
		}
		drawNewXPM(buttonsXpm.backxpm, 30, 30);
	}
	else gameStatus = game;
}

void drawMenu(){
	if(printButtons){
		for(unsigned int i = 2; i < 6; i++){
			switch(i){
				case 2:
					drawNewXPM(buttonsXpm.startxpm, getHRes()/2 - 100, i*getVRes()/6 - 15);
					break;
				case 3:
					drawNewXPM(buttonsXpm.hiscoxpm, getHRes()/2 - 100, i*getVRes()/6 - 15);
					break;
				case 4:
					drawNewXPM(buttonsXpm.optionsxpm, getHRes()/2 - 100, i*getVRes()/6 - 15);
					break;
				case 5:
					drawNewXPM(buttonsXpm.quitxpm, getHRes()/2 - 100, i*getVRes()/6 - 15);
					break;
				default:
					break;
			}
		}
	}
	else gameStatus = game;
}

void drawBoost(){
	if(boost.active && printBoost){
		int difX = boost.x - souto->x;
		int difY = boost.y - souto->y;
		
		drawNewXPM(boostxpm, (getHRes()/2)+difX-20, (getVRes()/2)+difY-20);
	}
}

void drawMenuBackground(){
	clearScreenDouble();
	if(printFloor){
		for(int i=0; i < ((getHRes()/200)+2); i++){
			for(int j=0; j < ((getVRes()/200)+2); j++){
				drawNewXPM(floorxpm, i*200, j*200);
			}
		}
	}
	else{
		for(int i=0; i < getHRes(); i++){
			for(int j=0; j < getVRes(); j++){
				if(((i+souto->x)-(getHRes()/2) > 0) && ((j+souto->y)-(getVRes()/2) > 0) && ((i+souto->x)-(getHRes()/2) < 2000) && ((j+souto->y)-(getVRes()/2) < 2000)){
					if(((i+souto->x)-(getHRes()/2))%100 == 0 || ((j+souto->y)-(getVRes()/2))%100 == 0){
						set_pixel_double(i,j,0xFFFFFF);
					}
					else set_pixel_double(i,j,0x0000FF);
				}
				else set_pixel_double(i,j,0x000000);
			}
		}
	}
}

void drawBackground(){
	clearScreenDouble();
	if(printFloor){
		for(int i=0; i < ((getHRes()/200)+2); i++){
			for(int j=0; j < ((getVRes()/200)+2); j++){
				int xPos = ((i*200)+souto->x)-(getHRes()/2);
				int yPos = ((j*200)+souto->y)-(getVRes()/2);
				if((xPos >= 0) && (yPos >= 0) && (xPos < 2000) && (yPos < 2000)){
					drawNewXPM(floorxpm,(((i*200)/*-(getHRes()/2)*/) - (xPos%200)),(((j*200)/*-(getVRes()/2)*/) - (yPos%200)));
				}
				else set_pixel_double(i,j,0x000000);
			}
		}
	}
	else{
		for(int i=0; i < getHRes(); i++){
			for(int j=0; j < getVRes(); j++){
				if(((i+souto->x)-(getHRes()/2) > 0) && ((j+souto->y)-(getVRes()/2) > 0) && ((i+souto->x)-(getHRes()/2) < 2000) && ((j+souto->y)-(getVRes()/2) < 2000)){
					if(((i+souto->x)-(getHRes()/2))%100 == 0 || ((j+souto->y)-(getVRes()/2))%100 == 0){
						set_pixel_double(i,j,0xFFFFFF);
					}
					else set_pixel_double(i,j,0x0000FF);
				}
				else set_pixel_double(i,j,0x000000);
			}
		}
	}
}

void chaseHero(zombie *student){
	if(student->x < souto->x && student->speed_x < 5){
		student->speed_x+=1;
	}
	else if(student->x > souto->x && student->speed_x > -5){
		student->speed_x-=1;
	}
	else if(student->x == souto->x){
		student->speed_x = 0;
	}
	
	if(student->y < souto->y && student->speed_y < 5){
		student->speed_y+=1;
	}
	else if(student->y > souto->y && student->speed_y > -5){
		student->speed_y-=1;
	}
	else if(student->y == souto->y){
		student->speed_y = 0;
	}
	if(student->speed_x > 5) student->speed_x = 5;
	if(student->speed_x < -5) student->speed_x = -5;
	if(student->speed_y > 5) student->speed_y = 5;
	if(student->speed_y < -5) student->speed_y = -5;
	
	if(colision(student->x + student->speed_x, student->y + student->speed_y) != (int)student->id && colision(student->x + student->speed_x, student->y + student->speed_y) != -1){
		student->x = student->x;
		student->y = student->y;
	}
	else{
		student->x += student->speed_x;
		student->y += student->speed_y;
	}
	
	if(student->x < 0) student->x = 0;
	else if(student->x > 2000) student->x = 2000;
	if(student->y < 0) student->y = 0;
	else if(student->y > 2000) student->y = 2000;
}

void drawZombie(zombie *student){
	int difX = student->x - souto->x;
	int difY = student->y - souto->y;
	
	if(mod(difX)-100 < getHRes()/2 && mod(difY)-92 < getVRes()/2){
		if(printZombie == false)vg_draw_rectangle_double((getHRes()/2)+difX, (getVRes()/2)+difY, 100, 92, 0xFF0000);
		else drawNewXPM(zombiexpm, (getHRes()/2)+difX-50, (getVRes()/2)+difY-47);
	}
}

void handleFrozenEnemies(){
	zombie * curr = alunos->first;
	for(unsigned int i=0; i < alunos->size; i++){
		if(!(curr->alive)){
			break;
		}
		drawZombie(curr);
		curr = curr->next;
	}
}

int handleEnemies(){
	zombie * curr = alunos->first;
	for(unsigned int i=0; i < alunos->size; i++){
		if(!(curr->alive)){
			break;
		}
		chaseHero(curr);
		if(colision(curr->x, curr->y) == -1) return 1;
		drawZombie(curr);
		curr = curr->next;
	}
	return 0;
}

void keyHandle(){
	//bool msb = (kbc_code & BIT(7)) ? true : false;

          uint8_t kbc_code_bytes[3];

          kbc_code_bytes[0] = (kbc_code >> 0) & 0xFF;
          kbc_code_bytes[1] = (kbc_code >> 8) & 0xFF;
          kbc_code_bytes[2] = (kbc_code >> 16) & 0xFF;

          //kbd_print_scancode(msb, 1, kbc_code_bytes);
          if (kbc_code_bytes[0] == ESC_MAKE)
          {
			  ks.esc = true;
          }
          if (kbc_code_bytes[0] == ESC_BREAK)
          {
			  ks.esc = false;
          }
          if (kbc_code_bytes[0] == ONE_MAKE)
          {
			  ks.numb1 = true;
		  }
          if (kbc_code_bytes[0] == ONE_BREAK)
          {
			  ks.numb1 = false;
		  }
          if (kbc_code_bytes[0] == TWO_MAKE)
          {
			  ks.numb2 = true;
		  }
          if (kbc_code_bytes[0] == TWO_BREAK)
          {
			  ks.numb2 = false;
		  }
          if (kbc_code_bytes[0] == THREE_MAKE)
          {
			  ks.numb3 = true;
		  }
          if (kbc_code_bytes[0] == THREE_BREAK)
          {
			  ks.numb3 = false;
		  }
          if (kbc_code_bytes[0] == FOUR_MAKE)
          {
			  ks.numb4 = true;
		  }
          if (kbc_code_bytes[0] == FOUR_BREAK)
          {
			  ks.numb4 = false;
		  }
          if (kbc_code_bytes[0] == FIVE_MAKE)
          {
			  ks.numb5 = true;
		  }
          if (kbc_code_bytes[0] == FIVE_BREAK)
          {
			  ks.numb5 = false;
		  }
          if (kbc_code_bytes[0] == W_MAKE)
          {
			  ks.w = true;
		  }
          if (kbc_code_bytes[0] == W_BREAK)
          {
			  ks.w = false;
		  }
          if (kbc_code_bytes[0] == S_MAKE)
          {
			  ks.s = true;
		  }
          if (kbc_code_bytes[0] == S_BREAK)
          {
			  ks.s = false;
		  }
          if (kbc_code_bytes[0] == A_MAKE)
          {
			  ks.a = true;
		  }
          if (kbc_code_bytes[0] == A_BREAK)
          {
			  ks.a = false;
		  }
          if (kbc_code_bytes[0] == D_MAKE)
          {
			  ks.d = true;
		  }
          if (kbc_code_bytes[0] == D_BREAK)
          {
			  ks.d = false;
		  }
		  if (kbc_code_bytes[0] == L_SHIFT_MAKE)
		  {
			  ks.shift = true;
		  }
		  if (kbc_code_bytes[0] == L_SHIFT_BREAK)
		  {
			  ks.shift = false;
		  }
		  if (kbc_code_bytes[0] == UP_ARROW_MAKE)
		  {
			  ks.up = true;
		  }
		  if (kbc_code_bytes[0] == UP_ARROW_BREAK)
		  {
			  ks.up = false;
		  }
		  if (kbc_code_bytes[0] == DOWN_ARROW_MAKE)
		  {
			  ks.down = true;
		  }
		  if (kbc_code_bytes[0] == DOWN_ARROW_BREAK)
		  {
			  ks.down = false;
		  }
		  if (kbc_code_bytes[0] == LEFT_ARROW_MAKE)
		  {
			  ks.left = true;
		  }
		  if (kbc_code_bytes[0] == LEFT_ARROW_BREAK)
		  {
			  ks.left = false;
		  }
		  if (kbc_code_bytes[0] == RIGHT_ARROW_MAKE)
		  {
			  ks.right = true;
		  }
		  if (kbc_code_bytes[0] == RIGHT_ARROW_BREAK)
		  {
			  ks.right = false;
		  }
		  if (kbc_code_bytes[0] == ENTER_MAKE)
		  {
			  ks.enter = true;
		  }
		  if (kbc_code_bytes[0] == ENTER_BREAK)
		  {
			  ks.enter = false;
		  }
}

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (proj_main_loop)(int argc, char *argv[]) {
  /* To be completed */
  printf("%s(%d, %p): under construction\n", __func__, argc, argv);
  
  srand(time(0));

  int estado, fr_rate=60;
  uint8_t kbd_bit, mouse_bit, tmr_bit, rtc_bit, max_speed = 5;
  uint16_t x=0, y=0;
  message msg;
  bool running = true;
  bool lb = false, rb=false;
  
  //int sx=0, sy=0, px=0,py=0;
  
  gameInit();
  
  enable_rtc_int();
	
  disable_per_rtc_int();

  if (kbd_subscribe_int(&kbd_bit) != 0)
  {
		printf("kbd_subscribe_int(34): Couldn't subscribe to keyboard interrupts\n");
		goto UNSUB_LABEL;
  }
  
  timer_subscribe_int(&tmr_bit);
  
  if(init_mouse(&mouse_bit)){
		printf("mouse_test_packet(46): problem initializing mouse!\n");
		goto UNSUB_LABEL;
  }
  
  if(rtc_subscribe_int(&rtc_bit)!=0){
		printf("Failed to subscribe to rtc!\n");
		goto UNSUB_LABEL;
  }
  
  char *vm = (char*)vg_init(0x115);
  
  bool printSouto = false, printMouse = false;
  
  xpm_image_t soutoxpm, mousexpm;
  
  if (xpm_load(floor_xpm, XPM_8_8_8, &floorxpm) != NULL){
	  printFloor = true;
  }
  
  if (xpm_load(souto_xpm, XPM_8_8_8, &soutoxpm) != NULL){
	  printSouto = true;
  }
  
  if (xpm_load(zombie_xpm, XPM_8_8_8, &zombiexpm) != NULL){
	  printZombie = true;
  }
  
  if (xpm_load(mouse_xpm, XPM_8_8_8, &mousexpm) != NULL){
	  printMouse = true;
  }
  
  if (xpm_load(boost_xpm, XPM_8_8_8, &boostxpm) != NULL){
	  printBoost = true;
  }
  
  if(loadAlphanum() == 0) printAlphabet = true;
  if(loadButtons() == 0) printButtons = true;
  
  char player[] = "AAA";
  
  unsigned int namePos = 0;
  
  bool lap = false, rap = false, uap = false, dap = false;
  timedate currDate;
  
  hscore currScore;
  
  while (gameStatus != quit)
  {
    driver_receive(ANY, &msg, &estado);
    if (is_ipc_notify(estado))
    {
      switch (_ENDPOINT_P(msg.m_source))
      {
      case HARDWARE:
		if (msg.m_notify.interrupts & BIT(rtc_bit)){
			rtc_ih();
			
			if(rtc_flags_status.alarm_flag == true){
				makeBoost();
				souto->status = none;
				rtc_flags_status.alarm_flag = false;
			}
			
			if(rtc_flags_status.periodic_flag == true){
				rtc_flags_status.periodic_flag = false;
			}
			
			if(rtc_flags_status.update_flag == true){
				rtc_flags_status.update_flag = false;
			}
		}
        if (msg.m_notify.interrupts & BIT(mouse_bit)){
			mouse_ih();
						
			if(counter == -1)
				break;
							
			if(counter > 2) {
				
				if(paux->rb)rb=true;
				else if(!(paux->rb)) rb=false;
				if(paux->lb)lb=true;
				else if(!(paux->lb)) lb=false;
				
				if(!(paux->x_ov))x+=paux->delta_x;
				if(!(paux->y_ov))y-=paux->delta_y;
				
				if(x < 0) x=0;
				else if(x>getHRes()) x= getHRes();
				
				if(y < 0) y=0;
				else if(y>getVRes()) y= getVRes();
							
				counter = 0;
			}
		}
        if (msg.m_notify.interrupts & BIT(kbd_bit))
        {
          kbc_ih();
          keyHandle();
          
          if (ks.esc)
          { //Exists upon the release of ESC key
            running = false;
          }
          
          if (ks.numb1)
          {
			  vg_exit();
  
			  printf("%x\n",vm);
			  
			  vm = (char*)vg_init(0x10F);
		  }
          if (ks.numb2)
          {
			  vg_exit();
  
			  printf("%x\n",vm);
			  
			  vm = (char*)vg_init(0x112);
		  }
          if (ks.numb3)
          {
			  vg_exit();
  
			  printf("%x\n",vm);
			  
			  vm = (char*)vg_init(0x115);
		  }
          if (ks.numb4)
          {
			  vg_exit();
  
			  printf("%x\n",vm);
			  
			  vm = (char*)vg_init(0x118);
		  }
          if (ks.numb5)
          {
			  vg_exit();
  
			  printf("%x\n",vm);
			  
			  vm = (char*)vg_init(0x11B);
		  }
          
        }
        if (msg.m_notify.interrupts & BIT(tmr_bit))
        {
			//cnt++;
			timer_int_handler();
			if((cont*fr_rate) % 60 == 0){
				if(gameStatus == menu){
					if(ks.esc){
						gameStatus = quit;
						break;
					}
					drawMenuBackground();
					drawMenu();
					if(printMouse){
						drawNewXPM(mousexpm, x-12, y-12);
					}
					
					if(lb == true){
						lb = false;
						if(x > getHRes()/2 - 100 && x < getHRes()/2 + 100){
							if(y > 2*getVRes()/6 - 15 && y < 2*getVRes()/6 + 15){
								gameInit();
								gameStatus = game;
							}
							else if(y > 3*getVRes()/6 - 15 && y < 3*getVRes()/6 + 15) gameStatus = hiscore;
							else if(y > 4*getVRes()/6 - 15 && y < 4*getVRes()/6 + 15) gameStatus = options;
							else if(y > 5*getVRes()/6 - 15 && y < 5*getVRes()/6 + 15) gameStatus = quit;
						}
					}
					
					swap();
				}
				else if(gameStatus == options){
					if(ks.esc){
						gameStatus = menu;
						ks.esc = false;
						break;
					}
					drawMenuBackground();
					drawOptions();
					if(printMouse){
						drawNewXPM(mousexpm, x-12, y-12);
					}
					
					if(lb == true){
						lb = false;
						if(x > getHRes()/2 - 100 && x < getHRes()/2 + 100){
							if(y > 1*getVRes()/6 - 15 && y < 1*getVRes()/6 + 15){
								vg_exit();
					  
								printf("%x\n",vm);
								  
								vm = (char*)vg_init(0x10F);
							}
							else if(y > 2*getVRes()/6 - 15 && y < 2*getVRes()/6 + 15){
								vg_exit();
					  
								printf("%x\n",vm);
								  
								vm = (char*)vg_init(0x112);
							}
							else if(y > 3*getVRes()/6 - 15 && y < 3*getVRes()/6 + 15){
								vg_exit();
					  
								printf("%x\n",vm);
								  
								vm = (char*)vg_init(0x115);
							}
							else if(y > 4*getVRes()/6 - 15 && y < 4*getVRes()/6 + 15){
								vg_exit();
					  
								printf("%x\n",vm);
								  
								vm = (char*)vg_init(0x118);
							}
							else if(y > 5*getVRes()/6 - 15 && y < 5*getVRes()/6 + 15){
								vg_exit();
					  
								printf("%x\n",vm);
								  
								vm = (char*)vg_init(0x11B);
							}
						}
						else if(x > 30 && x < 230 && y > 30 && y < 60) gameStatus = menu;
					}
					
					swap();
				}
				else if(gameStatus == save){
					//TODO
					drawBackground();
					if(printSouto == true) drawNewXPM(soutoxpm, (getHRes()/2)-50, (getVRes()/2)-33);
					handleFrozenEnemies();
					printPoints();
					writeHorCenter("You just died.", 75);
					writeHorCenter("Insert your name", 120);
					writeHorCenter("to save your score.", 165);
					writeHorCenter("Navigate with", 210);
					writeHorCenter("the arrow keys.", 255);
					writeHorCenter("Press Enter when finished.", 300);
					if(ks.esc){
						gameStatus = menu;
						ks.esc = false;
						break;
					}
					int currChar = player[namePos];
					if(ks.up){
						printf("up: %d %d = %d\n", 'A', player[namePos], (player[namePos] < 'Z'));
					}
					if(ks.up && player[namePos] < 'Z' && uap == false){
						uap = true;
						player[namePos]++;
					}
					if(ks.up == false){
						uap = false;
					}
					if(ks.down){
						printf("down: %d %d = %d %d\n", 'A', currChar, ((currChar*2)/2 > 65), (66>65));
					}
					if(ks.down && (player[namePos]) > 65 && dap == false){
						dap = true;
						player[namePos]--;
					}
					if(ks.down == false){
						dap = false;
					}
					if(ks.right && namePos < 2 && rap == false){
						rap = true;
						namePos++;
					}
					if(ks.right == false){
						rap = false;
					}
					if(ks.left && namePos > 0 && lap == false){
						lap = true;
						namePos--;
					}
					if(ks.left == false){
						lap = false;
					}
					
					if(ks.enter){
						currScore.points = souto->score;
						currScore.active = true;
						strcpy(currScore.player, player);
						sprintf(currScore.date, "%02d/%02d/%04d %02d:%02d:%02d", currDate.day, currDate.month, currDate.year, currDate.hours, currDate.minutes, currDate.seconds);
						//printf("%s\n",currScore.date);
						writeHorCenter(currScore.date, getVRes()-120);
					}
					
					writeHorCenter(player, getVRes()-60);
					swap();
					//gameStatus = menu;
				}
				else if(gameStatus == hiscore){
					//TODO
					gameStatus = game;
				}
				else if(gameStatus == game){
				
					if(onBoost(souto->x, souto->y)){
						destroyBoost();
						set_alarm(15);
						souto->status = fast;
					}
					
					clearScreenDouble();
					
					if(souto->status == fast) max_speed = 10;
					else max_speed = 5;
					
					if(ks.s && souto->y < 2000){
						if(souto->speed_y < max_speed) souto->speed_y++;
						souto->y+=souto->speed_y;
					}
					if(ks.w && souto->y > 0){
						if(souto->speed_y > -max_speed) souto->speed_y--;
						souto->y+=souto->speed_y;
					}
					if(!ks.s && souto->speed_y > 0){
						souto->speed_y--;
						souto->y+=souto->speed_y;
					}
					if(!ks.w && souto->speed_y < 0){
						souto->speed_y++;
						souto->y+=souto->speed_y;
					}
					if(ks.d && souto->x < 2000){
						if(souto->speed_x < max_speed) souto->speed_x++;
						souto->x+=souto->speed_x;
					}
					if(ks.a && souto->x > 0){
						if(souto->speed_x > -max_speed) souto->speed_x--;
						souto->x+=souto->speed_x;
					}
					if(!ks.d && souto->speed_x > 0){
						souto->speed_x--;
						souto->x+=souto->speed_x;
					}
					if(!ks.a && souto->speed_x < 0){
						souto->speed_x++;
						souto->x+=souto->speed_x;
					}
					
					if(souto->x < 0) souto->x = 0;
					else if(souto->x > 2000) souto->x = 2000;
					if(souto->y < 0) souto->y = 0;
					else if(souto->y > 2000) souto->y = 2000;
					
					drawBackground();

					if(handleEnemies()==1){
						currDate = get_date();
						strcpy(player, "AAA");
						gameStatus = save;
					}
					
					//printf("before souto\n");
					if(printSouto == true){
						drawNewXPM(soutoxpm, (getHRes()/2)-50, (getVRes()/2)-33);
					}
					else vg_draw_rectangle_double((getHRes()/2)-50, (getVRes()/2)-33, 100, 66, 0x00FF00);
					//printf("after souto\n");
					
					if(printMouse){
						drawNewXPM(mousexpm, x-12, y-12);
					}
					else vg_draw_rectangle_double(x-12, y-12, 25, 25, 0xFFFFFF);
					
					if(lb == true){
						int mouseX = souto->x - getHRes()/2 + x;
						int mouseY = souto->y - getVRes()/2 + y;
						if(mouseX > 0 && mouseX < 2000 && mouseY > 0 && mouseY < 2000){
							int zomb = colision(mouseX, mouseY);
							if(zomb > 0){
								souto->score++;
								killZombie((unsigned int) zomb);
							}
						}
					}
					
					drawBoost();
					
					printPoints();
					
					swap();
					//if(((cont*fr_rate) / 3600) == 5) running = false;
					
					if(cont*fr_rate % 3600 == 0){
						spawnZombie();
						cont = 0;
					}
				}
			}
		}
        break;
      default:
        break;
      }
    }
  }  
  
  vg_exit();
  
  printf("%x\n",vm);

UNSUB_LABEL:  
  if(rtc_unsubscribe_int()!=0)
	printf("Failed to unsubscribe the rtc!\n");


  if (kbd_unsubscribe_int() != 0)
	printf("Failed to unsubscribe the keyboard!\n");
    
  if(timer_unsubscribe_int() != 0)
	printf("Failed to unsubscribe the timer!\n");
	
  if (ps2_unsubscribe_int() != 0)
	printf("Failed to unsubscribe the mouse!\n");
	
  if(ps2_ignore() != 0)
	printf("mouse_test_packet(53): Failed ignoring ps2 buffer!\n");
  
  disable_rtc_int();

  return 1;
}
