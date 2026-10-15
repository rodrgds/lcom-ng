#include "view.h"

uint8_t *main_frame_buffer;
uint8_t *secondary_frame_buffer;
uint8_t *drawing_frame_buffer;
uint32_t frame_buffer_size;
extern int timer_interrupts;
extern vbe_mode_info_t mode_info;
extern MouseInfo mouse_info;
extern real_time_info time_info;
extern MenuState menuState;

extern Sprite *mouse;
extern Sprite *mouse_click;
extern Sprite *smile;

extern Sprite *gameName;
extern Sprite *playButton;
extern Sprite *multiplayerButton;
extern Sprite *helpButton;
extern Sprite *quitButton;
extern Sprite *resumeButton;
extern Sprite *againButton;
extern Sprite *menuButton;
extern Sprite *gameOver;
extern Sprite *gamePaused;
extern Sprite *purpleWon;
extern Sprite *orangeWon;
extern Sprite *mountain1;
extern Sprite *mountain2;
extern Sprite *mountain3;
extern Sprite *mountain4;
extern Sprite *towerO0;
extern Sprite *towerO1;
extern Sprite *towerO2;
extern Sprite *towerO3;
extern Sprite *towerO4;
extern Sprite *towerO5;
extern Sprite *towerO6;
extern Sprite *towerP0;
extern Sprite *towerP1;
extern Sprite *towerP2;
extern Sprite *towerP3;
extern Sprite *towerP4;
extern Sprite *towerP5;
extern Sprite *towerP6;
extern Sprite *cloud1;
extern Sprite *cloud2;
extern Sprite *cloud3;
extern Sprite *cloud4;
extern Sprite *cloud5;
extern Sprite *cloud6;
extern Sprite *cloud7;
extern Sprite *cloud8;
extern Sprite *healthBar0;
extern Sprite *healthBar20;
extern Sprite *healthBar40;
extern Sprite *healthBar60;
extern Sprite *healthBar80;
extern Sprite *healthBar100;
extern Sprite *barV[51];
extern Sprite *cannon_ball;
extern Sprite *cannon_ball2;
extern Sprite *helpPage1;
extern Sprite *helpPage2;
extern Sprite *backButton;
extern int timer_interrupts;
extern int air_time;

struct sprite_pos playButton_pos = {350, 200};
struct sprite_pos multiplayerButton_pos = {350, 300};
struct sprite_pos helpButton_pos = {350, 400};
struct sprite_pos quitButton_pos = {350, 500};
struct sprite_pos resumeButton_pos = {150, 250};
struct sprite_pos menuButton_pos = {550, 250};
struct sprite_pos againButton_pos = {150, 250};
struct velocity_vector cannonball_v0 = {0, 0};
struct sprite_pos cannonball_pos0 = {0, 0};
struct velocity_vector cannonball2_v0 = {0, 0};
struct sprite_pos cannonball2_pos0 = {0, 0};
struct sprite_pos backButton_pos = {350, 500};

int WIDTH_BUTTON = 100;
int HEIGHT_BUTTON = 50;
int value = 0;
bool controlValue = true;
int velocity = 0;
bool barcontrol = true;
bool space_pressed = false;
extern bool prev_pressed_space;
extern bool projectile_fired;
bool purpleNext = true;
bool multiplayer = false;
bool botShot = false;
int purple_life = 100;
int orange_life = 100;
bool purple_won = false;

uint32_t sky_color;

void setGameVariables(){
  purple_life = 100;
  orange_life = 100;
  barcontrol = true;
  space_pressed = false;
  purpleNext = true;
  multiplayer = false;
  botShot = false;
}

void set_sky_color()
{
  if ((time_info.hours >= 22 || time_info.hours <= 3))
    sky_color = 0X00001F;
  else if ((time_info.hours >= 4 && time_info.hours <= 7))
    sky_color = 0X7BFFF9;
  else if ((time_info.hours >= 8 && time_info.hours <= 17))
    sky_color = 0X51CEF0;
  else if ((time_info.hours >= 18 && time_info.hours <= 19))
    sky_color = 0XF9940B;
  else if ((time_info.hours >= 20 && time_info.hours <= 21))
    sky_color = 0X130075;
}

int set_frame_buffers(uint16_t mode)
{

  set_sky_color();

  if (set_frame_buffer(mode, &main_frame_buffer))
    return 1;
  frame_buffer_size = mode_info.XResolution * mode_info.YResolution * ((mode_info.BitsPerPixel + 7) / 8);
  if (DOUBLE_BUFFER)
  {
    secondary_frame_buffer = (uint8_t *)malloc(frame_buffer_size);
    drawing_frame_buffer = secondary_frame_buffer;
  }
  else
  {
    drawing_frame_buffer = main_frame_buffer;
  }
  return 0;
}

void swap_buffers()
{
  memcpy(main_frame_buffer, secondary_frame_buffer, frame_buffer_size);
  if (timer_interrupts % GAME_FREQUENCY == 0)
    display_real_time();
}

void draw_new_frame()
{
  switch (menuState)
  {
  case START:
    draw_initial_menu();
    setGameVariables();
    break;
  case GAME:
    multiplayer = false;
    draw_game();
    break;
  case MULTIPLAYER:
    multiplayer = true;
    draw_game();
    break;
  case PAUSE:
    draw_pause_menu();
    break;
  case HELP:
    draw_help_page();
    break;
  case END:
    draw_finish_menu();
    setGameVariables();
    break;
  }
  draw_mouse();
}

void draw_mountains()
{
  draw_sprite_xpm(mountain1, 0, 400);
  draw_sprite_xpm(mountain3, 400, 400);
  draw_sprite_xpm(mountain2, 200, 300);
  draw_sprite_xpm(mountain4, 600, 400);
}

void draw_clouds()
{
  draw_sprite_xpm(cloud1, cloud1->x_pos, cloud1->y_pos);
  draw_sprite_xpm(cloud2, cloud2->x_pos, cloud2->y_pos);
  draw_sprite_xpm(cloud3, cloud3->x_pos, cloud3->y_pos);
  draw_sprite_xpm(cloud4, cloud4->x_pos, cloud4->y_pos);
  draw_sprite_xpm(cloud5, cloud5->x_pos, cloud5->y_pos);
  draw_sprite_xpm(cloud6, cloud6->x_pos, cloud6->y_pos);
  draw_sprite_xpm(cloud7, cloud7->x_pos, cloud7->y_pos);
  draw_sprite_xpm(cloud8, cloud8->x_pos, cloud8->y_pos);
}

void draw_towers()
{
  if (purpleNext)
  {
    draw_sprite_xpm(towerO0, 650, 470);
    switch (value)
    {
    case 0:
      draw_sprite_xpm(towerP0, 70, 470);
      break;
    case 1:
      draw_sprite_xpm(towerP1, 70, 470);
      break;
    case 2:
      draw_sprite_xpm(towerP2, 70, 470);
      break;
    case 3:
      draw_sprite_xpm(towerP3, 70, 470);
      break;
    case 4:
      draw_sprite_xpm(towerP4, 70, 470);
      break;
    case 5:
      draw_sprite_xpm(towerP5, 70, 470);
      break;
    case 6:
      draw_sprite_xpm(towerP6, 70, 470);
      break;
    }
  }
  else
  {
    draw_sprite_xpm(towerP0, 70, 470);
    switch (value)
    {
    case 0:
      draw_sprite_xpm(towerO0, 650, 470);
      break;
    case 1:
      draw_sprite_xpm(towerO1, 650, 470);
      break;
    case 2:
      draw_sprite_xpm(towerO2, 650, 470);
      break;
    case 3:
      draw_sprite_xpm(towerO3, 650, 470);
      break;
    case 4:
      draw_sprite_xpm(towerO4, 650, 470);
      break;
    case 5:
      draw_sprite_xpm(towerO5, 650, 470);
      break;
    case 6:
      draw_sprite_xpm(towerO6, 650, 470);
      break;
    }
  }
}

void draw_initial_menu()
{
  draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, sky_color, drawing_frame_buffer);

  draw_mountains();

  draw_sprite_xpm(gameName, 275, 100);

  draw_sprite_xpm(playButton, playButton_pos.x, playButton_pos.y);
  draw_sprite_xpm(multiplayerButton, multiplayerButton_pos.x, multiplayerButton_pos.y);
  draw_sprite_xpm(helpButton, helpButton_pos.x, helpButton_pos.y);
  draw_sprite_xpm(quitButton, quitButton_pos.x, quitButton_pos.y);
}

void draw_game()
{
  if (!space_pressed && !prev_pressed_space)
    velocity = 0;
  if (!multiplayer && !purpleNext){
    prev_pressed_space = false;
    space_pressed = false;
  }

  draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, sky_color, drawing_frame_buffer);

  draw_mountains();

  if (timer_interrupts % 12 == 0)
  {
    if (!space_pressed)
    {
      if (controlValue)
      {
        value++;
      }
      else
      {
        value--;
      }
      if (value == 6 || value == 0)
      {
        controlValue = !controlValue;
      }
    }
  }

  draw_towers();
  draw_clouds();
  draw_health_bars();

  if (timer_interrupts % (GAME_FREQUENCY * 2) == 0)
  {
    cloud1->x_pos += 10;
    if (cloud1->x_pos > 800)
      cloud1->x_pos = -5;
    cloud2->x_pos += 10;
    if (cloud2->x_pos > 800)
      cloud2->x_pos = -5;
    cloud3->x_pos += 10;
    if (cloud3->x_pos > 800)
      cloud3->x_pos = -5;
    cloud4->x_pos += 10;
    if (cloud4->x_pos > 800)
      cloud4->x_pos = -5;
    cloud5->x_pos += 10;
    if (cloud5->x_pos > 800)
      cloud5->x_pos = -5;
    cloud6->x_pos += 10;
    if (cloud6->x_pos > 800)
      cloud6->x_pos = -5;
    cloud7->x_pos += 10;
    if (cloud7->x_pos > 800)
      cloud7->x_pos = -5;
    cloud8->x_pos += 10;
    if (cloud8->x_pos > 800)
      cloud8->x_pos = -5;
  }

  if (multiplayer || purpleNext)
  {
    botShot = false;
    player_mechanics();
    endOfGame();
  }

  else
  { // bot shooting
    bot_mechanics();
    endOfGame();
  }
}

void bot_mechanics()
{
  if (timer_interrupts % 120 == 0)
    botShot = true;

  if (botShot)
  {
    if (!projectile_fired)
    {
      projectile_fired = true;
      velocity = 20 + rand() % 30;
      air_time = timer_interrupts;
      set_cannonball_initial_position(value);
    }

    if(projectile_fired)
    {
      update_cannonball_position();
      if (collision_detected())
      {
        projectile_fired = false;
        purpleNext = true;
        value = 0;
        controlValue = true;
      }
    }
  }
}

void draw_health_bars(){
  switch (purple_life)
  {
  case 0:
    draw_sprite_xpm(healthBar0, 30, 20);
    break;
  case 20:
    draw_sprite_xpm(healthBar20, 30, 20);
    break;
  case 40:
    draw_sprite_xpm(healthBar40, 30, 20);
    break;
  case 60:
    draw_sprite_xpm(healthBar60, 30, 20);
    break;
  case 80:
    draw_sprite_xpm(healthBar80, 30, 20);
    break;
  case 100:
    draw_sprite_xpm(healthBar100, 30, 20);
    break;
  }

  switch (orange_life)
  {
  case 0:
    draw_sprite_xpm(healthBar0, 620, 20);
    break;
  case 20:
    draw_sprite_xpm(healthBar20, 620, 20);
    break;
  case 40:
    draw_sprite_xpm(healthBar40, 620, 20);
    break;
  case 60:
    draw_sprite_xpm(healthBar60, 620, 20);
    break;
  case 80:
    draw_sprite_xpm(healthBar80, 620, 20);
    break;
  case 100:
    draw_sprite_xpm(healthBar100, 620, 20);
    break;
  }
}

void player_mechanics()
{
  if (space_pressed && !projectile_fired)
  {
    if (barcontrol)
    {
      velocity++;
    }
    else
    {
      velocity--;
    }
    if (velocity == 50 || velocity == 0)
    {
      barcontrol = !barcontrol;
    }
  }
  else if (!space_pressed && prev_pressed_space && !projectile_fired)
  {
    prev_pressed_space = false;
    projectile_fired = true;
    air_time = timer_interrupts;
    set_cannonball_initial_position(value);
  }

  if (!space_pressed && !barcontrol && !projectile_fired)
    barcontrol = true;

  if (space_pressed && !projectile_fired)
  {
    if (purpleNext)
      draw_sprite_xpm(barV[velocity], 30, 350);
    else
      draw_sprite_xpm(barV[velocity], 740, 350);
  }
  if (projectile_fired)
  {
    update_cannonball_position();
    if (collision_detected())
    {
      projectile_fired = false;
      purpleNext = !purpleNext;
      value = 0;
      controlValue = true;
    }
  }
}

void draw_help_page()
{
  draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, sky_color, drawing_frame_buffer);
  draw_mountains();
  draw_sprite_xpm(helpPage1, 75, 100);
  draw_sprite_xpm(helpPage2, 400, 100);
  draw_sprite_xpm(backButton, backButton_pos.x, backButton_pos.y);
}

void set_cannonball_initial_position(int value)
{
  if (purpleNext)
  {
    switch (value)
    {
    case 0:
      cannonball_pos0.x = 160;
      cannonball_pos0.y = 465;
      cannonball_v0.vx = velocity;
      cannonball_v0.vy = 0;
      break;
    case 1:
      cannonball_pos0.x = 160;
      cannonball_pos0.y = 460;
      cannonball_v0.vx = velocity * cos(M_PI / 12);
      cannonball_v0.vy = velocity * sin(M_PI / 12);
      break;
    case 2:
      cannonball_pos0.x = 155;
      cannonball_pos0.y = 450;
      cannonball_v0.vx = velocity * cos(M_PI / 6);
      cannonball_v0.vy = velocity * sin(M_PI / 6);
      break;
    case 3:
      cannonball_pos0.x = 155;
      cannonball_pos0.y = 435;
      cannonball_v0.vx = velocity * cos(M_PI / 4);
      cannonball_v0.vy = velocity * sin(M_PI / 4);
      break;
    case 4:
      cannonball_pos0.x = 155;
      cannonball_pos0.y = 420;
      cannonball_v0.vx = velocity * cos(M_PI / 3);
      cannonball_v0.vy = velocity * sin(M_PI / 3);
      break;
    case 5:
      cannonball_pos0.x = 125;
      cannonball_pos0.y = 415;
      cannonball_v0.vx = velocity * cos(M_PI * 5 / 12);
      cannonball_v0.vy = velocity * sin(M_PI * 5 / 12);
      break;
    case 6:
      cannonball_pos0.x = 110;
      cannonball_pos0.y = 410;
      cannonball_v0.vx = 0;
      cannonball_v0.vy = velocity;
      break;
    }
  }

  else
  {
    switch (value)
    {
    case 0:
      cannonball_pos0.x = 590;
      cannonball_pos0.y = 465;
      cannonball_v0.vx = -velocity;
      cannonball_v0.vy = 0;
      break;
    case 1:
      cannonball_pos0.x = 590;
      cannonball_pos0.y = 460;
      cannonball_v0.vx = -velocity * cos(M_PI / 12);
      cannonball_v0.vy = velocity * sin(M_PI / 12);
      break;
    case 2:
      cannonball_pos0.x = 595;
      cannonball_pos0.y = 450;
      cannonball_v0.vx = -velocity * cos(M_PI / 6);
      cannonball_v0.vy = velocity * sin(M_PI / 6);
      break;
    case 3:
      cannonball_pos0.x = 595;
      cannonball_pos0.y = 435;
      cannonball_v0.vx = -velocity * cos(M_PI / 4);
      cannonball_v0.vy = velocity * sin(M_PI / 4);
      break;
    case 4:
      cannonball_pos0.x = 595;
      cannonball_pos0.y = 420;
      cannonball_v0.vx = -velocity * cos(M_PI / 3);
      cannonball_v0.vy = velocity * sin(M_PI / 3);
      break;
    case 5:
      cannonball_pos0.x = 625;
      cannonball_pos0.y = 415;
      cannonball_v0.vx = -velocity * cos(M_PI * 5 / 12);
      cannonball_v0.vy = velocity * sin(M_PI * 5 / 12);
      break;
    case 6:
      cannonball_pos0.x = 640;
      cannonball_pos0.y = 410;
      cannonball_v0.vx = 0;
      cannonball_v0.vy = velocity;
      break;
    }
  }
}

void update_cannonball_position()
{
  cannon_ball->x_pos = cannonball_pos0.x + cannonball_v0.vx * (timer_interrupts - air_time);
  cannon_ball->y_pos = cannonball_pos0.y - cannonball_v0.vy * (timer_interrupts - air_time) + 0.5 * GRAVITY * (timer_interrupts - air_time) * (timer_interrupts - air_time);
  draw_sprite_xpm(cannon_ball, cannon_ball->x_pos, cannon_ball->y_pos);
}

bool collision_detected()
{

  if (cannon_ball->x_pos >= mode_info.XResolution || cannon_ball->x_pos < 0 || cannon_ball->y_pos >= mode_info.YResolution)
  {
    return true;
  }

  if ((cannon_ball->x_pos + cannon_ball->width) >= towerO0->x_pos && cannon_ball->x_pos <= (towerO0->x_pos + towerO0->width) && (cannon_ball->y_pos + cannon_ball->height) >= towerO0->y_pos && cannon_ball->y_pos <= (towerO0->y_pos + towerO0->height))
  {
    orange_life -= 20;
    return true;
  }
  if ((cannon_ball->x_pos + cannon_ball->width) >= towerP0->x_pos && cannon_ball->x_pos <= (towerP0->x_pos + towerP0->width) && (cannon_ball->y_pos + cannon_ball->height) >= towerP0->y_pos && cannon_ball->y_pos <= (towerP0->y_pos + towerP0->height))
  {
    purple_life -= 20;
    return true;
  }
  return false;
}

void endOfGame(){
  if (purple_life <= 0){
    purple_won = false;
    menuState = END;
  }
  if (orange_life <= 0){
    purple_won = true;
    menuState = END;
  }
}

void draw_finish_menu()
{
  draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, sky_color, drawing_frame_buffer);
  draw_mountains();

  draw_clouds();

  draw_towers();
  if(purple_won)
    draw_sprite_xpm(purpleWon, 275, 100);
  else
    draw_sprite_xpm(orangeWon, 275, 100);
  draw_sprite_xpm(againButton, againButton_pos.x, againButton_pos.y);
  draw_sprite_xpm(menuButton, menuButton_pos.x, menuButton_pos.y);
}

void draw_pause_menu()
{
  draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, sky_color, drawing_frame_buffer);

  draw_mountains();

  draw_clouds();

  draw_towers();

  draw_sprite_xpm(gamePaused, 275, 100);
  draw_sprite_xpm(resumeButton, resumeButton_pos.x, resumeButton_pos.y);
  draw_sprite_xpm(menuButton, menuButton_pos.x, menuButton_pos.y);
}

void mouse_position()
{
  if (mouse_info.x > playButton_pos.x && mouse_info.y > playButton_pos.y && mouse_info.x < (playButton_pos.x + WIDTH_BUTTON) && mouse_info.y < (playButton_pos.y + HEIGHT_BUTTON))
  {
    draw_sprite_xpm(mouse_click, mouse_info.x, mouse_info.y);
  }
  else if (mouse_info.x > multiplayerButton_pos.x && mouse_info.y > multiplayerButton_pos.y && mouse_info.x < multiplayerButton_pos.x + WIDTH_BUTTON && mouse_info.y < multiplayerButton_pos.y + HEIGHT_BUTTON)
  {
    draw_sprite_xpm(mouse_click, mouse_info.x, mouse_info.y);
  }
  else if (mouse_info.x > helpButton_pos.x && mouse_info.y > helpButton_pos.y && mouse_info.x < helpButton_pos.x + WIDTH_BUTTON && mouse_info.y < helpButton_pos.y + HEIGHT_BUTTON)
  {
    draw_sprite_xpm(mouse_click, mouse_info.x, mouse_info.y);
  }
  else if (mouse_info.x > quitButton_pos.x && mouse_info.y > quitButton_pos.y && mouse_info.x < quitButton_pos.x + WIDTH_BUTTON && mouse_info.y < quitButton_pos.y + HEIGHT_BUTTON)
  {
    draw_sprite_xpm(mouse_click, mouse_info.x, mouse_info.y);
  }
  else
  {
    draw_sprite_xpm(mouse, mouse_info.x, mouse_info.y);
  }
}

void mouse_position_pause()
{
  if (mouse_info.x > resumeButton_pos.x && mouse_info.y > resumeButton_pos.y && mouse_info.x < (resumeButton_pos.x + WIDTH_BUTTON) && mouse_info.y < (resumeButton_pos.y + HEIGHT_BUTTON))
  {
    draw_sprite_xpm(mouse_click, mouse_info.x, mouse_info.y);
  }
  else if (mouse_info.x > menuButton_pos.x && mouse_info.y > menuButton_pos.y && mouse_info.x < menuButton_pos.x + WIDTH_BUTTON && mouse_info.y < menuButton_pos.y + HEIGHT_BUTTON)
  {
    draw_sprite_xpm(mouse_click, mouse_info.x, mouse_info.y);
  }
  else
  {
    draw_sprite_xpm(mouse, mouse_info.x, mouse_info.y);
  }
}
void mouse_position_end()
{
  if (mouse_info.x > againButton_pos.x && mouse_info.y > againButton_pos.y && mouse_info.x < (againButton_pos.x + WIDTH_BUTTON) && mouse_info.y < (againButton_pos.y + HEIGHT_BUTTON))
  {
    draw_sprite_xpm(mouse_click, mouse_info.x, mouse_info.y);
  }
  else if (mouse_info.x > menuButton_pos.x && mouse_info.y > menuButton_pos.y && mouse_info.x < menuButton_pos.x + WIDTH_BUTTON && mouse_info.y < menuButton_pos.y + HEIGHT_BUTTON)
  {
    draw_sprite_xpm(mouse_click, mouse_info.x, mouse_info.y);
  }
  else
  {
    draw_sprite_xpm(mouse, mouse_info.x, mouse_info.y);
  }
}
void mouse_position_help()
{
  if (mouse_info.x > backButton_pos.x && mouse_info.y > backButton_pos.y && mouse_info.x < (backButton_pos.x + WIDTH_BUTTON) && mouse_info.y < (backButton_pos.y + HEIGHT_BUTTON))
  {
    draw_sprite_xpm(mouse_click, mouse_info.x, mouse_info.y);
  }
  else
  {
    draw_sprite_xpm(mouse, mouse_info.x, mouse_info.y);
  }
}

void draw_mouse()
{
  switch (menuState)
  {
  case START:
    mouse_position();
    break;
  case END:
    mouse_position_end();
    break;
  case PAUSE:
    mouse_position_pause();
    break;
  case MULTIPLAYER:
    break;
  case HELP:
    mouse_position_help();
    break;
  case GAME:
    break;
  }
}

int draw_sprite_xpm(Sprite *sprite, int x, int y)
{
  uint16_t height = sprite->height;
  uint16_t width = sprite->width;
  uint32_t current_color;
  for (int h = 0; h < height; h++)
  {
    for (int w = 0; w < width; w++)
    {
      current_color = sprite->colors[w + h * width];
      if (current_color == TRANSPARENT)
        continue;
      if (draw_pixel(x + w, y + h, current_color, drawing_frame_buffer) != 0)
        return 1;
    }
  }
  return 0;
}

void display_real_time()
{
  printf("NOW: %d/%d/%d @%d:%d:%d\n", 2000 + time_info.year, time_info.month, time_info.day, time_info.hours, time_info.minutes, time_info.seconds);
}
