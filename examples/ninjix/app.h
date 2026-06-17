/**
 * @file app.h
 * @brief Application lifecycle entry point.
 * @ingroup group_app
 *
 * The app layer owns the high-level state machine (menu, gameplay, pause,
 * overlays) and dispatches hardware interrupts to the active screen.
 */

#ifndef PROJ_APP_H
#define PROJ_APP_H

/**
 * @brief Initialise hardware, load resources, and run the main loop.
 *
 * This is the sole entry point called from proj_main_loop(). It configures
 * VBE graphics, subscribes to timer/keyboard/mouse interrupts, loads all
 * sprites via resources_load(), initialises the menu, and enters the
 * lcom_event_wait() loop that dispatches events until APP_SCREEN_EXIT.
 *
 * @return 0 on clean exit, non-zero on initialisation or unrecoverable failure.
 * @callgraph
 */
int app_run(void);

#endif
