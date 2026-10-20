#include "snake.h"

struct Tile Field[N][M];

struct Snake newSnake;

void snake_init(){
  for(size_t i=0;i<N;i++){//inicializa a matriz
    for(size_t j=0;j<M;j++){
      Field[i][j].Head=false;
      Field[i][j].Body=false;
      Field[i][j].Tail=false;
      Field[i][j].dir=EMPTY;
    }
  }
  newSnake.size=10;
  newSnake.x=N/2; //x e y da Head da snake
  newSnake.y=M/2;
  newSnake.snake_dir=UP;
  unsigned int tmpy=newSnake.y;

  for (size_t i=1; i<=newSnake.size;i++){ //inicializar a snake nas coordenadas x,y
    if (i==1){
      Field[newSnake.x][tmpy].Head=true;
      Field[newSnake.x][tmpy].dir=UP;
    }
    else if (i==newSnake.size){
      Field[newSnake.x][tmpy].Tail=true;
      Field[newSnake.x][tmpy].dir=UP;
    }
    else{
      Field[newSnake.x][tmpy].Body=true;
      Field[newSnake.x][tmpy].dir=UP;
    }
    tmpy++;
  }
}

void printSnake(uint8_t * head_up_adr, xpm_image_t head_up_img,uint8_t * head_down_adr, xpm_image_t head_down_img, uint8_t * head_left_adr, xpm_image_t head_left_img, uint8_t * head_right_adr, xpm_image_t head_right_img,uint8_t* body_h_adr, xpm_image_t body_h_img,uint8_t* body_v_adr, xpm_image_t body_v_img, uint8_t* tail_right_adr, xpm_image_t tail_right_img, uint8_t* tail_left_adr, xpm_image_t tail_left_img,uint8_t* tail_down_adr, xpm_image_t tail_down_img,uint8_t* tail_up_adr, xpm_image_t tail_up_img ){

  for(size_t i=0;i<N;i++){
    for(size_t j=0;j<M;j++){
        if (Field[i][j].Head){
          if (Field[i][j].dir==RIGHT)
            draw_xpm(head_right_adr,head_right_img, i*SPEED, j*SPEED);
          else if (Field[i][j].dir==LEFT)
            draw_xpm(head_left_adr,head_left_img, i*SPEED, j*SPEED);
          else if (Field[i][j].dir==UP)
            draw_xpm(head_up_adr,head_up_img, i*SPEED, j*SPEED);
          else if (Field[i][j].dir==DOWN)
            draw_xpm(head_down_adr,head_down_img, i*SPEED, j*SPEED);
        }
        else if (Field[i][j].Body){
          if(Field[i][j].dir==RIGHT || Field[i][j].dir==LEFT)
            draw_xpm(body_h_adr,body_h_img, i*SPEED, j*SPEED);
          else if(Field[i][j].dir==UP || Field[i][j].dir==DOWN)
            draw_xpm(body_v_adr,body_v_img, i*SPEED, j*SPEED);
        }
        else if (Field[i][j].Tail){
            if (Field[i][j].dir==RIGHT)
            draw_xpm(tail_right_adr,tail_right_img, i*SPEED, j*SPEED);
          else if (Field[i][j].dir==LEFT)
            draw_xpm(tail_left_adr,tail_left_img, i*SPEED, j*SPEED);
          else if (Field[i][j].dir==UP)
            draw_xpm(tail_up_adr,tail_up_img, i*SPEED, j*SPEED);
          else if (Field[i][j].dir==DOWN)
            draw_xpm(tail_down_adr,tail_down_img, i*SPEED, j*SPEED);
        }
    }
  }
  draw_buffer();
  
}

int movement(enum direction snake_dir){
  enum direction move_dir = newSnake.snake_dir;
  short nx = newSnake.x;
  short ny = newSnake.y;

  if (snake_dir==RIGHT){
    if (newSnake.snake_dir==LEFT){ //se carregar na seta direita e a snake estiver a andar para a esquerda continua a andar para a esquerda.
      move_dir=LEFT;
      nx--;
    }
    else{
      move_dir=RIGHT;
      nx++;
    }
  }
  else if (snake_dir==LEFT){
    if (newSnake.snake_dir==RIGHT){
      move_dir=RIGHT;
      nx++;
    }
    else{
      move_dir=LEFT;
      nx--;
    }
  }
  else if (snake_dir==UP){
    if (newSnake.snake_dir==DOWN){
      move_dir=DOWN;
      ny++;
    }
    else{
      move_dir=UP;
      ny--;
    }
  }
  else if (snake_dir==DOWN){
    if (newSnake.snake_dir==UP){
      move_dir=UP;
      ny--;
    }
    else{
      move_dir=DOWN;
      ny++;
    }
  }

  // Reject an off-grid step before touching the field so the head never
  // indexes outside Field.
  if (nx < 0 || nx >= N || ny < 0 || ny >= M){
    return 1;
  }

  Field[newSnake.x][newSnake.y].Head=false;
  Field[newSnake.x][newSnake.y].Body=true;
  Field[newSnake.x][newSnake.y].dir=move_dir;

  newSnake.x=nx;
  newSnake.y=ny;
  newSnake.snake_dir=move_dir;
  Field[newSnake.x][newSnake.y].dir=move_dir;
  Field[newSnake.x][newSnake.y].Head=true;

  if (Field[newSnake.x][newSnake.y].Body || Field[newSnake.x][newSnake.y].Tail){
    return 1;
  }

  if (newSnake.x>=50 || newSnake.y>=40 || newSnake.x<=0 || newSnake.y<=0){
    return 1;
  }
  return 0;
}

void UpdateTail(){
  int tmpi=-1,tmpj=-1;
  for(size_t i=0;i<N;i++){
    for(size_t j=0;j<M;j++){
      if(Field[i][j].Tail){
        Field[i][j].Tail=false;
        tmpi=i;
        tmpj=j;
        break;
      }
    }
  }
  if (tmpi < 0 || tmpj < 0)
    return;
  if (Field[tmpi][tmpj].dir==DOWN && tmpj+1 < M){
      Field[tmpi][tmpj+1].Body=false;
      Field[tmpi][tmpj+1].Tail=true;
      Field[tmpi][tmpj].dir=EMPTY;
  }
  else if (Field[tmpi][tmpj].dir==UP && tmpj-1 >= 0){
      Field[tmpi][tmpj-1].Body=false;
      Field[tmpi][tmpj-1].Tail=true;
      Field[tmpi][tmpj].dir=EMPTY;
  }
  else if (Field[tmpi][tmpj].dir==RIGHT && tmpi+1 < N){
      Field[tmpi+1][tmpj].Body=false;
      Field[tmpi+1][tmpj].Tail=true;
      Field[tmpi][tmpj].dir=EMPTY;
  }
  else if (Field[tmpi][tmpj].dir==LEFT && tmpi-1 >= 0){
      Field[tmpi-1][tmpj].Body=false;
      Field[tmpi-1][tmpj].Tail=true;
      Field[tmpi][tmpj].dir=EMPTY;
  }
}

struct Snake getNewSnake() {
    return newSnake;
}
