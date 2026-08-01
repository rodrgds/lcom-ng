{ pkgs, ... }:

{
  packages = [
    pkgs.clang
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
    npm ci
    npm --prefix media/remotion ci
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
    echo "Machine Lab dev shell"
    echo "  machinelab           run ./build/machinelab, building it first when needed"
    echo "  machinelab-install   install locked docs and Remotion dependencies"
    echo "  machinelab-build     configure and build with SDL"
    echo "  machinelab-test      build and run tests"
    echo "  machinelab-verify    run the full code, docs, media, and diff gate"
    echo "  agent-preflight      show the read-only agent/worktree snapshot"
    echo "  machinelab-run-sdl   run the interactive SDL demo"
    echo "  machinelab-replay-flappy-video render a deterministic Flappy replay MP4"
    echo "  mlab/lowlab/lcom     compatibility aliases for machinelab"
  '';
}
