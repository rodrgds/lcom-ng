#ifndef _STATUS_BAR_H_
#define _STATUS_BAR_H_

#include <stdint.h>
#include "../init.h"
#include "../../../lab1/rtc.h"

/**
 * @file status_bar.h
 * @brief Declares status bar drawing data and API.
 */

/**
 * @brief Dynamic values rendered by the menu status bar.
 */
typedef struct {
  /** @brief Current real-world date from the RTC. */
  const rtc_date *date;

  /** @brief Current real-world time from the RTC. */
  const rtc_time_t *time;

  /** @brief Current measured frames per second. */
  uint32_t fps;
} StatusBarInfo;

/**
 * @brief Draw the status bar with RTC and performance information.
 *
 * @param assets Shared assets used for drawing.
 * @param status Values to display in the status bar.
 * @return 0 on success, non-zero on failure.
 */
int ui_status_bar_draw(const Assets *assets, const StatusBarInfo *status);

#endif /* _STATUS_BAR_H_ */
