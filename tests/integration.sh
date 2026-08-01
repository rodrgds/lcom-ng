#!/bin/sh
set -eu

BIN_DIR="${1:-build}"
EX_DIR="${2:-build/examples}"
OUT_DIR="${3:-build/test-output}"
CLI="$BIN_DIR/machinelab"
COMPAT_CLI="$BIN_DIR/lcom"
LOWLAB_CLI="$BIN_DIR/lowlab"

mkdir -p "$OUT_DIR"

ppm_rgb_at() {
  ppm_path="$1"
  ppm_x="$2"
  ppm_y="$3"
  ppm_offset=$((15 + (ppm_y * 800 + ppm_x) * 3))
  dd if="$ppm_path" bs=1 skip="$ppm_offset" count=3 2>/dev/null |
    od -An -tx1 |
    tr -d ' \n'
}

test -x "$CLI"
test -x "$COMPAT_CLI"
test -x "$LOWLAB_CLI"
grep -q "# Machine Lab" README.md
grep -q "## Choose Your Lane" README.md
grep -q "Machine Lab developers" README.md
grep -q "MIT" README.md
grep -q "CC BY 4.0" README.md
test -f LICENSE
grep -q "MIT License" LICENSE
test -f course/LICENSE.md
grep -q "CC BY 4.0" course/LICENSE.md
test -f CONTRIBUTING.md
grep -q "Audience Boundaries" CONTRIBUTING.md
test -f docs/index.md
grep -q "Student-facing" docs/index.md
test -f docs/adoption.md
grep -qi "one week" docs/adoption.md
test -f docs-site/index.md
test -f docs-site/.vitepress/config.mts
test -f docs-site/labs/lab1/tasks.md
test -f docs-site/labs/lab7/check.md
grep -q "Machine Lab" docs-site/index.md
grep -q "pages_build_output_dir" wrangler.toml
grep -q "deploy_pages" .github/workflows/docs.yml
test -f docs/student-guide.md
test -f docs/instructor-guide.md
test -f docs/developer-guide.md
grep -q "Usually Ignore These" docs/student-guide.md
grep -q "Audience Boundaries" docs/developer-guide.md
grep -q "Syllabus Map" docs/instructor-guide.md
grep -q "Project Rubric" docs/instructor-guide.md
test -d sdk/include/lcom
test -d sdk/client
test -d course/labs/docs
test -d course/labs/templates
test -d dev/lab-solutions
test -d dev/lab-tests
test ! -e labs
grep -q "# Machine Lab Course" course/README.md
grep -q "Audience Split" course/README.md
grep -q "Final Project Studio" course/README.md
grep -q "guided gaps" course/labs/docs/lab1-bitwise-rtc.md
test -s docs/assets/screenshots/word-processor.png
test -s docs/assets/screenshots/music-maker.png
test -s docs/assets/screenshots/breakout.png
test -s docs/assets/screenshots/flappy.png
for showcase_image in \
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
  if ! test -s "docs/assets/screenshots/$showcase_image.png"; then
    echo "missing canonical showcase frame: $showcase_image.png" >&2
    echo "run: devenv shell -- sh scripts/capture_showcase_examples.sh" >&2
    exit 1
  fi
  showcase_bytes="$(wc -c < "docs/assets/screenshots/$showcase_image.png" | tr -d ' ')"
  if [ "$showcase_bytes" -le 4096 ]; then
    echo "canonical showcase frame is suspiciously small: $showcase_image.png" >&2
    exit 1
  fi
done
if cmp -s docs/assets/screenshots/serial-whiteboard-left.png \
    docs/assets/screenshots/serial-whiteboard-right.png; then
  echo "canonical whiteboard peer frames must be distinct" >&2
  exit 1
fi
if cmp -s docs/assets/screenshots/serial-transfer-sender.png \
    docs/assets/screenshots/serial-transfer-receiver.png; then
  echo "canonical transfer peer frames must be distinct" >&2
  exit 1
fi
test -s docs/assets/demo/machine-lab-demo.mp4
for lab_doc in course/labs/docs/lab*.md; do
  grep -q "\\[!IMPORTANT\\]" "$lab_doc"
  grep -q "\\[!TIP\\]" "$lab_doc"
  grep -q "External Reading" "$lab_doc"
done
"$CLI" --help > "$OUT_DIR/top-help.txt" 2>&1 || true
grep -q "Machine Lab" "$OUT_DIR/top-help.txt"
"$COMPAT_CLI" --help > "$OUT_DIR/lcom-help.txt" 2>&1 || true
grep -q "Machine Lab" "$OUT_DIR/lcom-help.txt"
"$LOWLAB_CLI" --help > "$OUT_DIR/lowlab-help.txt" 2>&1 || true
grep -q "Machine Lab" "$OUT_DIR/lowlab-help.txt"

test -f ".github/workflows/release.yml"
grep -q "ubuntu-24.04" ".github/workflows/release.yml"
grep -q "macos-14" ".github/workflows/release.yml"
grep -q "windows-2025" ".github/workflows/release.yml"
grep -q "machinelab-" ".github/workflows/release.yml"
grep -q "softprops/action-gh-release" ".github/workflows/release.yml"

"$CLI" lab list > "$OUT_DIR/labs.txt"
grep -q "lab7" "$OUT_DIR/labs.txt"
grep -q "lab1 (rtc)" "$OUT_DIR/labs.txt"
"$CLI" lab show lab5 > "$OUT_DIR/lab5.txt"
grep -q "video_set_mode" "$OUT_DIR/lab5.txt"
"$CLI" lab show uart > "$OUT_DIR/lab7.txt"
grep -q "lab7 (uart)" "$OUT_DIR/lab7.txt"
grep -q "uart_config" "$OUT_DIR/lab7.txt"
"$CLI" docs cli > "$OUT_DIR/cli-docs.md"
grep -q "# machinelab CLI" "$OUT_DIR/cli-docs.md"
grep -q "machinelab bundle" "$OUT_DIR/cli-docs.md"
"$CLI" completion bash > "$OUT_DIR/machinelab.bash"
grep -q "_machinelab_complete" "$OUT_DIR/machinelab.bash"
grep -q -- "--left-dump-frame" "$OUT_DIR/machinelab.bash"
grep -q -- "--right-dump-frame" "$OUT_DIR/machinelab.bash"
"$CLI" completion zsh > "$OUT_DIR/_machinelab"
grep -q -- "--left-dump-frame" "$OUT_DIR/_machinelab"
grep -q -- "--right-dump-frame" "$OUT_DIR/_machinelab"
"$CLI" completion fish > "$OUT_DIR/machinelab.fish"
grep -q -- "left-dump-frame" "$OUT_DIR/machinelab.fish"
grep -q -- "right-dump-frame" "$OUT_DIR/machinelab.fish"

for command in run run-pair replay setup test bundle lab docs completion; do
  "$CLI" "$command" --help > "$OUT_DIR/help-$command.txt" 2>&1
  grep -Fq "$command [OPTIONS]" "$OUT_DIR/help-$command.txt"
  ! grep -q "machinelab: .*requires\|machinelab: unknown" "$OUT_DIR/help-$command.txt"
done
grep -q -- "--left-dump-frame" "$OUT_DIR/help-run-pair.txt"
grep -q -- "--right-dump-frame" "$OUT_DIR/help-run-pair.txt"

if "$CLI" not-a-command --help > "$OUT_DIR/help-unknown.txt" 2>&1; then
  echo "unknown commands should fail even when followed by --help" >&2
  exit 1
fi
grep -q "unknown command not-a-command" "$OUT_DIR/help-unknown.txt"

if "$CLI" run > "$OUT_DIR/run-missing.txt" 2>&1; then
  echo "machinelab run without a program should fail" >&2
  exit 1
fi
grep -q "run requires <program>" "$OUT_DIR/run-missing.txt"
grep -Fq "run [OPTIONS]" "$OUT_DIR/run-missing.txt"

if "$CLI" bundle > "$OUT_DIR/bundle-missing.txt" 2>&1; then
  echo "machinelab bundle without a project dir should fail" >&2
  exit 1
fi
grep -q "bundle requires <project-dir>" "$OUT_DIR/bundle-missing.txt"
grep -Fq "bundle [OPTIONS] project-dir" "$OUT_DIR/bundle-missing.txt"

if "$CLI" completion > "$OUT_DIR/completion-missing.txt" 2>&1; then
  echo "machinelab completion without a shell should fail" >&2
  exit 1
fi
grep -q "completion requires <shell>" "$OUT_DIR/completion-missing.txt"
grep -Fq "completion [OPTIONS] shell" "$OUT_DIR/completion-missing.txt"

if "$CLI" replay > "$OUT_DIR/replay-missing.txt" 2>&1; then
  echo "machinelab replay without a script/program should fail" >&2
  exit 1
fi
grep -q "replay requires <script>" "$OUT_DIR/replay-missing.txt"
grep -Fq "replay [OPTIONS] script" "$OUT_DIR/replay-missing.txt"
grep -Fq -- "--max-ticks" "$OUT_DIR/replay-missing.txt"

if "$CLI" run-pair > "$OUT_DIR/run-pair-missing.txt" 2>&1; then
  echo "machinelab run-pair without programs should fail" >&2
  exit 1
fi
grep -q "run-pair requires" "$OUT_DIR/run-pair-missing.txt"

"$CLI" run --headless -- "$EX_DIR/hello" > "$OUT_DIR/hello.txt"
grep -q "hello from Machine Lab" "$OUT_DIR/hello.txt"

"$CLI" run --headless -- "$EX_DIR/timer_int" 3 > "$OUT_DIR/timer.txt"
grep -q "timer tick 3" "$OUT_DIR/timer.txt"

"$CLI" run --headless --trace "$OUT_DIR/timer.trace.jsonl" -- "$EX_DIR/timer_int" 1 > "$OUT_DIR/timer_trace.txt"
test -s "$OUT_DIR/timer.trace.jsonl"
grep -q '"event":"irq"' "$OUT_DIR/timer.trace.jsonl"

"$CLI" run --headless --script scripts/type_a_esc.mlabscript -- "$EX_DIR/keyboard_scan" > "$OUT_DIR/kbd.txt"
grep -q "kbd make 0x1e" "$OUT_DIR/kbd.txt"
grep -q "kbd break 0x81" "$OUT_DIR/kbd.txt"

"$CLI" replay scripts/type_a_esc.mlabscript --headless -- "$EX_DIR/keyboard_scan" > "$OUT_DIR/replay.txt"
grep -q "kbd break 0x81" "$OUT_DIR/replay.txt"

"$CLI" run --headless --rtc 2026-06-16T12:34:56 -- "$EX_DIR/rtc_date" > "$OUT_DIR/rtc.txt"
grep -q "rtc 26-06-16 12:34:56" "$OUT_DIR/rtc.txt"

"$CLI" run --headless -- "$EX_DIR/uart_loopback" > "$OUT_DIR/uart.txt"
grep -q "uart rx L" "$OUT_DIR/uart.txt"

"$CLI" run --headless -- "$EX_DIR/uart_pair" > "$OUT_DIR/uart_pair.txt"
grep -q "uart pair LCOM" "$OUT_DIR/uart_pair.txt"

"$CLI" run-pair --headless "$EX_DIR/uart_peer_sender" --right "$EX_DIR/uart_peer_receiver" --max-ticks 5000 > "$OUT_DIR/run_pair_uart.txt" 2>&1
grep -q "peer receiver got PAIR" "$OUT_DIR/run_pair_uart.txt"
grep -q "\\[left\\] Program exited with status 0" "$OUT_DIR/run_pair_uart.txt"
grep -q "\\[right\\] Program exited with status 0" "$OUT_DIR/run_pair_uart.txt"
! grep -q "max virtual ticks reached" "$OUT_DIR/run_pair_uart.txt"

if "$CLI" run-pair --headless /usr/bin/false --right /usr/bin/false \
    > "$OUT_DIR/run_pair_failure.txt" 2>&1; then
  echo "run-pair must propagate guest failures" >&2
  exit 1
fi
grep -q "\\[left\\] Program exited with status 1" "$OUT_DIR/run_pair_failure.txt"
grep -q "\\[right\\] Program exited with status 1" "$OUT_DIR/run_pair_failure.txt"

if "$CLI" run-pair --headless \
    --left-script scripts/demo_exit.mlabscript \
    --right-script scripts/demo_exit.mlabscript \
    --left-dump-frame "$OUT_DIR" \
    "$EX_DIR/sdl_demo" --right "$EX_DIR/sdl_demo" \
    > "$OUT_DIR/run_pair_frame_failure.txt" 2>&1; then
  echo "run-pair frame output failures must fail the run" >&2
  exit 1
fi
grep -q "could not write left frame dump" "$OUT_DIR/run_pair_frame_failure.txt"

if "$CLI" run-pair --headless --max-ticks 8 \
    --left-dump-frame "$OUT_DIR/run_pair_timeout_left.ppm" \
    --right-dump-frame "$OUT_DIR/run_pair_timeout_right.ppm" \
    "$BIN_DIR/tests/framebuffer_dirty_client" \
    --right "$BIN_DIR/tests/framebuffer_dirty_client" \
    > "$OUT_DIR/run_pair_timeout_frames.txt" 2>&1; then
  echo "run-pair max-tick termination should fail the run" >&2
  exit 1
fi
grep -q "max virtual ticks reached (8)" "$OUT_DIR/run_pair_timeout_frames.txt"
test -s "$OUT_DIR/run_pair_timeout_left.ppm"
test -s "$OUT_DIR/run_pair_timeout_right.ppm"
test "$(ppm_rgb_at "$OUT_DIR/run_pair_timeout_left.ppm" 0 0)" = "e14f9a"
test "$(ppm_rgb_at "$OUT_DIR/run_pair_timeout_right.ppm" 0 0)" = "e14f9a"

"$CLI" run-pair --headless "$EX_DIR/ninjix" --right "$EX_DIR/ninjix" --max-ticks 900 > "$OUT_DIR/run_pair_ninjix.txt"
grep -q "ninjix pair started as ATTACK" "$OUT_DIR/run_pair_ninjix.txt"
grep -q "ninjix pair started as DEFEND" "$OUT_DIR/run_pair_ninjix.txt"
grep -q "ninjix pair smoke complete" "$OUT_DIR/run_pair_ninjix.txt"
! grep -q "max virtual ticks reached" "$OUT_DIR/run_pair_ninjix.txt"

rm -rf "$OUT_DIR/student_workspace"
"$CLI" setup "$OUT_DIR/student_workspace" > "$OUT_DIR/setup.txt"
test -f "$OUT_DIR/student_workspace/include/lcom/lcom.h"
test -f "$OUT_DIR/student_workspace/Makefile"
test -f "$OUT_DIR/student_workspace/proj/main.c"
test -f "$OUT_DIR/student_workspace/labs/rtc/bitwise.c"
test -f "$OUT_DIR/student_workspace/labs/uart/include/uart_lib.h"
test -f "$OUT_DIR/student_workspace/lib/kbc/kbc.h"
grep -q "## Edit These" "$OUT_DIR/student_workspace/README.md"
grep -q "## Usually Ignore These" "$OUT_DIR/student_workspace/README.md"
grep -q "Machine Lab developer internals" "$OUT_DIR/student_workspace/README.md"
make -C "$OUT_DIR/student_workspace" labs > "$OUT_DIR/student_make_labs.txt"
if "$CLI" test rtc --project "$OUT_DIR/student_workspace" > "$OUT_DIR/student_rtc.txt" 2>&1; then
  echo "fresh student rtc TODO stubs should fail predefined tests" >&2
  exit 1
fi
grep -q "Running rtc predefined tests" "$OUT_DIR/student_rtc.txt"
grep -q "check failed" "$OUT_DIR/student_rtc.txt"

"$CLI" run --headless --audio-wav "$OUT_DIR/tone.wav" -- "$EX_DIR/audio_tone" > "$OUT_DIR/audio.txt"
grep -q "audio tone" "$OUT_DIR/audio.txt"
test -s "$OUT_DIR/tone.wav"
head -c 4 "$OUT_DIR/tone.wav" | grep -q RIFF

if "$CLI" run --headless --audio-wav "$OUT_DIR" -- "$EX_DIR/audio_tone" \
    > "$OUT_DIR/audio_write_failure.txt" 2>&1; then
  echo "audio output failures must fail the run" >&2
  exit 1
fi

if "$CLI" run --headless --dump-frame "$OUT_DIR" -- "$EX_DIR/vbe_rectangle" \
    > "$OUT_DIR/frame_write_failure.txt" 2>&1; then
  echo "frame output failures must fail the run" >&2
  exit 1
fi

"$CLI" run --headless --script scripts/write_note.mlabscript --dump-frame "$OUT_DIR/word_processor.ppm" -- "$EX_DIR/word_processor" > "$OUT_DIR/word_processor.txt"
grep -q "word processor text Machine Lab notes" "$OUT_DIR/word_processor.txt"
grep -q "Portable C, visible results." "$OUT_DIR/word_processor.txt"
grep -q "Tests, traces, screenshots, sound." "$OUT_DIR/word_processor.txt"
test -s "$OUT_DIR/word_processor.ppm"

"$CLI" run --headless --script scripts/music_maker_demo.mlabscript --audio-wav "$OUT_DIR/music_maker.wav" --dump-frame "$OUT_DIR/music_maker.ppm" -- "$EX_DIR/music_maker" > "$OUT_DIR/music_maker.txt"
grep -q "music maker pattern tracks=4 steps=16 active=" "$OUT_DIR/music_maker.txt"
grep -Eq "submitted=([2-9]|[1-9][0-9]+)" "$OUT_DIR/music_maker.txt"
test -s "$OUT_DIR/music_maker.wav"
head -c 4 "$OUT_DIR/music_maker.wav" | grep -q RIFF
music_duration=$(ffprobe -v error -show_entries format=duration -of default=noprint_wrappers=1:nokey=1 "$OUT_DIR/music_maker.wav")
awk -v duration="$music_duration" 'BEGIN { exit !(duration > 1.5) }'
test -s "$OUT_DIR/music_maker.ppm"

"$CLI" run --headless --script scripts/breakout_demo.mlabscript --dump-frame "$OUT_DIR/breakout_demo.ppm" -- "$EX_DIR/breakout_demo" > "$OUT_DIR/breakout_demo.txt"
grep -q "breakout demo score=" "$OUT_DIR/breakout_demo.txt"
test -s "$OUT_DIR/breakout_demo.ppm"

"$CLI" run --headless --rtc 2026-08-12T14:35:42 \
    --script scripts/pixel_studio_demo.mlabscript \
    --dump-frame "$OUT_DIR/pixel_studio.ppm" --max-ticks 900 -- \
    "$EX_DIR/pixel_studio" "$OUT_DIR/pixel_studio_sprite.h" \
    > "$OUT_DIR/pixel_studio.txt"
grep -q "pixel studio summary pixels=256" "$OUT_DIR/pixel_studio.txt"
grep -q "exports=1" "$OUT_DIR/pixel_studio.txt"
grep -q "rtc=14:35:42" "$OUT_DIR/pixel_studio.txt"
grep -q "PIXEL_STUDIO_SPRITE_WIDTH 16u" "$OUT_DIR/pixel_studio_sprite.h"
grep -q "PIXEL_STUDIO_SPRITE_HEIGHT 16u" "$OUT_DIR/pixel_studio_sprite.h"
grep -q "pixel_studio_sprite\\[256\\]" "$OUT_DIR/pixel_studio_sprite.h"
grep -q "Pixel Studio export: 2026-08-12 14:35:42" \
    "$OUT_DIR/pixel_studio_sprite.h"
test "$(grep -Eo '0x[0-9A-F]{8}u' "$OUT_DIR/pixel_studio_sprite.h" | wc -l | tr -d ' ')" -eq 256
"${CC:-cc}" -std=c11 -pedantic-errors -fsyntax-only \
    -include "$OUT_DIR/pixel_studio_sprite.h" -x c /dev/null
test -s "$OUT_DIR/pixel_studio.ppm"

"$CLI" run --headless --rtc 2026-08-12T15:10:05 \
    --script scripts/tilemap_editor_demo.mlabscript \
    --dump-frame "$OUT_DIR/tilemap_editor.ppm" --max-ticks 700 -- \
    "$EX_DIR/tilemap_editor" "$OUT_DIR/tilemap_demo.mlmap" \
    > "$OUT_DIR/tilemap_editor.txt"
grep -q "tilemap editor summary mode=play valid=1" "$OUT_DIR/tilemap_editor.txt"
grep -q "solved=1" "$OUT_DIR/tilemap_editor.txt"
grep -q "exports=1" "$OUT_DIR/tilemap_editor.txt"
grep -q "^# Machine Lab tilemap v1$" "$OUT_DIR/tilemap_demo.mlmap"
grep -q "^# saved 2026-08-12T15:10:05$" "$OUT_DIR/tilemap_demo.mlmap"
grep -q "^# validation valid$" "$OUT_DIR/tilemap_demo.mlmap"
grep -q "^size 10 8$" "$OUT_DIR/tilemap_demo.mlmap"
awk '
  /^size 10 8$/ { in_map = 1; next }
  /^end$/ { in_map = 0; found_end = 1; next }
  in_map {
    if (length($0) != 10 || $0 ~ /[^.#GCP+*]/) exit 1
    rows++
  }
  END { if (rows != 8 || !found_end) exit 1 }
' "$OUT_DIR/tilemap_demo.mlmap"
test -s "$OUT_DIR/tilemap_editor.ppm"

"$CLI" run --headless --rtc 2026-07-30T09:40:00 \
    --script scripts/pomodoro_clock_demo.mlabscript \
    --audio-wav "$OUT_DIR/pomodoro_clock.wav" \
    --dump-frame "$OUT_DIR/pomodoro_clock.ppm" --max-ticks 500 -- \
    "$EX_DIR/pomodoro_clock" > "$OUT_DIR/pomodoro_clock.txt"
grep -q "pomodoro clock summary" "$OUT_DIR/pomodoro_clock.txt"
grep -Eq "completions=[1-9][0-9]*" "$OUT_DIR/pomodoro_clock.txt"
grep -Eq "rtc_jumps=[1-9][0-9]*" "$OUT_DIR/pomodoro_clock.txt"
grep -Eq "audio_signals=[1-9][0-9]*" "$OUT_DIR/pomodoro_clock.txt"
head -c 4 "$OUT_DIR/pomodoro_clock.wav" | grep -q RIFF
test -s "$OUT_DIR/pomodoro_clock.ppm"

"$CLI" run --headless --rtc 2026-07-30T09:40:00 \
    --script scripts/interrupt_scope_demo.mlabscript \
    --dump-frame "$OUT_DIR/interrupt_scope.ppm" --max-ticks 360 -- \
    "$EX_DIR/interrupt_scope" > "$OUT_DIR/interrupt_scope.txt"
grep -q "interrupt scope summary" "$OUT_DIR/interrupt_scope.txt"
grep -Eq "keyboard_irq=[1-9][0-9]*" "$OUT_DIR/interrupt_scope.txt"
grep -Eq "mouse_packets=[1-9][0-9]*" "$OUT_DIR/interrupt_scope.txt"
grep -Eq "uart_tx=[1-9][0-9]* uart_rx=[1-9][0-9]*" "$OUT_DIR/interrupt_scope.txt"
grep -Eq "rtc_polls=[1-9][0-9]*" "$OUT_DIR/interrupt_scope.txt"
grep -q "frozen=0 loopback=1" "$OUT_DIR/interrupt_scope.txt"
test -s "$OUT_DIR/interrupt_scope.ppm"

"$CLI" run --headless --rtc 2026-07-30T08:15:00 \
    --script scripts/ambient_aquarium_demo.mlabscript \
    --audio-wav "$OUT_DIR/ambient_aquarium.wav" \
    --dump-frame "$OUT_DIR/ambient_aquarium.ppm" --max-ticks 520 -- \
    "$EX_DIR/ambient_aquarium" > "$OUT_DIR/ambient_aquarium.txt"
grep -q "aquarium summary" "$OUT_DIR/ambient_aquarium.txt"
grep -q "phase=day lighting=manual" "$OUT_DIR/ambient_aquarium.txt"
grep -Eq "motion_ticks=[1-9][0-9]*" "$OUT_DIR/ambient_aquarium.txt"
grep -q "feeds=2" "$OUT_DIR/ambient_aquarium.txt"
grep -Eq "rtc_reads=[2-9][0-9]*" "$OUT_DIR/ambient_aquarium.txt"
grep -q "lighting_changes=3 audio_signals=3" "$OUT_DIR/ambient_aquarium.txt"
grep -q "paused=0" "$OUT_DIR/ambient_aquarium.txt"
head -c 4 "$OUT_DIR/ambient_aquarium.wav" | grep -q RIFF
test -s "$OUT_DIR/ambient_aquarium.ppm"

"$CLI" run-pair --headless \
    --left-script scripts/serial_whiteboard_left.mlabscript \
    --right-script scripts/serial_whiteboard_right.mlabscript \
    --left-dump-frame "$OUT_DIR/serial_whiteboard_left.ppm" \
    --right-dump-frame "$OUT_DIR/serial_whiteboard_right.ppm" \
    --max-ticks 1000 \
    "$EX_DIR/serial_whiteboard" left \
    --right "$EX_DIR/serial_whiteboard" right \
    > "$OUT_DIR/serial_whiteboard.txt" 2>&1
grep -q "serial whiteboard role=left connected=yes" "$OUT_DIR/serial_whiteboard.txt"
grep -q "serial whiteboard role=right connected=yes" "$OUT_DIR/serial_whiteboard.txt"
test "$(grep -c "local_segments=120 remote_segments=120" "$OUT_DIR/serial_whiteboard.txt")" -eq 2
grep -q "role=left connected=yes local_segments=120 remote_segments=120 local_hash=0A remote_hash=DE" \
    "$OUT_DIR/serial_whiteboard.txt"
grep -q "role=right connected=yes local_segments=120 remote_segments=120 local_hash=DE remote_hash=0A" \
    "$OUT_DIR/serial_whiteboard.txt"
grep -Eq "reconnects=[1-9][0-9]*" "$OUT_DIR/serial_whiteboard.txt"
grep -Eq "peer_resets=[1-9][0-9]*" "$OUT_DIR/serial_whiteboard.txt"
grep -q "\\[left\\] Program exited with status 0" "$OUT_DIR/serial_whiteboard.txt"
grep -q "\\[right\\] Program exited with status 0" "$OUT_DIR/serial_whiteboard.txt"
! grep -q "max virtual ticks reached" "$OUT_DIR/serial_whiteboard.txt"
test -s "$OUT_DIR/serial_whiteboard_left.ppm"
test -s "$OUT_DIR/serial_whiteboard_right.ppm"
test "$(ppm_rgb_at "$OUT_DIR/serial_whiteboard_left.ppm" 450 232)" = "2d7ff9"
test "$(ppm_rgb_at "$OUT_DIR/serial_whiteboard_right.ppm" 450 232)" = "2d7ff9"
if cmp -s "$OUT_DIR/serial_whiteboard_left.ppm" \
    "$OUT_DIR/serial_whiteboard_right.ppm"; then
  echo "whiteboard peer frame dumps should be visually distinct" >&2
  exit 1
fi

"$CLI" run-pair --headless \
    --left-script scripts/serial_whiteboard_left.mlabscript \
    --right-script scripts/serial_whiteboard_right.mlabscript \
    --max-ticks 1000 \
    "$EX_DIR/serial_whiteboard" left restart \
    --right "$EX_DIR/serial_whiteboard" right \
    > "$OUT_DIR/serial_whiteboard_restart.txt" 2>&1
grep -q "role=left connected=yes local_segments=120 remote_segments=120 local_hash=0A remote_hash=DE" \
    "$OUT_DIR/serial_whiteboard_restart.txt"
grep -q "role=right connected=yes local_segments=120 remote_segments=120 local_hash=DE remote_hash=0A" \
    "$OUT_DIR/serial_whiteboard_restart.txt"
grep -q "simulated_restart=used event_error=no" \
    "$OUT_DIR/serial_whiteboard_restart.txt"
grep -Eq "peer_resets=[1-9][0-9]*" "$OUT_DIR/serial_whiteboard_restart.txt"
! grep -q "max virtual ticks reached" "$OUT_DIR/serial_whiteboard_restart.txt"

"$CLI" run-pair --headless --max-ticks 1200 \
    "$EX_DIR/serial_transfer" send \
    --right "$EX_DIR/serial_transfer" receive \
    > "$OUT_DIR/serial_transfer.txt" 2>&1
test "$(grep -c "complete bytes=256 crc32=FBFBFFB6 blocks=32 retries=0" \
    "$OUT_DIR/serial_transfer.txt")" -eq 2
! grep -q "max virtual ticks reached" "$OUT_DIR/serial_transfer.txt"

"$CLI" run-pair --headless \
    --left-dump-frame "$OUT_DIR/serial_transfer_sender.ppm" \
    --right-dump-frame "$OUT_DIR/serial_transfer_receiver.ppm" \
    --max-ticks 1400 \
    "$EX_DIR/serial_transfer" send fault \
    --right "$EX_DIR/serial_transfer" receive \
    > "$OUT_DIR/serial_transfer_fault.txt" 2>&1
grep -q "serial transfer simulated drop offset=56" "$OUT_DIR/serial_transfer_fault.txt"
grep -q "role=sender complete bytes=256 crc32=FBFBFFB6 blocks=32 retries=1" \
    "$OUT_DIR/serial_transfer_fault.txt"
grep -q "role=receiver complete bytes=256 crc32=FBFBFFB6 blocks=32 retries=0" \
    "$OUT_DIR/serial_transfer_fault.txt"
grep -q "fault=drop:recovered" "$OUT_DIR/serial_transfer_fault.txt"
grep -q "\\[left\\] Program exited with status 0" "$OUT_DIR/serial_transfer_fault.txt"
grep -q "\\[right\\] Program exited with status 0" "$OUT_DIR/serial_transfer_fault.txt"
! grep -q "max virtual ticks reached" "$OUT_DIR/serial_transfer_fault.txt"
test -s "$OUT_DIR/serial_transfer_sender.ppm"
test -s "$OUT_DIR/serial_transfer_receiver.ppm"
test "$(ppm_rgb_at "$OUT_DIR/serial_transfer_sender.ppm" 112 240)" = "ef6461"
test "$(ppm_rgb_at "$OUT_DIR/serial_transfer_receiver.ppm" 112 240)" = "ef6461"
if cmp -s "$OUT_DIR/serial_transfer_sender.ppm" \
    "$OUT_DIR/serial_transfer_receiver.ppm"; then
  echo "transfer peer frame dumps should be visually distinct" >&2
  exit 1
fi

"$CLI" run-pair --headless --max-ticks 1400 \
    "$EX_DIR/serial_transfer" send \
    --right "$EX_DIR/serial_transfer" receive dropack \
    > "$OUT_DIR/serial_transfer_dropack.txt" 2>&1
grep -Eq "role=sender complete bytes=256 crc32=FBFBFFB6 blocks=32 retries=[1-9][0-9]*" \
    "$OUT_DIR/serial_transfer_dropack.txt"
grep -q "role=receiver complete bytes=256 crc32=FBFBFFB6 blocks=32 retries=0" \
    "$OUT_DIR/serial_transfer_dropack.txt"
grep -q "fault=done-ack:recovered" "$OUT_DIR/serial_transfer_dropack.txt"
grep -Eq "duplicates=[1-9][0-9]*" "$OUT_DIR/serial_transfer_dropack.txt"
! grep -q "max virtual ticks reached" "$OUT_DIR/serial_transfer_dropack.txt"

if "$CLI" run-pair --headless --max-ticks 1400 \
    "$EX_DIR/serial_transfer" send \
    --right "$EX_DIR/serial_transfer" receive corrupt \
    > "$OUT_DIR/serial_transfer_corrupt.txt" 2>&1; then
  echo "whole-file CRC corruption should fail both transfer peers" >&2
  exit 1
fi
test "$(grep -c "reason=1 coordinated=yes" \
    "$OUT_DIR/serial_transfer_corrupt.txt")" -eq 2
grep -q "fault=payload:rejected" "$OUT_DIR/serial_transfer_corrupt.txt"
grep -q "\\[left\\] Program exited with status 1" "$OUT_DIR/serial_transfer_corrupt.txt"
grep -q "\\[right\\] Program exited with status 1" "$OUT_DIR/serial_transfer_corrupt.txt"
! grep -q "max virtual ticks reached" "$OUT_DIR/serial_transfer_corrupt.txt"

if "$CLI" run-pair --headless --max-ticks 1400 \
    "$EX_DIR/serial_transfer" send badcrc \
    --right "$EX_DIR/serial_transfer" receive \
    > "$OUT_DIR/serial_transfer_badcrc.txt" 2>&1; then
  echo "an invalid manifest CRC should fail both transfer peers" >&2
  exit 1
fi
test "$(grep -c "reason=1 coordinated=yes" \
    "$OUT_DIR/serial_transfer_badcrc.txt")" -eq 2
grep -q "fault=badcrc:rejected" "$OUT_DIR/serial_transfer_badcrc.txt"
grep -q "\\[left\\] Program exited with status 1" "$OUT_DIR/serial_transfer_badcrc.txt"
grep -q "\\[right\\] Program exited with status 1" "$OUT_DIR/serial_transfer_badcrc.txt"
! grep -q "max virtual ticks reached" "$OUT_DIR/serial_transfer_badcrc.txt"

if "$CLI" run-pair --headless --max-ticks 1400 \
    "$EX_DIR/serial_transfer" send stall \
    --right "$EX_DIR/serial_transfer" receive \
    > "$OUT_DIR/serial_transfer_stall.txt" 2>&1; then
  echo "a stalled transfer should end in a coordinated peer timeout" >&2
  exit 1
fi
test "$(grep -c "reason=2 coordinated=yes" \
    "$OUT_DIR/serial_transfer_stall.txt")" -eq 2
grep -q "fault=stall:timeout" "$OUT_DIR/serial_transfer_stall.txt"
grep -q "\\[left\\] Program exited with status 1" "$OUT_DIR/serial_transfer_stall.txt"
grep -q "\\[right\\] Program exited with status 1" "$OUT_DIR/serial_transfer_stall.txt"
! grep -q "max virtual ticks reached" "$OUT_DIR/serial_transfer_stall.txt"

if "$CLI" run --headless --max-ticks 500 -- "$EX_DIR/word_processor" > "$OUT_DIR/word_processor_lifetime.txt" 2>&1; then
  echo "word_processor should keep running until ESC" >&2
  exit 1
fi
grep -q "max virtual ticks reached" "$OUT_DIR/word_processor_lifetime.txt"

if "$CLI" run --headless --max-ticks 500 -- "$EX_DIR/music_maker" > "$OUT_DIR/music_maker_lifetime.txt" 2>&1; then
  echo "music_maker should keep running until ESC" >&2
  exit 1
fi
grep -q "max virtual ticks reached" "$OUT_DIR/music_maker_lifetime.txt"

if "$CLI" run --headless --max-ticks 500 -- "$EX_DIR/breakout_demo" > "$OUT_DIR/breakout_demo_lifetime.txt" 2>&1; then
  echo "breakout_demo should keep running until ESC" >&2
  exit 1
fi
grep -q "max virtual ticks reached" "$OUT_DIR/breakout_demo_lifetime.txt"

"$CLI" run --headless --script scripts/mouse_move.mlabscript -- "$EX_DIR/mouse_packet" > "$OUT_DIR/mouse.txt"
grep -q "mouse packet" "$OUT_DIR/mouse.txt"
grep -q "dx=5 dy=-2 lb=1" "$OUT_DIR/mouse.txt"

"$CLI" run --headless --dump-frame "$OUT_DIR/rectangle.ppm" -- "$EX_DIR/vbe_rectangle" > "$OUT_DIR/vbe.txt"
grep -q "rectangle drawn" "$OUT_DIR/vbe.txt"
test -s "$OUT_DIR/rectangle.ppm"

"$CLI" run --headless --script scripts/demo_exit.mlabscript --dump-frame "$OUT_DIR/sdl_demo.ppm" -- "$EX_DIR/sdl_demo" > "$OUT_DIR/sdl_demo.txt"
grep -q "SDL demo" "$OUT_DIR/sdl_demo.txt"
grep -q "kbd byte 0x81" "$OUT_DIR/sdl_demo.txt"
test -s "$OUT_DIR/sdl_demo.ppm"

"$CLI" run --headless --script scripts/caption_expiry_demo.mlabscript \
    --dump-frame "$OUT_DIR/sdl_demo_caption_expiry.ppm" -- \
    "$EX_DIR/sdl_demo" > "$OUT_DIR/sdl_demo_caption_expiry.txt"
cmp "$OUT_DIR/sdl_demo.ppm" "$OUT_DIR/sdl_demo_caption_expiry.ppm"

"$CLI" run --headless --script scripts/flappy_demo.mlabscript --audio-wav "$OUT_DIR/flappy.wav" --dump-frame "$OUT_DIR/flappy.ppm" -- "$EX_DIR/flappy_bird" > "$OUT_DIR/flappy.txt"
grep -q "LCOM Bird" "$OUT_DIR/flappy.txt"
test -s "$OUT_DIR/flappy.ppm"
test -s "$OUT_DIR/flappy.wav"

rm -rf "$OUT_DIR/flappy_space_frames"
"$CLI" run --headless --script scripts/flappy_space_stress.mlabscript --audio null --frame-dir "$OUT_DIR/flappy_space_frames" -- "$EX_DIR/flappy_bird" > "$OUT_DIR/flappy_space_stress.txt"
grep -q "Program exited with status 0" "$OUT_DIR/flappy_space_stress.txt"
test "$(find "$OUT_DIR/flappy_space_frames" -name 'frame_*.ppm' | wc -l | tr -d ' ')" -ge 45

"$CLI" run --headless --script scripts/ninjix_menu_exit.mlabscript --dump-frame "$OUT_DIR/ninjix_menu.ppm" -- "$EX_DIR/ninjix" > "$OUT_DIR/ninjix_menu.txt"
grep -q "Program exited with status 0" "$OUT_DIR/ninjix_menu.txt"
! grep -q "serial .* failed" "$OUT_DIR/ninjix_menu.txt"
test -s "$OUT_DIR/ninjix_menu.ppm"

"$CLI" run --headless --script scripts/pvz_menu_exit.mlabscript --dump-frame "$OUT_DIR/pvz_menu.ppm" --max-ticks 240 -- "$EX_DIR/pvz" > "$OUT_DIR/pvz_menu.txt"
grep -q "Program exited with status 0" "$OUT_DIR/pvz_menu.txt"
test -s "$OUT_DIR/pvz_menu.ppm"

"$CLI" replay scripts/flappy_demo.mlabscript --headless --video "$OUT_DIR/flappy_replay.mp4" --video-fps 30 -- "$EX_DIR/flappy_bird" > "$OUT_DIR/flappy_replay.txt"
grep -q "Rendered video" "$OUT_DIR/flappy_replay.txt"
test -s "$OUT_DIR/flappy_replay.mp4"

"$CLI" replay scripts/flappy_caption_demo.mlabscript --headless --video "$OUT_DIR/flappy_caption.mp4" --video-fps 30 -- "$EX_DIR/flappy_bird" > "$OUT_DIR/flappy_caption.txt"
grep -q "Rendered video" "$OUT_DIR/flappy_caption.txt"
test -s "$OUT_DIR/flappy_caption.mp4"

if [ "${LCOM_TEST_SDL:-0}" = "1" ]; then
  "$CLI" replay scripts/flappy_caption_demo.mlabscript --audio null --dump-frame "$OUT_DIR/flappy_replay_sdl.ppm" -- "$EX_DIR/flappy_bird" > "$OUT_DIR/flappy_replay_sdl.txt"
  grep -q "Program exited with status 0" "$OUT_DIR/flappy_replay_sdl.txt"
  test -s "$OUT_DIR/flappy_replay_sdl.ppm"
fi

rm -rf "$OUT_DIR/flappy-headless.mlab"
"$CLI" bundle . --program "$EX_DIR/flappy_bird" --name flappy-headless --output "$OUT_DIR/flappy-headless.mlab" --script scripts/flappy_demo.mlabscript --headless > "$OUT_DIR/bundle.txt"
test -x "$OUT_DIR/flappy-headless.mlab"
"$OUT_DIR/flappy-headless.mlab" > "$OUT_DIR/bundle-run.txt"
grep -q "Program exited with status 0" "$OUT_DIR/bundle-run.txt"

rm -rf "$OUT_DIR/flappy-headless-dir.mlab"
"$CLI" bundle . --format dir --program "$EX_DIR/flappy_bird" --name flappy-headless-dir --output "$OUT_DIR/flappy-headless-dir.mlab" --script scripts/flappy_demo.mlabscript --headless > "$OUT_DIR/bundle-dir.txt"
test -x "$OUT_DIR/flappy-headless-dir.mlab/run.sh"
test -f "$OUT_DIR/flappy-headless-dir.mlab/sdk/include/lcom/lcom.h"

echo "integration tests passed"
