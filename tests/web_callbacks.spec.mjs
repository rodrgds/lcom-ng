import {test,expect} from '@playwright/test';
import {execFile} from 'node:child_process';
import {promisify} from 'node:util';
import {createServer} from 'node:http';
import {mkdtempSync,mkdirSync,readFileSync,rmSync,writeFileSync} from 'node:fs';
import {tmpdir} from 'node:os';
import path from 'node:path';

const root=path.resolve(import.meta.dirname,'..');
test('Sentence Sprint source preserves WASM allocation, names and saved records',async()=>{
  test.setTimeout(120000);
  await promisify(execFile)('python3',[path.join(root,'tests/community_wasm_regressions.py')],
    {cwd:root,maxBuffer:4*1024*1024});
});

test('concurrent exports preserve runtime archives and legacy callback ABIs',async({page})=>{
  test.setTimeout(120000);
  const directory=mkdtempSync(path.join(tmpdir(),'lcom-callbacks-'));
  let server;
  try {
    await Promise.all(['one','two'].map(name=>promisify(execFile)('python3',
      [path.join(root,'sdk/web/export.py'),path.join(root,'tests/fixtures/web/callbacks'),'--output',path.join(directory,name)],
      {cwd:root,maxBuffer:4*1024*1024})));
    await promisify(execFile)('python3',
      [path.join(root,'tests/export_with_replaced_cache.py'),
       '--sdk',path.join(root,'sdk'),'--project',path.join(root,'tests/fixtures/web/callbacks'),
       '--output',path.join(directory,'replaced-cache')],
      {cwd:root,maxBuffer:4*1024*1024});
    const mime={'.js':'text/javascript','.wasm':'application/wasm','.html':'text/html','.json':'application/json','.css':'text/css'};
    server=createServer((req,res)=>{
      const file=path.join(directory,req.url==='/'?'index.html':req.url.split('?')[0]);
      try {res.setHeader('Content-Type',mime[path.extname(file)]||'application/octet-stream');res.end(readFileSync(file));}
      catch {res.writeHead(404).end();}
    });
    await new Promise(r=>server.listen(0,'127.0.0.1',r));
    const messages=[];
    page.on('console',m=>messages.push(m.text()));
    page.on('pageerror',e=>messages.push(String(e)));
    for (const name of ['one','two','replaced-cache']) {
      messages.length=0;
      await page.goto('http://127.0.0.1:'+server.address().port+'/'+name+'/index.html');
      await expect.poll(async()=>messages.join('\n')+'\n'+await page.locator('#status').textContent()).toContain('legacy callback:42');
    }
  } finally {
    await page.close();
    if(server) {server.closeAllConnections();await new Promise(r=>server.close(r));}
    rmSync(directory,{recursive:true,force:true});
  }
});


test('explicit game C dialect survives alternate configuration in both web source paths',async({page})=>{
  test.setTimeout(120000);
  const directory=mkdtempSync(path.join(tmpdir(),'lcom-c-dialect-'));
  let server;
  try {
    for(const kind of ['minix','flat']) {
      const project=path.join(directory,kind);mkdirSync(project);
      writeFileSync(path.join(project,'main.c'),'#include <stdio.h>\ninline __attribute__((noinline)) int value(void){return 42;}\nint(*volatile fn)(void)=value;int main(void){printf("DIALECT_%d\\n",fn());return 42;}\n');
      if(kind==='minix')writeFileSync(path.join(project,'Makefile'),'PROG=proj\nSRCS=main.c\n');
      writeFileSync(path.join(project,'lcom.json'),JSON.stringify({build:{cStandard:'gnu11'}}));
      const profile=path.join(directory,kind+'-original-compiler.json');
      writeFileSync(profile,JSON.stringify({build:{cStandard:'gnu89'}}));
      await promisify(execFile)('python3',[path.join(root,'sdk/web/export.py'),project,'--config',profile,'--output',path.join(directory,kind+'-relocated')],{cwd:root,maxBuffer:4*1024*1024});
      rmSync(project,{recursive:true});rmSync(profile);
    }
    const mime={'.js':'text/javascript','.wasm':'application/wasm','.html':'text/html','.json':'application/json','.css':'text/css'};
    server=createServer((req,res)=>{
      const file=path.join(directory,req.url.split('?')[0]);
      try {res.setHeader('Content-Type',mime[path.extname(file)]||'application/octet-stream');res.end(readFileSync(file));}
      catch {res.writeHead(404).end();}
    });
    await new Promise(r=>server.listen(0,'127.0.0.1',r));
    const messages=[],errors=[];page.on('console',m=>messages.push(m.text()));page.on('pageerror',e=>errors.push(String(e)));
    for(const kind of ['minix','flat']) {
      messages.length=0;
      await page.goto('http://127.0.0.1:'+server.address().port+'/'+kind+'-relocated/index.html');
      await expect(page.locator('#status')).toHaveText('Game exited with status 42.');
      expect(messages.join('\n')).toContain('DIALECT_42');
      expect(errors).toEqual([]);
    }
  } finally {
    await page.close();
    if(server) {server.closeAllConnections();await new Promise(r=>server.close(r));}
    rmSync(directory,{recursive:true,force:true});
  }
});

test('Minix IRQ notify bits and opaque handles survive embedded policy cleanup and reset',async({page})=>{
  test.setTimeout(120000);
  const directory=mkdtempSync(path.join(tmpdir(),'lcom-irq-handles-'));
  let server;
  try {
    const project=path.join(directory,'original-policy');mkdirSync(project);
    writeFileSync(path.join(project,'Makefile'),'PROG=irq-policy\nSRCS=main.c\nLDADD=-llcf\n');
    writeFileSync(path.join(project,'main.c'),readFileSync(path.join(root,'tests/compat_client.c')));
    writeFileSync(path.join(project,'lcom.json'),JSON.stringify({build:{arguments:['--irq-policy-reset']},mobile:{controls:{pointer:'off',keyboard:false,mouseButtons:false,joystick:false,quickKeys:[]}}}));
    const output=path.join(directory,'web');
    await promisify(execFile)('python3',[path.join(root,'sdk/web/export.py'),project,'--output',output],{cwd:root,maxBuffer:4*1024*1024});
    const mime={'.js':'text/javascript','.wasm':'application/wasm','.html':'text/html','.json':'application/json','.css':'text/css'};
    server=createServer((req,res)=>{
      const file=path.join(output,req.url==='/'?'index.html':req.url.split('?')[0]);
      try {res.setHeader('Content-Type',mime[path.extname(file)]||'application/octet-stream');res.end(readFileSync(file));}
      catch {res.writeHead(404).end();}
    });
    await new Promise(r=>server.listen(0,'127.0.0.1',r));
    const messages=[],errors=[];
    page.on('console',m=>messages.push(m.text()));page.on('pageerror',e=>errors.push(String(e)));
    await page.goto('http://127.0.0.1:'+server.address().port+'/');
    await expect.poll(()=>messages.join('\n')).toContain('IRQ input bits and opaque handles passed');
    await expect.poll(()=>messages.join('\n')).toContain('IRQ policy replacement contracts passed');
    await expect(page.locator('#status')).toContainText('Game finished.');
    expect(errors).toEqual([]);
  } finally {
    await page.close();
    if(server){server.closeAllConnections();await new Promise(r=>server.close(r));}
    rmSync(directory,{recursive:true,force:true});
  }
});


test('original volatile byte port routines retain RTC and UART values after WASM reload',async({page})=>{
  test.setTimeout(120000);
  const directory=mkdtempSync(path.join(tmpdir(),'lcom-byte-ports-'));
  let server;
  try {
    const output=path.join(directory,'web');
    await promisify(execFile)('python3',[path.join(root,'sdk/web/export.py'),path.join(root,'tests/fixtures/compat-byte-ports'),'--output',output],{cwd:root,maxBuffer:4*1024*1024});
    const mime={'.js':'text/javascript','.wasm':'application/wasm','.html':'text/html','.json':'application/json','.css':'text/css'};
    server=createServer((req,res)=>{
      const file=path.join(output,req.url==='/'?'index.html':req.url.split('?')[0]);
      try {res.setHeader('Content-Type',mime[path.extname(file)]||'application/octet-stream');res.end(readFileSync(file));}
      catch {res.writeHead(404).end();}
    });
    await new Promise(r=>server.listen(0,'127.0.0.1',r));
    const messages=[],errors=[];
    page.on('console',m=>messages.push(m.text()));page.on('pageerror',e=>errors.push(String(e)));
    for(let round=0;round<2;round++) {
      messages.length=0;
      await page.goto('http://127.0.0.1:'+server.address().port+'/');
      await expect(page.locator('#status')).toContainText('Game finished.');
      expect(messages.join('\n')).toContain('ORIGINAL_BYTE_PORTS_OK');
      expect(errors).toEqual([]);
    }
  } finally {
    await page.close();
    if(server){server.closeAllConnections();await new Promise(r=>server.close(r));}
    rmSync(directory,{recursive:true,force:true});
  }
});
