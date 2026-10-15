#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

// ---------------------------------------------------------
// MOCKS
// ---------------------------------------------------------

static int mock_rand_value = 0;
static int test_rand(void) { return mock_rand_value; }
#define rand test_rand

#include "../../../src/config.h"
#include "../../../src/game/worm/worm.h"
#include "../../../src/engine/graphics/animated_sprite.h"
#include "../../../src/assets/assets.h"
#include "../../../src/engine/graphics/renderer.h"

// Provide definition for AnimatedSprite
struct AnimatedSprite {
    int id;
    bool done;
    int reset_count;
    int tick_count;
};

// Mock assets
Sprite* const* assets_get_walk_l(void) { return NULL; }
uint8_t assets_get_walk_count(void) { return 1; }
Sprite* const* assets_get_walk_r(void) { return NULL; }
Sprite* const* assets_get_jump_l(void) { return NULL; }
uint8_t assets_get_jump_count(void) { return 1; }
Sprite* const* assets_get_jump_r(void) { return NULL; }
Sprite* const* assets_get_die(void) { return NULL; }
uint8_t assets_get_die_count(void) { return 1; }
Sprite* const* assets_get_idle1(void) { return NULL; }
uint8_t assets_get_idle1_count(void) { return 1; }
Sprite* const* assets_get_idle2(void) { return NULL; }
uint8_t assets_get_idle2_count(void) { return 1; }
Sprite* const* assets_get_idle3(void) { return NULL; }
uint8_t assets_get_idle3_count(void) { return 1; }
Sprite* const* assets_get_idle4(void) { return NULL; }
uint8_t assets_get_idle4_count(void) { return 1; }

// Mock animated_sprite
AnimatedSprite* animated_sprite_create(Sprite *const *frames, uint8_t frame_count, uint8_t ticks_per_frame, bool loop) {
    (void)frames; (void)frame_count; (void)ticks_per_frame; (void)loop;
    AnimatedSprite* s = malloc(sizeof(AnimatedSprite));
    s->id = 0;
    s->done = false;
    s->reset_count = 0;
    s->tick_count = 0;
    return s;
}

void animated_sprite_destroy(AnimatedSprite *as) { free(as); }
void animated_sprite_tick(AnimatedSprite *as) { if (as) as->tick_count++; }
void animated_sprite_reset(AnimatedSprite *as) {
    if (as) {
        as->reset_count++;
        as->done = false;
    }
}
const Sprite* animated_sprite_get_current_frame(const AnimatedSprite *as) { 
    (void)as;
    return NULL; 
}
bool animated_sprite_is_done(const AnimatedSprite *as) { return as ? as->done : false; }

// Mock renderer
int renderer_draw_sprite(const Sprite* sprite, int x, int y) { 
    (void)sprite; (void)x; (void)y;
    return 0; 
}

// Mock worm
static WormState mock_worm_state = WORM_IDLE;
static WormDirection mock_worm_direction = WORM_DIR_RIGHT;

int worm_get_x(const Worm *worm) { (void)worm; return 0; }
int worm_get_y(const Worm *worm) { (void)worm; return 0; }
WormState worm_get_state(const Worm *worm) { (void)worm; return mock_worm_state; }
WormDirection worm_get_direction(const Worm *worm) { (void)worm; return mock_worm_direction; }

// Include the source to test
#include "../../../src/game/worm/worm_viewer.c"


static void test_random_idle_animations(void) {
    mock_rand_value = 2; // Initial random value
    WormViewer* wv = worm_viewer_create();
    assert(wv != NULL);
    assert(wv->current_idle == 2);

    Worm* w = (Worm*) 1; // Dummy pointer to pass NULL check
    
    // Simulate idle animation
    mock_worm_state = WORM_IDLE;
    worm_viewer_draw(wv, w); // tick 1
    assert(wv->idle_anims[2]->tick_count == 1);
    
    // Make the animation finish
    wv->idle_anims[2]->done = true;
    mock_rand_value = 1; // Next random value
    
    worm_viewer_draw(wv, w); // Next frame should trigger random change
    
    assert(wv->current_idle == 1);
    assert(wv->idle_anims[1]->reset_count == 1);
    assert(wv->idle_anims[1]->tick_count == 1); // Because after reset it gets assigned as current_anim and ticked
    
    // Now idle_anims[1] is running
    wv->idle_anims[1]->done = true;
    mock_rand_value = 3;
    
    worm_viewer_draw(wv, w);
    assert(wv->current_idle == 3);
    assert(wv->idle_anims[3]->reset_count == 1);
    
    worm_viewer_destroy(wv);
}

int main(void) {
    test_random_idle_animations();
    return 0;
}
