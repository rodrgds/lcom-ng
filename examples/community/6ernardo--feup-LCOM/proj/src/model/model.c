#include "model.h"
#include "xpm/mouse.xpm"
#include "xpm/buttonQuit.xpm"
#include "xpm/buttonDraw.xpm"
#include "xpm/pencil.xpm"
#include "xpm/numberZero.xpm"
#include "xpm/numberOne.xpm"
#include "xpm/numberTwo.xpm"
#include "xpm/numberThree.xpm"
#include "xpm/numberFour.xpm"
#include "xpm/numberFive.xpm"
#include "xpm/numberSix.xpm"
#include "xpm/numberSeven.xpm"
#include "xpm/numberEight.xpm"
#include "xpm/numberNine.xpm"
#include "xpm/colon.xpm"
#include "xpm/eraser.xpm"
#include "xpm/hand.xpm"
#include "xpm/bucket.xpm"
#include "xpm/instructions.xpm"
#include "xpm/erasercursor.xpm"
#include "xpm/pencilcursor.xpm"
#include "xpm/bucketcursor.xpm"
#include "xpm/outline.xpm"
#include "xpm/buttonHelp.xpm"
#include "xpm/vsdraw.xpm"
#include "xpm/letterB.xpm"
#include "xpm/Inst1.xpm"
#include "xpm/Inst2.xpm"
#include "xpm/arrow_next.xpm"
#include "xpm/arrow_previous.xpm"
Sprite *mouse;
Sprite *button_quit;
Sprite *button_draw;
Sprite *pencil_draw;
Sprite *number_zero;
Sprite *number_one;
Sprite *number_two;
Sprite *number_three;
Sprite *number_four;
Sprite *number_five;
Sprite *number_six;
Sprite *number_seven;
Sprite *number_eight;
Sprite *number_nine;
Sprite *colon;
Sprite *eraser;
Sprite *hand;
Sprite *bucket;
Sprite *instructions;
Sprite *button_help;
Sprite *vsdraw;
Sprite *letter_b;
Sprite *inst1;
Sprite *inst2;
Sprite *arrow_next;
Sprite *arrow_previous;

Sprite *eraser_cursor;
Sprite *pencil_cursor;
Sprite *bucket_cursor;
Sprite *outline_tool;

void setupsprites(){
  mouse=create_sprite_xpm((xpm_map_t)mouse_xpm);
  button_quit=create_sprite_xpm((xpm_map_t)buttonQuit_xpm);
  button_draw=create_sprite_xpm((xpm_map_t)buttonDraw_xpm);
  pencil_draw=create_sprite_xpm((xpm_map_t)pencil_xpm);
  number_zero=create_sprite_xpm((xpm_map_t)zero_xpm);
  number_one=create_sprite_xpm((xpm_map_t)one_xpm);
  number_two=create_sprite_xpm((xpm_map_t)two_xpm);
  number_three=create_sprite_xpm((xpm_map_t)three_xpm);
  number_four=create_sprite_xpm((xpm_map_t)four_xpm);
  number_five=create_sprite_xpm((xpm_map_t)five_xpm);
  number_six=create_sprite_xpm((xpm_map_t)six_xpm);
  number_seven=create_sprite_xpm((xpm_map_t)seven_xpm);
  number_eight=create_sprite_xpm((xpm_map_t)eight_xpm);
  number_nine=create_sprite_xpm((xpm_map_t)nine_xpm);
  colon=create_sprite_xpm((xpm_map_t)colon_xpm);
  eraser=create_sprite_xpm((xpm_map_t)eraser_xpm);
  hand=create_sprite_xpm((xpm_map_t)hand_xpm);
  bucket=create_sprite_xpm((xpm_map_t)bucket_xpm);
  instructions=create_sprite_xpm((xpm_map_t)instructions_xpm);
  eraser_cursor=create_sprite_xpm((xpm_map_t)erasercursor_xpm);
  pencil_cursor=create_sprite_xpm((xpm_map_t)pencilcursor_xpm);
  bucket_cursor=create_sprite_xpm((xpm_map_t)bucketcursor_xpm);
  outline_tool=create_sprite_xpm((xpm_map_t)outline_xpm);
  button_help=create_sprite_xpm((xpm_map_t)buttonHelp_xpm);
  vsdraw=create_sprite_xpm((xpm_map_t)vsdraw_xpm);
  letter_b=create_sprite_xpm((xpm_map_t)letter_b_xpm);
  inst1=create_sprite_xpm((xpm_map_t)Inst1_xpm);
  inst2=create_sprite_xpm((xpm_map_t)Inst2_xpm);
  arrow_next=create_sprite_xpm((xpm_map_t)arrow_next_xpm);
  arrow_previous=create_sprite_xpm((xpm_map_t)arrow_previous_xpm);
}
void destroy_sprites(){
  destroy_sprite(mouse);
  destroy_sprite(button_quit);
  destroy_sprite(button_draw);
  destroy_sprite(pencil_draw);
  destroy_sprite(number_one);
  destroy_sprite(number_two);
  destroy_sprite(number_three);
  destroy_sprite(number_four);
  destroy_sprite(number_five);
  destroy_sprite(number_six);
  destroy_sprite(number_seven);
  destroy_sprite(number_eight);
  destroy_sprite(number_zero);
  destroy_sprite(number_nine);
  destroy_sprite(colon);
  destroy_sprite(eraser);
  destroy_sprite(bucket);
  destroy_sprite(instructions);

}
