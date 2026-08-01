#!/usr/bin/env sh
set -eu

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
cd "$ROOT"

if [ ! -x node_modules/.bin/vitepress ] ||
   [ ! -x media/remotion/node_modules/.bin/tsc ]; then
  echo "Machine Lab JS dependencies are missing; run machinelab-install." >&2
  exit 1
fi

echo "==> Machine Lab build and CTest"
cmake -S . -B build -G Ninja -DMACHINE_LAB_WITH_SDL=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure

echo "==> Public docs and asset checks"
npm run docs:check

echo "==> Remotion typecheck"
(
  cd media/remotion
  npm exec -- tsc --noEmit
)

echo "==> Git whitespace check"
git diff --check

echo "Machine Lab verification passed"

