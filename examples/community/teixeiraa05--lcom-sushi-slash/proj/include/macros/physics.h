#ifndef _MACROS_PHYSICS_H_
#define _MACROS_PHYSICS_H_

/**
 * @file physics.h
 * @brief Defines fixed-point physics conversion helpers.
 */

/** @brief Number of fixed-point units that represent one pixel. */
#define PHYSICS_SCALE 1000

/** @brief Smaller fixed-point scale used for slow-motion motion calculations. */
#define SLOWMO_PHYSICS_SCALE 50

/** @brief Convert a whole-pixel value to fixed-point units. */
#define PHYSICS_FROM_PIXELS(value) ((value) * PHYSICS_SCALE)

/** @brief Convert a whole-pixel value to slow-motion fixed-point units. */
#define SLOWMO_PHYSICS_FROM_PIXELS(value) ((value) * SLOWMO_PHYSICS_SCALE)

/** @brief Convert a thousandths value into the fixed-point representation. */
#define PHYSICS_FROM_THOUSANDTHS(value) (value)

/** @brief Convert fixed-point units to whole pixels. */
#define PHYSICS_TO_PIXELS(value) ((value) / PHYSICS_SCALE)

/** @brief Build a fixed-point value from whole pixels and thousandths. */
#define PHYSICS_VALUE(integer, thousandths) \
  ((integer) * PHYSICS_SCALE + (thousandths))

#endif /* _MACROS_PHYSICS_H_ */
