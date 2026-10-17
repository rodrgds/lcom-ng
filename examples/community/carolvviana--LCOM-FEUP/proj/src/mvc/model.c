#include "model.h"

int menu_options[3] = {MENU_SINGLEPLAYER_OPTION, MENU_MULTIPLAYER_OPTION, MENU_CREDITS_OPTION};

int menu_entry = 0;
extern game_state_t game_state;

extern int16_t x_res;
extern int16_t y_res;

struct Cursor cursor = {500, 250};

int sign_x = 0;
int sign_y = 0;

void(upMenuOption)() {
  if (menu_entry == 0) {
    menu_entry = 2;
  }
  else {
    menu_entry = (menu_entry - 1) % 3;
  }
}

void(downMenuOption)() {
  menu_entry = (menu_entry + 1) % 3;
}

int(getMenuOption)() {
  return menu_options[menu_entry];
}

int initial_pos[4] = {150, 280, 630, 280}; // p1_x, p1_y, p2_x, p2_y
int initial_ball[4] = {400, 300, 10, 10};    // ball_x, ball_y, ball_x_velocity, ball_y_velocity

const unsigned int p1_color = 0x00ffff, p2_color = 0xff00ff;

struct Player player = {150, 280, p1_color, 20, 0, 40, 20, 0};
struct Player player2 = {630, 280, p2_color, 20, 0, 40, 20, 0};
struct Player AIplayer = {650, 280, p2_color, 8, 0, 40, 20, 0};
struct Arena arena = {750, 50, 500, 100, 400, 700};
struct Ball ball = {400, 300, 10, 10, 0xFFFFFF, 15, 15};

// int powerupActiveTimer = 0;
// int powerupCooldownTimer = (60/REFRESH_RATE) * 60; // ? seconds
int powerupCooldownTimer = 0; // sofia

void(resetPositions)() {
  player.x_pos = initial_pos[0];
  player.y_pos = initial_pos[1];

  AIplayer.x_pos = initial_pos[2];
  AIplayer.y_pos = initial_pos[3];

  player2.x_pos = initial_pos[2];
  player2.y_pos = initial_pos[3];

  ball.x_pos = initial_ball[0];
  ball.y_pos = initial_ball[1];

  // player.x_pos = p1_initial_x_pos;
  // player.y_pos = p1_initial_y_pos;

  // AIplayer.x_pos = p2_initial_x_pos;
  // AIplayer.y_pos =p2_initial_y_pos;

  // player2.x_pos = p2_initial_x_pos;
  // player2.y_pos = p2_initial_y_pos;

  // ball.x_pos = ball_initial_x_pos;
  // ball.y_pos = ball_initial_y_pos;
}

void(reset)() {
  resetPositions();

  ball.x_velocity = initial_ball[2];
  
  // ball.x_velocity = ball_initial_x_velocity;
  //   ball.x_velocity = rand() % 2 == 0 ? ball_initial_x_velocity : -ball_initial_x_velocity;

  if (game_state == SINGLE_PLAYER_GAME) {
    srand(time(NULL));
    int y_vel = initial_ball[3];
    ball.y_velocity = rand() % 2 == 0 ? y_vel : -y_vel;
    // ball.y_velocity = rand() % 2 == 0 ?  ball_initial_y_velocity : -ball_initial_y_velocity;
  }
  else {
    ball.y_velocity = 5;
    ball.x_velocity = 5;
    // ball.y_velocity = ball_initial_y_velocity;
  }
  resetPowerup();
  powerupCooldownTimer = (60 / REFRESH_RATE) * 0;
}

void(resetGame)() {
  reset();
  player.color = p1_color;
  player2.color = p2_color;
  player.score = 0;
  player2.score = 0;
  AIplayer.score = 0;
}

void(movePlayerUp)(struct Player *player) {
  int maybe_pos = player->y_pos - player->y_velocity;
  int min_pos = arena.y_min; // isto é o caso em que se ele andar para cima toda a velocidade, vai ultrapassar o limite da arena
  if (maybe_pos >= min_pos) {
    player->y_pos = maybe_pos;
  }
  else {
    player->y_pos = min_pos;
  } // etste if é quando aquilo acontece (mete o a andar apenas o que "consegue"
}

void(movePlayerDown)(struct Player *player) {
  int maybe_pos = player->y_pos + player->y_velocity;
  int max_pos = arena.y_max - player->height; // isto é o caso em que se ele andar para baixo toda a velocidade, vai ultrapassar o limite da arena
  if (maybe_pos <= max_pos) {
    player->y_pos = maybe_pos;
  }
  else {
    player->y_pos = max_pos;
  } // etste if é quando aquilo acontece (mete o a andar apenas o que "consegue"
}

void(movePlayerLeft)(struct Player *player) {
  int maybe_pos = player->x_pos - player->x_velocity;
  int max_pos = arena.x_min + player->width;
  if (maybe_pos < max_pos) {
    player->x_pos = arena.x_min;
  }
  else {
    player->x_pos = maybe_pos;
  }
}

void(movePlayerRight)(struct Player *player) {
  int arena_lenght = arena.x_max - arena.x_min;
  int max_pos = arena.x_min + arena_lenght / 2 - player->width - player->x_velocity;

  if (player->x_pos <= max_pos) {
    player->x_pos += player->x_velocity;
  }
  else if (player->x_pos > max_pos) {
    player->x_pos += (arena.x_min + arena_lenght / 2 - player->x_pos - player->width);
  }
}

void(activatePowerup1)() {
  powerupCooldownTimer = (60 / REFRESH_RATE) * 80;
  player.x_velocity = 20;
}

void(activatePowerup2)() {
  powerupCooldownTimer = (60 / REFRESH_RATE) * 80;
  player.height += 20;
}

void(resetPowerup)() {
  player.x_pos = initial_pos[0];
  // player.x_pos = p1_initial_x_pos;
  player.x_velocity = 0;
  player.height = 40;
}

void moveAI(struct Player *player) {
  // Determine the position of the ball and the paddle relative to the ball's position
  int ball_x_pos = ball.x_pos;
  int ball_y_pos = ball.y_pos;
  int paddle_y_pos = player->y_pos;

  if (ball_x_pos > arena.width / 2 + arena.x_min) {

    if (ball_y_pos < paddle_y_pos + player->height / 2) {
      movePlayerUp(player);
    }
    else if (ball_y_pos > paddle_y_pos + player->height / 2) {
      movePlayerDown(player);
    }
  }
}

enum CollisionType(checkBallCollision)(struct Player *player) {
  bool sameYPosition = (ball.y_pos + ball.height >= player->y_pos) && (ball.y_pos <= player->y_pos + player->height);
  bool sameXPosition = (ball.x_pos + ball.width >= player->x_pos) && (ball.x_pos <= player->x_pos + player->width);

  if (!sameYPosition || !sameXPosition) {
    return NoCollision;
  }

  bool onTop = (ball.y_pos + ball.height >= player->y_pos) && (ball.y_pos + ball.height < player->y_pos + player->height) && sameXPosition;
  bool onBottom = ball.y_pos == player->y_pos + player->height && sameXPosition;
  bool onLeft = (ball.x_pos + ball.width >= player->x_pos) && (ball.x_pos + ball.width < player->x_pos + player->width) && sameYPosition;
  bool onRight = ball.x_pos == player->x_pos + player->width && sameYPosition;

  if (onLeft || onRight) {
    return SideCollision;
  }
  else if (onTop || onBottom) {
    return TopBottomCollision;
  }

  return NoCollision;
}

bool(moveBall)() {
  // if (ball.x_pos + ball.x_velocity <= 50 || ball.x_pos + ball.x_velocity >= 750) {
  //   ball.x_velocity = -ball.x_velocity;
  //   sign_x++;
  // }

  if (ball.y_pos + ball.y_velocity <= 100 || ball.y_pos + ball.width + ball.y_velocity >= 500) {
    ball.y_velocity = -ball.y_velocity;
    sign_y++;
  }

  enum CollisionType playerCollision = checkBallCollision(&player);
  if (playerCollision == SideCollision) {
    ball.x_velocity = -ball.x_velocity;
  }
  else if (playerCollision == TopBottomCollision) {
    ball.y_velocity = -ball.y_velocity;
  }

  switch (game_state) {
    case SINGLE_PLAYER_GAME: {
      enum CollisionType AICollision = checkBallCollision(&AIplayer);
      if (AICollision == SideCollision) {
        ball.x_velocity = -ball.x_velocity;
      }
      else if (AICollision == TopBottomCollision) {
        ball.y_velocity = -ball.y_velocity;
      }
      break;
    }
    case MULTI_PLAYER_GAME: {
      enum CollisionType player2Collision = checkBallCollision(&player2);
      if (player2Collision == SideCollision) {
        ball.x_velocity = -ball.x_velocity;
      }
      else if (player2Collision == TopBottomCollision) {
        ball.y_velocity = -ball.y_velocity;
      }
      break;
    }
    default:
      break;
  }

  sign_x %= 2;
  sign_y %= 2;
  ball.x_pos += ball.x_velocity;
  ball.y_pos += ball.y_velocity;

  if (isGoal()) {
    reset();
    return true;
  }

  return false;
}

bool(isGoal)() {
  // switch (game_state) {
  //   case MULTI_PLAYER_GAME: {
  //     if (ball.x_pos + ball.width >= arena.x_max) { // golo nosso
  //       player.score++;
  //       send_data_serialp(BIT(2)); // golo meu
  //       // player.x_pos >= player2.x_pos ? player2.score++ : player.score++ ;
  //       // player.x_pos < player2.x_pos ? send_data_serialp(BIT(2)) : send_data_serialp(BIT(3));
  //       return true;




  //       // if (player.x_pos < player2.x_pos) {
  //       //   player.score++;
  //       //   send_data_serialp(BIT(2));
  //       //   return true;
  //       // }
  //       // else {
  //       //   player2.score++;
  //       //   send_data_serialp(BIT(3));
  //       //   return true;
  //       // }
       
  //     }
  //     else if (ball.x_pos - ball.width <= arena.x_min) { // golo deles
  //       player2.score++;
  //       send_data_serialp(BIT(3)); // golo deles
  //       // player.x_pos >= player2.x_pos ? player.score++ : player2.score++;
  //       // player.x_pos >= player2.x_pos ? send_data_serialp(BIT(2)) : send_data_serialp(BIT(3));
  //       return true;
  //       // if (player.x_pos < player2.x_pos) {
  //       //   player2.score++;
  //       //   send_data_serialp(BIT(3));
  //       //   return true;
  //       // }
  //       // else {
  //       //   player.score++;
  //       //   send_data_serialp(BIT(2));
  //       //   return true;
  //       // }
        
  //     }
  //     break;
  //   }

  //   case SINGLE_PLAYER_GAME: {
  //     if (ball.x_pos + ball.width >= arena.x_max) { // golo nosso
  //       player.score++;
  //       return true;
  //     }
  //     else if (ball.x_pos - ball.width <= arena.x_min) { // golo deles
  //       AIplayer.score++;
  //       return true;
  //     }
  //     break;
  //   }
  //   default: break;
  //   // return false;
  // }
  // return false;

  //podemos ir comentando e descomentando isto para testar
  if (ball.x_pos + ball.width >= arena.x_max) { // golo noss
    switch (game_state) {
      case SINGLE_PLAYER_GAME: {
        player.score++;
        break;
      }
      case MULTI_PLAYER_GAME: {
        player.score++;
        send_data_serialp(BIT(7)); // golo meu
        break;
      }
      default: {
        break;
      }
    }

    return true;
  }

  else if (ball.x_pos - ball.width <= arena.x_min) { // golo deles
    switch (game_state) {
      case SINGLE_PLAYER_GAME: {
        AIplayer.score++;
        break;
      }
      case MULTI_PLAYER_GAME: {
        player2.score++;
        send_data_serialp(BIT(6)); // golo deles
        break;
      }
      default: {
        break;
      }
    }
    return true;
  }

  return false;
}

enum Winner(checkScore)() {
  switch (game_state) {
    case SINGLE_PLAYER_GAME: {
      if (player.score == 7) {
        return PLAYER1;
      }
      else if (AIplayer.score == 7) {
        return PLAYER2;
      }
      break;
    }
    case MULTI_PLAYER_GAME: {
      if (player.score == 7) {
        send_data_serialp(BIT(0)); // ganhei, envio sinal a avisar que adversário perdeu
        return PLAYER1;
      }
      else if (player2.score == 7) {
        send_data_serialp(BIT(1)); // perdi, envio sinal a avisar que adversário ganhou
        return PLAYER2;
      }
      break;
    }
    default:
      break;
  }
  return NO_WINNER;
}

void(setMenuOption)(int option) {
  menu_entry = option;
}

void(moveCursor)(int dx, int dy) {

  if (cursor.x + dx < 0) {
    cursor.x = 0;
    return;
  }
  if (cursor.x + dx > x_res) {
    cursor.x = x_res;
    return;
  }
  if (cursor.y - dy < 0) {
    cursor.y = 0;
    return;
  }
  if (cursor.y - dy > y_res) {
    cursor.y = y_res;
    return;
  }

  cursor.x = cursor.x + dx;
  cursor.y = cursor.y - dy;
  return;
}

uint8_t(compressData)(int data, int compressor) {
  return (uint8_t) (data / compressor);
}

uint8_t(decompressData)(uint8_t data, int decompressor) {
  return (uint8_t) (data * decompressor);
}

void(switchPlayers)() {
  switchColorPlayers();
}

void(switchColorPlayers)() {
  unsigned int switch_color = player.color;
  player.color = player2.color;
  player2.color = switch_color;
}

void(switchBall)() {
  ball.x_velocity = -ball.x_velocity;
}
