#include "multiplayer/controller/multiplayer.h"

#include "protocol/protocol.h"

char *word = "bruhs";

void (multiplayer_start)() {
  protocol_init();
  protocol_send_handshake(word);
}

void (multiplayer_end)() {
  protocol_cleanup();
}

void (multiplayer_handle_ready)(struct event *ev) {
  system("echo 'ready' >> /home/lcom/labs/proj/src/pls.txt");
}

void (multiplayer_handle_submission)(struct event *ev) {
  system("echo 'submission' >> /home/lcom/labs/proj/src/pls.txt");
}
