#include <lcom/lcf.h>

#include "game.h"

#include "math.h"

// ----------------------------------------------------------------------------------------

static Crosshair crosshair;
static Hero hero;
static Bullet bullet;
static Fence fences[NUM_FENCES];
static Robot robots[NUM_ROBOTS];

// ----------------------------------------------------------------------------------------

int (gameplay)() {
  if (loadGame())
    return 1;

  int r, ipc_status;
  message msg;

  int freq = 60;
  int_counter = 0;
  uint8_t key_code = 0;

  int process = 1;
  while (process) {
      /* Get a request message. */
      if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
          printf("driver_receive failed with: %d", r);
          continue;
      }
      if (is_ipc_notify(ipc_status)) { // received notification
          switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: // hardware interrupt notification
              if (msg.m_notify.interrupts & timer_irq_set) { // subscribed interrupt (Timer 0)
                timer_int_handler();
                if (!(int_counter % (freq / GAME_FPS))) {
                  if (!validGame()) {
                    return 0;
                  }
                  if (displayArena()) {
                    return 1;
                  }
                }
                if (!(int_counter % 10))
                  moveRobots();
                if (!(int_counter % 2))
                  bulletTravel();
              }
              if (msg.m_notify.interrupts & kbd_irq_set) { // subscribed interrupt (Keyboard)
                kbc_ih();
                if (!kbd_inc_code()) {
                  key_code = get_key();

                  if (key_code == KEY_ESC)
                    return 0;
                  
                  moveHero(key_code);
                }
              }
              if (msg.m_notify.interrupts & mouse_irq_set) { // subscribed interrupt (Mouse)
                mouse_ih();
                if (mouse_packet_full() && !mouse_overflow()) {
                  struct mouse_ev event = mouse_event();
                  
                  if (event.type == LB_RELEASED) {
                    if (bullet.hit)
                      shoot();
                  }
                  else if (event.type == MOUSE_MOV) 
                    moveCrosshair(event.delta_x, event.delta_y);
                }
              }
              break;
            default:
              break; // no other notifications expected: do nothing
          }
      }
      else { // received a standard message, not a notification
        // no standard messages expected: do nothing
      }
  }

  return 0;
}

// ----------------------------------------------------------------------------------------

int (getPixelX)(int logX) {
  return ARENA_X_ORIGIN + logX * ENTITY_WIDTH;
}

int (getPixelY)(int logY) {
  return ARENA_Y_ORIGIN + logY * ENTITY_HEIGHT;
}

int (loadGame)() {
  hero.x = getPixelX(11);
  hero.y = getPixelY(11);
  hero.alive = true;

  crosshair.x = getPixelX(20);
  crosshair.y = getPixelY(1);

  bulletReset();

  int cnt = 0;

  /* top row */
  for (int i = 0; i < ARENA_WIDTH; i++) {
    Fence f = {getPixelX(i), getPixelY(0)};
    fences[cnt] = f;
    cnt++;
  }

  /* bottom row */
  for (int i = 0; i < ARENA_WIDTH; i++) {
    Fence f = {getPixelX(i), getPixelY(21)};
    fences[cnt] = f;
    cnt++;
  }

  /* right column */
  for (int i = 1; i <= (ARENA_HEIGHT - 2); i++) {
    Fence f = {getPixelX(0), getPixelY(i)};
    fences[cnt] = f;
    cnt++;
  }

  /* left column */
  for (int i = 1; i <= (ARENA_HEIGHT - 2); i++) {
    Fence f = {getPixelX(21), getPixelY(i)};
    fences[cnt] = f;
    cnt++;
  }

  /* random fences inside the arena */

  Fence f1 = {getPixelX(3), getPixelY(3)};
  fences[cnt] = f1;
  cnt++;

  Fence f2 = {getPixelX(5), getPixelY(10)};
  fences[cnt] = f2;
  cnt++;

  Fence f3 = {getPixelX(16), getPixelY(4)};
  fences[cnt] = f3;
  cnt++;

  Fence f4 = {getPixelX(18), getPixelY(17)};
  fences[cnt] = f4;
  cnt++;

  Fence f5 = {getPixelX(9), getPixelY(18)};
  fences[cnt] = f5;
  cnt++;

  Fence f6 = {getPixelX(15), getPixelY(12)};
  fences[cnt] = f6;
  cnt++;

  Fence f7 = {getPixelX(15), getPixelY(19)};
  fences[cnt] = f7;
  cnt++;

  Fence f8 = {getPixelX(5), getPixelY(16)};
  fences[cnt] = f8;
  cnt++;

  Fence f9 = {getPixelX(8), getPixelY(6)};
  fences[cnt] = f9;
  cnt++;

  Fence f10 = {getPixelX(18), getPixelY(10)};
  fences[cnt] = f10;
  cnt++;

  if (cnt != NUM_FENCES) {
    if (leave())
      return 1;
    return 1;
  }

  /* random robots inside the arena */

  Robot r0 = {getPixelX(11), getPixelY(5), true};
  robots[0] = r0;

  Robot r1 = {getPixelX(4), getPixelY(6), true};
  robots[1] = r1;

  Robot r2 = {getPixelX(19), getPixelY(13), true};
  robots[2] = r2;

  Robot r3 = {getPixelX(7), getPixelY(18), true};
  robots[3] = r3;

  Robot r4 = {getPixelX(17), getPixelY(19), true};
  robots[4] = r4;

  Robot r5 = {getPixelX(2), getPixelY(4), true};
  robots[5] = r5;

  Robot r6 = {getPixelX(9), getPixelY(2), true};
  robots[6] = r6;

  Robot r7 = {getPixelX(15), getPixelY(14), true};
  robots[7] = r7;

  Robot r8 = {getPixelX(3), getPixelY(16), true};
  robots[8] = r8;

  Robot r9 = {getPixelX(18), getPixelY(3), true};
  robots[9] = r9;

  return 0;
}

// ----------------------------------------------------------------------------------------

int (displayArena)() {
  vg_clear_screen();

  if (drawFences())
    return 1;

  if (drawRobots())
    return 1;

  if (drawBullet())
    return 1;

  if (drawHero())
    return 1;

  if (drawCrosshair())
    return 1;

  vg_draw_content();

  return 0;
}

int (drawCrosshair)() {
  if (vg_draw_element(crosshair_sprite, crosshair.x, crosshair.y)) {
    if (leave())
      return 1;
    return 1;
  }

  return 0;
}

int (drawFences)() {
  for (int i = 0; i < NUM_FENCES; i++) {
    if (vg_draw_element(fence_sprite, fences[i].x, fences[i].y)) {
      if (leave())
        return 1;
      return 1;
    }
  }

  return 0;
}

int (drawHero)() {
  if (vg_draw_element(hero_sprite, hero.x, hero.y)) {
    if (leave())
      return 1;
    return 1;
  }

  return 0;
}

int (drawRobots)() {
  for (int i = 0; i < NUM_ROBOTS; i++) {
    if (robots[i].alive) {
      if (vg_draw_element(robot_sprite, robots[i].x, robots[i].y)) {
        if (leave())
          return 1;
        return 1;
      }
    }
    else {
      if (vg_draw_element(dead_robot_sprite, robots[i].x, robots[i].y)) {
        if (leave())
          return 1;
        return 1;
      }
    }
  }

  return 0;
}

int (drawBullet)() {
  if (!bullet.hit) {
    if (vg_draw_element(bullet_sprite, bullet.x, bullet.y)) {
      if (leave())
        return 1;
      return 1;
    }
  }

  return 0;
}

// ----------------------------------------------------------------------------------------

bool (validGame)() {
  if (!hero.alive)
    return false;

  int dead_robots = 0;
  for(int i = 0 ; i < NUM_ROBOTS; i++)
    if (!robots[i].alive)
      dead_robots++;

  if (dead_robots == NUM_ROBOTS)
    return false;

  return true;
}

void (moveCrosshair)(int16_t x, int16_t y) {
  crosshair.x = (0 < crosshair.x + x) ? crosshair.x + x : 0;
  crosshair.x = (crosshair.x < CONSOLE_WIDTH - 1) ? crosshair.x : (CONSOLE_WIDTH - 1);

  crosshair.y = (0 < crosshair.y - y) ? crosshair.y - y : 0;
  crosshair.y = (crosshair.y < CONSOLE_HEIGHT - 1) ? crosshair.y : (CONSOLE_HEIGHT - 1);
}

// ----------------------------------------------------------------------------------------

bool (checkCollision)(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
  return ((x1 < x2 + w2) && (x1 + w1 > x2) && (y1 < y2 + h2) && (y1 + h1 > y2));
}

bool (canHeroMove)(uint16_t newX, uint16_t newY) {
  for (int i = 0; i < NUM_ROBOTS; i++) {
    if (checkCollision(newX, newY, ENTITY_WIDTH, ENTITY_HEIGHT, robots[i].x, robots[i].y, ENTITY_WIDTH, ENTITY_HEIGHT)) {
      if (robots[i].alive)
        hero.alive = false;
      return false;
    }
  }

  for (int i = 0; i < NUM_FENCES; i++) {
    if (checkCollision(newX, newY, ENTITY_WIDTH, ENTITY_HEIGHT, fences[i].x, fences[i].y, ENTITY_WIDTH, ENTITY_HEIGHT)) {
      hero.alive = false;
      return false;
    }
  }

  return true;
}

void (moveHero)(uint8_t key_code) {
  switch (key_code) {
      case KEY_D:
        if (canHeroMove(hero.x + HERO_X_SPEED, hero.y)) {
          if (hero.alive) {
            hero.x += HERO_X_SPEED;
            bulletStore();
          }
        }
        break;
      case KEY_A:
        if (canHeroMove(hero.x - HERO_X_SPEED, hero.y)) {
          if (hero.alive) {
            hero.x -= HERO_X_SPEED;
            bulletStore();
          }
        }
        break;
      case KEY_W:
        if (canHeroMove(hero.x, hero.y - HERO_Y_SPEED)) {
          if (hero.alive) {
            hero.y -= HERO_Y_SPEED;
            bulletStore();
          }
        }
        break;
      case KEY_S:
        if (canHeroMove(hero.x, hero.y + HERO_Y_SPEED)) {
          if (hero.alive) {
            hero.y += HERO_Y_SPEED;
            bulletStore();
          }
        }
        break;
      default:
        break;
    }
}

// ----------------------------------------------------------------------------------------

enum Movement (shortestPath)(int x1, int y1, int x2, int y2) {
  if (y1 == y2) { 
		if (x1 > x2) 
			return RIGHT; 
		else if (x1 < x2) 
			return LEFT; 
	}
	else if (x1 == x2) {
		if (y1 > y2)
			return DOWN; 
		else if (y1 < y2) 
			return UP; 
	}
	else if (y1 > y2) { 
		if (x1 > x2)
			return DOWN_RIGHT;
		else if (x1 < x2) 
			return DOWN_LEFT; 
	}
	else if (y1 < y2) { 
		if (x1 > x2)
			return UP_RIGHT;
		else if (x1 < x2)
			return UP_LEFT; 
	}

  return INVALID;
}

bool (canRobotMove)(int id, uint16_t newX, uint16_t newY) {
  if (checkCollision(newX, newY, ENTITY_WIDTH, ENTITY_HEIGHT, hero.x, hero.y, ENTITY_WIDTH, ENTITY_HEIGHT)) {
    hero.alive = false;
    return false;
  }

  if (checkCollision(newX, newY, ENTITY_WIDTH, ENTITY_HEIGHT, bullet.x, bullet.y, BULLET_WIDTH, BULLET_HEIGHT)) {
    robots[id].alive = false;
    return false;
  }

  for (int i = 0; i < NUM_ROBOTS; i++) {
    if (i == id)
      continue;
    else {
      if (checkCollision(newX, newY, ENTITY_WIDTH, ENTITY_HEIGHT, robots[i].x, robots[i].y, ENTITY_WIDTH, ENTITY_HEIGHT))
          return false;
    }
  }

  for (int i = 0; i < NUM_FENCES; i++)
    if (checkCollision(newX, newY, ENTITY_WIDTH, ENTITY_HEIGHT, fences[i].x, fences[i].y, ENTITY_WIDTH, ENTITY_HEIGHT))
      return false;

  return true;
}

void (moveRobots)() {
  for (int i = 0; i < NUM_ROBOTS; i++) {
    if (robots[i].alive) {
      switch (shortestPath(hero.x, hero.y, robots[i].x, robots[i].y)) {
        case RIGHT:
          if (canRobotMove(i, robots[i].x + ROBOT_X_SPEED, robots[i].y))
            if (robots[i].alive)
              robots[i].x += ROBOT_X_SPEED;
          break;
        case LEFT:
          if (canRobotMove(i, robots[i].x - ROBOT_X_SPEED, robots[i].y))
            if (robots[i].alive)
              robots[i].x -= ROBOT_X_SPEED;
          break;
        case DOWN:
          if (canRobotMove(i, robots[i].x, robots[i].y + ROBOT_Y_SPEED))
            if (robots[i].alive)
              robots[i].y += ROBOT_Y_SPEED;
          break;
        case UP:
          if (canRobotMove(i, robots[i].x, robots[i].y - ROBOT_Y_SPEED))
            if (robots[i].alive)
              robots[i].y -= ROBOT_Y_SPEED;
          break;
        case DOWN_RIGHT:
          if (canRobotMove(i, robots[i].x + ROBOT_X_SPEED, robots[i].y + ROBOT_Y_SPEED)) {
            if (robots[i].alive) {
              robots[i].x += ROBOT_X_SPEED;
              robots[i].y += ROBOT_Y_SPEED;
            }
          }
          break;
        case DOWN_LEFT:
          if (canRobotMove(i, robots[i].x - ROBOT_X_SPEED, robots[i].y + ROBOT_Y_SPEED)) {
            if (robots[i].alive) {
              robots[i].x -= ROBOT_X_SPEED;
              robots[i].y += ROBOT_Y_SPEED;
            }
          }
          break;
        case UP_RIGHT:
          if (canRobotMove(i, robots[i].x + ROBOT_X_SPEED, robots[i].y - ROBOT_Y_SPEED)) {
            if (robots[i].alive) {
              robots[i].x += ROBOT_X_SPEED;
              robots[i].y -= ROBOT_Y_SPEED;
            }
          }
          break;
        case UP_LEFT:
          if (canRobotMove(i, robots[i].x - ROBOT_X_SPEED, robots[i].y - ROBOT_Y_SPEED)) {
            if (robots[i].alive) {
              robots[i].x -= ROBOT_X_SPEED;
              robots[i].y -= ROBOT_Y_SPEED;
            }
          }
          break;  
        default:
          break;
      }
    }
  }
} 

// ----------------------------------------------------------------------------------------

int (crosshairCenterX)() {
  return (crosshair.x + BULLET_WIDTH / 2);
}

int (crosshairCenterY)() {
  return (crosshair.y + BULLET_HEIGHT / 2);
}

void (bulletReset)() {
  bullet.hit = true;
  bullet.x = hero.x;
  bullet.y = hero.y;
  bullet.xStep = 0;
  bullet.yStep = 0;
}

void (shoot)() {
  bullet.hit = false;

  bulletDirection();
}

void (bulletDirection)() {
  int x = crosshair.x - bullet.x;
  int y = crosshair.y - bullet.y;

  double s = pow(x, 2) + pow(y, 2);
  int hypo = (int)(round(sqrt(s)));

  bullet.xStep = (x * BULLET_SPEED) / hypo;
  bullet.yStep = (y * BULLET_SPEED) / hypo;
}

bool (checkHit)(uint16_t newX, uint16_t newY) {
  for (int i = 0; i < NUM_FENCES; i++) {
    if (checkCollision(newX, newY, BULLET_WIDTH, BULLET_HEIGHT, fences[i].x, fences[i].y, ENTITY_WIDTH, ENTITY_HEIGHT)) {
      bulletReset();
      return true;
    }
  }

  for (int i = 0; i < NUM_ROBOTS; i++) {
    if (checkCollision(newX, newY, BULLET_WIDTH, BULLET_HEIGHT, robots[i].x, robots[i].y, ENTITY_WIDTH, ENTITY_HEIGHT)) {
      bulletReset();
      robots[i].alive = false;
      return true;
    }
  }

  return false;
}

void (bulletTravel)() {
  if (bullet.hit)
    return;

  if (!checkHit(bullet.x + bullet.xStep, bullet.y + bullet.yStep)) {
    bullet.x += bullet.xStep;
    bullet.y += bullet.yStep;
  }
}

void (bulletStore)() {
  if (bullet.hit) {
    bullet.x = hero.x;
    bullet.y = hero.y;
  }
}

// ----------------------------------------------------------------------------------------
