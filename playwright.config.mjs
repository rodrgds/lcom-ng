import { defineConfig } from '@playwright/test';

export default defineConfig({
  testDir: './tests',
  testMatch: ['web_export.spec.mjs', 'collection_web.spec.mjs', 'collection_navigation.spec.mjs', 'web_startup.spec.mjs'],
  workers: 1,
  timeout: 30_000,
  use: {
    launchOptions: process.env.CHROME_PATH ? { executablePath: process.env.CHROME_PATH } :
      process.platform === 'darwin' ? { channel: 'chrome' } : {},
    trace: 'retain-on-failure',
  },
  outputDir: 'build/web-test-results',
});
