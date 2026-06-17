/**
 * @file app_constants.h
 * @brief Global timing constants -- timer, physics, and render frequencies.
 * @ingroup group_app
 *
 * All three subsystems run at 60 Hz. The timer hardware interrupts at
 * APP_TIMER_HZ and the app sub-samples to APP_PHYSICS_HZ and APP_RENDER_HZ
 * using per-frame tick accumulation.
 */

#ifndef PROJ_APP_CONSTANTS_H
#define PROJ_APP_CONSTANTS_H

#define APP_TIMER_HZ 60
#define APP_PHYSICS_HZ 60
#define APP_RENDER_HZ 60

#endif
