#!/usr/bin/env sh
set -eu

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
cd "$ROOT"

if [ ! -x node_modules/.bin/vitepress ] ||
   [ ! -x media/remotion/node_modules/.bin/tsc ]; then
  echo "lcom-ng JS dependencies are missing; run lcom-ng-install." >&2
  exit 1
fi

echo "==> lcom-ng build and CTest"
cmake -S . -B build -G Ninja -DMACHINE_LAB_WITH_SDL=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure

echo "==> WebAssembly export and browser input checks"
bun run test:web

echo "==> Public docs and asset checks"
bun run docs:check

echo "==> Remotion typecheck"
(
  cd media/remotion
  bunx tsc --noEmit
)

echo "==> Git whitespace check"
git diff --check

echo "lcom-ng verification passed"
