#ifndef _GAME_H
#define _GAME_H

#include "vga.h"
#include "img/alphabet.xpm"
#include "img/buttons/320x200.xpm"
#include "img/buttons/640x480.xpm"
#include "img/buttons/800x600.xpm"
#include "img/buttons/1024x768.xpm"
#include "img/buttons/1280x1024.xpm"
#include "img/buttons/back.xpm"
#include "img/buttons/hisco.xpm"
#include "img/buttons/options.xpm"
#include "img/buttons/quit.xpm"
#include "img/buttons/start.xpm"

typedef struct _keystatus {
    bool q;
    bool w;
    bool e; 
    bool r;
    bool t;
    bool y;
    bool u;
    bool i;
    bool o;
    bool p;
    bool a;
    bool s; 
    bool d;
    bool f;
    bool g;
    bool h;
    bool j;
    bool k;
    bool z;
    bool x;
    bool c;
    bool v;
    bool b;
    bool n;
    bool m;
    bool shift;
    bool caps;
    bool tab;
    bool esc;
    bool numb1;
    bool numb2;
    bool numb3;
    bool numb4;
    bool numb5;
    bool numb6;
    bool numb7;
    bool numb8;
    bool numb9;
    bool numb0;
    bool up;
    bool down;
    bool left;
    bool right;
    bool enter;
} keystatus;

typedef struct alpha_sprites {
	xpm_image_t letters[26];
	xpm_image_t numbers[10];
	xpm_image_t dashxpm;
	xpm_image_t slashxpm;
	xpm_image_t	dotxpm;
	xpm_image_t colonxpm;
} alpha_sprites;

typedef struct hscore{
	unsigned int points;
	char player[4];
	char date[20];
	bool active;
}hscore;

typedef struct _pu{
	int x, y;
	bool active;
} pu;

typedef enum _powerup{
	none, fast, slow, invincible
} powerup;

typedef enum game_state{
	menu, options, game, quit, hiscore, save
} game_state;

typedef struct _sprite{
	unsigned int width, height;
	char *map;
} sprite;

typedef struct _zombie{
	int x, y, speed_x, speed_y;
	unsigned int id;
	bool alive;
	sprite *img;
	struct _zombie* prev;
	struct _zombie* next;
} zombie;

typedef struct _enemies{
	unsigned int size;
	zombie* first;
	zombie* last;
} enemies;

typedef struct _hero{
	int x, y, speed_x, speed_y, score;
	sprite img;
	powerup status;
}hero;

typedef struct buttons{
	xpm_image_t size_1_xpm, size_2_xpm, size_3_xpm, size_4_xpm, size_5_xpm, backxpm, optionsxpm, quitxpm, hiscoxpm, startxpm;
}buttons;

enemies *alunos;
hero *souto;
keystatus ks;
pu boost;
alpha_sprites alphabet;
buttons buttonsXpm;
hscore hScores[5];

game_state gameStatus;

bool printAlphabet = false, printButtons = false;

int loadAlphanum(){
	if (xpm_load(_0_xpm, XPM_8_8_8, &(alphabet.numbers[0])) == NULL){
		return 1;
	}
	if (xpm_load(_1_xpm, XPM_8_8_8, &(alphabet.numbers[1])) == NULL){
		return 1;
	}
	if (xpm_load(_2_xpm, XPM_8_8_8, &(alphabet.numbers[2])) == NULL){
		return 1;
	}
	if (xpm_load(_3_xpm, XPM_8_8_8, &(alphabet.numbers[3])) == NULL){
		return 1;
	}
	if (xpm_load(_4_xpm, XPM_8_8_8, &(alphabet.numbers[4])) == NULL){
		return 1;
	}
	if (xpm_load(_5_xpm, XPM_8_8_8, &(alphabet.numbers[5])) == NULL){
		return 1;
	}
	if (xpm_load(_6_xpm, XPM_8_8_8, &(alphabet.numbers[6])) == NULL){
		return 1;
	}
	if (xpm_load(_7_xpm, XPM_8_8_8, &(alphabet.numbers[7])) == NULL){
		return 1;
	}
	if (xpm_load(_8_xpm, XPM_8_8_8, &(alphabet.numbers[8])) == NULL){
		return 1;
	}
	if (xpm_load(_9_xpm, XPM_8_8_8, &(alphabet.numbers[9])) == NULL){
		return 1;
	}
	if (xpm_load(a_xpm, XPM_8_8_8, &(alphabet.letters[0])) == NULL){
		return 1;
	}
	if (xpm_load(b_xpm, XPM_8_8_8, &(alphabet.letters[1])) == NULL){
		return 1;
	}
	if (xpm_load(c_xpm, XPM_8_8_8, &(alphabet.letters[2])) == NULL){
		return 1;
	}
	if (xpm_load(d_xpm, XPM_8_8_8, &(alphabet.letters[3])) == NULL){
		return 1;
	}
	if (xpm_load(e_xpm, XPM_8_8_8, &(alphabet.letters[4])) == NULL){
		return 1;
	}
	if (xpm_load(f_xpm, XPM_8_8_8, &(alphabet.letters[5])) == NULL){
		return 1;
	}
	if (xpm_load(g_xpm, XPM_8_8_8, &(alphabet.letters[6])) == NULL){
		return 1;
	}
	if (xpm_load(h_xpm, XPM_8_8_8, &(alphabet.letters[7])) == NULL){
		return 1;
	}
	if (xpm_load(i_xpm, XPM_8_8_8, &(alphabet.letters[8])) == NULL){
		return 1;
	}
	if (xpm_load(j_xpm, XPM_8_8_8, &(alphabet.letters[9])) == NULL){
		return 1;
	}
	if (xpm_load(k_xpm, XPM_8_8_8, &(alphabet.letters[10])) == NULL){
		return 1;
	}
	if (xpm_load(l_xpm, XPM_8_8_8, &(alphabet.letters[11])) == NULL){
		return 1;
	}
	if (xpm_load(m_xpm, XPM_8_8_8, &(alphabet.letters[12])) == NULL){
		return 1;
	}
	if (xpm_load(n_xpm, XPM_8_8_8, &(alphabet.letters[13])) == NULL){
		return 1;
	}
	if (xpm_load(o_xpm, XPM_8_8_8, &(alphabet.letters[14])) == NULL){
		return 1;
	}
	if (xpm_load(p_xpm, XPM_8_8_8, &(alphabet.letters[15])) == NULL){
		return 1;
	}
	if (xpm_load(q_xpm, XPM_8_8_8, &(alphabet.letters[16])) == NULL){
		return 1;
	}
	if (xpm_load(r_xpm, XPM_8_8_8, &(alphabet.letters[17])) == NULL){
		return 1;
	}
	if (xpm_load(s_xpm, XPM_8_8_8, &(alphabet.letters[18])) == NULL){
		return 1;
	}
	if (xpm_load(t_xpm, XPM_8_8_8, &(alphabet.letters[19])) == NULL){
		return 1;
	}
	if (xpm_load(u_xpm, XPM_8_8_8, &(alphabet.letters[20])) == NULL){
		return 1;
	}
	if (xpm_load(v_xpm, XPM_8_8_8, &(alphabet.letters[21])) == NULL){
		return 1;
	}
	if (xpm_load(w_xpm, XPM_8_8_8, &(alphabet.letters[22])) == NULL){
		return 1;
	}
	if (xpm_load(x_xpm, XPM_8_8_8, &(alphabet.letters[23])) == NULL){
		return 1;
	}
	if (xpm_load(y_xpm, XPM_8_8_8, &(alphabet.letters[24])) == NULL){
		return 1;
	}
	if (xpm_load(z_xpm, XPM_8_8_8, &(alphabet.letters[25])) == NULL){
		return 1;
	}
	if (xpm_load(dash_xpm, XPM_8_8_8, &(alphabet.dashxpm)) == NULL){
		return 1;
	}
	if (xpm_load(slash_xpm, XPM_8_8_8, &(alphabet.slashxpm)) == NULL){
		return 1;
	}
	if (xpm_load(dot_xpm, XPM_8_8_8, &(alphabet.dotxpm)) == NULL){
		return 1;
	}
	if (xpm_load(colon_xpm, XPM_8_8_8, &(alphabet.colonxpm)) == NULL){
		return 1;
	}
	return 0;
}

int loadButtons(){
	if (xpm_load(_320x200_xpm, XPM_8_8_8, &(buttonsXpm.size_1_xpm)) == NULL){
		return 1;
	}
	if (xpm_load(_640x480_xpm, XPM_8_8_8, &(buttonsXpm.size_2_xpm)) == NULL){
		return 1;
	}
	if (xpm_load(_800x600_xpm, XPM_8_8_8, &(buttonsXpm.size_3_xpm)) == NULL){
		return 1;
	}
	if (xpm_load(_1024x768_xpm, XPM_8_8_8, &(buttonsXpm.size_4_xpm)) == NULL){
		return 1;
	}
	if (xpm_load(_1280x1024_xpm, XPM_8_8_8, &(buttonsXpm.size_5_xpm)) == NULL){
		return 1;
	}
	if (xpm_load(back_xpm, XPM_8_8_8, &(buttonsXpm.backxpm)) == NULL){
		return 1;
	}
	if (xpm_load(start_xpm, XPM_8_8_8, &(buttonsXpm.startxpm)) == NULL){
		return 1;
	}
	if (xpm_load(hisco_xpm, XPM_8_8_8, &(buttonsXpm.hiscoxpm)) == NULL){
		return 1;
	}
	if (xpm_load(options_xpm, XPM_8_8_8, &(buttonsXpm.optionsxpm)) == NULL){
		return 1;
	}
	if (xpm_load(quit_xpm, XPM_8_8_8, &(buttonsXpm.quitxpm)) == NULL){
		return 1;
	}
	return 0;
}

int killZombie(unsigned int id){
	//printf("%d\n", id);
	if(alunos->last->id == id && alunos->last->alive == true){
		//printf("last %d\n", id);
		alunos->last->alive = false;
		return 0;
	}
	zombie * curr;
	for(curr = alunos->first; curr != alunos->last; curr = curr->next){
		//printf("curr: %d, alive: %d, next: 0x%X\n", curr->id, curr->alive, curr->next);
		if(curr->alive != true) return 1;
		//printf("It's alive!\n");
		if(curr->id == id){
			//printf("Same id, prev: 0x%X, next: 0x%X\n", curr->prev, curr->next);
			curr->speed_x = 0;
			curr->speed_y = 0;
			curr->x = 3000;
			curr->y = 3000;
			if(curr->prev != NULL){
				curr->prev->next = curr->next;
				curr->prev = alunos->last;
			}
			curr->next->prev = curr->prev;
			curr->alive = false;
			curr->next = NULL;
			alunos->last->next = curr;
			alunos->last = curr;
			return 0;
		}
	}
	//printf("No such zombie!\n");
	return 1;
}

int gameInit(){
	gameStatus = menu;
	
	souto = (hero*)malloc(sizeof(hero));
	alunos = (enemies*)malloc(sizeof(enemies));
	alunos->first = (zombie*)malloc(sizeof(zombie));
	alunos->size = 100;
	
	zombie *aux = alunos->first;
	
	sprite *zombies = (sprite *)malloc(sizeof(sprite));
	sprite heroes;
	
	boost.active = true;
	
	boost.x = rand() % (2000-getHRes());
	boost.y = rand() % (2000-getVRes());
	
	/*heroes.map = read_xpm(souto_xpm, &(heroes.width), &(heroes.height));
	
	zombies.map = read_xpm(zombie_xpm, &(zombies.width), &(zombies.height));*/
	
	souto->img = heroes;
	souto->score = 0;
	souto->speed_x = 0;
	souto->speed_y = 0;
	souto->status = none;
	souto->x = 0;
	souto->y = 0;
	
	aux->id = 0;
	aux->alive = false;
	aux->img = zombies;
	aux->prev = NULL;
	aux->speed_x=0;
	aux->speed_y=0;
	aux->x=0;
	aux->y=0;
	
	for(unsigned int i=1; i < alunos->size; i++){
		zombie *newZ = (zombie*)malloc(sizeof(zombie));
		newZ->prev = aux;
		aux->next = newZ;
		aux = newZ;
		aux->id = i;
		aux->alive = false;
		aux->img = zombies;
		aux->speed_x=0;
		aux->speed_y=0;
		aux->x=0;
		aux->y=0;
	}
	
	alunos->last = aux;
	
	return 0;
}

float mod(float a){
	if(a < 0) return 0-a;
	else return a;
}

int colision(int x, int y){
	if(mod(souto->x - x) < 50 && mod(souto->y - y) < 33){
		return -1;
	}
	zombie *curr = alunos->first;
	for(unsigned int i=0; i < alunos->size; i++){
		if(curr->alive == false) break ;
		else{
			if(mod(curr->x - x) < 100 && mod(curr->y - y) < 92){
				return curr->id;
			}
		}
		curr = curr->next;
	}
	return 0;
}

int spawnZombie(){
	/*static int couve=0;
	printf("Spawning zombie! %d\n", couve++);*/
	if(alunos->last->alive == false){
		zombie *aux = alunos->last;
		aux->alive = true;
		alunos->last = aux->prev;
		alunos->last->next = NULL;
		aux->prev = NULL;
		aux->next = alunos->first;
		alunos->first->prev = aux;
		alunos->first = aux;
		aux->x = rand() % (2000-getHRes());
		aux->y = rand() % (2000-getVRes());
		
		int difX = mod(aux->x - souto->x);
		int difY = mod(aux->y - souto->y);
		
		if(difX < getHRes()/2 && difY < getVRes()/2){
			aux->x += getHRes();
			aux->y += getVRes();
		}
		
		while(/*colision(aux->x, aux->y) != 0 || */colision(aux->x, aux->y) != (int)aux->id){
			aux->x = rand() % (2000-getHRes());
			aux->y = rand() % (2000-getVRes());
			
			difX = mod(aux->x - souto->x);
			difY = mod(aux->y - souto->y);
			
			if(difX < getHRes()/2 && difY < getVRes()/2){
				aux->x += getHRes();
				aux->y += getVRes();
			}
		}
		return 0;
	}
	
	return 1;
}

void makeBoost(){
	boost.active = true;
	
	boost.x = rand() % (2000-getHRes());
	boost.y = rand() % (2000-getVRes());
}

void destroyBoost(){
	boost.active = false;
	
	boost.x = -2000;
	boost.y = -2000;
}

bool onBoost(int x, int y){
	if(!(boost.active)) return false;
	if(x > boost.x-20 && x < boost.x+20 && y > boost.y-20 && y < boost.y+20) return true;
	return false;
}

#endif
