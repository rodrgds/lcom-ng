#!/usr/bin/env sh
set -eu

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
PUBLIC_DIR="$ROOT/docs-site/public/assets"
PUBLIC_PARENT="$ROOT/docs-site/public"
STAGING_DIR=""
BACKUP_DIR=""

cleanup() {
  if [ -n "$BACKUP_DIR" ] && [ -d "$BACKUP_DIR" ]; then
    if [ ! -e "$PUBLIC_DIR" ]; then
      mv "$BACKUP_DIR" "$PUBLIC_DIR"
    else
      rm -rf "$BACKUP_DIR"
    fi
  fi
  if [ -n "$STAGING_DIR" ] && [ -d "$STAGING_DIR" ]; then
    rm -rf "$STAGING_DIR"
  fi
}
trap cleanup 0
trap 'exit 1' 1 2 3 15

mkdir -p "$PUBLIC_PARENT"
STAGING_DIR="$(mktemp -d "$PUBLIC_PARENT/.assets.XXXXXX")"
mkdir -p "$STAGING_DIR/screenshots" "$STAGING_DIR/demo"

cp "$ROOT/docs/assets/screenshots/word-processor.png" "$STAGING_DIR/screenshots/word-processor.png"
cp "$ROOT/docs/assets/screenshots/music-maker.png" "$STAGING_DIR/screenshots/music-maker.png"
cp "$ROOT/docs/assets/screenshots/breakout.png" "$STAGING_DIR/screenshots/breakout.png"
cp "$ROOT/docs/assets/screenshots/flappy.png" "$STAGING_DIR/screenshots/flappy.png"
cp "$ROOT/docs/assets/screenshots/ninjix.png" "$STAGING_DIR/screenshots/ninjix.png"
cp "$ROOT/docs/assets/screenshots/pixel-studio.png" "$STAGING_DIR/screenshots/pixel-studio.png"
cp "$ROOT/docs/assets/screenshots/serial-whiteboard-left.png" "$STAGING_DIR/screenshots/serial-whiteboard-left.png"
cp "$ROOT/docs/assets/screenshots/serial-whiteboard-right.png" "$STAGING_DIR/screenshots/serial-whiteboard-right.png"
cp "$ROOT/docs/assets/screenshots/pomodoro-clock.png" "$STAGING_DIR/screenshots/pomodoro-clock.png"
cp "$ROOT/docs/assets/screenshots/interrupt-scope.png" "$STAGING_DIR/screenshots/interrupt-scope.png"
cp "$ROOT/docs/assets/screenshots/tilemap-editor.png" "$STAGING_DIR/screenshots/tilemap-editor.png"
cp "$ROOT/docs/assets/screenshots/serial-transfer-sender.png" "$STAGING_DIR/screenshots/serial-transfer-sender.png"
cp "$ROOT/docs/assets/screenshots/serial-transfer-receiver.png" "$STAGING_DIR/screenshots/serial-transfer-receiver.png"
cp "$ROOT/docs/assets/screenshots/ambient-aquarium.png" "$STAGING_DIR/screenshots/ambient-aquarium.png"
if [ -d "$ROOT/docs/assets/screenshots/gallery" ]; then
  cp -R "$ROOT/docs/assets/screenshots/gallery" "$STAGING_DIR/screenshots/gallery"
fi
cp "$ROOT/docs/assets/demo/machine-lab-demo.mp4" "$STAGING_DIR/demo/machine-lab-demo.mp4"

if [ -e "$PUBLIC_DIR" ]; then
  BACKUP_DIR="$(mktemp -d "$PUBLIC_PARENT/.assets-backup.XXXXXX")"
  rmdir "$BACKUP_DIR"
  mv "$PUBLIC_DIR" "$BACKUP_DIR"
fi
if mv "$STAGING_DIR" "$PUBLIC_DIR"; then
  STAGING_DIR=""
else
  cleanup
  exit 1
fi
if [ -n "$BACKUP_DIR" ]; then
  rm -rf "$BACKUP_DIR"
  BACKUP_DIR=""
fi

echo "synced docs-site assets"
