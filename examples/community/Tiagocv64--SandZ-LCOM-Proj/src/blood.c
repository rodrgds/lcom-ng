#include <lcom/lcf.h>
#include "Bitmap.h"
#include "blood.h"

Bitmap* blood;

struct Blood list_blood[20];

extern unsigned long long counter;

void init_blood(){
  struct Blood bld;
  bld.bloodExists = false;

  for(int i=0; i < 20; i++)
    list_blood[i] = bld;
  blood = loadBitmap("/home/lcom/labs/lcom1819-t4g13/proj/assets/blood.bmp");

}

void destroy_blood(){
  deleteBitmap(blood);
}

void create_blood(uint32_t x, uint32_t y){
  struct Blood bld;
  bld.x = x;
  bld.y = y;
  bld.timer = counter + (4 * 60);
  bld.bloodExists = true;

  for (int i = 0; i < 20; i++){
    if (!list_blood[i].bloodExists){
      list_blood[i] = bld;
      return;
    }
  }
}

void draw_blood(){
   for (int i = 0; i < 20; i++){
    if (list_blood[i].bloodExists)
      drawBitmap(blood, list_blood[i].x - blood->bitmapInfoHeader.width/2, list_blood[i].y - blood->bitmapInfoHeader.height/2);
   }
}

void update_blood(){
  for(int i = 0; i < 20; i++){
    if(list_blood[i].bloodExists)
      if(counter == list_blood[i].timer)
        list_blood[i].bloodExists = false;
  }
}
