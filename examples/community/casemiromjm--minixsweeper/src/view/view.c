#include "view.h"

static Sprite *cursor_sprite = NULL;

// Sprites do jogo
static Sprite *closedCell_sprite = NULL;
static Sprite *openCell_sprite = NULL;
static Sprite *flaggedCell_sprite = NULL;
static Sprite *bombCell_sprite = NULL;
static Sprite *explodedCell_sprite = NULL;
static Sprite *cell1_sprite = NULL;
static Sprite *cell2_sprite = NULL;
static Sprite *cell3_sprite = NULL;
static Sprite *cell4_sprite = NULL;
static Sprite *cell5_sprite = NULL;
static Sprite *cell6_sprite = NULL;
static Sprite *cell7_sprite = NULL;
static Sprite *cell8_sprite = NULL;
static Sprite *game_over_sprite = NULL;
static Sprite *game_win_sprite = NULL;
static Sprite *number_0_sprite = NULL;
static Sprite *number_1_sprite = NULL;
static Sprite *number_2_sprite = NULL;
static Sprite *number_3_sprite = NULL;
static Sprite *number_4_sprite = NULL;
static Sprite *number_5_sprite = NULL;
static Sprite *number_6_sprite = NULL;
static Sprite *number_7_sprite = NULL;
static Sprite *number_8_sprite = NULL;
static Sprite *number_9_sprite = NULL;
static Sprite *bomb_sprite = NULL;
static Sprite *game_background_sprite = NULL;
static Sprite *colon_sprite = NULL;
static Sprite *lamp_sprite = NULL;
static Sprite *lamp_off_sprite = NULL;
static Sprite *big_border_sprite = NULL;
static Sprite *medium_border_sprite = NULL;
static Sprite *small_border_sprite = NULL;
static Sprite *timer_border_sprite = NULL;
static Sprite *leave_sprite = NULL;

// Menu Sprites
static Sprite *menu_background_sprite = NULL;
static Sprite *start_button_sprite = NULL;
static Sprite *start_button_hover_sprite = NULL;
static Sprite *normal_button_sprite = NULL;
static Sprite *deceiver_button_sprite = NULL;
static Sprite *left_arrow_sprite = NULL;
static Sprite *left_arrow_hover_sprite = NULL;
static Sprite *right_arrow_sprite = NULL;
static Sprite *right_arrow_hover_sprite = NULL;
static Sprite *easy_button_sprite = NULL;
static Sprite *medium_button_sprite = NULL;
static Sprite *hard_button_sprite = NULL;
static Sprite *exit_button_sprite = NULL;
static Sprite *exit_button_hover_sprite = NULL;

void init_allsprites() {
    cursor_sprite = create_sprite((const char **)mouse_cursor, 0, 0, 0, 0);
    closedCell_sprite = create_sprite((const char **)closed_cell, 0, 0, 0, 0);
    openCell_sprite = create_sprite((const char **)open_cell, 0, 0, 0, 0);
    flaggedCell_sprite = create_sprite((const char **)flagged_cell, 0, 0, 0, 0);
    bombCell_sprite = create_sprite((const char **)bomb_cell, 0, 0, 0, 0);
    explodedCell_sprite = create_sprite((const char **)exploded_cell, 0, 0, 0, 0);
    cell1_sprite = create_sprite((const char **)cell_1, 0, 0, 0, 0);
    cell2_sprite = create_sprite((const char **)cell_2, 0, 0, 0, 0);
    cell3_sprite = create_sprite((const char **)cell_3, 0, 0, 0, 0);
    cell4_sprite = create_sprite((const char **)cell_4, 0, 0, 0, 0);
    cell5_sprite = create_sprite((const char **)cell_5, 0, 0, 0, 0);
    cell6_sprite = create_sprite((const char **)cell_6, 0, 0, 0, 0);
    cell7_sprite = create_sprite((const char **)cell_7, 0, 0, 0, 0);
    cell8_sprite = create_sprite((const char **)cell_8, 0, 0, 0, 0);
    game_over_sprite = create_sprite((const char **)game_over, 0, 0, 0, 0);
    game_win_sprite = create_sprite((const char **)game_win, 0, 0, 0, 0);
    number_0_sprite = create_sprite((const char **)number_0, 0, 0, 0, 0);
    number_1_sprite = create_sprite((const char **)number_1, 0, 0, 0, 0);
    number_2_sprite = create_sprite((const char **)number_2, 0, 0, 0, 0);
    number_3_sprite = create_sprite((const char **)number_3, 0, 0, 0, 0);
    number_4_sprite = create_sprite((const char **)number_4, 0, 0, 0, 0);
    number_5_sprite = create_sprite((const char **)number_5, 0, 0, 0, 0);
    number_6_sprite = create_sprite((const char **)number_6, 0, 0, 0, 0);
    number_7_sprite = create_sprite((const char **)number_7, 0, 0, 0, 0);
    number_8_sprite = create_sprite((const char **)number_8, 0, 0, 0, 0);
    number_9_sprite = create_sprite((const char **)number_9, 0, 0, 0, 0);
    bomb_sprite = create_sprite((const char **)bomb, 0, 0, 0, 0);
    game_background_sprite = create_sprite((const char **)game_background, 0, 0, 0, 0);
    colon_sprite = create_sprite((const char **)colon, 0, 0, 0, 0);
    lamp_sprite = create_sprite((const char **)lamp, 0, 0, 0, 0);
    lamp_off_sprite = create_sprite((const char **)lamp_off, 0, 0, 0, 0);
    big_border_sprite = create_sprite((const char **)big_border, 0, 0, 0, 0);
    medium_border_sprite = create_sprite((const char **)medium_border, 0, 0, 0, 0);
    small_border_sprite = create_sprite((const char **)small_border, 0, 0, 0, 0);
    timer_border_sprite = create_sprite((const char **)timer_border, 0, 0, 0, 0);
    leave_sprite = create_sprite((const char **)leave, 0, 0, 0, 0);

    // Menu Sprites
    menu_background_sprite = create_sprite((const char **)menu_background, 0, 0, 0, 0);
    start_button_sprite = create_sprite((const char **)start_button, 0, 0, 0, 0);
    start_button_hover_sprite = create_sprite((const char **)start_button_hover, 0, 0, 0, 0);
    normal_button_sprite = create_sprite((const char **)normal_button, 0, 0, 0, 0);
    deceiver_button_sprite = create_sprite((const char **)deceiver_button, 0, 0, 0, 0);
    left_arrow_sprite = create_sprite((const char **)left_arrow, 0, 0, 0, 0);
    left_arrow_hover_sprite = create_sprite((const char **)left_arrow_hover, 0, 0, 0, 0);
    right_arrow_sprite = create_sprite((const char **)right_arrow, 0, 0, 0, 0);
    right_arrow_hover_sprite = create_sprite((const char **)right_arrow_hover, 0, 0, 0, 0);
    easy_button_sprite = create_sprite((const char **)easy_button, 0, 0, 0, 0);
    medium_button_sprite = create_sprite((const char **)medium_button, 0, 0, 0, 0);
    hard_button_sprite = create_sprite((const char **)hard_button, 0, 0, 0, 0);
    exit_button_sprite = create_sprite((const char **)exit_button, 0, 0, 0, 0);
    exit_button_hover_sprite = create_sprite((const char **)exit_button_hover, 0, 0, 0, 0);
}

void destroy_allsprites() {
    destroy_sprite(cursor_sprite);
    destroy_sprite(closedCell_sprite);
    destroy_sprite(openCell_sprite);
    destroy_sprite(flaggedCell_sprite);
    destroy_sprite(bombCell_sprite);
    destroy_sprite(explodedCell_sprite);
    destroy_sprite(cell1_sprite);
    destroy_sprite(cell2_sprite);
    destroy_sprite(cell3_sprite);
    destroy_sprite(cell4_sprite);
    destroy_sprite(cell5_sprite);
    destroy_sprite(cell6_sprite);
    destroy_sprite(cell7_sprite);
    destroy_sprite(cell8_sprite);
    destroy_sprite(game_over_sprite);
    destroy_sprite(game_win_sprite);
    destroy_sprite(number_0_sprite);
    destroy_sprite(number_1_sprite);
    destroy_sprite(number_2_sprite);
    destroy_sprite(number_3_sprite);
    destroy_sprite(number_4_sprite);
    destroy_sprite(number_5_sprite);
    destroy_sprite(number_6_sprite);
    destroy_sprite(number_7_sprite);
    destroy_sprite(number_8_sprite);
    destroy_sprite(number_9_sprite);
    destroy_sprite(bomb_sprite);
    destroy_sprite(game_background_sprite);
    destroy_sprite(colon_sprite);
    destroy_sprite(lamp_sprite);
    destroy_sprite(lamp_off_sprite);
    destroy_sprite(big_border_sprite);
    destroy_sprite(medium_border_sprite);
    destroy_sprite(small_border_sprite);
    destroy_sprite(timer_border_sprite);
    destroy_sprite(leave_sprite);

    // Destroy Menu Sprites
    destroy_sprite(menu_background_sprite);
    destroy_sprite(start_button_sprite);
    destroy_sprite(start_button_hover_sprite);
    destroy_sprite(normal_button_sprite);
    destroy_sprite(deceiver_button_sprite);
    destroy_sprite(left_arrow_sprite);
    destroy_sprite(left_arrow_hover_sprite);
    destroy_sprite(right_arrow_sprite);
    destroy_sprite(right_arrow_hover_sprite);
    destroy_sprite(easy_button_sprite);
    destroy_sprite(medium_button_sprite);
    destroy_sprite(hard_button_sprite);
    destroy_sprite(exit_button_sprite);
    destroy_sprite(exit_button_hover_sprite);
}

// Funções de desenho gerais

void draw_cursor(int x, int y) {
    if (!cursor_sprite) return;

    cursor_sprite->x = x;
    cursor_sprite->y = y;

    draw_sprite(cursor_sprite, get_buffer());
}

void draw_menu_background() {
    if (!menu_background_sprite) return;

    menu_background_sprite->x = 0;
    menu_background_sprite->y = 0;

    draw_sprite(menu_background_sprite, get_buffer());
}

void draw_game_background() {
    if (!game_background_sprite) return;

    game_background_sprite->x = 0;
    game_background_sprite->y = 0;

    draw_sprite(game_background_sprite, get_buffer());
}

void draw_background() {
    vg_draw_rectangle(0, 0, 800, 600, 0x808080); // Cor do fundo (cinza claro)
}

void draw_background_pixel(int x, int y) {
    vg_draw_pixel(x, y, 0x808080); // Cor do fundo (cinza claro)
}

// Funções de desenho do jogo

void draw_closedCell(int x, int y) {
    if (!closedCell_sprite) return;

    closedCell_sprite->x = x;
    closedCell_sprite->y = y;

    draw_sprite(closedCell_sprite, get_buffer());
}

void draw_openCell(int x, int y) {
    if (!openCell_sprite) return;

    openCell_sprite->x = x;
    openCell_sprite->y = y;

    draw_sprite(openCell_sprite, get_buffer());
}

void draw_flaggedCell(int x, int y) {
    if (!flaggedCell_sprite) return;

    flaggedCell_sprite->x = x;
    flaggedCell_sprite->y = y;

    draw_sprite(flaggedCell_sprite, get_buffer());
}

void draw_bombCell(int x, int y) {
    if (!bombCell_sprite) return;

    bombCell_sprite->x = x;
    bombCell_sprite->y = y;

    draw_sprite(bombCell_sprite, get_buffer());
}

void draw_explodedCell(int x, int y) {
    if (!explodedCell_sprite) return;

    explodedCell_sprite->x = x;
    explodedCell_sprite->y = y;

    draw_sprite(explodedCell_sprite, get_buffer());
}

void draw_cell1(int x, int y) {
    if (!cell1_sprite) return;

    cell1_sprite->x = x;
    cell1_sprite->y = y;

    draw_sprite(cell1_sprite, get_buffer());
}

void draw_cell2(int x, int y) {
    if (!cell2_sprite) return;

    cell2_sprite->x = x;
    cell2_sprite->y = y;

    draw_sprite(cell2_sprite, get_buffer());
}

void draw_cell3(int x, int y) {
    if (!cell3_sprite) return;

    cell3_sprite->x = x;
    cell3_sprite->y = y;

    draw_sprite(cell3_sprite, get_buffer());
}

void draw_cell4(int x, int y) {
    if (!cell4_sprite) return;

    cell4_sprite->x = x;
    cell4_sprite->y = y;

    draw_sprite(cell4_sprite, get_buffer());
}

void draw_cell5(int x, int y) {
    if (!cell5_sprite) return;

    cell5_sprite->x = x;
    cell5_sprite->y = y;

    draw_sprite(cell5_sprite, get_buffer());
}

void draw_cell6(int x, int y) {
    if (!cell6_sprite) return;

    cell6_sprite->x = x;
    cell6_sprite->y = y;

    draw_sprite(cell6_sprite, get_buffer());
}

void draw_cell7(int x, int y) {
    if (!cell7_sprite) return;

    cell7_sprite->x = x;
    cell7_sprite->y = y;

    draw_sprite(cell7_sprite, get_buffer());
}

void draw_cell8(int x, int y) {
    if (!cell8_sprite) return;

    cell8_sprite->x = x;
    cell8_sprite->y = y;

    draw_sprite(cell8_sprite, get_buffer());
}

void draw_game_over(int x, int y) {
    if (!game_over_sprite) return;

    game_over_sprite->x = x;
    game_over_sprite->y = y;

    draw_sprite(game_over_sprite, get_buffer());
}

void draw_game_win(int x, int y) {
    if (!game_win_sprite) return;

    game_win_sprite->x = x;
    game_win_sprite->y = y;

    draw_sprite(game_win_sprite, get_buffer());
}

void draw_number(int x, int y, unsigned int number) {
    Sprite *num_sprite = NULL;
    switch (number) {
        case 0: num_sprite = number_0_sprite; break;
        case 1: num_sprite = number_1_sprite; break;
        case 2: num_sprite = number_2_sprite; break;
        case 3: num_sprite = number_3_sprite; break;
        case 4: num_sprite = number_4_sprite; break;
        case 5: num_sprite = number_5_sprite; break;
        case 6: num_sprite = number_6_sprite; break;
        case 7: num_sprite = number_7_sprite; break;
        case 8: num_sprite = number_8_sprite; break;
        case 9: num_sprite = number_9_sprite; break;
        default: return;
    }
    if (!num_sprite) return;
    num_sprite->x = x;
    num_sprite->y = y;
    draw_sprite(num_sprite, get_buffer());
}

void draw_bomb(int x, int y) {
    if (!bomb_sprite) return;

    bomb_sprite->x = x;
    bomb_sprite->y = y;

    draw_sprite(bomb_sprite, get_buffer());
}

void draw_lamp(Map* map) {
    if (!lamp_sprite || !lamp_off_sprite) return;

    unsigned int x = map->start_x; 
    unsigned int y = map->start_y - BORDER_SPACING -CELL_SIZE;

    if (!map->lamp_used) {
        lamp_sprite->x = x;
        lamp_sprite->y = y;
        draw_sprite(lamp_sprite, get_buffer());
    } else {
        lamp_off_sprite->x = x;
        lamp_off_sprite->y = y;
        draw_sprite(lamp_off_sprite, get_buffer());
    }
}

void draw_number_of_bombs(Map *map) {
    unsigned number_to_draw;

    if (map->flags_used > map->total_bombs) {
        number_to_draw = 0;
    }
    else {
        number_to_draw = map->total_bombs - map->flags_used;
    }

    unsigned int x = map->start_x + map->cols * CELL_SIZE - 3 * CELL_SIZE;
    unsigned int y = map->start_y - CELL_SIZE - BORDER_SPACING;

    draw_bomb(x, y);
    draw_number(x + 2 * CELL_SIZE, y, number_to_draw % 10);
    number_to_draw /= 10;
    draw_number(x + CELL_SIZE, y, number_to_draw % 10);
}

void draw_map(Map* map) {
    for (unsigned int row = 0; row < map->rows; ++row) {
        for (unsigned int col = 0; col < map->cols; ++col) {
            Cell *cell = &map->cells[row][col];

            unsigned int x = map->start_x + col * CELL_SIZE;
            unsigned int y = map->start_y + row * CELL_SIZE;

            if (cell->is_revealed) {
                if (cell->is_bomb) {
                    if (cell->is_exploded) {
                        draw_explodedCell(x, y);
                    }
                    else {
                        draw_bombCell(x, y);
                    }
                } 
                else {
                    switch (cell->adjacent_bombs) {
                        case 0: draw_openCell(x, y); break;
                        case 1: draw_cell1(x, y); break;
                        case 2: draw_cell2(x, y); break;
                        case 3: draw_cell3(x, y); break;
                        case 4: draw_cell4(x, y); break;
                        case 5: draw_cell5(x, y); break;
                        case 6: draw_cell6(x, y); break;
                        case 7: draw_cell7(x, y); break;
                        case 8: draw_cell8(x, y); break;
                    }
                }
            }
            else if (cell->is_flagged) {
                draw_flaggedCell(x, y);
            }
            else {
                draw_closedCell(x, y);
            }
        }
    }
}

void draw_colon(int x, int y) {
    if (!colon_sprite) return;

    colon_sprite->x = x;
    colon_sprite->y = y;

    draw_sprite(colon_sprite, get_buffer());
}

void draw_game() {
    draw_game_background();
    if (map) {
        draw_map(map);

        switch (menu->selected_difficulty) {
            case EASY:
                draw_small_border(map->start_x - BORDER_SPACING, map->start_y - 2 * BORDER_SPACING - CELL_SIZE);
                break;
            case MEDIUM:
                draw_medium_border(map->start_x - BORDER_SPACING, map->start_y - 2 * BORDER_SPACING - CELL_SIZE);
                break;
            case HARD:
                draw_big_border(map->start_x - BORDER_SPACING, map->start_y - 2 * BORDER_SPACING - CELL_SIZE);
                break;
            default:
                break;
        }
        
        draw_lamp(map);
        draw_number_of_bombs(map);
        draw_timer_border((SCREEN_WIDTH - TIMER_BORDER_WIDTH) / 2, 0);
        int timer_x = (SCREEN_WIDTH - TIMER_BORDER_WIDTH + 2 * BORDER_SPACING) / 2;
        int timer_y = 0;
        draw_time(timer_x, timer_y, get_elapsed_seconds());
        int leave_x = SCREEN_WIDTH - 50;
        int leave_y = 0; 
        draw_leave(leave_x, leave_y);
    }
}

// Funções de desenho do menu

void draw_menu(Menu *menu) {
    draw_menu_background();

    draw_start_button(menu);
    draw_mode_button(menu);
    draw_difficulty_button(menu);
    draw_exit_button(menu);
}

void draw_button(Button *btn, Sprite *label_sprite) {
    if (label_sprite) {
        int label_x = btn->x;
        int label_y = btn->y;
        label_sprite->x = label_x;
        label_sprite->y = label_y;
        draw_sprite(label_sprite, get_buffer());
    }
}

void draw_left_arrow(int x, int y, bool hovered) {
    if (hovered) {
        if (!left_arrow_hover_sprite) return;

        left_arrow_hover_sprite->x = x;
        left_arrow_hover_sprite->y = y;

        draw_sprite(left_arrow_hover_sprite, get_buffer());
        return;
    }
    else {
        if (!left_arrow_sprite) return;

        left_arrow_sprite->x = x;
        left_arrow_sprite->y = y;

        draw_sprite(left_arrow_sprite, get_buffer());
    }
}

void draw_right_arrow(int x, int y, bool hovered) {
    if (hovered) {
        if (!right_arrow_hover_sprite) return;

        right_arrow_hover_sprite->x = x;
        right_arrow_hover_sprite->y = y;

        draw_sprite(right_arrow_hover_sprite, get_buffer());
        return;
    }
    else {
        if (!right_arrow_sprite) return;

        right_arrow_sprite->x = x;
        right_arrow_sprite->y = y;

        draw_sprite(right_arrow_sprite, get_buffer());
    }
}

void draw_start_button(Menu *menu) {
    if (menu->start_button.hovered) {
        draw_button(&menu->start_button, start_button_hover_sprite);
    }
    else {
        draw_button(&menu->start_button, start_button_sprite);
    }
}

void draw_difficulty_button(Menu *menu) {
    draw_left_arrow(menu->left_difficulty_arrow.x, menu->left_difficulty_arrow.y, menu->left_difficulty_arrow.hovered);
    switch (menu->selected_difficulty) {
        case EASY:
            draw_button(&menu->difficulty_button, easy_button_sprite);
            break;
        case MEDIUM:
            draw_button(&menu->difficulty_button, medium_button_sprite);
            break;
        case HARD:
            draw_button(&menu->difficulty_button, hard_button_sprite);
            break;
        default:
            break;
    }
    draw_right_arrow(menu->right_difficulty_arrow.x, menu->right_difficulty_arrow.y, menu->right_difficulty_arrow.hovered);
}

void draw_mode_button(Menu *menu) {
    draw_left_arrow(menu->left_mode_arrow.x, menu->left_mode_arrow.y, menu->left_mode_arrow.hovered);
    switch (menu->selected_mode) {
        case NORMAL:
            draw_button(&menu->mode_button, normal_button_sprite);
            break;
        case DECEIVER:
            draw_button(&menu->mode_button, deceiver_button_sprite);
            break;
        default:
            break;
    }
    draw_right_arrow(menu->right_mode_arrow.x, menu->right_mode_arrow.y, menu->right_mode_arrow.hovered);
}

void draw_exit_button(Menu *menu) {
    if (menu->exit_button.hovered) {
        draw_button(&menu->exit_button, exit_button_hover_sprite);
    }
    else {
        draw_button(&menu->exit_button, exit_button_sprite);
    }
}

void draw_time(int x, int y, int seconds) {
    int minutes = seconds / 60;
    int secs = seconds % 60;

    // minutos
    draw_number(x, y, minutes / 10);
    draw_number(x + CELL_SIZE, y, minutes % 10);

    // :
    draw_colon(x + 2 * CELL_SIZE, y);
    // segundos
    draw_number(x + 3 * CELL_SIZE, y, secs / 10);
    draw_number(x + 4 * CELL_SIZE, y, secs % 10);
}

void draw_game_win_screen(int elapsed_seconds, int gw_x, int gw_y, int timer_x, int timer_y, int mouse_x, int mouse_y) {
    draw_game();
    draw_game_win(gw_x, gw_y);
    draw_cursor(mouse_x, mouse_y);
    copyBufferToVideo();
}

void draw_big_border(int x, int y) {
    if (!big_border_sprite) return;

    big_border_sprite->x = x;
    big_border_sprite->y = y;

    draw_sprite(big_border_sprite, get_buffer());
}

void draw_medium_border(int x, int y) {
    if (!medium_border_sprite) return;

    medium_border_sprite->x = x;
    medium_border_sprite->y = y;

    draw_sprite(medium_border_sprite, get_buffer());
}

void draw_small_border(int x, int y) {
    if (!small_border_sprite) return;

    small_border_sprite->x = x;
    small_border_sprite->y = y;

    draw_sprite(small_border_sprite, get_buffer());
}

void draw_timer_border(int x, int y) {
    if (!timer_border_sprite) return;

    timer_border_sprite->x = x;
    timer_border_sprite->y = y;

    draw_sprite(timer_border_sprite, get_buffer());
}

void draw_leave(int x, int y) {
    if (!leave_sprite) return;

    leave_sprite->x = x;
    leave_sprite->y = y;

    draw_sprite(leave_sprite, get_buffer());
}

