#!/usr/bin/env sh
# Ubuntu 24.04 has SDL2, so build the pinned SDL3 desktop dependencies once.
set -eu
prefix=${1:?usage: build_release_sdl.sh INSTALL_PREFIX}
work=${2:-build/release-sdl}
mkdir -p "$work" "$prefix"
prefix=$(CDPATH= cd -- "$prefix" && pwd)
fetch() {
  project=$1
  archive=$2
  version=$3
  expected=$4
  file="$work/$archive-$version.tar.gz"
  curl --fail --location --retry 3 "https://github.com/libsdl-org/$project/releases/download/release-$version/$archive-$version.tar.gz" -o "$file"
  printf '%s  %s\n' "$expected" "$file" | sha256sum --check
  tar -xzf "$file" -C "$work"
}
fetch SDL SDL3 3.2.28 1330671214d146f8aeb1ed399fc3e081873cdb38b5189d1f8bb6ab15bbc04211
cmake -S "$work/SDL3-3.2.28" -B "$work/sdl-build" -G Ninja \
  -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="$prefix" \
  -DSDL_SHARED=ON -DSDL_STATIC=OFF -DSDL_TESTS=OFF
cmake --build "$work/sdl-build" --parallel 4
cmake --install "$work/sdl-build"
fetch SDL_ttf SDL3_ttf 3.2.2 63547d58d0185c833213885b635a2c0548201cc8f301e6587c0be1a67e1e045d
cmake -S "$work/SDL3_ttf-3.2.2" -B "$work/ttf-build" -G Ninja \
  -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="$prefix" \
  -DCMAKE_PREFIX_PATH="$prefix" -DSDLTTF_VENDORED=OFF \
  -DSDLTTF_HARFBUZZ=OFF -DSDLTTF_PLUTOSVG=OFF -DSDLTTF_SAMPLES=OFF
cmake --build "$work/ttf-build" --parallel 4
cmake --install "$work/ttf-build"
