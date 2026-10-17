#pragma once

#include <lcom/lcf.h>

#include "events/events.h"

void (remote_player_start)();

void (remote_player_end)();

void (remote_player_handle_submission)(struct event *ev);
