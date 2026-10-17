#include <stdio.h>
#include "input.h"
#include "util.h"

static int wait_for_arrow = 0;

extern int WIDTH,HEIGHT;

/*!
 * Inicializa a estrutura de input e respetivo buffer
 */
void init_Input() {
	Input i;
	memset(&i,0,sizeof(Input));
	i.mouse_x = WIDTH/2;
	i.mouse_y = HEIGHT/2;

	input = i;
	input_buffer = i;
};

/*!
 * Converte um codigo do teclado para um caracter ASCII.
 * Apenas suporta codigos das letras e o espaco.
 *\param in codigo proveniente do teclado
 *\return devolve um caracter ascii ou 0 caso seja um codigo nao suportado.
 */
char input_text_convert(unsigned char in) {
	char res = 0;
	if (in >= 0x10 && in <= 0x19){
		res = letras1[in - 0x10] - 32;
	}
	else if (in >= 0x1e && in <= 0x26){
		res = letras2[in - 0x1e] - 32;
	}
	else if (in >= 0x2c && in <= 0x32){
		res = letras3[in - 0x2c] - 32;
	}
	else if (in == key_space){
		res = ' ';
	}
	return res;
}

/*!
 * Trata um packet do rato proveniente do handler de interrupts do rato.
 * \param packet packet do rato
 */
void input_handle_mouse(mousePacket packet){
		input_buffer.mouse_x += packet.xdelta;
		input_buffer.mouse_x = max(min(input_buffer.mouse_x,WIDTH-32),0);
		input_buffer.mouse_y -= packet.ydelta;
		input_buffer.mouse_y = max(min(input_buffer.mouse_y,HEIGHT-32),0);
		input_buffer.mouse_LB = packet.lbutton;
		input_buffer.mouse_RB = packet.rbutton;
	};

/*!
 * Trata um codigo proveniente do handler de interrupts do teclado.
 * \param in codigo de um evento do teclado
 */
void input_handle_kbd(unsigned char in){

		if(wait_for_arrow == 1){
			switch(in & 0x7f){
			case key_upArrow: {if (in & 0x80) input_buffer.UP = 0; else input_buffer.UP = 1; break;}
			case key_downArrow:  {if (in & 0x80) input_buffer.DOWN = 0; else input_buffer.DOWN = 1; break;}
			case key_leftArrow:  {if (in & 0x80) input_buffer.LEFT = 0; else input_buffer.LEFT = 1; break;}
			case key_rightArrow:  {if (in & 0x80) input_buffer.RIGHT = 0; else input_buffer.RIGHT = 1; break;}
			}
			wait_for_arrow = 0;
		}
		else if (in == key_Arrow_ID)
				wait_for_arrow = 1;
		else
		{
			if ((in & 0x7f) == key_space) {
				if (in & 0x80)
					input_buffer.SPACE = 0;
				else
					input_buffer.SPACE = 1;
			}
			if ((in & 0x7f) == key_enter) {
				if (in & 0x80)
					input_buffer.ENTER = 0;
				else
					input_buffer.ENTER = 1;
			}
			if(!(in & 0x80)){
				if (in == key_back) {
					input_buffer.pos_text = max(input_buffer.pos_text-1,0);
					input_buffer.text[input_buffer.pos_text] = 0;
				}
				else if (input_buffer.pos_text < 16){
					//printf("%.2x\n",in & 0xFF);
					char c = input_text_convert(in);
					if (c != 0 || !(c = ' ' && input_buffer.pos_text == 0) ) {
						input_buffer.text[input_buffer.pos_text] = c;
						input_buffer.pos_text++;
					}
				}
			}
			wait_for_arrow = 0;
		}
}; //kbd

/*!
 * Limpa o buffer de texto do input
 */
void input_text_clear(){
	memset(input_buffer.text,0,17);
	input_buffer.pos_text = 0;
}


void input_swap_buffers_aux(int new, int old, int* pressed,int* released){
	if(new && !old) *pressed = 1; else *pressed = 0;
	if (!new && old) *released = 1; else *released = 0;
}

/*!
 * Copia o buffer de input para o input principal para ser usado pelo jogo.
 * Esta funcao tambem mantem os membros com terminacao _P e _R que representam impulsos unitarios para uma tecla premida ou largada
 */
void input_swap_buffers() {
	input_swap_buffers_aux(input_buffer.SPACE, input.SPACE, &input_buffer.SPACE_P, &input_buffer.SPACE_R);
	input_swap_buffers_aux(input_buffer.ENTER, input.ENTER, &input_buffer.ENTER_P, &input_buffer.ENTER_R);
	input_swap_buffers_aux(input_buffer.UP, input.UP, &input_buffer.UP_P, &input_buffer.UP_R);
	input_swap_buffers_aux(input_buffer.DOWN, input.DOWN, &input_buffer.DOWN_P, &input_buffer.DOWN_R);
	input_swap_buffers_aux(input_buffer.LEFT, input.LEFT, &input_buffer.LEFT_P, &input_buffer.LEFT_R);
	input_swap_buffers_aux(input_buffer.RIGHT, input.RIGHT, &input_buffer.RIGHT_P, &input_buffer.RIGHT_R);
	input_swap_buffers_aux(input_buffer.mouse_RB, input.mouse_RB, &input_buffer.mouse_RB_P, &input_buffer.mouse_RB_R);
	input_swap_buffers_aux(input_buffer.mouse_LB, input.mouse_LB, &input_buffer.mouse_LB_P, &input_buffer.mouse_LB_R);
	input = input_buffer;
};

/*!
 * Acesso ao input principal para ser usado pelo jogo.
 */
Input get_Input() {return input;};



