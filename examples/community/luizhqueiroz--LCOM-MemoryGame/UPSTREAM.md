# Upstream provenance

Original repository: https://github.com/luizhqueiroz/LCOM-MemoryGame

Pinned revision: `d740913f2049d0ddc7e089eef59a865160111cf7`

Original sources and game art are retained. Exact source hashes and verification records are in UPSTREAM.json.

License: No license file found upstream. Public source availability does not imply a reuse license.

## Recorded source corrections

src/main.c: Original callback definition has no parameters, conflicting with the required LCF int(argc,argv) callback signature. Compile failed at main.c:130; only signature corrected, behavior unchanged.

Exact original/patched SHA256 and SOURCE_PATCH.diff are recorded in UPSTREAM.json.

## Controls

Original keyboard route:1 starts,2 records; arrows select card and ENTER flips. ESC returns/exits, full keyboard enters winner name. Pointer disabled because keyboard covers the game.

Cursor rendering correction: Draw the complete normal or selection cursor inside the frame in menu, game, winner and record phases without changing logical card/button hit coordinates. Verified by `test_memory_quest_draws_complete_cursor_in_every_phase` with the actual student rendering owner.
