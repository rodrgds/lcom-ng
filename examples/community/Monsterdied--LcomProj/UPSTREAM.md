# Upstream provenance

Original repository: https://github.com/Monsterdied/LcomProj

Pinned revision: `a2cf0924aaf80f1e2a9ae078bf9066e3daab962d`

Original sources and game art are retained. Exact source hashes and verification records are in UPSTREAM.json.

License: No license file found upstream. Public source availability does not imply a reuse license.

## Recorded source corrections

proj/src/main.c: Original callback int() conflicts with required LCF int(argc,argv) signature. Only signature corrected after compiler failure, original game logic unchanged.

Exact original/patched SHA256 and SOURCE_PATCH.diff are recorded in UPSTREAM.json.

## Controls

Original Bomberman-style two-player game: player1 WASD/SPACE, player2 arrows/INSERT. W/S/ENTER selects menu, typed names before game; ESC exits. Full keyboard supports names. Mouse route optional, disabled to keep board open.
