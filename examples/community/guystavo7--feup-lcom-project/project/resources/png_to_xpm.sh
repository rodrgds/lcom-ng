#!/bin/bash

INPUT_DIR="${1:-.}"
OUTPUT_DIR="${2:-xpm_out}"

mkdir -p "$OUTPUT_DIR"
TMP_DIR="$OUTPUT_DIR/tmp"

mkdir -p "$TMP_DIR"

echo "=== Step 1: Convert to XPM ==="

for img in "$INPUT_DIR"/*.{png,jpg,jpeg,bmp}; do
    [ -e "$img" ] || continue

    name=$(basename "$img")
    name="${name%.*}"

    echo "Converting: $img"

    magick "$img" \
        -colors 256 \
        -dither None \
        "$TMP_DIR/$name.xpm"
done

echo "=== Step 2: Force hex colors ==="

for f in "$TMP_DIR"/*.xpm; do
    [ -e "$f" ] || continue

    name=$(basename "$f")

    sed -E \
      's/([[:space:]]c[[:space:]])black/\1#000000/g;
       s/([[:space:]]c[[:space:]])white/\1#FFFFFF/g;
       s/([[:space:]]c[[:space:]])gray0/\1#000000/g;
       s/([[:space:]]c[[:space:]])gray100/\1#FFFFFF/g' \
      "$f" > "$name"
done

rm -rf "$TMP_DIR"
