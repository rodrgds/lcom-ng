#ifndef SPLASH_MENU_H
#define SPLASH_MENU_H

/**
 * @brief Actions the splash screen can return.
 */
typedef enum {
    SPLASH_ACTION_NONE = 0,
    SPLASH_ACTION_DONE,
} SplashAction;

/**
 * @brief Resets the internal tick counter. Call when (re-)entering STATE_SPLASH.
 */
void splash_menu_reset(void);

/**
 * @brief Draws the full-screen splash: centered logo sprite, or title text as fallback.
 *        Calls renderer_clear() and renderer_flip() internally.
 */
void splash_menu_draw(void);

/**
 * @brief Advances the tick counter by one frame.
 * @return SPLASH_ACTION_DONE when SPLASH_DURATION_TICKS have elapsed,
 *         SPLASH_ACTION_NONE otherwise.
 */
SplashAction splash_menu_update(void);

#endif /* SPLASH_MENU_H */
