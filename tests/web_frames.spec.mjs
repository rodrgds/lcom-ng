import {test,expect,chromium,firefox} from '@playwright/test';
import {execFileSync} from 'node:child_process';
import {createServer} from 'node:http';
import {mkdtempSync,readFileSync,rmSync} from 'node:fs';
import {tmpdir} from 'node:os';
import path from 'node:path';

// Each engine launches explicitly, without inheriting the macOS Chrome channel.
test.use({launchOptions:{}});
const root=path.resolve(import.meta.dirname,'..');let directory,server,base;
test.beforeAll(async()=>{
  test.setTimeout(120_000);
  directory=mkdtempSync(path.join(tmpdir(),'lcom-web-frames-'));
  execFileSync(process.env.LCOM_CLI||path.join(root,'build/machinelab'),
    ['export','web',path.join(root,'tests/fixtures/frame-formats'),'--output',directory],{stdio:'pipe'});
  server=createServer((request,response)=>{
    const name=new URL(request.url,'http://localhost').pathname;
    try {
      response.setHeader('Content-Type',({'.js':'text/javascript','.wasm':'application/wasm','.json':'application/json','.css':'text/css'})[path.extname(name)]||'text/html');
      response.end(readFileSync(path.join(directory,name==='/'?'index.html':name)));
    } catch {response.writeHead(404).end();}
  });
  await new Promise(resolve=>server.listen(0,'127.0.0.1',resolve));base=`http://127.0.0.1:${server.address().port}`;
});
test.afterAll(async()=>{
  if(server)await new Promise(resolve=>server.close(resolve));
  if(directory)rmSync(directory,{recursive:true,force:true});
});

for(const [name,engine] of [['chromium',chromium],['firefox',firefox]]) {
  test(`${name}: all framebuffer formats preserve opaque pixels after mode changes and heap growth`,async()=>{
    const browser=await engine.launch(name==='chromium'?(process.env.CHROME_PATH?{executablePath:process.env.CHROME_PATH}:{channel:process.platform==='darwin'?'chrome':'chromium'}):{});
    try {
      const page=await browser.newPage(),output=[];page.on('console',message=>output.push(message.text()));
      await page.goto(base);await expect(page.locator('#status')).toHaveText('Game running.');
      const cases=[
        [1280,1024,[143,38,7],[12,63,215]], [1152,864,[143,38,7],[12,63,215]],
        [800,600,[143,38,7],[12,63,215]], [1024,768,[143,38,7],[12,63,215]],
        [640,480,[32,32,32],[255,255,0]], [640,480,[32,16,32],[255,255,0]],
        [1024,768,[32,16,32],[255,255,0]], [1280,1024,[32,16,32],[255,255,0]],
        [1024,768,[125,125,255],[125,255,125]], [1280,1024,[143,38,7],[12,63,215]],
      ];
      for(let i=0;i<cases.length;i++) {
        const [width,height,a,b]=cases[i];if(i)await page.keyboard.press('Enter');
        await expect.poll(()=>output.join('\n')).toContain('FRAME_FORMAT step='+i+' ');
        await expect.poll(()=>page.locator('canvas').evaluate(canvas=>({
          width:canvas.width,height:canvas.height,
          pixels:[[0,0],[canvas.width-1,0],[0,canvas.height-1],[canvas.width-1,canvas.height-1]].map(([x,y])=>[...canvas.getContext('2d').getImageData(x,y,1,1).data]),
        }))).toEqual({width,height,pixels:[[...a,255],[...b,255],[...b,255],[...a,255]]});
      }
      expect(output.join('\n')).toContain('FRAME_FORMAT heap-grown');
    } finally {await browser.close();}
  });
}
