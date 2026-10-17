//ssh lcom@127.0.0.1 -p 2222
#include "proj.h"

//set all the timer and keyboard variables
uint8_t irq_set_kbc;
uint8_t irq_set_timer;
uint8_t irq_set_mouse;
uint8_t irq_set_rtc;

//colors
uint32_t color_enemy;
uint32_t color_laser;
uint32_t color_background;

//user related variables
uint16_t x;
uint16_t y;
uint16_t lives;
uint16_t score;

//user movement related variables
bool right;
bool left;
bool up;
bool down;

//level related variables
extern int shootfrequency;
extern int movementmode;
extern int gamemode;
int randspawn;
int spawnchance = 5;

//enemy related variables
struct EnemyNode *el_t; //el = enemy list
extern struct EnemyNode *EnemyFirst; //first node of the list
extern struct EnemyNode *EnemyAdded; //last node of the list
extern int EnemySize;

//Animation related variables
struct AnimationNode *al_t; //al = animation list
extern struct AnimationNode *AnimationFirst; //first node of the list
extern struct AnimationNode *AnimationAdded; //last node of the list
extern int AnimationSize;

//laser related variables
struct LaserNode *ll_t; //ll = laser list
extern struct LaserNode *LaserFirst; //first node of the list
extern struct LaserNode *LaserAdded; //last node of the list

//driver related variables
extern uint8_t scancode;
extern vbe_mode_info_t mode_inf;

//mouse related variables
extern int mouse_x;
extern int mouse_y;
extern int mouse_counter;
extern struct packet mousePacket;
extern uint8_t mouseBytes[3];

//rtc related variables
extern uint8_t rtc_mode;
extern clock_fields_struct info_rtc;
int rtc_counter;

//temp
int closeProj = 0;

//sprite related variables
extern uint16_t *user_sprite;
extern xpm_image_t user_img;

extern uint16_t *explosion1_sprite;
extern xpm_image_t explosion1_img;
extern uint16_t *explosion2_sprite;
extern xpm_image_t explosion2_img;
extern uint16_t *explosion3_sprite;
extern xpm_image_t explosion3_img;
extern uint16_t *explosion4_sprite;
extern xpm_image_t explosion4_img;
extern uint16_t *explosion5_sprite;
extern xpm_image_t explosion5_img;
extern uint16_t *explosion6_sprite;
extern xpm_image_t explosion6_img;
extern uint16_t *explosion7_sprite;
extern xpm_image_t explosion7_img;
extern uint16_t *cursor_sprite;
extern xpm_image_t cursor_img;

extern uint16_t *eb1_sprite;
extern xpm_image_t eb1_img;
extern uint16_t *eb2_sprite;
extern xpm_image_t eb2_img;
extern uint16_t *eb3_sprite;
extern xpm_image_t eb3_img;

extern uint16_t *enemy1_sprite;
extern xpm_image_t enemy1_img;
extern uint16_t *enemy2_sprite;
extern xpm_image_t enemy2_img;
extern uint16_t *enemy3_sprite;
extern xpm_image_t enemy3_img;
int current_enemy_sprite = 0;

extern uint16_t *eb1_sprite;
extern xpm_image_t eb1_img;
extern uint16_t *eb2_sprite;
extern xpm_image_t eb2_img;
extern uint16_t *eb3_sprite;
extern xpm_image_t eb3_img;
int current_ebullet_sprite = 0;

//other variables
int mytime = 60;
int mytime2 = 1800;

int (main)(int argc, char *argv[]) {

  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/g3/proj/trace.txt");
  lcf_log_output("/home/lcom/labs/g3/proj/output.txt");
  if (lcf_start(argc, argv)) return 1;

  lcf_cleanup();

  return 0;
}

//sets the color variables to their correct value
void normalize_all()
{
    color_enemy = normalize_color(ENEMY_COLOR);
    color_laser = normalize_color(LASER_COLOR);
    color_background = normalize_color(BACKGROUND_COLOR);
}

int interrupt_loop()
{
    int r,i=0;
    uint8_t bytes[2];

    //interrupt loop
    int ipc_status;
    message msg;
    while(scancode != KBC_BRK_ESC_KEY && !closeProj) //only exits with ESC
    { /* You may want to use a different condition */
        /* Get a request message. */
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) 
        {
        case HARDWARE: /* hardware interrupt notification */				
            if (msg.m_notify.interrupts & irq_set_kbc) //keyboard interrupts
            { /* subscribed interrupt */
                //get the scanconde
                kbc_ih();
                if(scancode == KBC_2BYTE_CODE){
                    bytes[i] = scancode;
                    i++;
                    continue;
                }
                bytes[i] = scancode;
                i=0;
                
                if(scancode == KBC_W_MAKE_KEY) up = true; //move up
                else if(scancode == KBC_W_BREAK_KEY) up = false; //stop moving up
                else if(scancode == KBC_S_MAKE_KEY) down = true; //move down
                else if(scancode == KBC_S_BREAK_KEY) down = false; //stop moving down
                else if(scancode == KBC_A_MAKE_KEY) left = true; //move left
                else if(scancode == KBC_A_BREAK_KEY) left = false; //stop moving left
                else if(scancode == KBC_D_MAKE_KEY) right = true; //move right
                else if(scancode == KBC_D_BREAK_KEY) right = false; //stop moving right
                else if (scancode == KBC_SPACE_MAKE_KEY) ll_addNode(ll_getNewNode(x, y, 1)); //shoot laser
                else if(scancode == KBC_E_MAKE_KEY) el_addNode(el_getNewNode(0, 200, 200, mode_inf.XResolution - 200, 200, 200)); //add enemy
                else if(scancode == KBC_B_MAKE_KEY) 
                {
                    if(EnemyFirst != NULL)//remove enemy
                    {
                        el_removeFirst(); 
                    }
                } 
            }
            if (msg.m_notify.interrupts & irq_set_timer) 
            {
                clear_buffer();
                increment_missile();
                //if(draw_xpm(cursor_sprite, cursor_img, mouse_x, mouse_y)) return 1;
                if (vg_draw_rectangle(mouse_x, mouse_y, 10, 10, 0xffffff)) return 1;
                //if(EnemyFirst == NULL)
                //draw cursor
                if(EnemySize == 0)
                {
                    if(gamemode == 0) return 0;
                    if(gamemode == 1) spawn_endless();
                }
                //if (clock_fields_update()) return 1;
                handle_top_bar();
                if(gamemode == 1)
                {
                    if ((rand() % 1000) < spawnchance)
                    {
                        randspawn = rand() % 3;
                        if(randspawn == 0)
                        {
                            el_addNode(el_getNewNode(100, 100, 100, 1150, 100, 100));
                        }
                        else if(randspawn == 1)
                        {
                            el_addNode(el_getNewNode(100, 400, 100, 1150, 400, 400));
                        }
                        else
                        {
                            el_addNode(el_getNewNode(100, 700, 100, 1150, 700, 700));
                        }
                    }
                }
                //call the respective handler for each object
                ll_t = LaserFirst;
                while(ll_t != NULL)
                {
                    if(handle_laser()) return 1;
                }
                mytime--;
                if (mytime == 30) {if (clock_fields_update()) return 1;} // Update rtc every second
                if(mytime == 0)
                {
                    if (clock_fields_update()) return 1; // Update rtc every second
                    printf("\nscore = %u", score);
                    if(score >= 2)
                    {
                        score -= 2;
                    }
                    if(score == 1)
                    {
                        score = 0;
                    }
                    mytime = 60;
                    current_enemy_sprite++;
                    if(current_enemy_sprite == 3) current_enemy_sprite = 0;
                    current_ebullet_sprite++;
                    if(current_ebullet_sprite == 3) current_ebullet_sprite = 0;
                }
                mytime2--;
                if(mytime2 == 0)
                {
                    mytime2 = 1800;
                    if(spawnchance < 300)
                    {
                        spawnchance++;
                    }
                }
                el_t = EnemyFirst;
                while(el_t != NULL)
                {
                    if(handle_enemy()) return 1;
                }
                al_t = AnimationFirst;
                while(al_t != NULL)
                {
                    if(handle_animation()) return 1;
                }
                if(handle_missile()) return 1;
                handle_user();

                //if (vg_draw_rectangle(mouse_x, mouse_y, 10, 10, 0x000000)) return 1;
                video_switch();
            }
            if (msg.m_notify.interrupts & irq_set_mouse) {
                mouse_ih();

                if (mouse_counter == 3)  {
                    mouse_counter = 0;
                    mouse_write_to_packet();
                    mouse_update_coords();
                    if (mousePacket.lb)  {shoot_missile();}
                }
            }
            /*if (msg.m_notify.interrupts & irq_set_rtc) {
                rtc_counter++;
                //get_rtc_info();
                if (rtc_counter % 30 == 0) {
                    if (clock_fields_update()) return 1;
                    //handle_top_bar();
                    rtc_counter = 60;
                }
                printf("interrupt");
            }*/
            break;
        default:
            break; /* no other notifications expected: do nothing */	
        }
        } 
        else 
        { /* received a standard message, not a notification */
            /* no standard messages expected: do nothing */
        }
    }
    return 0;
}

void player_defaults()
{
    //set the players initial values
    x = mode_inf.XResolution/2 - 15;
    y = 600 + ((mode_inf.YResolution - 550) / 2) - 25;
    lives = 3;
    score = 1000;
    right = false;
    left = false;
    up = false;
    down = false;
    startup_missile();
    el_CleanUp(EnemyFirst);
    EnemyFirst = NULL;
    EnemyAdded = NULL;
    ll_CleanUp(LaserFirst);
    LaserFirst = NULL;
    LaserAdded = NULL;
    al_CleanUp(AnimationFirst);
    AnimationFirst = NULL;
    AnimationAdded = NULL;
}

int (proj_main_loop)(int argc, char **argv)
{
    //graphics card setup
    if(map_vram(0x11A)) return 1;

    if(change_graphics_mode(0x11A)) return 1;

    //load all xpm data
    load_xpm();

    //set all the color variables
    normalize_all();

    //subscribe to time and keyboard and set the frequency of the times
    if(timer_subscribe_int(&irq_set_timer)) return 1;
    
    if(timer_set_frequency(0, 30)) return 1;
    
    if(kbc_subscribe(&irq_set_kbc)) return 1;

    if(proj_enable_data_reporting()) return 1;

    if(mouse_subscribe_int(&irq_set_mouse)) return 1;

    get_rtc_info();


    //if(rtc_subscribe_int(&irq_set_rtc)) return 1;

    
    player_defaults();

    //draw the player in their initial position
    //if(vg_draw_rectangle(x, y, 50, 30, color_user)) return 1;
    if(draw_xpm(user_sprite, user_img, x, y)) return 1;

    call_level(4);
    

    //Clean up the memory, probably not necessary
    ll_CleanUp(LaserFirst);
    LaserFirst = NULL;
    LaserAdded = NULL;   
    el_CleanUp(EnemyFirst);
    EnemyFirst = NULL;
    EnemyAdded = NULL;


    //unsubscribe to the keyboard and timer
    if(kbc_unsubscribe()) return 1;
    
    if(timer_unsubscribe_int()) return 1;

    if (mouse_unsubscribe_int()) return 1;

    //if (rtc_unsubscribe_int()) return 1;

    return vg_exit();
}
