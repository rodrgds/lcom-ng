#!/usr/bin/env sh
set -eu

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
BUILD_DIR="$ROOT/build"
EXAMPLE_DIR="$BUILD_DIR/examples"
SCREENSHOT_DIR="$ROOT/docs/assets/screenshots"
TMP_DIR="$BUILD_DIR/showcase-captures"
CLI="$BUILD_DIR/machinelab"

ppm_rgb_at() {
  ppm_path="$1"
  ppm_x="$2"
  ppm_y="$3"
  ppm_offset=$((15 + (ppm_y * 800 + ppm_x) * 3))
  dd if="$ppm_path" bs=1 skip="$ppm_offset" count=3 2>/dev/null |
    od -An -tx1 |
    tr -d ' \n'
}

if [ ! -f "$BUILD_DIR/CMakeCache.txt" ]; then
  cmake -S "$ROOT" -B "$BUILD_DIR" -DMACHINE_LAB_WITH_SDL=ON
fi

cmake --build "$BUILD_DIR" --target \
  machinelab \
  pixel_studio \
  serial_whiteboard \
  pomodoro_clock \
  interrupt_scope \
  tilemap_editor \
  serial_transfer \
  ambient_aquarium \
  -j4

mkdir -p "$SCREENSHOT_DIR" "$TMP_DIR"

"$CLI" run --headless --rtc 2026-08-12T14:35:42 \
  --script "$ROOT/scripts/pixel_studio_demo.mlabscript" \
  --dump-frame "$TMP_DIR/pixel-studio.ppm" --max-ticks 900 -- \
  "$EXAMPLE_DIR/pixel_studio" "$TMP_DIR/pixel-studio-sprite.h" \
  > "$TMP_DIR/pixel-studio.txt"
grep -q "exports=1" "$TMP_DIR/pixel-studio.txt"
grep -q "PIXEL_STUDIO_SPRITE_WIDTH 16u" "$TMP_DIR/pixel-studio-sprite.h"
grep -q "PIXEL_STUDIO_SPRITE_HEIGHT 16u" "$TMP_DIR/pixel-studio-sprite.h"
grep -q "pixel_studio_sprite\\[256\\]" "$TMP_DIR/pixel-studio-sprite.h"
grep -q "Pixel Studio export: 2026-08-12 14:35:42" \
  "$TMP_DIR/pixel-studio-sprite.h"
test "$(grep -Eo '0x[0-9A-F]{8}u' "$TMP_DIR/pixel-studio-sprite.h" | wc -l | tr -d ' ')" -eq 256
"${CC:-cc}" -std=c11 -pedantic-errors -fsyntax-only \
  -include "$TMP_DIR/pixel-studio-sprite.h" -x c /dev/null

"$CLI" run-pair --headless \
  --left-script "$ROOT/scripts/serial_whiteboard_left.mlabscript" \
  --right-script "$ROOT/scripts/serial_whiteboard_right.mlabscript" \
  --left-dump-frame "$TMP_DIR/serial-whiteboard-left.ppm" \
  --right-dump-frame "$TMP_DIR/serial-whiteboard-right.ppm" \
  --max-ticks 1000 \
  "$EXAMPLE_DIR/serial_whiteboard" left \
  --right "$EXAMPLE_DIR/serial_whiteboard" right \
  > "$TMP_DIR/serial-whiteboard.txt" 2>&1
test "$(grep -c "connected=yes" "$TMP_DIR/serial-whiteboard.txt")" -eq 2
grep -q "role=left connected=yes local_segments=120 remote_segments=120 local_hash=0A remote_hash=DE" \
  "$TMP_DIR/serial-whiteboard.txt"
grep -q "role=right connected=yes local_segments=120 remote_segments=120 local_hash=DE remote_hash=0A" \
  "$TMP_DIR/serial-whiteboard.txt"
test "$(ppm_rgb_at "$TMP_DIR/serial-whiteboard-left.ppm" 450 232)" = "2d7ff9"
test "$(ppm_rgb_at "$TMP_DIR/serial-whiteboard-right.ppm" 450 232)" = "2d7ff9"

"$CLI" run --headless --rtc 2026-07-30T09:40:00 \
  --script "$ROOT/scripts/pomodoro_clock_demo.mlabscript" \
  --audio-wav "$TMP_DIR/pomodoro-clock.wav" \
  --dump-frame "$TMP_DIR/pomodoro-clock.ppm" --max-ticks 500 -- \
  "$EXAMPLE_DIR/pomodoro_clock" > "$TMP_DIR/pomodoro-clock.txt"
grep -Eq "completions=[1-9][0-9]*" "$TMP_DIR/pomodoro-clock.txt"
grep -Eq "rtc_jumps=[1-9][0-9]*" "$TMP_DIR/pomodoro-clock.txt"
grep -Eq "audio_signals=[1-9][0-9]*" "$TMP_DIR/pomodoro-clock.txt"
head -c 4 "$TMP_DIR/pomodoro-clock.wav" | grep -q RIFF

"$CLI" run --headless --rtc 2026-07-30T09:40:00 \
  --script "$ROOT/scripts/interrupt_scope_demo.mlabscript" \
  --dump-frame "$TMP_DIR/interrupt-scope.ppm" --max-ticks 360 -- \
  "$EXAMPLE_DIR/interrupt_scope" > "$TMP_DIR/interrupt-scope.txt"
grep -Eq "uart_tx=[1-9][0-9]* uart_rx=[1-9][0-9]*" \
  "$TMP_DIR/interrupt-scope.txt"
grep -Eq "keyboard_irq=[1-9][0-9]*" "$TMP_DIR/interrupt-scope.txt"
grep -Eq "mouse_packets=[1-9][0-9]*" "$TMP_DIR/interrupt-scope.txt"
grep -Eq "rtc_polls=[1-9][0-9]*" "$TMP_DIR/interrupt-scope.txt"
grep -q "frozen=0 loopback=1" "$TMP_DIR/interrupt-scope.txt"

"$CLI" run --headless --rtc 2026-08-12T15:10:05 \
  --script "$ROOT/scripts/tilemap_editor_demo.mlabscript" \
  --dump-frame "$TMP_DIR/tilemap-editor.ppm" --max-ticks 700 -- \
  "$EXAMPLE_DIR/tilemap_editor" "$TMP_DIR/tilemap-demo.mlmap" \
  > "$TMP_DIR/tilemap-editor.txt"
grep -q "valid=1" "$TMP_DIR/tilemap-editor.txt"
grep -q "solved=1" "$TMP_DIR/tilemap-editor.txt"
grep -q "^# saved 2026-08-12T15:10:05$" "$TMP_DIR/tilemap-demo.mlmap"
grep -q "^# validation valid$" "$TMP_DIR/tilemap-demo.mlmap"
awk '
  /^size 10 8$/ { in_map = 1; next }
  /^end$/ { in_map = 0; found_end = 1; next }
  in_map {
    if (length($0) != 10 || $0 ~ /[^.#GCP+*]/) exit 1
    rows++
  }
  END { if (rows != 8 || !found_end) exit 1 }
' "$TMP_DIR/tilemap-demo.mlmap"

"$CLI" run-pair --headless \
  --left-dump-frame "$TMP_DIR/serial-transfer-sender.ppm" \
  --right-dump-frame "$TMP_DIR/serial-transfer-receiver.ppm" \
  --max-ticks 1400 \
  "$EXAMPLE_DIR/serial_transfer" send fault \
  --right "$EXAMPLE_DIR/serial_transfer" receive \
  > "$TMP_DIR/serial-transfer.txt" 2>&1
grep -q "role=sender complete bytes=256" "$TMP_DIR/serial-transfer.txt"
grep -q "fault=drop:recovered" "$TMP_DIR/serial-transfer.txt"
grep -q "role=receiver complete bytes=256" "$TMP_DIR/serial-transfer.txt"
test "$(ppm_rgb_at "$TMP_DIR/serial-transfer-sender.ppm" 112 240)" = "ef6461"
test "$(ppm_rgb_at "$TMP_DIR/serial-transfer-receiver.ppm" 112 240)" = "ef6461"

"$CLI" run --headless --rtc 2026-07-30T08:15:00 \
  --script "$ROOT/scripts/ambient_aquarium_demo.mlabscript" \
  --audio-wav "$TMP_DIR/ambient-aquarium.wav" \
  --dump-frame "$TMP_DIR/ambient-aquarium.ppm" --max-ticks 520 -- \
  "$EXAMPLE_DIR/ambient_aquarium" > "$TMP_DIR/ambient-aquarium.txt"
grep -q "phase=day lighting=manual" "$TMP_DIR/ambient-aquarium.txt"
grep -Eq "motion_ticks=[1-9][0-9]*" "$TMP_DIR/ambient-aquarium.txt"
grep -Eq "rtc_reads=[2-9][0-9]*" "$TMP_DIR/ambient-aquarium.txt"
grep -q "lighting_changes=3 audio_signals=3" "$TMP_DIR/ambient-aquarium.txt"
grep -q "feeds=2" "$TMP_DIR/ambient-aquarium.txt"
grep -q "paused=0" "$TMP_DIR/ambient-aquarium.txt"
head -c 4 "$TMP_DIR/ambient-aquarium.wav" | grep -q RIFF

for name in \
  pixel-studio \
  serial-whiteboard-left \
  serial-whiteboard-right \
  pomodoro-clock \
  interrupt-scope \
  tilemap-editor \
  serial-transfer-sender \
  serial-transfer-receiver \
  ambient-aquarium
do
  ffmpeg -y -loglevel error \
    -i "$TMP_DIR/$name.ppm" "$SCREENSHOT_DIR/$name.png"
  dimensions="$(ffprobe -v error -select_streams v:0 \
    -show_entries stream=width,height -of csv=s=x:p=0 \
    "$SCREENSHOT_DIR/$name.png")"
  test "$dimensions" = "800x600"
  bytes="$(wc -c < "$SCREENSHOT_DIR/$name.png" | tr -d ' ')"
  test "$bytes" -gt 4096
done

if cmp -s "$SCREENSHOT_DIR/serial-whiteboard-left.png" \
    "$SCREENSHOT_DIR/serial-whiteboard-right.png"; then
  echo "whiteboard peer captures must be visually distinct" >&2
  exit 1
fi
if cmp -s "$SCREENSHOT_DIR/serial-transfer-sender.png" \
    "$SCREENSHOT_DIR/serial-transfer-receiver.png"; then
  echo "transfer peer captures must be visually distinct" >&2
  exit 1
fi

echo "captured seven showcase studios in $SCREENSHOT_DIR"
