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
