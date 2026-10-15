#!/usr/bin/env sh
set -eu

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"

"$ROOT/scripts/capture_marketing_examples.sh"
"$ROOT/scripts/capture_showcase_examples.sh"
"$ROOT/scripts/capture_launch_clips.sh"
python3 "$ROOT/scripts/compose_launch_audio.py" "$ROOT/build/launch-clips/ambient.wav"

cd "$ROOT/media/remotion"
bun run render
