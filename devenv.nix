{ pkgs, ... }:

{
  languages.python.enable = true;
  languages.python.package = pkgs.python3.withPackages (ps: [ ps.pillow ps.fonttools ps.brotli ]);
  packages = [
    pkgs.clang
    pkgs.emscripten
    pkgs.cli11
    pkgs.cmake
    pkgs.ninja
    pkgs.pkg-config
    pkgs.sdl3
    pkgs.sdl3-image
    pkgs.sdl3-ttf
    pkgs.sdl3-mixer
    pkgs.ffmpeg
    pkgs.glib
    pkgs.harfbuzz
    pkgs.freetype
    pkgs.dejavu_fonts
    pkgs.nodejs_22
    pkgs.bun
  ];

  env.MACHINE_LAB_FONT = "${pkgs.dejavu_fonts}/share/fonts/truetype/DejaVuSansMono.ttf";

  scripts.machinelab-configure.exec = ''
    cmake -S . -B build -G Ninja -DMACHINE_LAB_WITH_SDL=ON -DCMAKE_BUILD_TYPE=Debug
  '';

  scripts.machinelab-build.exec = ''
    cmake -S . -B build -G Ninja -DMACHINE_LAB_WITH_SDL=ON -DCMAKE_BUILD_TYPE=Debug
    cmake --build build
  '';

  scripts.machinelab-test.exec = ''
    cmake -S . -B build -G Ninja -DMACHINE_LAB_WITH_SDL=ON -DCMAKE_BUILD_TYPE=Debug
    cmake --build build
    ctest --test-dir build --output-on-failure
  '';

  scripts.machinelab-install.exec = ''
    bun install --frozen-lockfile
    bun install --cwd media/remotion --frozen-lockfile
  '';

  scripts.machinelab-verify.exec = ''
    exec sh scripts/verify.sh
  '';

  scripts.agent-preflight.exec = ''
    exec sh scripts/agent_preflight.sh
  '';

  scripts.machinelab.exec = ''
    if [ ! -x ./build/machinelab ]; then
      cmake -S . -B build -G Ninja -DMACHINE_LAB_WITH_SDL=ON -DCMAKE_BUILD_TYPE=Debug
      cmake --build build --target machinelab
    fi
    exec ./build/machinelab "$@"
  '';

  scripts.lcom.exec = ''
    exec machinelab "$@"
  '';

  scripts.lowlab.exec = ''
    exec machinelab "$@"
  '';

  scripts.mlab.exec = ''
    exec machinelab "$@"
  '';

  scripts.lcom-ng.exec = ''
    exec machinelab "$@"
  '';
  scripts.lcom-ng-build.exec = ''
    exec machinelab-build "$@"
  '';
  scripts.lcom-ng-test.exec = ''
    exec machinelab-test "$@"
  '';
  scripts.lcom-ng-install.exec = ''
    exec machinelab-install "$@"
  '';
  scripts.lcom-ng-verify.exec = ''
    exec machinelab-verify "$@"
  '';
  scripts.lcom-ng-configure.exec = ''
    exec machinelab-configure "$@"
  '';
  scripts.lcom-ng-web-test.exec = ''
    machinelab-build
    exec bun run test:web
  '';
  scripts.lcom-ng-android-test.exec = ''
    machinelab-build
    exec python3 tests/android_export.py "$@"
  '';
  scripts.lcom-ng-run-sdl.exec = ''
    exec machinelab-run-sdl "$@"
  '';
  scripts.lcom-ng-replay-flappy-video.exec = ''
    exec machinelab-replay-flappy-video "$@"
  '';

  scripts.machinelab-run-sdl.exec = ''
    cmake -S . -B build -G Ninja -DMACHINE_LAB_WITH_SDL=ON -DCMAKE_BUILD_TYPE=Debug
    cmake --build build
    build/machinelab run build/examples/sdl_demo
  '';

  scripts.machinelab-replay-flappy-video.exec = ''
    cmake -S . -B build -G Ninja -DMACHINE_LAB_WITH_SDL=ON -DCMAKE_BUILD_TYPE=Debug
    cmake --build build
    mkdir -p build/replays
    build/machinelab replay scripts/flappy_demo.mlabscript --headless --video build/replays/flappy.mp4 -- build/examples/flappy_bird
  '';

  enterShell = ''
    echo "lcom-ng dev shell"
    echo "  lcom-ng             run the runtime, building it first when needed"
    echo "  lcom-ng-build       configure and build with SDL"
    echo "  lcom-ng-verify      run the full code, docs, media, and diff gate"
    echo "  lcom-ng-install     install locked docs and Remotion dependencies"
    echo "  lcom-ng-test        build and run tests"
    echo "  agent-preflight      show the read-only agent/worktree snapshot"
    echo "  lcom-ng-run-sdl     run the interactive SDL demo"
    echo "  lcom-ng-replay-flappy-video render a deterministic Flappy replay MP4"
    echo "  machinelab/mlab/lowlab/lcom remain available as aliases"
  '';
}
