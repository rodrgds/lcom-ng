#include ENTITIES_SOURCE
#include <assert.h>
#include <limits.h>
uint8_t scancode;int counter,timer_update;
int main(void){timer_update=1;counter=23;enemy_init();enemy_update();assert(enemy_count==0);counter=24;enemy_update();assert(enemy_count==1&&enemy_list[0].y>=2&&enemy_list[0].y<=10);free(enemy_list);enemy_init();
 const int elapsed[]={24,25,26,60,INT_MAX};for(unsigned n=0;n<sizeof elapsed/sizeof elapsed[0];n++){timer_update=elapsed[n];counter=1;enemy_update();assert(enemy_count==1);assert(enemy_list[0].x>=0&&enemy_list[0].x<864);free(enemy_list);enemy_init();}return 0;}
