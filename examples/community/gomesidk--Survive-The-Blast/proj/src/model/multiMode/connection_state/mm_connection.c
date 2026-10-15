#include "mm_connection.h"  

struct mm_connection_imp {
  Sprite *background; // Background image for the connection state
  Sprite *waiting_connection; // Sprite for waiting connection
  Sprite *warning_connection; // Sprite for warning connection
  Button *leave;
  bool received_hello;
  int time_until_failure; // Time until the connection fails
  int delay;
};

MmConnection *create_mm_connection(SpriteLoader *loader) {
  MmConnection *mm_connection = (MmConnection *) malloc(sizeof(MmConnection));
  if (mm_connection == NULL) {
    return NULL;
  }

  sp_clear_buffers(); // Clear the serial port buffer

  mm_connection->background = get_died_background(loader);
  mm_connection->leave = create_button( (get_sprite_width(mm_connection->background) - get_sprite_width(get_exit_button(loader))) / 2, 800, get_exit_button(loader));
  mm_connection->received_hello = false;
  mm_connection->time_until_failure = 900; //30 seconds until failure
  mm_connection->delay = 15; // Delay for sending hello
  mm_connection->waiting_connection = get_waiting_connection(loader);
  mm_connection->warning_connection = get_warning_connection(loader);
  return mm_connection;
}

void destroy_mm_connection(MmConnection *mm_connection) {
  if (mm_connection == NULL) {
    return;
  }
  destroy_button(mm_connection->leave);
  free(mm_connection);
}

void draw_mm_connection(MmConnection *mm_connection) {
  if (mm_connection == NULL) {
    return;
  }
  
  draw_sprite(mm_connection->background, 0, 0);
  draw_button(mm_connection->leave);
  draw_sprite(mm_connection->waiting_connection, 
              (get_sprite_width(mm_connection->background) - get_sprite_width(mm_connection->waiting_connection)) / 2, 200);
  draw_sprite(mm_connection->warning_connection, 
              (get_sprite_width(mm_connection->background) - get_sprite_width(mm_connection->warning_connection)) / 2, 500);
              
}

int process_mm_connection_mouse(MmConnection *mm_connection, Cursor *c) {
  if (mm_connection == NULL || c == NULL) {
    return 1; // Go back to menu
  }

  if (is_button_clicked(mm_connection->leave, c)) {
    return 1; // Go back to menu
  }

  return 0; // Continue connection state
}

int process_mm_connection_timer(MmConnection *mm_connection) {
  if (mm_connection == NULL || mm_connection->time_until_failure <= 0) {
    return 1; // Go back to menu
  }

  if (!mm_connection->received_hello) {
    if (mm_connection->delay > 0) {
      mm_connection->delay--;
    } else {
      send_byte(HANDSHAKE_HELLO); 
      mm_connection->delay = 15; 
    }
  }
  mm_connection->time_until_failure--;
  return 0; 
}

int process_mm_connection_sp(MmConnection *mm_connection, uint8_t byte) {
  if (mm_connection == NULL) {
    return -1; // Go back to menu
  }

  if (byte == HANDSHAKE_HELLO) {
    mm_connection->received_hello = true; 
    send_byte(HANDSHAKE_REPLY); // Reply to the hello message
    return 1; // Player 1
  }else if(byte == HANDSHAKE_REPLY) {
    return 2; // Player 2
  }
  return 0; 
}

