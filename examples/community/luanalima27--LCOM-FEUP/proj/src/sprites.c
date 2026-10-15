#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "pixmapproj.h"
#include "sprites.h"

//Sprites will load at the start of the execution taking in acount the hours 

//Blacks and White means clear in their respective modes
uint8_t *spaceship_sprite;
uint8_t *spaceship_black_sprite;
uint8_t *gameover_sprite;
uint8_t *win_sprite;
uint8_t *win_black_sprite;
uint8_t *gameover_black_sprite;
uint8_t *start_sprite;
uint8_t *selected_start_sprite;
uint8_t *start_black_sprite;
uint8_t *quit_sprite;
uint8_t *selected_quit_sprite;
uint8_t *quit_black_sprite;
uint8_t *cursor_sprite;
uint8_t *cursor_black_sprite;
uint8_t *alien_sprite;
uint8_t *alien_black_sprite;
uint8_t *target_sprite;
uint8_t *target_black_sprite;
uint8_t *spaceship_projectile_sprite;
uint8_t *spaceship_projectile_black_sprite;
uint8_t *alien_projectile_sprite;
uint8_t *alien_projectile_black_sprite;
xpm_image_t img_spaceship_sprite;
xpm_image_t img_spaceship_black_sprite;
xpm_image_t img_gameover_sprite;
xpm_image_t img_win_sprite;
xpm_image_t img_gameover_black_sprite;
xpm_image_t img_win_black_sprite;
xpm_image_t img_start_sprite;
xpm_image_t img_selected_start_sprite;
xpm_image_t img_start_black_sprite;
xpm_image_t img_quit_sprite;
xpm_image_t img_selected_quit_sprite;
xpm_image_t img_quit_black_sprite;
xpm_image_t img_cursor_sprite;
xpm_image_t img_cursor_black_sprite;
xpm_image_t img_alien_sprite;
xpm_image_t img_alien_black_sprite;
xpm_image_t img_target_sprite;
xpm_image_t img_target_black_sprite;
xpm_image_t img_spaceship_projectile_sprite;
xpm_image_t img_spaceship_projectile_black_sprite;
xpm_image_t img_alien_projectile_sprite;
xpm_image_t img_alien_projectile_black_sprite;


uint8_t *exclamationw_sprite;
uint8_t *start_sprite;
uint8_t *percentagew_sprite;
uint8_t *andw_sprite;
uint8_t *leftwhite_sprite;
uint8_t *rightbracketw_sprite;
uint8_t *commaw_sprite;
uint8_t *semicolonw_sprite;
uint8_t *zerow_sprite;
uint8_t *onew_sprite;
uint8_t *twow_sprite;
uint8_t *threew_sprite;
uint8_t *fourw_sprite;
uint8_t *fivew_sprite;
uint8_t *sixw_sprite;
uint8_t *sevenw_sprite;
uint8_t *eightw_sprite;
uint8_t *ninew_sprite;
uint8_t *Aw_sprite;
uint8_t *Bw_sprite;
uint8_t *Cw_sprite;
uint8_t *Çw_sprite;
uint8_t *Dw_sprite;
uint8_t *Ew_sprite;
uint8_t *Fw_sprite;
uint8_t *Gw_sprite;
uint8_t *Hw_sprite;
uint8_t *Iw_sprite;
uint8_t *Jw_sprite;
uint8_t *Kw_sprite;
uint8_t *law_sprite;
uint8_t *lbw_sprite;
uint8_t *lcw_sprite;
uint8_t *lçw_sprite;
uint8_t *ldw_sprite;
uint8_t *lew_sprite;
uint8_t *lfw_sprite;
uint8_t *lgw_sprite;
uint8_t *lhw_sprite;
uint8_t *liw_sprite;
uint8_t *ljw_sprite;
uint8_t *lkw_sprite;
uint8_t *llw_sprite;
uint8_t *lmw_sprite;
uint8_t *lnw_sprite;
uint8_t *low_sprite;
uint8_t *lpw_sprite;
uint8_t *lqw_sprite;
uint8_t *lrw_sprite;
uint8_t *lsw_sprite;
uint8_t *ltw_sprite;
uint8_t *luw_sprite;
uint8_t *lvw_sprite;
uint8_t *Lw_sprite;
uint8_t *lww_sprite;
uint8_t *lxw_sprite;
uint8_t *lyw_sprite;
uint8_t *lzw_sprite;
uint8_t *Mw_sprite;
uint8_t *Nw_sprite;
uint8_t *Ow_sprite;
uint8_t *Pw_sprite;
uint8_t *questionmarkw_sprite;
uint8_t *Qw_sprite;
uint8_t *Rw_sprite;
uint8_t *sidebarw_sprite;
uint8_t *Sw_sprite;
uint8_t *Tw_sprite;
uint8_t *twodotsw_sprite;
uint8_t *Uw_sprite;
uint8_t *Vw_sprite;
uint8_t *Ww_sprite;
uint8_t *Xw_sprite;
uint8_t *Yw_sprite;
uint8_t *Zw_sprite;
uint8_t *eraseblackletters_sprite;
uint8_t *erasewhiteletters_sprite;
uint8_t *dotw_sprite;
xpm_image_t img_exclamationw_sprite;
xpm_image_t img_start_sprite;
xpm_image_t img_percentagew_sprite;
xpm_image_t img_andw_sprite;
xpm_image_t img_leftwhite_sprite;
xpm_image_t img_rightbracketw_sprite;
xpm_image_t img_commaw_sprite;
xpm_image_t img_semicolonw_sprite;
xpm_image_t img_zerow_sprite;
xpm_image_t img_onew_sprite;
xpm_image_t img_twow_sprite;
xpm_image_t img_threew_sprite;
xpm_image_t img_fourw_sprite;
xpm_image_t img_fivew_sprite;
xpm_image_t img_sixw_sprite;
xpm_image_t img_sevenw_sprite;
xpm_image_t img_eightw_sprite;
xpm_image_t img_ninew_sprite;
xpm_image_t img_Aw_sprite;
xpm_image_t img_Bw_sprite;
xpm_image_t img_Cw_sprite;
xpm_image_t img_Çw_sprite;
xpm_image_t img_Dw_sprite;
xpm_image_t img_Ew_sprite;
xpm_image_t img_Fw_sprite;
xpm_image_t img_Gw_sprite;
xpm_image_t img_Hw_sprite;
xpm_image_t img_Iw_sprite;
xpm_image_t img_Jw_sprite;
xpm_image_t img_Kw_sprite;
xpm_image_t img_law_sprite;
xpm_image_t img_lbw_sprite;
xpm_image_t img_lcw_sprite;
xpm_image_t img_lçw_sprite;
xpm_image_t img_ldw_sprite;
xpm_image_t img_lew_sprite;
xpm_image_t img_lfw_sprite;
xpm_image_t img_lgw_sprite;
xpm_image_t img_lhw_sprite;
xpm_image_t img_liw_sprite;
xpm_image_t img_ljw_sprite;
xpm_image_t img_lkw_sprite;
xpm_image_t img_llw_sprite;
xpm_image_t img_lmw_sprite;
xpm_image_t img_lnw_sprite;
xpm_image_t img_low_sprite;
xpm_image_t img_lpw_sprite;
xpm_image_t img_lqw_sprite;
xpm_image_t img_lrw_sprite;
xpm_image_t img_lsw_sprite;
xpm_image_t img_ltw_sprite;
xpm_image_t img_luw_sprite;
xpm_image_t img_lvw_sprite;
xpm_image_t img_Lw_sprite;
xpm_image_t img_lww_sprite;
xpm_image_t img_lxw_sprite;
xpm_image_t img_lyw_sprite;
xpm_image_t img_lzw_sprite;
xpm_image_t img_Mw_sprite;
xpm_image_t img_Nw_sprite;
xpm_image_t img_Ow_sprite;
xpm_image_t img_Pw_sprite;
xpm_image_t img_questionmarkw_sprite;
xpm_image_t img_Qw_sprite;
xpm_image_t img_Rw_sprite;
xpm_image_t img_sidebarw_sprite;
xpm_image_t img_Sw_sprite;
xpm_image_t img_Tw_sprite;
xpm_image_t img_twodotsw_sprite;
xpm_image_t img_Uw_sprite;
xpm_image_t img_Vw_sprite;
xpm_image_t img_Ww_sprite;
xpm_image_t img_Xw_sprite;
xpm_image_t img_Yw_sprite;
xpm_image_t img_Zw_sprite;
xpm_image_t img_eraseblackletters_sprite;
xpm_image_t img_erasewhiteletters_sprite;
xpm_image_t img_dotw_sprite;

uint8_t *mmbg_sprite;
xpm_image_t img_mmbg;
uint8_t * erasebg_sprite;
xpm_image_t img_erasebg;

uint8_t * rightw_sprite;
xpm_image_t img_rightw_sprite;
uint8_t * leftw_sprite;
xpm_image_t img_leftw_sprite;

uint8_t *back_sprite;
uint8_t *selected_back_sprite;
uint8_t *back_black_sprite;
xpm_image_t img_back_sprite;
xpm_image_t img_selected_back_sprite;
xpm_image_t img_back_black_sprite;

uint8_t *easy_sprite;
uint8_t *selected_easy_sprite;
uint8_t *easy_black_sprite;
xpm_image_t img_easy_sprite;
xpm_image_t img_selected_easy_sprite;
xpm_image_t img_easy_black_sprite;

uint8_t *hard_sprite;
uint8_t *selected_hard_sprite;
uint8_t *hard_black_sprite;
xpm_image_t img_hard_sprite;
xpm_image_t img_selected_hard_sprite;
xpm_image_t img_hard_black_sprite;

uint8_t *medium_sprite;
uint8_t *selected_medium_sprite;
uint8_t *medium_black_sprite;
xpm_image_t img_medium_sprite;
xpm_image_t img_selected_medium_sprite;
xpm_image_t img_medium_black_sprite;

void(load_xpm)() {
  extern uint8_t hours;

    selected_start_sprite = xpm_load(selected_start, XPM_8_8_8, &img_selected_start_sprite);
    selected_quit_sprite = xpm_load(selected_quit, XPM_8_8_8, &img_selected_quit_sprite);
    start_sprite = xpm_load(start, XPM_8_8_8, &img_start_sprite);



    
    win_sprite = xpm_load(win_white, XPM_8_8_8, &img_win_sprite);
    selected_start_sprite = xpm_load(selected_start, XPM_8_8_8, &img_selected_start_sprite);
    win_black_sprite = xpm_load(win_black, XPM_8_8_8, &img_win_black_sprite);
    start_sprite = xpm_load(start, XPM_8_8_8, &img_start_sprite);
    quit_sprite = xpm_load(quit, XPM_8_8_8, &img_quit_sprite);
    easy_sprite = xpm_load(easy, XPM_8_8_8, &img_easy_sprite);
    medium_sprite = xpm_load(medium, XPM_8_8_8, &img_medium_sprite);
    hard_sprite = xpm_load(hard, XPM_8_8_8, &img_hard_sprite);
    back_sprite = xpm_load(back, XPM_8_8_8, &img_back_sprite);
    selected_easy_sprite = xpm_load(selected_easy, XPM_8_8_8, &img_selected_easy_sprite);
    selected_medium_sprite = xpm_load(selected_medium, XPM_8_8_8, &img_selected_medium_sprite);
    selected_hard_sprite = xpm_load(selected_hard, XPM_8_8_8, &img_selected_hard_sprite);
    selected_back_sprite = xpm_load(selected_back, XPM_8_8_8, &img_selected_back_sprite);
    



    eraseblackletters_sprite = xpm_load(eraseblackletters, XPM_8_8_8, &img_eraseblackletters_sprite);
    

  if ((hours < 6 || hours > 20)){
      spaceship_sprite = xpm_load(spaceship, XPM_8_8_8, &img_spaceship_sprite);
      spaceship_black_sprite = xpm_load(black_spaceship, XPM_8_8_8, &img_spaceship_black_sprite); 

      alien_sprite = xpm_load(green_alien, XPM_8_8_8, &img_alien_sprite);
      alien_black_sprite = xpm_load(black_alien, XPM_8_8_8, &img_alien_black_sprite);
      target_sprite = xpm_load(target, XPM_8_8_8, &img_target_sprite);
      target_black_sprite = xpm_load(target_black, XPM_8_8_8, &img_target_black_sprite);
      spaceship_projectile_sprite = xpm_load(spaceship_projectile, XPM_8_8_8, &img_spaceship_projectile_sprite);
      spaceship_projectile_black_sprite = xpm_load(spaceship_projectile_black, XPM_8_8_8, &img_spaceship_projectile_black_sprite);
      alien_projectile_sprite = xpm_load(alien_projectile, XPM_8_8_8, &img_alien_projectile_sprite);
      alien_projectile_black_sprite = xpm_load(alien_projectile_black, XPM_8_8_8, &img_alien_projectile_black_sprite);

      cursor_sprite = xpm_load(cursor, XPM_8_8_8, &img_cursor_sprite);
      cursor_black_sprite = xpm_load(black_cursor, XPM_8_8_8, &img_cursor_black_sprite);

      start_black_sprite = xpm_load(black_start, XPM_8_8_8, &img_start_black_sprite);
      quit_black_sprite = xpm_load(black_quit, XPM_8_8_8, &img_quit_black_sprite);

      gameover_sprite = xpm_load(gameover_white, XPM_8_8_8, &img_gameover_sprite);
      gameover_black_sprite = xpm_load(gameover_black, XPM_8_8_8, &img_gameover_black_sprite);

      win_sprite = xpm_load(win_white, XPM_8_8_8, &img_win_sprite);
      win_black_sprite = xpm_load(win_black, XPM_8_8_8, &img_win_black_sprite);

      back_black_sprite = xpm_load(black_back, XPM_8_8_8, &img_back_black_sprite);
      easy_black_sprite = xpm_load(black_easy, XPM_8_8_8, &img_easy_black_sprite);
      medium_black_sprite = xpm_load(black_medium, XPM_8_8_8, &img_medium_black_sprite);
      hard_black_sprite = xpm_load(black_hard, XPM_8_8_8, &img_hard_black_sprite);



      dotw_sprite = xpm_load(dotw, XPM_8_8_8, &img_dotw_sprite);

      //nonletter char
      exclamationw_sprite = xpm_load(exclamationw, XPM_8_8_8, &img_exclamationw_sprite);
      percentagew_sprite = xpm_load(percentagew, XPM_8_8_8, &img_percentagew_sprite);
      andw_sprite = xpm_load(andw, XPM_8_8_8, &img_andw_sprite);
      semicolonw_sprite = xpm_load(semicolonw, XPM_8_8_8, &img_semicolonw_sprite);
      leftwhite_sprite = xpm_load(leftwhite, XPM_8_8_8, &img_leftwhite_sprite);
      commaw_sprite = xpm_load(commaw, XPM_8_8_8, &img_commaw_sprite);
      rightbracketw_sprite = xpm_load(rightbracketw, XPM_8_8_8, &img_rightbracketw_sprite);
      zerow_sprite = xpm_load(zerow, XPM_8_8_8, &img_zerow_sprite);
      onew_sprite = xpm_load(onew, XPM_8_8_8, &img_onew_sprite);
      twow_sprite = xpm_load(twow, XPM_8_8_8, &img_twow_sprite);
      threew_sprite = xpm_load(threew, XPM_8_8_8, &img_threew_sprite);
      fourw_sprite = xpm_load(fourw, XPM_8_8_8, &img_fourw_sprite);
      fivew_sprite = xpm_load(fivew, XPM_8_8_8, &img_fivew_sprite);
      sixw_sprite = xpm_load(sixw, XPM_8_8_8, &img_sixw_sprite);
      sevenw_sprite = xpm_load(sevenw, XPM_8_8_8, &img_sevenw_sprite);
      eightw_sprite = xpm_load(eightw, XPM_8_8_8, &img_eightw_sprite);
      ninew_sprite = xpm_load(ninew, XPM_8_8_8, &img_ninew_sprite);
      
      //letters
      Aw_sprite = xpm_load(Aw, XPM_8_8_8, &img_Aw_sprite);
      Bw_sprite = xpm_load(Bw, XPM_8_8_8, &img_Bw_sprite);
      Cw_sprite = xpm_load(Cw, XPM_8_8_8, &img_Cw_sprite);
      Çw_sprite = xpm_load(Çw, XPM_8_8_8, &img_Çw_sprite);
      Dw_sprite = xpm_load(Dw, XPM_8_8_8, &img_Dw_sprite);
      Ew_sprite = xpm_load(Ew, XPM_8_8_8, &img_Ew_sprite);
      Fw_sprite = xpm_load(Fw, XPM_8_8_8, &img_Fw_sprite);
      Gw_sprite = xpm_load(Gw, XPM_8_8_8, &img_Gw_sprite);
      Hw_sprite = xpm_load(Hw, XPM_8_8_8, &img_Hw_sprite);
      Iw_sprite = xpm_load(Iw, XPM_8_8_8, &img_Iw_sprite);
      Jw_sprite = xpm_load(Jw, XPM_8_8_8, &img_Jw_sprite);
      Kw_sprite = xpm_load(Kw, XPM_8_8_8, &img_Kw_sprite);
      law_sprite = xpm_load(law, XPM_8_8_8, &img_law_sprite);
      lbw_sprite = xpm_load(lbw, XPM_8_8_8, &img_lbw_sprite);
      lcw_sprite = xpm_load(lcw, XPM_8_8_8, &img_lcw_sprite);
      lçw_sprite = xpm_load(lçw, XPM_8_8_8, &img_lçw_sprite);
      ldw_sprite = xpm_load(ldw, XPM_8_8_8, &img_ldw_sprite);
      lew_sprite = xpm_load(lew, XPM_8_8_8, &img_lew_sprite);
      lfw_sprite = xpm_load(lfw, XPM_8_8_8, &img_lfw_sprite);
      lgw_sprite = xpm_load(lgw, XPM_8_8_8, &img_lgw_sprite);
      lhw_sprite = xpm_load(lhw, XPM_8_8_8, &img_lhw_sprite);
      liw_sprite = xpm_load(liw, XPM_8_8_8, &img_liw_sprite);
      ljw_sprite = xpm_load(ljw, XPM_8_8_8, &img_ljw_sprite);
      lkw_sprite = xpm_load(lkw, XPM_8_8_8, &img_lkw_sprite);
      llw_sprite = xpm_load(llw, XPM_8_8_8, &img_llw_sprite);
      lmw_sprite = xpm_load(lmw, XPM_8_8_8, &img_lmw_sprite);
      lnw_sprite = xpm_load(lnw, XPM_8_8_8, &img_lnw_sprite);
      low_sprite = xpm_load(low, XPM_8_8_8, &img_low_sprite);
      lpw_sprite = xpm_load(lpw, XPM_8_8_8, &img_lpw_sprite);
      lqw_sprite = xpm_load(lqw, XPM_8_8_8, &img_lqw_sprite);
      lrw_sprite = xpm_load(lrw, XPM_8_8_8, &img_lrw_sprite);
      lsw_sprite = xpm_load(lsw, XPM_8_8_8, &img_lsw_sprite);
      ltw_sprite = xpm_load(ltw, XPM_8_8_8, &img_ltw_sprite);
      luw_sprite = xpm_load(luw, XPM_8_8_8, &img_luw_sprite);
      lvw_sprite = xpm_load(lvw, XPM_8_8_8, &img_lvw_sprite);
      Lw_sprite = xpm_load(Lw, XPM_8_8_8, &img_Lw_sprite);
      lww_sprite = xpm_load(lww, XPM_8_8_8, &img_lww_sprite);
      lxw_sprite = xpm_load(lxw, XPM_8_8_8, &img_lxw_sprite);
      lyw_sprite = xpm_load(lyw, XPM_8_8_8, &img_lyw_sprite);
      lzw_sprite = xpm_load(lzw, XPM_8_8_8, &img_lzw_sprite);
      Mw_sprite = xpm_load(Mw, XPM_8_8_8, &img_Mw_sprite);
      Nw_sprite = xpm_load(Nw, XPM_8_8_8, &img_Nw_sprite);
      Ow_sprite = xpm_load(Ow, XPM_8_8_8, &img_Ow_sprite);
      Pw_sprite = xpm_load(Pw, XPM_8_8_8, &img_Pw_sprite);
      Qw_sprite = xpm_load(Qw, XPM_8_8_8, &img_Qw_sprite);
      Rw_sprite = xpm_load(Rw, XPM_8_8_8, &img_Rw_sprite);
      Sw_sprite = xpm_load(Sw, XPM_8_8_8, &img_Sw_sprite);
      Tw_sprite = xpm_load(Tw, XPM_8_8_8, &img_Tw_sprite);
      Uw_sprite = xpm_load(Uw, XPM_8_8_8, &img_Uw_sprite);
      Vw_sprite = xpm_load(Vw, XPM_8_8_8, &img_Vw_sprite);
      Ww_sprite = xpm_load(Ww, XPM_8_8_8, &img_Ww_sprite);
      Xw_sprite = xpm_load(Xw, XPM_8_8_8, &img_Xw_sprite);
      Yw_sprite = xpm_load(Yw, XPM_8_8_8, &img_Yw_sprite);
      Zw_sprite = xpm_load(Zw, XPM_8_8_8, &img_Zw_sprite);

      //background
    
    mmbg_sprite = xpm_load(mmdarkbg,XPM_8_8_8, &img_mmbg);
    erasebg_sprite = xpm_load(erasebgdark, XPM_8_8_8, &img_erasebg); //also works as game bg
    eraseblackletters_sprite = xpm_load(eraseblackletters, XPM_8_8_8, &img_eraseblackletters_sprite);


  }
  else{
    spaceship_sprite = xpm_load(spaceshiplight, XPM_8_8_8, &img_spaceship_sprite);
    spaceship_black_sprite = xpm_load(black_spaceshiplight, XPM_8_8_8, &img_spaceship_black_sprite);
    
    alien_sprite = xpm_load(green_alienlight, XPM_8_8_8, &img_alien_sprite);
    alien_black_sprite = xpm_load(black_alienlight, XPM_8_8_8, &img_alien_black_sprite);
    target_sprite = xpm_load(targetlight, XPM_8_8_8, &img_target_sprite);
    target_black_sprite = xpm_load(target_blacklight, XPM_8_8_8, &img_target_black_sprite);
    spaceship_projectile_sprite = xpm_load(spaceship_projectilelight, XPM_8_8_8, &img_spaceship_projectile_sprite);
    spaceship_projectile_black_sprite = xpm_load(spaceship_projectile_blacklight, XPM_8_8_8, &img_spaceship_projectile_black_sprite);
    alien_projectile_sprite = xpm_load(alien_projectilelight, XPM_8_8_8, &img_alien_projectile_sprite);
    alien_projectile_black_sprite = xpm_load(alien_projectile_blacklight, XPM_8_8_8, &img_alien_projectile_black_sprite);

    cursor_sprite = xpm_load(cursorlight, XPM_8_8_8, &img_cursor_sprite);
    cursor_black_sprite = xpm_load(black_cursorlight, XPM_8_8_8, &img_cursor_black_sprite);

    start_black_sprite = xpm_load(black_startlight, XPM_8_8_8, &img_start_black_sprite);
    quit_black_sprite = xpm_load(black_quitlight, XPM_8_8_8, &img_quit_black_sprite);

    gameover_sprite = xpm_load(gameover_whitelight, XPM_8_8_8, &img_gameover_sprite);
    gameover_black_sprite = xpm_load(gameover_blacklight, XPM_8_8_8, &img_gameover_black_sprite);

    win_sprite = xpm_load(win_whitelight, XPM_8_8_8, &img_win_sprite);
    win_black_sprite = xpm_load(win_blacklight, XPM_8_8_8, &img_win_black_sprite);

    back_black_sprite = xpm_load(black_backlight, XPM_8_8_8, &img_back_black_sprite);
    easy_black_sprite = xpm_load(black_easylight, XPM_8_8_8, &img_easy_black_sprite);
    medium_black_sprite = xpm_load(black_mediumlight, XPM_8_8_8, &img_medium_black_sprite);
    hard_black_sprite = xpm_load(black_hardlight, XPM_8_8_8, &img_hard_black_sprite);

    dotw_sprite = xpm_load(dotb, XPM_8_8_8, &img_dotw_sprite);

    //nonletterchars
    exclamationw_sprite = xpm_load(exclamationb, XPM_8_8_8, &img_exclamationw_sprite);
    percentagew_sprite = xpm_load(percentageb, XPM_8_8_8, &img_percentagew_sprite);
    semicolonw_sprite = xpm_load(semicolonb, XPM_8_8_8, &img_semicolonw_sprite);
    andw_sprite = xpm_load(andb, XPM_8_8_8, &img_andw_sprite);
    leftw_sprite = xpm_load(leftb, XPM_8_8_8, &img_leftw_sprite);
    rightw_sprite = xpm_load(rightb, XPM_8_8_8, &img_rightw_sprite);
    commaw_sprite = xpm_load(commab, XPM_8_8_8, &img_commaw_sprite);
    zerow_sprite = xpm_load(zerob, XPM_8_8_8, &img_zerow_sprite);
    onew_sprite = xpm_load(oneb, XPM_8_8_8, &img_onew_sprite);
    twow_sprite = xpm_load(twob, XPM_8_8_8, &img_twow_sprite);
    threew_sprite = xpm_load(threeb, XPM_8_8_8, &img_threew_sprite);
    fourw_sprite = xpm_load(fourb, XPM_8_8_8, &img_fourw_sprite);
    fivew_sprite = xpm_load(fiveb, XPM_8_8_8, &img_fivew_sprite);
    sixw_sprite = xpm_load(sixb, XPM_8_8_8, &img_sixw_sprite);
    sevenw_sprite = xpm_load(sevenb, XPM_8_8_8, &img_sevenw_sprite);
    eightw_sprite = xpm_load(eightb, XPM_8_8_8, &img_eightw_sprite);
    ninew_sprite = xpm_load(nineb, XPM_8_8_8, &img_ninew_sprite);

    //letters
    Aw_sprite = xpm_load(Ab, XPM_8_8_8, &img_Aw_sprite);
    Bw_sprite = xpm_load(Bb, XPM_8_8_8, &img_Bw_sprite);
    Cw_sprite = xpm_load(Cb, XPM_8_8_8, &img_Cw_sprite);
    Çw_sprite = xpm_load(Çb, XPM_8_8_8, &img_Çw_sprite);
    Dw_sprite = xpm_load(Db, XPM_8_8_8, &img_Dw_sprite);
    Ew_sprite = xpm_load(Eb, XPM_8_8_8, &img_Ew_sprite);
    Fw_sprite = xpm_load(Fb, XPM_8_8_8, &img_Fw_sprite);
    Gw_sprite = xpm_load(Gb, XPM_8_8_8, &img_Gw_sprite);
    Hw_sprite = xpm_load(Hb, XPM_8_8_8, &img_Hw_sprite);
    Iw_sprite = xpm_load(Ib, XPM_8_8_8, &img_Iw_sprite);
    Jw_sprite = xpm_load(Jb, XPM_8_8_8, &img_Jw_sprite);
    Kw_sprite = xpm_load(Kb, XPM_8_8_8, &img_Kw_sprite);
    law_sprite = xpm_load(lab, XPM_8_8_8, &img_law_sprite);
    Lw_sprite = xpm_load(Lb, XPM_8_8_8, &img_Lw_sprite);
    lbw_sprite = xpm_load(lbb, XPM_8_8_8, &img_lbw_sprite);
    lcw_sprite = xpm_load(lcb, XPM_8_8_8, &img_lcw_sprite);
    lçw_sprite = xpm_load(lçb, XPM_8_8_8, &img_lçw_sprite);
    ldw_sprite = xpm_load(ldb, XPM_8_8_8, &img_ldw_sprite);
    lew_sprite = xpm_load(leb, XPM_8_8_8, &img_lew_sprite);
    lfw_sprite = xpm_load(lfb, XPM_8_8_8, &img_lfw_sprite);
    lgw_sprite = xpm_load(lgb, XPM_8_8_8, &img_lgw_sprite);
    lhw_sprite = xpm_load(lhb, XPM_8_8_8, &img_lhw_sprite);
    liw_sprite = xpm_load(lib, XPM_8_8_8, &img_liw_sprite);
    ljw_sprite = xpm_load(ljb, XPM_8_8_8, &img_ljw_sprite);
    lkw_sprite = xpm_load(lkb, XPM_8_8_8, &img_lkw_sprite);
    llw_sprite = xpm_load(llb, XPM_8_8_8, &img_llw_sprite);
    lmw_sprite = xpm_load(lmb, XPM_8_8_8, &img_lmw_sprite);
    lnw_sprite = xpm_load(lnb, XPM_8_8_8, &img_lnw_sprite);
    low_sprite = xpm_load(lob, XPM_8_8_8, &img_low_sprite);
    lpw_sprite = xpm_load(lpb, XPM_8_8_8, &img_lpw_sprite);
    lqw_sprite = xpm_load(lqb, XPM_8_8_8, &img_lqw_sprite);
    lrw_sprite = xpm_load(lrb, XPM_8_8_8, &img_lrw_sprite);
    lsw_sprite = xpm_load(lsb, XPM_8_8_8, &img_lsw_sprite);
    ltw_sprite = xpm_load(ltb, XPM_8_8_8, &img_ltw_sprite);
    luw_sprite = xpm_load(lub, XPM_8_8_8, &img_luw_sprite);
    lvw_sprite = xpm_load(lvb, XPM_8_8_8, &img_lvw_sprite);
    lww_sprite = xpm_load(lwb, XPM_8_8_8, &img_lww_sprite);
    lxw_sprite = xpm_load(lxb, XPM_8_8_8, &img_lxw_sprite);
    lyw_sprite = xpm_load(lyb, XPM_8_8_8, &img_lyw_sprite);
    lzw_sprite = xpm_load(lzb, XPM_8_8_8, &img_lzw_sprite);
    Mw_sprite = xpm_load(Mb, XPM_8_8_8, &img_Mw_sprite);
    Nw_sprite = xpm_load(Nb, XPM_8_8_8, &img_Nw_sprite);
    Ow_sprite = xpm_load(Ob, XPM_8_8_8, &img_Ow_sprite);
    Pw_sprite = xpm_load(Pb, XPM_8_8_8, &img_Pw_sprite);
    Qw_sprite = xpm_load(Qb, XPM_8_8_8, &img_Qw_sprite);
    Rw_sprite = xpm_load(Rb, XPM_8_8_8, &img_Rw_sprite);
    Sw_sprite = xpm_load(Sb, XPM_8_8_8, &img_Sw_sprite);
    Tw_sprite = xpm_load(Tb, XPM_8_8_8, &img_Tw_sprite);
    Uw_sprite = xpm_load(Ub, XPM_8_8_8, &img_Uw_sprite);
    Vw_sprite = xpm_load(Vb, XPM_8_8_8, &img_Vw_sprite);
    Ww_sprite = xpm_load(Wb, XPM_8_8_8, &img_Ww_sprite);
    Xw_sprite = xpm_load(Xb, XPM_8_8_8, &img_Xw_sprite);
    Yw_sprite = xpm_load(Yb, XPM_8_8_8, &img_Yw_sprite);
    Zw_sprite = xpm_load(Zb, XPM_8_8_8, &img_Zw_sprite);

    //background
    
    mmbg_sprite = xpm_load(mmlightbg,XPM_8_8_8, &img_mmbg);
    erasebg_sprite = xpm_load(erasebglight, XPM_8_8_8, &img_erasebg);

    eraseblackletters_sprite = xpm_load(erasewhiteletters, XPM_8_8_8, &img_eraseblackletters_sprite);

  }
  

    erasewhiteletters_sprite = xpm_load(erasewhiteletters, XPM_8_8_8, &img_eraseblackletters_sprite);

  
}
