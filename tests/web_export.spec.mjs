import { test, expect } from '@playwright/test';
import { execFileSync, spawn, spawnSync } from 'node:child_process';
import { createServer } from 'node:http';
import { mkdtempSync, mkdirSync, readFileSync, readdirSync, renameSync, rmSync, copyFileSync, cpSync, writeFileSync } from 'node:fs';
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
  const settings=path.join(directory,'lcom.json');
  writeFileSync(settings,JSON.stringify({name:'Sushi config fixture',icon:path.join(root,'docs/assets/android-icons/sushi-slash.png'),mobile:{orientation:'landscape',fullscreen:true,controls:{keyboard:false,mouseButtons:false,quickKeys:[],joystick:false,layout:'overlay'}}}));
  exportGame('tests/fixtures/web/native','configured',cli,'--config',settings);
  writeFileSync(settings,JSON.stringify({mobile:{controls:{keyboard:false,mouseButtons:false,quickKeys:[],joystick:false,layout:'overlay',pointer:'direct',cursor:{x:400,y:300,maxX:799,maxY:599}}}}));
  exportGame('tests/fixtures/web/native','configured-direct',cli,'--config',settings);
  writeFileSync(settings,JSON.stringify({name:'Move and aim',mobile:{controls:{keyboard:true,mouseButtons:true,quickKeys:['ESC'],joystick:false,dpad:{side:'left',keys:'wasd'},look:{side:'right',sensitivity:1.5},layout:'overlay',pointer:'move'},theme:{accent:'#ff8844',keyboardAccent:'#77cc99',button:'#203040',font:'mono'}}}));
  exportGame('tests/fixtures/web/native','configured-dual',cli,'--config',settings);

  exportGame('tests/fixtures/web/native', 'pause', cli, '--asset', 'assets', '--arg=--keep-open');
  exportGame('tests/fixtures/web/minix', 'legacy', cli, '--asset-root', '..', '--asset', 'minix/assets');
  exportGame('examples/flappy', 'flappy', cli, '--name', 'LCOM Bird');
  exportGame('examples/ninjix', 'ninjix', cli, '--name', 'Ninjix');
  exportGame('examples/community/Cast-10--LCOMProject/proj', 'minix-legends', cli);
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
  const collection=path.join(directory,'classroom');
  mkdirSync(collection);
  const projects=[];
  for (const [name,title] of [['one','First game'],['two','Second game']]) {
    const project=path.join(directory,name); cpSync(path.join(root,'tests/fixtures/web/native'),project,{recursive:true});
    writeFileSync(path.join(project,'lcom.json'),JSON.stringify({name:title,description:'Find matching tiles and beat your best score.',preview:path.join(root,'docs/assets/android-icons/sushi-slash.png'),icon:path.join(root,'docs/assets/android-icons/sushi-slash.png'),build:{assets:['assets'],environment:{LCOM_COLLECTION_TEST:'ready from profile'}},mobile:{fullscreen:true,controls:{quickKeys:['KP_ENTER','A','B','C','D','E','F','G','H','I'],keyboard:false,joystick:false,mouseButtons:false,layout:'overlay'}}}));
    projects.push('../'+name);
  }
  writeFileSync(path.join(collection,'lcom.collection.json'),JSON.stringify({name:'Classroom Arcade',projects}));
  execFileSync(installedCli,['export','web',collection,'--output',path.join(directory,'games','collection')],{stdio:'pipe'});
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

test('two players hold their pads and action buttons independently', async ({browser})=>{
  const context=await browser.newContext({viewport:{width:844,height:390},isMobile:true,hasTouch:true});
  const page=await context.newPage();
  await page.route('**/configured-dual/game.json',async route=>{
    const response=await route.fetch(),settings=await response.json();
    Object.assign(settings.mobile.controls,{keyboard:false,mouseButtons:false,look:false,dpad:false,quickKeys:[],pointer:'off',
      pads:[{side:'left',mode:'wasd',style:'dpad'},{side:'right',mode:'keypad',style:'dpad'}],
      actions:[{side:'left',label:'P1 Bomb',key:'SPACE'},{side:'right',label:'P2 Bomb',key:'KP0'}]});
    await route.fulfill({response,json:settings});
  });
  await start(page,'configured-dual');
  const buttons=[page.locator('.thumb-controls[data-side="left"] [data-direction="up"]'),page.locator('.thumb-controls[data-side="right"] [data-direction="up"]'),
    page.getByRole('button',{name:'P1 Bomb',exact:true}),page.getByRole('button',{name:'P2 Bomb',exact:true})];
  const points=[];
  for(const [i,button] of buttons.entries()) {
    const box=await button.boundingBox();expect(box.width).toBeGreaterThanOrEqual(44);expect(box.height).toBeGreaterThanOrEqual(44);
    points.push({x:box.x+box.width/2,y:box.y+box.height/2,id:i+1});
  }
  const cdp=await context.newCDPSession(page);
  await cdp.send('Input.dispatchTouchEvent',{type:'touchStart',touchPoints:points});
  for(const code of ['11','48','39','52']) await expect.poll(()=>consoleText(page)).toContain('key:'+code);
  expect(consoleText(page)).not.toContain('key:e0');
  await cdp.send('Input.dispatchTouchEvent',{type:'touchEnd',touchPoints:[points[0]]});
  await expect.poll(()=>consoleText(page)).toContain('key:91');
  expect(consoleText(page)).not.toContain('key:c8');
  await cdp.send('Input.dispatchTouchEvent',{type:'touchCancel',touchPoints:[]});
  for(const code of ['c8','b9','d2']) await expect.poll(()=>consoleText(page)).toContain('key:'+code);
  await context.close();
});

test('tap-to-click permits hover drags and cancels without selecting', async ({ browser }) => {
  const context=await browser.newContext({viewport:{width:844,height:390},isMobile:true,hasTouch:true});
  const page=await context.newPage();
  await page.route('**/configured-direct/game.json',async route=>{
    const response=await route.fetch(),settings=await response.json();
    settings.mobile.controls.touch='tap';
    settings.mobile.controls.tapDuration=200;
    settings.mobile.controls.mouseButtons=true;
    await route.fulfill({response,json:settings});
  });
  await start(page,'configured-direct');
  await expect.poll(()=>consoleText(page)).toContain('ready');
  const box=await page.locator('canvas').boundingBox();
  const point=(x,y)=>({x:box.x+x*box.width/800,y:box.y+y*box.height/600,id:1});
  const cdp=await context.newCDPSession(page);
  await cdp.send('Input.dispatchTouchEvent',{type:'touchStart',touchPoints:[point(100,200)]});
  await expect.poll(()=>mousePackets(page).length).toBeGreaterThan(0);
  expect(mousePackets(page).every(([flags])=>!(flags&1))).toBeTruthy();
  await cdp.send('Input.dispatchTouchEvent',{type:'touchMove',touchPoints:[point(600,350)]});
  await cdp.send('Input.dispatchTouchEvent',{type:'touchEnd',touchPoints:[]});
  await page.waitForTimeout(100);
  expect(mousePackets(page).every(([flags])=>!(flags&1))).toBeTruthy();
  await page.locator('canvas').tap({position:{x:box.width/2,y:box.height/2}});
  await expect(page.getByRole('button',{name:'Left click',exact:true})).toHaveAttribute('aria-pressed','true');
  await expect(page.getByRole('button',{name:'Left click',exact:true})).toHaveAttribute('aria-pressed','false');
  await expect.poll(()=>mousePackets(page).filter(([flags])=>flags&1).length).toBe(1);
  expect(mousePackets(page).at(-1)[0]&1).toBe(0);
  await cdp.send('Input.dispatchTouchEvent',{type:'touchStart',touchPoints:[point(300,350)]});
  await cdp.send('Input.dispatchTouchEvent',{type:'touchCancel',touchPoints:[]});
  await page.waitForTimeout(100);
  expect(mousePackets(page).filter(([flags])=>flags&1)).toHaveLength(1);
  await context.close();
});

test('hold-drag moves without selecting, then holds and releases in one gesture',async({browser})=>{
  const context=await browser.newContext({viewport:{width:844,height:390},isMobile:true,hasTouch:true});
  const page=await context.newPage();
  await page.route('**/configured-direct/game.json',async route=>{
    const response=await route.fetch(),settings=await response.json();settings.mobile.controls.touch='hold-drag';
    settings.mobile.controls.quickKeys=['ESC'];
    await route.fulfill({response,json:settings});
  });
  await start(page,'configured-direct');
  const mode=page.getByRole('button',{name:'Hold to drag',exact:true});
  await expect(mode.locator('svg')).toBeVisible();
  await expect(mode).toHaveText('');
  await expect(mode).toHaveAttribute('aria-pressed','true');
  expect((await mode.boundingBox()).width).toBe(44);
  const box=await page.locator('canvas').boundingBox();
  const point=(x,y)=>({x:box.x+x*box.width/800,y:box.y+y*box.height/600,id:1});
  const cdp=await context.newCDPSession(page);
  await cdp.send('Input.dispatchTouchEvent',{type:'touchStart',touchPoints:[point(100,200)]});
  await cdp.send('Input.dispatchTouchEvent',{type:'touchMove',touchPoints:[point(300,250)]});
  await page.waitForTimeout(250);
  await cdp.send('Input.dispatchTouchEvent',{type:'touchEnd',touchPoints:[]});
  await expect.poll(()=>mousePackets(page).length).toBeGreaterThan(0);
  expect(mousePackets(page).every(([flags])=>!(flags&1))).toBeTruthy();
  await cdp.send('Input.dispatchTouchEvent',{type:'touchStart',touchPoints:[point(100,200)]});
  await expect.poll(()=>mousePackets(page).at(-1)?.[0]&1).toBe(1);
  await cdp.send('Input.dispatchTouchEvent',{type:'touchMove',touchPoints:[point(600,350)]});
  await cdp.send('Input.dispatchTouchEvent',{type:'touchEnd',touchPoints:[]});
  await expect.poll(()=>mousePackets(page).at(-1)?.[0]&1).toBe(0);
  let x=400,y=300,selected=null,placed=null;
  for(const [flags,dx,dy] of mousePackets(page)) {
    x+=dx-((flags&16)?256:0);y-=dy-((flags&32)?256:0);
    if((flags&1) && !selected) selected=[x,y];
    if(!(flags&1) && selected) placed=[x,y];
  }
  expect(selected).toEqual([100,200]);expect(placed).toEqual([600,350]);
  const presses=mousePackets(page).filter(([flags])=>flags&1).length;
  await cdp.send('Input.dispatchTouchEvent',{type:'touchStart',touchPoints:[point(100,200)]});
  await cdp.send('Input.dispatchTouchEvent',{type:'touchCancel',touchPoints:[]});
  await page.waitForTimeout(250);
  expect(mousePackets(page).filter(([flags])=>flags&1)).toHaveLength(presses);
  await mode.tap();
  await expect(page.getByRole('button',{name:'Move only',exact:true})).toHaveAttribute('aria-pressed','false');
  await cdp.send('Input.dispatchTouchEvent',{type:'touchStart',touchPoints:[point(100,200)]});
  await page.waitForTimeout(250);
  await cdp.send('Input.dispatchTouchEvent',{type:'touchEnd',touchPoints:[]});
  expect(mousePackets(page).filter(([flags])=>flags&1)).toHaveLength(presses);
  await context.close();
});

test('corner actions and horizontal WASD pad expose only useful large targets',async({browser})=>{
  const context=await browser.newContext({viewport:{width:844,height:390},isMobile:true,hasTouch:true});
  const page=await context.newPage();
  await page.route('**/configured-dual/game.json',async route=>{
    const response=await route.fetch(),settings=await response.json();
    Object.assign(settings.mobile.controls,{look:false,mouseButtons:false,quickKeys:[],toolbar:'top-left',keyboardPosition:'left',
      dpad:{side:'left',keys:'wasd',axes:'horizontal'},actions:[{position:'top-left',label:'Start',key:'ENTER'},{position:'top-right',label:'Menu',key:'ESC'},{position:'bottom-right',label:'Fire',key:'SPACE'}]});
    await route.fulfill({response,json:settings});
  });
  await start(page,'configured-dual');
  await expect(page.locator('#dpad button')).toHaveCount(2);
  await expect(page.locator('#dpad [data-direction="left"]')).toHaveText('A');
  await expect(page.locator('#dpad [data-direction="right"]')).toHaveText('D');
  const targets=await page.locator('#dpad button').evaluateAll(buttons=>buttons.map(b=>b.getBoundingClientRect().toJSON()));
  expect(targets.every(r=>r.width>=52 && r.height>=52)).toBeTruthy();
  const menu=await page.getByRole('button',{name:'Menu',exact:true}).boundingBox();
  const fire=await page.getByRole('button',{name:'Fire',exact:true}).boundingBox();
  expect(menu.width).toBeLessThanOrEqual(80);
  expect(menu.width).toBeGreaterThanOrEqual(44);
  expect(menu.x).toBeGreaterThan(600);expect(menu.y).toBeLessThan(100);
  expect(fire.x).toBeGreaterThan(600);expect(fire.y+fire.height).toBeGreaterThan(330);
  const startButton=page.getByRole('button',{name:'Start',exact:true});
  const startRect=await startButton.boundingBox();
  for(const selector of ['#controls-toggle','#fullscreen','#control-toolbar']) {
    const rect=await page.locator(selector).boundingBox();
    if(rect) expect(Math.min(startRect.x+startRect.width,rect.x+rect.width)<=Math.max(startRect.x,rect.x) ||
      Math.min(startRect.y+startRect.height,rect.y+rect.height)<=Math.max(startRect.y,rect.y)).toBeTruthy();
  }
  await startButton.tap();
  await expect.poll(()=>consoleText(page)).toContain('key:1c');
  await page.getByRole('button',{name:'Fire',exact:true}).tap();
  await expect.poll(()=>consoleText(page)).toContain('key:39');
  await page.getByRole('button',{name:'Keyboard',exact:true}).tap();
  const keys=await page.locator('#keyboard').boundingBox();
  expect(keys.x).toBeLessThan(10);expect(keys.width).toBeLessThan(430);
  await context.close();
});

test('configured step keys repeat while held and stop on cancellation',async({browser})=>{
  const context=await browser.newContext({viewport:{width:844,height:390},isMobile:true,hasTouch:true});
  const page=await context.newPage();
  await page.route('**/configured-dual/game.json',async route=>{
    const response=await route.fetch(),settings=await response.json();settings.mobile.controls.repeatKeys=['W'];
    await route.fulfill({response,json:settings});
  });
  await start(page,'configured-dual');
  const box=await page.locator('#dpad [data-direction="up"]').boundingBox();
  const cdp=await context.newCDPSession(page);
  const makes=()=>[...consoleText(page).matchAll(/key:11\b/g)].length;
  await cdp.send('Input.dispatchTouchEvent',{type:'touchStart',touchPoints:[{id:1,x:box.x+box.width/2,y:box.y+box.height/2}]});
  await expect.poll(makes).toBeGreaterThanOrEqual(3);
  await cdp.send('Input.dispatchTouchEvent',{type:'touchCancel',touchPoints:[]});
  await expect.poll(()=>consoleText(page)).toContain('key:91');
  const released=makes();await page.waitForTimeout(250);expect(makes()).toBe(released);
  await context.close();
});

test('custom pad modifier directions release with the finger',async({browser})=>{
  const context=await browser.newContext({viewport:{width:844,height:390},isMobile:true,hasTouch:true});
  const page=await context.newPage();
  await page.route('**/configured-dual/game.json',async route=>{
    const response=await route.fetch(),settings=await response.json();
    Object.assign(settings.mobile.controls,{dpad:false,look:false,pads:[{side:'left',mode:'arrows',style:'dpad',keys:{up:'LSHIFT',left:'LCTRL',right:'LALT',down:'S'}}]});
    await route.fulfill({response,json:settings});
  });
  await start(page,'configured-dual');
  for(const [direction,make,release] of [['up','2a','aa'],['left','1d','9d'],['right','38','b8']]) {
    const button=page.locator(`.custom-dpad [data-direction="${direction}"]`);
    await button.tap();
    await expect.poll(()=>consoleText(page)).toContain('key:'+make);
    await expect.poll(()=>consoleText(page)).toContain('key:'+release);
    await expect(button).not.toHaveClass(/held/);
  }
  await context.close();
});

test('tap-hold translates piece and destination taps into a mouse drag',async({browser})=>{
  const context=await browser.newContext({viewport:{width:844,height:390},isMobile:true,hasTouch:true});
  const page=await context.newPage();
  await page.route('**/configured-direct/game.json',async route=>{
    const response=await route.fetch(),settings=await response.json();settings.mobile.controls.touch='tap-hold';settings.mobile.controls.mouseButtons=true;
    await route.fulfill({response,json:settings});
  });
  await start(page,'configured-direct');
  const box=await page.locator('canvas').boundingBox();
  const point=(x,y)=>({x:box.x+x*box.width/800,y:box.y+y*box.height/600,id:1});
  const cdp=await context.newCDPSession(page);
  await cdp.send('Input.dispatchTouchEvent',{type:'touchStart',touchPoints:[point(100,200)]});
  await cdp.send('Input.dispatchTouchEvent',{type:'touchMove',touchPoints:[point(300,250)]});
  await cdp.send('Input.dispatchTouchEvent',{type:'touchEnd',touchPoints:[]});
  await expect.poll(()=>mousePackets(page).length).toBeGreaterThan(0);
  expect(mousePackets(page).every(([flags])=>!(flags&1))).toBeTruthy();
  await page.locator('canvas').tap({position:{x:box.width*100/800,y:box.height*200/600}});
  await expect.poll(()=>mousePackets(page).at(-1)?.[0]&1).toBe(1);
  await page.locator('canvas').tap({position:{x:box.width*600/800,y:box.height*350/600}});
  await expect.poll(()=>mousePackets(page).at(-1)?.[0]&1).toBe(0);
  let x=400,y=300,selected=null,moved=null;
  for(const [flags,dx,dy] of mousePackets(page)) {
    x+=dx-((flags&16)?256:0);y-=dy-((flags&32)?256:0);
    if((flags&1) && !selected) selected=[x,y];
    if(!(flags&1) && selected) moved=[x,y];
  }
  expect(selected).toEqual([100,200]);expect(moved).toEqual([600,350]);
  await page.locator('canvas').tap({position:{x:box.width*100/800,y:box.height*200/600}});
  await expect.poll(()=>mousePackets(page).at(-1)?.[0]&1).toBe(1);
  await page.getByRole('button',{name:'Right click',exact:true}).tap();
  await expect.poll(()=>mousePackets(page).at(-1)?.[0]&7).toBe(0);
  await context.close();
});

test('direct touch lands before clicking, follows drags and ignores letterboxing', async ({ browser }) => {
  const context=await browser.newContext({viewport:{width:844,height:390},isMobile:true,hasTouch:true});
  const page=await context.newPage();
  await start(page,'configured-direct');
  await expect.poll(()=>consoleText(page)).toContain('ready');
  const positions=()=>{
    let x=400,y=300;
    return mousePackets(page).map(([flags,dx,dy])=>{
      x=Math.max(0,Math.min(799,x+dx-(flags&16?256:0)));
      y=Math.max(0,Math.min(599,y-dy+(flags&32?256:0)));
      return [x,y,flags&7];
    });
  };
  const box=await page.locator('canvas').boundingBox();
  const point=(x,y,id=1)=>({x:box.x+x*box.width/800,y:box.y+y*box.height/600,id});
  const cdp=await context.newCDPSession(page);
  await cdp.send('Input.dispatchTouchEvent',{type:'touchStart',touchPoints:[point(50,80)]});
  await expect.poll(()=>positions().at(-1)).toEqual([50,80,1]);
  expect(positions().filter(p=>p[2]===1)).toEqual([[50,80,1]]);
  await cdp.send('Input.dispatchTouchEvent',{type:'touchMove',touchPoints:[point(650,480)]});
  await expect.poll(()=>positions().at(-1)).toEqual([650,480,1]);
  await cdp.send('Input.dispatchTouchEvent',{type:'touchCancel',touchPoints:[]});
  await expect.poll(()=>positions().at(-1)).toEqual([650,480,0]);
  await page.locator('canvas').tap({position:{x:100*box.width/800,y:200*box.height/600}});
  await expect.poll(()=>positions().at(-1)).toEqual([100,200,0]);
  expect(positions().filter(p=>p[2]===1).at(-1)).toEqual([100,200,1]);
  const before=positions().length;
  await page.touchscreen.tap(10,195);
  await page.locator('canvas').focus();
  await page.keyboard.press('b');
  await expect.poll(()=>consoleText(page)).toContain('key:b0');
  expect(positions()).toHaveLength(before);
  await context.close();
});

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
  expect(await page.evaluate(() => document.pointerLockElement)).toBeNull();
  await expect(page.locator('canvas')).toHaveCSS('cursor', 'none');
  await page.mouse.move(500, 300);
  await page.keyboard.press('ControlRight');
  await expect(page.locator('canvas')).toHaveCSS('cursor', 'auto');
  await page.keyboard.down('ArrowLeft');
  await expect.poll(() => consoleText(page)).toContain('key:4b');
  await page.evaluate(() => window.dispatchEvent(new Event('blur')));
  await page.evaluate(() => window.dispatchEvent(new Event('focus')));
  await expect.poll(() => consoleText(page)).toContain('key:cb');
  await page.keyboard.up('ArrowLeft');
  await page.locator('canvas').click({ position: { x: 40, y: 40 } });
  expect(await page.evaluate(() => document.pointerLockElement)).toBeNull();
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
  await expect.poll(() => consoleText(page)).toContain('legacy save:winner');
  await expect.poll(() => consoleText(page)).toContain('legacy polling ready');
  await expect.poll(() => consoleText(page)).toContain('legacy common:42');
  await expect.poll(() => consoleText(page)).toContain('legacy asset:legacy asset loaded');
  await page.keyboard.press('a');
  await expect.poll(() => consoleText(page)).toContain('legacy:1e');
  await expect.poll(() => consoleText(page)).toContain('legacy:9e');
  await page.keyboard.press('Escape');
  await expect(page.locator('#status')).toContainText('Game finished.');
});

test('Escape stays in the game without locking or releasing the browser mouse', async ({ page }) => {
  await start(page, 'pause');
  await page.locator('canvas').click();
  expect(await page.evaluate(() => document.pointerLockElement)).toBeNull();
  await page.keyboard.press('Escape');
  await expect.poll(() => pixel(page)).toEqual([0, 0, 255, 255]);
  await expect.poll(() => consoleText(page)).toContain('key:81');
  expect(consoleText(page).match(/key:01/g)).toHaveLength(1);
  await expect(page.locator('#status')).toHaveText('Game running.');
  await expect(page.locator('canvas')).toHaveCSS('cursor', 'none');
  const previous = consoleText(page).match(/mouse:/g)?.length || 0;
  await page.mouse.move(500, 300);
  await expect.poll(() => consoleText(page).match(/mouse:/g)?.length || 0).toBeGreaterThan(previous);
});

for (const [platform, release, guestModifier] of [['MacIntel', 'AltRight', 'MetaRight'], ['Win32', 'ControlRight', 'AltRight']]) {
  test(`${platform} mouse release stops guest motion until a click, preserving app modifiers`, async ({ page }) => {
    await page.addInitScript(platform => Object.defineProperty(navigator, 'platform', { get: () => platform }), platform);
    await start(page);
    await page.locator('canvas').click();
    await page.keyboard.press(release);
    await expect(page.locator('canvas')).toHaveCSS('cursor', 'auto');
    await page.keyboard.press(guestModifier);
    await expect.poll(() => consoleText(page)).toContain(guestModifier === 'MetaRight' ? 'key:dc' : 'key:b8');
    // Wait for a real guest keyboard acknowledgement before sampling its mouse log.
    const previous = consoleText(page).match(/mouse:/g)?.length || 0;
    await page.mouse.move(500, 300);
    await page.keyboard.press('b');
    await expect.poll(() => consoleText(page)).toContain('key:b0');
    expect(consoleText(page).match(/mouse:/g)?.length || 0).toBe(previous);
    await page.locator('canvas').click();
    await expect(page.locator('canvas')).toHaveCSS('cursor', 'none');
    await page.mouse.move(520, 320);
    await expect.poll(() => consoleText(page).match(/mouse:/g)?.length || 0).toBeGreaterThan(previous);
    expect(await page.evaluate(() => document.pointerLockElement)).toBeNull();
  });
}

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
  await cdp.send('Input.dispatchTouchEvent', { type: 'touchEnd', touchPoints: [{ ...arrowRight, x: arrows.x + arrows.width / 2, y: arrows.y + arrows.height / 2 }] });
  await page.getByLabel('Joystick mode').selectOption('wasd');
  const wasd = await page.locator('#joystick').boundingBox();
  await cdp.send('Input.dispatchTouchEvent', { type: 'touchStart', touchPoints: [{ x: wasd.x + wasd.width - 10, y: wasd.y + wasd.height / 2, id: 1 }] });
  await expect.poll(() => consoleText(page)).toContain('key:20');
  await cdp.send('Input.dispatchTouchEvent', { type: 'touchCancel', touchPoints: [] });
  await expect.poll(() => consoleText(page)).toContain('key:a0');
  await page.getByRole('button', { name: 'Joystick', exact: true }).tap();
  // Both orientations must leave room for the game and avoid page overflow.
  for (const viewport of [{ width: 390, height: 844 }, { width: 844, height: 390 }]) {
    await page.setViewportSize(viewport);
    await expect.poll(async () => {
      const area = await page.locator('main').boundingBox();
      const expectedHeight = Math.min(area.width * 3 / 4, area.height);
      return Math.abs((await page.locator('canvas').boundingBox()).height - expectedHeight);
    }).toBeLessThan(1);
    await expect.poll(() => page.evaluate(() => document.documentElement.scrollWidth <= innerWidth && document.documentElement.scrollHeight <= innerHeight)).toBe(true);
  }
  const side = await page.locator('#touch-controls').boundingBox();
  const gameArea = await page.locator('main').boundingBox();
  expect(side.x).toBeGreaterThanOrEqual(gameArea.x + gameArea.width);
  await page.getByRole('button', { name: 'Hide controls', exact: true }).tap();
  await expect(page.locator('#touch-controls')).toBeHidden();
  expect((await page.locator('main').boundingBox()).width).toBeGreaterThan(gameArea.width + 200);
  await page.getByRole('button', { name: 'Controls', exact: true }).tap();
  await expect(page.locator('#touch-controls')).toBeVisible();
  await page.setViewportSize({ width: 390, height: 844 });
  await expect(page.locator('#controls-toggle')).toBeVisible();
  await expect(page.locator('#touch-controls')).toBeVisible();
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
      await expect(page.getByRole('button', { name: 'Keyboard', exact: true })).toBeHidden();
      await expect(page.getByRole('button', { name: 'Joystick', exact: true })).toBeHidden();
      await expect(page.getByRole('button', { name: 'SPACE', exact: true })).toBeVisible();
      await page.setViewportSize({ width: 844, height: 390 });
      await expect.poll(async () => {
        const game = await page.locator('canvas').boundingBox();
        const available = await page.locator('main').boundingBox();
        return Math.abs(game.height - Math.min(available.width * 3 / 4, available.height));
      }).toBeLessThan(1);
      // Let the mobile compositor paint the new orientation before capturing it.
      await page.evaluate(() => new Promise(resolve => requestAnimationFrame(() => requestAnimationFrame(resolve))));
      await page.screenshot({ path: path.join(root, 'build/web-qa/mobile-landscape.png') });
      await page.getByRole('button', { name: 'Hide controls', exact: true }).tap();
      await page.evaluate(() => new Promise(resolve => requestAnimationFrame(() => requestAnimationFrame(resolve))));
      await page.screenshot({ path: path.join(root, 'build/web-qa/mobile-landscape-collapsed.png') });
      await page.getByRole('button', { name: 'Controls', exact: true }).tap();
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


test('project identity, fullscreen orientation and minimal touch profile survive export relocation', async ({ browser }) => {
  const context=await browser.newContext({viewport:{width:844,height:390},isMobile:true,hasTouch:true});
  const page=await context.newPage(); await start(page,'configured');
  await expect(page).toHaveTitle('Sushi config fixture · lcom-ng');
  await expect(page.locator('#touch-controls')).toBeHidden();
  await expect(page.locator('#keyboard-toggle')).toBeHidden();
  const area=await page.locator('main').boundingBox();
  expect(area.width).toBe(844); expect(area.height).toBe(390);
  const manifest=await (await page.request.get(`${base}/configured/manifest.webmanifest`)).json();
  expect(manifest.name).toBe('Sushi config fixture'); expect(manifest.display).toBe('fullscreen'); expect(manifest.orientation).toBe('landscape');
  expect((await page.request.get(`${base}/configured/${manifest.icons[0].src}`)).ok()).toBe(true);
  await expect(page.locator('link[rel="icon"]')).toHaveAttribute('href','./icon.png');
  await page.setViewportSize({width:390,height:844});
  await expect(page.locator('#rotation-hint')).toBeVisible();
  await expect(page.locator('#rotation-hint')).toHaveText('Rotate your device to landscape.');
  await context.close();
});

test('configured overlay places movement left and mouse aim right with independent cancellation and theme', async ({ browser }) => {
  const context=await browser.newContext({viewport:{width:844,height:390},isMobile:true,hasTouch:true});
  const page=await context.newPage(); await start(page,'configured-dual');
  await expect(page.locator('#pointer-toggle')).toBeHidden();
  const cdp=await context.newCDPSession(page);
  const dpad=await page.getByRole('group',{name:'Directional pad'}).boundingBox();
  const aim=await page.getByRole('group',{name:'Mouse joystick',exact:true}).boundingBox();
  expect(dpad.x).toBeLessThan(80); expect(aim.x).toBeGreaterThan(650);
  expect((await page.locator('main').boundingBox()).width).toBe(844);
  await expect(page.getByRole('button',{name:'Keyboard',exact:true})).toHaveCSS('background-color','rgb(32, 48, 64)');
  await expect(page.getByRole('button',{name:'Keyboard',exact:true})).toHaveCSS('font-family','ui-monospace, monospace');
  const button=await page.locator('#dpad [data-direction="left"]').boundingBox();
  const left={id:1,x:button.x+button.width/2,y:button.y+button.height/2};
  const look={id:2,x:aim.x+aim.width*.8,y:aim.y+aim.height*.2};
  await cdp.send('Input.dispatchTouchEvent',{type:'touchStart',touchPoints:[left]});
  await expect.poll(()=>consoleText(page)).toContain('key:1e');
  const before=mousePackets(page).length;
  await cdp.send('Input.dispatchTouchEvent',{type:'touchStart',touchPoints:[left,look]});
  await expect.poll(()=>mousePackets(page).length).toBeGreaterThan(before+2);
  expect(mousePackets(page).slice(before).every(([flags,dx,dy])=>(flags&3)===0 && dx>0 && dy>0)).toBe(true);
  await cdp.send('Input.dispatchTouchEvent',{type:'touchCancel',touchPoints:[]});
  await expect.poll(()=>consoleText(page)).toContain('key:9e');
  const stopped=mousePackets(page).length;
  await page.evaluate(()=>new Promise(resolve=>requestAnimationFrame(()=>requestAnimationFrame(resolve))));
  expect(mousePackets(page).length).toBe(stopped);
  await page.getByRole('button',{name:'Keyboard',exact:true}).tap();
  await expect(page.getByRole('region',{name:'On-screen keyboard'})).toBeVisible();
  await page.screenshot({path:path.join(root,'build/project-config-qa/configured-keyboard.png')});
  await page.getByRole('button',{name:'Keyboard',exact:true}).tap();
  await expect(page.getByRole('region',{name:'On-screen keyboard'})).toBeHidden();
  await expect(page.getByRole('group',{name:'Mouse joystick',exact:true})).toBeVisible();
  await expect(page.getByRole('group',{name:'Directional pad'})).toBeVisible();
  await context.close();
});


test('collection is responsive, forwards profiles, and discards game state on return', async ({ browser }) => {
  const context=await browser.newContext({viewport:{width:390,height:844},hasTouch:true,isMobile:true});
  const page=await context.newPage(); observe(page);
  await page.goto(`${base}/collection/`);
  await expect(page.getByRole('heading',{name:'Classroom Arcade'})).toBeVisible();
  const columns=()=>page.locator('#games').evaluate(el=>getComputedStyle(el).gridTemplateColumns.split(' ').length);
  expect(await columns()).toBe(2);
  await page.setViewportSize({width:1280,height:800});
  expect(await columns()).toBeGreaterThan(3);
  expect(await page.evaluate(() => document.documentElement.scrollWidth <= innerWidth)).toBeTruthy();
  expect(await page.locator('.game').evaluateAll(items => items.every(item => item.querySelector('.art').getBoundingClientRect().width <= item.getBoundingClientRect().width + 1))).toBeTruthy();
  await page.getByRole('button',{name:'Find a game',exact:true}).click();
  await page.getByRole('searchbox').fill('Second');
  await expect(page.getByRole('link',{name:'Play Second game'})).toBeVisible();
  await expect(page.getByRole('link',{name:'Play First game'})).toHaveCount(0);
  await page.getByRole('searchbox').fill('');
  await page.getByRole('link',{name:'Play First game'}).click();
  const frame=page.frameLocator('iframe');
  await expect(frame.locator('#status')).toHaveText('Game running.');
  await expect.poll(()=>consoleText(page)).toContain('environment:ready from profile');
  const targets=await frame.locator('#quick-keys .key').evaluateAll(keys=>keys.map(key=>{const box=key.getBoundingClientRect();return {x:box.x,y:box.y,w:box.width,h:box.height};}));
  expect(targets).toHaveLength(10);
  expect(targets.every(box=>box.w>=44 && box.h>=44)).toBeTruthy();
  expect(new Set(targets.map(box=>box.y)).size).toBeGreaterThan(1);
  await frame.getByRole('button',{name:'Keypad Enter',exact:true}).click();
  await expect.poll(()=>consoleText(page)).toContain('key:e0');
  await expect.poll(()=>consoleText(page)).toContain('key:1c');
  await expect(page.getByRole('button',{name:'Games',exact:true})).toBeVisible();
  const toggleBox=await frame.locator('#controls-toggle').boundingBox();
  const gameBox=await frame.locator('canvas').boundingBox();
  const overlap=(a,b)=>Math.max(0,Math.min(a.x+a.width,b.x+b.width)-Math.max(a.x,b.x))*Math.max(0,Math.min(a.y+a.height,b.y+b.height)-Math.max(a.y,b.y));
  await expect.poll(async()=>overlap(await page.locator('#back').boundingBox(),gameBox)).toBe(0);
  expect(overlap(await page.locator('#back').boundingBox(),toggleBox)).toBe(0);
  await frame.locator('#controls-toggle').click();
  await expect(frame.locator('#touch-controls')).toBeHidden();
  await expect(page.locator('iframe')).toHaveCount(1);
  expect(await page.evaluate(()=>document.fullscreenElement?.tagName)).toBe('HTML');
  expect(await page.locator('iframe').boundingBox()).toMatchObject({x:0,y:0,width:1280,height:800});
  await page.getByRole('button',{name:'Games',exact:true}).click();
  await expect(page.locator('iframe')).toHaveCount(0);
  await page.getByRole('link',{name:'Play Second game'}).click();
  await expect(frame.locator('#status')).toHaveText('Game running.');
  await page.goBack();
  await expect(page.getByRole('heading',{name:'Classroom Arcade'})).toBeVisible();
  await context.close();
});


test('collection loading supports cancellation, download failures, and retry to the first frame', async ({page}) => {
  await page.goto(`${base}/collection/`);
  const catalog = await (await page.request.get(`${base}/collection/collection.json`)).json();
  const [first, second] = catalog.games;
  await expect(page.getByRole('link',{name:'Play First game'}).locator('.game-icon')).toHaveAttribute('src',first.icon);
  let release, requested;
  const gate = new Promise(resolve => { release = resolve; });
  const started = new Promise(resolve => { requested = resolve; });
  let mode = 'hold';
  await page.route('**/collection/games/*/game.wasm', async route => {
    if(mode === 'hold') { requested(); await gate; }
    if(mode === 'fail') await route.fulfill({status:503,body:'Unavailable'});
    else { try { await route.continue(); } catch { /* The canceled frame no longer exists. */ } }
  });
  await page.getByRole('link',{name:'Play First game'}).click();
  await started;
  await expect(page.locator('#loading')).toBeVisible();
  await expect(page.locator('#load-title')).toHaveText('First game');
  await expect(page.locator('.load-logo')).toHaveAttribute('src',first.icon);
  await expect.poll(() => page.locator('.load-logo').evaluate(image => image.naturalWidth)).toBeGreaterThan(0);
  await expect(page.locator('#load-description')).toHaveText('Find matching tiles and beat your best score.');
  await expect(page.getByRole('progressbar',{name:'Game download'})).toBeVisible();
  // An unrelated same-origin window must not dismiss another frame's loader.
  await page.evaluate(() => window.postMessage({type:'lcom-player-loading',phase:'ready'},location.origin));
  await expect(page.locator('#loading')).toBeVisible();
  await page.getByRole('button',{name:'Games',exact:true}).click();
  await expect(page.locator('iframe')).toHaveCount(0);
  mode = 'fail'; release();
  await page.getByRole('link',{name:'Play Second game'}).click();
  await expect(page.locator('#load-title')).toHaveText('Second game');
  await expect(page.locator('.load-logo')).toHaveAttribute('src',second.icon);
  await expect(page.getByRole('button',{name:'Try again',exact:true})).toBeVisible();
  mode = 'run';
  await page.getByRole('button',{name:'Try again',exact:true}).click();
  await expect(page.locator('#loading')).toBeHidden();
  const frame = page.frameLocator('iframe');
  await expect.poll(() => pixel(frame)).toEqual([17,34,51,255]);
  await expect(frame.locator('#status')).toHaveText('Game running.');
  await page.getByRole('button',{name:'Games',exact:true}).click();
  await expect(page.getByRole('link',{name:'Play Second game'})).toBeFocused();
});

test('phone collection cards keep long titles clear of game icons', async ({page}) => {
  await page.setViewportSize({width:320,height:844});
  const title = 'Star Wars: Defend the Republic';
  await page.route('**/collection/collection.json', async route => {
    const catalog = await (await route.fetch()).json();
    catalog.games[0].name = title;
    await route.fulfill({json:catalog});
  });
  await page.goto(`${base}/collection/`);
  const card = page.getByRole('link',{name:'Play '+title,exact:true});
  await expect(card).toBeVisible();
  await page.evaluate(() => document.fonts.ready);
  const label = await card.locator('.game-name').boundingBox();
  const icon = await card.locator('.game-icon').boundingBox();
  expect(label.y+label.height).toBeLessThanOrEqual(icon.y);
});

test('failed player module downloads offer retry in collections and standalone exports', async ({page}) => {
  let fail = true;
  await page.route('**/loading.js*', route => fail ? route.abort('failed') : route.continue());
  await page.goto(`${base}/collection/`);
  await page.getByRole('link',{name:'Play First game',exact:true}).click();
  await expect(page.getByRole('button',{name:'Try again',exact:true})).toBeVisible();
  fail = false;
  await page.getByRole('button',{name:'Try again',exact:true}).click();
  await expect(page.locator('#loading')).toBeHidden();
  await expect.poll(() => pixel(page.frameLocator('iframe'))).toEqual([17,34,51,255]);
  fail = true;
  await page.goto(`${base}/fixture/`);
  await expect(page.getByRole('button',{name:'Reload',exact:true})).toBeVisible();
  fail = false;
  await page.getByRole('button',{name:'Reload',exact:true}).click();
  await expect.poll(() => pixel(page)).toEqual([17,34,51,255]);
});


test('steering wheel turns around its rim while pedals and physical keys remain independent', async ({browser}) => {
  const context=await browser.newContext({viewport:{width:844,height:390},isMobile:true,hasTouch:true});
  const page=await context.newPage();
  let wheelProfile={maxAngle:120,deadZone:8};
  await page.route('**/configured-dual/game.json', async route => {
    const response=await route.fetch(),settings=await response.json();
    Object.assign(settings.mobile.controls,{keyboard:false,mouseButtons:false,joystick:false,look:false,dpad:false,
      quickKeys:[],pointer:'off',pads:[{side:'left',mode:'wasd',style:'wheel',axes:'horizontal',...wheelProfile}],
      actions:[{position:'bottom-right',label:'Accelerate',key:'W',icon:'accelerator'},
               {position:'bottom-right',label:'Brake / reverse',key:'S',icon:'brake'}]});
    await route.fulfill({response,json:settings});
  });
  await start(page,'configured-dual');
  const wheel=page.getByRole('slider',{name:'Steering wheel',exact:true});
  await expect(wheel).toHaveAttribute('aria-valuenow','0');
  const bounds=await wheel.boundingBox();
  expect(bounds.width).toBeGreaterThanOrEqual(128);
  const rim=angle=>({id:1,x:bounds.x+bounds.width/2+Math.sin(angle*Math.PI/180)*bounds.width*.4,
    y:bounds.y+bounds.height/2-Math.cos(angle*Math.PI/180)*bounds.height*.4});
  const pedal=async(name,id)=>{const b=await page.getByRole('button',{name,exact:true}).boundingBox();
    expect(b.width).toBeGreaterThanOrEqual(44);expect(b.height).toBeGreaterThanOrEqual(44);
    return{id,x:b.x+b.width/2,y:b.y+b.height/2};};
  const gas=await pedal('Accelerate',2),brake=await pedal('Brake / reverse',3);
  const cdp=await context.newCDPSession(page);
  await cdp.send('Input.dispatchTouchEvent',{type:'touchStart',touchPoints:[rim(170)]});
  await expect(wheel).toHaveAttribute('aria-valuenow','0');
  await cdp.send('Input.dispatchTouchEvent',{type:'touchStart',touchPoints:[rim(170),gas]});
  await cdp.send('Input.dispatchTouchEvent',{type:'touchMove',touchPoints:[rim(190),gas]});
  await expect(wheel).toHaveAttribute('aria-valuenow','20');
  for(const code of ['11','20']) await expect.poll(()=>consoleText(page)).toContain('key:'+code);
  expect(consoleText(page)).not.toContain('key:1f');
  await cdp.send('Input.dispatchTouchEvent',{type:'touchStart',touchPoints:[rim(190),gas,brake]});
  await expect.poll(()=>consoleText(page)).toContain('key:1f');
  await cdp.send('Input.dispatchTouchEvent',{type:'touchEnd',touchPoints:[gas]});
  await expect.poll(()=>consoleText(page)).toContain('key:91');
  expect(consoleText(page)).not.toContain('key:9f');
  await cdp.send('Input.dispatchTouchEvent',{type:'touchMove',touchPoints:[rim(90),brake]});
  await expect(wheel).toHaveAttribute('aria-valuenow','-80');
  await expect.poll(()=>consoleText(page)).toContain('key:1e');
  await page.keyboard.down('a');
  await cdp.send('Input.dispatchTouchEvent',{type:'touchEnd',touchPoints:[rim(90)]});
  await expect(wheel).toHaveAttribute('aria-valuenow','0');
  expect(consoleText(page)).not.toContain('key:9e');
  await page.keyboard.up('a');
  await expect.poll(()=>consoleText(page)).toContain('key:9e');
  await cdp.send('Input.dispatchTouchEvent',{type:'touchCancel',touchPoints:[]});
  await expect.poll(()=>consoleText(page)).toContain('key:9f');
  // A second finger cannot seize an already held wheel.
  const other={...rim(0),id:4};
  await cdp.send('Input.dispatchTouchEvent',{type:'touchStart',touchPoints:[rim(0)]});
  await cdp.send('Input.dispatchTouchEvent',{type:'touchStart',touchPoints:[rim(0),other]});
  await cdp.send('Input.dispatchTouchEvent',{type:'touchMove',touchPoints:[rim(0),{...rim(90),id:4}]});
  await expect(wheel).toHaveAttribute('aria-valuenow','0');
  await cdp.send('Input.dispatchTouchEvent',{type:'touchMove',touchPoints:[rim(90),{...rim(90),id:4}]});
  await expect(wheel).toHaveAttribute('aria-valuenow','90');
  await cdp.send('Input.dispatchTouchEvent',{type:'touchMove',touchPoints:[rim(180),{...rim(90),id:4}]});
  await expect(wheel).toHaveAttribute('aria-valuenow','120');
  await cdp.send('Input.dispatchTouchEvent',{type:'touchCancel',touchPoints:[]});
  await expect(wheel).toHaveAttribute('aria-valuenow','0');
  await wheel.focus();await page.keyboard.down('ArrowLeft');
  await expect.poll(()=>consoleText(page).split('key:1e').length).toBeGreaterThan(2);
  await page.keyboard.up('ArrowLeft');await expect(wheel).toHaveAttribute('aria-valuenow','0');
  // Keyboard steering must cross even the largest configurable dead zone.
  wheelProfile={maxAngle:45,deadZone:30};
  await start(page,'configured-dual');await wheel.focus();
  await page.keyboard.down('ArrowLeft');
  await expect.poll(()=>consoleText(page)).toContain('key:1e');
  await expect(wheel).toHaveAttribute('aria-valuenow','-45');
  await page.keyboard.up('ArrowLeft');
  await expect(wheel).toHaveAttribute('aria-valuenow','0');
  await expect.poll(()=>consoleText(page)).toContain('key:9e');
  await context.close();
});


test('touch clicks can start disabled while hover, explicit actions and the toggle work', async ({browser}) => {
  const context=await browser.newContext({viewport:{width:844,height:390},isMobile:true,hasTouch:true});
  const page=await context.newPage();
  await page.route('**/configured-direct/game.json',async route=>{
    const response=await route.fetch(),settings=await response.json();
    Object.assign(settings.mobile.controls,{touch:'tap',touchClick:false,
      actions:[{position:'bottom-right',label:'Discover',mouseButton:'left'},
               {position:'bottom-right',label:'Flag',mouseButton:'right'}]});
    await route.fulfill({response,json:settings});
  });
  await start(page,'configured-direct');
  const toggle=page.locator('#pointer-toggle');
  await expect(toggle).toHaveAttribute('aria-pressed','false');
  const b=await page.locator('canvas').boundingBox();
  await page.touchscreen.tap(b.x+b.width*.4,b.y+b.height*.4);
  await expect.poll(()=>mousePackets(page).length).toBeGreaterThan(0);
  expect(mousePackets(page).every(([flags])=>(flags&3)===0)).toBe(true);
  await page.getByRole('button',{name:'Discover',exact:true}).tap();
  await expect.poll(()=>mousePackets(page).some(([flags])=>flags&1)).toBe(true);
  await page.getByRole('button',{name:'Flag',exact:true}).tap();
  await expect.poll(()=>mousePackets(page).some(([flags])=>flags&2)).toBe(true);
  await toggle.tap();await expect(toggle).toHaveAttribute('aria-pressed','true');
  const before=mousePackets(page).length;
  await page.touchscreen.tap(b.x+b.width*.6,b.y+b.height*.4);
  await expect.poll(()=>mousePackets(page).slice(before).some(([flags])=>flags&1)).toBe(true);
  await context.close();
});

test('a fast desktop mouse move preserves its full travel to the top of the game', async ({page}) => {
  await start(page);
  const b=await page.locator('canvas').boundingBox();
  await page.mouse.click(b.x+b.width/2,b.y+b.height/2);
  await page.waitForTimeout(80);
  output.get(page).length=0;
  await page.mouse.move(b.x+b.width/2,b.y+b.height*25/600);
  const travel=()=>mousePackets(page).reduce((sum,[flags,,dy])=>sum+((flags&32)?dy-256:dy),0);
  await expect.poll(travel).toBe(275);
});

test('Ninjix consumes mouse startup replies before following the cursor to the top', async ({page}) => {
  await start(page,'ninjix');
  const canvas=page.locator('canvas'),b=await canvas.boundingBox();
  const move=async(x,y)=>{await page.mouse.move(b.x+x*b.width/800,b.y+y*b.height/600);await page.waitForTimeout(180);};
  const pixels=()=>canvas.evaluate(c=>Array.from(c.getContext('2d').getImageData(0,0,220,120).data));
  for(const phase of ['menu','playing']) {
  if(phase==='playing') {await page.mouse.click(b.x+400*b.width/800,b.y+220*b.height/600);await page.waitForTimeout(350);}
  await move(100,300);const before=await pixels();
  await move(100,25);const after=await pixels();
  let minX=220,minY=120,changed=0;
  for(let y=0;y<120;y++)for(let x=0;x<220;x++) {
    const i=(y*220+x)*4;
    if(before[i]!==after[i]||before[i+1]!==after[i+1]||before[i+2]!==after[i+2]) {
      minX=Math.min(minX,x);minY=Math.min(minY,y);changed++;
    }
  }
  expect(changed).toBeGreaterThan(10);
  expect(minX).toBeGreaterThanOrEqual(95);expect(minX).toBeLessThanOrEqual(110);
  expect(minY,phase+' cursor').toBeGreaterThanOrEqual(20);expect(minY,phase+' cursor').toBeLessThanOrEqual(40);
  }
});

test('configured mouse actions preserve short taps, cancel immediately and keep independent holds', async ({browser}) => {
  const context=await browser.newContext({viewport:{width:844,height:390},isMobile:true,hasTouch:true});
  const page=await context.newPage();
  await page.route('**/configured-direct/game.json',async route=>{
    const response=await route.fetch(),settings=await response.json();
    Object.assign(settings.mobile.controls,{tapDuration:250,touch:'tap',touchClick:false,
      actions:[{position:'bottom-right',label:'Flip',mouseButton:'left'},
               {position:'bottom-right',label:'Cancel',mouseButton:'right'}]});
    await route.fulfill({response,json:settings});
  });
  await start(page,'configured-direct');
  const flip=page.getByRole('button',{name:'Flip',exact:true}),cancel=page.getByRole('button',{name:'Cancel',exact:true});
  const point=async(button,id)=>{const b=await button.boundingBox();return{id,x:b.x+b.width/2,y:b.y+b.height/2};};
  const left=await point(flip,1),right=await point(cancel,2),cdp=await context.newCDPSession(page);
  await cdp.send('Input.dispatchTouchEvent',{type:'touchStart',touchPoints:[left]});
  await cdp.send('Input.dispatchTouchEvent',{type:'touchEnd',touchPoints:[left]});
  await expect(flip).toHaveAttribute('aria-pressed','true');
  await expect(flip).toHaveAttribute('aria-pressed','false');
  await cdp.send('Input.dispatchTouchEvent',{type:'touchStart',touchPoints:[left]});
  await cdp.send('Input.dispatchTouchEvent',{type:'touchCancel',touchPoints:[]});
  await expect(flip).toHaveAttribute('aria-pressed','false');
  await cdp.send('Input.dispatchTouchEvent',{type:'touchStart',touchPoints:[left]});
  await cdp.send('Input.dispatchTouchEvent',{type:'touchStart',touchPoints:[left,right]});
  await cdp.send('Input.dispatchTouchEvent',{type:'touchEnd',touchPoints:[right]});
  await expect(cancel).toHaveAttribute('aria-pressed','false');
  await expect(flip).toHaveAttribute('aria-pressed','true');
  await page.waitForTimeout(280);
  await cdp.send('Input.dispatchTouchEvent',{type:'touchEnd',touchPoints:[left]});
  await expect(flip).toHaveAttribute('aria-pressed','false');
  await cdp.send('Input.dispatchTouchEvent',{type:'touchStart',touchPoints:[left]});
  await cdp.send('Input.dispatchTouchEvent',{type:'touchEnd',touchPoints:[left]});
  await page.locator('#controls-toggle').tap();
  await page.waitForTimeout(280);
  await expect(page.getByRole('button',{name:'Flip',exact:true,includeHidden:true})).toHaveAttribute('aria-pressed','false');
  await context.close();
});

// A guest using integer delta/2 needs even raw packets. Its logical position
// must survive focus release, canvas exit and re-entry, without stale deltas.
test('calibrated desktop mouse respects guest gain, bounds and re-entry', async ({page}) => {
  let scale=0.5;
  await page.route('**/configured-dual/game.json',async route=>{
    const response=await route.fetch(),settings=await response.json();
    Object.assign(settings.mobile.controls,{pointer:'move',cursor:{x:0,y:0,minX:0,minY:0,maxX:799,maxY:599,desktopScale:scale}});
    await route.fulfill({response,json:settings});
  });
  for (scale of [0.5,2]) {
    await start(page,'configured-dual');
    await page.locator('canvas').focus();
    const move=async(x,y)=>{
      await page.locator('canvas').evaluate((canvas,{x,y})=>{
        const r=canvas.getBoundingClientRect();
        canvas.dispatchEvent(new PointerEvent('pointermove',{pointerType:'mouse',clientX:r.left+x*r.width/canvas.width,clientY:r.top+y*r.height/canvas.height}));
      },{x,y});
      await page.waitForTimeout(250);
    };
    const position=()=>mousePackets(page).reduce(([x,y],packet)=>{
      const dx=packet[1]-(packet[0]&16?256:0),dy=packet[2]-(packet[0]&32?256:0);
      return [Math.max(0,Math.min(799,x+Math.trunc(dx*scale))),Math.max(0,Math.min(599,y-Math.trunc(dy*scale)))];
    },[0,0]);
    await move(100,550);
    expect(position()).toEqual([100,550]);
    await move(100,1000);expect(position()).toEqual([100,scale===2?598:599]);
    // Release/re-entry must retain the guest coordinate, not restart at centre.
    await page.locator('canvas').evaluate(canvas=>canvas.blur());
    const click=await page.locator('canvas').evaluate(canvas=>{const r=canvas.getBoundingClientRect();return {x:r.left+101*r.width/canvas.width,y:r.top+101*r.height/canvas.height};});
    await page.mouse.click(click.x,click.y);
    await page.waitForTimeout(250);
    expect(position()).toEqual(scale===2?[100,102]:[101,101]);
    await move(102,102);expect(position()).toEqual([102,102]);
  }
});

test('calibrated physical mouse follows touch travel and large cursor ranges', async ({browser}) => {
  const context=await browser.newContext({viewport:{width:844,height:390},isMobile:true,hasTouch:true});
  const page=await context.newPage();
  await page.route('**/configured-dual/game.json',async route=>{
    const response=await route.fetch(),settings=await response.json();
    Object.assign(settings.mobile.controls,{pointer:'move',touch:'move',cursor:{x:0,y:0,maxX:16000,maxY:16000,desktopScale:0.5}});
    await route.fulfill({response,json:settings});
  });
  await start(page,'configured-dual');
  const point=async(x,y)=>page.locator('canvas').evaluate((canvas,{x,y})=>{const r=canvas.getBoundingClientRect();return {x:r.left+x*r.width/canvas.width,y:r.top+y*r.height/canvas.height};},{x,y});
  const move=async(x,y)=>{const p=await point(x,y);await page.locator('canvas').dispatchEvent('pointermove',{pointerType:'mouse',clientX:p.x,clientY:p.y});await page.waitForTimeout(200);};
  const position=()=>mousePackets(page).reduce(([x,y],p)=>[x+Math.trunc((p[1]-(p[0]&16?256:0))/2),y-Math.trunc((p[2]-(p[0]&32?256:0))/2)],[0,0]);
  await move(100,100);expect(position()).toEqual([100,100]);
  const cdp=await context.newCDPSession(page),origin=await point(200,200),end=await point(240,220);
  await cdp.send('Input.dispatchTouchEvent',{type:'touchStart',touchPoints:[{...origin,id:1}]});
  await cdp.send('Input.dispatchTouchEvent',{type:'touchMove',touchPoints:[{...end,id:1}]});
  await cdp.send('Input.dispatchTouchEvent',{type:'touchEnd',touchPoints:[{...end,id:1}]});
  await page.waitForTimeout(200);expect(position()).toEqual([120,110]);
  await move(100,100);expect(position()).toEqual([100,100]);
  await move(6000,7000);expect(position()).toEqual([6000,7000]);
  await move(20,30);expect(position()).toEqual([20,30]);
  await context.close();
});

test('fractional guest cursors keep odd touch packets aligned with physical mouse', async ({browser}) => {
  const context=await browser.newContext({viewport:{width:844,height:390},isMobile:true,hasTouch:true});
  const page=await context.newPage();
  await page.route('**/configured-dual/game.json',async route=>{
    const response=await route.fetch(),settings=await response.json();
    Object.assign(settings.mobile.controls,{pointer:'move',touch:'move',cursor:{x:0,y:0,maxX:799,maxY:599,desktopScale:0.5,desktopFractional:true}});
    await route.fulfill({response,json:settings});
  });
  await start(page,'configured-dual');
  const point=async(x,y)=>page.locator('canvas').evaluate((canvas,{x,y})=>{const r=canvas.getBoundingClientRect();return {x:r.left+x*r.width/canvas.width,y:r.top+y*r.height/canvas.height};},{x,y});
  const move=async(x,y)=>{const p=await point(x,y);await page.locator('canvas').dispatchEvent('pointermove',{pointerType:'mouse',clientX:p.x,clientY:p.y});await page.waitForTimeout(200);};
  const position=()=>mousePackets(page).reduce(([x,y],p)=>[x+(p[1]-(p[0]&16?256:0))/2,y-(p[2]-(p[0]&32?256:0))/2],[0,0]);
  await move(100,100);expect(position()).toEqual([100,100]);
  const cdp=await context.newCDPSession(page);
  await cdp.send('Input.dispatchTouchEvent',{type:'touchStart',touchPoints:[{...await point(200,200),id:1}]});
  for(const n of [213,226]) {
    await cdp.send('Input.dispatchTouchEvent',{type:'touchMove',touchPoints:[{...await point(n,n),id:1}]});
    await page.waitForTimeout(100);
  }
  await cdp.send('Input.dispatchTouchEvent',{type:'touchEnd',touchPoints:[{...await point(226,226),id:1}]});
  await page.waitForTimeout(100);expect(position()).toEqual([113,113]);
  await move(100,100);expect(position()).toEqual([100,100]);
  await context.close();
});

test('Minix Legends enables mouse motion and keeps its whole cursor visible at edges', async ({page}) => {
  const strings=[...readFileSync(path.join(root,'examples/community/Cast-10--LCOMProject/proj/assets/cursor/cursor_xpms.h'),'utf8').matchAll(/"((?:\\.|[^"\\])*)"/g)].map(match=>JSON.parse('"'+match[1].replaceAll('\t','\\t')+'"'));
  const [width,height,colors,cpp]=strings[0].split(' ').map(Number);
  const transparent=new Set(strings.slice(1,1+colors).filter(line=>/\bc None$/.test(line)).map(line=>line.slice(0,cpp)));
  const mask=[];
  for(let y=0;y<height;y++)for(let x=0;x<width;x++)if(!transparent.has(strings[1+colors+y].slice(x*cpp,(x+1)*cpp)))mask.push([x,y]);
  await start(page,'minix-legends');
  const canvas=page.locator('canvas');
  const template=await canvas.evaluate((canvas,mask)=>{
    const d=canvas.getContext('2d').getImageData(0,0,canvas.width,canvas.height).data;
    return mask.map(([x,y])=>[x,y,...d.slice(((384+y)*canvas.width+512+x)*4,((384+y)*canvas.width+512+x)*4+3)]);
  },mask);
  expect(new Set(template.map(sample=>sample.slice(2).join(','))).size).toBeGreaterThan(10);
  for(const [x,y] of [[100,25],[0,0],[974,0],[974,716],[0,716],[100,25]]) {
    const point=await canvas.evaluate((c,{x,y})=>{const r=c.getBoundingClientRect();return {x:r.left+x*r.width/c.width,y:r.top+y*r.height/c.height};},{x,y});
    await page.mouse.move(point.x,point.y);
    await expect.poll(()=>canvas.evaluate((c,{x,y,template})=>{
      const d=c.getContext('2d').getImageData(0,0,c.width,c.height).data;
      return template.filter(([dx,dy,r,g,b])=>{const i=((y+dy)*c.width+x+dx)*4;return d[i]===r&&d[i+1]===g&&d[i+2]===b;}).length;
    },{x,y,template})).toBe(template.length);
  }
});
