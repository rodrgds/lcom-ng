#!/bin/bash

OUTPUT="all_xpm.h"

echo "/* Auto-generated XPM bundle */" > "$OUTPUT"
echo "" >> "$OUTPUT"

names=""

sanitize() {
    echo "$1" | sed 's/[^a-zA-Z0-9_]/_/g'
}

# Sort XPM files alphabetically
for f in $(printf '%s\n' *.xpm | sort); do
    base=$(basename "$f" .xpm)
    name=$(sanitize "$base")

    # store clean pointer name
    names="$names $name"

    echo "/* ===== $name ===== */" >> "$OUTPUT"
    cat "$f" >> "$OUTPUT"
    echo "" >> "$OUTPUT"
done

# ---- master pointer array ----
echo "/* ===== master pointer list ===== */" >> "$OUTPUT"
echo "static char **all_xpms[] = {" >> "$OUTPUT"

for name in $names; do
    echo "    ${name}," >> "$OUTPUT"
done

echo "};" >> "$OUTPUT"
