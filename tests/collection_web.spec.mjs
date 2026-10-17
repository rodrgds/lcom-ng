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
  featured: index < 4, featuredPool: index >= 4, year: 2020, yearLabel: 'Updated 2020', downloadBytes: 1250000,
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
      response.setHeader('Content-Type', ({'.js':'text/javascript','.css':'text/css','.svg':'image/svg+xml','.ttf':'font/ttf'})[path.extname(name)] || 'text/html');
      let body = readFileSync(path.join(template, name === '/' ? 'index.html' : name));
      if (name === '/') body = body.toString().replaceAll('@NAME@', 'Mosaic Fixture');
      response.end(body);
    } catch { response.writeHead(404).end(); }
  });
  await new Promise(resolve => server.listen(0, '127.0.0.1', resolve));
  base = 'http://127.0.0.1:' + server.address().port;
});
test.afterAll(async () => new Promise(resolve => server.close(resolve)));

test('typing preserves matching result order and does not animate the grid on each key',async({page})=>{
  await page.goto(base);
  const input=page.getByRole('searchbox');
  await input.fill('Game');
  const order=()=>page.locator('#games .game-launch').evaluateAll(links=>links.map(link=>link.dataset.id));
  const before=await order();
  await input.fill('Game ');
  expect(await order()).toEqual(before);
  expect(await page.locator('#games .game').evaluateAll(items=>items.flatMap(item=>item.getAnimations()).length)).toBe(0);
  await input.fill('Game 0');
  expect(await order()).toEqual(before.filter(id=>Number(id.slice(5))<10));
});

test('square icon mosaic keeps staples plus five curated random highlights and shuffles without fetching galleries', async ({page}) => {
  await page.route('**/collection.json',route=>route.fulfill({json:{name:'Mosaic Fixture',games:games.map(game=>({...game,name:'Minixsweeper'}))}}));
  const shots = []; page.on('request', request => {if (request.url().includes('/shot-')) shots.push(request.url());});
  await page.setViewportSize({width:1440,height:1000}); await page.goto(base);
  await expect(page.locator('#games a[data-id]')).toHaveCount(24);
  const featuredIds=()=>page.locator('.featured .game-launch').evaluateAll(links=>links.map(link=>link.dataset.id).sort());
  await expect(page.locator('.featured')).toHaveCount(9);
  const staples=games.filter(game=>game.featured).map(game=>game.id),initial=await featuredIds();
  expect(staples.every(id=>initial.includes(id))).toBe(true);
  expect(initial.filter(id=>!staples.includes(id))).toHaveLength(5);
  for (const width of [1440, 390, 320]) {
    await page.setViewportSize({width,height:1000});
    await expect.poll(()=>page.locator('.game').evaluateAll(items=>items.every(item=>Math.abs(item.offsetWidth-item.offsetHeight)<2))).toBeTruthy();
    const featured = await page.locator('.featured').evaluateAll(items=>items.map(item=>item.getBoundingClientRect().top));
    expect(new Set(featured).size).toBe(9);
    expect(await featuredIds()).toEqual(initial);
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
  // Independent random inputs must choose different extra games. A shuffle
  // must also demote previous picks rather than accumulating large cards.
  const selections=[];
  for(const random of [.01,.99]) {
    await page.evaluate(value=>{Math.random=()=>value;},random);
    await page.getByRole('button',{name:'Shuffle games'}).click();
    await expect(page.locator('.featured')).toHaveCount(9);
    const current=await featuredIds();expect(staples.every(id=>current.includes(id))).toBe(true);
    selections.push(current.filter(id=>!staples.includes(id)));
    const ids=await page.locator('#games .game-launch').evaluateAll(links=>links.map(link=>link.dataset.id));
    expect(ids).toHaveLength(24);expect(new Set(ids).size).toBe(24);
    expect(await page.locator('.featured').evaluateAll(items=>{
      const normal=document.querySelector('.game:not(.featured)').getBoundingClientRect(),gap=12;
      return items.every(item=>Math.abs(item.getBoundingClientRect().width-(normal.width*2+gap))<2);
    })).toBe(true);
  }
  expect(selections[0]).not.toEqual(selections[1]);
  expect(shots).toEqual([]);
});

test('repeated archetypes pack nine playable games into four normal tile spaces', async ({page}, testInfo) => {
  // Keep twenty variants outside the curated pool, so two complete groups
  // remain while all five random highlights are selected independently.
  const catalog = Array.from({length:30},(_,index)=>({...games[index%games.length],
    id:'game-'+index,name:(index>=4&&index<24?'Space Invaders: Galactic Defense ':'Game ')+String(index).padStart(2,'0'),url:`games/${index}/`,
    featured:index<4,featuredPool:index>=24,archetype:index>=4&&index<24?'Snake':null}));
  await page.route('**/collection.json', route => route.fulfill({json: {name: 'Archetype fixture', games: catalog}}));
  await page.goto(base);
  await expect(page.locator('.game-cluster')).toHaveCount(2);
  for (const width of [1440, 768, 390, 320]) {
    await page.setViewportSize({width, height: 900});
    await expect.poll(() => page.locator('.game-cluster').first().evaluate(group => {
      const box = group.getBoundingClientRect(), tiles = [...group.querySelectorAll('.game')].map(tile => tile.getBoundingClientRect());
      return tiles.length === 9 && new Set(tiles.map(tile => Math.round(tile.x))).size === 3 &&
        new Set(tiles.map(tile => Math.round(tile.y))).size === 3 &&
        tiles.every(tile => Math.abs(tile.width - tile.height) < 2 && tile.width >= 48 && tile.right <= box.right + 1);
    })).toBeTruthy();
    const dimensions = await page.evaluate(() => {
      const normal = document.querySelector('.game:not(.featured):not(.game-cluster .game)').getBoundingClientRect();
      const group = document.querySelector('.game-cluster').getBoundingClientRect();
      return {normal: normal.width, group: group.width, gap: parseFloat(getComputedStyle(document.querySelector('#grid')).gap), overflow: document.documentElement.scrollWidth > innerWidth};
    });
    expect(dimensions.group).toBeCloseTo(dimensions.normal * 2 + dimensions.gap, 0);
    expect(dimensions.overflow).toBe(false);
    await page.screenshot({path: testInfo.outputPath(`archetypes-${width}.png`), fullPage: true});
  }
  await page.setViewportSize({width:1440,height:900});await page.emulateMedia({reducedMotion:'reduce'});
  await expect.poll(()=>page.locator('#games .game:not(.featured):not(.compact) .game-launch').first().evaluate(element=>element.getBoundingClientRect().width)).toBeLessThan(240);
  const normal=page.locator('#games .game:not(.featured):not(.compact) .game-launch').first();
  const ordinary=await normal.boundingBox();await normal.focus();
  await expect.poll(async()=>(await normal.boundingBox()).width).toBeGreaterThan(ordinary.width*1.6);
  const normalPreview=await normal.boundingBox();
  const compact=page.locator('.game-cluster .game-launch').first();await compact.focus();
  await expect.poll(async()=>(await compact.boundingBox()).width).toBeCloseTo(normalPreview.width,0);
  expect((await compact.boundingBox()).height).toBeCloseTo(normalPreview.height,0);
  expect(normalPreview.width).toBeGreaterThan(ordinary.width*2);expect(normalPreview.height).toBeGreaterThan(ordinary.height*1.3);
  const content=await compact.locator('..').locator('.preview-copy').boundingBox(),preview=await compact.boundingBox();
  expect(content.y).toBeGreaterThanOrEqual(preview.y+24);expect(content.y+content.height).toBeLessThanOrEqual(preview.y+preview.height);
  await page.screenshot({path:testInfo.outputPath('compact-preview-full-size.png')});
  await page.getByRole('button', {name: 'Shuffle games'}).click();
  await expect(page.locator('.game-cluster .game')).toHaveCount(18);
  const game = page.locator('.game-cluster .game-launch').first();
  await game.focus(); await expect(game).toBeFocused();
  await page.keyboard.press('Enter'); await expect(page.locator('iframe')).toHaveCount(1);await expect(page.locator('#loading')).toBeHidden();
  await page.locator('#player-fullscreen').focus();await page.keyboard.press('Shift+Escape'); await expect(game).toBeFocused();
  await page.getByRole('searchbox').fill('04');
  await expect(page.locator('#games .game-launch')).toHaveCount(1);
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

test('persistent search preserves context through play and restores keyboard focus', async ({page},testInfo) => {
  await page.goto(base);
  const input=page.getByRole('searchbox');await input.focus();
  await input.fill('no such game');await expect(page.locator('#filter-empty')).toBeVisible();
  await input.fill('07');await expect(page.locator('#games .game-launch')).toHaveCount(1);
  await page.keyboard.press('Enter');await expect(page.locator('#games .game-launch')).toBeFocused();
  await page.keyboard.press('Enter');await expect(page.locator('iframe')).toHaveCount(1);await expect(page.locator('#loading')).toBeHidden();
  const home=page.locator('#player-fullscreen');
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
  await page.keyboard.press('Shift+Escape');await expect(input).toBeVisible();
  await expect(input).toHaveValue('07');await expect(page.locator('#games .game-launch')).toBeFocused();

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
  await page.route('**/collection.json',route=>route.fulfill({json:{name:'Mosaic Fixture',games:games.map((game,index)=>index?game:{...game,academicYear:'2022/23',academicYearSource:{confidence:'inferred',detail:'Original game development history.'}})}}));
  await page.goto(base);
  const card=page.locator('.game').filter({has:page.locator('a[data-id="game-0"]')});
  await card.locator('a[data-id]').focus();
  const repo=card.getByRole('link',{name:'Repository for Game 00'});
  await expect(repo).toBeVisible();
  await expect(card.locator('.game-facts')).toHaveText('2022/23 · 1.3 MB');
  await expect(repo).toHaveText('');await expect(repo.locator('svg')).toBeVisible();
  expect((await repo.boundingBox()).width).toBeGreaterThanOrEqual(44);
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
  await game.click();await expect(page.locator('iframe')).toHaveCount(1);await expect(page.locator('#loading')).toBeHidden();
  await page.locator('#player-fullscreen').focus();await page.keyboard.press('Shift+Escape');
  await expect(game).toBeFocused();
  // Returning from the fixed player restores the library's scroll position.
  // A visible keyboard-focused preview must survive a later scroll event too.
  await page.evaluate(()=>new Promise(resolve=>{
    window.addEventListener('scroll',()=>requestAnimationFrame(resolve),{once:true});
    window.scrollBy(0,scrollY>0?-8:8);
  }));
  await game.hover({trial:true});
  const square=await card.evaluate(element=>element.getBoundingClientRect().width);
  await expect.poll(async()=>(await game.boundingBox()).width).toBeGreaterThan(square*1.6);
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

test('Surprise Me ticks through a longer spin, lands within a card and reveals a playable game', async ({page},testInfo) => {
  await page.addInitScript(()=>{
    Math.random=()=>.02;
    window.audioPulses=[];
    for(const Type of [OscillatorNode,AudioBufferSourceNode]) {
      const start=Type.prototype.start;Type.prototype.start=function(...args){window.audioPulses.push(performance.now());return start.apply(this,args);};
    }
  });
  const catalog=games.map(game=>({...game,description:'A quick round of '+game.name+'.'}));
  await page.route('**/collection.json',route=>route.fulfill({json:{name:'Roulette fixture',games:catalog}}));
  await page.setViewportSize({width:1280,height:800});await page.goto(base);
  const surprise=page.getByRole('button',{name:'Surprise me',exact:true});
  await page.evaluate(()=>document.querySelector('#surprise').addEventListener('click',()=>{
    window.reelFrames=[];
    const sample=()=>{
      const dialog=document.querySelector('#surprise-panel'),track=dialog.querySelector('.roulette-track'),marker=dialog.querySelector('.roulette-marker').getBoundingClientRect();
      const middle=marker.x+marker.width/2, cards=[...track.children];
      const index=cards.findIndex(card=>{const r=card.getBoundingClientRect();return r.left<=middle&&r.right>=middle;});
      window.reelFrames.push({time:performance.now(),index,x:track.getBoundingClientRect().x,height:dialog.getBoundingClientRect().height});
      if(dialog.open&&performance.now()-window.reelFrames[0].time<11000)requestAnimationFrame(sample);
    };requestAnimationFrame(sample);
  },{once:true}));
  const started=await page.evaluate(()=>performance.now());await surprise.click();
  const dialog=page.getByRole('dialog',{name:'Surprise me',exact:true});
  await expect(dialog).toBeVisible();
  await expect(dialog.getByRole('button')).toHaveCount(0);
  await expect(dialog.getByRole('heading')).toHaveCount(0);
  const rollingBox=await dialog.boundingBox(),track=dialog.locator('.roulette-track');
  const play=dialog.getByRole('button',{name:/^Play /});await expect(play).toBeVisible({timeout:11000});
  const elapsed=await page.evaluate(()=>performance.now())-started;
  expect(elapsed).toBeGreaterThan(7500);expect(elapsed).toBeLessThan(11000);
  const name=(await play.getAttribute('aria-label')).slice(5),winner=catalog.find(game=>game.name===name);expect(winner).toBeTruthy();
  await expect(dialog.getByRole('heading',{name:winner.name,exact:true})).toBeVisible();
  await expect(dialog.locator('#surprise-facts')).toHaveText('2020 · 1.3 MB');
  await expect(dialog.locator('#surprise-description')).toHaveText(winner.description);
  await expect(dialog.getByRole('link',{name:'Repository for '+winner.name})).toHaveAttribute('href',winner.repository);
  expect(winner.screenshots).toContain(await dialog.locator('.surprise-scene').getAttribute('src'));
  await expect.poll(async()=>(await dialog.boundingBox()).height).toBeGreaterThan(rollingBox.height*1.8);
  await page.waitForTimeout(600);
  const marker=await dialog.locator('.roulette-marker').boundingBox(),card=await track.locator('[data-selected="true"]').boundingBox();
  const landing=(marker.x+marker.width/2-card.x)/card.width;
  expect(landing).toBeGreaterThan(.04);expect(landing).toBeLessThan(.2);
  const observed=await page.evaluate(()=>({frames:reelFrames,pulses:audioPulses}));
  const nearest=time=>observed.frames.reduce((best,frame)=>Math.abs(frame.time-observed.frames[0].time-time)<Math.abs(best.time-observed.frames[0].time-time)?frame:best);
  const speed=(from,to)=>{const a=nearest(from),b=nearest(to);return (a.x-b.x)/(b.time-a.time);};
  expect(speed(1000,1200)).toBeGreaterThan(speed(50,250)*1.5);
  expect(speed(6000,6200)).toBeLessThan(speed(1000,1200)*.4);
  const passed=[...new Set(observed.frames.filter(frame=>frame.index>=0).map(frame=>frame.index))];
  const pulses=observed.pulses.filter((time,index,all)=>!index||time-all[index-1]>10);
  // Every card crossing has its own attack, even during the fast part. The
  // previous time throttle dropped audible ticks for roughly half the cards.
  expect(pulses.length).toBeGreaterThanOrEqual(passed.length);
  expect(pulses.length).toBeLessThanOrEqual(passed.length+4);
  const finalHeight=(await dialog.boundingBox()).height;
  expect(observed.frames.some(frame=>frame.height>rollingBox.height+15&&frame.height<finalHeight-15)).toBe(true);
  await testInfo.attach('roulette-motion-and-audio',{body:JSON.stringify(observed),contentType:'application/json'});
  await page.screenshot({path:testInfo.outputPath('surprise-result.png')});
  await play.click();await expect(page.locator('iframe')).toHaveAttribute('src',new RegExp(winner.url));
  await page.locator('#player-fullscreen').focus();await page.keyboard.press('Shift+Escape');await expect(surprise).toBeFocused();
  await surprise.click();await page.mouse.click(2,2);await expect(dialog).not.toBeVisible();
  const afterClose=await page.evaluate(()=>audioPulses.length);
  await page.waitForTimeout(9500);await expect(page.locator('iframe')).toHaveCount(0);await expect(surprise).toBeFocused();
  expect(await page.evaluate(()=>audioPulses.length)).toBe(afterClose);
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
  await page.getByRole('button',{name:'Filter games',exact:true}).click();
  await expect.poll(()=>page.evaluate(()=>audioRms()),{intervals:[10]}).toBeGreaterThan(.00001);
  expect(await page.evaluate(()=>audioContexts.length)).toBe(1);
  await page.keyboard.press('Escape');
  const starts=await page.evaluate(()=>audioStarts);
  await page.getByRole('button',{name:'Surprise me',exact:true}).click();
  await expect.poll(()=>page.evaluate(()=>audioStarts)).toBeGreaterThan(starts);
  await page.keyboard.press('Escape');
  await page.evaluate(()=>localStorage.setItem('lcom-arcade-sound','false'));
  await page.reload();await expect(page.locator('#games .game-launch')).toHaveCount(24);
  expect(await page.evaluate(()=>audioContexts.length)).toBe(0);
  await page.getByRole('button',{name:'Shuffle games',exact:true}).click();
  await expect.poll(()=>page.evaluate(()=>audioRms()),{intervals:[10]}).toBeGreaterThan(.00001);
  expect(errors).toEqual([]);
});

test('touch roulette preserves an off-center landing on resize with reduced motion and no Web Audio', async ({browser},testInfo) => {
  const context=await browser.newContext({viewport:{width:390,height:844},isMobile:true,hasTouch:true,reducedMotion:'reduce'});
  const page=await context.newPage(),errors=[];page.on('pageerror',error=>errors.push(error.message));
  await page.addInitScript(()=>{Math.random=()=>.98;Object.defineProperty(window,'AudioContext',{value:undefined});Object.defineProperty(window,'webkitAudioContext',{value:undefined});});
  await page.goto(base);
  await page.getByRole('button',{name:'Surprise me',exact:true}).tap();
  const dialog=page.getByRole('dialog',{name:'Surprise me',exact:true});
  await expect(dialog.getByRole('button',{name:/^Play /})).toBeVisible({timeout:1000});
  await expect(dialog.getByRole('button')).toHaveCount(1);
  const transform=await dialog.locator('.roulette-track').evaluate(element=>element.style.transform);
  await page.waitForTimeout(150);expect(await dialog.locator('.roulette-track').evaluate(element=>element.style.transform)).toBe(transform);
  const alignment=()=>dialog.evaluate(element=>{const card=element.querySelector('[data-selected]').getBoundingClientRect(),marker=element.querySelector('.roulette-marker').getBoundingClientRect();return (marker.x+marker.width/2-card.x)/card.width;});
  const landing=await alignment();expect(landing).toBeGreaterThan(.8);expect(landing).toBeLessThan(.96);
  await page.screenshot({path:testInfo.outputPath('surprise-phone.png')});
  await page.setViewportSize({width:844,height:390});await expect.poll(alignment).toBeCloseTo(landing,2);
  expect(await page.evaluate(()=>document.documentElement.scrollWidth)).toBeLessThanOrEqual(844);
  const box=await dialog.boundingBox();expect(box.y).toBeGreaterThanOrEqual(0);expect(box.y+box.height).toBeLessThanOrEqual(390);
  const button=dialog.getByRole('button',{name:/^Play /});
  const action=await button.boundingBox();expect(action.y+action.height).toBeLessThanOrEqual(box.y+box.height);
  expect(await button.evaluate(element=>{const rect=element.getBoundingClientRect();return element.contains(document.elementFromPoint(rect.x+rect.width/2,rect.y+rect.height/2));})).toBe(true);
  await page.screenshot({path:testInfo.outputPath('surprise-landscape.png')});
  await page.touchscreen.tap(2,2);await expect(dialog).not.toBeVisible();
  await page.getByRole('button',{name:'Surprise me',exact:true}).tap();
  await expect(dialog.getByRole('button',{name:/^Play /})).toBeVisible();expect(await alignment()).toBeCloseTo(landing,2);
  await page.keyboard.press('Escape');await expect(dialog).not.toBeVisible();expect(errors).toEqual([]);await context.close();
});


test('small collections use regular tiles without oversized highlights',async({page})=>{
  let catalog;
  await page.route('**/collection.json',route=>route.fulfill({json:{name:'Small collection',games:catalog}}));
  for(const ordinaryCount of [0,1,2]) {
    catalog=games.slice(0,4+ordinaryCount);
    await page.goto(base);
    await expect(page.locator('.featured')).toHaveCount(0);
    await expect(page.locator('#games .game-launch')).toHaveCount(catalog.length);
    await page.getByRole('button',{name:'Shuffle games'}).click();
    await expect(page.locator('.featured')).toHaveCount(0);
    expect(await page.locator('#games .game-launch').evaluateAll(links=>new Set(links.map(link=>link.dataset.id)).size)).toBe(catalog.length);
  }
});


test('grid icons load near the viewport, follow Shuffle, and wait to fetch distant games',async({page})=>{
  const catalog=Array.from({length:120},(_,index)=>({...games[index%games.length],id:`lazy-${index}`,icon:`/icon-full-${index}.svg`,iconThumbnail:`/icon-lazy-${index}.svg`,featured:index<4}));
  await page.setViewportSize({width:390,height:844});await page.emulateMedia({reducedMotion:'reduce'});
  await page.route('**/collection.json',route=>route.fulfill({json:{name:'Large library',games:catalog}}));
  const icons=new Set(),shots=[],fullIcons=[];
  page.on('request',request=>{const name=new URL(request.url()).pathname;if(name.startsWith('/icon-lazy-'))icons.add(name);if(name.startsWith('/shot-'))shots.push(name);if(name.startsWith('/icon-full-'))fullIcons.push(name);});
  await page.goto(base);await expect(page.locator('.game')).toHaveCount(120);await page.waitForLoadState('networkidle');
  const visibleLoaded=()=>page.locator('.game').evaluateAll(items=>items.filter(item=>{
    const rect=item.getBoundingClientRect();return rect.bottom>0&&rect.top<innerHeight;
  }).every(item=>{const icon=item.querySelector('.game-icon');return icon.complete&&icon.naturalWidth>0&&getComputedStyle(icon).opacity==='1';}));
  await expect.poll(visibleLoaded).toBe(true);
  expect(icons.size).toBeLessThan(40);expect(shots).toEqual([]);
  const distant=page.locator('.game').last(),id=await distant.locator('.game-launch').getAttribute('data-id');
  const source=catalog.find(game=>game.id===id).iconThumbnail;
  expect((await distant.boundingBox()).y).toBeGreaterThan(844*3);
  expect(icons.has(source)).toBe(false);
  await distant.scrollIntoViewIfNeeded();
  await expect.poll(()=>distant.locator('.game-icon').evaluate(icon=>icon.complete&&icon.naturalWidth>0)).toBe(true);
  expect(icons.has(source)).toBe(true);
  await page.locator('.identity').scrollIntoViewIfNeeded();
  await page.getByRole('button',{name:'Shuffle games'}).click();
  await expect.poll(visibleLoaded).toBe(true);await expect(page.locator('.game')).toHaveCount(120);
  expect(shots).toEqual([]);expect(fullIcons).toEqual([]);
});

for(const reduced of [false,true])test(`loaded icons ${reduced?'appear immediately with a working fallback':'fade in after decoding without moving their tiles'}`,async({page})=>{
  await page.emulateMedia({reducedMotion:reduced?'reduce':'no-preference'});
  let release;const gate=new Promise(resolve=>{release=resolve;});
  await page.route('**/icon-*.svg',async route=>{await gate;if(reduced)await route.fulfill({status:404,body:'missing'});else await route.continue();});
  try{
    await page.goto(base);await expect(page.locator('.game')).toHaveCount(24);
    const target=page.locator('.game').first().locator('.game-icon');
    await expect(target).toHaveCSS('opacity','0');const before=await page.locator('.game').first().boundingBox();
    await target.evaluate(icon=>{
      window.iconFrames=new Promise(resolve=>icon.addEventListener('load',()=>{
        const frames=[],start=performance.now();
        const sample=now=>{const style=getComputedStyle(icon);frames.push({time:now-start,opacity:Number(style.opacity)});if(now-start<500)requestAnimationFrame(sample);else resolve(frames);};
        requestAnimationFrame(sample);
      },{once:true}));
    });
    release();const frames=await page.evaluate(()=>window.iconFrames);
    expect(frames.at(-1).opacity).toBe(1);
    if(reduced){expect(frames.every(frame=>frame.opacity===0||frame.opacity===1)).toBe(true);await expect(target).toHaveAttribute('src','assets/arcade.svg');}
    else expect(frames.some(frame=>frame.opacity>.01&&frame.opacity<.99)).toBe(true);
    // The grid owns dimensions before and after loading, including failed art.
    const after=await page.locator('.game').first().boundingBox();expect(after.width).toBeCloseTo(before.width,0);expect(after.height).toBeCloseTo(before.height,0);
  }finally{release();}
});

test('desktop player offers a clickable library shortcut and fullscreen control',async({page},testInfo)=>{
  await page.setViewportSize({width:1440,height:900});await page.goto(base);
  const id=await page.locator('#games .game-launch').first().getAttribute('data-id'),tile=page.locator('#games .game-launch[data-id="'+id+'"]');await tile.focus();await page.keyboard.press('Enter');
  await expect(page.locator('iframe')).toHaveCount(1);await expect(page.locator('#loading')).toBeHidden();
  await page.screenshot({path:testInfo.outputPath('launcher-desktop-player.png')});
  await expect(page.locator('#back')).toBeVisible();
  const shortcut=page.getByRole('button',{name:'Games',exact:true});await expect(shortcut).toBeVisible();
  await expect(shortcut).toHaveAttribute('aria-keyshortcuts','Shift+Escape');
  const screen=page.locator('#player-fullscreen');await expect(screen).toHaveAccessibleName('Enter fullscreen');
  await screen.click();await expect(screen).toHaveAccessibleName('Exit fullscreen');
  expect(await page.evaluate(()=>!!document.fullscreenElement)).toBe(true);
  await expect(page.locator('#fullscreen')).toHaveAttribute('aria-label','Exit fullscreen');
  await screen.focus();await page.keyboard.press('Escape');await expect(page.locator('iframe')).toHaveCount(1);
  await shortcut.click();await expect(page.locator('iframe')).toHaveCount(0);await expect(tile).toBeFocused();
  await tile.press('Enter');await expect(page.locator('#loading')).toBeHidden();
  await screen.focus();await page.keyboard.press('Shift+Escape');
  await expect(page.locator('iframe')).toHaveCount(0);await expect(tile).toBeFocused();
});

test('touch player retains Home and an unobscured fullscreen control at phone and landscape widths',async({browser},testInfo)=>{
  const context=await browser.newContext({viewport:{width:390,height:844},isMobile:true,hasTouch:true});
  const page=await context.newPage();try{
    await page.goto(base);const id=await page.locator('#games .game-launch').first().getAttribute('data-id'),tile=page.locator('#games .game-launch[data-id="'+id+'"]');await tile.tap();await expect(page.locator('iframe')).toHaveCount(1);await expect(page.locator('#loading')).toBeHidden();
    const home=page.getByRole('button',{name:'Games',exact:true}),screen=page.locator('#player-fullscreen');
    // Desktop Chromium cannot resize its window while mobile launch fullscreen is active.
    await page.evaluate(async()=>{if(document.fullscreenElement)await document.exitFullscreen();});
    for(const viewport of [{width:390,height:844},{width:844,height:390}]){
      await page.setViewportSize(viewport);await expect(home).toBeVisible();await expect(screen).toBeVisible();await expect(page.locator('#library-hint')).toBeHidden();
      const a=await home.boundingBox(),b=await screen.boundingBox();expect(a.width).toBeGreaterThanOrEqual(44);expect(a.height).toBeGreaterThanOrEqual(44);expect(b.width).toBeGreaterThanOrEqual(44);expect(b.height).toBeGreaterThanOrEqual(44);
      expect(a.x+a.width<=b.x||b.x+b.width<=a.x||a.y+a.height<=b.y||b.y+b.height<=a.y).toBe(true);
      expect(b.x+b.width).toBeLessThanOrEqual(viewport.width);
      if(viewport.width===390)await page.screenshot({path:testInfo.outputPath('launcher-touch-player.png')});
    }
    const before=await page.evaluate(()=>!!document.fullscreenElement);await screen.tap();await expect.poll(()=>page.evaluate(()=>!!document.fullscreenElement)).toBe(!before);
    await home.tap();await expect(tile).toBeFocused();await expect(page.locator('iframe')).toHaveCount(0);
  }finally{await context.close();}
});

test('only the active same-origin player can return to the library and release a captured mouse',async({page})=>{
  await page.goto(base);const id=await page.locator('#games .game-launch').first().getAttribute('data-id'),tile=page.locator('#games .game-launch[data-id="'+id+'"]');await tile.click();await expect(page.locator('iframe')).toHaveCount(1);await expect(page.locator('#loading')).toBeHidden();
  await page.evaluate(()=>window.postMessage({type:'lcom-player-library'},location.origin));
  await expect(page.locator('iframe')).toHaveCount(1);
  const frame=page.frames().find(frame=>frame.url().includes('/games/'));
  await frame.evaluate(()=>{document.body.innerHTML='<button id="capture" style="position:absolute;left:200px;top:120px">Capture fixture mouse</button>';document.querySelector('#capture').onclick=()=>document.body.requestPointerLock();});
  await frame.locator('#capture').click();await expect.poll(()=>frame.evaluate(()=>!!document.pointerLockElement)).toBe(true);
  await frame.evaluate(()=>parent.postMessage({type:'lcom-player-library'},location.origin));
  await expect(page.locator('iframe')).toHaveCount(0);await expect(tile).toBeFocused();
  expect(await page.evaluate(()=>!!document.pointerLockElement)).toBe(false);
});

test('desktop loading failures keep a reachable cancel action and return focus to the launching tile',async({page})=>{
  await page.route('**/games/**',route=>route.fulfill({contentType:'text/html',body:'<script>parent.postMessage({type:"lcom-player-loading",phase:"error"},location.origin)</script>'}));
  await page.goto(base);const id=await page.locator('#games .game-launch').first().getAttribute('data-id'),tile=page.locator('#games .game-launch[data-id="'+id+'"]');await tile.click();
  await expect(page.getByRole('button',{name:'Try again',exact:true})).toBeFocused();
  await page.locator('#load-error').getByRole('button',{name:'Back to games',exact:true}).click();
  await expect(page.locator('iframe')).toHaveCount(0);await expect(tile).toBeFocused();
});

test('an active cross-origin frame cannot send the library shortcut message',async({page})=>{
  const external=base.replace('127.0.0.1','localhost');
  await page.route('**/collection.json',route=>route.fulfill({json:{name:'Origin fixture',games:[{...games[0],url:external+'/games/0/'}]}}));
  await page.goto(base);await page.locator('.game-launch').click();
  await expect.poll(()=>page.frames().find(frame=>frame.url().startsWith(external+'/games/'))?.url()).toBeTruthy();
  await page.frames().find(frame=>frame.url().startsWith(external+'/games/')).evaluate(()=>parent.postMessage({type:'lcom-player-library'},'*'));
  await expect(page.locator('#play')).toBeVisible();await expect(page.locator('iframe')).toHaveCount(1);
  await page.locator('#player-fullscreen').focus();await page.keyboard.press('Shift+Escape');
  await expect(page.locator('iframe')).toHaveCount(0);
});


test('side suggestions use two bright columns, preview and launch outside gameplay',async({page})=>{
  await page.setViewportSize({width:1440,height:900});await page.goto(base);
  await page.locator('#games .game-launch').first().click();await expect(page.locator('iframe')).toHaveCount(1);await expect(page.locator('#loading')).toBeHidden();
  const frame=page.frames().find(frame=>frame.url().includes('/games/'));
  const report=async(game,occupied=[])=>frame.evaluate(({game,occupied})=>parent.postMessage({type:'lcom-player-layout',width:innerWidth,height:innerHeight,game,occupied},location.origin),{game,occupied});
  await report({x:240,y:0,width:960,height:900});
  const rails=page.locator('.game-rail:not([hidden])');await expect(rails).toHaveCount(2);
  expect(await rails.evaluateAll(items=>items.every(item=>{const r=item.getBoundingClientRect();return r.right<=224||r.x>=1216;}))).toBe(true);
  expect(await rails.first().evaluate(rail=>Number(getComputedStyle(rail).opacity))).toBeGreaterThanOrEqual(.7);
  const choices=rails.first().getByRole('link',{name:/^Play /});await expect(choices.first()).toBeVisible();
  const positions=await choices.evaluateAll(links=>links.map(link=>Math.round(link.getBoundingClientRect().left)));expect(new Set(positions).size).toBe(2);
  const railBox=await rails.first().boundingBox();await page.mouse.move(railBox.x+2,railBox.y+2);
  const choice=choices.first(),id=await choice.getAttribute('data-id');await choice.hover();await expect(choice.locator('..')).toHaveClass(/previewing/);
  await expect(choice.locator('.preview-frame')).toBeVisible();await choice.click();await expect(page).toHaveURL(new RegExp('#game='+id+'$'));
  await expect(page.locator('iframe')).toHaveAttribute('title',games.find(game=>game.id===id).name);
  await expect(page.locator('#loading')).toBeHidden();
  const next=page.frames().find(frame=>frame.url().includes('/games/'));
  await page.setViewportSize({width:390,height:844});await next.evaluate(()=>parent.postMessage({type:'lcom-player-layout',width:390,height:844,game:{x:0,y:200,width:390,height:292},occupied:[]},location.origin));
  await expect(rails).toHaveCount(0);await page.locator('#player-fullscreen').focus();await page.keyboard.press('Shift+Escape');await expect(page.locator('.game-rail .game-launch')).toHaveCount(0);await expect(page.locator('#library')).toBeVisible();
});

test('direct game links fetch a bounded set of decorations only when side space is available',async({browser})=>{
  for(const [width,height] of [[1440,900],[390,844]]) {
    const context=await browser.newContext({viewport:{width,height}}),page=await context.newPage();
    const icons=[];page.on('request',request=>{if(request.url().includes('/icon-'))icons.push(request.url());});
    await page.goto(base+'/#game=game-0');await expect(page.locator('iframe')).toBeVisible();
    await expect(page.locator('iframe')).toHaveCount(1);await expect(page.locator('#loading')).toBeHidden();
    const frame=page.frames().find(frame=>frame.url().includes('/games/'));
    await frame.evaluate(()=>parent.postMessage({type:'lcom-player-layout',width:innerWidth,height:innerHeight,
      game:innerWidth>1000?{x:240,y:0,width:960,height:900}:{x:0,y:200,width:390,height:292},occupied:[]},location.origin));
    await expect(page.locator('.game-rail:not([hidden])')).toHaveCount(width>1000?2:0);
    expect(new Set(icons).size).toBeLessThanOrEqual(width>1000?49:1);
    await context.close();
  }
});

test('Home stays visible across physical keyboard and touch input',async({browser},testInfo)=>{
  const context=await browser.newContext({viewport:{width:390,height:844},isMobile:true,hasTouch:true});
  const page=await context.newPage();try{
    await page.goto(base);const id=await page.locator('#games .game-launch').first().getAttribute('data-id'),tile=page.locator('#games .game-launch[data-id="'+id+'"]');
    await tile.tap();await expect(page.locator('iframe')).toHaveCount(1);await expect(page.locator('#loading')).toBeHidden();
    const home=page.locator('#back'),hint=page.locator('#library-hint');await expect(home).toBeVisible();await expect(hint).toBeHidden();
    await page.evaluate(()=>new Promise(resolve=>{window.addEventListener('message',()=>requestAnimationFrame(resolve),{once:true});window.postMessage({type:'lcom-player-input',keyboard:true},location.origin);}));await expect(home).toBeVisible();
    const frame=page.frames().find(frame=>frame.url().includes('/games/'));
    const input=keyboard=>frame.evaluate(keyboard=>new Promise(resolve=>{
      const done=event=>{if(event.source===window&&event.data?.type==='lcom-player-input'){parent.removeEventListener('message',done);requestAnimationFrame(resolve);}};
      parent.addEventListener('message',done);parent.postMessage({type:'lcom-player-input',keyboard},location.origin);
    }),keyboard);
    await input(true);await expect(home).toBeVisible();await expect(hint).toBeHidden();
    await page.screenshot({path:testInfo.outputPath('hybrid-keyboard.png')});
    await input(0);await expect(hint).toBeHidden();
    await input(false);await expect(home).toBeVisible();await expect(hint).toBeHidden();
    await page.locator('#player-fullscreen').focus();await page.keyboard.press('A');await expect(home).toBeVisible();await expect(hint).toBeHidden();
    await input(false);await expect(home).toBeVisible();await expect(hint).toBeHidden();
    await page.screenshot({path:testInfo.outputPath('hybrid-touch.png')});
    await input(true);await frame.evaluate(()=>parent.postMessage({type:'lcom-player-library'},location.origin));await expect(tile).toBeFocused();
    await tile.tap();await expect(page.locator('iframe')).toHaveCount(1);await expect(page.locator('#loading')).toBeHidden();await expect(home).toBeVisible();
  }finally{await context.close();}
});

test('Home reapplies the latest child layout after keyboard use and rotation',async({browser})=>{
  const context=await browser.newContext({viewport:{width:390,height:844},hasTouch:true,isMobile:true});
  try{
    const page=await context.newPage();await page.goto(base);await page.locator('.game-launch').first().tap();
    await expect(page.locator('iframe')).toHaveCount(1);await expect(page.locator('#loading')).toBeHidden();
    const frame=page.frames().find(frame=>frame.url().includes('/games/')),home=page.locator('#back');
    const send=payload=>frame.evaluate(payload=>new Promise(resolve=>{
      const done=event=>{if(event.source===window&&event.data?.type===payload.type){parent.removeEventListener('message',done);requestAnimationFrame(resolve);}};
      parent.addEventListener('message',done);parent.postMessage(payload,location.origin);
    }),payload);
    await send({type:'lcom-player-layout',width:390,height:844,game:{x:0,y:120,width:390,height:312},occupied:[{x:0,y:600,width:390,height:244}]});
    await expect(home).toBeVisible();expect((await home.boundingBox()).y+44).toBeLessThanOrEqual(120);
    await send({type:'lcom-player-input',keyboard:true});await expect(home).toBeVisible();
    // Leave launch fullscreen before simulating device rotation through a desktop window resize.
    await page.evaluate(async()=>{if(document.fullscreenElement)await document.exitFullscreen();});
    await page.setViewportSize({width:844,height:390});
    const game={x:0,y:0,width:480,height:390},controls=[{x:500,y:0,width:344,height:52},{x:600,y:120,width:160,height:180}];
    await send({type:'lcom-player-layout',width:844,height:390,game,occupied:controls});
    await send({type:'lcom-player-input',keyboard:false});await expect(home).toBeVisible();
    const box=await home.boundingBox(),overlap=r=>Math.max(0,Math.min(box.x+box.width,r.x+r.width)-Math.max(box.x,r.x))*Math.max(0,Math.min(box.y+box.height,r.y+r.height)-Math.max(box.y,r.y));
    expect(overlap(game),'restored Home must avoid the current landscape game').toBe(0);
    expect(controls.reduce((sum,rect)=>sum+overlap(rect),0),'restored Home must avoid current controls').toBe(0);
    expect(box.x).toBeGreaterThanOrEqual(0);expect(box.y).toBeGreaterThanOrEqual(0);
    expect(box.x+box.width).toBeLessThanOrEqual(844);expect(box.y+box.height).toBeLessThanOrEqual(390);
  }finally{await context.close();}
});

test('genre, player and academic-year filters compose, preserve shuffle and reset',async({page})=>{
  const tagged=games.map((game,index)=>({...game,tags:[index%2?'racing':'space-shooter',index%4?'single-player':'local-versus'],academicYear:index<12?'2022/23':'2023/24'}));
  const taxonomy=[{id:'space-shooter',label:'Space shooters',facet:'genre'},{id:'racing',label:'Racing',facet:'genre'},{id:'local-versus',label:'Local 1v1',facet:'players'},{id:'single-player',label:'Solo',facet:'players'}];
  await page.route('**/collection.json',route=>route.fulfill({json:{name:'Mosaic Fixture',games:tagged,taxonomy}}));
  await page.goto(base);const filter=page.getByRole('button',{name:'Filter games',exact:true});await filter.click();
  const panel=page.locator('#browse-panel');await expect(panel).toBeVisible();
  await panel.getByRole('button',{name:'Space shooters',exact:true}).click();
  await expect(page.locator('#games .game-launch')).toHaveCount(12);
  await panel.getByRole('button',{name:'Local 1v1',exact:true}).click();
  await panel.getByRole('button',{name:'2022/23',exact:true}).click();
  await expect(page.locator('#games .game-launch')).toHaveCount(3);
  const visible=()=>page.locator('#games .game-launch').evaluateAll(links=>links.map(link=>link.dataset.id).sort());
  expect(await visible()).toEqual(['game-0','game-4','game-8']);
  await panel.getByRole('button',{name:'Close filters',exact:true}).click();
  await page.getByRole('button',{name:'Shuffle games'}).click();expect(await visible()).toEqual(['game-0','game-4','game-8']);
  await filter.click();await panel.getByRole('button',{name:'Solo',exact:true}).click();
  await expect(page.locator('#games .game-launch')).toHaveCount(6);
  await panel.getByRole('button',{name:'Clear filters',exact:true}).click();await expect(page.locator('#games .game-launch')).toHaveCount(24);
  // Clearing may disable the focused reset button. Escape still closes the open popover.
  await page.getByRole('searchbox').focus();
  await page.keyboard.press('Escape');await expect(panel).toBeHidden();await expect(filter).toBeFocused();
  await filter.click();await panel.getByRole('button',{name:'Space shooters',exact:true}).click();await page.keyboard.press('Escape');
  const chip=page.getByRole('button',{name:'Remove Space shooters',exact:true});await chip.focus();await page.keyboard.press('Enter');await expect(page.getByRole('searchbox')).toBeFocused();
  await page.getByRole('searchbox').fill('Game 0');const clear=page.getByRole('button',{name:'Clear search and filters',exact:true});await clear.focus();await page.keyboard.press('Enter');await expect(page.getByRole('searchbox')).toBeFocused();
});

test('startup is a compact dismissible modal over the library until a real ready message',async({page})=>{
  await page.route('**/games/**',route=>route.fulfill({contentType:'text/html',body:'<!doctype html><title>Waiting for first frame</title>'}));
  await page.setViewportSize({width:1440,height:900});await page.goto(base);const tile=page.locator('#games .game-launch').first(),name=(await tile.getAttribute('aria-label')).slice(5);await tile.click();
  const modal=page.getByRole('dialog',{name,exact:true});await expect(modal).toBeVisible();await expect(page.locator('#library')).toBeVisible();
  const rect=await modal.boundingBox();expect(rect.width).toBeLessThan(800);expect(rect.height).toBeLessThan(600);
  await page.keyboard.press('Escape');await expect(modal).toBeHidden();await expect(page.locator('iframe')).toHaveCount(0);await expect(tile).toBeFocused();
  await tile.click();await expect(modal).toBeVisible();const frame=page.frames().find(frame=>frame.url().includes('/games/'));
  await frame.evaluate(()=>parent.postMessage({type:'lcom-player-loading',phase:'ready'},location.origin));
  await expect(modal).toBeHidden();await expect(page.locator('#library')).toBeHidden();await expect(page.locator('iframe')).toBeFocused();
  await page.locator('#player-fullscreen').focus();await page.keyboard.press('Shift+Escape');await expect(page.locator('iframe')).toHaveCount(0);await expect(tile).toBeFocused();
});

test('search filters the library, persists with tags, and packs sparse results',async({page},testInfo)=>{
  const tagged=games.map((game,index)=>({...game,tags:[index<15?'snake':'racing'],featuredPool:index>=4&&index<16}));
  await page.route('**/collection.json',route=>route.fulfill({json:{name:'Search fixture',games:tagged,taxonomy:[{id:'snake',label:'Snake',facet:'genre'},{id:'racing',label:'Racing',facet:'genre'}]}}));
  await page.setViewportSize({width:1920,height:1000});await page.emulateMedia({reducedMotion:'reduce'});await page.goto(base);
  const input=page.getByRole('searchbox');await expect(input).toBeVisible();
  await input.fill('Game 0');await expect(page.locator('#games .game-launch')).toHaveCount(10);
  await page.getByRole('button',{name:'Filter games',exact:true}).click();await page.locator('#browse-panel').getByRole('button',{name:'Snake',exact:true}).click();
  await page.keyboard.press('Escape');await input.fill('Game 01');
  await expect(page.locator('#games .game-launch')).toHaveCount(1);
  const box=await page.locator('#games .game').boundingBox();expect(box.width).toBeLessThan(180);
  expect(box.x).toBeLessThan(600);expect(box.y).toBeLessThan(220);
  await page.reload();await expect(input).toHaveValue('Game 01');await expect(page.locator('#games .game-launch')).toHaveCount(1);
  await page.locator('#games .game-launch').click();await expect(page.locator('iframe')).toHaveCount(1);await expect(page.locator('#loading')).toBeHidden();await expect(page.locator('#library')).toBeHidden();
  await page.locator('#player-fullscreen').focus();await page.keyboard.press('Shift+Escape');
  await expect(page.locator('iframe')).toHaveCount(0);await expect(input).toBeVisible();await expect(input).toHaveValue('Game 01');await expect(page.locator('#games .game-launch')).toHaveCount(1);
  await input.fill('missing');await expect(page.locator('#filter-empty')).toBeVisible();
  await page.getByRole('button',{name:'Clear search and filters',exact:true}).click();
  await expect(input).toHaveValue('');await expect(page.locator('#games .game-launch')).toHaveCount(24);
  await page.screenshot({path:testInfo.outputPath('search-reset.png')});
});

test('five random featured picks come only from the curated pool',async({page})=>{
  const pool=games.slice(4,15).map(game=>game.id);
  await page.route('**/collection.json',route=>route.fulfill({json:{name:'Curated fixture',games:games.map(game=>({...game,featured:false,featuredPool:pool.includes(game.id)}))}}));
  await page.goto(base);
  for(const random of [.01,.99]) {
    await page.evaluate(value=>Math.random=()=>value,random);await page.getByRole('button',{name:'Shuffle games'}).click();
    const extras=await page.locator('.featured .game-launch').evaluateAll(links=>links.map(link=>link.dataset.id));
    expect(extras).toHaveLength(5);expect(extras.every(id=>pool.includes(id))).toBe(true);
  }
});

test('mouse capture shows only a brief centered shortcut hint and keeps Home visible',async({page},testInfo)=>{
  await page.goto(base);await page.locator('#games .game-launch').first().click();
  await expect(page.locator('iframe')).toHaveCount(1);await expect(page.locator('#loading')).toBeHidden();
  const home=page.locator('#back'),hint=page.locator('#library-hint');await expect(home).toBeVisible();await expect(hint).toBeHidden();
  const frame=page.frames().find(frame=>frame.url().includes('/games/'));
  const capture=captured=>frame.evaluate(captured=>parent.postMessage({type:'lcom-player-pointer',captured},location.origin),captured);
  await capture(true);await expect(hint).toBeHidden();await expect(hint).toBeVisible({timeout:2500});
  const box=await hint.boundingBox();expect(box.x+box.width/2).toBeCloseTo((await page.viewportSize()).width/2,0);expect(box.y).toBeGreaterThan((await page.viewportSize()).height*.8);
  await expect(home).toBeVisible();await page.screenshot({path:testInfo.outputPath('brief-centered-return-hint.png')});
  await expect(hint).toBeHidden({timeout:6000});await expect(home).toBeVisible();
  await capture(false);await capture(true);await capture(false);await page.waitForTimeout(1200);await expect(hint).toBeHidden();
  await home.click();await expect(page.locator('iframe')).toHaveCount(0);
});

test('Surprise and startup show game tags with a two-second minimum loading presentation',async({page})=>{
  const taxonomy=[{id:'platformer',label:'Platformer',facet:'genre'},{id:'single-player',label:'Solo',facet:'players'}];
  await page.route('**/collection.json',route=>route.fulfill({json:{name:'Tagged collection',taxonomy,games:games.map(game=>({...game,featured:false,tags:['platformer','single-player']}))}}));
  await page.emulateMedia({reducedMotion:'reduce'});await page.goto(base);
  await expect(page.getByText('Find your game',{exact:true})).toHaveCount(0);
  await expect(page.locator('#count')).toHaveText('24 games, ready to play');
  await page.getByRole('button',{name:'Surprise me',exact:true}).click();
  await expect(page.locator('#surprise-tags li')).toHaveText(['Platformer','Solo']);
  const started=Date.now();await page.locator('#surprise-play').click();
  await expect(page.locator('#loading')).toBeVisible();await expect(page.locator('#load-tags li')).toHaveText(['Platformer','Solo']);
  await page.waitForTimeout(1000);await expect(page.locator('#loading')).toBeVisible();
  await expect(page.locator('#loading')).toBeHidden();expect(Date.now()-started).toBeGreaterThanOrEqual(1950);
  await page.locator('#back').click();await expect(page.locator('iframe')).toHaveCount(0);
  await page.locator('#games .game-launch').first().click();await expect(page.locator('#loading')).toBeVisible();
  await page.locator('#load-dismiss').click();await page.waitForTimeout(2100);
  await expect(page.locator('iframe')).toHaveCount(0);await expect(page.locator('#library')).toBeVisible();await expect(page.locator('#loading')).toBeHidden();
});

test('side columns loop in alternating directions and pause for focus and reduced motion',async({page})=>{
  await page.setViewportSize({width:1920,height:900});await page.goto(base+'/#game=game-0');
  await expect(page.locator('#loading')).toBeHidden();
  const frame=page.frames().find(frame=>frame.url().includes('/games/'));
  await frame.evaluate(()=>parent.postMessage({type:'lcom-player-layout',width:1920,height:900,game:{x:480,y:0,width:960,height:900},occupied:[]},location.origin));
  const columns=page.locator('.game-rail:not([hidden]) .rail-column');
  await expect(columns).toHaveCount(8);
  const tracks=page.locator('.game-rail:not([hidden]) .rail-track');
  const movement=()=>tracks.evaluateAll(items=>items.map(item=>new DOMMatrixReadOnly(getComputedStyle(item).transform).m42));
  const settled=()=>tracks.first().evaluate(async track=>{for(const animation of track.closest('.game-rail').getAnimations({subtree:true}))await animation.ready;});
  const before=await movement();await page.waitForTimeout(250);const after=await movement();
  expect(after[0]).toBeLessThan(before[0]);expect(after[1]).toBeGreaterThan(before[1]);
  await columns.first().getByRole('link',{name:/^Play /}).first().focus();
  await settled();
  const paused=(await movement()).slice(0,4);await page.waitForTimeout(250);expect((await movement()).slice(0,4)).toEqual(paused);
  await page.locator('.game-rail').first().getByRole('button',{name:'Pause suggestions'}).click();
  await page.locator('#player-fullscreen').focus();
  await settled();
  const manual=(await movement()).slice(0,4);await page.waitForTimeout(250);expect((await movement()).slice(0,4)).toEqual(manual);
  const seam=await tracks.first().evaluate(track=>{
    const animation=track.getAnimations()[0],rail=track.closest('.game-rail').getBoundingClientRect();
    const visible=()=>[...track.querySelectorAll('.game')].map(tile=>({id:tile.querySelector('a').dataset.id,y:tile.getBoundingClientRect().y,height:tile.getBoundingClientRect().height})).filter(tile=>tile.y+tile.height/2>rail.y&&tile.y+tile.height/2<rail.bottom).map(tile=>({id:tile.id,y:Math.round(tile.y)}));
    animation.currentTime=.01;const first=visible();animation.currentTime=animation.effect.getTiming().duration-.01;return {first,last:visible()};
  });
  expect(seam.last).toEqual(seam.first);
  await frame.evaluate(()=>parent.postMessage({type:'lcom-player-layout',width:1920,height:900,game:{x:120,y:0,width:1680,height:900},occupied:[]},location.origin));
  await expect(page.locator('.game-rail:not([hidden]) .rail-column')).toHaveCount(2);
  const narrow=page.locator('.game-rail:not([hidden]) .rail-cycle').first().getByRole('link',{name:/^Play /}).first();
  await narrow.focus();
  await expect.poll(()=>narrow.evaluate(link=>link.getBoundingClientRect().width)).toBeLessThan(100);
  await page.emulateMedia({reducedMotion:'reduce'});
  expect(await tracks.evaluateAll(items=>items.every(item=>getComputedStyle(item).animationName==='none'))).toBe(true);
  const ids=await page.locator('.game-rail [id]').evaluateAll(items=>items.map(item=>item.id));
  expect(new Set(ids).size).toBe(ids.length);
});
