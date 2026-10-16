import {test, expect} from '@playwright/test';
import {createServer} from 'node:http';
import {readFileSync} from 'node:fs';
import path from 'node:path';

const root = path.resolve(import.meta.dirname, '..');
const template = process.env.LCOM_COLLECTION_TEMPLATE || path.join(root, 'sdk/collection/web-template');
let server, base;
const games = Array.from({length: 24}, (_, index) => ({
  id: 'game-' + index, name: 'Game ' + String(index).padStart(2, '0'),
  url: `games/${index}/`, icon: `/icon-${index}.svg`,
  screenshots: ['/shot-red.svg', '/shot-blue.svg', '/shot-green.svg'],
  featured: index < 4, year: 2020, yearLabel: 'Updated 2020', downloadBytes: 1250000,
  repository: index === 0 ? 'https://github.com/example/game' : undefined,
}));
test.beforeAll(async () => {
  server = createServer((request, response) => {
    const name = new URL(request.url, 'http://localhost').pathname;
    if (name === '/collection.json') { response.setHeader('Content-Type', 'application/json'); response.end(JSON.stringify({name:'Mosaic Fixture',games})); return; }
    if (/^\/(?:icon-|shot-)/.test(name)) {
      response.setHeader('Content-Type', 'image/svg+xml');
      const color = name.includes('shot-') ? name.slice(6, -4) : '#c5ff62';
      response.end(`<svg xmlns="http://www.w3.org/2000/svg" width="400" height="300"><path fill="${color}" d="M0 0h400v300H0z"/></svg>`); return;
    }
    if (name.startsWith('/games/')) {
      response.setHeader('Content-Type', 'text/html');
      response.end('<!doctype html><title>Playable fixture</title><script>setTimeout(()=>parent.postMessage({type:"lcom-player-loading",phase:"ready"},location.origin),20)</script>'); return;
    }
    try {
      response.setHeader('Content-Type', name.endsWith('.js') ? 'text/javascript' : name.endsWith('.css') ? 'text/css' : 'text/html');
      let body = readFileSync(path.join(template, name === '/' ? 'index.html' : name));
      if (name === '/') body = body.toString().replaceAll('@NAME@', 'Mosaic Fixture');
      response.end(body);
    } catch { response.writeHead(404).end(); }
  });
  await new Promise(resolve => server.listen(0, '127.0.0.1', resolve));
  base = 'http://127.0.0.1:' + server.address().port;
});
test.afterAll(async () => new Promise(resolve => server.close(resolve)));

test('square icon mosaic spaces featured games and shuffles without fetching galleries', async ({page}) => {
  const shots = []; page.on('request', request => {if (request.url().includes('/shot-')) shots.push(request.url());});
  await page.setViewportSize({width:1440,height:1000}); await page.goto(base);
  await expect(page.locator('#games a[data-id]')).toHaveCount(24);
  for (const width of [1440, 390, 320]) {
    await page.setViewportSize({width,height:1000});
    await expect.poll(()=>page.locator('.game').evaluateAll(items=>items.every(item=>Math.abs(item.offsetWidth-item.offsetHeight)<2))).toBeTruthy();
    const featured = await page.locator('.featured').evaluateAll(items=>items.map(item=>item.getBoundingClientRect().top));
    expect(new Set(featured).size).toBe(4);
    expect(await page.evaluate(()=>document.documentElement.scrollWidth)).toBeLessThanOrEqual(width);
  }
  const order = () => page.locator('#games a[data-id]').evaluateAll(links=>links.map(link=>link.getAttribute('href')));
  const before = await order(); await page.getByRole('button',{name:'Shuffle games'}).click();
  expect(await order()).not.toEqual(before);
  expect(shots).toEqual([]);
});

test('hover opens a bounded wide preview and pushes consecutive screenshots across it', async ({page}) => {
  await page.setViewportSize({width:1280,height:800}); await page.goto(base);
  const card = page.locator('.game:not(.featured) a').first();
  const rest = await card.boundingBox(); await card.hover();
  await expect.poll(async()=> (await card.boundingBox()).width).toBeGreaterThan(rest.width*1.6);
  await expect.poll(async()=> (await card.boundingBox()).height).toBeGreaterThan(rest.height*1.05);
  const box = await card.boundingBox(); expect(box.x).toBeGreaterThanOrEqual(7); expect(box.x+box.width).toBeLessThanOrEqual(1273);
  await expect(card.locator('img[src="/shot-red.svg"]')).toHaveCount(1);
  // During a real push, both image rectangles straddle the window. A fade or an
  // instantaneous src swap cannot satisfy this observable geometry.
  await expect.poll(()=>card.evaluate(element=>{
    const a=element.querySelector('img[src="/shot-red.svg"]'),b=element.querySelector('img[src="/shot-blue.svg"]');
    if (!a||!b) return false;
    const box=element.getBoundingClientRect(),left=a.getBoundingClientRect(),right=b.getBoundingClientRect();
    return left.x < box.x-10 && right.x > box.x+10 && right.x < box.right-10;
  }),{timeout:7000,intervals:[40]}).toBeTruthy();
  await page.mouse.move(1,1); await page.waitForTimeout(400);
  expect((await card.boundingBox()).width).toBeCloseTo(rest.width,0);
});

test('search is modal, preserves context through play, and restores keyboard focus', async ({page}) => {
  await page.goto(base);
  const toggle=page.getByRole('button',{name:'Find a game',exact:true});await toggle.click();
  const dialog=page.getByRole('dialog');await expect(dialog).toBeVisible();
  const input=page.getByRole('searchbox');await expect(input).toBeFocused();
  await input.fill('no such game');await expect(dialog.getByText('No games match. Try another name.')).toBeVisible();
  await input.fill('07');await expect(dialog.locator('#search-results a')).toHaveCount(1);
  await expect(page.locator('#games a[data-id]')).toHaveCount(24);
  await page.keyboard.press('ArrowDown');await expect(dialog.locator('#search-results a')).toBeFocused();
  await page.keyboard.press('Enter');await expect(page.locator('#loading')).toBeHidden();
  await expect(page.locator('iframe')).toHaveCount(1);
  await page.getByRole('button',{name:'Games',exact:true}).click();await expect(dialog).toBeVisible();
  await expect(input).toHaveValue('07');await expect(dialog.locator('#search-results a')).toBeFocused();
  await page.keyboard.press('Escape');await expect(dialog).not.toBeVisible();await expect(toggle).toBeFocused();
});

test('keyboard previews survive scrolling a focused game into view', async ({page}) => {
  await page.setViewportSize({width:1000,height:650}); await page.goto(base);
  const last=page.locator('#games a[data-id]').last();
  await last.focus();
  await expect(last).toBeFocused();
  await expect.poll(()=>last.evaluate(link=>link.parentElement.classList.contains('preview-ready'))).toBeTruthy();
  await page.waitForTimeout(400);
  await expect(page.locator('.game').last().locator('.game-name')).toBeVisible();
  expect(await last.evaluate(link=>link.parentElement.classList.contains('previewing'))).toBeTruthy();
});

test('hover facts show a plain year and the repository opens without launching the game', async ({page}) => {
  await page.context().route('https://github.com/example/game', route=>route.fulfill({contentType:'text/html',body:'<title>Repository fixture</title>'}));
  await page.goto(base);
  const card=page.locator('.game').filter({has:page.locator('a[data-id="game-0"]')});
  await card.locator('a[data-id]').focus();
  const repo=card.getByRole('link',{name:'Repository for Game 00'});
  await expect(repo).toBeVisible();
  await expect(card.locator('.game-facts')).toHaveText('2020 · 1.3 MB');
  await page.keyboard.press('Tab'); await expect(repo).toBeFocused();
  const popupPromise=page.waitForEvent('popup'); await repo.click();
  const popup=await popupPromise; await popup.waitForLoadState();
  expect(popup.url()).toBe('https://github.com/example/game');
  expect(page.url()).toBe(base+'/');
  await expect(page.locator('iframe')).toHaveCount(0);
  await popup.close();
});

test('reduced motion keeps previews still and touch launches in one tap', async ({browser}) => {
  const context=await browser.newContext({reducedMotion:'reduce',viewport:{width:1000,height:800}});
  const page=await context.newPage();const shots=[];page.on('request',request=>{if(request.url().includes('/shot-'))shots.push(request.url());});
  await page.goto(base);await page.locator('#games a[data-id]').first().hover();
  await expect.poll(()=>shots.length).toBe(1);await page.waitForTimeout(3600);expect(shots).toHaveLength(1);await context.close();
  const phone=await browser.newContext({viewport:{width:390,height:844},isMobile:true,hasTouch:true});
  const mobile=await phone.newPage();await mobile.goto(base);await mobile.locator('#games a[data-id]').first().tap();
  await expect(mobile.locator('iframe')).toHaveCount(1);await expect(mobile.locator('#loading')).toBeHidden();await phone.close();
});
