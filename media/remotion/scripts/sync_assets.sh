#!/usr/bin/env sh
set -eu
ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/../../.." && pwd)"
PUBLIC_DIR="$ROOT/media/remotion/public"
mkdir -p "$PUBLIC_DIR/screenshots" "$PUBLIC_DIR/clips" "$PUBLIC_DIR/audio" "$PUBLIC_DIR/fonts"
cp "$ROOT"/docs/assets/screenshots/*.png "$PUBLIC_DIR/screenshots/"
for name in music-maker; do
  cp "$ROOT/docs/assets/demo/$name.mp4" "$PUBLIC_DIR/clips/$name.mp4"
done
for name in pixel_studio tilemap_editor ambient_aquarium interrupt_scope ninjix serial_whiteboard pvz breakout flappy word-processor; do
  cp "$ROOT/build/launch-clips/$name.mp4" "$PUBLIC_DIR/clips/$name.mp4"
done
cp "$ROOT/build/launch-clips/ambient.wav" "$PUBLIC_DIR/audio/ambient.wav"
cp -f "$ROOT"/media/remotion/assets/*.ttf "$PUBLIC_DIR/fonts/"
echo "Synced launch footage, typefaces and original ambient score"
