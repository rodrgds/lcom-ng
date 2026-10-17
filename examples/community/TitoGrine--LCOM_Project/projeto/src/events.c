#include "events.h"
#include "player.h"

extern bool x_sign; /**< True if horizontal displacement is negative */
extern bool y_sign; // True if vertical displacement is negative

extern char *words[]; // Array of string of random words

player *p1;                        // Player 1
player *p2;                        // Player 2
static char winner_name[20] = {0}; // Name of the player who won

static int tmp_points_p1 = 0; // Player who guessed
static int tmp_points_p2 = 0; // Player who drew the image

static int16_t speed_x = 0; // Mouse horizontal displacement
static int16_t speed_y = 0; // Mouse vertical displacement

static int16_t delta_x = 0; // Ink horizontal displacement
static int16_t delta_y = 0; // Ink vertical displacement

static int color = 0; // Current paint color

static int chronometer = 60; // Timer for stopwatch while playing

static unsigned int counter = 0; // Counter used to store round number

char time_string[20] = {0}; // String that stores current date and time
static char timer[2] = {0}; // String to display stopwatch time

// Boolean that is set to 1 if the given active button sprite is drawn (prevents constant active sprites drawing)
static bool active_play = false;
static bool active_draw = false;
static bool active_exit = false;
static bool active_ready = false;

static bool rtc_active = false; // True if date and time are being dispayed

static bool avatar_choosen = false; // True if the current player has choosen an avatar

static bool word_choosen = false; // True is player has choosen a word

extern char *chosen_word; // Pointer to string of the choosen word

bool mouse_events = false; // True if any mouse button state changed

bool exit_program = false; // True if player has choosen to leave the program

bool second_passed = false; // True if a second has passed since last true

bool alarm_rang = false; // True if RTC alarm interruption was read

Events curr_state; // Events sruct for current program information

// Background Sprites
static Sprite *victory_board, *main_menu, *set_up_menu;

// Cursor Sprites
static Sprite *cursor, *pencil, *pen, *eraser, *bucket;

// RTC sprites
static Sprite *date_icon, *date_slot;

// Button Sprites
static Sprite *play_b_active, *play_b_inactive, *draw_b_active, *draw_b_inactive, *exit_b_active, *exit_b_inactive;
static Sprite *ready_b_inactive_1, *ready_b_inactive_2, *ready_b_active_1, *ready_b_active_2;
static Sprite *grid, *return_b;
static Sprite *trash_b, *bucket_b, *pencil_b, *pen_b, *eraser_b, *spray_b;

// Pallete Sprites
static Sprite *pallete_color, *pallete_grey;

// General cursor Sprites
static Sprite *brush_type, *current, *ink, *tip, *white, *spray;

// Canvas Sprites
static Sprite *canvas, *draw_canvas, *stats, *ink_splatters, *word_header, *timer_slot, *guess_slot;

// Avatar Sprites
static Sprite *toggle_1, *toggle_2, *african_avatar, *arab_avatar, *batman_avatar, *dalai_avatar, *gandhi_avatar, *indian_avatar, *lenin_avatar, *obama_avatar, *robot_avatar, *ronaldo_avatar, *stalin_avatar, *trump_avatar;

// Victory Menu Sprites
static Sprite *winner_frame, *red_balloon, *green_balloon, *yellow_balloon, *orange_balloon, *blue_balloon, *pink_balloon;

//===========================================================================================//

int16_t complement_to_decimal(uint16_t binary) {

  // Checks if the number is already in binary
  if (binary & 0x8000)
    return binary;

  // If it's in 2's complement converts to binary and returns it
  return (int16_t)(~(binary - 0x01));
}

void intialize_states() {
  curr_state.mouse.mb = IGNORE;
  curr_state.mouse.rb = IGNORE;
  curr_state.mouse.lb = IGNORE;

  curr_state.state = INITIAL_MENU;
}

void set_up_sprites() {

  // BACKGROUND SPRITES

  main_menu = create_sprite(background, 0, 0, 0, 0);

  set_up_menu = create_sprite(setup_menu, 0, 0, 0, 0);

  victory_board = create_sprite(victory_background_xpm, 0, 0, 0, 0);

  // MAIN MENU SPRITES

  ink_splatters = create_sprite(splatters, 0, 0, 0, 0);

  play_b_inactive = create_sprite(play_button_inactive, 291, 230, 0, 0);

  play_b_active = create_sprite(play_button_active, 291, 230, 0, 0);

  draw_b_inactive = create_sprite(draw_button_inactive, 291, 358, 0, 0);

  draw_b_active = create_sprite(draw_button_active, 291, 358, 0, 0);

  exit_b_inactive = create_sprite(exit_button_inactive, 291, 484, 0, 0);

  exit_b_active = create_sprite(exit_button_active, 291, 484, 0, 0);

  // SET UP MENU SPRITES

  ready_b_inactive_1 = create_sprite(ready_button_inactive, 407, 396, 0, 0);

  ready_b_active_1 = create_sprite(ready_button_active, 407, 396, 0, 0);

  ready_b_inactive_2 = create_sprite(ready_button_inactive, 407, 618, 0, 0);

  ready_b_active_2 = create_sprite(ready_button_active, 407, 618, 0, 0);

  toggle_1 = create_sprite(avatar_toggle1, 0, 0, 0, 0);

  toggle_2 = create_sprite(avatar_toggle2, 0, 0, 0, 0);

  african_avatar = create_sprite(african, 631, 232, 0, 0);

  arab_avatar = create_sprite(arab, 741, 232, 0, 0);

  batman_avatar = create_sprite(batman, 851, 232, 0, 0);

  dalai_avatar = create_sprite(dalai, 631, 342, 0, 0);

  gandhi_avatar = create_sprite(gandhi, 741, 342, 0, 0);

  indian_avatar = create_sprite(indian, 851, 342, 0, 0);

  lenin_avatar = create_sprite(lenin, 631, 452, 0, 0);

  obama_avatar = create_sprite(obama, 741, 452, 0, 0);

  robot_avatar = create_sprite(robot, 851, 452, 0, 0);

  ronaldo_avatar = create_sprite(ronaldo, 631, 562, 0, 0);

  stalin_avatar = create_sprite(stalin, 741, 562, 0, 0);

  trump_avatar = create_sprite(trump, 851, 562, 0, 0);

  // PLAY/DRAW SPRITES

  canvas = create_sprite(paint_canvas, 171, 185, 0, 0);

  draw_canvas = create_sprite(drawing_canvas, 10, 185, 0, 0);

  stats = create_sprite(stats_frame, 10, 185, 0, 0);

  grid = create_sprite(button_grid, 863, 185, 0, 0);

  pencil_b = create_sprite(pencil_button, 878, 205, 0, 0);

  pen_b = create_sprite(pen_button, 878, 287, 0, 0);

  spray_b = create_sprite(spray_button, 888, 360, 0, 0);

  bucket_b = create_sprite(bucket_button, 879, 443, 0, 0);

  eraser_b = create_sprite(eraser_button, 878, 525, 0, 0);

  trash_b = create_sprite(trash_button, 880, 603, 0, 0);

  pallete_color = create_sprite(color_pallete, 953, 185, 0, 0);

  pallete_grey = create_sprite(grey_pallete, 953, 495, 0, 0);

  return_b = create_sprite(return_button, 953, 619, 0, 0);

  guess_slot = create_sprite(guess_frame, 333, 684, 0, 0);

  // VICTORY SPRITES

  winner_frame = create_sprite(winner_frame_xpm, 252, 272, 0, 0);

  red_balloon = create_sprite(red_balloon_xpm, 10, 10, 5, -5);

  green_balloon = create_sprite(green_balloon_xpm, 200, 120, 5, -5);

  yellow_balloon = create_sprite(yellow_balloon_xpm, 400, 230, 5, 5);

  orange_balloon = create_sprite(orange_balloon_xpm, 600, 340, -5, -5);

  blue_balloon = create_sprite(blue_balloon_xpm, 800, 450, -5, 5);

  pink_balloon = create_sprite(pink_balloon_xpm, 10, 560, -5, 5);

  // GENERAL SPRITES

  word_header = create_sprite(word_choice, 241, 215, 0, 0);

  timer_slot = create_sprite(timer_clock, 48, 96, 0, 0);

  date_icon = create_sprite(RTC_icon, 2, 721, 0, 0);

  date_slot = create_sprite(RTC_active, 47, 721, 0, 0);
}

void destroy_sprites() {
  destroy_sprite(play_b_inactive);
  destroy_sprite(draw_b_inactive);
  destroy_sprite(exit_b_inactive);
  destroy_sprite(african_avatar);
  destroy_sprite(arab_avatar);
  destroy_sprite(batman_avatar);
  destroy_sprite(dalai_avatar);
  destroy_sprite(gandhi_avatar);
  destroy_sprite(indian_avatar);
  destroy_sprite(lenin_avatar);
  destroy_sprite(obama_avatar);
  destroy_sprite(robot_avatar);
  destroy_sprite(ronaldo_avatar);
  destroy_sprite(stalin_avatar);
  destroy_sprite(trump_avatar);
  destroy_sprite(canvas);
  destroy_sprite(draw_canvas);
  destroy_sprite(grid);
  destroy_sprite(trash_b);
  destroy_sprite(bucket_b);
  destroy_sprite(pencil_b);
  destroy_sprite(pen_b);
  destroy_sprite(eraser_b);
  destroy_sprite(spray_b);
  destroy_sprite(pallete_color);
  destroy_sprite(pallete_grey);
  destroy_sprite(return_b);
  destroy_sprite(date_slot);
  destroy_sprite(pencil);
  destroy_sprite(ink);
  destroy_sprite(pen);
  destroy_sprite(tip);
  destroy_sprite(white);
  destroy_sprite(bucket);
  destroy_sprite(eraser);
  destroy_sprite(spray);
  destroy_sprite(main_menu);
  destroy_sprite(ink_splatters);
  destroy_sprite(victory_board);
  destroy_sprite(winner_frame);
  destroy_sprite(red_balloon);
  destroy_sprite(green_balloon);
  destroy_sprite(yellow_balloon);
  destroy_sprite(orange_balloon);
  destroy_sprite(blue_balloon);
  destroy_sprite(pink_balloon);
  destroy_sprite(timer_slot);
}

void set_up_cursor() {
  // Creates the default cursor sprite in default position
  cursor = create_sprite(mouse_pointer, 50, 50, 0, 0);

  // Sets the current cursor to be the default one
  current = cursor;

  draw_cursor(current, color);

  // Initializes cursor positions
  curr_state.x_pos = 50;
  curr_state.x_prev = 50;
  curr_state.y_pos = 50;
  curr_state.y_prev = 50;
}

void set_up_cursors() {
  pencil = create_sprite(brush_pointer, 0, 0, 0, 0);

  ink = create_sprite(brush_pointer, 0, 0, 0, 0);

  pen = create_sprite(pen_pointer, 0, 0, 0, 0);

  tip = create_sprite(pen_pointer, 0, 0, 0, 0);

  white = create_sprite(white_ink, 0, 0, 0, 0);

  bucket = create_sprite(fill_cursor, 0, 0, 0, 0);

  eraser = create_sprite(eraser_pointer, 0, 0, 0, 0);

  spray = create_sprite(spray_cursor, 0, 0, 0, 0);
}

void set_up_main_menu() {

  // Initializes seed for random number generator
  time_t t;
  srand((unsigned) time(&t));

  // Initializes screen and sets it to the background//

  set_up_sprites();

  draw_Sprite(main_menu);

  draw_Sprite(ink_splatters);

  draw_menu_buttons();

  draw_Sprite(date_icon);

  // Sets up default cursor
  set_up_cursor();

  double_buffer();
}

void set_up_player_menu() {
  draw_Sprite(set_up_menu);

  draw_avatars();

  draw_Sprite(ready_b_inactive_1);
  draw_Sprite(ready_b_inactive_2);

  draw_Sprite(date_icon);

  double_buffer();
}

void set_up_paint_menu() {

  draw_Sprite(main_menu);

  draw_paint_buttons();

  draw_Sprite(date_icon);

  double_buffer();
}

void set_up_victory_menu() {
  draw_Sprite(victory_board);
  draw_Sprite(winner_frame);

  draw_Sprite(red_balloon);
  draw_Sprite(green_balloon);
  draw_Sprite(yellow_balloon);
  draw_Sprite(orange_balloon);
  draw_Sprite(blue_balloon);
  draw_Sprite(pink_balloon);
}

void set_up_words() {

  get_words();

  draw_Sprite(word_header);

  draw_string(words[0], 257, 270);
  draw_string(words[1], 436, 270);
  draw_string(words[2], 614, 270);
}

void draw_menu_buttons() {
  draw_Sprite(play_b_inactive);
  draw_Sprite(draw_b_inactive);
  draw_Sprite(exit_b_inactive);
}

void draw_avatars() {

  // Resets the avatars to there default position
  change_position(african_avatar, 631, 232);
  change_position(arab_avatar, 741, 232);
  change_position(batman_avatar, 851, 232);
  change_position(dalai_avatar, 631, 342);
  change_position(gandhi_avatar, 741, 342);
  change_position(indian_avatar, 851, 342);
  change_position(lenin_avatar, 631, 452);
  change_position(obama_avatar, 741, 452);
  change_position(robot_avatar, 851, 452);
  change_position(ronaldo_avatar, 631, 562);
  change_position(stalin_avatar, 741, 562);
  change_position(trump_avatar, 851, 562);

  // Draws the sprites in the grid
  draw_Sprite(african_avatar);
  draw_Sprite(arab_avatar);
  draw_Sprite(batman_avatar);
  draw_Sprite(dalai_avatar);
  draw_Sprite(gandhi_avatar);
  draw_Sprite(indian_avatar);
  draw_Sprite(lenin_avatar);
  draw_Sprite(obama_avatar);
  draw_Sprite(robot_avatar);
  draw_Sprite(ronaldo_avatar);
  draw_Sprite(stalin_avatar);
  draw_Sprite(trump_avatar);
}

void draw_paint_buttons() {
  // Draws the buttons that are painting related
  if (curr_state.state == DRAW)
    draw_Sprite(draw_canvas);
  else
    draw_Sprite(canvas);

  draw_Sprite(grid);
  draw_Sprite(trash_b);
  draw_Sprite(bucket_b);
  draw_Sprite(pencil_b);
  draw_Sprite(pen_b);
  draw_Sprite(eraser_b);
  draw_Sprite(spray_b);
  draw_Sprite(pallete_color);
  draw_Sprite(pallete_grey);
  draw_Sprite(return_b);

  if (curr_state.state == PLAY) {
    draw_Sprite(stats);
    draw_Sprite(p1->avatar);
    draw_Sprite(p2->avatar);
    if (counter % 2 == 0) {
      char str[6] = {0};
      sprintf(str, "%04d", p1->points);
      draw_string(str, 55, 366);
      sprintf(str, "%04d", p2->points);
      draw_string(str, 55, 593);
    }
    else {
      char str[6] = {0};
      sprintf(str, "%04d", p2->points);
      draw_string(str, 55, 366);
      sprintf(str, "%04d", p1->points);
      draw_string(str, 55, 593);
    }
  }
}

void move_balloons() {
  // Draws static sprites
  draw_Sprite(victory_board);
  draw_Sprite(winner_frame);
  draw_Sprite(p1->avatar);
  draw_string(winner_name, 442, 337);

  // Updates every balloons position (adds their speed)
  move_xpm(red_balloon);
  move_xpm(green_balloon);
  move_xpm(yellow_balloon);
  move_xpm(orange_balloon);
  move_xpm(blue_balloon);
  move_xpm(pink_balloon);

  // Checks if any collisions happened
  check_collisions();

  // Draws all balloon sprites
  draw_Sprite(red_balloon);
  draw_Sprite(green_balloon);
  draw_Sprite(yellow_balloon);
  draw_Sprite(orange_balloon);
  draw_Sprite(blue_balloon);
  draw_Sprite(pink_balloon);
}

void check_collisions() {

  // Checks every pair of balloons for collisions between themselves
  detect_collision_pair(red_balloon, green_balloon);
  detect_collision_pair(red_balloon, yellow_balloon);
  detect_collision_pair(red_balloon, orange_balloon);
  detect_collision_pair(red_balloon, blue_balloon);
  detect_collision_pair(red_balloon, pink_balloon);
  detect_collision_pair(green_balloon, yellow_balloon);
  detect_collision_pair(green_balloon, orange_balloon);
  detect_collision_pair(green_balloon, blue_balloon);
  detect_collision_pair(green_balloon, pink_balloon);
  detect_collision_pair(yellow_balloon, orange_balloon);
  detect_collision_pair(yellow_balloon, blue_balloon);
  detect_collision_pair(yellow_balloon, pink_balloon);
  detect_collision_pair(orange_balloon, blue_balloon);
  detect_collision_pair(orange_balloon, pink_balloon);
  detect_collision_pair(blue_balloon, pink_balloon);

  // Checks every pair for collisions with the screen limits
  detect_collision(red_balloon);
  detect_collision(green_balloon);
  detect_collision(yellow_balloon);
  detect_collision(orange_balloon);
  detect_collision(blue_balloon);
  detect_collision(pink_balloon);
}

void detect_collision(Sprite *sp) {
  bool x_collision = false;
  bool y_collision = false;

  // Checks collisions with the left and right sides of the screen
  if (sp->x < 0 || sp->x + sp->width > 1024) {
    x_collision = true;
    sp->x += -sp->xspeed;
  }

  // Checks collisions with the upper and bottom sides of the screen
  if (sp->y < 0 || sp->y + sp->height > 768) {
    y_collision = true;
    sp->y += -sp->yspeed;
  }

  // If any collisions happened corrects the speed of the sprite
  if (x_collision)
    sp->xspeed = -sp->xspeed;

  if (y_collision)
    sp->yspeed = -sp->yspeed;
}

void detect_collision_pair(Sprite *s, Sprite *p) {
  bool x_collision = false;
  bool y_collision = false;

  // Checks if the collision was horizontal
  if ((s->x >= p->x && s->x <= p->x + p->width) || (p->x >= s->x && p->x <= s->x + s->width)) {
    if ((s->y >= p->y && s->y <= p->y + p->height) || (p->y >= s->y && p->y <= s->y + s->height)) {
      x_collision = true;
      s->x += -s->xspeed;
      p->x += -p->xspeed;
    }
  }

  // Checks if the collision was vertical
  if ((s->y >= p->y && s->y <= p->y + p->height) || (p->y > s->y && p->y <= s->y + s->height)) {
    if ((s->x >= p->x && s->x <= p->x + p->width) || (p->x > s->x && p->x <= s->x + s->width)) {
      y_collision = true;
      if (x_collision) {
        x_collision = false;
        s->x += s->xspeed;
        p->x += p->xspeed;
        s->y += -s->yspeed;
        p->y += -p->yspeed;
      }
    }
  }

  // If there are any collisions updates the speed accordingly

  if (x_collision) {
    s->xspeed = -s->xspeed;
    p->xspeed = -p->xspeed;
  }

  if (y_collision) {
    s->yspeed = -s->yspeed;
    p->yspeed = -p->yspeed;
  }
}

void check_rtc() {
  // If the user does not have the cursor on the date icon and the date is showing then it erases the date. If the user does have the mouse pointer on the date icon but the date is not showing then it draws the date
  if (rtc_active && !button_hovering(date_icon, curr_state.x_pos, curr_state.y_pos)) {
    erase_dynamic(date_slot, background);
    rtc_active = false;
  }
  else if (!rtc_active && button_hovering(date_icon, curr_state.x_pos, curr_state.y_pos)) {
    rtc_active = true;
    update_rtc();
  }
}

void check_words() {
  if (button_hovering(word_header, curr_state.x_pos, curr_state.y_pos))
    if (curr_state.y_pos > 254 && curr_state.y_pos < 314) {
      // Checks if user selected word on the left, middle or right position
      if (curr_state.x_pos > 251 && curr_state.x_pos < 420) {
        word_choosen = true;
        set_word(words[0]);
      }
      else if (curr_state.x_pos > 428 && curr_state.x_pos < 597) {
        word_choosen = true;
        set_word(words[1]);
      }
      else if (curr_state.x_pos > 605 && curr_state.x_pos < 774) {
        word_choosen = true;
        set_word(words[2]);
      }
    }
}

void check_released() {
  if (curr_state.mouse.lb == RELEASED)
    curr_state.mouse.lb = IGNORE;

  if (curr_state.mouse.mb == RELEASED)
    curr_state.mouse.mb = IGNORE;

  if (curr_state.mouse.rb == RELEASED)
    curr_state.mouse.rb = IGNORE;
}

void check_avatar() {

  // Divides the avatar grid in four lines and checks which one the cursor is in and then only checks the avatars in the giving line to avoid always checking every avatar

  if (curr_state.y_pos < 335) {
    if (button_hovering(african_avatar, curr_state.x_pos, curr_state.y_pos)) {
      (curr_state.state == SETUP_PLAYER_1) ? toggle_avatar(african_avatar, toggle_1) : toggle_avatar(african_avatar, toggle_2);
    }
    else if (button_hovering(arab_avatar, curr_state.x_pos, curr_state.y_pos)) {
      (curr_state.state == SETUP_PLAYER_1) ? toggle_avatar(arab_avatar, toggle_1) : toggle_avatar(arab_avatar, toggle_2);
    }
    else if (button_hovering(batman_avatar, curr_state.x_pos, curr_state.y_pos)) {
      (curr_state.state == SETUP_PLAYER_1) ? toggle_avatar(batman_avatar, toggle_1) : toggle_avatar(batman_avatar, toggle_2);
    }
  }
  else if (curr_state.y_pos < 445) {
    if (button_hovering(dalai_avatar, curr_state.x_pos, curr_state.y_pos)) {
      (curr_state.state == SETUP_PLAYER_1) ? toggle_avatar(dalai_avatar, toggle_1) : toggle_avatar(dalai_avatar, toggle_2);
    }
    else if (button_hovering(gandhi_avatar, curr_state.x_pos, curr_state.y_pos)) {
      (curr_state.state == SETUP_PLAYER_1) ? toggle_avatar(gandhi_avatar, toggle_1) : toggle_avatar(gandhi_avatar, toggle_2);
    }
    else if (button_hovering(indian_avatar, curr_state.x_pos, curr_state.y_pos)) {
      (curr_state.state == SETUP_PLAYER_1) ? toggle_avatar(indian_avatar, toggle_1) : toggle_avatar(indian_avatar, toggle_2);
    }
  }
  else if (curr_state.y_pos < 555) {
    if (button_hovering(lenin_avatar, curr_state.x_pos, curr_state.y_pos)) {
      (curr_state.state == SETUP_PLAYER_1) ? toggle_avatar(lenin_avatar, toggle_1) : toggle_avatar(lenin_avatar, toggle_2);
    }
    else if (button_hovering(obama_avatar, curr_state.x_pos, curr_state.y_pos)) {
      (curr_state.state == SETUP_PLAYER_1) ? toggle_avatar(obama_avatar, toggle_1) : toggle_avatar(obama_avatar, toggle_2);
    }
    else if (button_hovering(robot_avatar, curr_state.x_pos, curr_state.y_pos)) {
      (curr_state.state == SETUP_PLAYER_1) ? toggle_avatar(robot_avatar, toggle_1) : toggle_avatar(robot_avatar, toggle_2);
    }
  }
  else {
    if (button_hovering(ronaldo_avatar, curr_state.x_pos, curr_state.y_pos)) {
      (curr_state.state == SETUP_PLAYER_1) ? toggle_avatar(ronaldo_avatar, toggle_1) : toggle_avatar(ronaldo_avatar, toggle_2);
    }
    else if (button_hovering(stalin_avatar, curr_state.x_pos, curr_state.y_pos)) {
      (curr_state.state == SETUP_PLAYER_1) ? toggle_avatar(stalin_avatar, toggle_1) : toggle_avatar(stalin_avatar, toggle_2);
    }
    else if (button_hovering(trump_avatar, curr_state.x_pos, curr_state.y_pos)) {
      (curr_state.state == SETUP_PLAYER_1) ? toggle_avatar(trump_avatar, toggle_1) : toggle_avatar(trump_avatar, toggle_2);
    }
  }
}

void toggle_avatar(Sprite *avatar, Sprite *toggle) {

  // Checks if any other avatars were toggle and if so it erases previous toggle
  if (toggle->x != 0 && toggle->y != 0) {
    erase_dynamic(toggle, setup_menu);
    draw_avatars();

    if (curr_state.state == SETUP_PLAYER_2) {
      draw_Sprite(toggle_1);
    }
  }

  // Finds correct position for the toggle
  change_position(toggle, avatar->x - 5, avatar->y - 4);

  if (curr_state.state == SETUP_PLAYER_1) {
    set_avatar(p1, avatar);
    draw_Sprite(toggle);
    avatar_choosen = true;
  }
  else if (curr_state.state == SETUP_PLAYER_2) {
    // Checks if the player 2 selected the same avatar as player 1 by checking if the selected avatar is toggled
    int toggle_color = get_color(avatar->x - 4, avatar->y);
    if (toggle_color != 0x8ED858) {
      set_avatar(p2, avatar);
      draw_Sprite(toggle);
      avatar_choosen = true;
    }
    else {
      avatar_choosen = false;
    }
  }
}

void paint() {

  // Checks if their was just a button click or if the button is being pressed
  if (curr_state.mouse.lb == PRESSED) {

    if (curr_state.keyboard.key == PRESS && curr_state.keyboard.key_code[0] == 0x13)
      loop_color(&color);

    if (curr_state.paint_mode == PENCIL) {
      update_ink(ink);
      fill_displacement(ink, delta_x, delta_y, color);
    }
    else if (curr_state.paint_mode == PEN) {
      update_ink(tip);
      fill_displacement(tip, delta_x, delta_y, color);
    }
    else if (curr_state.paint_mode == ERASER) {
      update_ink(white);
      fill_displacement(white, delta_x, delta_y, 0xFFFFFF);
    }
    else if (curr_state.paint_mode == SPRAY) {
      update_delta();
      spray_area(curr_state.x_prev, curr_state.y_prev, delta_x, delta_y, color);
    }
  }
  else if (curr_state.mouse.lb == RELEASED) {

    if (curr_state.keyboard.key == PRESS && curr_state.keyboard.key_code[0] == 0x13)
      loop_color(&color);

    if (curr_state.paint_mode == PENCIL) {
      update_ink(ink);
      paint_Sprite(ink, color);
    }
    else if (curr_state.paint_mode == PEN) {
      update_ink(tip);
      fill_displacement(tip, delta_x, delta_y, color);
    }
    else if (curr_state.paint_mode == ERASER) {
      update_ink(white);
      draw_Sprite(white);
    }
    else if (curr_state.paint_mode == BUCKET) {
      fill_area(curr_state.x_pos + 7, curr_state.y_pos, color);
    }
    else if (curr_state.paint_mode == SPRAY) {
      update_delta();
      spray_area(curr_state.x_pos, curr_state.y_pos, delta_x, delta_y, color);
    }
  }
}

void update_rtc() {
  if (!rtc_is_updating() && rtc_active) {
    get_time_string(time_string);
    draw_Sprite(date_slot);
    draw_string(time_string, 58, 731);
  }
}

void update_chrono() {
  // Displays the current time in the chronometer
  draw_Sprite(timer_slot);
  sprintf(timer, "%02d", chronometer);
  draw_string(timer, 72, 128);

  // Updates chronometer
  chronometer -= 1;
  second_passed = false;
}

void update_delta() {
  delta_x = curr_state.x_pos - curr_state.x_prev;
  delta_y = curr_state.y_pos - curr_state.y_prev;
}

void update_cursor(Sprite *sprite) {

  current = sprite;

  // Adjusts the speeds to make sure the cursor doesn't pass window limits
  speed_x = calc_speed_x(current, speed_x);
  speed_y = calc_speed_y(current, speed_y);

  // Makes sure the cursor sprite has the correct position,not guaranted when there is a change in the cursor (ex. pointer -> brush)
  current->x = curr_state.x_pos;
  current->y = curr_state.y_pos;

  // Updates previous and current mouse position
  curr_state.x_prev = curr_state.x_pos;
  curr_state.y_prev = curr_state.y_pos;

  curr_state.x_pos += speed_x;
  curr_state.y_pos += speed_y;

  move_cursor(current, speed_x, speed_y, color);

  // Resets the speed
  speed_x = 0;
  speed_y = 0;
}

void update_ink(Sprite *sp) {
  // Updates the position of sprite ink

  update_delta();

  sp->x = curr_state.x_prev;
  sp->y = curr_state.y_prev;
}

void update_paint_type() {
  if (curr_state.mouse.lb == RELEASED) {

    if (button_hovering(pencil_b, curr_state.x_pos, curr_state.y_pos)) {
      brush_type = pencil;

      curr_state.paint_mode = PENCIL;
    }

    if (button_hovering(pen_b, curr_state.x_pos, curr_state.y_pos)) {
      brush_type = pen;

      curr_state.paint_mode = PEN;
    }

    if (button_hovering(eraser_b, curr_state.x_pos, curr_state.y_pos)) {
      brush_type = eraser;

      curr_state.paint_mode = ERASER;
    }

    if (button_hovering(spray_b, curr_state.x_pos, curr_state.y_pos)) {
      brush_type = spray;

      curr_state.paint_mode = SPRAY;
    }

    if (button_hovering(bucket_b, curr_state.x_pos, curr_state.y_pos)) {
      brush_type = bucket;

      curr_state.paint_mode = BUCKET;
    }

    if (button_hovering(trash_b, curr_state.x_pos, curr_state.y_pos)) {
      // To clear canvas just redraws canvas sprite
      if (curr_state.state == PLAY)
        draw_Sprite(canvas);
      else if (curr_state.state == DRAW)
        draw_Sprite(draw_canvas);
    }
  }
}

void update_color() {
  if (pallete_hovering(pallete_color, curr_state.x_pos, curr_state.y_pos) && curr_state.mouse.lb == RELEASE) {
    color = get_color(curr_state.x_pos, curr_state.y_pos);
  }

  if (pallete_hovering(pallete_grey, curr_state.x_pos, curr_state.y_pos) && curr_state.mouse.lb == RELEASE) {
    color = get_color(curr_state.x_pos, curr_state.y_pos);
  }
}

void update_mouse_event(struct packet *packet_data) {

  mouse_events = false;

  switch (curr_state.mouse.mb) {
    case IGNORE:
      if (packet_data->mb) {
        curr_state.mouse.mb = PRESSED;
        mouse_events = true;
      }
      else
        curr_state.mouse.mb = IGNORE;
      break;

    case PRESSED:
      if (packet_data->mb)
        curr_state.mouse.mb = PRESSED;
      else {
        curr_state.mouse.mb = RELEASED;
        mouse_events = true;
      }
      break;

    case RELEASED:
      if (packet_data->mb) {
        curr_state.mouse.mb = PRESSED;
        mouse_events = true;
      }
      else
        curr_state.mouse.mb = IGNORE;
      break;
  }

  switch (curr_state.mouse.rb) {
    case IGNORE:
      if (packet_data->rb) {
        curr_state.mouse.rb = PRESSED;
        mouse_events = true;
      }
      else
        curr_state.mouse.rb = IGNORE;
      break;

    case PRESSED:
      if (packet_data->rb)
        curr_state.mouse.rb = PRESSED;
      else {
        curr_state.mouse.rb = RELEASED;
        mouse_events = true;
      }
      break;

    case RELEASED:
      if (packet_data->rb) {
        curr_state.mouse.rb = PRESSED;
        mouse_events = true;
      }
      else
        curr_state.mouse.rb = IGNORE;
      break;
  }

  switch (curr_state.mouse.lb) {
    case IGNORE:
      if (packet_data->lb) {
        curr_state.mouse.lb = PRESSED;
        mouse_events = true;
      }
      else
        curr_state.mouse.lb = IGNORE;
      break;

    case PRESSED:
      if (packet_data->lb)
        curr_state.mouse.lb = PRESSED;
      else {
        curr_state.mouse.lb = RELEASED;
        mouse_events = true;
      }
      break;

    case RELEASED:
      if (packet_data->lb) {
        curr_state.mouse.lb = PRESSED;
        mouse_events = true;
      }
      else
        curr_state.mouse.lb = IGNORE;
      break;
  }

  if (x_sign)
    speed_x += complement_to_decimal(packet_data->delta_x);
  else
    speed_x += -complement_to_decimal(packet_data->delta_x);

  if (y_sign)
    speed_y += -complement_to_decimal(packet_data->delta_y);
  else
    speed_y += complement_to_decimal(packet_data->delta_y);
}

void update_keyboard_event(uint8_t scancode[]) {
  curr_state.keyboard.key_code[1] = scancode[1];
  curr_state.keyboard.key_code[0] = scancode[0];
  // determine if the key was PRESSED or RELEASED
  curr_state.keyboard.key = (scancode[0] & BIT(7)) ? RELEASE : PRESS;

  char tmp;
  if (scancode[0] == 0xe0) {
    tmp = key_handler(scancode[1]);
  }
  else {
    tmp = key_handler(scancode[0]);
  }

  static bool got_string = false; // False until the string is complete
                                  // used to stop the player to write over his own name
  static bool first_run = true;   // True for only the first run after the main menu

  if (curr_state.state == SETUP_PLAYER_1) {

    if (!got_string) {
      write_string(tmp, 60, 318, 0xFFFFFF); // writes player 1 name on the screen
    }

    if (tmp == '\n') { // once the ENTER key is pressed the string is complete
      got_string = true;
    }
  }
  else if (curr_state.state == SETUP_PLAYER_2) {
    if (first_run) { // if it is the first run
      first_run = false;
      got_string = false; // clear the flag so player 2 may write his name
    }

    if (!got_string) {
      write_string(tmp, 60, 552, 0xFFFFFF); // writes player 2 name on the screen
    }

    if (tmp == '\n') { // once the ENTER key is pressed the string is complete
      got_string = true;
    }
  }
  else if (curr_state.state == GUESS) {
    write_string(tmp, 353, 718, 0xFFFFFF);
  }
  else if (curr_state.state == INITIAL_MENU) { // everytime the program returns to the main menu
    got_string = false;                        // reset the flags
    first_run = true;
  }
}

void update_state() {

  switch (curr_state.state) {

    case INITIAL_MENU:

      update_cursor(current);

      // Checks if the mouse is hovering the play button

      if (button_hovering(play_b_inactive, curr_state.x_pos, curr_state.y_pos)) {
        if (!active_play) {
          draw_Sprite(play_b_active);
          active_play = true;
        }
      }
      else if (active_play) {
        draw_Sprite(play_b_inactive);
        active_play = false;
      }

      // Checks if the mouse is hovering the draw button
      if (button_hovering(draw_b_inactive, curr_state.x_pos, curr_state.y_pos)) {
        if (!active_draw) {
          draw_Sprite(draw_b_active);
          active_draw = true;
        }
      }
      else if (active_draw) {
        draw_Sprite(draw_b_inactive);
        active_draw = false;
      }

      // Checks if the mouse is hovering the exit button
      if (button_hovering(exit_b_inactive, curr_state.x_pos, curr_state.y_pos)) {
        if (!active_exit) {
          draw_Sprite(exit_b_active);
          active_exit = true;
        }
      }
      else if (active_exit) {
        draw_Sprite(exit_b_inactive);
        active_exit = false;
      }

      // Checks if when the mouse button was clicked was within the limits
      if (mouse_events){
        if (curr_state.mouse.lb == RELEASED) {

          // Clicked on the PLAY button
          if (button_hovering(play_b_inactive, curr_state.x_pos, curr_state.y_pos)) {

            set_up_player_menu();

            uint8_t code[2] = {0}; // simulates a key press so the keyboard flags are reset
            update_keyboard_event(code);

            color = 0;

            curr_state.state = SETUP_PLAYER_1;

            set_up_cursors();
          }

          // Clicked on the DRAW button
          if (button_hovering(draw_b_active, curr_state.x_pos, curr_state.y_pos)) {
            curr_state.state = DRAW;
            curr_state.paint_mode = PENCIL;

            set_up_paint_menu();

            set_up_cursors();

            // Sets the initial in canvas cursor to be normal pencil;
            brush_type = pencil;
          }
        } else if(button_hovering(exit_b_active, curr_state.x_pos, curr_state.y_pos) && curr_state.mouse.mb == RELEASED) {
            exit_program = true;
        }
      }

      update_cursor(current);

      double_buffer();

      break;

    case PLAY:

      // if the ENTER key is pressed, the player will stop drawing and the other player will guess
      if (curr_state.keyboard.key == PRESS && curr_state.keyboard.key_code[0] == 0x1C) {
        chronometer = 0;
      }

      if (chronometer == 0) {
        update_cursor(cursor);
        curr_state.state = GUESS;
        chronometer = 30;
        draw_Sprite(guess_slot);
        break;
      }

      if (in_canvas(canvas, current, curr_state.x_pos, curr_state.y_pos) && in_canvas(canvas, current, curr_state.x_prev, curr_state.y_prev)) {
        erase_cursor(current);

        current = brush_type;

        paint();

        update_cursor(current);
      }
      else {
        erase_cursor(current);

        current = cursor;

        update_cursor(current);
      }

      if (mouse_events) {
        // Checks if any buttons were pressed
        update_paint_type();

        // Checks if color has been changed
        update_color();

        // Checks if player wants to return to main menu
        if (curr_state.mouse.rb == RELEASED && button_hovering(return_b, curr_state.x_pos, curr_state.y_pos)) {

          // deletes both players and sets the round counter to zero
          delete_player(p1);
          delete_player(p2);
          counter = 0;

          draw_Sprite(main_menu);

          draw_Sprite(ink_splatters);

          draw_menu_buttons();

          draw_Sprite(date_icon);

          curr_state.state = INITIAL_MENU;

          double_buffer();
        }
      }

      if (second_passed)
        update_chrono();

      double_buffer();

      break;

    case SETUP_PLAYER_1:
      update_cursor(current);

      static bool got_string = false;
      static bool first_setup = true;

      // creates an empty player on the first run
      if (first_setup) {
        p1 = create_empty_player();
        first_setup = false;
      }

      // cursor is hovering the READY button
      if (button_hovering(ready_b_inactive_1, curr_state.x_pos, curr_state.y_pos)) {
        if (!active_ready) {
          draw_Sprite(ready_b_active_1);
          active_ready = true;
        }
      }
      else if (active_ready) {
        draw_Sprite(ready_b_inactive_1);
        active_ready = false;
      }
      if (mouse_events)
        if (curr_state.mouse.lb == RELEASED) {

          // Player clicked on an avatar
          if (curr_state.x_pos > 629 && curr_state.x_pos < 953)
            if (curr_state.y_pos > 231 && curr_state.y_pos < 665) {
              check_avatar();
            }

          // Clicked the READY button
          if (button_hovering(ready_b_active_1, curr_state.x_pos, curr_state.y_pos) && avatar_choosen && got_string) {
            active_ready = false;
            avatar_choosen = false;
            got_string = false;
            first_setup = true;

            curr_state.state = SETUP_PLAYER_2;
          }
        }

      // if the player has finished typing is name
      if (complete_string && !got_string) {
        set_name(p1, resulting_string);
        clear_string();
        got_string = true;
      }

      update_cursor(current);

      double_buffer();
      break;

    case SETUP_PLAYER_2:
      update_cursor(current);

      // creates an empty player on the first run
      if (first_setup) {
        p2 = create_empty_player();
        first_setup = false;
      }

      // cursor is hovering the READY button
      if (button_hovering(ready_b_inactive_2, curr_state.x_pos, curr_state.y_pos)) {
        if (!active_ready) {
          draw_Sprite(ready_b_active_2);
          active_ready = true;
        }
      }
      else if (active_ready) {
        draw_Sprite(ready_b_inactive_2);
        active_ready = false;
      }

      if (mouse_events)
        if (curr_state.mouse.lb == RELEASED) {
          // player clicked on an avatar
          if (curr_state.x_pos > 629 && curr_state.x_pos < 953)
            if (curr_state.y_pos > 231 && curr_state.y_pos < 665) {
              check_avatar();
            }

          // clicked the READY button
          if (button_hovering(ready_b_active_2, curr_state.x_pos, curr_state.y_pos) && avatar_choosen && got_string) {
            active_ready = false;
            avatar_choosen = false;
            got_string = false;
            first_setup = true;

            set_up_paint_menu();

            set_up_words(); // get three random words for the player to choose one from

            /* only now are the positions of the avatars changed, because this way we eliminate the
               necessity of having duplicate sprites */
            change_position(p1->avatar, 35, 256);
            change_position(p2->avatar, 35, 483);

            curr_state.state = CHOOSE_WORD;
          }
        }

      // player has finished typing his name
      if (complete_string && !got_string) {
        set_name(p2, resulting_string);
        clear_string();
        got_string = true;
      }

      update_cursor(current);

      double_buffer();
      break;

    case CHOOSE_WORD:

      update_cursor(current);

      // gets the word chosen by the player
      if (curr_state.mouse.lb == RELEASED) {
        check_words();
      }

      // once the word has been chosen
      if (word_choosen) {

        chronometer = 60;

        curr_state.state = PLAY;
        curr_state.paint_mode = PENCIL;

        set_up_paint_menu();

        // Sets the initial in canvas cursor to be normal pencil;
        brush_type = pencil;

        update_cursor(brush_type);

        word_choosen = false;
      }

      update_cursor(current);

      double_buffer();
      break;

    case GUESS: {
      static bool guessed_word = false; // true when the guess is correct
      static int time_points = 100;

      // every second the players' points are decremented
      if (second_passed) {
        update_chrono();
        time_points -= 1;
      }

      // once the ENTER key is pressed
      if (complete_string) {
        // if the guess is correct
        if (strcmp(resulting_string, chosen_word) == 0) {
          chronometer = 0;     //
          guessed_word = true; // set the flag to true
          clear_string();      // clear the string flags
        }
        // if the guess is incorrect
        else {
          time_points -= 10; // decrement the guessing player's points
          draw_Sprite(guess_slot);
        }
      }

      if (chronometer == 0) {

        if (guessed_word) { // if the word was guessed
          tmp_points_p1 += 150 + ((time_points > 0) ? time_points : 0);
          tmp_points_p2 += 150; // increment the drawing player's points
        }

        // update both the players' scores
        update_points(p1, tmp_points_p1);
        update_points(p2, tmp_points_p2);

        // switch the players for the next round
        player *tmp_ptr = p1;
        p1 = p2;
        p2 = tmp_ptr;

        time_points = 100;
        chronometer = 3;
        curr_state.state = POINTS;
        clear_string();
        guessed_word = false;
        counter++;
      }

      update_cursor(current);

      double_buffer();
      break;
    }
    case POINTS:

      // on the first run
      if (chronometer == 3) {
        char p1_str[6] = {0};
        char p2_str[6] = {0};

        // create strings to display the players' scores of the round
        if (tmp_points_p1 >= 0)
          sprintf(p1_str, "+%03d", tmp_points_p1);
        else
          sprintf(p1_str, "%03d", tmp_points_p1);

        if (tmp_points_p2 >= 0)
          sprintf(p2_str, "+%03d", tmp_points_p2);
        else
          sprintf(p2_str, "%03d", tmp_points_p2);

        /* because the players are switched each round, in order to make sure that the scores are
           displayed on the correct location we implemented a round counter. If the counter is an 
           even value the points will be displayed on one location, if it is odd the locations 
           are switched */
        if (counter % 2 == 0) {
          draw_string(p2_str, 55, 402);
          draw_string(p1_str, 55, 628);
        }
        else {
          draw_string(p1_str, 55, 402);
          draw_string(p2_str, 55, 628);
        }
      }

      if (second_passed) {
        chronometer--;
        second_passed = false;
      }

      if (chronometer == 0) {

        tmp_points_p1 = tmp_points_p2 = 0; // reset the values

        if (counter > 5) { // once 6 rounds have been played

          if (p1->points < p2->points) { // determine the player with the most points
            delete_player(p1);
            p1 = p2;
          }
          else
            delete_player(p2);

          counter = 0; // reset the round counter

          change_position(p1->avatar, 462, 380); // change the position for the victory screen

          sprintf(winner_name, "%s", p1->name); // get the winner's name

          set_up_victory_menu();
          curr_state.state = END;
          alarm_rang = false;

          rtc_set_alarm(10);
        }
        else { // sets up the next round
          chronometer = 60;

          curr_state.state = CHOOSE_WORD;

          set_up_paint_menu();
          set_up_words();
        }
      }

      update_cursor(current);

      double_buffer();

      break;

    case END:

      move_balloons();

      // When the RTC alarm interruption is read, exit screen
      if (alarm_rang) {
        delete_player(p1); // delete the remaining player

        draw_Sprite(main_menu);
        draw_Sprite(ink_splatters);
        draw_menu_buttons();
        draw_Sprite(date_icon);
        curr_state.state = INITIAL_MENU;
        double_buffer();
      }

      double_buffer();

      break;

    case DRAW:

      if (in_canvas(draw_canvas, current, curr_state.x_pos, curr_state.y_pos) && in_canvas(draw_canvas, current, curr_state.x_prev, curr_state.y_prev)) {
        erase_cursor(current);

        current = brush_type;

        paint();

        update_cursor(current);
      }
      else {
        erase_cursor(current);

        current = cursor;

        update_cursor(current);
      }

      if (mouse_events) {
        // Checks if any buttons were pressed
        update_paint_type();

        // Checks if color has been changed
        update_color();

        // Checks if player wants to return to main menu
        if (curr_state.mouse.rb == RELEASED && button_hovering(return_b, curr_state.x_pos, curr_state.y_pos)) {
          draw_Sprite(main_menu);

          draw_Sprite(ink_splatters);

          draw_menu_buttons();

          draw_Sprite(date_icon);

          curr_state.state = INITIAL_MENU;

          double_buffer();
        }
      }

      double_buffer();

      break;

    default:
      break;
  }

  check_rtc();

  check_released();
}
