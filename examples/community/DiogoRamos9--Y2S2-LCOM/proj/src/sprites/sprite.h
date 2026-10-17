#include <lcom/lcf.h>

#include "../imgs/menu.xpm"
#include "../imgs/menu/menu_settings_selected.xpm"
#include "../imgs/menu/menu_start_selected.xpm"
#include "../imgs/menu/menu_instructions_selected.xpm"
#include "../imgs/menu/menu_exit_selected.xpm"
#include "../imgs/menu/instructions.xpm"
#include "../imgs/menu/leaderboard.xpm"
#include "../imgs/menu/leaderboard_selected.xpm"
#include "../imgs/menu/menu_none_selected.xpm"
#include "../imgs/menu/leaderboard_background.xpm"

#include "../imgs/menu/settings/options_480_selected.xpm"
#include "../imgs/menu/settings/options_600_selected.xpm"
#include "../imgs/menu/settings/options_768_selected.xpm"
#include "../imgs/menu/settings/options_864_selected.xpm"
#include "../imgs/menu/settings/options_1024_selected.xpm"
#include "../imgs/menu/settings/options_clear_selected.xpm"
#include "../imgs/menu/settings/options_exit_selected.xpm"

#include "../imgs/game_over/gameover_tryagain_selected.xpm"
#include "../imgs/game_over/gameover_exit_selected.xpm"

#include "../imgs/pause/pause_resume_selected.xpm"
#include "../imgs/pause/pause_exit_selected.xpm"

#include "../imgs/player.xpm"
#include "../imgs/background.xpm"
#include "../imgs/laser.xpm"
#include "../imgs/cannon/cannon_up.xpm"
#include "../imgs/cannon/cannon_right.xpm"
#include "../imgs/cannon/cannon_down.xpm"
#include "../imgs/cannon/cannon_left.xpm"
#include "../imgs/cannon/cannon_diag1.xpm"
#include "../imgs/cannon/cannon_diag2.xpm"
#include "../imgs/cannon/cannon_diag3.xpm"
#include "../imgs/cannon/cannon_diag4.xpm"
#include "../imgs/asteroid.xpm"
#include "../imgs/cursor.xpm"

#include "../imgs/numbers/char_0.xpm"
#include "../imgs/numbers/char_1.xpm"
#include "../imgs/numbers/char_2.xpm"
#include "../imgs/numbers/char_3.xpm"
#include "../imgs/numbers/char_4.xpm"
#include "../imgs/numbers/char_5.xpm"
#include "../imgs/numbers/char_6.xpm"
#include "../imgs/numbers/char_7.xpm"
#include "../imgs/numbers/char_8.xpm"
#include "../imgs/numbers/char_9.xpm"

#include "../imgs/letters/char_a.xpm"
#include "../imgs/letters/char_b.xpm"
#include "../imgs/letters/char_c.xpm"
#include "../imgs/letters/char_d.xpm"
#include "../imgs/letters/char_e.xpm"
#include "../imgs/letters/char_f.xpm"
#include "../imgs/letters/char_g.xpm"
#include "../imgs/letters/char_h.xpm"
#include "../imgs/letters/char_i.xpm"
#include "../imgs/letters/char_j.xpm"
#include "../imgs/letters/char_k.xpm"
#include "../imgs/letters/char_l.xpm"
#include "../imgs/letters/char_m.xpm"
#include "../imgs/letters/char_n.xpm"
#include "../imgs/letters/char_o.xpm"
#include "../imgs/letters/char_p.xpm"
#include "../imgs/letters/char_q.xpm"
#include "../imgs/letters/char_r.xpm"
#include "../imgs/letters/char_s.xpm"
#include "../imgs/letters/char_t.xpm"
#include "../imgs/letters/char_u.xpm"
#include "../imgs/letters/char_v.xpm"
#include "../imgs/letters/char_w.xpm"
#include "../imgs/letters/char_x.xpm"
#include "../imgs/letters/char_y.xpm"
#include "../imgs/letters/char_z.xpm"

#include "../imgs/specialChars/char_dash.xpm"
#include "../imgs/specialChars/char_doubledot.xpm"

#include "../imgs/powerups/bomb.xpm"
#include "../imgs/powerups/coin.xpm"

typedef struct {
    int x, y; // current position
    int width, height; // dimensions
    int xspeed, yspeed; // current speed
    uint32_t *map; // the pixmap
} Sprite;

Sprite *create_sprite(xpm_map_t pic);

void destroy_sprite(Sprite *sp);

void load_xpms();

void destroy_xpms();

int draw_sprite(Sprite *sp, int x, int y);
