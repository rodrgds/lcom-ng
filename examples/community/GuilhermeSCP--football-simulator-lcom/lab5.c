#include <minix/syslib.h>
#include <minix/drivers.h>
#include "test5.h"
#include "video_txt.h"
#include "vt_info.h"

struct Rato {
int x,y,rb,lb;
x=20;
y=5;
rb=0;
lb=0;
}

void set_pos_rato(Rato r, int newx, int newy){
r.x= (x + newx)/scr_width; /*define o x*/
r.y= (y - newy)/scr_lines; /*define o y*/
if(r.x<0) 
	{r.x=0;} /*limita o x ao ecra*/
if(r.x>80/* PREENCHER valor maximo de casinhas x*/)
	{r.x=80;} /*limita o x ao ecra*/
if(r.y<0)
	{r.y=0;} /*limita o y ao ecra*/
if(r.y>25/* PREENCHER valor maximo de casinhas y*/)
	{r.y=25;} /*limita o y ao ecra*/
}

void desenhar_rato(Rato r,x,y){

vt_print_char(0xDB,y,x,0x08);/*verificar se isto funciona com os hex e etc*/

}

/* falta definir cenas em relacao as casinhas e assim*/

int main() {

rato r;
short x,y;
int i=0;

r.x=20;
r.y=5;
r.lb=0;
r.rb=0;

while(1) { 	/*while(de hoje a 8 == pra semana) */
		if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			printf("driver_receive failed");
			continue;
		}
		/* se houver falha */
		
		if (is_ipc_notify(ipc_status)) { 		/* recebe notificacao */
			if (_ENDPOINT_P(msg.m_source) == HARDWARE) {
				/* notificacao do interrupt do hardware */
				if (msg.NOTIFY_ARG & irq_mouse) /* subscreve o interrupt */
				{
					packet[i] = mouse_read();
					if(i == 2)
					{
						x = ((packet[0] & BIT(4))?0xff00:0);/* qq cena a ver com o sinal dos valores*/
						y = ((packet[0] & BIT(5))?0xff00:0);/* "" """...*/
						x = x | packet[1]; /*define o novo x do rato*/
						y = y | packet[2]; /*define o novo y do rato*/
						r.rb=(packet[0] & BIT(1))>>1; /*recebe a informacao do botao direito*/
						r.lb=(packet[0] & BIT(0)); /*sempre 0*/
						set_pos_rato(r,x,y);
						i=0;

						if(/* PREENCHER condições dos butões*/ && r.rb)
						{
							/*entrar nos menus e acontecer cenas*/
						}
						
						desenhar_rato(r,r.x,r.y);
						
						kbc_clean_buf();
					}
					else i++;
					
				}
			}
			else break;
		}
	}
	
kbc_clean_buf();
mouse_unsubscribe_int();

return 0;
}
	