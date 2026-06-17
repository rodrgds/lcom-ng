/**
 * @file text_utils.h
 * @brief Text layout utilities -- width estimation for proportional HUD font.
 * @ingroup group_ui
 *
 * Estimates the pixel width of a text string when rendered at a given
 * scale, used to centre-align text on buttons and panels.
 */

#ifndef PROJ_TEXT_UTILS_H
#define PROJ_TEXT_UTILS_H

#include <stdint.h>

/**
 * @brief Estimate the pixel width of a rendered text string.
 *
 * Sums glyph widths from the HUD font data and multiplies by the scale
 * factor.  Used to centre-align text on buttons and panels.
 *
 * @param text   Null-terminated string to measure.
 * @param scale  Integer scale factor.
 * @return Estimated width in pixels.
 */
uint16_t text_utils_estimate_width(const char *text, uint16_t scale);

#endif
