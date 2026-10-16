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
  await page.route('**/collection.json',route=>route.fulfill({json:{name:'Mosaic Fixture',games:games.map(game=>({...game,name:'Minixsweeper'}))}}));
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
  await page.setViewportSize({width:1440,height:1000});
  const spread=()=>page.locator('.featured').evaluateAll(items=>items.map(item=>{const b=item.getBoundingClientRect();return {x:b.x,right:b.right};}));
  await expect.poll(async()=>{const items=await spread();return items.filter(b=>b.x>32&&b.right<1408).length;},
    {message:'featured games include interior positions on a wide grid'}).toBeGreaterThan(0);
  expect(new Set((await spread()).map(b=>Math.round(b.x))).size).toBeGreaterThan(2);
  const order = () => page.locator('#games a[data-id]').evaluateAll(links=>links.map(link=>link.getAttribute('href')));
  const before = await order(); await page.getByRole('button',{name:'Shuffle games'}).click();
  expect(await order()).not.toEqual(before);
  for(let shuffle=0;shuffle<5;shuffle++) {
    await page.getByRole('button',{name:'Shuffle games'}).click();
    await expect.poll(async()=>new Set((await spread()).map(b=>Math.round(b.x))).size).toBeGreaterThan(2);
  }
  expect(shots).toEqual([]);
});

test('hover opens a bounded wide preview and pushes consecutive screenshots across it', async ({page},testInfo) => {
  await page.setViewportSize({width:1280,height:800}); await page.goto(base);
  const card = page.locator('.game:not(.featured) a').first();
  const rest = await card.boundingBox(); await card.hover();
  await expect.poll(async()=> (await card.boundingBox()).width).toBeGreaterThan(rest.width*1.6);
  await expect.poll(async()=> (await card.boundingBox()).height).toBeGreaterThan(rest.height*1.05);
  const box = await card.boundingBox(); expect(box.x).toBeGreaterThanOrEqual(7); expect(box.x+box.width).toBeLessThanOrEqual(1273);
  await expect(card.locator('img[src="/shot-red.svg"]')).toHaveCount(1);
  const progress=()=>card.locator('.preview-steps i').first().evaluate(bar=>new DOMMatrixReadOnly(getComputedStyle(bar,'::after').transform).a);
  expect(await progress()).toBeLessThan(.35);
  await expect.poll(progress,{timeout:4000,intervals:[30]}).toBeGreaterThan(.98);
  await card.screenshot({path:testInfo.outputPath('first-progress-full.png')});
  await page.mouse.move(1,1);await page.waitForTimeout(400);
  const started=await page.evaluate(()=>performance.now());await card.hover();
  await expect(card.locator('img[src="/shot-red.svg"]')).toHaveCount(1);
  expect(await progress()).toBeLessThan(.35);
  await card.screenshot({path:testInfo.outputPath('first-progress-restarted.png')});
  // During a real push, both image rectangles straddle the window. A fade or an
  // instantaneous src swap cannot satisfy this observable geometry.
  await expect.poll(()=>card.evaluate(element=>{
    const a=element.querySelector('img[src="/shot-red.svg"]'),b=element.querySelector('img[src="/shot-blue.svg"]');
    if (!a||!b) return false;
    const box=element.getBoundingClientRect(),left=a.getBoundingClientRect(),right=b.getBoundingClientRect();
    return left.x < box.x-10 && right.x > box.x+10 && right.x < box.right-10;
  }),{timeout:7000,intervals:[40]}).toBeTruthy();
  expect(await page.evaluate(()=>performance.now())-started).toBeLessThan(2900);
  await page.mouse.move(1,1); await page.waitForTimeout(400);
  expect((await card.boundingBox()).width).toBeCloseTo(rest.width,0);
});

test('search is modal, preserves context through play, and restores keyboard focus', async ({page},testInfo) => {
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
  const home=page.getByRole('button',{name:'Games',exact:true});
  await expect(home.locator('svg')).toHaveCSS('stroke','rgb(255, 255, 255)');
  await expect(home.locator('svg')).toHaveCSS('fill','none');
  await expect(home).toHaveCSS('background-color','rgb(34, 34, 34)');
  // A game may use light controls and dark text. Collection navigation keeps
  // its own contrasting colors when the actual child reports that theme.
  await page.frames().find(frame=>frame.url().includes('/games/')).evaluate(()=>parent.postMessage({
    type:'lcom-player-layout',width:innerWidth,height:innerHeight,
    game:{x:0,y:0,width:innerWidth,height:innerHeight},occupied:[],
    theme:{button:'#dddddd',text:'#111111',font:'mono'},
  },location.origin));
  await expect(home).toHaveCSS('background-color','rgb(34, 34, 34)');
  await expect(home.locator('svg')).toHaveCSS('stroke','rgb(255, 255, 255)');
  await home.screenshot({path:testInfo.outputPath('home-normal.png')});
  await home.hover();
  await expect(home.locator('svg')).toHaveCSS('stroke','rgb(255, 255, 255)');
  await home.screenshot({path:testInfo.outputPath('home-hover.png')});
  await page.keyboard.press('Shift+Tab');
  await expect(home).toBeFocused();
  await expect(home).toHaveCSS('outline-style','solid');
  await expect(home).toHaveCSS('outline-width','3px');
  await page.screenshot({path:testInfo.outputPath('home-focus.png')});
  await page.keyboard.press('Enter');await expect(dialog).toBeVisible();
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
  await page.evaluate(()=>new Promise(resolve=>{
    window.addEventListener('scroll',()=>requestAnimationFrame(resolve),{once:true});
    window.scrollBy(0,scrollY>0?-8:8);
  }));
  // Crossing another tile while aiming for a focused Repo link must not hide it.
  const neighbor=()=>page.locator('.game-launch').evaluateAll(links=>{
    for (const link of links) {
      if (link.dataset.id === 'game-0') continue;
      const box=link.getBoundingClientRect(),x=box.x+box.width/2,y=box.y+box.height/2;
      if (x>0 && x<innerWidth && y>0 && y<innerHeight && document.elementFromPoint(x,y)?.closest('.game-launch')===link) return {x,y,id:link.dataset.id};
    }
  });
  const target=await neighbor();expect(target).toBeTruthy();await page.mouse.move(target.x,target.y);
  const [popup]=await Promise.all([page.waitForEvent('popup'),repo.click({timeout:3000})]);
  await popup.waitForLoadState();
  expect(popup.url()).toBe('https://github.com/example/game');
  expect(page.url()).toBe(base+'/');
  await expect(page.locator('iframe')).toHaveCount(0);
  await popup.close();
  const game=card.locator('a[data-id]');
  await game.click();await expect(page.locator('#loading')).toBeHidden();
  await page.getByRole('button',{name:'Games',exact:true}).click();
  await expect(game).toBeFocused();await game.hover({trial:true});
  const square=await card.evaluate(element=>element.getBoundingClientRect().width);
  expect((await game.boundingBox()).width).toBeGreaterThan(square*1.6);
  const next=await neighbor();expect(next).toBeTruthy();
  const nextLink=page.locator(`a[data-id="${next.id}"]`),rest=await nextLink.boundingBox();
  await page.mouse.move(next.x,next.y);
  await expect.poll(async()=>(await nextLink.boundingBox()).width).toBeGreaterThan(rest.width*1.6);
  await expect.poll(async()=>(await game.boundingBox()).width).toBeCloseTo(square,0);
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

test('Surprise Me accelerates, settles on a real game, and restores its opener after play', async ({page},testInfo) => {
  await page.setViewportSize({width:1280,height:800});await page.goto(base);
  const surprise=page.getByRole('button',{name:'Surprise me',exact:true});
  await surprise.click();
  const dialog=page.getByRole('dialog',{name:'Surprise me',exact:true});
  await expect(dialog).toBeVisible();
  const track=dialog.locator('.roulette-track');
  const position=()=>track.evaluate(element=>element.getBoundingClientRect().x);
  const distance=async()=>{const before=await position();await page.waitForTimeout(200);return before-await position();};
  const early=await distance();await page.waitForTimeout(800);const middle=await distance();
  expect(middle).toBeGreaterThan(early*1.5);
  await page.waitForTimeout(2700);const late=await distance();expect(late).toBeLessThan(middle*.4);
  const play=dialog.getByRole('button',{name:/^Play /});await expect(play).toBeVisible();
  const name=(await play.getAttribute('aria-label')).slice(5),winner=games.find(game=>game.name===name);expect(winner).toBeTruthy();
  const marker=await dialog.locator('.roulette-marker').boundingBox();
  const card=await track.locator('[data-selected="true"]').boundingBox();
  expect(Math.abs(card.x+card.width/2-marker.x-marker.width/2)).toBeLessThan(2);
  await page.screenshot({path:testInfo.outputPath('surprise-result.png')});
  await play.click();await expect(page.locator('iframe')).toHaveAttribute('src',new RegExp(winner.url));
  await page.getByRole('button',{name:'Games',exact:true}).click();await expect(surprise).toBeFocused();
  await surprise.click();await dialog.getByRole('button',{name:'Skip animation',exact:true}).click();
  await expect(dialog.getByRole('button',{name:/^Play /})).toBeVisible();
  await dialog.getByRole('button',{name:'Spin again',exact:true}).click();
  await page.keyboard.press('Escape');await expect(dialog).not.toBeVisible();
  await page.waitForTimeout(5500);await expect(page.locator('iframe')).toHaveCount(0);await expect(surprise).toBeFocused();
});

test('real collection sounds start on interaction without a sound setting or autoplay', async ({page}) => {
  const errors=[];page.on('pageerror',error=>errors.push(error.message));
  await page.addInitScript(()=>{
    window.audioContexts=[];window.audioStarts=0;
    const Native=window.AudioContext;
    window.AudioContext=new Proxy(Native,{construct(target,args){const context=Reflect.construct(target,args);window.audioContexts.push(context);return context;}});
    for(const Type of [OscillatorNode,AudioBufferSourceNode]) {
      const start=Type.prototype.start;Type.prototype.start=function(...args){window.audioStarts++;return start.apply(this,args);};
    }
    const connect=AudioNode.prototype.connect;
    AudioNode.prototype.connect=function(destination,...args){
      if(destination===this.context.destination){const meter=this.context.createAnalyser();meter.fftSize=512;window.audioMeter=meter;connect.call(this,meter);connect.call(meter,destination);return destination;}
      return connect.call(this,destination,...args);
    };
    window.audioRms=()=>{if(!window.audioMeter)return 0;const data=new Float32Array(window.audioMeter.fftSize);window.audioMeter.getFloatTimeDomainData(data);return Math.sqrt(data.reduce((sum,n)=>sum+n*n,0)/data.length);};
  });
  await page.goto(base);await expect(page.locator('#games .game-launch')).toHaveCount(24);
  await expect(page.getByRole('button',{name:/UI sounds/})).toHaveCount(0);
  expect(await page.evaluate(()=>audioContexts.length)).toBe(0);
  await page.keyboard.press('Control+k');
  await expect.poll(()=>page.evaluate(()=>audioRms()),{intervals:[10]}).toBeGreaterThan(.00001);
  expect(await page.evaluate(()=>audioContexts.length)).toBe(1);
  await page.keyboard.press('Escape');
  const starts=await page.evaluate(()=>audioStarts);
  await page.getByRole('button',{name:'Surprise me',exact:true}).click();
  await expect.poll(()=>page.evaluate(()=>audioStarts)).toBeGreaterThan(starts);
  await page.getByRole('button',{name:'Skip animation',exact:true}).click();
  await page.getByRole('button',{name:'Close Surprise me',exact:true}).click();
  await page.evaluate(()=>localStorage.setItem('lcom-arcade-sound','false'));
  await page.reload();await expect(page.locator('#games .game-launch')).toHaveCount(24);
  expect(await page.evaluate(()=>audioContexts.length)).toBe(0);
  await page.getByRole('button',{name:'Shuffle games',exact:true}).click();
  await expect.poll(()=>page.evaluate(()=>audioRms()),{intervals:[10]}).toBeGreaterThan(.00001);
  expect(errors).toEqual([]);
});

test('touch roulette supports reduced motion, resize and missing Web Audio', async ({browser},testInfo) => {
  const context=await browser.newContext({viewport:{width:390,height:844},isMobile:true,hasTouch:true,reducedMotion:'reduce'});
  const page=await context.newPage(),errors=[];page.on('pageerror',error=>errors.push(error.message));
  await page.addInitScript(()=>{Object.defineProperty(window,'AudioContext',{value:undefined});Object.defineProperty(window,'webkitAudioContext',{value:undefined});});
  await page.goto(base);
  await page.getByRole('button',{name:'Surprise me',exact:true}).tap();
  const dialog=page.getByRole('dialog',{name:'Surprise me',exact:true});
  await expect(dialog.getByRole('button',{name:/^Play /})).toBeVisible({timeout:1000});
  const transform=await dialog.locator('.roulette-track').evaluate(element=>element.style.transform);
  await page.waitForTimeout(150);expect(await dialog.locator('.roulette-track').evaluate(element=>element.style.transform)).toBe(transform);
  const alignment=()=>dialog.evaluate(element=>{const card=element.querySelector('[data-selected]').getBoundingClientRect(),marker=element.querySelector('.roulette-marker').getBoundingClientRect();return Math.abs(card.x+card.width/2-marker.x-marker.width/2);});
  expect(await alignment()).toBeLessThan(2);
  await page.screenshot({path:testInfo.outputPath('surprise-phone.png')});
  await page.setViewportSize({width:844,height:390});await expect.poll(alignment).toBeLessThan(2);
  expect(await page.evaluate(()=>document.documentElement.scrollWidth)).toBeLessThanOrEqual(844);
  const box=await dialog.boundingBox();expect(box.y).toBeGreaterThanOrEqual(0);expect(box.y+box.height).toBeLessThanOrEqual(390);
  for(const button of [dialog.getByRole('button',{name:/^Play /}),dialog.getByRole('button',{name:'Spin again',exact:true})]) {
    const action=await button.boundingBox();expect(action.y+action.height).toBeLessThanOrEqual(box.y+box.height);
    expect(await button.evaluate(element=>{const rect=element.getBoundingClientRect();return element.contains(document.elementFromPoint(rect.x+rect.width/2,rect.y+rect.height/2));})).toBe(true);
  }
  await page.screenshot({path:testInfo.outputPath('surprise-landscape.png')});
  await dialog.getByRole('button',{name:'Spin again',exact:true}).tap();
  await expect(dialog.getByRole('button',{name:/^Play /})).toBeVisible();expect(await alignment()).toBeLessThan(2);
  await dialog.getByRole('button',{name:'Close Surprise me',exact:true}).tap();
  await expect(dialog).not.toBeVisible();expect(errors).toEqual([]);await context.close();
});
