#pragma once

#include <lcom/lcf.h>

#include "events/events.h"

void (multiplayer_start)();
void (multiplayer_end)();

void (multiplayer_handle_packet)(struct event *ev);
void (multiplayer_handle_handshake)(struct event *ev);
void (multiplayer_smth)(struct event *ev);
