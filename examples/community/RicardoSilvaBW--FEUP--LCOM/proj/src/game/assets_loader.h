#ifndef ASSETS_LOADER_H
#define ASSETS_LOADER_H

#include "../model/note.h"

void (initialize_notes)();
void (initialize_hud_assets)(void);
void (destroy_sprites)();
void (assign_note_type)(note* n);

#endif
