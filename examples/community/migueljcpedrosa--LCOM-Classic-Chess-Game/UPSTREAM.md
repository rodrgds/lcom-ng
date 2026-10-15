# Upstream provenance

Original repository: https://github.com/migueljcpedrosa/LCOM-Classic-Chess-Game

Pinned revision: `d4174ee225077df23cdf0c453639f88a2c8c0848`

Original sources and game art are retained. Exact source hashes and verification records are in UPSTREAM.json.

License: No license file found upstream. Public source availability does not imply a reuse license.

## Recorded source corrections

proj/src/model/game/pieces.h: Original pieces.h redefines WHITE and BLACK already declared by included game_util.h Color. Use the already-defined Color type documented for this member; values and behavior unchanged.

Exact original/patched SHA256 and SOURCE_PATCH.diff are recorded in UPSTREAM.json.

## Controls

Original ENTER starts, typed player name, mouse selects/moves pieces and ESC exits. Full keyboard supports name entry. Relative pointer avoids an unverified guest cursor contract.
