#!/usr/bin/env sh
set -eu
ROOT="${LCOM_NG_ROOT:-$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)}"
CLI="$ROOT/build/machinelab"
OUT="${LCOM_NG_LAUNCH_CLIPS:-$ROOT/build/launch-clips}"
mkdir -p "$OUT"
cmake --build "$ROOT/build" --target machinelab word_processor pixel_studio tilemap_editor \
  ambient_aquarium interrupt_scope ninjix serial_whiteboard breakout_demo flappy_bird pvz -j4
# Screenshots and action clips share authored inputs and the same rebuilt apps.
for app in pixel_studio tilemap_editor ambient_aquarium interrupt_scope; do
  case "$app" in
    pixel_studio) artifact="$OUT/sprite.h" ;;
    tilemap_editor) artifact="$OUT/map.mlmap" ;;
    *) artifact="" ;;
  esac
  if [ -n "$artifact" ]; then rm -f "$artifact"; fi
  sed '/ caption /d' "$ROOT/scripts/${app}_demo.mlabscript" > "$OUT/$app.mlabscript"
  "$CLI" run --headless --rtc 2026-08-12T14:35:42 \
    --script "$OUT/$app.mlabscript" \
    --video "$OUT/$app.mp4" --video-fps 30 --max-ticks 1000 -- \
    "$ROOT/build/examples/$app" "$artifact" > "$OUT/$app.log"
done
# The launch opens on actual gameplay, with captions omitted from the art.
sed '/ caption /d' "$ROOT/scripts/ninjix_level1_demo.mlabscript" \
  > "$OUT/ninjix.mlabscript"
"$CLI" run --headless --rtc 2026-08-12T14:35:42 \
  --script "$OUT/ninjix.mlabscript" --max-ticks 4000 \
  --video "$OUT/ninjix.mp4" --video-fps 30 -- \
  "$ROOT/build/examples/ninjix" > "$OUT/ninjix.log"
# The launch uses longer action excerpts than the short screenshot replays.
python3 - "$ROOT" "$OUT" <<'PYCODE'
import json
from pathlib import Path
import sys
root, output = map(Path, sys.argv[1:])
apps = json.loads((root / 'scripts/showcase-gallery.json').read_text())['apps']
for slug, state_name, name, stop in [
    ('plants-vs-zombies', 'garden', 'pvz', 'at 1140 key P down\nat 1141 key P up\nat 1144 key X down\nat 1145 key X up\n'),
    ('breakout', 'brick-wall', 'breakout', 'at 240 key ESC down\nat 241 key ESC up\n'),
    ('flappy-bird', 'through-gap', 'flappy', 'at 420 key ESC down\nat 421 key ESC up\n'),
]:
    app = next(app for app in apps if app['slug'] == slug)
    state = next(state for state in app['states'] if state['slug'] == state_name)
    (output / (name + '.mlabscript')).write_text(state['script'] + stop)
PYCODE
for name in breakout flappy pvz; do
  case "$name" in
    breakout) executable=breakout_demo ;;
    flappy) executable=flappy_bird ;;
    pvz) executable=pvz ;;
  esac
  (cd "$OUT" && "$CLI" run --headless --rtc 2026-08-12T14:35:42 \
    --script "$OUT/$name.mlabscript" --max-ticks 1200 \
    --video "$OUT/$name.mp4" --video-fps 30 -- \
    "$ROOT/build/examples/$executable" > "$OUT/$name.log")
done
# Show a real edit and Command-S save on a populated notebook, with time to read.
python3 - "$ROOT" "$OUT" <<'PYCODE'
import json
from pathlib import Path
import sys
root, output = map(Path, sys.argv[1:])
apps = json.loads((root / 'scripts/showcase-gallery.json').read_text())['apps']
app = next(app for app in apps if app['slug'] == 'word-processor')
state = next(state for state in app['states'] if state['slug'] == 'saved')
(output / 'notebook.txt').write_text(app['fixtures']['notebook.txt'])
(output / 'word-processor.mlabscript').write_text(state['script'] +
    'at 180 key ESC down\nat 181 key ESC up\n')
PYCODE
"$CLI" run --headless --rtc 2026-08-12T14:35:42 \
  --script "$OUT/word-processor.mlabscript" --max-ticks 300 \
  --video "$OUT/word-processor.mp4" --video-fps 30 -- \
  "$ROOT/build/examples/word_processor" "$OUT/notebook.txt" > "$OUT/word-processor.log"
python3 "$(dirname -- "$0")/capture_paired_clip.py" --root "$ROOT" --output "$OUT"
echo "Captured launch footage in $OUT"
