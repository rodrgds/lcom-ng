#!/usr/bin/env sh
set -eu

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
cd "$ROOT"

echo "lcom-ng agent preflight"
printf "root: %s\n" "$ROOT"
git status --short --branch

cmake_version=$(sed -n 's/^project(lcom-ng VERSION \([^ ]*\).*/\1/p' CMakeLists.txt)
package_version=$(sed -n 's/^[[:space:]]*"version": "\([^"]*\)".*/\1/p' package.json | head -n 1)
printf "version: CMake=%s package=%s\n" "$cmake_version" "$package_version"

for command in cmake ninja ffmpeg node bun; do
  if command -v "$command" >/dev/null 2>&1; then
    printf "tool: %-6s available\n" "$command"
  else
    printf "tool: %-6s missing\n" "$command"
  fi
done

if [ -x node_modules/.bin/vitepress ]; then
  echo "deps: root present"
else
  echo "deps: root missing (run machinelab-install)"
fi

if [ -x media/remotion/node_modules/.bin/tsc ]; then
  echo "deps: remotion present"
else
  echo "deps: remotion missing (run machinelab-install)"
fi

test -f course/labs/function-requests.json
test -f docs/assets/demo/machine-lab-demo.mp4

echo "lab truth: course/labs/function-requests.json"
echo "public demo: docs/assets/demo/machine-lab-demo.mp4"
echo "full gate: devenv shell -- machinelab-verify"
