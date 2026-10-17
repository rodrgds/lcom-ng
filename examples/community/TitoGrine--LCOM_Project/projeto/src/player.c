#include "player.h"


player* create_empty_player() {

  // allocate memory for the player struct
  player* p = malloc(sizeof(*p));

  // if the allocation fails
  if ( p == NULL ) {
    return NULL;
  }

  // initialize the player's members
  strcpy(p->name, "");
  p->points = 0;
  p->avatar = NULL;

  return p;
}

void delete_player(player* p) {
  free(p);
}

void set_name(player* p, char* name) {
  strcpy(p->name, name);
}

void set_avatar(player* p, Sprite* avatar) {
  p->avatar = avatar;
}

void update_points(player* p, int points) {

  // because the player's points cannot be negative there is the need
  // to set them to 0 in case the absolute value to decrement is superior
  // to the points of the player
  if ( points < 0 && (unsigned int) abs(points) > p->points ) {
    p->points = 0;
  }
  else {
    p->points += points;
  }
}

