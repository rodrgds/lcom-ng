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
      response.setHeader('Content-Type',({'.js':'text/javascript','.css':'text/css','.svg':'image/svg+xml','.ttf':'font/ttf'})[path.extname(name)]||'text/html');
      let body=readFileSync(path.join(template,name==='/'?'index.html':name));
      if(name==='/') body=body.toString().replaceAll('@NAME@','Navigation Fixture');
      response.end(body);
    } catch {response.writeHead(404).end();}
  });
  await new Promise(resolve=>server.listen(0,'127.0.0.1',resolve));
  base='http://127.0.0.1:'+server.address().port;
});
test.afterAll(async()=>new Promise(resolve=>server.close(resolve)));

test('search shortcuts open the dialog without taking typing, other modal or guest input',async({page})=>{
  await page.goto(base);
  const input=page.getByRole('searchbox',{name:'Search games'});
  await expect(input).toBeHidden();
  await page.keyboard.press('Control+k');await expect(input).toBeFocused();
  await input.fill('Game');await page.keyboard.press('/');await expect(input).toHaveValue('Game/');
  await page.keyboard.press('Meta+k');await expect(input).toBeFocused();await expect(input).toHaveValue('Game/');
  await page.keyboard.press('Escape');await expect(page.locator('#search-toggle')).toBeFocused();
  await page.locator('.identity').focus();await page.keyboard.press('/');await expect(input).toBeFocused();
  await input.fill('Game');await page.keyboard.press('Escape');
  await page.getByRole('button',{name:'Filter games',exact:true}).click();
  await expect(page.locator('#browse-panel')).toBeVisible();
  await page.keyboard.press('Control+k');await expect(input).toBeFocused();await expect(page.locator('#browse-panel')).toBeHidden();
  await page.keyboard.press('Escape');await page.getByRole('button',{name:'Surprise me',exact:true}).click();
  await expect(page.locator('#surprise-panel')).toBeVisible();await page.keyboard.press('Control+k');await page.keyboard.press('/');
  await expect(input).toBeHidden();await expect(page.locator('#search')).toHaveValue('Game');await page.keyboard.press('Escape');
  await page.locator('#games .game-launch').first().click();
  const guest=page.frameLocator('iframe').getByRole('textbox',{name:'Guest input'});
  await guest.focus();await page.keyboard.press('Control+k');await page.keyboard.press('Meta+k');await page.keyboard.press('/');
  await expect(guest).toHaveValue('/');await expect(guest).toBeFocused();await expect(input).toBeHidden();
});

test('search result launch and Home restore query and focus without changing the mosaic',async({page})=>{
  await page.goto(base);await page.keyboard.press('/');
  const input=page.getByRole('searchbox',{name:'Search games'}),results=page.locator('#search-results a');
  await input.fill('07');await expect(results).toHaveCount(1);await expect(results).toHaveAttribute('data-id','game-7');
  await page.keyboard.press('Enter');await expect(page.locator('iframe')).toHaveCount(1);await expect(page.locator('#loading')).toBeHidden();
  await page.locator('#back').click();await expect(input).toBeVisible();await expect(input).toHaveValue('07');await expect(results).toBeFocused();
  await page.keyboard.press('Escape');await expect(page.locator('#search-toggle')).toBeFocused();await expect(page.locator('#games .game-launch')).toHaveCount(24);
});

for(const width of [1280,390]) test(`search arrows follow the result list at ${width}px`,async({page})=>{
  await page.setViewportSize({width,height:844});await page.goto(base);await page.keyboard.press('/');
  const input=page.getByRole('searchbox',{name:'Search games'}),links=page.locator('#search-results a');
  await input.fill('Game');await expect(links).toHaveCount(24);
  await page.keyboard.press('ArrowDown');await expect(links.nth(0)).toBeFocused();
  if(width>600){await page.keyboard.press('ArrowRight');await expect(links.nth(1)).toBeFocused();}
  await page.keyboard.press('ArrowDown');await expect(links.nth(width>600?3:1)).toBeFocused();
  await page.keyboard.press('ArrowUp');await expect(links.nth(width>600?1:0)).toBeFocused();
  await page.keyboard.press('ArrowUp');await expect(input).toBeFocused();
  await page.keyboard.press('Control+k');await expect(input).toBeFocused();await expect(input).toHaveValue('Game');
  await input.fill('not found');await page.keyboard.press('ArrowDown');await expect(input).toBeFocused();await expect(links).toHaveCount(0);
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
  await page.route('**/collection.json',route=>route.fulfill({json:{name:'LCOM Arcade',games}}));
  await page.goto(base);
  const panel=page.getByRole('group',{name:'Library controls',exact:true});
  for(const width of [320,390,584,768,1280,1440,1920]) {
    await page.setViewportSize({width,height:900});await expect(panel).toBeVisible();
    // ResizeObserver can reflow between two protocol calls. Read both tiles
    // in one layout snapshot and wait for the requested responsive arrangement.
    await expect.poll(()=>panel.evaluate(element=>{
      const brand=document.querySelector('.identity').getBoundingClientRect(),tools=element.getBoundingClientRect();
      return tools.x>=brand.right && Math.abs(brand.y-tools.y)<2 && Math.abs(brand.width-tools.width)<2 && Math.abs(brand.height-tools.height)<2;
    })).toBe(true);
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
      if(width===1280)expect(await panel.evaluate(element=>{
        const grid=document.querySelector('#grid'),normal=document.querySelector('.game:not(.featured)').getBoundingClientRect();
        const gap=parseFloat(getComputedStyle(grid).gap);
        return Math.round((grid.clientWidth+gap)/(normal.width+gap));
      })).toBe(9);
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
    expect(await page.locator('.identity h1').evaluate(element=>{
      const range=document.createRange();range.selectNodeContents(element);
      const lines=[...range.getClientRects()],box=element.getBoundingClientRect();
      return new Set(lines.map(line=>Math.round(line.top))).size<=2&&lines.every(line=>line.right<=box.right+1);
    })).toBe(true);
    await page.screenshot({path:testInfo.outputPath(`controls-${width}.png`)});
  }
  await page.locator('.identity').focus();
  for(const id of ['search-toggle','fullscreen','shuffle','surprise','browse-toggle']) {
    await page.keyboard.press('Tab');await expect(page.locator('#'+id)).toBeFocused();
  }
  await page.locator('.identity').focus();await page.keyboard.press('ArrowRight');await expect(page.locator('#search-toggle')).toBeFocused();
  await page.keyboard.press('ArrowRight');await expect(page.locator('#fullscreen')).toBeFocused();
  await page.keyboard.press('ArrowDown');await expect(page.locator('#surprise')).toBeFocused();
  await page.keyboard.press('ArrowLeft');await expect(page.locator('#shuffle')).toBeFocused();
  await page.locator('#browse-toggle').focus();await page.keyboard.press('ArrowLeft');await expect(page.locator('#fullscreen')).toBeFocused();
});
