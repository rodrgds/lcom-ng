import {test} from 'node:test';
import assert from 'node:assert/strict';
import {gzipSync,gunzipSync} from 'node:zlib';
import worker from '../collections/arcade/worker.js';

test('game delivery honors original client encoding and preserves the module bytes',async()=>{
  const original=Buffer.from([0,97,115,109,1,0,0,0]);
  for(const [header,clientEncoding,compressed] of [
    ['gzip, br',undefined,true],['identity',undefined,false],
    ['gzip, br','identity',false],['gzip, br','gzip;q=0, *;q=1',false],
    ['gzip, br','gzip;q=0.5',true],['gzip, br','*;q=0',false],
  ]) {
    const request=new Request('https://arcade.rgo.pt/games/fixture/game.wasm',{headers:{'Accept-Encoding':header}});
    if(clientEncoding!==undefined)request.cf={clientAcceptEncoding:clientEncoding};
    const response=await worker.fetch(request,{ASSETS:{fetch:async()=>new Response(gzipSync(original))}});
    assert.equal(response.headers.get('Content-Encoding'),compressed?'gzip':null,clientEncoding||header);
    const body=Buffer.from(await response.arrayBuffer());
    assert.deepEqual(compressed?gunzipSync(body):body,original);
  }
});

test('share links serve the launcher with the game metadata and root-relative assets',async()=>{
  const catalog={name:'LCOM Arcade',games:[{slug:'afg-ninjix',name:'Ninjix',description:'Ninja defence',
    authors:['Afonso Alves','Filipe Faria','Gonçalo Gomes'],screenshotThumbnail:'static/shot-123.webp'}]};
  const shell='<!doctype html><html><head><title>LCOM Arcade</title></head><body></body></html>';
  const env={ASSETS:{fetch:async request=>{
    const pathname=new URL(request.url).pathname;
    if(pathname==='/collection.json')return new Response(JSON.stringify(catalog));
    if(pathname==='/')return new Response(shell,{headers:{'Content-Type':'text/html','ETag':'"shell"'}});
    return new Response('Not found',{status:404});
  }}};
  const response=await worker.fetch(new Request('https://arcade.rgo.pt/share/afg-ninjix'),env);
  assert.equal(response.status,200);
  assert.equal(response.headers.get('Content-Type'),'text/html; charset=utf-8');
  const html=await response.text();
  assert.match(html,/<base href="\/">/);
  assert.match(html,/<title>Play Ninjix · LCOM Arcade<\/title>/);
  assert.match(html,/property="og:image" content="https:\/\/arcade\.rgo\.pt\/static\/shot-123\.webp"/);
  assert.match(html,/Afonso Alves, Filipe Faria, Gonçalo Gomes/);
  assert.match(html,/rel="canonical" href="https:\/\/arcade\.rgo\.pt\/share\/afg-ninjix"/);
  const unknown=await worker.fetch(new Request('https://arcade.rgo.pt/share/missing-game'),env);
  assert.equal(unknown.status,200);
  assert.ok(!(await unknown.text()).includes('og:image'));
  const invalid=await worker.fetch(new Request('https://arcade.rgo.pt/share/Not%20a%20slug'),env);
  assert.equal(invalid.status,404);
});
