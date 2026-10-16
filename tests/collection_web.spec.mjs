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

test('square icon mosaic keeps staples plus two random highlights and shuffles without fetching galleries', async ({page}) => {
  await page.route('**/collection.json',route=>route.fulfill({json:{name:'Mosaic Fixture',games:games.map(game=>({...game,name:'Minixsweeper'}))}}));
  const shots = []; page.on('request', request => {if (request.url().includes('/shot-')) shots.push(request.url());});
  await page.setViewportSize({width:1440,height:1000}); await page.goto(base);
  await expect(page.locator('#games a[data-id]')).toHaveCount(24);
  const featuredIds=()=>page.locator('.featured .game-launch').evaluateAll(links=>links.map(link=>link.dataset.id).sort());
  await expect(page.locator('.featured')).toHaveCount(6);
  const staples=games.filter(game=>game.featured).map(game=>game.id),initial=await featuredIds();
  expect(staples.every(id=>initial.includes(id))).toBe(true);
  expect(initial.filter(id=>!staples.includes(id))).toHaveLength(2);
  for (const width of [1440, 390, 320]) {
    await page.setViewportSize({width,height:1000});
    await expect.poll(()=>page.locator('.game').evaluateAll(items=>items.every(item=>Math.abs(item.offsetWidth-item.offsetHeight)<2))).toBeTruthy();
    const featured = await page.locator('.featured').evaluateAll(items=>items.map(item=>item.getBoundingClientRect().top));
    expect(new Set(featured).size).toBe(6);
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
    await expect(page.locator('.featured')).toHaveCount(6);
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
  await page.getByRole('button',{name:'Games',exact:true}).click();await expect(surprise).toBeFocused();
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
  await page.keyboard.press('Control+k');
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


test('random highlights handle collections with fewer than two ordinary games',async({page})=>{
  let catalog;
  await page.route('**/collection.json',route=>route.fulfill({json:{name:'Small collection',games:catalog}}));
  for(const ordinaryCount of [0,1,2]) {
    catalog=games.slice(0,4+ordinaryCount);
    await page.goto(base);
    await expect(page.locator('.featured')).toHaveCount(catalog.length);
    await expect(page.locator('#games .game-launch')).toHaveCount(catalog.length);
    await page.getByRole('button',{name:'Shuffle games'}).click();
    await expect(page.locator('.featured')).toHaveCount(catalog.length);
    expect(await page.locator('#games .game-launch').evaluateAll(links=>new Set(links.map(link=>link.dataset.id)).size)).toBe(catalog.length);
  }
});


test('grid icons load near the viewport, follow Shuffle, and wait to fetch distant games',async({page})=>{
  const catalog=Array.from({length:120},(_,index)=>({...games[index%games.length],id:`lazy-${index}`,icon:`/icon-lazy-${index}.svg`,featured:index<4}));
  await page.setViewportSize({width:390,height:844});await page.emulateMedia({reducedMotion:'reduce'});
  await page.route('**/collection.json',route=>route.fulfill({json:{name:'Large library',games:catalog}}));
  const icons=new Set(),shots=[];
  page.on('request',request=>{const name=new URL(request.url()).pathname;if(name.startsWith('/icon-lazy-'))icons.add(name);if(name.startsWith('/shot-'))shots.push(name);});
  await page.goto(base);await expect(page.locator('.game')).toHaveCount(120);await page.waitForLoadState('networkidle');
  const visibleLoaded=()=>page.locator('.game').evaluateAll(items=>items.filter(item=>{
    const rect=item.getBoundingClientRect();return rect.bottom>0&&rect.top<innerHeight;
  }).every(item=>{const icon=item.querySelector('.game-icon');return icon.complete&&icon.naturalWidth>0&&getComputedStyle(icon).opacity==='1';}));
  await expect.poll(visibleLoaded).toBe(true);
  expect(icons.size).toBeLessThan(40);expect(shots).toEqual([]);
  const distant=page.locator('.game').last(),id=await distant.locator('.game-launch').getAttribute('data-id');
  const source=catalog.find(game=>game.id===id).icon;
  expect((await distant.boundingBox()).y).toBeGreaterThan(844*3);
  expect(icons.has(source)).toBe(false);
  await distant.scrollIntoViewIfNeeded();
  await expect.poll(()=>distant.locator('.game-icon').evaluate(icon=>icon.complete&&icon.naturalWidth>0)).toBe(true);
  expect(icons.has(source)).toBe(true);
  await page.locator('.identity').scrollIntoViewIfNeeded();
  await page.getByRole('button',{name:'Shuffle games'}).click();
  await expect.poll(visibleLoaded).toBe(true);await expect(page.locator('.game')).toHaveCount(120);
  expect(shots).toEqual([]);
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


test('game surroundings stay outside gameplay, reuse loaded icons, and stop on return',async({page})=>{
  await page.setViewportSize({width:1440,height:900});await page.goto(base);
  await expect.poll(()=>page.locator('.game-icon').evaluateAll(images=>images.filter(image=>image.complete&&image.naturalWidth).length)).toBeGreaterThan(4);
  const loaded=await page.locator('.game-icon').evaluateAll(images=>images.filter(image=>image.complete&&image.naturalWidth).map(image=>image.src));
  await page.locator('.game-launch').first().click();await expect(page.locator('#loading')).toBeHidden();
  const frame=page.frames().find(frame=>frame.url().includes('/games/'));
  const report=async(game,occupied=[])=>frame.evaluate(({game,occupied})=>parent.postMessage({type:'lcom-player-layout',width:innerWidth,height:innerHeight,game,occupied},location.origin),{game,occupied});
  await report({x:240,y:0,width:960,height:900});
  await expect(page.locator('#play')).toHaveCSS('background-color','rgb(16, 16, 33)');
  const rails=page.locator('.game-rail:not([hidden])');await expect(rails).toHaveCount(2);
  expect(await rails.evaluateAll(rails=>rails.every(rail=>{const r=rail.getBoundingClientRect();return r.right<=224||r.x>=1216;}))).toBe(true);
  expect(await rails.locator('img').evaluateAll(images=>images.every(image=>image.alt===''&&image.closest('[aria-hidden="true"]')))).toBe(true);
  const borrowed=await rails.locator('img').evaluateAll(images=>images.map(image=>image.src));
  expect(borrowed.every(src=>loaded.includes(src))).toBe(true);
  const track=rails.first().locator('.rail-track');
  const position=()=>track.evaluate(element=>getComputedStyle(element).transform);
  const before=await position();await expect.poll(position).not.toBe(before);
  await page.emulateMedia({reducedMotion:'reduce'});await expect(track).toHaveCSS('animation-name','none');
  // A control in the left gutter suppresses its decoration, without moving
  // the game or taking over any pointer input from the embedded player.
  await report({x:240,y:0,width:960,height:900},[{x:60,y:400,width:100,height:100}]);
  await expect(rails).toHaveCount(1);await expect(rails).toHaveCSS('pointer-events','none');
  await page.setViewportSize({width:390,height:844});
  await report({x:0,y:200,width:390,height:292});await expect(rails).toHaveCount(0);
  await page.getByRole('button',{name:'Games',exact:true}).click();
  await expect(page.locator('.game-rail img')).toHaveCount(0);
  await expect(page.locator('#library')).toBeVisible();
});

test('direct game links fetch a bounded set of decorations only when side space is available',async({browser})=>{
  for(const [width,height] of [[1440,900],[390,844]]) {
    const context=await browser.newContext({viewport:{width,height}}),page=await context.newPage();
    const icons=[];page.on('request',request=>{if(request.url().includes('/icon-'))icons.push(request.url());});
    await page.goto(base+'/#game=game-0');await expect(page.locator('iframe')).toBeVisible();
    await expect(page.locator('#loading')).toBeHidden();
    const frame=page.frames().find(frame=>frame.url().includes('/games/'));
    await frame.evaluate(()=>parent.postMessage({type:'lcom-player-layout',width:innerWidth,height:innerHeight,
      game:innerWidth>1000?{x:240,y:0,width:960,height:900}:{x:0,y:200,width:390,height:292},occupied:[]},location.origin));
    await expect(page.locator('.game-rail:not([hidden])')).toHaveCount(width>1000?2:0);
    expect(new Set(icons).size).toBeLessThanOrEqual(width>1000?9:1);
    await context.close();
  }
});
