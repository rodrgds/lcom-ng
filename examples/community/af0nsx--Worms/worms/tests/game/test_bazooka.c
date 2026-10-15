#include "test_bazooka.h"
#include <stdio.h>
#include <stdbool.h>

#include "../../src/config.h"
#include "../../src/game/game.h"
#include "../../src/game/weapons/bazooka/bazooka_controller.h"
#include "../../src/game/terrain/terrain.h"

void run_bazooka_tests(void) {
    printf("[Test] Running Bazooka logic tests...\n");

    GameContext *ctx = game_context_create();
    if (ctx == NULL) {
        printf("[Test] FAILED: Could not create GameContext.\n");
        return;
    }

    terrain_explode(100, 100, 50);

    int32_t start_x = 100;
    int32_t start_y = 100;
    Bazooka *created = bazooka_create((double)start_x, (double)start_y, BAZOOKA_SPEED, 0);
    game_context_set_bazooka(ctx, created);

    Bazooka *b = game_context_get_bazooka(ctx);
    if (b == NULL || !bazooka_is_active(b)) {
        printf("[Test] FAILED: Bazooka not active after creation.\n");
        game_context_destroy(ctx);
        return;
    }

    Projectile *p_init = bazooka_get_projectile(b);
    double vx = projectile_get_vx(p_init);
    double vy = projectile_get_vy(p_init);
    if (vx != BAZOOKA_SPEED || vy != 0) {
        printf("[Test] FAILED: Velocity mismatch (vx=%.2f, vy=%.2f).\n", vx, vy);
        game_context_destroy(ctx);
        return;
    }

    bazooka_controller_tick(ctx, b);
    
    Projectile *p = bazooka_get_projectile(b);
    Hitbox hb = projectile_get_hitbox(p);
    
    int32_t expected_x = start_x + (int32_t)BAZOOKA_SPEED;
    if (hb.x != expected_x || hb.y != start_y) {
        printf("[Test] FAILED: Movement physics mismatch. Expected (x=%d, y=%d), got (x=%d, y=%d).\n", 
               expected_x, start_y, hb.x, hb.y);
        game_context_destroy(ctx);
        return;
    }

    int max_ticks = 1000;
    bool exploded = false;
    
    for (int i = 0; i < max_ticks; i++) {
        bazooka_controller_tick(ctx, b);
        
        if (!bazooka_is_active(b)) {
            exploded = true;
            break;
        }
    }
    
    if (!exploded) {
        printf("[Test] FAILED: Bazooka never exploded or reached bounds.\n");
        game_context_destroy(ctx);
        return;
    }
    
    printf("[Test] Bazooka physics and collision logic tests PASSED!\n");

    game_context_destroy(ctx);
}
