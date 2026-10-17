#include <lcom/lcf.h>
#include <lcom/proj.h>
#include <stdint.h>
#include <stdio.h>


// Any header files included below this line should have been created by you
#include "timer.h"

#include "mouse.c"
#include "keyboard.c"
#include "i8042.h"
#include "moves.c"
#include "utils.h"




//XPM
#include "xpm/menu.xpm"
#include "xpm/cursor_mouse.xpm"
#include "xpm/cao.xpm"
#include "xpm/cao_up.xpm"
#include "xpm/cao_down.xpm"
#include "xpm/fundo.xpm"
#include "xpm/alien.xpm"
#include "xpm/asteroides.xpm"
#include "xpm/fogo.xpm"
#include "xpm/instrucoes.xpm"
#include "xpm/vida.xpm"
#include "xpm/coin.xpm"
#include "xpm/gameover.xpm"
#include "xpm/chao.xpm"
#include "xpm/0.xpm"
#include "xpm/1.xpm"
#include "xpm/2.xpm"
#include "xpm/3.xpm"
#include "xpm/4.xpm"
#include "xpm/5.xpm"
#include "xpm/6.xpm"
#include "xpm/7.xpm"
#include "xpm/8.xpm"
#include "xpm/9.xpm"





//EXTERN
extern uint8_t data;
extern uint32_t counter;
extern int mouse_x;
extern int mouse_y;
extern int packet_byte_cnt;
extern uint8_t status;
extern uint8_t packet[3];
extern uint8_t arr[2];
extern enum game_state state;

uint32_t counter_packets=0;


int dog_init_pos=300;
int asteroide_idx=0;
int fogo_idx=0;
int et_idx=0;
int vidas_idx=3;
int coin_idx=0;
uint8_t* asteroides[51];
uint8_t* fogos[51];
uint8_t* ets[51];
uint8_t* vidas[3];
uint8_t* coins[101];
static void remove_object(uint8_t **sprites,int *xs,int *ys,int *count,int index){
  for(int i=index; i<*count; i++){
    sprites[i]=sprites[i+1];
    xs[i]=xs[i+1];
    ys[i]=ys[i+1];
  }
  sprites[*count]=NULL;
  (*count)--;
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

int (proj_main_loop)(int argc, char* argv[]){
  //IMAGE
    xpm_image_t menu_img;
    xpm_image_t cursor_img;
    xpm_image_t cao_img;
    xpm_image_t cao_up_img;
    xpm_image_t cao_down_img;
    xpm_image_t fundo_img;
    xpm_image_t fogo_img;
    xpm_image_t alien_img;
    xpm_image_t asteroides_img;
    xpm_image_t instrucoes_img;
    xpm_image_t vida_img;
    xpm_image_t coin_img;
    xpm_image_t gameover_img;
    xpm_image_t chao_img;
    xpm_image_t zero_img;
    xpm_image_t um_img;
    xpm_image_t dois_img;
    xpm_image_t tres_img;
    xpm_image_t quatro_img;
    xpm_image_t cinco_img;
    xpm_image_t seis_img;
    xpm_image_t sete_img;
    xpm_image_t oito_img;
    xpm_image_t nove_img;
    



  //XPM LOAD
    uint8_t* menu = (xpm_load)(menu_xpm, XPM_8_8_8, &menu_img);
    uint8_t* cursor = (xpm_load)(cursor_xpm, XPM_8_8_8, &cursor_img);
    uint8_t* cao = (xpm_load)(cao_xpm, XPM_8_8_8, &cao_img);
    uint8_t* cao_up = (xpm_load)(cao_up_xpm, XPM_8_8_8, &cao_up_img);
    uint8_t* cao_down = (xpm_load)(cao_down_xpm, XPM_8_8_8, &cao_down_img);
    uint8_t* fundo = (xpm_load)(fundo_xpm, XPM_8_8_8, &fundo_img);
    uint8_t* chao = (xpm_load)(chao_xpm, XPM_8_8_8, &chao_img);
    uint8_t* instrucoes = (xpm_load)(instrucoes_xpm, XPM_8_8_8, &instrucoes_img);
    uint8_t* gameover = (xpm_load)(gameover_xpm, XPM_8_8_8, &gameover_img);
    uint8_t* zero = (xpm_load)(zero_xpm, XPM_8_8_8, &zero_img);
    uint8_t* um = (xpm_load)(um_xpm, XPM_8_8_8, &um_img);
    uint8_t* dois = (xpm_load)(dois_xpm, XPM_8_8_8, &dois_img);
    uint8_t* tres = (xpm_load)(tres_xpm, XPM_8_8_8, &tres_img);
    uint8_t* quatro = (xpm_load)(quatro_xpm, XPM_8_8_8, &quatro_img);
    uint8_t* cinco = (xpm_load)(cinco_xpm, XPM_8_8_8, &cinco_img);
    uint8_t* seis = (xpm_load)(seis_xpm, XPM_8_8_8, &seis_img);
    uint8_t* sete = (xpm_load)(sete_xpm, XPM_8_8_8, &sete_img);
    uint8_t* oito = (xpm_load)(oito_xpm, XPM_8_8_8, &oito_img);
    uint8_t* nove = (xpm_load)(nove_xpm, XPM_8_8_8, &nove_img);





    enum game_state state = MENU;

    uint8_t bit_n, timer0_bit, mouse_bit;
    int score = 0;
    int r;
    int ipc_status;
    uint32_t kbc_irq_set, timer_irq_set,mouse_irq_set;
    message msg;
    struct packet pp;
    char score_str[5];
    int chao_cnt=0;
    asteroides_pos_arr();
    fogos_pos_arr();
    ets_pos_arr();
    coins_pos_arr();

    vg_init(0x115);

    if (kbd_subscribe_int(&bit_n) != 0)
      return 1;
    if (mouse_subscribe_int(&mouse_bit) != 0)
      return 1;
    if (timer_subscribe_int(&timer0_bit) != 0)
      return 1;

    kbc_irq_set = BIT(bit_n);
    timer_irq_set = BIT(timer0_bit);
    mouse_irq_set = BIT(mouse_bit);

    if(enable()!=0) return 1;
    

    while(state != EXIT) { /* You may want to use a different condition */
      /* Get a request message. */
      if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
        printf("driver_receive failed with: %d", r);
        continue;
      }

      if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: /* hardware interrupt notification */
            if (msg.m_notify.interrupts & kbc_irq_set) { /* subscribed interrupt */
              (kbc_ih)();
              get_data();
            }
            if (msg.m_notify.interrupts & mouse_irq_set) { /* subscribed interrupt */
              mouse_ih();
              get_mouse_status();
              if(packet_byte_cnt==3){
                if(isSyncronized()){
                  packet_assemble(&pp);
                  mouse_print_packet(&pp);
                  packet_byte_cnt=0;
                  mouse_pos(&pp, &state);
                }
              }
            }    

            if (msg.m_notify.interrupts & timer_irq_set) {
              timer_int_handler(); 
              
              switch(state){
              case MENU:     
                dog_init_pos=300;
                asteroide_idx=0;
                fogo_idx=0;
                et_idx=0;
                vidas_idx=3;
                coin_idx=0;  
                score = 0;
                asteroides_pos_arr();
                fogos_pos_arr();
                ets_pos_arr();
                coins_pos_arr();
                vg_draw_xpm_proj(menu_img, 0,0, menu);
                vg_draw_xpm_proj(cursor_img, mouse_x, mouse_y, cursor);
                change_buffer();
                break;
              
              case PLAY:
              chao_cnt-=50;
                vg_draw_xpm_proj(fundo_img, 0,0, fundo);
                int b=919;
                for (int i=0; i<100;i++) {
                    chao_cnt-=b;
                    vg_draw_xpm_proj(chao_img,chao_cnt,350, chao);
                }


                //VIDAS
                int vida_x=642;
                for(int i=0; i<vidas_idx;i++){
                    vidas[i]=(xpm_load)(vida_xpm, XPM_8_8_8, &vida_img);
                    vg_draw_xpm_proj(vida_img, vida_x,28, vidas[i]);
                    printf("vidas:%x\n", vidas_idx);
                    vida_x+=35;
                  }
                

                //asteroides
                if(counter%30==0){
                  while(asteroide_idx<50){
                    asteroide_idx++;
                    asteroide_x[asteroide_idx]=1100;
                    asteroide_y[asteroide_idx]=rand()%401+50;
                    asteroides[asteroide_idx]=(xpm_load)(asteroide_xpm, XPM_8_8_8, &asteroides_img);
                    break;
                  }
                }
                int n=1;
                while (n<asteroide_idx+1 && asteroide_idx>0){
                  vg_draw_xpm_proj(asteroides_img, asteroide_x[n], asteroide_y[n], asteroides[n]);
                  n++;
                }
                //fogos
                int f=1;
                while (f<fogo_idx+1 && fogo_idx>0){
                  vg_draw_xpm_proj(fogo_img, fogo_x[f], fogo_y[f], fogos[f]);
                  f++;
                }
                //ets
                if(counter%40==0){
                  while(et_idx<50){
                      et_idx++;
                    et_x[et_idx]=1100;
                    et_y[et_idx]=rand()%401+50;
                    ets[et_idx]=(xpm_load)(alien_xpm, XPM_8_8_8, &alien_img);
                      break;
                  }
                }
                int e=1;
                while (e<et_idx+1 && et_idx>0){
                  vg_draw_xpm_proj(alien_img, et_x[e], et_y[e], ets[e]);
                  e++;
                }
                //coins
                if(counter%10==0){
                  while(coin_idx<100){
                    coin_idx++;
                    coin_x[coin_idx]=1100;
                    coin_y[coin_idx]=rand()%401+90;
                    coins[coin_idx]=(xpm_load)(coin_xpm, XPM_8_8_8, &coin_img);
                    break;
                  }
                }
                int c=1;
                while (c<coin_idx+1 && coin_idx>0){
                  vg_draw_xpm_proj(coin_img, coin_x[c], coin_y[c], coins[c]);
                  c++;
                }
                

                if(arr[0]==0x11){
                  up(cao_up_img,cao_up);
                  for(int i=fogo_idx+1; i<=50; i++){
                      fogo_y[i]=dog_init_pos+40;
                  }
                  change_buffer();
                }
                else if(arr[0]==0x1F){
                  down(cao_down_img,cao_down);
                  for(int i=fogo_idx+1; i<=50; i++){
                      fogo_y[i]=dog_init_pos+40;
                  }
                  change_buffer();
                }
                else if(arr[0]==0x39){
                  vg_draw_xpm_proj(cao_img, 40, dog_init_pos, cao);
                  while(fogo_idx<50){
                    fogo_idx++;
                    fogo_x[fogo_idx]=50;
                    fogo_y[fogo_idx]=dog_init_pos+40;
                    fogos[fogo_idx]=(xpm_load)(fogo_xpm, XPM_8_8_8, &fogo_img);
                    break;
                  }
                }
                else{
                  vg_draw_xpm_proj(cao_img, 40, dog_init_pos, cao);
                }

                //cao e asteroides/coins
                for(int k=1; k<=asteroide_idx; k++){
                  if(((dog_init_pos>=asteroide_y[k] && dog_init_pos<=asteroide_y[k]+60) || (dog_init_pos+90>=asteroide_y[k] && dog_init_pos+90<=asteroide_y[k]+60) ||
                  (asteroide_y[k]>=dog_init_pos && asteroide_y[k]<=dog_init_pos+90) ||
                  (asteroide_y[k]+60>=dog_init_pos && asteroide_y[k]+60<=dog_init_pos+90)) && (160>=asteroide_x[k])){
                    
                    remove_object(asteroides,asteroide_x,asteroide_y,&asteroide_idx,k);
                    k--;
                    vidas_idx--;
                    printf("vidas ast:%x\n", vidas_idx);
                    
                  }
                }
                //cao e ets
                for(int k=1; k<=et_idx; k++){
                  printf("et_y: %x\n", et_y[k]);
                  printf("k: %x\n", k);
                  if(((dog_init_pos>=et_y[k] && dog_init_pos<=et_y[k]+83) || 
                  (dog_init_pos+90>=et_y[k] && dog_init_pos+90<=et_y[k]+83) ||
                  (et_y[k]>=dog_init_pos && et_y[k]<=dog_init_pos+90) ||
                  (et_y[k]+83>=dog_init_pos && et_y[k]+83<=dog_init_pos+90))&& 
                  (160>=et_x[k])){
                   
                    remove_object(ets,et_x,et_y,&et_idx,k);
                     vidas_idx--;
                     break;
                     printf("vidas et:%x\n", vidas_idx);
                  }
                }

                if(vidas_idx <= 0) state=GAMEOVER;

                //cao e coins
                for(int k=1; k<=coin_idx; k++){
                  if(((dog_init_pos>=coin_y[k] && dog_init_pos<=coin_y[k]+29) ||
                  (dog_init_pos+102>=coin_y[k] && dog_init_pos+102<=coin_y[k]+29) ||
                  (coin_y[k]>=dog_init_pos && coin_y[k]<=dog_init_pos+102) ||
                  (coin_y[k]+29>=dog_init_pos && coin_y[k]+20<=dog_init_pos+102)) && 
                  (160>=coin_x[k])){
                    remove_object(coins,coin_x,coin_y,&coin_idx,k);
                    k--;
                    score += 25;
                  }
                }
              
                for(int i=1; i<=fogo_idx; i++){
                  bool projectile_removed=false;
                  for(int k=1; k<=asteroide_idx; k++){
                    if(((fogo_x[i]>=asteroide_x[k] && fogo_x[i]<=asteroide_x[k]+70 )||
                    (fogo_x[i]+65>=asteroide_x[k] && fogo_x[i]+65<=asteroide_x[k]+70 )||
                    (asteroide_x[k]>=fogo_x[i] && asteroide_x[k]<=fogo_x[i]+65)||
                    (asteroide_x[k]+70>=fogo_x[i] && asteroide_x[k]+70<=fogo_x[i]+65)) && 
                    ((fogo_y[i]>=asteroide_y[k] && fogo_y[i]<=asteroide_y[k]+70 )||
                    (fogo_y[i]+40>=asteroide_y[k] && fogo_y[i]+40<=asteroide_y[k]+70 )||
                    (asteroide_y[k]>=fogo_y[i] && asteroide_y[k]<=fogo_y[i]+40)||
                    (asteroide_y[k]+70>=fogo_y[i] && asteroide_y[k]+70<=fogo_y[i]+40))){
                      remove_object(fogos,fogo_x,fogo_y,&fogo_idx,i);
                      remove_object(asteroides,asteroide_x,asteroide_y,&asteroide_idx,k);
                      score += 150;
                      projectile_removed=true;
                      break;
                    }
                  }
                  if(projectile_removed){i--;continue;}
                  for(int j=1; j<=et_idx; j++){
                    if(((fogo_x[i]>=et_x[j] && fogo_x[i]<=et_x[j]+150 )||
                    (fogo_x[i]+65>=et_x[j] && fogo_x[i]+65<=et_x[j]+150 )||
                    (et_x[j]>=fogo_x[i] && et_x[j]<=fogo_x[i]+65)||
                    (et_x[j]+150>=fogo_x[i] && et_x[j]+150<=fogo_x[i]+65)) && 
                    ((fogo_y[i]>=et_y[j] && fogo_y[i]<=et_y[j]+93 )||
                    (fogo_y[i]+40>=et_y[j] && fogo_y[i]+40<=et_y[j]+93 )||
                    (et_y[j]>=fogo_y[i] && et_y[j]<=fogo_y[i]+40)||
                    (et_y[j]+93>=fogo_y[i] && et_y[j]+93<=fogo_y[i]+40))){
                      printf("faaaaa\n");

                      remove_object(fogos,fogo_x,fogo_y,&fogo_idx,i);
                      remove_object(ets,et_x,et_y,&et_idx,j);
                       score += 200;
                       i--;
                       break;
                    }
                }
              }
          
              for(int i=asteroide_idx; i>0; i--){
                asteroide_x[i]-=10;
                if(asteroide_x[i]<=90) {
                  remove_object(asteroides,asteroide_x,asteroide_y,&asteroide_idx,i);

                }
              }
              for(int i=fogo_idx; i>0; i--){
                fogo_x[i]+=20;
                if(fogo_x[i]>800) remove_object(fogos,fogo_x,fogo_y,&fogo_idx,i);
              }
              for(int i=et_idx; i>0; i--){
                et_x[i]-=20;
                if(et_x[i]<=-150) {
                  remove_object(ets,et_x,et_y,&et_idx,i);

                }
              }
              for(int i=coin_idx; i>0; i--){
                coin_x[i]-=30;
                if(coin_x[i]<=-40) {
                  remove_object(coins,coin_x,coin_y,&coin_idx,i);

                }
              }

                if(data== ESC_BREAKCODE) state=GAMEOVER;
                change_buffer();

                break;
              
              case INSTRUCTIONS:
                vg_draw_xpm_proj(instrucoes_img, 0,0, instrucoes);
                if(arr[0]==0x39) state = MENU;
                change_buffer();
                break;

              case EXIT:
                break;

              case GAMEOVER:
                vg_draw_xpm_proj(gameover_img, 0,0, gameover);
                sprintf(score_str, "%d", score);
                int x = 320;
                for(int i=0; score_str[i] != '\0'; i++, x+=35){
                  if(score_str[i] == '0') vg_draw_xpm_proj(zero_img, x, 300, zero);
                  if(score_str[i] == '1') vg_draw_xpm_proj(um_img, x, 300, um);
                  if(score_str[i] == '2') vg_draw_xpm_proj(dois_img, x, 300, dois);
                  if(score_str[i] == '3') vg_draw_xpm_proj(tres_img, x, 300, tres);
                  if(score_str[i] == '4') vg_draw_xpm_proj(quatro_img, x, 300, quatro);
                  if(score_str[i] == '5') vg_draw_xpm_proj(cinco_img, x, 300, cinco);
                  if(score_str[i] == '6') vg_draw_xpm_proj(seis_img, x, 300, seis);
                  if(score_str[i] == '7') vg_draw_xpm_proj(sete_img, x, 300, sete);
                  if(score_str[i] == '8') vg_draw_xpm_proj(oito_img, x, 300, oito);
                  if(score_str[i] == '9') vg_draw_xpm_proj(nove_img, x, 300, nove);
                }
                
                mouse_x = 400;
                mouse_y = 220;
                if(arr[0]==0x39) state = MENU;
                change_buffer();
                break;

              default:
                break;
              }
              
            }
            break;
          default:
            break; /* no other notifications expected: do nothing */
        }
      }

      
      else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
      }
    }    
    

    if(disable()!=0) return 1;

    if (kbd_unsubscribe_int() != 0)
        return 1;
    if (mouse_unsubscribe_int() != 0)
      return 1;
    if (timer_unsubscribe_int() != 0)
        return 1;
    vg_exit();
    return 0;
}
