import {test, expect, chromium, firefox} from '@playwright/test';
import {execFileSync} from 'node:child_process';
import {createServer} from 'node:http';
import {mkdtempSync, readFileSync, rmSync, writeFileSync} from 'node:fs';
import {tmpdir} from 'node:os';
import path from 'node:path';

// Each case launches its own browser, without inheriting the macOS Chrome channel.
test.use({launchOptions: {}});
const root = path.resolve(import.meta.dirname, '..');
let directory, server, base;
test.beforeAll(async () => {
  test.setTimeout(120_000);
  directory = mkdtempSync(path.join(tmpdir(), 'lcom-startup-'));
  writeFileSync(path.join(directory, 'lcom.collection.json'), JSON.stringify({
    name: 'Startup collection', projects: ['startup', 'pre-draw-input'].map(name => path.join(root, 'tests/fixtures/web', name)),
  }));
  execFileSync(process.env.LCOM_CLI || path.join(root, 'build/machinelab'),
    ['export', 'web', directory, '--output', path.join(directory, 'web')], {stdio: 'pipe'});
  const mime = {'.js': 'text/javascript', '.wasm': 'application/wasm', '.json': 'application/json', '.css': 'text/css', '.html': 'text/html'};
  server = createServer((request, response) => {
    const name = new URL(request.url, 'http://localhost').pathname;
    try {
      response.setHeader('Content-Type', mime[path.extname(name)] || 'text/html');
      response.end(readFileSync(path.join(directory, 'web', name.endsWith('/') ? name + 'index.html' : name)));
    } catch { response.writeHead(404).end(); }
  });
  await new Promise(resolve => server.listen(0, '127.0.0.1', resolve));
  base = `http://127.0.0.1:${server.address().port}`;
});
test.afterAll(async () => {
  if (server) { server.closeAllConnections(); await new Promise(resolve => server.close(resolve)); }
  if (directory) rmSync(directory, {recursive: true, force: true});
});

for (const browserType of [chromium, firefox]) {
  test(`${browserType.name()}: collection startup survives blur before its first frame and pauses afterwards`, async ({}, testInfo) => {
    const browser = await browserType.launch(browserType === chromium ?
      (process.env.CHROME_PATH ? {executablePath: process.env.CHROME_PATH} : process.platform === 'darwin' ? {channel: 'chrome'} : {}) : {});
    try {
      const page = await browser.newPage();
      const preparing = page.waitForEvent('console', message => message.text() === 'Preparing first frame');
      await page.goto(base);
      await page.getByRole('link', {name: 'Play startup', exact: true}).click();
      await preparing;
      const frame = page.frames().find(frame => frame.url().includes('/games/'));
      // Firefox can blur an inert iframe while its game initializes. Deliver the
      // same event deterministically while the guest is waiting to draw.
      await frame.evaluate(() => window.dispatchEvent(new Event('blur')));
      await expect(page.locator('#loading')).toBeHidden();
      const pixels = () => frame.locator('canvas').evaluate(canvas =>
        [...canvas.getContext('2d').getImageData(0, 0, 2, 1).data]);
      expect((await pixels()).slice(0, 4)).toEqual([17, 34, 51, 255]);
      await frame.evaluate(() => window.dispatchEvent(new Event('blur')));
      await page.waitForTimeout(150);
      const paused = await pixels();
      await page.waitForTimeout(250);
      expect(await pixels()).toEqual(paused);
      await frame.evaluate(() => window.dispatchEvent(new Event('focus')));
      await expect.poll(pixels).not.toEqual(paused);
      await page.screenshot({path: testInfo.outputPath('first-frame.png')});
    } finally { await browser.close(); }
  });
}

for (const mode of ['panel', 'overlay', 'right keyboard']) {
  test(`collection touch ${mode} keeps controls clear of the parent fullscreen button`, async ({}, testInfo) => {
    const browser = await chromium.launch(process.env.CHROME_PATH ? {executablePath: process.env.CHROME_PATH} :
      {channel: process.platform === 'darwin' ? 'chrome' : 'chromium'});
    const context = await browser.newContext({viewport: {width: 844, height: 390}, isMobile: true, hasTouch: true});
    const page = await context.newPage();
    try {
      await page.route('**/game.json', async route => {
        const response = await route.fetch(), settings = await response.json();
        settings.mobile.orientation = 'auto';
        Object.assign(settings.mobile.controls, {
          layout: mode === 'overlay' ? 'overlay' : 'panel', toolbar: 'auto',
          keyboard: mode === 'right keyboard', keyboardPosition: 'right',
          mouseButtons: false, joystick: false, look: false, dpad: false, pointer: 'off',
          quickKeys: mode === 'right keyboard' ? [] : ['1', '2', '3', 'ESC'], pads: [], actions: [],
        });
        await route.fulfill({response, json: settings});
      });
      await page.goto(base);
      await page.getByRole('link', {name: 'Play startup', exact: true}).tap();
      await expect(page.locator('#loading')).toBeHidden();
      const frame = page.frames().find(frame => frame.url().includes('/games/'));
      // This is the exported C guest, not a child that fabricates ready/layout IPC.
      await expect.poll(() => frame.locator('canvas').evaluate(canvas =>
        [...canvas.getContext('2d').getImageData(0, 0, 1, 1).data])).toEqual([17, 34, 51, 255]);
      if (mode === 'right keyboard') {
        await frame.locator('#keyboard-toggle').tap();
        await expect(frame.locator('#keyboard-toggle')).toHaveAttribute('aria-expanded', 'true');
      }
      const collisions = async () => {
        const fullscreen = await page.locator('#player-fullscreen').boundingBox();
        const buttons = await frame.locator('#touch-controls button:visible').evaluateAll(items =>
          items.map(element => ({label: element.getAttribute('aria-label') || element.textContent,
            ...element.getBoundingClientRect().toJSON()})));
        return buttons.filter(button => Math.min(button.right, fullscreen.x + fullscreen.width) > Math.max(button.left, fullscreen.x) &&
          Math.min(button.bottom, fullscreen.y + fullscreen.height) > Math.max(button.top, fullscreen.y));
      };
      await page.screenshot({path: testInfo.outputPath('collection-touch-chrome.png')});
      await expect.poll(collisions).toEqual([]);
      await page.locator('#back').tap();
      await expect(page.locator('#search-toggle')).toBeVisible();
    } finally { await browser.close(); }
  });
}

test('collection portrait rotation advice stays clear of authored top-corner actions', async ({}, testInfo) => {
  const browser = await chromium.launch(process.env.CHROME_PATH ? {executablePath: process.env.CHROME_PATH} :
    {channel: process.platform === 'darwin' ? 'chrome' : 'chromium'});
  try {
    const context = await browser.newContext({viewport: {width: 390, height: 844}, isMobile: true, hasTouch: true});
    const page = await context.newPage();
    await page.route('**/game.json', async route => {
      const response = await route.fetch(), settings = await response.json();
      settings.mobile.orientation = 'landscape';
      Object.assign(settings.mobile.controls, {
        layout: 'panel', toolbar: 'auto', keyboard: false, mouseButtons: false,
        joystick: false, look: false, dpad: false, pointer: 'off', quickKeys: [], pads: [],
        actions: [{label: 'Start', key: 'ENTER', position: 'top-right'},
          {label: 'Pause', key: 'ESC', position: 'top-right'}],
      });
      await route.fulfill({response, json: settings});
    });
    await page.goto(base);
    await page.getByRole('link', {name: 'Play startup', exact: true}).tap();
    await expect(page.locator('#loading')).toBeHidden();
    const frame = page.frames().find(frame => frame.url().includes('/games/'));
    await expect(frame.locator('#rotation-hint')).toBeVisible();
    await page.screenshot({path: testInfo.outputPath('portrait-rotation-actions.png')});
    await expect.poll(() => frame.evaluate(() => {
      const hint = document.querySelector('#rotation-hint').getBoundingClientRect();
      return [...document.querySelectorAll('.corner-actions button')].filter(button => {
        const box = button.getBoundingClientRect();
        return Math.min(box.right, hint.right) > Math.max(box.left, hint.left) &&
          Math.min(box.bottom, hint.bottom) > Math.max(box.top, hint.top);
      }).map(button => button.textContent);
    })).toEqual([]);
    await frame.getByRole('button', {name: 'Start', exact: true}).tap();
    await page.locator('#back').tap();
    await expect(page.locator('#search-toggle')).toBeVisible();
  } finally { await browser.close(); }
});

test('collection touch can explicitly open controls for a guest waiting for input before drawing', async ({}, testInfo) => {
  test.setTimeout(45_000);
  const browser = await chromium.launch(process.env.CHROME_PATH ? {executablePath: process.env.CHROME_PATH} :
    {channel: process.platform === 'darwin' ? 'chrome' : 'chromium'});
  try {
    const context = await browser.newContext({viewport: {width: 390, height: 844}, isMobile: true, hasTouch: true});
    const page = await context.newPage();
    await page.route('**/collection.json', async route => {
      const response = await route.fetch(), catalog = await response.json();
      catalog.taxonomy = [{id:'single-player', label:'Solo', facet:'players'}];
      for (const game of catalog.games) game.tags = ['single-player'];
      await route.fulfill({response, json:catalog});
    });
    await page.goto(base);
    const tile = page.getByRole('link', {name: 'Play Pre-draw input', exact: true});
    await tile.tap();
    const modal = page.locator('#loading');
    const showControls = page.getByRole('button', {name: 'Show controls', exact: true});
    await expect(showControls).toBeVisible();
    await expect(showControls).toBeInViewport({ratio:1});
    await expect(page.locator('#load-tags li')).toHaveText(['Solo']);
    await expect(page.locator('#load-tags')).toBeInViewport({ratio:1});
    await expect(page.locator('#load-description')).toBeVisible();
    await expect(modal).toBeVisible();
    const output = page.getByLabel('Game messages', {exact: true});
    await expect(output).toContainText('<Continue> Press Space to open the game');
    expect((await output.textContent()).length).toBeLessThanOrEqual(4096);
    await expect(output.locator('continue')).toHaveCount(0);
    const frame = page.frames().find(frame => frame.url().includes('/games/'));
    const continueButton = frame.getByRole('button', {name: 'Continue', exact: true});
    // Child visibility alone does not inspect a hidden ancestor iframe.
    await expect(continueButton.tap({timeout: 700})).rejects.toThrow(/hidden frame|intercepts pointer events/);
    await page.screenshot({path: testInfo.outputPath('original-input-prompt.png')});
    await showControls.tap();
    await expect(modal).toBeHidden();
    await expect(continueButton).toBeVisible();
    // No synthetic key is sent when showing controls: the guest still has not drawn.
    const pixels = () => frame.locator('canvas').evaluate(canvas =>
      [...canvas.getContext('2d').getImageData(0, 0, 1, 1).data]);
    expect(await pixels()).toEqual([0, 0, 0, 0]);
    await continueButton.tap();
    await expect.poll(pixels).toEqual([29, 71, 103, 255]);
    await expect(showControls).toBeHidden();
    await page.screenshot({path: testInfo.outputPath('ordinary-continue-drew-game.png')});
    await page.locator('#back').tap();
    await expect(page.locator('iframe')).toHaveCount(0);
    await expect(tile).toBeFocused();
    await tile.tap();
    await expect(showControls).toBeVisible();
    await page.locator('#load-dismiss').tap();
    await expect(page.locator('iframe')).toHaveCount(0);
    await expect(tile).toBeFocused();
    await page.setViewportSize({width:844,height:390});
    await tile.tap();
    await expect(showControls).toBeVisible();
    await expect(showControls).toBeInViewport({ratio:1});
    await expect(page.locator('#load-tags')).toBeInViewport({ratio:1});
    await page.screenshot({path:testInfo.outputPath('landscape-input-prompt.png')});
    await showControls.tap();
    const failingFrame = page.frames().find(frame => frame.url().includes('/games/'));
    await expect(failingFrame.locator('canvas')).toBeFocused();
    // X avoids the browser's reserved fullscreen Escape gesture.
    // The real fixture exits with status 6 before its first draw.
    await page.keyboard.press('x');
    await expect(page.locator('#load-error')).toBeVisible();
    await expect(showControls).toBeHidden();
    await expect(page.getByRole('button', {name: 'Try again', exact: true})).toBeFocused();
    await page.getByRole('button', {name: 'Try again', exact: true}).tap();
    await expect(showControls).toBeVisible();
    await showControls.tap();
    const retriedFrame = page.frames().find(frame => frame.url().includes('/games/'));
    await retriedFrame.getByRole('button', {name: 'Continue', exact: true}).tap();
    await expect.poll(() => retriedFrame.locator('canvas').evaluate(canvas =>
      [...canvas.getContext('2d').getImageData(0, 0, 1, 1).data])).toEqual([29, 71, 103, 255]);
    await page.locator('#back').tap();
    await expect(tile).toBeFocused();
  } finally { await browser.close(); }
});
