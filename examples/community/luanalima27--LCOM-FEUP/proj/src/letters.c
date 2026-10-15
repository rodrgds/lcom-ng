// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include "video.h"
#include "letters.h"
#include "sprites.h"
#include "i8042.h"


extern uint8_t *exclamationw_sprite;
extern uint8_t *start_sprite;
extern uint8_t *percentagew_sprite;
extern uint8_t *andw_sprite;
extern uint8_t *leftwhite_sprite;
extern uint8_t *rightbracketw_sprite;
extern uint8_t *commaw_sprite;
extern uint8_t *semicolonw_sprite;
extern uint8_t *zerow_sprite;
extern uint8_t *onew_sprite;
extern uint8_t *twow_sprite;
extern uint8_t *threew_sprite;
extern uint8_t *fourw_sprite;
extern uint8_t *fivew_sprite;
extern uint8_t *sixw_sprite;
extern uint8_t *sevenw_sprite;
extern uint8_t *eightw_sprite;
extern uint8_t *ninew_sprite;
extern uint8_t *Aw_sprite;
extern uint8_t *Bw_sprite;
extern uint8_t *Cw_sprite;
extern uint8_t *Çw_sprite;
extern uint8_t *Dw_sprite;
extern uint8_t *Ew_sprite;
extern uint8_t *Fw_sprite;
extern uint8_t *Gw_sprite;
extern uint8_t *Hw_sprite;
extern uint8_t *Iw_sprite;
extern uint8_t *Jw_sprite;
extern uint8_t *Kw_sprite;
extern uint8_t *law_sprite;
extern uint8_t *lbw_sprite;
extern uint8_t *lcw_sprite;
extern uint8_t *lçw_sprite;
extern uint8_t *ldw_sprite;
extern uint8_t *lew_sprite;
extern uint8_t *lfw_sprite;
extern uint8_t *lgw_sprite;
extern uint8_t *lhw_sprite;
extern uint8_t *liw_sprite;
extern uint8_t *ljw_sprite;
extern uint8_t *lkw_sprite;
extern uint8_t *llw_sprite;
extern uint8_t *lmw_sprite;
extern uint8_t *lnw_sprite;
extern uint8_t *low_sprite;
extern uint8_t *lpw_sprite;
extern uint8_t *lqw_sprite;
extern uint8_t *lrw_sprite;
extern uint8_t *lsw_sprite;
extern uint8_t *ltw_sprite;
extern uint8_t *luw_sprite;
extern uint8_t *lvw_sprite;
extern uint8_t *Lw_sprite;
extern uint8_t *lww_sprite;
extern uint8_t *lxw_sprite;
extern uint8_t *lyw_sprite;
extern uint8_t *lzw_sprite;
extern uint8_t *Mw_sprite;
extern uint8_t *Nw_sprite;
extern uint8_t *Ow_sprite;
extern uint8_t *Pw_sprite;
extern uint8_t *questionmarkw_sprite;
extern uint8_t *Qw_sprite;
extern uint8_t *Rw_sprite;
extern uint8_t *sidebarw_sprite;
extern uint8_t *Sw_sprite;
extern uint8_t *Tw_sprite;
extern uint8_t *twodotsw_sprite;
extern uint8_t *Uw_sprite;
extern uint8_t *Vw_sprite;
extern uint8_t *Ww_sprite;
extern uint8_t *Xw_sprite;
extern uint8_t *Yw_sprite;
extern uint8_t *Zw_sprite;
extern uint8_t *eraseblackletters_sprite;
extern uint8_t *erasewhiteletters_sprite;
extern uint8_t *dotw_sprite;
extern xpm_image_t img_exclamationw_sprite;
extern xpm_image_t img_start_sprite;
extern xpm_image_t img_percentagew_sprite;
extern xpm_image_t img_andw_sprite;
extern xpm_image_t img_leftwhite_sprite;
extern xpm_image_t img_rightbracketw_sprite;
extern xpm_image_t img_commaw_sprite;
extern xpm_image_t img_semicolonw_sprite;
extern xpm_image_t img_zerow_sprite;
extern xpm_image_t img_onew_sprite;
extern xpm_image_t img_twow_sprite;
extern xpm_image_t img_threew_sprite;
extern xpm_image_t img_fourw_sprite;
extern xpm_image_t img_fivew_sprite;
extern xpm_image_t img_sixw_sprite;
extern xpm_image_t img_sevenw_sprite;
extern xpm_image_t img_eightw_sprite;
extern xpm_image_t img_ninew_sprite;
extern xpm_image_t img_Aw_sprite;
extern xpm_image_t img_Bw_sprite;
extern xpm_image_t img_Cw_sprite;
extern xpm_image_t img_Çw_sprite;
extern xpm_image_t img_Dw_sprite;
extern xpm_image_t img_Ew_sprite;
extern xpm_image_t img_Fw_sprite;
extern xpm_image_t img_Gw_sprite;
extern xpm_image_t img_Hw_sprite;
extern xpm_image_t img_Iw_sprite;
extern xpm_image_t img_Jw_sprite;
extern xpm_image_t img_Kw_sprite;
extern xpm_image_t img_law_sprite;
extern xpm_image_t img_lbw_sprite;
extern xpm_image_t img_lcw_sprite;
extern xpm_image_t img_lçw_sprite;
extern xpm_image_t img_ldw_sprite;
extern xpm_image_t img_lew_sprite;
extern xpm_image_t img_lfw_sprite;
extern xpm_image_t img_lgw_sprite;
extern xpm_image_t img_lhw_sprite;
extern xpm_image_t img_liw_sprite;
extern xpm_image_t img_ljw_sprite;
extern xpm_image_t img_lkw_sprite;
extern xpm_image_t img_llw_sprite;
extern xpm_image_t img_lmw_sprite;
extern xpm_image_t img_lnw_sprite;
extern xpm_image_t img_low_sprite;
extern xpm_image_t img_lpw_sprite;
extern xpm_image_t img_lqw_sprite;
extern xpm_image_t img_lrw_sprite;
extern xpm_image_t img_lsw_sprite;
extern xpm_image_t img_ltw_sprite;
extern xpm_image_t img_luw_sprite;
extern xpm_image_t img_lvw_sprite;
extern xpm_image_t img_Lw_sprite;
extern xpm_image_t img_lww_sprite;
extern xpm_image_t img_lxw_sprite;
extern xpm_image_t img_lyw_sprite;
extern xpm_image_t img_lzw_sprite;
extern xpm_image_t img_Mw_sprite;
extern xpm_image_t img_Nw_sprite;
extern xpm_image_t img_Ow_sprite;
extern xpm_image_t img_Pw_sprite;
extern xpm_image_t img_questionmarkw_sprite;
extern xpm_image_t img_Qw_sprite;
extern xpm_image_t img_Rw_sprite;
extern xpm_image_t img_sidebarb_sprite;
extern xpm_image_t img_sidebarw_sprite;
extern xpm_image_t img_Sw_sprite;
extern xpm_image_t img_Tb_sprite;
extern xpm_image_t img_Tw_sprite;
extern xpm_image_t img_twodotsw_sprite;
extern xpm_image_t img_Uw_sprite;
extern xpm_image_t img_Vw_sprite;
extern xpm_image_t img_Ww_sprite;
extern xpm_image_t img_Xw_sprite;
extern xpm_image_t img_Yw_sprite;
extern xpm_image_t img_Zw_sprite;
extern xpm_image_t img_eraseblackletters_sprite;
extern xpm_image_t img_erasewhiteletters_sprite;
extern xpm_image_t img_dotw_sprite;



int draw_exclamationw(uint16_t x, uint16_t y) {
  if(draw_xpm(exclamationw_sprite, img_exclamationw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}



int draw_percentagew(uint16_t x, uint16_t y) {
  if(draw_xpm(percentagew_sprite, img_percentagew_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}


int draw_andw(uint16_t x, uint16_t y) {
  if(draw_xpm(andw_sprite, img_andw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}


int draw_leftwhite(uint16_t x, uint16_t y) {
  if(draw_xpm(leftwhite_sprite, img_leftwhite_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_rightbracketw(uint16_t x, uint16_t y) {
  if(draw_xpm(rightbracketw_sprite, img_rightbracketw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}


int draw_commaw(uint16_t x, uint16_t y) {
  if(draw_xpm(commaw_sprite, img_commaw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}


int draw_semicolonw(uint16_t x, uint16_t y) {
  if(draw_xpm(semicolonw_sprite, img_semicolonw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}


int draw_zerow(uint16_t x, uint16_t y) {
  if(draw_xpm(zerow_sprite, img_zerow_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}


int draw_onew(uint16_t x, uint16_t y) {
  if(draw_xpm(onew_sprite, img_onew_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}


int draw_twow(uint16_t x, uint16_t y) {
  if(draw_xpm(twow_sprite, img_twow_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}


int draw_threew(uint16_t x, uint16_t y) {
  if(draw_xpm(threew_sprite, img_threew_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_fourw(uint16_t x, uint16_t y) {
  if(draw_xpm(fourw_sprite, img_fourw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}


int draw_fivew(uint16_t x, uint16_t y) {
  if(draw_xpm(fivew_sprite, img_fivew_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}


int draw_sixw(uint16_t x, uint16_t y) {
  if(draw_xpm(sixw_sprite, img_sixw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_sevenw(uint16_t x, uint16_t y) {
  if(draw_xpm(sevenw_sprite, img_sevenw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_eightw(uint16_t x, uint16_t y) {
  if(draw_xpm(eightw_sprite, img_eightw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_ninew(uint16_t x, uint16_t y) {
  if(draw_xpm(ninew_sprite, img_ninew_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_Aw(uint16_t x, uint16_t y) {
  if(draw_xpm(Aw_sprite, img_Aw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_Bw(uint16_t x, uint16_t y) {
  if(draw_xpm(Bw_sprite, img_Bw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_Cw(uint16_t x, uint16_t y) {
  if(draw_xpm(Cw_sprite, img_Cw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_Çw(uint16_t x, uint16_t y) {
  if(draw_xpm(Çw_sprite, img_Çw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_Dw(uint16_t x, uint16_t y) {
  if(draw_xpm(Dw_sprite, img_Dw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_Ew(uint16_t x, uint16_t y) {
  if(draw_xpm(Ew_sprite, img_Ew_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}
int draw_Fw(uint16_t x, uint16_t y) {
  if(draw_xpm(Fw_sprite, img_Fw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_Gw(uint16_t x, uint16_t y) {
  if(draw_xpm(Gw_sprite, img_Gw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_Hw(uint16_t x, uint16_t y) {
  if(draw_xpm(Hw_sprite, img_Hw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_Iw(uint16_t x, uint16_t y) {
  if(draw_xpm(Iw_sprite, img_Iw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_Jw(uint16_t x, uint16_t y) {
  if(draw_xpm(Jw_sprite, img_Jw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_Kw(uint16_t x, uint16_t y) {
  if(draw_xpm(Kw_sprite, img_Kw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_law(uint16_t x, uint16_t y) {
  if(draw_xpm(law_sprite, img_law_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}
int draw_lbw(uint16_t x, uint16_t y) {
  if(draw_xpm(lbw_sprite, img_lbw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_lcw(uint16_t x, uint16_t y) {
  if(draw_xpm(lcw_sprite, img_lcw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_lçw(uint16_t x, uint16_t y) {
  if(draw_xpm(lçw_sprite, img_lçw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_ldw(uint16_t x, uint16_t y) {
  if(draw_xpm(ldw_sprite, img_ldw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_lew(uint16_t x, uint16_t y) {
  if(draw_xpm(lew_sprite, img_lew_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_lfw(uint16_t x, uint16_t y) {
  if(draw_xpm(lfw_sprite, img_lfw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_lgw(uint16_t x, uint16_t y) {
  if(draw_xpm(lgw_sprite, img_lgw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_lhw(uint16_t x, uint16_t y) {
  if(draw_xpm(lhw_sprite, img_lhw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_liw(uint16_t x, uint16_t y) {
  if(draw_xpm(liw_sprite, img_liw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_ljw(uint16_t x, uint16_t y) {
  if(draw_xpm(ljw_sprite, img_ljw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_lkw(uint16_t x, uint16_t y) {
  if(draw_xpm(lkw_sprite, img_lkw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_llw(uint16_t x, uint16_t y) {
  if(draw_xpm(llw_sprite, img_llw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_lmw(uint16_t x, uint16_t y) {
  if(draw_xpm(lmw_sprite, img_lmw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_lnw(uint16_t x, uint16_t y) {
  if(draw_xpm(lnw_sprite, img_lnw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_low(uint16_t x, uint16_t y) {
  if(draw_xpm(low_sprite, img_low_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_lpw(uint16_t x, uint16_t y) {
  if(draw_xpm(lpw_sprite, img_lpw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_lqw(uint16_t x, uint16_t y) {
  if(draw_xpm(lqw_sprite, img_lqw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_lrw(uint16_t x, uint16_t y) {
  if(draw_xpm(lrw_sprite, img_lrw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_lsw(uint16_t x, uint16_t y) {
  if(draw_xpm(lsw_sprite, img_lsw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_ltw(uint16_t x, uint16_t y) {
  if(draw_xpm(ltw_sprite, img_ltw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_luw(uint16_t x, uint16_t y) {
  if(draw_xpm(luw_sprite, img_luw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_lvw(uint16_t x, uint16_t y) {
  if(draw_xpm(lvw_sprite, img_lvw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_Lw(uint16_t x, uint16_t y) {
  if(draw_xpm(Lw_sprite, img_Lw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_lww(uint16_t x, uint16_t y) {
  if(draw_xpm(lww_sprite, img_lww_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_lxw(uint16_t x, uint16_t y) {
  if(draw_xpm(lxw_sprite, img_lxw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_lyw(uint16_t x, uint16_t y) {
  if(draw_xpm(lyw_sprite, img_lyw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_lzw(uint16_t x, uint16_t y) {
  if(draw_xpm(lzw_sprite, img_lzw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_Mw(uint16_t x, uint16_t y) {
  if(draw_xpm(Mw_sprite, img_Mw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_Nw(uint16_t x, uint16_t y) {
  if(draw_xpm(Nw_sprite, img_Nw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_Ow(uint16_t x, uint16_t y) {
  if(draw_xpm(Ow_sprite, img_Ow_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_Pw(uint16_t x, uint16_t y) {
  if(draw_xpm(Pw_sprite, img_Pw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_Qw(uint16_t x, uint16_t y) {
  if(draw_xpm(Qw_sprite, img_Qw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_Rw(uint16_t x, uint16_t y) {
  if(draw_xpm(Rw_sprite, img_Rw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_Sw(uint16_t x, uint16_t y) {
  if(draw_xpm(Sw_sprite, img_Sw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_Tw(uint16_t x, uint16_t y) {
  if(draw_xpm(Tw_sprite, img_Tw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_Uw(uint16_t x, uint16_t y) {
  if(draw_xpm(Uw_sprite, img_Uw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_Vw(uint16_t x, uint16_t y) {
  if(draw_xpm(Vw_sprite, img_Vw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_Ww(uint16_t x, uint16_t y) {
  if(draw_xpm(Ww_sprite, img_Ww_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_Xw(uint16_t x, uint16_t y) {
  if(draw_xpm(Xw_sprite, img_Xw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_Yw(uint16_t x, uint16_t y) {
  if(draw_xpm(Yw_sprite, img_Yw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int draw_Zw(uint16_t x, uint16_t y) {
  if(draw_xpm(Zw_sprite, img_Zw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int eraseblackletters(uint16_t x, uint16_t y) {
  if(draw_xpm(eraseblackletters_sprite, img_eraseblackletters_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

int erasewhiteletters(uint16_t x, uint16_t y) {
  if(draw_xpm(erasewhiteletters_sprite, img_erasewhiteletters_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}


int draw_dotw(uint16_t x, uint16_t y) {
  if(draw_xpm(dotw_sprite, img_dotw_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }
  return 0;
}

