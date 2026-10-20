# Export a native Android app

Build a supported C game into an APK:

```sh
lcom-ng export android path/to/my-game --output dist/my-game-android
adb install -r dist/my-game-android/game.apk
```

In this repository, prefix commands with `devenv shell --`:

```sh
devenv shell -- lcom-ng export android examples/flappy --output dist/flappy-android
```

Student code, the compatibility SDK and simulated devices compile to native ARM64
with SDL3. APKs optimize native code even with the debug signing key. The app works
offline without WebAssembly, a WebView or a game server; sources stay unchanged.

## Build tools

Install Java 17 or newer and these Android SDK packages:

```sh
sdkmanager 'platforms;android-36' 'build-tools;36.0.0' 'ndk;27.1.12297006' 'cmake;3.22.1' 'platform-tools'
```

Set `ANDROID_HOME` and make Java available through `JAVA_HOME` and `PATH`. The
exporter downloads a checksum-verified SDL3 archive. The Gradle wrapper fetches its
pinned distribution and dependencies on first use; later builds reuse the caches.

Output includes `game.apk`, its SHA-256 checksum and a standalone Android project.
Open it in Android Studio, or run `./gradlew assembleDebug` inside it. Build
intermediates are omitted.

Distinct filenames such as `A.xpm` and `a.xpm` need a case-sensitive checkout and
output filesystem; the exporter rejects overwrites.

Generate the project without local Android tools:

```sh
lcom-ng export android . --project-only --output dist/android
```

This still downloads SDL3. Java and the Android SDK are needed to build it.
Re-exporting replaces the generated directory, including edits there. Failed builds
preserve the previous export.

## Project options

Set the name, icon, orientation, controls and theme once in an optional
[`lcom.json`](./project-config). CLI options override the file. `instructions`
appears only in web collections and does not change this app.

Sources, includes, assets and game arguments use the same options as
[browser exports](./browser#your-project). Native projects, student workspaces and
supported Minix Makefiles are accepted.

```sh
lcom-ng export android . --name 'My game' --app-id pt.example.mygame \
  --icon icon.png --orientation landscape \
  --asset assets --arg=--practice --output dist/android
```

`--icon` accepts square PNG artwork at least 48 pixels wide and creates an adaptive
launcher icon. `landscape` allows both sideways directions; `portrait` both upright
directions; `auto` follows rotation. Exports declare Android's game category,
preserving orientation restrictions under
[Android 16's large-screen rules](https://developer.android.com/about/versions/16/behavior-changes-16#adaptive-layouts).

For legacy paths such as `/home/lcom/labs/proj/assets`, select the enclosing
workspace:

```sh
lcom-ng export android . --asset-root .. --asset proj/assets
```

The default ABI is `arm64-v8a`. Repeat `--abi` for more, such as
`--abi arm64-v8a --abi x86_64` for phones and an Intel emulator; `armeabi-v7a` is
also available. Every ABI rebuilds the sources.

## Controls

The game keeps its aspect ratio without stretching or cropping; overlays stay in the
display's safe area.

- Drag over the game to move its cursor with left click held. Projects can enable
  [direct touch](./project-config#direct-touch). A pointer button toggles clicking
  and moving only.
- **Left click** and **Right click** can be held with another finger.
- Default quick keys are arrow keys, **Esc**, **Space** and **Enter**. Projects can
  replace them with a pad or other keys.
- **Keyboard** opens more keys; combine with a physical keyboard. **Fn** shows
  function and navigation keys, then **#+=** shows punctuation.
- The collapse button hides controls; tap it again to reopen.
- Android Back sends Escape. Backgrounding pauses virtual time, stops sound and
  releases held input. Rotation keeps the game running.

Bundled assets are copied to private app storage before launch, so normal C file
access works. Game-created files survive restarts; uninstalling removes them, and
updating assets replaces them.

## Limits and verification

APKs require Android 8 or newer and use a debug key for testing and sideloading.
Play signing, app bundles and publication are separate. Student code runs in the app
process; a crash terminates it. Long loops without SDK calls delay input.

Single-player and UART loopback are supported; paired runtimes, external processes
and arbitrary host libraries are not. Architecture assumptions, inline assembly and
macro-generated includes may need options or source changes.

Local verification covers SDK contracts, relocated builds, signed ARM64 APK
contents, and an Android 15 ARM64 emulator with rendering, touch, keyboard, resume,
rotation and direct tapping. CI also builds x86-64 APKs. Device latency, battery and
performance remain unmeasured.
