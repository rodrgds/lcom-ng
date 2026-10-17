# Export a native Android app

Build a supported C game into an installable APK:

```sh
lcom-ng export android path/to/my-game --output dist/my-game-android
adb install -r dist/my-game-android/game.apk
```

In this repository, prefix commands with `devenv shell --`:

```sh
devenv shell -- lcom-ng export android examples/flappy --output dist/flappy-android
```

Student code, the compatibility SDK and simulated devices compile to native ARM64.
SDL3 provides graphics, audio and input. APK builds enable native optimization
even when using the debug signing key. The app works offline and does not use
WebAssembly, a WebView or a game server. Original source files stay unchanged.

## Build tools

Install Java 17 or newer and these Android SDK packages through Android Studio's
SDK Manager, or `sdkmanager`:

```sh
sdkmanager 'platforms;android-36' 'build-tools;36.0.0' 'ndk;27.1.12297006' 'cmake;3.22.1' 'platform-tools'
```

Set `ANDROID_HOME` to your Android SDK directory and make Java available through
`JAVA_HOME` and `PATH`. The exporter downloads a checksum-verified SDL3 Android
archive. The included Gradle wrapper downloads its pinned distribution and build
dependencies on first use. Subsequent builds can reuse those caches.

The output includes `game.apk`, its SHA-256 checksum and a standalone Android
project. Move the directory to another machine, open it in Android Studio, or run
`./gradlew assembleDebug` inside it. Build intermediates are omitted.

Projects with distinct filenames such as `A.xpm` and `a.xpm` need a case-sensitive
source checkout and output filesystem. The exporter rejects destinations that
would overwrite one name with the other.

Generate the project without local Android build tools:

```sh
lcom-ng export android . --project-only --output dist/android
```

This still downloads SDL3. Java and the Android SDK are needed when building
the generated project. Re-exporting replaces an existing generated directory,
including edits made there. Failed builds preserve the previous export.

## Project options

Set the name, icon, mobile orientation, controls and theme once in an optional
[`lcom.json`](./project-config). CLI options override the file.


Sources, includes, assets and game arguments use the same options as
[browser exports](./browser#your-project). Native projects, generated student
workspaces and supported Minix Makefiles are accepted.

```sh
lcom-ng export android . --name 'My game' --app-id pt.example.mygame \
  --icon icon.png --orientation landscape \
  --asset assets --arg=--practice --output dist/android
```

`--icon` accepts square PNG artwork at least 48 pixels wide and creates an
adaptive Android launcher icon. Use `--orientation landscape` for wide games;
both sideways directions are allowed. `portrait` allows either upright direction,
and the default `auto` follows device rotation.
Exports declare Android's game category, which preserves orientation restrictions
under [Android 16's large-screen rules](https://developer.android.com/about/versions/16/behavior-changes-16#adaptive-layouts).

For legacy paths such as `/home/lcom/labs/proj/assets`, select the enclosing
workspace as the asset root:

```sh
lcom-ng export android . --asset-root .. --asset proj/assets
```

The default architecture is `arm64-v8a`. Repeat `--abi` to include other supported
architectures, for example `--abi arm64-v8a --abi x86_64` for phones and an Intel
Android emulator. `armeabi-v7a` is also available. Every selected architecture
rebuilds the original sources; existing desktop and Minix binaries cannot be used.

## Controls

The game keeps its aspect ratio and fills the available width or height without
stretching or cropping. Overlay controls stay inside the display's safe area
while the game uses the full surface.

- By default, drag over the game to move its cursor while holding left click.
  Projects can enable [direct touch](./project-config#direct-touch) to tap at the
  finger and drag from there. PvZ and Sushi presets use direct touch.
  The mode button shows **Drag on** or **Move only**. Tap it to switch.
- **Left click** and **Right click** can be held with another finger while moving.
- Arrow keys, **Esc**, **Space** and **Enter** are always available.
- **Keyboard** opens more keys. Hold multiple keys or combine them with a
  physical keyboard. **Fn** shows function and navigation keys, then **Symbols**
  shows punctuation and more modifiers.
- **Hide** collapses the controls; **Controls** opens them again.
- Android Back sends Escape to the game. Backgrounding pauses virtual time,
  stops sound and releases held input. Rotation keeps the running game.

Bundled assets are copied into private app storage before the game starts, so
ordinary C file access works. Game-created files there can survive app restarts;
uninstalling removes them. Updating bundled assets replaces those files.

## Limits and verification

These APKs require Android 8 or newer and use a debug signing key for testing and
sideloading. Google Play release signing, app bundles and store publication are
separate work. Native student code executes inside the app process; a student
crash terminates the app. Long loops without SDK calls can delay input handling.

Single-player and UART loopback are supported. Paired runtimes, external
processes and arbitrary host system libraries are not part of the export.
Architecture-specific assumptions, inline assembly and macro-generated include
paths may require explicit source/include options or project changes.

Local verification covers shared SDK contracts, relocated native and Minix builds,
signed ARM64 APK contents, and an Android 15 ARM64 emulator. The emulator checks
rendering, touch, hardware keyboard, restored pixels after resume, landscape and
direct tapping, and exit. The Android CI workflow also builds x86-64 APKs; its
GitHub execution is separate evidence. Physical-device latency, battery use and
performance remain unmeasured.
