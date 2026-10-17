#include "sprite.h"
Sprite *createSprite(xpm_image_t *image, int x, int y) {
  Sprite *s = (Sprite *) malloc(sizeof(Sprite));
  s->x = x;
  s->y = y;
  s->realx = (double) x;
  s->realy = (double) y;
  //s->mx = 0;
  //s->my = 0;
  s->speedX = 0;
  s->speedY = 0;
  //s->mspeedX = 0;
  //s->mspeedY= 0;
  s->image = image;
  s->arr = image->bytes;
  s->isMovementUnique = 0;
  s->ignoresTime = 0;
  s->rotation = 0;
  return s;
}

void getCenter(Sprite *sprite, int *x, int *y) {
  *x = sprite->x + sprite->image->width / 2;
  *y = sprite->y + sprite->image->height / 2;
}

double getAngle(int x, int y) {
  const double PI = 3.1415926;
  double distance = sqrt(x * x + y * y);
  double angle = acos(((double) x) / (distance));
  if (y < 0) {
    angle = 2 * PI - angle;
  }
  return angle;
}

double getRotation(Sprite *source, unsigned x, unsigned y) {
  int centerX, centerY;
  getCenter(source, &centerX, &centerY);

  int intx = x - centerX, inty = y - centerY;
  return getAngle(intx, inty);
}

void getRotationCoordinate(Sprite *sprite, int oldx, int oldy, int *newx, int *newy) {
  int centerX, centerY;
  getCenter(sprite, &centerX, &centerY);
  int x = oldx - centerX;
  int y = oldy - centerY;
  double distance = sqrt(x * x + y * y);
  double angle = getAngle(x, y);

  angle = angle + sprite->rotation;

  double xRotated = distance * cos(angle);
  double yRotated = distance * sin(angle);

  y = (int) round(yRotated);
  x = (int) round(xRotated);

  *newx = (x + (int) centerX);
  *newy = (y + (int) centerY);
}

void getUnrotatedCoordinate(Sprite *sprite, int oldx, int oldy, int *newx, int *newy) {
  int centerX, centerY;
  getCenter(sprite, &centerX, &centerY);
  int x = oldx - centerX;
  int y = oldy - centerY;
  double distance = sqrt(x * x + y * y);
  double angle = getAngle(x, y);

  angle = angle - sprite->rotation;

  double xRotated = distance * cos(angle);
  double yRotated = distance * sin(angle);

  y = (int) round(yRotated);
  x = (int) round(xRotated);

  *newx = (x + (int) centerX);
  *newy = (y + (int) centerY);
}

void setSpeed(Sprite *sprite, int speed, int vectorX, int vectorY) {

  //Get vector's direction
  double angle = getAngle(vectorX, vectorY);
  sprite->rotation = angle;
  //Assign x value and y value of speed
  sprite->speedX = speed * cos(angle);
  sprite->speedY = speed * sin(angle);

  /*
  sprite->speedX = (int) speedX;
  sprite->speedY = (int) speedY;
  sprite->mspeedX = (int) (speedX * 1000 - sprite->speedX * 1000);
  sprite->mspeedY = (int) (speedY * 1000 - sprite->speedY * 1000);
  */
}

int intcompare(const void *a, const void *b) {
  return (*(int *) a - *(int *) b);
}

int checkColision(Sprite *a, Sprite *b) {
  /*
(rect1.x < rect2.x + rect2.width &&
   rect1.x + rect1.width > rect2.x &&
   rect1.y < rect2.y + rect2.height &&
   rect1.y + rect1.height > rect2.y)
*/
  int ax1 = a->x, ax2 = ax1 + a->image->width, ay1 = a->y, ay2 = ay1 + a->image->height,
      bx1 = b->x, bx2 = bx1 + b->image->width, by1 = b->y, by2 = by1 + b->image->height;

  if (ax1 < bx2 &&
      ax2 > bx1 &&
      ay1 < by2 &&
      ay2 > by1) {
          /*
    //Check pixels
    int yarr[] = {ay1, ay2, by1, by2};
    int xarr[] = {ax1, ax2, bx1, bx2};
    qsort(yarr, 4, sizeof(int), intcompare);
    qsort(xarr, 4, sizeof(int), intcompare);

    int x1 = xarr[1], x2 = xarr[2], y1 = yarr[1], y2 = yarr[2];

    for (int y = y1; y <= y2; y++) {
      for (int x = x1; x <= x2; x++) {
        int ax, ay, bx, by;
        getUnrotatedCoordinate(a, x, y, &ax, &ay);
        getUnrotatedCoordinate(b, x, y, &bx, &by);
        uint32_t acolor = getColor(a, ax, ay);
        uint32_t bcolor = getColor(b, bx, by);

        if (acolor != 0xFFFFFF && bcolor != 0xFFFFFF) {
          return 1;
        }
      }
    }
    */
    return 1;
  }
  return 0;
}

uint32_t getColor(Sprite *sprite, int x, int y) {
  uint32_t color;
  x = x - sprite->x;
  y = y - sprite->y;
  if(x < 0 || y < 0){
      printf("%d %d %d %d || %d %d\n", sprite->x, sprite->y, x + sprite->x, y + sprite->y, x, y);
      printf("ERROOOOO\n");
      return 0xFFFFFF;
  }
  
  int position = (x + y * sprite->image->width) * 3;
  if(position >= sprite->image->height * sprite->image->width * 3){
      printf("Esta cena aconteceu\n");
      return 0xFFFFFF;
  }
  memcpy(&color, &sprite->image->bytes[position], 3);
  return color;
}

void keepInsideScreen(int x_res, int y_res, Sprite *sprite){
  if(sprite->x < 0){
    sprite->x = 0;
    sprite->realx = 0.0;
  } else if(sprite->x + sprite->image->width >= x_res){
    sprite-> x = x_res - 1 - sprite->image->width;
    sprite->realx = (double) sprite->x;
  }

  if(sprite->y < 0 ){
    sprite->y = 0;
    sprite->realy = 0.0;
  } else if(sprite->y + sprite->image->height>= y_res){
    sprite->y = y_res - 1 - sprite->image->height;
    sprite->realy = (double) sprite->y ;
  }
}
