#include "graphics.h"

static xpm_image_t password_xpm[PASSWORD_SIZE];
static xpm_image_t numbers_xpm[10];
static xpm_image_t menu_xpm, wall_texture_xpm, cursor_xpm, pause_menu_xpm, door_texture_lock_xpm, door_texture_open_xpm, defeat_xpm, victory_xpm,
        player_select_xpm, two_dots_xpm;

int (graphics_construct) () {
    if (vg_get_resolution(&width, &height) != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }
    if (map_get_grid_dimensions(&map_width, &map_height) != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }
    load_xpms();
    return 0;
}

int (graphics_draw_game) () {
    if (set_background_color(BLACK) != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }

    graphics_draw_floor();
    graphics_draw_ceiling();

    if (graphics_draw_player_camera() != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }
    if (graphics_draw_map() != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }

    if (graphics_draw_timer() != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }

    swap_buffer();
    return 0;
}

int (graphics_draw_menu) () {
    if (vg_draw_xpm(0, 0, menu_xpm.width, menu_xpm.size, menu_xpm.bytes) != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }

    if (graphics_draw_cursor() != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }

    swap_buffer();
    return 0;
}

int (graphics_draw_pause_menu) () {
    if (vg_draw_xpm(0, 0, pause_menu_xpm.width, pause_menu_xpm.size, pause_menu_xpm.bytes) != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }

    if (graphics_draw_cursor() != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }

    swap_buffer();
    return 0;
}

int (graphics_draw_defeat_screen) () {
    if (vg_draw_xpm(0, 0, defeat_xpm.width, defeat_xpm.size, defeat_xpm.bytes) != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }

    if (graphics_draw_cursor() != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }

    swap_buffer();
    return 0;
}

int (graphics_draw_victory_screen) () {
    if (vg_draw_xpm(0, 0, victory_xpm.width, victory_xpm.size, victory_xpm.bytes) != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }

    if (graphics_draw_cursor() != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }

    swap_buffer();
    return 0;
}

int (graphics_draw_intercom) () {
    if (set_background_color(BLACK) != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }

    uint8_t letters[PASSWORD_SIZE];
    uint8_t password[PASSWORD_SIZE];
    intercom_get_letters(letters);
    intercom_get_password(password);
    for (int i = 0; i < 4; i++) {
        if (letters[i] == password[i])
            vg_draw_xpm(125 + 150 * i, 250, password_xpm[i].width, password_xpm[i].size, password_xpm[i].bytes);
    }

    swap_buffer();
    return 0;
}

int (graphics_draw_player_select) () {
    if (vg_draw_xpm(0, 0, player_select_xpm.width, player_select_xpm.size, player_select_xpm.bytes) != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }

    if (graphics_draw_cursor() != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }

    swap_buffer();
    return 0;
}

int (graphics_draw_floor) () {
  uint32_t color_map[4] = {0x345461, 0x497282, 0x7496a3, 0x1f3e4a};
  int n = 0;
  for (int i = 0; i < 800; i++) {
    for (int j = 300; j < 600; j++) {
      vg_draw_pixel(i, j, color_map[n % 3]);
      n++;
    }
  }
  return 0;
}

int (graphics_draw_ceiling) () {
  uint32_t color_map[4] = {0xb5b5b3, 0x70706d, 0xababab, 0xffffff};
  int n = 0;
  for (int i = 0; i < 800; i++) {
    for (int j = 0; j < 300; j++) {
      vg_draw_pixel(i, j, color_map[n % 4]);
      n++;
    }
  }
  return 0;
}

int (graphics_draw_timer) () {
    uint8_t hours, minutes, seconds;
    if (clock_get_elapsed_time(&hours, &minutes, &seconds) != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }

    if (vg_draw_rectangle(width - 300, height - 100, 290, 100, 0x000000) != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }

    printf("%d:%d:%d\n", hours, minutes, seconds);
    if (seconds < 10) {
        vg_draw_xpm(width - 140, height - 90, 60, numbers_xpm[0].size, numbers_xpm[0].bytes);
        vg_draw_xpm(width - 80, height - 90, 60, numbers_xpm[seconds].size, numbers_xpm[seconds].bytes);
    } else {
        vg_draw_xpm(width - 80, height - 90, 60, numbers_xpm[seconds % 10].size, numbers_xpm[seconds % 10].bytes);
        vg_draw_xpm(width - 140, height - 90, 60, numbers_xpm[seconds / 10].size, numbers_xpm[seconds / 10].bytes);
    }
    vg_draw_xpm(width - 170, height - 90, two_dots_xpm.width, two_dots_xpm.size, two_dots_xpm.bytes);
    if (minutes < 10) {
        vg_draw_xpm(width - 290, height - 90, 60, numbers_xpm[0].size, numbers_xpm[0].bytes);
        vg_draw_xpm(width - 230, height - 90, 60, numbers_xpm[minutes].size, numbers_xpm[minutes].bytes);
    } else {
        vg_draw_xpm(width - 290, height - 90, 60, numbers_xpm[minutes / 10].size, numbers_xpm[minutes / 10].bytes);
        vg_draw_xpm(width - 230, height - 90, 60, numbers_xpm[minutes % 10].size, numbers_xpm[minutes % 10].bytes);
    }
    return 0;
}

int (graphics_draw_player_camera) () {
    int cellsize = height / map_height;

    for (uint32_t i = 0; i < width; i++) {
        uint8_t delimiter;
        double ray_angle = get_ray_angle(i);
        struct point2D* line = (struct point2D*)malloc(sizeof(struct point2D) * LINE_SIZE);
        int line_size = create_line(line, ray_angle, &delimiter);
        if (line_size <= 0) {
            printf("ERROR: %s\n", __func__ );
            free(line);
            return 1;
        }

        struct point2D collision_point = line[line_size - 1];
        double distance_to_wall = sqrt(pow(collision_point.x - player_get_position().x, 2) + pow(collision_point.y - player_get_position().y, 2));
        distance_to_wall *= fabs(cos(to_radians(ray_angle - player_get_angle())));

        int wall_height = (int) ((height * cellsize) / distance_to_wall);
        int draw_start = -wall_height / 2 + height / 2;
        if (draw_start < 0) draw_start = 0;
        int16_t draw_end = wall_height / 2 + height / 2;
        if (draw_end >= (int)height) draw_end = height - 1;

        if (line_size == LINE_SIZE - 1) {
            vg_draw_vline(width - i, draw_start, draw_end - draw_start, BLACK);
        } else {
            uint32_t colors[16];
            if (delimiter == 1 || delimiter == 3 || delimiter == 4) {
                for (int j = 0; j < 16; j++) {
                    uint32_t color = (wall_texture_xpm.bytes[(i % 16 + 16 * j) * 3 + 2] << 16)
                                     | (wall_texture_xpm.bytes[(i % 16 + 16 * j) * 3 + 1] << 8)
                                     | wall_texture_xpm.bytes[(i % 16 + 16 * j) * 3];
                    colors[j] = color;
                }
            } else if (delimiter == 8) {
                for (int j = 0; j < 16; j++) {
                    uint32_t color = (door_texture_lock_xpm.bytes[(i % 16 + 16 * j) * 3 + 2] << 16)
                                     | (door_texture_lock_xpm.bytes[(i % 16 + 16 * j) * 3 + 1] << 8)
                                     | door_texture_lock_xpm.bytes[(i % 16 + 16 * j) * 3];
                    colors[j] = color;
                }
            } else if (delimiter == 16) {
              for (int j = 0; j < 16; j++) {
                uint32_t color = (door_texture_open_xpm.bytes[(i % 16 + 16 * j) * 3 + 2] << 16)
                                 | (door_texture_open_xpm.bytes[(i % 16 + 16 * j) * 3 + 1] << 8)
                                 | door_texture_open_xpm.bytes[(i % 16 + 16 * j) * 3];
                colors[j] = color;
              }
            }
            vg_draw_vline_colormap(width - i, draw_start, draw_end - draw_start, colors);

            if (delimiter == 3 || delimiter == 4) {
              wall_height = (int) ((height * cellsize / 2) / distance_to_wall);
              draw_start = -wall_height / 2 + height / 2;
              if (draw_start < 0) draw_start = 0;
              draw_end = wall_height / 2 + height / 2;
              if (draw_end >= (int)height) draw_end = height - 1;
              uint32_t color = delimiter == 4 ? 0x00FF00 : 0x141c17;
              vg_draw_vline(width - i, draw_start, draw_end - draw_start, color);
            }
        }
        free(line);
    }
    return 0;
}

int (graphics_draw_map) () {
    if (vg_draw_rectangle(0, height - MINIMAP_HEIGHT - 40,
                          MINIMAP_WIDTH + 40, MINIMAP_HEIGHT + 40, BLACK) != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }
    if (vg_draw_rectangle(5, height - MINIMAP_HEIGHT - 35,
                          MINIMAP_WIDTH + 30, MINIMAP_HEIGHT + 30, 0xb5b5b3) != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }

    for (uint32_t y = 0; y < map_height; y++) {
        for (uint32_t x = 0; x < map_width; x++) {
            uint8_t pos;
            if (map_get_grid_pos(x, y, &pos) != 0) {
                printf("ERROR: %s\n", __func__ );
                return 1;
            }

            uint32_t color;

            if (pos == 1 || pos == 4) {
              color = 0x000000;
            } else if (pos == 8) {
              color = 0xff0000;
            } else if (pos == 16) {
              color = 0x00ff00;
            } else if (pos == 3) {
              color = 0xffff00;
            } else {
              color = 0xb5b5b3;
            }

            if (vg_draw_rectangle(x * (MINIMAP_WIDTH / map_width) + 15, (height - MINIMAP_HEIGHT - 30) + (y * (MINIMAP_HEIGHT / map_height) + 15)
                    , (MINIMAP_WIDTH / map_width), (MINIMAP_HEIGHT / map_height), color) != 0) {
                printf("ERROR: %s\n", __func__ );
                return 1;
            }
        }
    }

    struct point2D player = player_get_position();
    if (vg_draw_rectangle((player.x * MINIMAP_WIDTH / width + 15) -3, ((height - MINIMAP_HEIGHT) + player.y * MINIMAP_HEIGHT / height - 15) - 3,
                          6, 6, 0x0000FF) != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }
    return 0;
}

int (graphics_draw_cursor) () {
    uint16_t x, y;
    cursor_get_position(&x, &y);

    if (vg_draw_xpm(x, y, cursor_xpm.width, cursor_xpm.size, cursor_xpm.bytes) != 0) {
        printf("ERROR: %s\n", __func__ );
        return -1;
    }

    return 0;
}

int (create_line) (struct point2D* line, double angle, uint8_t* delimiter) {
    int index = 0;
    int x1 = player_get_position().x;
    int y1 = player_get_position().y;
    int x2 = (int) (x1 + LINE_DISTANCE * cos(to_radians(angle)));
    int y2 = (int) (y1 + LINE_DISTANCE * sin(to_radians(angle)));
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? -1 : 1;

    int err = dx - dy;

    while ((x1 != x2 || y1 != y2) && index < LINE_SIZE - 1) {
        uint8_t pos;
        if (map_get_grid_pos(x1 * 32 / 800, y1 * 24 / 600, &pos) != 0) {
            printf("ERROR: %s\n", __func__ );
            return -1;
        }

        if (pos != 0) {
            line[index].x = x1; line[index].y = y1;
            index++;
            *delimiter = pos;
            return index;
        }

        line[index].x = x1; line[index].y = y1;
        index++;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;

        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
    return index;
}

int (set_background_color) (uint32_t color) {
    if (vg_draw_rectangle(0, 0, width, height, color) != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }
    return 0;
}

void (load_xpms) () {
    xpm_load(main_menu, XPM_8_8_8, &menu_xpm);
    xpm_load(whitebrick, XPM_8_8_8, &wall_texture_xpm);
    xpm_load(cursor, XPM_8_8_8, &cursor_xpm);
    xpm_load(pause_menu, XPM_8_8_8, &pause_menu_xpm);
    xpm_load(doorLazerBad, XPM_8_8_8, &door_texture_lock_xpm);
    xpm_load(doorLazer, XPM_8_8_8, &door_texture_open_xpm);
    xpm_load(defeat, XPM_8_8_8, &defeat_xpm);
    xpm_load(victory, XPM_8_8_8, &victory_xpm);
    xpm_load(player_select, XPM_8_8_8, &player_select_xpm);
    xpm_load(letter_L, XPM_8_8_8, &password_xpm[0]);
    xpm_load(letter_C, XPM_8_8_8, &password_xpm[1]);
    xpm_load(letter_O, XPM_8_8_8, &password_xpm[2]);
    xpm_load(letter_M, XPM_8_8_8, &password_xpm[3]);
    xpm_load(number_0, XPM_8_8_8, &numbers_xpm[0]);
    xpm_load(number_1, XPM_8_8_8, &numbers_xpm[1]);
    xpm_load(number_2, XPM_8_8_8, &numbers_xpm[2]);
    xpm_load(number_3, XPM_8_8_8, &numbers_xpm[3]);
    xpm_load(number_4, XPM_8_8_8, &numbers_xpm[4]);
    xpm_load(number_5, XPM_8_8_8, &numbers_xpm[5]);
    xpm_load(number_6, XPM_8_8_8, &numbers_xpm[6]);
    xpm_load(number_7, XPM_8_8_8, &numbers_xpm[7]);
    xpm_load(number_8, XPM_8_8_8, &numbers_xpm[8]);
    xpm_load(number_9, XPM_8_8_8, &numbers_xpm[9]);
    xpm_load(two_dots, XPM_8_8_8, &two_dots_xpm);
}

double (get_ray_angle) (int diff) {
    return (player_get_angle() - (double) PLAYER_FOV / 2 + ((double) (PLAYER_FOV * diff) / width));
}
