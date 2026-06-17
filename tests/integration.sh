#!/bin/sh
set -eu

BIN_DIR="${1:-build}"
EX_DIR="${2:-build/examples}"
OUT_DIR="${3:-build/test-output}"

mkdir -p "$OUT_DIR"

"$BIN_DIR/lcom" lab list > "$OUT_DIR/labs.txt"
grep -q "lab7" "$OUT_DIR/labs.txt"
"$BIN_DIR/lcom" lab show lab5 > "$OUT_DIR/lab5.txt"
grep -q "video_set_mode" "$OUT_DIR/lab5.txt"
"$BIN_DIR/lcom" lab show lab7 > "$OUT_DIR/lab7.txt"
grep -q "uart_config" "$OUT_DIR/lab7.txt"
"$BIN_DIR/lcom" docs cli > "$OUT_DIR/cli-docs.md"
grep -q "lcom bundle" "$OUT_DIR/cli-docs.md"
"$BIN_DIR/lcom" completion bash > "$OUT_DIR/lcom.bash"
grep -q "_lcom_complete" "$OUT_DIR/lcom.bash"

if "$BIN_DIR/lcom" run > "$OUT_DIR/run-missing.txt" 2>&1; then
  echo "lcom run without a program should fail" >&2
  exit 1
fi
grep -q "run requires <program>" "$OUT_DIR/run-missing.txt"
grep -Fq "run [OPTIONS]" "$OUT_DIR/run-missing.txt"

if "$BIN_DIR/lcom" bundle > "$OUT_DIR/bundle-missing.txt" 2>&1; then
  echo "lcom bundle without a project dir should fail" >&2
  exit 1
fi
grep -q "bundle requires <project-dir>" "$OUT_DIR/bundle-missing.txt"
grep -Fq "bundle [OPTIONS] project-dir" "$OUT_DIR/bundle-missing.txt"

if "$BIN_DIR/lcom" completion > "$OUT_DIR/completion-missing.txt" 2>&1; then
  echo "lcom completion without a shell should fail" >&2
  exit 1
fi
grep -q "completion requires <shell>" "$OUT_DIR/completion-missing.txt"
grep -Fq "completion [OPTIONS] shell" "$OUT_DIR/completion-missing.txt"

"$BIN_DIR/lcom" run --headless -- "$EX_DIR/hello" > "$OUT_DIR/hello.txt"
grep -q "hello from liblcom-ng" "$OUT_DIR/hello.txt"

"$BIN_DIR/lcom" run --headless -- "$EX_DIR/timer_int" 3 > "$OUT_DIR/timer.txt"
grep -q "timer tick 3" "$OUT_DIR/timer.txt"

"$BIN_DIR/lcom" run --headless --trace "$OUT_DIR/timer.trace.jsonl" -- "$EX_DIR/timer_int" 1 > "$OUT_DIR/timer_trace.txt"
test -s "$OUT_DIR/timer.trace.jsonl"
grep -q '"event":"irq"' "$OUT_DIR/timer.trace.jsonl"

"$BIN_DIR/lcom" run --headless --script scripts/type_a_esc.lcomscript -- "$EX_DIR/keyboard_scan" > "$OUT_DIR/kbd.txt"
grep -q "kbd make 0x1e" "$OUT_DIR/kbd.txt"
grep -q "kbd break 0x81" "$OUT_DIR/kbd.txt"

"$BIN_DIR/lcom" replay scripts/type_a_esc.lcomscript --headless -- "$EX_DIR/keyboard_scan" > "$OUT_DIR/replay.txt"
grep -q "kbd break 0x81" "$OUT_DIR/replay.txt"

"$BIN_DIR/lcom" run --headless --rtc 2026-06-16T12:34:56 -- "$EX_DIR/rtc_date" > "$OUT_DIR/rtc.txt"
grep -q "rtc 26-06-16 12:34:56" "$OUT_DIR/rtc.txt"

"$BIN_DIR/lcom" run --headless -- "$EX_DIR/uart_loopback" > "$OUT_DIR/uart.txt"
grep -q "uart rx L" "$OUT_DIR/uart.txt"

"$BIN_DIR/lcom" run --headless -- "$EX_DIR/uart_pair" > "$OUT_DIR/uart_pair.txt"
grep -q "uart pair LCOM" "$OUT_DIR/uart_pair.txt"

"$BIN_DIR/lcom" run --headless --audio-wav "$OUT_DIR/tone.wav" -- "$EX_DIR/audio_tone" > "$OUT_DIR/audio.txt"
grep -q "audio tone" "$OUT_DIR/audio.txt"
test -s "$OUT_DIR/tone.wav"
head -c 4 "$OUT_DIR/tone.wav" | grep -q RIFF

"$BIN_DIR/lcom" run --headless --script scripts/mouse_move.lcomscript -- "$EX_DIR/mouse_packet" > "$OUT_DIR/mouse.txt"
grep -q "mouse packet" "$OUT_DIR/mouse.txt"
grep -q "dx=5 dy=-2 lb=1" "$OUT_DIR/mouse.txt"

"$BIN_DIR/lcom" run --headless --dump-frame "$OUT_DIR/rectangle.ppm" -- "$EX_DIR/vbe_rectangle" > "$OUT_DIR/vbe.txt"
grep -q "rectangle drawn" "$OUT_DIR/vbe.txt"
test -s "$OUT_DIR/rectangle.ppm"

"$BIN_DIR/lcom" run --headless --script scripts/demo_exit.lcomscript --dump-frame "$OUT_DIR/sdl_demo.ppm" -- "$EX_DIR/sdl_demo" > "$OUT_DIR/sdl_demo.txt"
grep -q "SDL demo" "$OUT_DIR/sdl_demo.txt"
grep -q "kbd byte 0x81" "$OUT_DIR/sdl_demo.txt"
test -s "$OUT_DIR/sdl_demo.ppm"

"$BIN_DIR/lcom" run --headless --script scripts/flappy_demo.lcomscript --audio-wav "$OUT_DIR/flappy.wav" --dump-frame "$OUT_DIR/flappy.ppm" -- "$EX_DIR/flappy_bird" > "$OUT_DIR/flappy.txt"
grep -q "LCOM Bird" "$OUT_DIR/flappy.txt"
test -s "$OUT_DIR/flappy.ppm"
test -s "$OUT_DIR/flappy.wav"

"$BIN_DIR/lcom" run --headless --script scripts/flappy_mouse_demo.lcomscript --audio-wav "$OUT_DIR/flappy_mouse.wav" --dump-frame "$OUT_DIR/flappy_mouse.ppm" -- "$EX_DIR/flappy_bird" > "$OUT_DIR/flappy_mouse.txt"
grep -q "mouse click flaps" "$OUT_DIR/flappy_mouse.txt"
test -s "$OUT_DIR/flappy_mouse.ppm"
test -s "$OUT_DIR/flappy_mouse.wav"

"$BIN_DIR/lcom" run --headless --script scripts/flappy_mouse_stress.lcomscript --dump-frame "$OUT_DIR/flappy_mouse_stress.ppm" -- "$EX_DIR/flappy_bird" > "$OUT_DIR/flappy_mouse_stress.txt"
grep -q "Program exited with status 0" "$OUT_DIR/flappy_mouse_stress.txt"
test -s "$OUT_DIR/flappy_mouse_stress.ppm"

rm -rf "$OUT_DIR/flappy_space_frames"
"$BIN_DIR/lcom" run --headless --script scripts/flappy_space_stress.lcomscript --audio null --frame-dir "$OUT_DIR/flappy_space_frames" -- "$EX_DIR/flappy_bird" > "$OUT_DIR/flappy_space_stress.txt"
grep -q "Program exited with status 0" "$OUT_DIR/flappy_space_stress.txt"
test "$(find "$OUT_DIR/flappy_space_frames" -name 'frame_*.ppm' | wc -l | tr -d ' ')" -ge 45

"$BIN_DIR/lcom" run --headless --script scripts/ninjix_menu_exit.lcomscript --dump-frame "$OUT_DIR/ninjix_menu.ppm" -- "$EX_DIR/ninjix" > "$OUT_DIR/ninjix_menu.txt"
grep -q "Program exited with status 0" "$OUT_DIR/ninjix_menu.txt"
! grep -q "serial .* failed" "$OUT_DIR/ninjix_menu.txt"
test -s "$OUT_DIR/ninjix_menu.ppm"

"$BIN_DIR/lcom" run --headless --script scripts/pvz_menu_exit.lcomscript --dump-frame "$OUT_DIR/pvz_menu.ppm" --max-ticks 240 -- "$EX_DIR/pvz" > "$OUT_DIR/pvz_menu.txt"
grep -q "Program exited with status 0" "$OUT_DIR/pvz_menu.txt"
test -s "$OUT_DIR/pvz_menu.ppm"

"$BIN_DIR/lcom" replay scripts/flappy_mouse_demo.lcomscript --headless --video "$OUT_DIR/flappy_replay.mp4" --video-fps 30 -- "$EX_DIR/flappy_bird" > "$OUT_DIR/flappy_replay.txt"
grep -q "Rendered video" "$OUT_DIR/flappy_replay.txt"
test -s "$OUT_DIR/flappy_replay.mp4"

rm -rf "$OUT_DIR/flappy-headless.lcom"
"$BIN_DIR/lcom" bundle . --program "$EX_DIR/flappy_bird" --name flappy-headless --output "$OUT_DIR/flappy-headless.lcom" --script scripts/flappy_mouse_demo.lcomscript --headless > "$OUT_DIR/bundle.txt"
test -x "$OUT_DIR/flappy-headless.lcom"
"$OUT_DIR/flappy-headless.lcom" > "$OUT_DIR/bundle-run.txt"
grep -q "Program exited with status 0" "$OUT_DIR/bundle-run.txt"

rm -rf "$OUT_DIR/flappy-headless-dir.lcom"
"$BIN_DIR/lcom" bundle . --format dir --program "$EX_DIR/flappy_bird" --name flappy-headless-dir --output "$OUT_DIR/flappy-headless-dir.lcom" --script scripts/flappy_mouse_demo.lcomscript --headless > "$OUT_DIR/bundle-dir.txt"
test -x "$OUT_DIR/flappy-headless-dir.lcom/run.sh"
test -f "$OUT_DIR/flappy-headless-dir.lcom/sdk/include/lcom/lcom.h"

echo "integration tests passed"
