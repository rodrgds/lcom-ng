#include <assert.h>

#include "../include/game/cut_profile.h"
#include "../include/game/sushi.h"
#include "../include/ui/ui.h"
#include "../assets/sushi_pieces/cut_profiles.h"

static uint32_t frame[SCREEN_HEIGHT][SCREEN_WIDTH];
static int drawn_pixels;

/* Minimal host XPM decoder for the original, checked-in image fixtures. */
uint8_t *xpm_load(xpm_map_t xpm, enum xpm_image_type type, xpm_image_t *image) {
    unsigned width, height, colors, cpp;
    assert(sscanf(xpm[0], "%u %u %u %u", &width, &height, &colors, &cpp) == 4);
    uint32_t palette[colors];
    for (unsigned i = 0; i < colors; i++) {
        char color[16];
        assert(sscanf(xpm[i + 1] + cpp, "%*s %15s", color) == 1);
        if (strcmp(color, "None") == 0) palette[i] = CHROMA_KEY_GREEN_888;
        else assert(sscanf(color, "#%x", &palette[i]) == 1);
    }
    uint8_t *pixels = malloc((size_t)width * height * 3);
    assert(pixels != NULL);
    for (unsigned y = 0; y < height; y++) {
        const char *row = xpm[1 + colors + y];
        for (unsigned x = 0; x < width; x++) {
            unsigned entry = 0;
            while (entry < colors && strncmp(row + x * cpp, xpm[entry + 1], cpp) != 0) entry++;
            assert(entry < colors);
            size_t index = ((size_t)y * width + x) * 3;
            pixels[index] = palette[entry] & 255;
            pixels[index + 1] = (palette[entry] >> 8) & 255;
            pixels[index + 2] = palette[entry] >> 16;
        }
    }
    image->width = width;
    image->height = height;
    return pixels;
}

int vg_draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
    assert(x < SCREEN_WIDTH && y < SCREEN_HEIGHT);
    frame[y][x] = color;
    drawn_pixels++;
    return 0;
}

int video_draw_sprite(const uint8_t *pixels, uint16_t width, uint16_t height, uint16_t x, uint16_t y) {
    sprite_t sprite = {(uint8_t *)pixels, width, height};
    for (int row = 0; row < height && y + row < SCREEN_HEIGHT; row++) {
        for (int col = 0; col < width && x + col < SCREEN_WIDTH; col++) {
            if (!sprite_pixel_visible(&sprite, col, row)) continue;
            const uint8_t *pixel = pixels + ((size_t)row * width + col) * 3;
            vg_draw_pixel(x + col, y + row, pixel[0] | ((uint32_t)pixel[1] << 8) | ((uint32_t)pixel[2] << 16));
        }
    }
    return 0;
}

static void destroy_pieces(cut_piece_t pieces[2]) {
    for (int i = 0; i < 2; i++) {
        sprite_destroy(pieces[i].sprite);
        free(pieces[i].sprite);
    }
}

static void check_profiles(sushi_t *sushi) {
    const cut_profile_set_t *set = &sushi_cut_profiles[sushi->type];
    assert(sushi->xpm->width == set->width && sushi->xpm->height == set->height);
    for (int i = 0; i < CUT_PROFILE_COUNT; i++) {
        const cut_profile_t *profile = &set->profiles[i];
        point_t middle = {profile->nx * profile->offset, profile->ny * profile->offset};
        point_t start = {middle.x - profile->ny * 200, middle.y + profile->nx * 200};
        point_t end = {middle.x + profile->ny * 200, middle.y - profile->nx * 200};
        const cut_profile_t *selected = cut_profile_select(sushi->type, sushi->xpm, start, end);
        assert(selected != NULL);
        assert(fabs(selected->nx - profile->nx) < 1e-8);
        assert(fabs(selected->ny - profile->ny) < 1e-8);
        assert(fabs(selected->offset - profile->offset) < 1e-6);
        assert(cut_profile_select(sushi->type, sushi->xpm, end, start) == selected);

        for (int angle = 0; angle < 360; angle += 17) {
            rotation_t rotation = rotation_for_angle(angle);
            point_t world_start = rotation_apply(rotation, start);
            point_t world_end = rotation_apply(rotation, end);
            assert(cut_profile_select(sushi->type, sushi->xpm,
                rotation_inverse(rotation, world_start), rotation_inverse(rotation, world_end)) == selected);
        }

        cut_piece_t pieces[2];
        assert(cut_profile_split(sushi->xpm, selected, pieces) == 0);
        for (int y = 0; y < sushi->xpm->height; y++) {
            for (int x = 0; x < sushi->xpm->width; x++) {
                int visible_count = 0;
                for (int side = 0; side < 2; side++) {
                    int px = x - pieces[side].x, py = y - pieces[side].y;
                    if (!sprite_pixel_visible(pieces[side].sprite, px, py)) continue;
                    visible_count++;
                    const uint8_t *original = sushi->xpm->pixmap + ((size_t)y * sushi->xpm->width + x) * 3;
                    const uint8_t *fragment = pieces[side].sprite->pixmap +
                        ((size_t)py * pieces[side].sprite->width + px) * 3;
                    assert(memcmp(original, fragment, 3) == 0);
                }
                assert(visible_count == (int)sprite_pixel_visible(sushi->xpm, x, y));
            }
        }
        destroy_pieces(pieces);
    }
}

static void check_collision_and_drawing(void) {
    uint8_t pixels[27];
    for (int i = 0; i < 9; i++) {
        pixels[i * 3] = CHROMA_KEY_GREEN_888 & 255;
        pixels[i * 3 + 1] = (CHROMA_KEY_GREEN_888 >> 8) & 255;
        pixels[i * 3 + 2] = (CHROMA_KEY_GREEN_888 >> 16) & 255;
    }
    pixels[12] = 255; pixels[13] = 0; pixels[14] = 0;
    sprite_t sprite = {pixels, 3, 3};
    cut_piece_t unchanged[2] = {{&sprite, 4, 5}, {&sprite, 6, 7}};
    cut_profile_t empty_side = {1, 0, 1000};
    assert(cut_profile_split(&sprite, &empty_side, unchanged) != 0);
    assert(unchanged[0].sprite == &sprite && unchanged[0].x == 4 && unchanged[0].y == 5);
    assert(unchanged[1].sprite == &sprite && unchanged[1].x == 6 && unchanged[1].y == 7);
    assert(!sprite_segment_intersects(&sprite, (point_t){-100, 0.5}, (point_t){100, 0.5}));
    assert(sprite_segment_intersects(&sprite, (point_t){-100, 1.5}, (point_t){100, 1.5}));
    assert(sprite_segment_intersects(&sprite, (point_t){1, 0}, (point_t){1, 3}));
    assert(sprite_segment_intersects(&sprite, (point_t){0, 0}, (point_t){1, 1}));
    assert(sprite_segment_intersects(&sprite, (point_t){1.2, 1.2}, (point_t){1.3, 1.3}));
    assert(!sprite_segment_intersects(&sprite, (point_t){1.5, 1.5}, (point_t){1.5, 1.5}));

    /* An asymmetric silhouette exposes an incorrect rotation or pivot. */
    pixels[3] = 100; pixels[4] = 0; pixels[5] = 0;
    sushi_t sushi = create_sushi(100, 100, 0, 0, 0, &sprite, 1);
    sushi.angle = 90;
    assert(sushi_segment_intersects(&sushi, 102, 101, 104, 101, 0));
    assert(!sushi_segment_intersects(&sushi, 103, 100, 104, 100, 20));
    sushi.kind = SUSHI_BOMB;
    assert(sushi_segment_intersects(&sushi, 103, 100, 104, 100, 20));

    memset(frame, 0, sizeof(frame));
    drawn_pixels = 0;
    assert(ui_draw_sprite_rotated(&sprite, 100, 100, 90) == 0);
    assert(drawn_pixels == 2 && frame[101][102] == 100 && frame[101][101] == 255);
    assert(ui_draw_sprite_rotated(&sprite, -1, -1, 45) == 0);
    int previous = drawn_pixels;
    assert(ui_draw_sprite_rotated(&sprite, -100, -100, 30) == 0);
    assert(ui_draw_sprite_rotated(&sprite, SCREEN_WIDTH, SCREEN_HEIGHT, 30) == 0);
    assert(drawn_pixels == previous);
}

static void check_cut_lifecycle(uint32_t *seed) {
    sushi_t sushi = create_random_sushi(10, 200, seed);
    assert(sushi.xpm != NULL);
    sushi.x = PHYSICS_FROM_PIXELS(100);
    sushi.y = PHYSICS_FROM_PIXELS(100);
    sushi.angle = 37;
    sushi.angular_velocity = -2;
    sprite_t *original = sushi.xpm;
    sushi_fragment_t first = {0}, second = {0};
    assert(cut_sushi(&sushi, 0, 0, 0, 0, &first, &second) != 0);
    assert(sushi.xpm == original && first.xpm == NULL && second.xpm == NULL);
    uint8_t type = sushi.type;
    sushi.type = 255;
    assert(cut_sushi(&sushi, 0, 120, 300, 120, &first, &second) != 0);
    assert(sushi.xpm == original);
    sushi.type = type;

    double cx = 100 + original->width / 2.0, cy = 100 + original->height / 2.0;
    int cut_y = (int)cy;
    rotation_t rotation = rotation_for_angle(sushi.angle);
    point_t start = rotation_inverse(rotation, (point_t){0 - cx, cut_y - cy});
    point_t end = rotation_inverse(rotation, (point_t){300 - cx, cut_y - cy});
    const cut_profile_t *profile = cut_profile_select(type, original, start, end);
    cut_piece_t expected[2];
    assert(cut_profile_split(original, profile, expected) == 0);
    int parent_vx = sushi.vx, parent_vy = sushi.vy;
    assert(cut_sushi(&sushi, 0, cut_y, 300, cut_y, &first, &second) == 0);
    assert(sushi.xpm == NULL);
    sushi_fragment_t *fragments[2] = {&first, &second};
    for (int i = 0; i < 2; i++) {
        sushi_fragment_t *fragment = fragments[i];
        assert(fragment->angle == 37 && fragment->angular_velocity == -2);
        assert(fragment->gravity == 200);
        double offset_x = expected[i].x + fragment->xpm->width / 2.0 - (cx - 100);
        double offset_y = expected[i].y + fragment->xpm->height / 2.0 - (cy - 100);
        point_t offset = rotation_apply(rotation, (point_t){offset_x, offset_y});
        assert(fabs(fragment->x / (double)PHYSICS_SCALE + fragment->xpm->width / 2.0 - cx - offset.x) < 0.001);
        assert(fabs(fragment->y / (double)PHYSICS_SCALE + fragment->xpm->height / 2.0 - cy - offset.y) < 0.001);
    }
    assert(first.vx + second.vx == parent_vx * 2);
    assert(first.vy + second.vy == parent_vy * 2);
    first.angle = 0;
    update_sushi_fragment(&first);
    assert(first.angle == 358);
    sushi_fragment_destroy(&first);
    sushi_fragment_destroy(&second);
    destroy_pieces(expected);
}

int main(void) {
    uint32_t seed = 12345;
    bool seen[sizeof(sushi_cut_profiles) / sizeof(sushi_cut_profiles[0])] = {false};
    size_t checked = 0;
    for (int attempt = 0; checked < sizeof(seen) / sizeof(seen[0]) && attempt < 1000; attempt++) {
        sushi_t sushi = create_random_sushi(10, 200, &seed);
        assert(sushi.xpm != NULL && sushi.type < sizeof(seen) / sizeof(seen[0]));
        assert(sushi.angle >= 0 && sushi.angle < 360 && sushi.angular_velocity != 0);
        if (!seen[sushi.type]) {
            check_profiles(&sushi);
            seen[sushi.type] = true;
            checked++;
        }
        sushi.angle = 359;
        sushi.angular_velocity = 2;
        update_sushi(&sushi);
        assert(sushi.angle == 1);
        sushi_destroy(&sushi);
    }
    assert(checked == sizeof(seen) / sizeof(seen[0]));
    check_collision_and_drawing();
    check_cut_lifecycle(&seed);
    puts("Passed: 240 profiles, pixel conservation, rotation, grazes, clipping, and cut lifecycle");
    return 0;
}
