#include "proj.h"
#include "alphabet.h"
#include "digits.h"

static State state = MENU;
static Game game;
static mouse_sprite mouse;

static void (*proj_step_state)(Event event);


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  //lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

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

int (proj_main_loop)(int argc, char* argv[]) {
  if (proj_setup()) return 1;


  while (state != OVER) {
    proj_step();
  }

  return proj_cleanup();
}

int proj_setup(void) {
  vg_init(VIDEO_MODE);

  if (subscribe_interrupts()) return 1;

  mouse_sprite_create(&mouse);
  menu_init();

  proj_set_state(state);
  return 0;
}

void proj_step(void) {
  if (!interrupt_handler()) // no interrupts
    return;
  Event event = get_event();
  mouse_sprite_step(&mouse, event);
  proj_step_state(event);
  if (event.type == TIMER) {
    if (event.info.timer.count_interrupts % 4)
      vg_refresh();
  }
}

int proj_cleanup(void) {
  int exit = unsubscribe_interrupts();
  if (vg_exit() || exit) return 1;
  return 0;
}

void proj_set_state(State new_state) {
  state = new_state;
  switch (new_state) {
    case GAME:
      game_create(&game);
      proj_step_state = proj_step_game;
      break;
    case MENU:
      proj_step_state = menu_step;
      break;
    case GAME_OVER:
      proj_step_state = proj_step_game_over;
      break;
    case SCORE:
      proj_step_state = score_board_step;
      break;
    default:
      break;
  }
}

void proj_step_game(Event event) {
  game_step(&game, &event);
}

void proj_step_game_over(Event event) {
  game_over_step(&game, event);
}

mouse_sprite* proj_get_mouse() {
  return &mouse;
}
