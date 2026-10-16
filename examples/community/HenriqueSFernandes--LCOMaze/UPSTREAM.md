# Upstream provenance

Original repository: https://github.com/HenriqueSFernandes/LCOMaze

Pinned revision: `a0dc6e641d93c36047e1e740833854013b54799d`

Original sources and game art are retained. Exact source hashes and verification records are in UPSTREAM.json.

License: No license file found upstream. Public source availability does not imply a reuse license.


## Controls

Original command argument host selects single-player-capable role. Mouse clicks choose menu and holding left button aims; WASD moves relative to aim. Source preserves half sensitivity and inclusive1152×864 clamping. The recorded source correction consumes complete packets once. Desktop calibration uses the original0,0 cursor and0.5 gain; mobile gestures remain relative.

## Source correction

Only complete mouse packets update menu/game cursor state. SOURCE_PATCH.diff records the correction; the public CLI regression renders the original cursor at10,5 after movement20,-10 followed by a neutral packet.
