#pragma once

/** @defgroup weapon weapon
 * @{
 * Functions for manipulating weapon objects
 */

/**
 * @brief      Represents one weapon object
 */
struct Weapon {
  int id;				/**< weapon id */
  int damage;			/**< weapon inflicting damage */
  int max_magazine;		/**< weapon max holding magazine */
  int current_magazine;	/**< weapon currently holding magazine */
};

/**
 * @brief      Initializw weapon structure
 */
void init_weapons();
