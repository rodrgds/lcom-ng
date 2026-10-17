#ifndef __INPUT_H_
#define __INPUT_H_

#include "KBC_mouse.h"
#include "KBC.h"

/*associaçao das teclas aos codigos*/
static char letras1[10] = {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p'};
static char letras2[10] = {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l'};
static char letras3[10] = {'z', 'x', 'c', 'v', 'b', 'n', 'm'};

#define key_q 0x10
#define key_w 0x11
#define key_e 0x12
#define key_r 0x13
#define key_t 0x14
#define key_y 0x15
#define key_u 0x16
#define key_i 0x17
#define key_o 0x18
#define key_p 0x19

#define key_a 0x1e
#define key_s 0x1f
#define key_d 0x20
#define key_f 0x21
#define key_g 0x22
#define key_h 0x23
#define key_j 0x24
#define key_k 0x25
#define key_l 0x26


#define key_z 0x2c
#define key_x 0x2d
#define key_c 0x2e
#define key_v 0x2f
#define key_b 0x30
#define key_n 0x31
#define key_m 0x32

#define key_back 0xe
#define key_enter 0x1c
#define key_space 0x39

#define key_1 0x2
#define key_2 0x3
#define key_3 0x4
#define key_4 0x5
#define key_5 0x6
#define key_6 0x7
#define key_7 0x8
#define key_8 0x9
#define key_9 0xa
#define key_0 0xb

#define key_Arrow_ID 0xe0
#define key_upArrow 0x48
#define key_downArrow 0x50
#define key_leftArrow 0x4b
#define key_rightArrow 0x4d


typedef struct {

	int mouse_x, mouse_y;
	int mouse_LB, mouse_RB;
	int mouse_LB_P, mouse_RB_P, mouse_LB_R, mouse_RB_R;
	int UP, DOWN, LEFT, RIGHT, SPACE, ENTER; //being pressed
	int UP_P, DOWN_P, LEFT_P, RIGHT_P, SPACE_P, ENTER_P; //pressed
	int UP_R, DOWN_R, LEFT_R, RIGHT_R, SPACE_R, ENTER_R; //released

	int pos_text;
	char text[17]; //16 chars + /0
} Input ;

static Input input_buffer; //as alteraçoes do input sao recebidas aqui para nao ser alterado enquanto se está a usar
static Input input;

char input_text_convert(unsigned char in);
void input_handle_mouse(mousePacket packet);
void input_handle_kbd(unsigned char in);
void input_text_clear();

void input_swap_buffers(); //copia o buffer para o input
Input get_Input();
void init_Input();
#endif
