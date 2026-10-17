#include "jogo.h"

//#define DEBUG

#define FPS 120
#define MAX_VGA_FPS 10
#define NPLATFORMS 8

#define PLSPEED 0.25
#define PLJUMP -7

static int kbd_hook_id = 0;
static int timer_hook_id = 1;
static int timer_input_hook_id = 2;
void *ptr_start;
void *double_buffer;
void *triple_buffer;

int start(Jogo* jogo){

  Log("Initiating game\n");

  Log("Initiating graphics in video mode\n");
  ptr_start = vg_init(0x114);  // Init graphics in video mode
  double_buffer = malloc(v_res*h_res*bits_per_pixel/8);
  triple_buffer = malloc(v_res*h_res*bits_per_pixel/8);
  if(double_buffer==NULL){
    printf("Not enough memory for double buffer\n");
    Log("Not enough memory for double buffer\n");
    quit(jogo);
  }
  if(triple_buffer==NULL){
    printf("Not enough memory for tripple buffer\n");
    Log("Not enough memory for tripple buffer\n");
    quit(jogo);
  }


  Log("Creating player\n");
  // Create the player
  Jogador jogador;
  jogador.w = 20;
  jogador.h = 30;
  jogador.x = h_res/2-jogador.h/2;
  jogador.y = v_res-2*jogador.h;
  jogador.yspeed = (PLJUMP*120)/FPS; // Player starts jumping
  jogo->jogador = &jogador;   // Add the player to the game struct
  // ...

  // Create the platforms
  jogo->plataformas = malloc(sizeof(Plataforma)*NPLATFORMS);
  if(jogo->plataformas == NULL){
    Log("####ERROR#### : Could not allocate memory for platforms\n");
    return 1;
  }
  unsigned int i;
  for(i=0;i<NPLATFORMS;i++){
    jogo->plataformas[i].w = rand() % 40 + 41;
    jogo->plataformas[i].h = 15;
    jogo->plataformas[i].x = rand() % (h_res-1-jogo->plataformas[i].w);
    //jogo->plataformas[i].x = h_res/2;
    jogo->plataformas[i].y = i*(jogo->plataformas[i].h+(v_res-1)/NPLATFORMS);
    jogo->plataformas[i].movable = 1;
    jogo->plataformas[i].boost = 1;
  }
  // ...

  Log("Setting timer to 120FPS\n");
  timer_set_square(0, FPS);   // Set the Timer at 120 FPS

  Log("Seeding random number\n");
  time_t t;
  srand((unsigned) time(&t));
  Log("Entering main game loop\n");
  loop(jogo);   // Main game loop (read, update, render)
  Log("Calling quit() function\n");
  quit(jogo);   // Cleanup and exit the game
  return 0;
}

/*
 * MAIN GAME LOOP
 */
int loop(Jogo* jogo){
  Log("Subscribing all the peripherals\n");
  // Set all the interrupts
  Log("Subscribing keyboard\n");
  int kbd_irq_set = kbd_subscribe_int(jogo);
  Log("Subscribing main timer\n");
  int timer_irq_set = timer_subscribe_int(jogo, timer_hook_id);
  Log("Subscribing input timer\n");
  int timer_input_irq_set = timer_subscribe_int(jogo, timer_input_hook_id);
  if (kbd_irq_set == -1 || timer_irq_set == -1 || timer_input_irq_set == -1){
    Log("####ERROR#### : Something went bad with subscribing to the peripherals\n");
    return 1;
  }
  // ...
  int ipc_status;
  int r, key_code;
  int quit = 0;
  message msg;
  // ...

  int moveright = 0;
  int moveleft = 0;

  unsigned int i;
  unsigned int vga_timer = 0;

  // Main loop
  Log("Main while() loop\n");
  while (quit == 0) {
    if ((r = driver_receive(ANY, &msg, &ipc_status) != 0)) continue;

#ifdef DEBUG
    Log("Checking interrupts\n");
#endif
    if (is_ipc_notify(ipc_status)){
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:

          /*
           * Keyboard input
           */
          if (msg.NOTIFY_ARG & kbd_irq_set) {
            key_code = kbd_int_handler(jogo);
            if(key_code == 0x20) moveright = 1;
            else if(key_code == 0xA0) moveright = 0;

            if(key_code == 0x1E) moveleft = 1;
            else if(key_code == 0x9E) moveleft = 0;

            if (key_code == 0x01)
              quit = 1;
          }
          /*
           * Read input timer
           */
          if (msg.NOTIFY_ARG & timer_input_irq_set) {


#ifdef DEBUG
            Log("Input timer interrupt\n");
#endif
            if(moveright) jogo->jogador->xspeed += (PLSPEED*120)/FPS;
            else if(moveleft) jogo->jogador->xspeed -= (PLSPEED*120)/FPS;
            else if(abs(jogo->jogador->xspeed)<=(0.05*120)/FPS) jogo->jogador->xspeed = 0;


            //  Update the player position
#ifdef DEBUG
            Log("\tUpdating player data\n");
#endif
            updateJogador(jogo->jogador, FPS);

            // Check boundaries for player
#ifdef DEBUG
            Log("\tChecking player boundaries\n");
#endif
            if(jogo->jogador->x+jogo->jogador->w+jogo->jogador->xspeed > h_res){
              jogo->jogador->x = 0;
            }
            else if(jogo->jogador->x+jogo->jogador->xspeed < 0){
              jogo->jogador->x = h_res-jogo->jogador->w;
            }
            if(jogo->jogador->y+jogo->jogador->h+jogo->jogador->yspeed > v_res){
              jogo->jogador->y = v_res-jogo->jogador->h;
              jogo->jogador->yspeed = (PLJUMP*120)/FPS;
            }
            else if(jogo->jogador->y+jogo->jogador->yspeed < 0){
              jogo->jogador->y = 1;
              jogo->jogador->yspeed = 0;
            }
            // ...

#ifdef DEBUG
            Log("\tChecking collisions\n");
#endif
            if(jogo->jogador->yspeed>0) checkCollisions(jogo);


#ifdef DEBUG
            Log("\tUpdating platforms data\n");
#endif
            for(i=0;i<NPLATFORMS;i++){
              if(jogo->jogador->yspeed<=0 && jogo->jogador->y < v_res/2-jogo->jogador->h/2){
                jogo->plataformas[i].yspeed = -jogo->jogador->yspeed;
              }
              else jogo->plataformas[i].yspeed = 0;
              updatePlataforma(&jogo->plataformas[i]);
            }
          }

          /*
           *
           * Update all the objects and redraw
           *
           */

          if (msg.NOTIFY_ARG & timer_irq_set) {
            vga_timer+=MAX_VGA_FPS;
            if(vga_timer>=FPS){
              draw(jogo);
              vga_timer = 0; 
            }
          }
          break;
        default:
          break;
      }
    }
  }
  return 0;
}

int draw(Jogo* jogo){
  unsigned int i;
#ifdef DEBUG
  Log("Graphics timer interrupt\n");
#endif
  // Clear the double buffer
  vg_clear(triple_buffer);
#ifdef DEBUG
  Log("\tDrawing platforms\n");
#endif
  /*for(i=0;i<NPLATFORMS;i++){
    vg_drawRectangle(triple_buffer
        , jogo->plataformas[i].x, jogo->plataformas[i].y, jogo->plataformas[i].h, jogo->plataformas[i].w, jogo->plataformas[i].color);
  }*/
#ifdef DEBUG
  Log("\tDrawing player\n");
#endif
  //vg_drawRectangle(triple_buffer, jogo->jogador->x, jogo->jogador->y, jogo->jogador->h, jogo->jogador->w, rgb(255, 255, 255));

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Bitmap* bmp;
  bmp = loadBitmap("/home/lcom/projeto/doodlejump2.bmp");
  for(i=0;i<NPLATFORMS;i++){
	  drawBitmap(bmp, triple_buffer,
			  jogo->plataformas[i].x, jogo->plataformas[i].y, jogo->plataformas[i].w, jogo->plataformas[i].h);
  }
  drawBitmap(bmp, triple_buffer, jogo->jogador->x, jogo->jogador->y, jogo->jogador->w, jogo->jogador->h);
  deleteBitmap(bmp);
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // Copy double buffer to VGA
  memcpy(ptr_start, double_buffer, h_res*v_res*bits_per_pixel/8);
  memcpy(double_buffer, triple_buffer, h_res*v_res*bits_per_pixel/8);

  return 0;
}
/*
 * END MAIN GAME LOOP
 */

int quit(Jogo* jogo){
  Log("Cleaning up\n");
  Log("Freeing memory\n");
  free(jogo->plataformas);
  free(double_buffer);
  free(triple_buffer);
  Log("Unsubscribing peripherals\n");
  kbd_unsubscribe_int(jogo);
  timer_unsubscribe_int(jogo, timer_hook_id);
  timer_unsubscribe_int(jogo, timer_input_hook_id);
  Log("Exiting graphics in video mode\n");
  vg_exit();
  return 0;
}

int checkCollisions(Jogo *jogo){
  unsigned int i;
  unsigned short plx,ply,plw,plh, platx, platy, platw, plath;
  plx = jogo->jogador->x; ply = jogo->jogador->y; plw = jogo->jogador->w; plh = jogo->jogador->h;
  for(i=0;i<NPLATFORMS;i++){
    platx = jogo->plataformas[i].x; platy = jogo->plataformas[i].y; platw = jogo->plataformas[i].w; plath = jogo->plataformas[i].h;

    if(plx< platx+platw && plx+plw > platx &&
        ply < platy+plath && ply+plh > platy){
      jogo->jogador->y = jogo->plataformas[i].y-jogo->jogador->h;
      if(jogo->plataformas[i].boost == 0)
        jogo->jogador->yspeed = (PLJUMP*1.8*120)/FPS;
      else jogo->jogador->yspeed = (PLJUMP*120)/FPS;
      return 0;
    }
  }
  return 1;
}

/*
 * INTERRUPTS
 */
/*
 * Keyboard
 */
int kbd_subscribe_int(Jogo* jogo){
  int hook_id_bit = 1 << kbd_hook_id;
  if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id) != OK)
    return -1;
  if (sys_irqenable(&kbd_hook_id) != OK)
    return -1;
  return hook_id_bit;
}
int kbd_unsubscribe_int(Jogo* jogo) {

  if (sys_irqrmpolicy(&kbd_hook_id) != OK)
    return 1;
  if (sys_irqdisable(&kbd_hook_id) != OK)
    return 1;
  return 0;
}
int kbd_int_handler(Jogo* jogo)
{
  unsigned char keycode;
  kbc_read_out_buf(&keycode);
  return keycode;
}
/*
 * END Keboard
 */

/*
 * Timer
 */
int timer_subscribe_int(Jogo* jogo, int hook) {
  int hook_id_bit = 1 << hook;
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook) != OK)
    return -1;
  if (sys_irqenable(&hook) != OK)
    return -1;
  return hook_id_bit;
}
int timer_unsubscribe_int(Jogo* jogo, int hook) {
  if (sys_irqrmpolicy(&hook) != OK)
    return 1;
  if (sys_irqdisable(&hook) != OK)
    return 1;

  return OK;
}

int timer_set_square(unsigned long timer, unsigned long freq) {
  unsigned short div;
  unsigned char msb, lsb, selection = 0x00;
  div = TIMER_FREQ/freq;
  lsb = div & 0xFF;
  msb = (div >> 8) & 0xFF;
  switch(timer){
    case 0:
      selection |= TIMER_SEL0;
      break;
    case 1:
      selection |= TIMER_SEL1;
      break;
    case 2:
      selection |= TIMER_SEL2;
      break;
    default:
      return 1;
      break;
  }
  int res;
  unsigned char st;
  res = timer_get_conf(timer, &st);
  if (res != 0) return 1;
  st &= 0x01; //00000001b
  selection |= st;

  res = sys_outb(TIMER_CTRL, selection | TIMER_LSB_MSB | TIMER_SQR_WAVE);
  if (res != 0) return 1;

  res = sys_outb(timer + TIMER_0, lsb);
  if (res != 0) return 1;

  res = sys_outb(timer + TIMER_0, msb);
  if (res != 0) return 1;

  return 0;
}
int timer_get_conf(unsigned long timer, unsigned char *st) {
  int res = sys_outb(TIMER_CTRL, TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer));
  if (res != 0) return 1;
  unsigned long stl;
  res = sys_inb(timer + TIMER_0, &stl);
  *st = stl;
  if (res != 0) return 1;
  return 0;
}
/*
 * END Timer
 */
/*
 * END Interrupts
 */
