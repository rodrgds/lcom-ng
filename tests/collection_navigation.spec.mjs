import {test, expect} from '@playwright/test';
import {createServer} from 'node:http';
import {readFileSync} from 'node:fs';
import path from 'node:path';

const template = process.env.LCOM_COLLECTION_TEMPLATE || path.resolve(import.meta.dirname, '../sdk/collection/web-template');
const games = Array.from({length:24}, (_, index) => ({
  id:`game-${index}`, name:`Game ${String(index).padStart(2,'0')}`, url:`games/${index}/`,
  icon:'/icon.svg', screenshots:['/icon.svg'], featured:index < 4,
  repository:'https://example.com/repository',
}));
let server, base;
test.beforeAll(async () => {
  server=createServer((request,response)=>{
    const name=new URL(request.url,'http://localhost').pathname;
    if(name==='/collection.json') {response.setHeader('Content-Type','application/json');response.end(JSON.stringify({name:'Navigation Fixture',games}));return;}
    if(name==='/icon.svg') {response.setHeader('Content-Type','image/svg+xml');response.end('<svg xmlns="http://www.w3.org/2000/svg" width="256" height="256"><path fill="#3a6" d="M0 0h256v256H0z"/></svg>');return;}
    if(name.startsWith('/games/')) {response.setHeader('Content-Type','text/html');response.end('<!doctype html><input aria-label="Guest input"><script>parent.postMessage({type:"lcom-player-loading",phase:"ready"},location.origin)</script>');return;}
    try {
      response.setHeader('Content-Type',name.endsWith('.js')?'text/javascript':name.endsWith('.css')?'text/css':'text/html');
      let body=readFileSync(path.join(template,name==='/'?'index.html':name));
      if(name==='/') body=body.toString().replaceAll('@NAME@','Navigation Fixture');
      response.end(body);
    } catch {response.writeHead(404).end();}
  });
  await new Promise(resolve=>server.listen(0,'127.0.0.1',resolve));
  base='http://127.0.0.1:'+server.address().port;
});
test.afterAll(async()=>new Promise(resolve=>server.close(resolve)));

test('search shortcuts open a compact dialog without taking typing or guest shortcuts',async({page})=>{
  await page.goto(base);
  const dialog=page.locator('#search-panel'), input=page.getByRole('searchbox');
  await page.keyboard.press('Control+k');await expect(dialog).toBeVisible();await expect(input).toBeFocused();
  await expect(dialog).toHaveAccessibleName('Find a game');
  await expect(dialog.locator('h1,h2,button')).toHaveCount(0);
  await input.fill('Game');await page.keyboard.press('/');await expect(input).toHaveValue('Game/');
  await page.keyboard.press('Meta+k');await expect(input).toBeFocused();await expect(input).toHaveValue('Game/');
  await page.keyboard.press('Escape');await expect(dialog).toBeHidden();
  await page.keyboard.press('Meta+k');await expect(dialog).toBeVisible();
  await page.mouse.click(2,2);await expect(dialog).toBeHidden();
  const surprise=page.getByRole('button',{name:'Surprise me',exact:true});
  await surprise.click();
  await page.keyboard.press('Control+k');await expect(dialog).toBeHidden();
  await page.keyboard.press('/');await expect(dialog).toBeHidden();
  await page.keyboard.press('Escape');
  await page.locator('#games .game-launch').first().click();
  const guest=page.frameLocator('iframe').getByRole('textbox',{name:'Guest input'});
  await guest.focus();await page.keyboard.press('Control+k');await page.keyboard.press('Meta+k');
  await expect(dialog).toBeHidden();await expect(guest).toBeFocused();
});

for(const width of [1280,390]) test(`search arrows follow the rendered ${width===1280?'two-column':'one-column'} grid`,async({page})=>{
  await page.setViewportSize({width,height:844});await page.goto(base);
  await page.keyboard.press('/');
  const input=page.getByRole('searchbox'), links=page.locator('#search-results a');
  await page.keyboard.press('ArrowDown');await expect(links.nth(0)).toBeFocused();
  if(width===1280) {
    await page.keyboard.press('ArrowRight');await expect(links.nth(1)).toBeFocused();
    await page.keyboard.press('ArrowDown');await expect(links.nth(3)).toBeFocused();
    await page.keyboard.press('ArrowLeft');await expect(links.nth(2)).toBeFocused();
    await page.keyboard.press('ArrowUp');await expect(links.nth(0)).toBeFocused();
    await page.keyboard.press('ArrowRight');await page.keyboard.press('ArrowUp');await expect(input).toBeFocused();
  } else {
    await page.keyboard.press('ArrowRight');await expect(links.nth(0)).toBeFocused();
    await page.keyboard.press('ArrowDown');await expect(links.nth(1)).toBeFocused();
    await page.keyboard.press('ArrowLeft');await expect(links.nth(1)).toBeFocused();
    await page.keyboard.press('ArrowUp');await expect(links.nth(0)).toBeFocused();
    await page.keyboard.press('ArrowUp');await expect(input).toBeFocused();
  }
  await input.fill('not found');await page.keyboard.press('ArrowDown');await expect(input).toBeFocused();
});

test('mosaic arrows use square placement rather than expanded previews or repository links',async({page})=>{
  await page.setViewportSize({width:1440,height:1000});await page.goto(base);
  const squares=await page.locator('.game').evaluateAll(items=>items.map(item=>{
    const box=item.getBoundingClientRect();return {id:item.querySelector('.game-launch').dataset.id,x:box.x,y:box.y,right:box.right,bottom:box.bottom};
  }));
  // Derive an unambiguous adjacent pair from actual packed squares. Featured
  // tiles are twice as large, so linear DOM +1 cannot represent this contract.
  const pair=squares.flatMap(from=>squares.filter(to=>Math.abs(to.x-from.right-12)<2 && Math.min(to.bottom,from.bottom)-Math.max(to.y,from.y)>10).map(to=>({from,to})))
    .find(({from,to})=>Math.abs((from.bottom-from.y)-(to.bottom-to.y))>20);
  expect(pair,'fixture has differently sized neighbors').toBeTruthy();
  const from=page.locator(`.game-launch[data-id="${pair.from.id}"]`);
  await from.focus();await page.waitForTimeout(400);await page.keyboard.press('ArrowRight');
  const focused=await page.locator(':focus').getAttribute('data-id');
  expect(squares.filter(item=>Math.abs(item.x-pair.to.x)<2 && Math.min(item.bottom,pair.from.bottom)-Math.max(item.y,pair.from.y)>10).map(item=>item.id)).toContain(focused);
  await expect(page.locator(':focus')).toHaveClass(/game-launch/);
  await page.locator('.identity').focus();await page.keyboard.press('ArrowLeft');await expect(page.locator('.identity')).toBeFocused();
});


test('separate brand and controls stay in two phone columns above three game columns',async({page},testInfo)=>{
  await page.goto(base);
  const panel=page.getByRole('group',{name:'Library controls',exact:true});
  for(const width of [320,390,584,768,1280,1440,1920]) {
    await page.setViewportSize({width,height:900});await expect(panel).toBeVisible();
    const {brand,tools}=await panel.evaluate(element=>({brand:document.querySelector('.identity').getBoundingClientRect().toJSON(),tools:element.getBoundingClientRect().toJSON()}));
    expect(tools.x).toBeGreaterThanOrEqual(brand.x+brand.width);
    expect(Math.abs(brand.y-tools.y)).toBeLessThan(2);expect(Math.abs(brand.height-tools.height)).toBeLessThan(2);
    expect(Math.abs(brand.width-tools.width)).toBeLessThan(2);
    if(width<=600) {
      await expect.poll(()=>page.locator('#games').evaluate(element=>{
        const normal=element.querySelector('.game:not(.featured)').getBoundingClientRect(),gap=parseFloat(getComputedStyle(element).gap);
        return Math.abs(element.clientWidth-normal.width*3-gap*2)<2;
      })).toBe(true);
      expect(await page.locator('#games').evaluate(element=>Math.min(...[...element.querySelectorAll('.game')].map(game=>game.getBoundingClientRect().top)))).toBeGreaterThanOrEqual(tools.y+tools.height);
    } else {
      await expect.poll(()=>panel.evaluate(element=>{
        const normal=document.querySelector('.game:not(.featured)').getBoundingClientRect();
        return Math.abs(element.getBoundingClientRect().width-normal.width)<2&&normal.width<=150;
      })).toBe(true);
      const normal=await page.locator('.game:not(.featured)').first().boundingBox();
      expect(normal.width).toBeLessThanOrEqual(150);expect(normal.width).toBeGreaterThanOrEqual(110);
    }
    await expect(panel.locator('button')).toHaveCount(4);
    for(const control of await panel.locator('a,button').all()) {
      const rect=await control.boundingBox();expect(rect.width).toBeGreaterThanOrEqual(44);expect(rect.height).toBeGreaterThanOrEqual(44);
      expect(await control.evaluate(element=>{const label=element.querySelector('span');return !label||(label.getBoundingClientRect().height<=parseFloat(getComputedStyle(label).fontSize)*1.5&&label.scrollWidth<=label.clientWidth+1);})).toBe(true);
      expect(await control.evaluate(element=>{const b=element.getBoundingClientRect();return element.contains(document.elementFromPoint(b.x+b.width/2,b.y+b.height/2));})).toBe(true);
    }
    expect(await panel.evaluate(element=>{
      const panel=element.getBoundingClientRect();return [...document.querySelectorAll('.game')].every(game=>{
        const b=game.getBoundingClientRect();return Math.min(b.right,panel.right)-Math.max(b.left,panel.left)<1||Math.min(b.bottom,panel.bottom)-Math.max(b.top,panel.top)<1;
      });
    })).toBe(true);
    expect(await page.evaluate(()=>document.documentElement.scrollWidth)).toBeLessThanOrEqual(width);
    await page.screenshot({path:testInfo.outputPath(`controls-${width}.png`)});
  }
  await page.locator('.identity').focus();
  for(const id of ['search-toggle','fullscreen','shuffle','surprise']) {
    await page.keyboard.press('Tab');await expect(page.locator('#'+id)).toBeFocused();
  }
  await page.locator('.identity').focus();await page.keyboard.press('ArrowRight');await expect(page.locator('#search-toggle')).toBeFocused();
  await page.keyboard.press('ArrowRight');await expect(page.locator('#fullscreen')).toBeFocused();
  await page.keyboard.press('ArrowDown');await expect(page.locator('#surprise')).toBeFocused();
  await page.keyboard.press('ArrowLeft');await expect(page.locator('#shuffle')).toBeFocused();
  await page.keyboard.press('ArrowUp');await expect(page.locator('#search-toggle')).toBeFocused();
});


for (const width of [1280,390]) test(`search moves smoothly into focus and out again at ${width}px`,async({page},testInfo)=>{
  await page.setViewportSize({width,height:900});await page.goto(base);
  await expect(page.locator('#games .game')).toHaveCount(24);
  // Observe painted frames at the real dialog boundary, without exposing an
  // animation hook or depending on CSS classes, keyframes or duration values.
  const record=async(event,selector)=>page.evaluate(({event,selector})=>{
    window.searchFrames=new Promise(resolve=>{
      document.querySelector(selector).addEventListener(event,()=>{
        const frames=[],start=performance.now();
        const sample=now=>{
          const dialog=document.querySelector('#search-panel'),style=getComputedStyle(dialog),box=dialog.getBoundingClientRect();
          frames.push({time:now-start,x:box.x,y:box.y,width:box.width,opacity:Number(style.opacity),display:style.display});
          if(now-start<500)requestAnimationFrame(sample);else resolve(frames);
        };
        requestAnimationFrame(sample);
      },{once:true});
    });
  },{event,selector});
  const dialog=page.locator('#search-panel'),input=page.getByRole('searchbox');
  await record('click','#search-toggle');await page.locator('#search-toggle').click();
  await expect(input).toBeFocused();
  const opening=await page.evaluate(()=>window.searchFrames),settled=opening.at(-1);
  expect(opening.some(frame=>frame.width>0&&frame.width<settled.width-3&&frame.y>settled.y+3&&frame.opacity<.95)).toBe(true);
  expect(settled.opacity).toBe(1);
  await page.screenshot({path:testInfo.outputPath('search-open.png')});
  await record('close','#search-panel');await page.keyboard.press('Escape');
  const closing=await page.evaluate(()=>window.searchFrames);
  expect(closing.some(frame=>frame.display!=='none'&&frame.opacity>0&&frame.opacity<.95)).toBe(true);
  expect(closing.at(-1).display).toBe('none');
  await expect(page.locator('#search-toggle')).toBeFocused();
  await testInfo.attach('search-motion-frames',{body:JSON.stringify({opening,closing},null,2),contentType:'application/json'});
  // Reversing an exit and closing during entrance must never steal focus,
  // discard typing, or leave a stale close callback that dismisses search.
  await page.keyboard.press('Control+k');await page.keyboard.press('Escape');
  await page.keyboard.press('Control+k');await input.fill('07');
  await expect(dialog).toBeVisible();await expect(input).toBeFocused();
  await expect(dialog.locator('#search-results a')).toHaveCount(1);
  await page.keyboard.press('ArrowDown');await page.keyboard.press('Enter');
  await expect(page.locator('iframe')).toHaveCount(1);
  await expect(dialog).toBeHidden();
});

test('search stays immediate with reduced motion',async({page})=>{
  await page.emulateMedia({reducedMotion:'reduce'});await page.goto(base);
  await expect(page.locator('#games .game')).toHaveCount(24);
  const state=await page.evaluate(()=>{
    document.querySelector('#search-toggle').click();
    const dialog=document.querySelector('#search-panel'),style=getComputedStyle(dialog);
    const opened={opacity:style.opacity,transform:style.transform,backdrop:getComputedStyle(dialog,'::backdrop').opacity,focused:document.activeElement.id};
    document.querySelector('#search').dispatchEvent(new KeyboardEvent('keydown',{key:'Escape',bubbles:true,cancelable:true}));
    return {opened,closed:getComputedStyle(dialog).display};
  });
  expect(state.opened).toEqual({opacity:'1',transform:'none',backdrop:'1',focused:'search'});
  expect(state.closed).toBe('none');
});
