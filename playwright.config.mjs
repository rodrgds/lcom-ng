import { defineConfig } from '@playwright/test';

export default defineConfig({
  testDir: './tests',
  testMatch: ['web_export.spec.mjs', 'collection_web.spec.mjs', 'collection_navigation.spec.mjs', 'web_callbacks.spec.mjs', 'web_startup.spec.mjs', 'web_frames.spec.mjs'],
  workers: 1,
  timeout: 30_000,
  use: {
    // Pointer Lock needs the regular browser backend, rather than headless shell.
    // Keep the real Web Audio graph while avoiding a stalled host audio device.
    // Chromium replaces only the final OS stream, so signal assertions still run.
    // https://chromium.googlesource.com/chromium/src/+/f29eb01290cd36a30177ecf8197f906c01088a0d
    launchOptions: {
      ...(process.env.CHROME_PATH ? { executablePath: process.env.CHROME_PATH } :
        { channel: process.platform === 'darwin' ? 'chrome' : 'chromium' }),
      args: ['--disable-audio-output'],
    },
    trace: 'retain-on-failure',
  },
  outputDir: 'build/web-test-results',
});
