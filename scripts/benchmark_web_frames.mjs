// Run through benchmark_web_frames.py inside Devenv. The actual browser host
// converts every frame; the callback excludes Canvas so upload cannot mask it.
import {chromium} from '@playwright/test';
import {createServer} from 'node:http';
import {readFileSync,writeFileSync} from 'node:fs';
import path from 'node:path';
import {cpus} from 'node:os';
const [directory,maximum,cpuRate='1']=process.argv.slice(2),root=path.resolve(directory);
const server=createServer((request,response)=>{
  try {
    const name=new URL(request.url,'http://localhost').pathname;
    if(name==='/'){response.setHeader('Content-Type','text/html');response.end('<!doctype html><title>Framebuffer benchmark</title>');return;}
    if(!['/frames.js','/frames.wasm'].includes(name)){response.writeHead(404).end();return;}
    response.setHeader('Content-Type',name.endsWith('.wasm')?'application/wasm':'text/javascript');
    response.end(readFileSync(path.join(root,name)));
  } catch {response.writeHead(404).end();}
});
await new Promise(resolve=>server.listen(0,'127.0.0.1',resolve));
let browser;
try {
  browser=await chromium.launch(process.env.CHROME_PATH?{executablePath:process.env.CHROME_PATH}:process.platform==='darwin'?{channel:'chrome'}:{channel:'chromium'});
  const page=await browser.newPage();
  const cdp=await page.context().newCDPSession(page);
  await cdp.send('Emulation.setCPUThrottlingRate',{rate:Number(cpuRate)});
  await page.goto(`http://127.0.0.1:${server.address().port}`);
  writeFileSync(path.join(root,'environment.json'),JSON.stringify({browser:browser.version(),platform:process.platform,arch:process.arch,cpu:cpus()[0]?.model,cpuRate:Number(cpuRate),timestamp:new Date().toISOString()},null,2)+'\n');
  const results=await page.evaluate(async()=>{
    const {default:create}=await import('/frames.js');let last;
    const module=await create({player:{present(bytes,width,height){last={bytes,width,height};}}});
    const rows=[];
    for(const mode of [0x200,0x14c,0x115,0x118,0x110,0x111,0x117,0x11a,0x105]) {
      const call=count=>module.ccall('benchmark_present','number',['number','number'],[mode,count]);
      call(40);
      const samples=Array.from({length:5},()=>call(60));
      const ms=[...samples].sort((a,b)=>a-b)[2];
      const sha256=[...new Uint8Array(await crypto.subtle.digest('SHA-256',last.bytes))].map(value=>value.toString(16).padStart(2,'0')).join('');
      rows.push({mode:'0x'+mode.toString(16),width:last.width,height:last.height,ms,samples,sha256});
    }
    return rows;
  });
  writeFileSync(path.join(root,'results.json'),JSON.stringify(results,null,2)+'\n');
  console.log(JSON.stringify(results,null,2));
  if(maximum&&results.some(row=>row.ms>Number(maximum)))process.exitCode=1;
} finally {
  await browser?.close();await new Promise(resolve=>server.close(resolve));
}
