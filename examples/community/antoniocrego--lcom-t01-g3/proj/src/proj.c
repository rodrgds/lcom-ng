#include <lcom/lcf.h>

// include drivers
#include "controllers/video/video_gr.h"
#include "controllers/video/gpu.h"
#include "controllers/mouse/mouse.h"
#include "controllers/keyboard/keyboard.h"
#include "controllers/timer/timer.h"
#include "controllers/utils/kbc.h"
#include "controllers/rtc/rtc.h"

// include game classes
#include "game/game.h"
#include "sprites/sprites.h"
#include "game/hitbox.h"
#include "game/list.h"


int (main)(int argc, char *argv[]) {

  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
  lcf_log_output("/home/lcom/labs/proj/output.txt");
  if (lcf_start(argc, argv)) return 1;

  lcf_cleanup();

  return 0;
}

extern uint8_t mouse_bytes[3];
extern uint8_t byte_index;
uint8_t pressedKey;
extern struct packet p;
extern vbe_mode_info_t mode_info;

extern node *head;
extern enemy_node* enemy_list_head;

extern Sprite* character;
extern Sprite* cursor;
extern Sprite* title1;
extern Sprite* title2;
extern Sprite *daybg;
extern Sprite *nightbg;
extern Sprite *level1;
extern Sprite *level2;
extern Sprite *level3;
extern Sprite *level4;
extern Sprite *level5;
extern Sprite *level6;
extern Sprite *level7;
extern Sprite *paused1;
extern Sprite *paused2;
extern Sprite *paused3;
extern Sprite *game_over;

extern Sprite *soul_blast_powerup;
extern invis_hitbox soul_blast;
extern invis_hitbox double_jump;
extern Sprite *jump_powerup;
extern Sprite *soul_blast_atk;
extern Sprite *attack_anim;
extern Sprite *mouse;
extern Sprite *victory;

extern boss_character l7boss;

/**
*@brief Run the game loop.
*
*Runs the main game loop, where the game logic is executed.
*/
int game_loop(){
  bool game = true;
  int ipc_status;
  uint8_t irq_kb, irq_mouse, irq_timer;
  message msg;
  
  player_character player = init_player();
  bool doubleJump = true;
  uint32_t timerCounter = 0;
  uint8_t rtc_hour;

  invis_hitbox player_predict = init_hitbox(0,0,player.character.hitbox.width, player.character.hitbox.length,"character");
  invis_hitbox feet_predict = init_hitbox(0,0, player.feethitbox.width, player.feethitbox.length, "character");

  level level_array[] = {{0,generateLevel1,level1},{1,generateLevel2,level2},{2,generateLevel3,level3},{3,generateLevel4,level4},{4,generateLevel5,level5},{5,generateLevel6,level6},{6,generateLevel7,level7}};
  attack slash_entity = {1000,1000,0,0,1,init_hitbox(1000,1000, 39, 90,"attack"),0, attack_anim, drawSprite};
  attack blast_entity = {1000,1000,0,0,1,init_hitbox(1000,1000,135,46,"attack"),0,soul_blast_atk, drawSprite};

  uint32_t store_timer = 0;

  uint8_t current_level = 0;
  game_state game_state = MAINMENU;
  int8_t selected = 0;

  uint32_t mouseX = 400;
  uint32_t mouseY = 300;

  if(timer_set_frequency(0,60)!=0) return 1;
  if(timer_subscribe_int(&irq_timer)!=0) return 1;
  if(keyboard_subscribe_interrupts(&irq_kb)!=0) return 1;
  if(mouse_subscribe_interrupts(&irq_mouse)!=0) return 1;
  if(write_to_mouse(0xF4)!=0) return 1; // enable data report


  while(game){
    if( driver_receive(ANY, &msg, &ipc_status) != 0 ){
        printf("Error");
        continue;
    }
    if(is_ipc_notify(ipc_status)) {
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_mouse) {
            mouse_ih();
            mouse_sync_bytes();
            if (byte_index==0 && (p.bytes[0]&BIT(3))){
              if (mouseX+p.delta_x<mode_info.XResolution && mouseX+p.delta_x>0) mouseX+=p.delta_x;
              if (mouseY-p.delta_y<mode_info.YResolution && mouseY-p.delta_y>0) mouseY-=p.delta_y;
              if (game_state==MAINMENU){
                if ((mouseX>=293 && mouseX<=503) && (mouseY>=286 && mouseY<=317)){
                  selected = 0;
                  if (p.lb){
                    player=init_player();
                    timerCounter=0;
                    level_array[0].addHitboxes();
                    current_level = 0;
                    selected=0;
                    game_state=RUNNING;
                  }
                }
                else if ((mouseX>=354 && mouseX<=444) && (mouseY>=396 && mouseY<=434)){
                  selected=1;
                  if (p.lb) game=false;
                }
              }
              else if (game_state==PAUSED){
                if ((mouseX>=318 && mouseX<=489) && (mouseY>=161 && mouseY<=196)){
                  selected=0;
                  if (p.lb) game_state=RUNNING;
                }
                else if ((mouseX>=284 && mouseX<=523)&&(mouseY>=239 && mouseY<=278)){
                  selected=1;
                  if (p.lb) game_state=MAINMENU;
                }
                else if ((mouseX>=313 && mouseX<=494) && (mouseY>=312 && mouseY<=351)){
                  selected=2;
                  if (p.lb) game=false;
                }
              }else if(game_state == RUNNING){
                if(p.lb && timerCounter >= slash_entity.end_time){
                  slash(player, &slash_entity, mouseX, mouseY, timerCounter);
                }
                if (p.rb && player.hasSoulBlast && timerCounter>=blast_entity.end_time){
                  blast(player,&blast_entity,mouseX,mouseY,timerCounter);
                }
              }
              else if(game_state==GAMEOVER || game_state==RESULTS){
                if(p.lb||p.mb||p.rb) game_state=MAINMENU;
              }
            }
          }
          //timer
          if (msg.m_notify.interrupts & irq_timer) {
            // call a function that receives the state and tells the things to draw to frame buffer
            draw_frame_buffer();
            if(game_state==MAINMENU){
              if (selected==0) drawSprite(title1,0,0);
              else if (selected==1) drawSprite(title2,0,0);
            }
            else if(game_state==RUNNING){
              if(get_rtc(4,&rtc_hour)){return 1;}
              if(rtc_hour>=0x20 || rtc_hour<=0x05 ){
                drawSprite(nightbg,0,0);
              }
              else{
                drawSprite(daybg,0,0);
              }
              drawSprite(level_array[current_level].level_sprite,0,0);
              timerCounter++;
              player_predict.x_pos=player.character.x_pos+player.character.x_vel;
              player_predict.y_pos=player.character.y_pos+player.character.y_vel;
              feet_predict.x_pos=player.feethitbox.x_pos+player.character.x_vel;
              feet_predict.y_pos=player.feethitbox.y_pos+player.character.y_vel;

              uint8_t collision=0;
              uint8_t feetCollision=0;
              gate transport = init_gate(-1,-1,0,0);
              
              node* temp=head;
              enemy_node* temp_enemy_node = enemy_list_head;
              while(head!=NULL){
                uint8_t collided_head = verifyCollision(player_predict,head->node);
                uint8_t collided_feet = verifyCollision(feet_predict,head->node);

                temp_enemy_node = enemy_list_head;
                while (temp_enemy_node!=NULL){
                  uint8_t enemy_head_collided = verifyCollision(temp_enemy_node->node.character.hitbox, head->node);
                  uint8_t enemy_feet_collided = verifyCollision(temp_enemy_node->node.feethitbox, head->node);

                  temp_enemy_node->node.head_collision = temp_enemy_node->node.head_collision | enemy_head_collided;
                  temp_enemy_node->node.feet_collision = temp_enemy_node->node.feet_collision | enemy_feet_collided;

                  temp_enemy_node = temp_enemy_node->next;
                }
                
                collision = collision | collided_head;
                feetCollision = feetCollision | collided_feet;

                if ((collided_head || collided_feet)&& strcmp(head->node.type,"door")==0){
                  transport=head->node.destination;
                }

                if((collided_head || collided_feet) && strcmp(head->node.type,"damage")==0 && player.character.inv_time <= timerCounter){
                  player.character.inv_time = timerCounter + 60;
                  knockback(&player.character, collided_feet | collided_head, 50);
                  player.character.hp -= temp_enemy_node->node.damage;
                }

                head=head->next;
              }
              head = temp;
              
              if(collision & TOP){
                  player.character.y_vel = 0;
              }
              if(feetCollision & BOTTOM){
                player.state=WALKING;
                doubleJump=true;
                player.character.y_vel = 0;
              }
              else player.state=JUMPING;

              if((collision & RIGHT) && (player.character.x_vel > 0)){
                  player.character.x_vel = 0;
              }
              if((collision & LEFT) && (player.character.x_vel < 0)){
                  player.character.x_vel = 0;
              }

              if (player.state==JUMPING && timerCounter%2==0) player.character.y_vel+=1;

              update_attack_position(&blast_entity);

              enemies_interactions(&player, timerCounter, &slash_entity, &blast_entity, &game_state, &store_timer);              

              update_player_position(&player,player.character.x_pos+player.character.x_vel,player.character.y_pos+player.character.y_vel);
              
              if (transport.destination_level!=-1){
                current_level=transport.destination_level;
                update_player_position(&player,transport.destination_x,transport.destination_y);
                level_array[current_level].addHitboxes();
              }
              if (!player.hasDoubleJump && current_level==3){
                drawSprite(jump_powerup,619,415);
                if (verifyCollision(double_jump,player.character.hitbox)) player.hasDoubleJump=true;
              }
              if (!player.hasSoulBlast && current_level==5){
                drawSprite(soul_blast_powerup,496,494);
                if (verifyCollision(soul_blast,player.character.hitbox)) player.hasSoulBlast=true;
              }
              if (timerCounter >= slash_entity.end_time){
                clear_attack(&slash_entity);
              }

              player.character.drawChar(player.character.char_sprite,player.character.x_pos,player.character.y_pos);
              slash_entity.drawFunc(slash_entity.sprite, slash_entity.x_pos, slash_entity.y_pos);
              blast_entity.drawFunc(soul_blast_atk,blast_entity.x_pos,blast_entity.y_pos);
              drawHUD(player.character.hp,10,10,10);
              if (current_level==6){
                if (timerCounter%420==0) addEnemyNode(generateRandomPositionEnemy());
              }
              if (player.hasSoulBlast && blast_entity.end_time<=timerCounter){
                drawSprite(mouse,700,500);
              }
              if(player.character.hp==0){
                game_state=DYING;
              }
            }
            else if (game_state==DYING){
                player.character.drawChar(player.character.char_sprite,player.character.x_pos,player.character.y_pos);
                draw_frame_buffer();
                p.lb=0;
                p.rb=0;
                p.mb=0;
                tickdelay(micros_to_ticks(5000000));
                game_state=GAMEOVER;
            }
            else if(game_state==PAUSED){
              if(selected==0) drawSprite(paused1,0,0);
              else if(selected==1) drawSprite(paused2,0,0);
              else if(selected==2) drawSprite(paused3,0,0);
            }
            else if (game_state==GAMEOVER){
              drawSprite(game_over,0,0);
            }
            else if (game_state==CUTSCENE){
              timerCounter++;
              if(get_rtc(4,&rtc_hour)){return 1;}
              if(rtc_hour>=0x20 || rtc_hour<=0x05 ){
                drawSprite(nightbg,0,0);
              }
              else{
                drawSprite(daybg,0,0);
              }
              drawSprite(level_array[current_level].level_sprite,0,0);
              player.character.drawChar(player.character.char_sprite,player.character.x_pos,player.character.y_pos);
              l7boss.enemystruct.character.drawChar(l7boss.enemystruct.character.char_sprite,l7boss.enemystruct.character.x_pos,l7boss.enemystruct.character.y_pos);
              if (timerCounter>=store_timer+720) game_state=RESULTS;
              else if ((timerCounter-store_timer)%180==0){
                l7boss.enemystruct.character.char_sprite=l7boss.array[(timerCounter-store_timer)/180];
              }
            }
            else if (game_state==RESULTS){
              drawSprite(victory,0,0);
            }
            drawSprite(cursor,mouseX,mouseY);
          }
          //keyboard
          if (msg.m_notify.interrupts & irq_kb) {
            // run a keyboard handler that receives the game state and character class
            read_KBC_output(0x60,&pressedKey,0);
            if(game_state==MAINMENU){
              if(selected==0 && pressedKey==0x1C){
                player=init_player();
                timerCounter=0;
                level_array[0].addHitboxes();
                current_level = 0;
                selected=0;
                game_state=RUNNING;
              }
              else if(selected==1 && pressedKey==0X1C) game=false;
              if(pressedKey==0x11 || pressedKey==0x1F) selected = !selected;
            }
            else if (game_state==RUNNING){
              if(pressedKey==0x81){
                game_state=PAUSED;
              }
              if(pressedKey==0x20){
                player.character.x_vel=3;
                player.character.drawChar=drawSprite;
              }
              if(pressedKey==0x1E){
                player.character.x_vel=-3;
                player.character.drawChar=draw_sprite_flipped;
              }
              if(pressedKey==0x9E || pressedKey==0xA0) player.character.x_vel=0;
              if(pressedKey==0x39 && player.state==WALKING){
                player.state=JUMPING;
                player.character.y_vel=-15;
              }
              else if(pressedKey==0x39 && player.state==JUMPING && doubleJump && player.hasDoubleJump){
                doubleJump=false;
                player.character.y_vel=-15;
              }
            }
            else if(game_state==PAUSED){
              if (pressedKey==0x81) game_state=RUNNING;
              if (pressedKey==0x11) selected--;
              if (pressedKey==0x1F) selected++;
              if (selected>2) selected=0;
              if (selected<0) selected=2;
              if (pressedKey==0x1C){
                if (selected==2) game=false;
                else if (selected==1){
                  selected=0;
                  game_state=MAINMENU;
                }
                else if (selected==0){
                  selected=0;
                  game_state=RUNNING;
                }
              }
            }
            else if(game_state==GAMEOVER || game_state==RESULTS){
              if (pressedKey) game_state=MAINMENU; 
            }
          }
          break;
        default:
          break;
      }
    }
  }
  if(mouse_unsubscribe_interrupts()!=0) return 1;
  if(keyboard_unsubscribe_interrupts()!=0) return 1;
  if(timer_unsubscribe_int()!=0) return 1;
  if(write_to_mouse(0xF5)!=0) return 1;
  return 0;
}

/**
 * @brief Main loop function of the project.
 *
 * This function initializes the necessary components, runs the game loop,
 * and cleans up resources before exiting.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line argument strings.
 * @return 0 upon successful execution.
 */
int (proj_main_loop)(int argc, char *argv[]){
    set_frame_buffer(0x115);
    set_graphic_mode(0x115);
    loadSprites();
    init_all_hitboxes();
    game_loop();
    unloadSprites();
    vg_exit();
    return 0;
}
