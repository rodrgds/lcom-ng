#include <lcom/lcf.h>
#include "jogo.h"

Game *createGame(){
  Game *mygame=malloc(sizeof(Game));
  mygame->background[0].x=0;
  mygame->background[0].y=0;
  mygame->score=0;
  mygame->background[0].speed=GAME_SPEED;
  mygame->coins_size=0;
  mygame->enemies_size=0;
  mygame->stage=0;
  mygame->count_secs=0;
  xpm_load(fundo_xpm,XPM_5_6_5,&mygame->background[0].sprite);
  read_highscores(mygame->highscores);
  mygame->background[1].speed=0;
  mygame->background[1].x=0;
  mygame->background[1].y=0;
  xpm_load(menu_inicial_xpm, XPM_5_6_5, &mygame->background[1].sprite);
  mygame->background[2].speed=0;
  mygame->background[2].x=0;
  mygame->background[2].y=0;
  xpm_load(menu_inicial_start_game_xpm, XPM_5_6_5, &mygame->background[2].sprite);
  mygame->background[3].speed=0;
  mygame->background[3].x=0;
  mygame->background[3].y=0;
  xpm_load(menu_inicial_instructions_xpm, XPM_5_6_5, &mygame->background[3].sprite);
  mygame->background[4].speed=0;
  mygame->background[4].x=0;
  mygame->background[4].y=0;
  xpm_load(menu_inicial_highscores_xpm, XPM_5_6_5, &mygame->background[4].sprite);
  mygame->background[5].speed=0;
  mygame->background[5].x=0;
  mygame->background[5].y=0;
  xpm_load(instructions_xpm, XPM_5_6_5, &mygame->background[5].sprite);
  mygame->background[6].speed=0;
  mygame->background[6].x=0;
  mygame->background[6].y=0;
  xpm_load(highscores_xpm, XPM_5_6_5, &mygame->background[6].sprite);
  mygame->background[7].speed=0;
  mygame->background[7].x=0;
  mygame->background[7].y=0;
  xpm_load(pausa_xpm, XPM_5_6_5, &mygame->background[7].sprite);
  mygame->background[8].speed=0;
  mygame->background[8].x=0;
  mygame->background[8].y=0;
  xpm_load(highscores_back_xpm, XPM_5_6_5, &mygame->background[8].sprite);
  mygame->background[9].speed=0;
  mygame->background[9].x=0;
  mygame->background[9].y=0;
  xpm_load(instructions_back_xpm, XPM_5_6_5, &mygame->background[9].sprite);
  mygame->background[10].speed=0;
  mygame->background[10].x=0;
  mygame->background[10].y=0;
  xpm_load(game_over_xpm, XPM_5_6_5, &mygame->background[10].sprite);
  mygame->background[11].speed=0;
  mygame->background[11].x=0;
  mygame->background[11].y=0;
  xpm_load(game_over_back_xpm, XPM_5_6_5, &mygame->background[11].sprite);
  mygame->background[12].speed=0;
  mygame->background[12].x=0;
  mygame->background[12].y=0;
  xpm_load(game_over_exit_xpm, XPM_5_6_5, &mygame->background[12].sprite);
  mygame->currentState = INITIAL_MENU;
  return mygame;
}

Tux* createTux()  {
    Tux *mytux=malloc(sizeof(Tux));
    mytux->currentsprite = 0;
    xpm_load(tux_stand_xpm,XPM_5_6_5,&mytux->sprites[0]);
    xpm_load(tux_walk_0_xpm,XPM_5_6_5,&mytux->sprites[1]);
    xpm_load(tux_walk_3_xpm,XPM_5_6_5,&mytux->sprites[2]);
    xpm_load(tux_walk_4_xpm,XPM_5_6_5,&mytux->sprites[3]);
    xpm_load(tux_jump_xpm,XPM_5_6_5,&mytux->sprites[4]);
    xpm_load(tux_duck_xpm,XPM_5_6_5,&mytux->sprites[5]);
    xpm_load(tux_standb_xpm,XPM_5_6_5,&mytux->sprites[6]);
    xpm_load(tux_walk_0b_xpm,XPM_5_6_5,&mytux->sprites[7]);
    xpm_load(tux_walk_3b_xpm,XPM_5_6_5,&mytux->sprites[8]);
    xpm_load(tux_walk_4b_xpm,XPM_5_6_5,&mytux->sprites[9]);
    xpm_load(tux_jumpb_xpm,XPM_5_6_5,&mytux->sprites[10]);
    xpm_load(tux_duckb_xpm,XPM_5_6_5,&mytux->sprites[11]);
    mytux->currentmove=STANDING_R;
    mytux->speed=JUMP_XMOV;
    mytux->xpos = 0;
    mytux->ypos = 0;
    mytux->fireballs=0;
    mytux->dead=false;
    return mytux;
  }

Cursor* createCursor(){
  Cursor *cursor = malloc(sizeof(Cursor));
  xpm_load(mousecursor_xpm, XPM_5_6_5, &cursor->m[0]); //Tux com a boca fechada
  xpm_load(mousecursor_click_xpm, XPM_5_6_5, &cursor->m[1]); //Tux com a boca aberta
  return cursor;
}

int jogo(){
  uint32_t timer_irq_set, kbd_irq_set, mouse_irq_set;
  if(subscribes_interrupts(&timer_irq_set, &kbd_irq_set, &mouse_irq_set)) return 1;

  Tux* mytux =createTux();
  Game* mygame=createGame();
  Numeros numeros=createNums();
  Cursor* cursor = createCursor();
  Letras *letras=createLetras();

  Date * d=get_date();
  printf("h: %u, m:%u",d->hour,d->minute);

  bool status_error_mouse=false;
  uint32_t npackets=0;
  int next_byte=0;
  struct packet mouse_packet;
  struct mouse_ev *mouse_event;
  
  int ipc_status=0;
  message msg;
  int r=0;
  
  bool make=false;
  uint8_t size=0;

  cursor->xpos=2;
  cursor->ypos=2;
  
  while(scancode!=0x92 && mygame->currentState != EXIT){ //secret key to exit is 'E'
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.m_notify.interrupts & kbd_irq_set) { /* subscribed interrupt */
					  kbc_ih();
            if(status_error)continue;
            scancode_bytes[size]=scancode;
            size++;
          
            //check if the scancode has only one byte
            if(scancode!=SECOND_BYTE_CODE){
            make=!(scancode & BREAK_CODE);
            scancode_handler(mygame,mytux);

            size=0;
						scancode_bytes[0]=0;
						scancode_bytes[1]=0;
            }
				}
        if(msg.m_notify.interrupts & timer_irq_set){
          timer_int_handler();
          if(counter%5==0){
            handle_tux_movs(mytux);
          }
          checkGameState(mygame, cursor, mytux, mouse_event);
          if (mygame->currentState == GAME_RUNNING){
            game_run(mytux,mygame,letras,&numeros);    
          }     
          if (mygame->currentState != GAME_RUNNING && mygame->currentState != GAME_PAUSED){
            if(mouse_packet.lb){
              draw_sprite(cursor->xpos, cursor->ypos, &cursor->m[1]);
            }
            if(!mouse_packet.lb){
              draw_sprite(cursor->xpos, cursor->ypos, &cursor->m[0]);
            }
          }
          refresh_graphics();
        }
        if (msg.m_notify.interrupts & mouse_irq_set) { /* subscribed interrupt */
					  mouse_ih();

            if(status_error_mouse)continue;

            if(next_byte==0){
              mouse_packet.bytes[0]=byte_mouse;
              next_byte=1;
            }
            else if (next_byte==1){
              mouse_packet.bytes[next_byte]=byte_mouse;
              next_byte=2;
            }
            else if(next_byte==2){
              bool sign_y=mouse_packet.bytes[0]& Y_SIGN;
              bool sign_x=mouse_packet.bytes[0]& X_SIGN;
              mouse_packet.bytes[next_byte]=byte_mouse;
              next_byte=0;
              npackets+=1;
              mouse_packet.rb=mouse_packet.bytes[0]& PRESSED_RB;
              mouse_packet.mb=mouse_packet.bytes[0]& PRESSED_MB;
              mouse_packet.lb=mouse_packet.bytes[0]& PRESSED_LB;
              mouse_packet.delta_x=convert(mouse_packet.bytes[1],sign_x);
              mouse_packet.delta_y=convert(mouse_packet.bytes[2],sign_y);
              mouse_packet.x_ov=mouse_packet.bytes[0]& X_OVF;
              mouse_packet.y_ov=mouse_packet.bytes[0]& Y_OVF;
              mouse_event = mouse_detect_ev(&mouse_packet);
              mouse_movement(mygame, cursor, &mouse_packet, sign_x, sign_y);
            }          
				}
				break;

			default:
				break;  /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}

	}
  unsubscribes_interrupts();
  free(mygame);
  free(mytux);
  free(letras);
  free(cursor);
  return 0;

}


