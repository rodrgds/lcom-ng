/** 
 * @file structs.h
 * @brief Declares all the structs used in this project
 */


#ifndef _STRUCTS_PROJ_H_
#define _STRUCTS_PROJ_H_

#include <lcom/lcf.h>


/**
 * @enum event_type
 * @brief Represents the type of event on an input_event struct
 */
enum event_type {
  MOUSE_EVENT,   ///< An event from the mouse
  KEYBOARD_EVENT ///< An event from the keyboard
};

/**
 * @struct input_event
 * @brief Represents a mouse or keyboard event with all the required information
 */
typedef struct {
  enum event_type event_type;   ///< The valid event type on the union
  union {
    struct {
      uint8_t scancode_nbytes;  ///< The number of used bytes for the scancode
      uint8_t scancode_byte1;   ///< The first byte of the scancode
      uint8_t scancode_byte2;   ///< The second byte of the scancode
    };
    struct packet mouse_packet; ///< The mouse packet information
  };
} input_event;

/**
 * @struct input_event_node
 * @brief Represents a node to be used for an input_event linked list
 */
typedef struct _input_event_node input_event_node;
struct _input_event_node{
  input_event event;            ///< The stored event
  input_event_node* next_event; ///< A pointer to the next event
};

/**
 * @struct animation
 * @brief Represents an animation with an array of sprites and the number of them
 */
typedef struct {
  uint32_t n_frames;     ///< The number of sprites in the animation
  xpm_image_t** sprites; ///< A pointer to an array of sprites
} animation;

/**
 * @struct entity
 * @brief Represents an entity on the game
 */
typedef struct {
  double pos_x;                   ///< The x coordinate of the entity
  double pos_y;                   ///< The y coordinate of the entity
  double speed_x;                 ///< The speed on the x axis of the entity
  double speed_y;                 ///< The speed on the y axis of the entity
  double speed_multiplier;        ///< The real speed of the entity
  bool last_horizontal_direction; ///< Last horizontal direction, right is true, left is false
  uint32_t health;                ///< The current health of the entity
  animation* animations;          ///< A pointer to the animations to be used by the entity
} entity;

/**
 * @struct entity_node
 * @brief Represents a node to be used for an entity linked list
 */
typedef struct _entity_node entity_node;
struct _entity_node{
  entity* entity;           ///< The stored entity
  entity_node* next_entity; ///< A pointer to the next entity
};

/**
 * @struct attack
 * @brief Represents an attack on the game
 */
typedef struct {
  double pos_x; ///< The x coordinate of the attack
  double pos_y; ///< The y coordinate of the attack
  double speed_x; ///< The speed on the x axis of the attack
  double speed_y; ///< The speed on the y axis of the attack
  uint32_t frames; ///< The number of frames the attack is present
  uint32_t damage; ///< The damage the attack deals
  animation animation; ///< The animation used by the attack
} attack;

/**
 * @struct attack_node
 * @brief Represents a node to be used for an attack linked list
 */
typedef struct _attack_node attack_node;
struct _attack_node{
  attack* attack;           ///< The stored attack
  attack_node* next_attack; ///< A pointer to the next attack
};

/**
 * @struct arena
 * @brief Represents an arena on the game
 * 
 * The stored entities have a position relative to the arena, while the mouse has a position relative to the screen
 */
typedef struct {
  entity* player;                ///< A pointer to the player entity
  entity* mouse;                 ///< A pointer to the mouse entity
  entity_node* enemies;          ///< A pointer to the active enemies list
  attack_node* player_attacks;   ///< A pointer to the active attacks list
  entity_node* hearts;           ///< A pointer to the active hearts list
  double pos_x;                  ///< The x coordinate of the arena
  double pos_y;                  ///< The y coordinate of the arena
  xpm_image_t* background_image; ///< A pointer to the game background image
} arena;

/**
 * @struct menu
 * @brief Represents a menu
 */
typedef struct {
  int menu_status;               ///< Indicates which option is selected by the index
  int num_options;               ///< Total number of options
  xpm_image_t** sprites;         ///< The sprites of the options
  xpm_image_t* background_image; ///< The menu background image
} menu;

#endif /*_STRUCTS_PROJ_H_ */
