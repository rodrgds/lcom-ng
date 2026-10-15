#!/usr/bin/env sh
set -eu

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
BUILD_DIR="$ROOT/build"
ASSET_DIR="$ROOT/docs/assets"
DEMO_DIR="$ASSET_DIR/demo"
SCREENSHOT_DIR="$ASSET_DIR/screenshots"
TMP_DIR="$BUILD_DIR/marketing-captures"

if [ ! -f "$BUILD_DIR/CMakeCache.txt" ]; then
  cmake -S "$ROOT" -B "$BUILD_DIR" -DMACHINE_LAB_WITH_SDL=ON
fi

cmake --build "$BUILD_DIR" --target machinelab word_processor music_maker breakout_demo flappy_bird ninjix -j4
mkdir -p "$DEMO_DIR" "$SCREENSHOT_DIR" "$TMP_DIR"

rm -f "$DEMO_DIR/word-processor.mp4" "$DEMO_DIR/music-maker.mp4" \
  "$DEMO_DIR/music-maker.wav" "$TMP_DIR/word.ppm" "$TMP_DIR/music.ppm" "$TMP_DIR/note.txt" "$TMP_DIR/pattern.mlpat"

"$BUILD_DIR/machinelab" replay "$ROOT/scripts/write_note.mlabscript" \
  --headless --video "$DEMO_DIR/word-processor.mp4" --video-fps 30 \
  --dump-frame "$TMP_DIR/word.ppm" -- "$BUILD_DIR/examples/word_processor" "$TMP_DIR/note.txt"

sed 's/^at 168 /at 360 /;s/^at 169 /at 361 /' \
  "$ROOT/scripts/music_maker_demo.mlabscript" > "$TMP_DIR/music.mlabscript"
"$BUILD_DIR/machinelab" replay "$TMP_DIR/music.mlabscript" \
  --headless --video "$DEMO_DIR/music-maker.mp4" --video-fps 30 \
  --audio-wav "$DEMO_DIR/music-maker.wav" --dump-frame "$TMP_DIR/music.ppm" \
  -- "$BUILD_DIR/examples/music_maker" "$TMP_DIR/pattern.mlpat"

ffmpeg -y -loglevel error -i "$TMP_DIR/word.ppm" "$SCREENSHOT_DIR/word-processor.png"
ffmpeg -y -loglevel error -i "$TMP_DIR/music.ppm" "$SCREENSHOT_DIR/music-maker.png"

"$BUILD_DIR/machinelab" replay "$ROOT/scripts/breakout_demo.mlabscript" \
  --headless --video "$DEMO_DIR/breakout.mp4" --video-fps 30 \
  --dump-frame "$TMP_DIR/breakout.ppm" -- "$BUILD_DIR/examples/breakout_demo"
"$BUILD_DIR/machinelab" replay "$ROOT/scripts/flappy_demo.mlabscript" \
  --headless --video "$DEMO_DIR/flappy.mp4" --video-fps 30 \
  --dump-frame "$TMP_DIR/flappy.ppm" -- "$BUILD_DIR/examples/flappy_bird"
"$BUILD_DIR/machinelab" run --headless --rtc 2026-08-12T14:35:42 \
  --script "$ROOT/scripts/ninjix_menu_exit.mlabscript" --max-ticks 300 \
  --dump-frame "$TMP_DIR/ninjix.ppm" -- "$BUILD_DIR/examples/ninjix"
for name in breakout flappy ninjix; do
  ffmpeg -y -loglevel error -i "$TMP_DIR/$name.ppm" "$SCREENSHOT_DIR/$name.png"
done

echo "captured marketing example assets"
