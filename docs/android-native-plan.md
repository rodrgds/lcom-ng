# Native Android export

Acceptance: a source project exports to a signed ARM64 APK, runs through SDL3
without WebView/WebAssembly, reads bundled assets, accepts touch/keyboard input,
plays audio, and pauses safely. Existing browser and desktop checks remain green.
Physical-device performance is separate from emulator execution.

1. Extract the in-process SDK into `runtime/embedded`, with browser and SDL hosts.
   Verify SDK contracts with deterministic host time and retain browser tests.
2. Build a reusable native SDL player. Keep the desktop process-isolated runtime
   intact. Verify the player on desktop before Android packaging.
3. Add source-based Android export, installed SDK payload, CLI help/completion,
   documentation and an Android smoke workflow. Verify APK contents and signature,
   then inspect a running game and exercise lifecycle/input on Android.

Use standard NDK/CMake, SDL's Android activity and Gradle packaging. Keep source
selection and compatibility Makefile parsing shared with existing exporters.
Avoid a generic plugin registry or changing the public C SDK. The platform seam
owns monotonic time, event pumping, presentation and audio; Machine owns devices.

Decision and verification trail: [android-native-decisions.tsv](android-native-decisions.tsv).
