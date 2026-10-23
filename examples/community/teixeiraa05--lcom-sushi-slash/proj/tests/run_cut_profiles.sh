#!/bin/sh
set -eu
cd "$(dirname "$0")/../.."
python3 proj/tools/generate_cut_profiles.py --check
test_binary=$(mktemp /tmp/sushi-cut-tests.XXXXXX)
trap 'rm -f "$test_binary"' EXIT HUP INT TERM
${CC:-cc} -std=c11 -Wall -Wextra -Werror -Wno-unused-parameter -Wno-error=format-truncation -pedantic \
    -fsanitize=address,undefined -fno-omit-frame-pointer \
    -ffunction-sections -fdata-sections -Iproj/tests/stubs \
    proj/tests/cut_profiles_test.c proj/src/cut_profile.c proj/src/sprite.c \
    proj/src/sushi.c proj/src/game.c proj/src/ui/ui.c -Wl,--gc-sections -lm -o "$test_binary"
"$test_binary"
