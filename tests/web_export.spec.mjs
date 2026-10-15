import { test, expect } from '@playwright/test';
import { execFileSync, spawn, spawnSync } from 'node:child_process';
import { createServer } from 'node:http';
import { mkdtempSync, mkdirSync, readFileSync, readdirSync, renameSync, rmSync, copyFileSync, cpSync } from 'node:fs';
import { tmpdir } from 'node:os';
import path from 'node:path';

const root = path.resolve(import.meta.dirname, '..');
const cli = process.env.LCOM_CLI || path.join(root, 'build/machinelab');
let directory, server, base, workspace;
const output = new WeakMap();
function observe(page) {
  const messages = [];
  output.set(page, messages);
  page.on('console', message => messages.push(message.text()));
}

test.beforeAll(async () => {
  test.setTimeout(180_000);
  directory = mkdtempSync(path.join(tmpdir(), 'lcom browser export '));
  const exportGame = (project, name, command, ...options) => {
    const original = path.join(directory, `original-${name}`);
    execFileSync(command, ['export', 'web', path.resolve(root, project), '--output', original, ...options], { cwd: directory, stdio: 'pipe' });
    const relocated = path.join(directory, 'games', name);
    mkdirSync(path.dirname(relocated), { recursive: true });
    renameSync(original, relocated);
  };
  exportGame('tests/fixtures/web/native', 'fixture', cli, '--asset', 'assets');
  exportGame('tests/fixtures/web/minix', 'legacy', cli, '--asset-root', '..', '--asset', 'minix/assets');
  exportGame('examples/flappy', 'flappy', cli, '--name', 'LCOM Bird');
  exportGame('examples/ninjix', 'ninjix', cli, '--name', 'Ninjix');
  workspace = path.join(directory, 'new student game');
  execFileSync(cli, ['setup', workspace], { stdio: 'pipe' });
  copyFileSync(path.join(root, 'tests/fixtures/web/native/main.c'), path.join(workspace, 'proj/main.c'));
  cpSync(path.join(root, 'tests/fixtures/web/native/assets'), path.join(workspace, 'assets'), { recursive: true });
  const installed = path.join(directory, 'installed');
  execFileSync('python3', ['-c',
    'from pathlib import Path; import sys; sys.path.insert(0, str(Path(sys.argv[1]) / "scripts")); from package_student import copy_student_payload; copy_student_payload(Path(sys.argv[1]), Path(sys.argv[1]) / "build", Path(sys.argv[2]))', root, installed]);
  mkdirSync(path.join(installed, 'bin'));
  const installedCli = path.join(installed, 'bin/lcom-ng');
  copyFileSync(cli, installedCli);
  exportGame(workspace, 'installed', installedCli, '--name', 'Installed Game');
  const mime = { '.js': 'text/javascript', '.wasm': 'application/wasm', '.json': 'application/json', '.css': 'text/css', '.html': 'text/html' };
  server = createServer((request, response) => {
    const filename = path.join(directory, decodeURIComponent(new URL(request.url, 'http://localhost').pathname));
    const file = filename.endsWith(path.sep) ? filename + 'index.html' : filename;
    if (!file.startsWith(directory + path.sep)) { response.writeHead(403).end(); return; }
    try { response.setHeader('Content-Type', mime[path.extname(file)] || 'application/octet-stream'); response.end(readFileSync(file)); }
    catch { response.writeHead(404).end(); }
  });
  await new Promise(resolve => server.listen(0, '127.0.0.1', resolve));
  base = `http://127.0.0.1:${server.address().port}/games`;
});

test.afterAll(async () => {
  if (server) await new Promise(resolve => server.close(resolve));
  if (directory) rmSync(directory, { recursive: true, force: true });
});

async function start(page, game = 'fixture') {
  observe(page);
  await page.goto(`${base}/${game}/`);
  await expect(page.locator('#status')).toHaveText('Game running.');
}
const consoleText = page => (output.get(page) || []).join('\n');
function mousePackets(page) {
  const bytes = [...consoleText(page).matchAll(/mouse:([0-9a-f]{2})/g)].map(match => parseInt(match[1], 16));
  if (bytes[0] === 0xfa) bytes.shift();
  return Array.from({ length: Math.floor(bytes.length / 3) }, (_, i) => bytes.slice(i * 3, i * 3 + 3));
}
const pixel = page => page.locator('canvas').evaluate(canvas => [...canvas.getContext('2d').getImageData(0, 0, 1, 1).data]);

test('desktop starts directly in a game with only black letterboxing', async ({ page }) => {
  await start(page);
  await expect.poll(() => pixel(page)).toEqual([17, 34, 51, 255]);
  await expect(page.locator('header, nav, h1, button:visible, details')).toHaveCount(0);
  const bounds = await page.locator('canvas').boundingBox();
  const viewport = page.viewportSize();
  expect(Math.abs(bounds.width / bounds.height - 4 / 3)).toBeLessThan(0.001);
  expect(Math.abs(bounds.height - viewport.height)).toBeLessThan(1);
  await expect(page.locator('body')).toHaveCSS('background-color', 'rgb(0, 0, 0)');
});

test('relocated export loads assets and renders, handles physical input, plays PCM and exits', async ({ page }) => {
  const errors = [];
  page.on('pageerror', error => errors.push(error.message));
  // Observe real Web Audio calls without replacing the audio implementation.
  await page.addInitScript(() => {
    const start = AudioBufferSourceNode.prototype.start;
    AudioBufferSourceNode.prototype.start = function (...args) {
      window.playedAudio = { channels: this.buffer.numberOfChannels, rate: this.buffer.sampleRate, sample: this.buffer.getChannelData(0)[0] };
      return start.apply(this, args);
    };
  });
  await start(page);
  await expect.poll(() => consoleText(page)).toContain('asset:browser asset loaded');
  await expect.poll(() => pixel(page)).toEqual([17, 34, 51, 255]);
  await expect.poll(() => consoleText(page)).toContain('tick60');
  await page.keyboard.press('a');
  await expect.poll(() => consoleText(page)).toContain('key:1e');
  await expect.poll(() => consoleText(page)).toContain('key:9e');
  await expect.poll(() => pixel(page)).toEqual([0, 255, 0, 255]);
  await expect.poll(() => page.evaluate(() => window.playedAudio)).toEqual({ channels: 2, rate: 48000, sample: 0.125 });
  await page.locator('canvas').click({ position: { x: 40, y: 40 } });
  await expect.poll(() => consoleText(page)).toContain('mouse:09');
  await expect.poll(() => page.evaluate(() => document.pointerLockElement?.id)).toBe('game');
  await expect(page.locator('canvas')).toHaveCSS('cursor', 'none');
  await page.mouse.move(500, 300);
  await page.keyboard.press('ControlRight');
  await expect.poll(() => page.evaluate(() => document.pointerLockElement)).toBeNull();
  await page.keyboard.down('ArrowLeft');
  await expect.poll(() => consoleText(page)).toContain('key:4b');
  await page.evaluate(() => window.dispatchEvent(new Event('blur')));
  await page.evaluate(() => window.dispatchEvent(new Event('focus')));
  await expect.poll(() => consoleText(page)).toContain('key:cb');
  await page.keyboard.up('ArrowLeft');
  await page.locator('canvas').click({ position: { x: 40, y: 40 } });
  await expect.poll(() => page.evaluate(() => document.pointerLockElement?.id)).toBe('game');
  await page.keyboard.press('Escape');
  await expect(page.locator('#status')).toContainText('Game finished.');
  expect(errors).toEqual([]);
  const files = readdirSync(path.join(directory, 'games/fixture'));
  expect(files).toContain('game.wasm');
  expect(files).toContain('game.data');
  expect(files.some(name => name.endsWith('.o') || name.endsWith('.c'))).toBe(false);
});

test('legacy polling loop yields for browser key input without source changes', async ({ page }) => {
  await start(page, 'legacy');
  await expect.poll(() => consoleText(page)).toContain('legacy polling ready');
  await expect.poll(() => consoleText(page)).toContain('legacy common:42');
  await expect.poll(() => consoleText(page)).toContain('legacy asset:legacy asset loaded');
  await page.keyboard.press('a');
  await expect.poll(() => consoleText(page)).toContain('legacy:1e');
  await expect.poll(() => consoleText(page)).toContain('legacy:9e');
  await page.keyboard.press('Escape');
  await expect(page.locator('#status')).toContainText('Game finished.');
});

test('browser-consumed Escape reaches the game when it releases mouse capture', async ({ page }) => {
  await start(page);
  const capture = async () => {
    const acquired = page.evaluate(() => new Promise(resolve => document.addEventListener('pointerlockchange', () => resolve(document.pointerLockElement?.id), { once: true })));
    await page.locator('canvas').click();
    expect(await acquired).toBe('game');
  };
  await capture();
  await page.evaluate(() => new Promise(resolve => {
    document.addEventListener('pointerlockchange', resolve, { once: true });
    window.dispatchEvent(new Event('blur'));
    document.exitPointerLock();
  }));
  await expect(page.locator('#status')).toHaveText('Game running.');
  expect(consoleText(page)).not.toContain('key:01');
  await page.evaluate(() => window.dispatchEvent(new Event('focus')));
  await capture();
  // Native Chrome consumes Escape before DOM keydown and releases pointer lock.
  await page.evaluate(() => document.exitPointerLock());
  await expect(page.locator('#status')).toContainText('Game finished.');
});

test('installed CLI exports an external project after relocation', async ({ page }) => {
  await start(page, 'installed');
  await expect(page).toHaveTitle('Installed Game · lcom-ng');
  await expect.poll(() => pixel(page)).toEqual([17, 34, 51, 255]);
  await expect.poll(() => consoleText(page)).toContain('asset:browser asset loaded');
});


test('mobile taps click and held touches drag without jumping on contact', async ({ browser }) => {
  const context = await browser.newContext({ viewport: { width: 390, height: 844 }, isMobile: true, hasTouch: true });
  const page = await context.newPage();
  await start(page);
  await expect.poll(() => consoleText(page)).toContain('ready');
  const cdp = await context.newCDPSession(page);
  const box = await page.locator('canvas').boundingBox();
  const contact = { x: box.x + box.width * 0.75, y: box.y + box.height * 0.75, id: 1 };
  const packets = () => mousePackets(page);
  await cdp.send('Input.dispatchTouchEvent', { type: 'touchStart', touchPoints: [contact] });
  await expect.poll(() => packets().at(-1)?.[0] & 3).toBe(1);
  expect(packets().at(-1).slice(1)).toEqual([0, 0]);
  await cdp.send('Input.dispatchTouchEvent', { type: 'touchMove', touchPoints: [{ ...contact, x: contact.x + box.width / 10 }] });
  await cdp.send('Input.dispatchTouchEvent', { type: 'touchEnd', touchPoints: [] });
  await expect.poll(() => packets().length).toBeGreaterThan(0);
  await expect.poll(() => packets().at(-1)?.[0] & 3).toBe(0);
  expect(packets().some(([flags, x]) => (flags & 3) === 1 && x > 0)).toBe(true);
  // One tenth of the 800px framebuffer, regardless of where the finger landed.
  expect(packets().reduce((sum, [flags, x]) => sum + x - (flags & 16 ? 256 : 0), 0)).toBe(80);
  const beforeTap = packets().length;
  await page.locator('canvas').tap({ position: { x: 20, y: 20 } });
  await expect.poll(() => packets().length).toBe(beforeTap + 2);
  expect(packets().slice(beforeTap).map(([flags]) => flags & 3)).toEqual([1, 0]);
  expect(packets().slice(beforeTap).every(([, dx, dy]) => dx === 0 && dy === 0)).toBe(true);
  const left = await page.getByRole('button', { name: 'Left click', exact: true }).boundingBox();
  const right = await page.getByRole('button', { name: 'Right click', exact: true }).boundingBox();
  const fingers = [left, right].map((box, id) => ({ x: box.x + box.width / 2, y: box.y + box.height / 2, id: id + 2 }));
  await cdp.send('Input.dispatchTouchEvent', { type: 'touchStart', touchPoints: [fingers[0]] });
  await expect.poll(() => packets().at(-1)?.[0] & 3).toBe(1);
  await cdp.send('Input.dispatchTouchEvent', { type: 'touchStart', touchPoints: fingers });
  await expect.poll(() => packets().at(-1)?.[0] & 3).toBe(3);
  await cdp.send('Input.dispatchTouchEvent', { type: 'touchStart', touchPoints: [...fingers, contact] });
  await cdp.send('Input.dispatchTouchEvent', { type: 'touchMove', touchPoints: [...fingers, { ...contact, y: contact.y - 20 }] });
  await expect.poll(() => packets().at(-1)?.[2]).toBeGreaterThan(0);
  expect(packets().at(-1)[0] & 3).toBe(3);
  expect(packets().reduce((sum, [flags, x]) => sum + x - (flags & 16 ? 256 : 0), 0)).toBe(80);
  await cdp.send('Input.dispatchTouchEvent', { type: 'touchCancel', touchPoints: [] });
  await expect.poll(() => packets().at(-1)?.[0] & 3).toBe(0);
  await expect(page.getByRole('button', { name: 'Left click', exact: true })).toHaveAttribute('aria-pressed', 'false');
  await context.close();
});

test('mobile controls stay compact, with a dismissible keyboard and optional mapped joystick', async ({ browser }) => {
  const context = await browser.newContext({ viewport: { width: 390, height: 844 }, isMobile: true, hasTouch: true });
  const page = await context.newPage();
  await start(page);
  const keyboard = page.getByRole('region', { name: 'On-screen keyboard' });
  await expect(keyboard).toBeHidden();
  await expect(page.locator('#joystick')).toBeHidden();
  expect((await page.locator('#touch-controls').boundingBox()).height).toBeLessThan(130);
  await page.getByRole('button', { name: 'Keyboard', exact: true }).tap();
  await expect(keyboard).toBeVisible();
  await page.locator('#keys [data-code="ShiftLeft"]').tap();
  await page.getByRole('button', { name: 'Keyboard', exact: true }).tap();
  await expect(keyboard).toBeHidden();
  await expect.poll(() => consoleText(page)).toContain('key:aa');
  await page.getByRole('button', { name: 'Joystick', exact: true }).tap();
  const cdp = await context.newCDPSession(page);
  const pad = await page.locator('#joystick').boundingBox();
  const right = { x: pad.x + pad.width - 10, y: pad.y + pad.height / 2, id: 1 };
  await cdp.send('Input.dispatchTouchEvent', { type: 'touchStart', touchPoints: [right] });
  await expect.poll(() => mousePackets(page).length).toBeGreaterThan(2);
  expect(mousePackets(page).every(([flags, dx, dy]) => (flags & 3) === 0 && dx > 0 && dy === 0)).toBe(true);
  expect(consoleText(page)).not.toContain('key:4d');
  await cdp.send('Input.dispatchTouchEvent', { type: 'touchCancel', touchPoints: [] });
  await page.getByLabel('Joystick mode').selectOption('arrows');
  const arrows = await page.locator('#joystick').boundingBox();
  const arrowRight = { x: arrows.x + arrows.width - 10, y: arrows.y + arrows.height / 2, id: 1 };
  await cdp.send('Input.dispatchTouchEvent', { type: 'touchStart', touchPoints: [arrowRight] });
  await expect.poll(() => consoleText(page)).toContain('key:4d');
  await cdp.send('Input.dispatchTouchEvent', { type: 'touchMove', touchPoints: [{ ...arrowRight, x: arrows.x + arrows.width / 2, y: arrows.y + arrows.height / 2 }] });
  await expect.poll(() => consoleText(page)).toContain('key:cd');
  await cdp.send('Input.dispatchTouchEvent', { type: 'touchEnd', touchPoints: [] });
  await page.getByLabel('Joystick mode').selectOption('wasd');
  await cdp.send('Input.dispatchTouchEvent', { type: 'touchStart', touchPoints: [right] });
  await expect.poll(() => consoleText(page)).toContain('key:20');
  await cdp.send('Input.dispatchTouchEvent', { type: 'touchCancel', touchPoints: [] });
  await expect.poll(() => consoleText(page)).toContain('key:a0');
  await page.getByRole('button', { name: 'Joystick', exact: true }).tap();
  // Both orientations must leave room for the game and avoid page overflow.
  for (const viewport of [{ width: 390, height: 844 }, { width: 844, height: 390 }]) {
    await page.setViewportSize(viewport);
    const controlHeight = (await page.locator('#touch-controls').boundingBox()).height;
    const expectedHeight = Math.min(viewport.width * 3 / 4, viewport.height - controlHeight);
    expect(Math.abs((await page.locator('canvas').boundingBox()).height - expectedHeight)).toBeLessThan(1);
    await expect.poll(() => page.evaluate(() => document.documentElement.scrollWidth <= innerWidth && document.documentElement.scrollHeight <= innerHeight)).toBe(true);
  }
  await context.close();
});

test('dual joysticks move and aim independently while a mouse button is held', async ({ browser }) => {
  const context = await browser.newContext({ viewport: { width: 390, height: 844 }, isMobile: true, hasTouch: true });
  const page = await context.newPage();
  await start(page);
  await page.getByRole('button', { name: 'Joystick', exact: true }).tap();
  const cdp = await context.newCDPSession(page);
  for (const [mode, make, release] of [['mouse-arrows', '4b', 'cb'], ['mouse-wasd', '1e', '9e']]) {
    await page.getByLabel('Joystick mode').selectOption(mode);
    const movement = await page.getByRole('group', { name: 'Movement joystick', exact: true }).boundingBox();
    const mouse = await page.getByRole('group', { name: 'Mouse joystick', exact: true }).boundingBox();
    const click = await page.getByRole('button', { name: 'Left click', exact: true }).boundingBox();
    const hold = { x: click.x + click.width / 2, y: click.y + click.height / 2, id: 3 };
    const move = { x: movement.x + 10, y: movement.y + movement.height / 2, id: 1 };
    const aim = { x: mouse.x + mouse.width * 0.75, y: mouse.y + mouse.height * 0.25, id: 2 };
    await cdp.send('Input.dispatchTouchEvent', { type: 'touchStart', touchPoints: [hold] });
    await cdp.send('Input.dispatchTouchEvent', { type: 'touchStart', touchPoints: [hold, move] });
    await expect.poll(() => consoleText(page)).toContain(`key:${make}`);
    const before = mousePackets(page).length;
    await cdp.send('Input.dispatchTouchEvent', { type: 'touchStart', touchPoints: [hold, move, aim] });
    await expect.poll(() => mousePackets(page).length).toBeGreaterThan(before + 2);
    expect(mousePackets(page).slice(before).every(([flags, dx, dy]) => (flags & 3) === 1 && dx > 0 && dy > 0)).toBe(true);
    // Lifting the movement finger releases keys while the other stick keeps aiming.
    await cdp.send('Input.dispatchTouchEvent', { type: 'touchEnd', touchPoints: [move] });
    await expect.poll(() => consoleText(page)).toContain(`key:${release}`);
    const afterMovement = mousePackets(page).length;
    await expect.poll(() => mousePackets(page).length).toBeGreaterThan(afterMovement + 2);
    await cdp.send('Input.dispatchTouchEvent', { type: 'touchCancel', touchPoints: [] });
    await expect(page.getByRole('button', { name: 'Left click', exact: true })).toHaveAttribute('aria-pressed', 'false');
    await expect.poll(() => mousePackets(page).at(-1)).toEqual([8, 0, 0]);
    const stopped = mousePackets(page).length;
    // Advance several painted frames to detect a cursor loop that outlives its touch.
    await page.evaluate(() => new Promise(resolve => requestAnimationFrame(() => requestAnimationFrame(() => requestAnimationFrame(resolve)))));
    expect(mousePackets(page).length).toBe(stopped);
  }
  await page.getByRole('button', { name: 'Joystick', exact: true }).tap();
  expect((await page.locator('#touch-controls').boundingBox()).height).toBeLessThan(130);
  await context.close();
});

test('mobile keyboard supports held keys, modifiers, simultaneous touch and cancellation', async ({ browser }) => {
  const context = await browser.newContext({ viewport: { width: 390, height: 844 }, isMobile: true, hasTouch: true });
  const page = await context.newPage();
  await start(page);
  await page.getByRole('button', { name: 'Keyboard', exact: true }).tap();
  await expect(page.getByRole('region', { name: 'On-screen keyboard' })).toBeVisible();
  const keyboardBounds = await page.locator('#touch-controls').boundingBox();
  expect(Math.abs(keyboardBounds.y + keyboardBounds.height - 844)).toBeLessThan(2);
  await expect.poll(() => page.evaluate(() => document.documentElement.scrollWidth <= innerWidth)).toBe(true);
  const keys = page.locator('#keys');
  const a = keys.locator('[data-code="KeyA"]');
  await a.tap();
  await expect.poll(() => consoleText(page)).toContain('key:1e');
  await expect.poll(() => consoleText(page)).toContain('key:9e');
  const shift = page.locator('[data-code="ShiftLeft"]');
  await shift.tap();
  await expect(shift).toHaveAttribute('aria-pressed', 'true');
  await a.tap();
  await shift.tap();
  await expect.poll(() => consoleText(page)).toContain('key:2a');
  await expect.poll(() => consoleText(page)).toContain('key:aa');
  // Real simultaneous contacts, including cancellation, through the browser touch protocol.
  const cdp = await context.newCDPSession(page);
  const arrow = await page.locator('[data-code="ArrowLeft"]').boundingBox();
  const space = await keys.locator('[data-code="Space"]').boundingBox();
  const points = [arrow, space].map((box, id) => ({ x: box.x + box.width / 2, y: box.y + box.height / 2, id }));
  await cdp.send('Input.dispatchTouchEvent', { type: 'touchStart', touchPoints: points });
  await expect.poll(() => consoleText(page)).toContain('key:4b');
  await expect.poll(() => consoleText(page)).toContain('key:39');
  await cdp.send('Input.dispatchTouchEvent', { type: 'touchCancel', touchPoints: [] });
  await expect.poll(() => consoleText(page)).toContain('key:cb');
  await expect.poll(() => consoleText(page)).toContain('key:b9');
  await page.getByRole('button', { name: 'Function keys' }).tap();
  await page.getByRole('button', { name: 'F1', exact: true }).tap();
  await expect.poll(() => consoleText(page)).toContain('key:3b');
  await page.getByRole('button', { name: 'Function keys' }).tap();
  await keys.getByRole('button', { name: 'ESC', exact: true }).tap();
  await expect(page.locator('#status')).toContainText('Game finished.');
  await context.close();
});

test('Flappy renders and restarts on desktop and mobile', async ({ browser }) => {
  mkdirSync(path.join(root, 'build/web-qa'), { recursive: true });
  for (const mobile of [false, true]) {
    const context = await browser.newContext(mobile ? { viewport: { width: 390, height: 844 }, isMobile: true, hasTouch: true } : { viewport: { width: 1280, height: 900 } });
    const page = await context.newPage();
    const errors = [];
    page.on('pageerror', error => errors.push(error.message));
    await start(page, 'flappy');
    await expect.poll(() => page.locator('canvas').evaluate(canvas => new Set(canvas.getContext('2d').getImageData(0, 0, canvas.width, canvas.height).data).size)).toBeGreaterThan(16);
    await page.screenshot({ path: path.join(root, `build/web-qa/${mobile ? 'mobile' : 'desktop'}.png`), fullPage: true });
    if (mobile) {
      await page.getByRole('button', { name: 'Keyboard', exact: true }).tap();
      await page.screenshot({ path: path.join(root, 'build/web-qa/mobile-keyboard.png'), fullPage: true });
      await page.getByRole('button', { name: 'Keyboard', exact: true }).tap();
      await page.getByRole('button', { name: 'Joystick', exact: true }).tap();
      await page.screenshot({ path: path.join(root, 'build/web-qa/mobile-joystick.png'), fullPage: true });
      await page.getByLabel('Joystick mode').selectOption('mouse-arrows');
      await page.screenshot({ path: path.join(root, 'build/web-qa/mobile-dual-joysticks.png'), fullPage: true });
      await page.setViewportSize({ width: 844, height: 390 });
      await expect(page.locator('#joystick')).toHaveCSS('width', '96px');
      await expect.poll(async () => {
        const game = await page.locator('canvas').boundingBox();
        const controls = await page.locator('#touch-controls').boundingBox();
        return Math.abs(game.height + controls.height - 390);
      }).toBeLessThan(1);
      // Let the mobile compositor paint the new orientation before capturing it.
      await page.evaluate(() => new Promise(resolve => requestAnimationFrame(() => requestAnimationFrame(resolve))));
      await page.screenshot({ path: path.join(root, 'build/web-qa/mobile-landscape.png') });
    }
    if (mobile) await page.getByRole('button', { name: 'ESC', exact: true }).tap();
    else await page.keyboard.press('Escape');
    await expect(page.locator('#status')).toContainText('Game finished.');
    if (mobile) await page.locator('canvas').tap(); else await page.keyboard.press('Enter');
    await expect(page.locator('#status')).toHaveText('Game running.');
    expect(errors).toEqual([]);
    await context.close();
  }
});

test('game project paths export without name shortcuts', async ({ page }) => {
  const missing = spawnSync(cli, ['export', 'web', 'ninjix', '--output', path.join(directory, 'missing-project')], { cwd: directory, encoding: 'utf8' });
  expect(missing.status).toBe(1);
  expect(missing.stderr).toContain('No such file or directory');
  await start(page, 'ninjix');
  await expect(page).toHaveTitle('Ninjix · lcom-ng');
  await expect.poll(() => page.locator('canvas').evaluate(canvas => new Set(canvas.getContext('2d').getImageData(0, 0, canvas.width, canvas.height).data).size)).toBeGreaterThan(16);
  await expect(page.locator('button:visible, header, nav')).toHaveCount(0);
  await page.screenshot({ path: path.join(root, 'build/web-qa/ninjix.png') });
});

test('one command builds and serves the current project with automatic assets', async ({ page }) => {
  test.setTimeout(90_000);
  const child = spawn(cli, ['web', '--no-open'], { cwd: workspace, detached: true });
  let transcript = '';
  child.stdout.on('data', bytes => { transcript += bytes; });
  child.stderr.on('data', bytes => { transcript += bytes; });
  try {
    await expect.poll(() => transcript, { timeout: 60_000 }).toMatch(/Play: http:\/\/127\.0\.0\.1:\d+\//);
    const url = transcript.match(/Play: (http:\/\/127\.0\.0\.1:\d+\/)/)[1];
    observe(page);
    await page.goto(url);
    await expect.poll(() => pixel(page)).toEqual([17, 34, 51, 255]);
    await expect.poll(() => consoleText(page)).toContain('asset:browser asset loaded');
    expect(await page.locator('canvas').evaluate(async () => (await fetch('game.wasm')).headers.get('content-type'))).toBe('application/wasm');
  } finally {
    process.kill(-child.pid, 'SIGINT');
    await new Promise(resolve => child.once('close', resolve));
  }
});
