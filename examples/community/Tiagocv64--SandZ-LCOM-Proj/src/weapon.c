#include "weapon.h"

struct Weapon shotgun;
struct Weapon assault_rifle;
struct Weapon sniper;

void init_weapons(){

  assault_rifle.id = 0;
  assault_rifle.damage = 70;
  assault_rifle.max_magazine = 30;
  assault_rifle.current_magazine = 30;

  shotgun.id = 1;
  shotgun.damage = 80;
  shotgun.max_magazine = 5;
  shotgun.current_magazine = 5;

  sniper.id = 2;
  sniper.damage = 70;
  sniper.max_magazine = 2;
  sniper.current_magazine = 2;
}
