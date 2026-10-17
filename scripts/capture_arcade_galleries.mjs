// The Python CLI owns image conversion, manifest checks and selection.
import {createServer} from 'node:http';
import {readFileSync,writeFileSync,mkdirSync,statSync} from 'node:fs';
import path from 'node:path';
import {createRequire} from 'node:module';
import {spawnSync} from 'node:child_process';
const require=createRequire(path.join(process.cwd(),'package.json'));
const {chromium}=require('playwright');
const [dist,planPath,output,...selected]=process.argv.slice(2);
const plans=JSON.parse(readFileSync(planPath)).games;
const mime={'.js':'text/javascript','.wasm':'application/wasm','.html':'text/html','.json':'application/json','.css':'text/css','.png':'image/png','.webp':'image/webp'};
const server=createServer((req,res)=>{try{const pathname=decodeURIComponent(new URL(req.url,'http://localhost').pathname);let file=path.resolve(dist,'.'+pathname);if(!file.startsWith(path.resolve(dist)+path.sep))throw Error('outside export');if(statSync(file).isDirectory())file=path.join(file,'index.html');let data=readFileSync(file);if(path.basename(file)==='player.js')data=Buffer.concat([data,Buffer.from('\nwindow.arcadeGalleryPlayer=player;\n')]);res.setHeader('Content-Type',mime[path.extname(file)]||'application/octet-stream');res.end(data);}catch{res.writeHead(404).end();}});
await new Promise(resolve=>server.listen(0,'127.0.0.1',resolve));
const browser=await chromium.launch({channel:'chrome',headless:true});const results=[];
try{for(const plan of plans){if(selected.length&&!selected.includes(plan.id)&&!selected.includes(plan.name))continue;
 const context=await browser.newContext({viewport:{width:1440,height:1080}});const page=await context.newPage();const errors=[];page.on('pageerror',e=>errors.push(e.stack||String(e)));const records=[];const held=new Set();let cursor=plan.cursor||[0,0],buttons=0,golfPhase=0,sentenceSprint=null,candyRouteState=null,rodolfoSentence=null,typoSentence=null,typoTyped=0;
 const wait=ms=>page.waitForTimeout(ms);
 const call=(name,args,types)=>page.evaluate(({name,args,types})=>window.arcadeGalleryPlayer.module.ccall(name,null,types,args),{name,args,types});
 const key=async(k,down)=>{if(k.startsWith('MOUSE')){buttons=down?buttons|({1:1,2:2,3:4}[Number(k.slice(5))]):buttons&~({1:1,2:2,3:4}[Number(k.slice(5))]);await call('lcom_web_mouse',[0,0,buttons],['number','number','number']);}else{await call('lcom_web_key',[k,down?1:0],['string','number']);if(down)held.add(k);else held.delete(k);}};
 try{await page.goto(`http://127.0.0.1:${server.address().port}/${plan.url}`);await page.waitForFunction(()=>window.arcadeGalleryPlayer?.module&&document.querySelector('#status')?.textContent==='Game running.',{timeout:30000});await wait(1200);
 for(const step of plan.actions){
  if(step.op==='wait')await wait(step.ms);
  else if(step.op==='key'){if(step.down!==undefined){await key(step.key,step.down);await wait(step.down?35:5);}else{await key(step.key,true);await wait(step.ms||100);await key(step.key,false);await wait(80);}}
  else if(step.op==='multi'){for(const k of step.keys)await key(k,true);await wait(step.ms||500);for(const k of step.keys)await key(k,false);await wait(100);}
  else if(step.op==='mouse'){buttons=step.buttons||0;await call('lcom_web_mouse',[step.dx,step.dy,buttons],['number','number','number']);cursor=[cursor[0]+step.dx*(plan.gain||1),cursor[1]-step.dy*(plan.gain||1)];await wait(step.settle||40);}
  else if(step.op==='point'&&step.absolute){await call('lcom_web_pointer',[step.x,step.y,step.buttons||0],['number','number','number']);await wait(200);cursor=[step.x,step.y];if(step.click!==false){await call('lcom_web_pointer',[step.x,step.y,1],['number','number','number']);await wait(step.ms||120);await call('lcom_web_pointer',[step.x,step.y,0],['number','number','number']);await wait(180);}}
  else if(step.op==='point'){const gain=plan.gain||1;let dx=Math.round((step.x-cursor[0])/gain),dy=Math.round((cursor[1]-step.y)/gain);const n=Math.max(1,Math.ceil(Math.max(Math.abs(dx),Math.abs(dy))/64));let oldX=0,oldY=0;for(let k=1;k<=n;k++){let x=Math.round(dx*k/n),y=Math.round(dy*k/n);await call('lcom_web_mouse',[x-oldX,y-oldY,step.buttons||0],['number','number','number']);oldX=x;oldY=y;await wait(25);}cursor=[step.x,step.y];await wait(160);if(step.click!==false){await call('lcom_web_mouse',[0,0,1],['number','number','number']);await wait(step.ms||100);await call('lcom_web_mouse',[0,0,0],['number','number','number']);await wait(180);}}
  else if(step.op==='gold-miner'){
   // Observe original visible stones/claw only; all actions are ordinary guest input.
   const state=()=>page.locator('canvas').evaluate(c=>{
    if(c.width!==1152||c.height!==864)throw Error('Unexpected original Gold Miner geometry');
    const w=c.width,h=c.height,d=c.getContext('2d').getImageData(0,0,w,h).data,out={stone:[],gold:[],claw:[]};
    for(const kind of Object.keys(out)){
     const seen=new Uint8Array(w*h),lo=kind==='claw'?385:500,hi=kind==='claw'?430:h-10;
     const match=i=>{const r=d[i*4],g=d[i*4+1],b=d[i*4+2];return kind==='stone'?r>=30&&r<=180&&Math.max(r,g,b)-Math.min(r,g,b)<=14:kind==='gold'?r>170&&g>105&&b<95&&r-g<100:b>r+7&&b>90&&g>80;};
     for(let y=lo;y<hi;y++)for(let x=20;x<w-20;x++){let i=y*w+x;if(seen[i]||!match(i))continue;const q=[i];seen[i]=1;let minx=x,maxx=x,miny=y,maxy=y;
      for(let k=0;k<q.length;k++){const t=q[k],xx=t%w,yy=Math.floor(t/w);minx=Math.min(minx,xx);maxx=Math.max(maxx,xx);miny=Math.min(miny,yy);maxy=Math.max(maxy,yy);for(const n of[t-1,t+1,t-w,t+w]){const nx=n%w,ny=Math.floor(n/w);if(nx>=20&&nx<w-20&&ny>=lo&&ny<hi&&!seen[n]&&match(n)){seen[n]=1;q.push(n);}}}
      if(q.length>(kind==='stone'?250:kind==='gold'?50:8)&&maxx-minx<65&&maxy-miny<50)out[kind].push({x:(minx+maxx)/2,y:(miny+maxy)/2,n:q.length});
     }
     out[kind].sort((a,b)=>b.n-a.n);
    }return out;
   });
   const hold=async(k,ms)=>{await key(k,true);await wait(ms);await key(k,false);await wait(50);};
   if(step.stage==='reveal'){
    await hold('D',2600);let s=await state(),targets=s.stone.filter(p=>p.y<660);if(!targets.length)targets=s.stone;if(!targets.length)throw Error('No visible original stones');
    targets.sort((a,b)=>Math.abs(a.x-950)+Math.abs(a.y-530)*.5-Math.abs(b.x-950)-Math.abs(b.y-530)*.5);const target=targets[0],aim=[Math.max(200,target.x-80),target.y-5];cursor=[500,450];
    const dx=Math.round(aim[0]-cursor[0]),dy=Math.round(cursor[1]-aim[1]),n=Math.max(1,Math.ceil(Math.max(Math.abs(dx),Math.abs(dy))/100));let px=0,py=0;
    for(let k=1;k<=n;k++){const x=Math.round(dx*k/n),y=Math.round(dy*k/n);await call('lcom_web_mouse',[x-px,y-py,0],['number','number','number']);px=x;py=y;await wait(15);}cursor=aim;
    await hold('MOUSE1',1400);s=await state();if(!s.gold.length){await hold('MOUSE1',1800);s=await state();}if(!s.gold.length)throw Error('No visible gold after original random reveal');console.log(plan.name+': revealed original gold '+JSON.stringify(s.gold));
   }else if(step.stage==='collect'){
    let s=await state(),target;for(let i=0;i<6;i++){if(!s.claw.length||!s.gold.length)throw Error('Missing visible raised claw or gold');const claw=s.claw[0];target=[...s.gold].sort((a,b)=>Math.abs(a.x-claw.x)-Math.abs(b.x-claw.x))[0];const gap=target.x-claw.x;if(Math.abs(gap)<=10)break;await hold(gap>0?'D':'A',Math.min(650,Math.max(30,Math.round(Math.abs(gap)/300*1000))));s=await state();if(i===5)throw Error('Visible gold alignment did not converge');}
    await hold('S',Math.max(150,Math.min(1100,Math.round((target.y-409)/420*1000)+40)));await hold('W',1400);console.log(plan.name+': original claw retrieved, review visible score');
   }else throw Error('Unknown Gold Miner observation stage');
  }
  else if(step.op==='wheel'){await call('lcom_web_wheel',[step.delta],['number']);await wait(step.settle||100);}
  else if(step.op==='ultimate-visible-letters'){
   // Match only the visible original glyph, then send ordinary guest key input.
   const glyphs=[...'ABCDEFGHIJKLMNOPQRSTUVWXYZ'].map(letter=>{
    const text=readFileSync(path.join(process.cwd(),step.fontDirectory,'Letter'+letter+'.xpm'),'utf8');
    const lines=[...text.matchAll(/"([^"]*)"/g)].map(m=>m[1]);
    const [width,height,colors,cpp]=lines[0].split(/\s+/).map(Number);
    if(width!==128||height!==128||cpp!==1)throw Error('Unexpected Ultimate original font geometry');
    const white=new Set(lines.slice(1,colors+1).filter(v=>/c\s+#ffffff$/i.test(v)).map(v=>v[0]));
    const rows=lines.slice(colors+1);
    return {letter,mask:Array.from({length:4096},(_,i)=>white.has(rows[Math.floor(i/64)*2+1][i%64*2+1])?1:0)};
   });
   for(let count=0;count<step.count;count++){
    const letter=await page.locator('canvas').evaluate((canvas,glyphs)=>{
     if(canvas.width!==1024||canvas.height!==768)throw Error('Ultimate observer expects original1024x768');
     const pixels=canvas.getContext('2d').getImageData(768,32,64,64).data;
     const mask=Array.from({length:4096},(_,i)=>pixels[i*4]>240&&pixels[i*4+1]>240&&pixels[i*4+2]>240?1:0);
     const scores=glyphs.map(g=>({letter:g.letter,error:g.mask.reduce((sum,v,i)=>sum+(v!==mask[i]?1:0),0)})).sort((a,b)=>a.error-b.error);
     if(scores[0].error!==0||scores[1].error===0)throw Error('Ambiguous visible Ultimate glyph: '+JSON.stringify(scores.slice(0,2)));
     return scores[0].letter;
    },glyphs);
    console.log(plan.name+': original visible letter '+letter);await key(letter,true);await wait(65);await key(letter,false);await wait(120);
   }
  }
  else if(step.op==='text'){for(const ch of step.text){await key(ch===' '?'SPACE':ch.toUpperCase(),true);await wait(35);await key(ch===' '?'SPACE':ch.toUpperCase(),false);await wait(75);}}
  else if(step.op==='type-visible-words'){
   // Read the original bitmap glyphs and match rendered pixels. This observes
   // the screen only; every submitted word still goes through normal keys.
   const glyphs='abcdefghijklmnopqrstuvwxyz'.split('').map(letter=>{const source=readFileSync(path.join(process.cwd(),step.fontDirectory,'ascii_lower_'+letter+'.xpm'),'utf8');const lines=[...source.matchAll(/"([^"]*)"/g)].map(m=>m[1]);const [width,height,colors]=lines[0].trim().split(/\s+/).map(Number);return {letter,width,height,mask:lines.slice(1+colors).slice(0,height).join('').split('').map(c=>c==='.'?1:0)};});
   for(let word=0;word<step.count;word++){
    const visible=await page.evaluate(glyphs=>{
     const c=document.querySelector('canvas'),pixels=c.getContext('2d').getImageData(0,0,c.width,c.height).data;
     const color=(x,y)=>{const n=(y*c.width+x)*4;return (pixels[n]<<16)|(pixels[n+1]<<8)|pixels[n+2];};
     const y=300,cw=21,ch=42;let start=-1;
     for(let x=0;x<c.width;x++)if(color(x,y)===0xE2B714&&color(x,y+ch-1)===0xE2B714){start=x;break;}
     if(start<0)throw Error('No visible typing caret in the original classic theme');let result='';
     for(let cell=0;cell<20;cell++){const x=start+cell*cw;if(x+cw>c.width)break;const foreground=cell===0?0x323437:0x646669;let best=null,bestError=Infinity;
      for(const glyph of glyphs){let mismatch=0;for(let row=0;row<ch;row++)for(let col=2;col<cw;col++){const actual=color(x+col,y+row)===foreground?1:0;if(actual!==glyph.mask[row*cw+col])mismatch++;}if(mismatch<bestError){bestError=mismatch;best=glyph.letter;}}
      if(bestError>2){if(cell===0)throw Error('Ambiguous rendered typing glyph');break;}result+=best;
     }
     if(!result)throw Error('No rendered word recognized');return result;
    },glyphs);
    for(const ch of visible){await key(ch.toUpperCase(),true);await wait(45);await key(ch.toUpperCase(),false);await wait(45);}await key('SPACE',true);await wait(60);await key('SPACE',false);await wait(100);console.log(plan.name+': typed visible word '+visible);
   }
  }
  else if(step.op==='panda-type-visible'){
   // Match the original 16x26 bitmap letters on the rendered first line.
   // Submit every recognized word through ordinary guest keys.
   const source=readFileSync(path.join(process.cwd(),step.fontFile),'utf8');
   const glyphs=[...source.matchAll(/KEY_([A-Z])_xpm\[\]\s*=\s*\{([\s\S]*?)\};/g)].map(m=>{
    const lines=[...m[2].matchAll(/"([^"]*)"/g)].map(v=>v[1]);
    if(lines[0]!=='16 26 2 1')throw Error('Unexpected original PandaType font geometry');
    return {letter:m[1],mask:lines.slice(3,29).join('').split('').map(v=>v==='!'?1:0)};
   });
   if(glyphs.length!==26)throw Error('Incomplete original PandaType alphabet');
   const words=await page.locator('canvas').evaluate((canvas,glyphs)=>{
    if(canvas.width!==1152||canvas.height!==864)throw Error('PandaType observer expects its original board');
    const pixels=canvas.getContext('2d').getImageData(0,0,canvas.width,canvas.height).data;let words=[],word='';
    for(let x=200;x+16<=952;x+=16){let mask=[],ink=0;
     for(let y=0;y<24;y++)for(let col=0;col<16;col++){const i=((200+y)*canvas.width+x+col)*4;const color=(pixels[i]<<16)|(pixels[i+1]<<8)|pixels[i+2];const present=[0x808080,0xffffff,0x336600,0xff0000].includes(color)?1:0;mask.push(present);ink+=present;}
     if(!ink){if(word){words.push(word);word='';}continue;}
     const scores=glyphs.map(g=>({letter:g.letter,error:mask.reduce((sum,v,i)=>sum+(v!==g.mask[i]?1:0),0)})).sort((a,b)=>a.error-b.error);
     if(scores[0].error!==0||scores[1].error===0)throw Error('Ambiguous original PandaType glyph at '+x+': '+JSON.stringify(scores.slice(0,2)));
     word+=scores[0].letter;
    }
    if(word)words.push(word);return words;
   },glyphs);
   const chosen=words.slice(step.startWord||0,(step.startWord||0)+step.count);if(chosen.length!==step.count)throw Error('Not enough complete visible PandaType words');
   for(const word of chosen){for(const letter of word){await key(letter,true);await wait(40);await key(letter,false);await wait(60);}await key('SPACE',true);await wait(70);await key('SPACE',false);await wait(100);console.log(plan.name+': typed original visible word '+word);}
  }
  else if(step.op==='golf-observed-turns'){
   const deadline=Date.now()+16000;
   while(golfPhase<step.until&&Date.now()<deadline){
    const ball=await page.locator('canvas').evaluate(c=>{
     if(c.width!==800||c.height!==600)throw Error('Golf observer expects its original board');
     const ctx=c.getContext('2d'),course=ctx.getImageData(760,440,1,1).data;if(!(course[1]>230&&course[0]<20&&course[2]<20))throw Error('Golf left the original course before its observed turns');
     const pixels=ctx.getImageData(0,30,c.width,c.height-30).data;let minX=c.width,minY=c.height;
     for(let i=0;i<pixels.length;i+=4)if(pixels[i]>240&&pixels[i+1]>240&&pixels[i+2]>240){minX=Math.min(minX,i/4%c.width);minY=Math.min(minY,Math.floor(i/4/c.width)+30);}
     return minX<c.width?[minX-1,minY]:null;
    });
    if(ball&&((golfPhase===0&&ball[0]>=236)||(golfPhase===1&&ball[1]<=320)||(golfPhase===2&&ball[0]>=430)||(golfPhase===3&&ball[1]<=60))){
     console.log(plan.name+': turn '+golfPhase+' from visible ball '+ball);await key('MOUSE1',true);await wait(65);await key('MOUSE1',false);await wait(25);golfPhase++;
    }else await wait(40);
   }
   if(golfPhase!==step.until)throw Error('Golf observed turn route did not reach '+step.until);
  }
  else if(step.op==='wait-food')await page.waitForFunction(()=>{const c=document.querySelector('canvas'),d=c.getContext('2d').getImageData(80,100,c.width-160,c.height-160).data;let count=0;for(let i=0;i<d.length;i+=16)if(d[i]>210&&d[i+1]>180&&d[i+2]>140)count++;return count>100;},null,{timeout:4000});
  else if(step.op==='wait-pixel')await page.waitForFunction(({x,y,condition,rgb,tolerance=0})=>{
   const c=document.querySelector('canvas'),p=c.getContext('2d').getImageData(x,y,1,1).data;
   if(Array.isArray(rgb)&&rgb.length===3)return rgb.every((value,index)=>Math.abs(p[index]-value)<=tolerance);
   return condition==='green'&&p[1]>p[0]*1.5&&p[1]>80;
  },step,{timeout:15000});
  else if(step.op==='mosu-target'){
   // Original ring index63 and cursor index36 render white/red in the legacy
   // EGA DAC. Follow those visible pixels using ordinary PS/2 motion.
   const observe=()=>page.locator('canvas').evaluate(c=>{
    if(c.width!==1024||c.height!==768)throw Error('MOsu observer expects the original 1024×768 board');
    const pixels=c.getContext('2d').getImageData(0,0,c.width,c.height).data;
    let x0=1024,y0=768,x1=-1,y1=-1,cx=1024,cy=768;
    for(let y=0;y<c.height;y++)for(let x=0;x<c.width;x++){const i=(y*c.width+x)*4,r=pixels[i],g=pixels[i+1],b=pixels[i+2];
     if(x<960&&y<650&&r===255&&g===255&&b===255){x0=Math.min(x0,x);y0=Math.min(y0,y);x1=Math.max(x1,x);y1=Math.max(y1,y);}
     if(r===255&&g===0&&b===0){cx=Math.min(cx,x);cy=Math.min(cy,y);}
    }
    return {ring:x1>=x0?{x:(x0+x1)/2,y:(y0+y1)/2,width:x1-x0,height:y1-y0}:null,cursor:cx<1024?[cx-2,cy]:null};
   });
   let aimed=false;const started=Date.now();
   while(Date.now()-started<10000){let state=await observe();const ring=state.ring;
    if(!ring||ring.width<215||ring.width>260||Math.abs(ring.width-ring.height)>30||!state.cursor){await wait(10);continue;}
    let dx=Math.round(ring.x-state.cursor[0]),dy=Math.round(state.cursor[1]-ring.y);
    const n=Math.max(1,Math.ceil(Math.max(Math.abs(dx),Math.abs(dy))/64));let oldX=0,oldY=0;
    for(let k=1;k<=n;k++){const x=Math.round(dx*k/n),y=Math.round(dy*k/n);await call('lcom_web_mouse',[x-oldX,y-oldY,0],['number','number','number']);oldX=x;oldY=y;await wait(25);}
    cursor=[ring.x,ring.y];
    for(let tick=0;tick<100;tick++){state=await observe();if(!state.ring||Math.abs(state.ring.x-ring.x)>25||Math.abs(state.ring.y-ring.y)>25)break;
     if(state.ring.width<=180&&state.ring.height<=180){if(!state.cursor||Math.abs(state.cursor[0]-ring.x)>30||Math.abs(state.cursor[1]-ring.y)>30)throw Error('MOsu cursor did not reach the visible target');aimed=true;break;}
     await wait(10);
    }
    if(aimed)break;
   }
   if(!aimed)throw Error('No fresh MOsu target reached its visible hit window');
  }
  else if(step.op==='sketch-recall-visible'){
   // Match the original bitmap prompt. Only normal guest keys submit the recall.
   const lines=[...readFileSync(path.join(process.cwd(),step.fontFile),'utf8').matchAll(/"([^"]*)"/g)].map(m=>m[1]);
   const [width,height,colors]=lines[0].split(/\s+/).map(Number);
   if(width!==1820||height!==70)throw Error('Unexpected original Sketch font geometry');
   const black=lines.slice(1,colors+1).filter(v=>/#000000/i.test(v)).map(v=>v[0]);
   const pixels=lines.slice(colors+1,colors+71),alphabet='qwertyuiopasdfghjklzxcvbnm';
   const glyphs=[...alphabet].map((letter,index)=>({letter,mask:pixels.flatMap(row=>[...row.slice(index*70,index*70+60)].map(ch=>black.includes(ch)?1:0))}));
   const word=await page.locator('canvas').evaluate((c,glyphs)=>{
    const d=c.getContext('2d').getImageData(0,0,c.width,c.height).data;let word='';
    for(let cell=0;cell<12;cell++){const mask=[];let ink=0;for(let y=780;y<850;y++)for(let x=80+cell*60;x<140+cell*60;x++){const i=(y*c.width+x)*4,v=d[i]===0&&d[i+1]===0&&d[i+2]===0?1:0;mask.push(v);ink+=v;}if(!ink)break;
     const ranked=glyphs.map(g=>({letter:g.letter,error:mask.reduce((sum,v,i)=>sum+(v!==g.mask[i]?1:0),0)})).sort((a,b)=>a.error-b.error);
     if(ranked[0].error!==0||ranked[1].error===0)throw Error('Ambiguous original recall glyph '+cell+': '+JSON.stringify(ranked.slice(0,2)));word+=ranked[0].letter;
    }if(!word)throw Error('No original prompt visible');return word;
   },glyphs);
   console.log(plan.name+': read original visible prompt '+word);await wait(6500);
   for(const ch of word){await key(ch.toUpperCase(),true);await wait(50);await key(ch.toUpperCase(),false);await wait(50);}await key('ENTER',true);await wait(100);await key('ENTER',false);await wait(200);
  }
  else if(step.op==='standing-territory-loop'){
   const position=()=>page.locator('canvas').evaluate(c=>{if(c.width!==800||c.height!==600)throw Error('Territory observer expects original800x600');const d=c.getContext('2d').getImageData(0,0,800,560).data,xs=[],ys=[];for(let y=0;y<560;y++)for(let x=0;x<800;x++){let i=(y*800+x)*4;if(d[i]===125&&d[i+1]===0&&d[i+2]===0){xs.push(x);ys.push(y);}}return xs.length?[Math.min(...xs),Math.min(...ys)]:null;});
   for(const [direction,coordinate,threshold,increasing]of[['D',0,315,true],['S',1,435,true],['A',0,205,false],['W',1,305,false]]){
    await key(direction,true);await wait(60);await key(direction,false);let reached=false;
    for(let attempt=0;attempt<200;attempt++){const p=await position();if(p&&(increasing?p[coordinate]>=threshold:p[coordinate]<=threshold)){console.log(plan.name+': visible territory turn '+direction+' '+p);reached=true;break;}await wait(35);}
    if(!reached)throw Error('Original territory route did not reach '+direction);
   }await wait(250);
  }
  else if(step.op==='green-circuit-food'){
   const result=await page.evaluate(async goal=>{
    const c=document.querySelector('canvas');if(c.width!==1152||c.height!==864)throw Error('Green Circuit observer expects its original board');
    const pause=ms=>new Promise(r=>setTimeout(r,ms));let last=window.arcadeGreenDirection||'DOWN';
    for(let attempt=0;attempt<500;attempt++){
     const d=c.getContext('2d').getImageData(0,0,c.width,c.height).data;let body=[],walls=[],head=null,apple=null;
     for(let gy=1;gy<=16;gy++)for(let gx=1;gx<=22;gx++){let dark=0,white=0,red=0,brick=0;for(let y=gy*48;y<(gy+1)*48;y++)for(let x=gx*48;x<(gx+1)*48;x++){let i=(y*c.width+x)*4,r=d[i],g=d[i+1],b=d[i+2];if(r<60&&g>65&&g<160&&b<80)dark++;if(r>180&&g>180&&b>180)white++;if(r>150&&g<80&&b<80)red++;if(r>140&&g>60&&g<150&&b>25&&b<90)brick++;}if(dark>1000){body.push([gx,gy]);if(white>25)head=[gx,gy];}if(red>500)apple=[gx,gy];if(brick>1000)walls.push([gx,gy]);}
     if(body.length>=goal){window.arcadeGreenDirection=last;return body.length;}
     if(head&&apple){const seen=new Set(),q=[[...head,[]]];let route;
      while(q.length){const[x,y,keys]=q.shift(),id=x+','+y;if(seen.has(id))continue;seen.add(id);if(x===apple[0]&&y===apple[1]){route=keys;break;}for(const[k,dx,dy]of[['RIGHT',1,0],['DOWN',0,1],['LEFT',-1,0],['UP',0,-1]]){const nx=x+dx,ny=y+dy;if(nx<1||nx>22||ny<1||ny>16||walls.some(p=>p[0]===nx&&p[1]===ny)||body.some(p=>p[0]===nx&&p[1]===ny))continue;if(!keys.length&&({RIGHT:'LEFT',LEFT:'RIGHT',UP:'DOWN',DOWN:'UP'})[k]===last)continue;q.push([nx,ny,[...keys,k]]);}}
      if(route?.length&&route[0]!==last){last=route[0];for(const down of[1,0])arcadeGalleryPlayer.module.ccall('lcom_web_key',null,['string','number'],[last,down]);}
     }await pause(20);
    }throw Error('No original Green Circuit food collection observed');
   },step.goal);console.log(plan.name+': visible snake length '+result);
  }
  else if(step.op==='checker-garden-food'){
   const result=await page.evaluate(async goal=>{
    const c=document.querySelector('canvas');if(c.width!==800||c.height!==600)throw Error('Checker Garden observer expects its original board');
    const pause=ms=>new Promise(r=>setTimeout(r,ms));let last=window.arcadeCheckerDirection||'RIGHT';const started=performance.now();
    while(performance.now()-started<35000){
     const d=c.getContext('2d').getImageData(0,0,c.width,c.height).data;let body=[],head=null,apple=null;
     for(let y=1;y<14;y++)for(let x=1;x<19;x++){const i=((y*40+20)*c.width+x*40+20)*4;if(d[i]===28&&d[i+1]===70&&d[i+2]===158)head=[x,y];if(d[i+2]>130&&d[i]<100&&d[i+1]<150)body.push([x,y]);}
     if(body.length>=goal){window.arcadeCheckerDirection=last;return body.length;}
     for(let y=40;y<560&&!apple;y++)for(let x=40;x<760;x++){const i=(y*c.width+x)*4;if(d[i]>170&&d[i+1]<120&&d[i+2]<120){apple=[Math.floor(x/40),Math.floor(y/40)];break;}}
     if(head&&apple){const seen=new Set(),q=[[...head,[]]];let route;
      while(q.length){const[x,y,keys]=q.shift(),id=x+','+y;if(seen.has(id))continue;seen.add(id);if(x===apple[0]&&y===apple[1]){route=keys;break;}for(const[k,dx,dy]of[['RIGHT',1,0],['DOWN',0,1],['LEFT',-1,0],['UP',0,-1]]){const nx=x+dx,ny=y+dy;if(nx<1||nx>18||ny<1||ny>13||body.some(p=>p[0]===nx&&p[1]===ny))continue;q.push([nx,ny,[...keys,k]]);}}
      if(route?.length&&route[0]!==last){last=route[0];arcadeGalleryPlayer.module.ccall('lcom_web_key',null,['string','number'],[last,1]);arcadeGalleryPlayer.module.ccall('lcom_web_key',null,['string','number'],[last,0]);}
     }await pause(20);
    }throw Error('No original Checker Garden apple collection observed');
   },step.goal);console.log(plan.name+': visible snake length '+result);
  }
  else if(step.op==='spacecom-hit'){
   // Match the original score digit and aim only at rendered gray asteroids.
   const glyphs=Array.from({length:10},(_,digit)=>{
    const source=readFileSync(path.join(process.cwd(),step.fontDirectory,'char_'+digit+'.xpm'),'utf8'),lines=[...source.matchAll(/"([^"]*)"/g)].map(m=>m[1]);
    const [w,h,n,cpp]=lines[0].trim().split(/\s+/).map(Number);if(w!==32||h!==32)throw Error('Unexpected original SpaceCOM digit geometry');
    const colors=new Map(lines.slice(1,n+1).map(line=>{const color=line.match(/ c #(\w{6})/);return [line.slice(0,cpp),color?parseInt(color[1].slice(0,2),16):0];}));
    return {digit,mask:lines.slice(n+1,n+1+h).flatMap(line=>Array.from({length:w},(_,x)=>(colors.get(line.slice(x*cpp,(x+1)*cpp))||0)>80?1:0))};
   });
   const score=()=>page.locator('canvas').evaluate((c,glyphs)=>{
    if(c.width!==800||c.height!==600)throw Error('SpaceCOM observer expects its original board');const p=c.getContext('2d').getImageData(292,10,32,32).data;
    const scores=glyphs.map(g=>({digit:g.digit,error:g.mask.reduce((sum,v,i)=>sum+(v!==(p[i*4]>80&&p[i*4+1]>80&&p[i*4+2]>80?1:0)?1:0),0)})).sort((a,b)=>a.error-b.error);
    if(scores[0].error>2||scores[1].error===scores[0].error)throw Error('Ambiguous original SpaceCOM score digit '+JSON.stringify(scores.slice(0,2)));return scores[0].digit;
   },glyphs);
   let selected=0,positive=false;
   for(let n=0;n<40;n++){
    if(await score()>0){positive=true;break;}
    const target=await page.locator('canvas').evaluate(c=>{const d=c.getContext('2d').getImageData(0,0,c.width,c.height).data,points=[];for(let y=60;y<590;y++)for(let x=10;x<790;x++){const r=Math.hypot(x-400,y-300),i=(y*c.width+x)*4;if(r>130&&d[i]===d[i+1]&&d[i+1]===d[i+2]&&d[i]>90&&d[i]<210)points.push([r,x,y]);}points.sort((a,b)=>a[0]-b[0]);return points[0]?.slice(1)||null;});
    if(!target){await wait(70);continue;}
    const sector=(Math.round(Math.atan2(target[0]-400,300-target[1])/(Math.PI/4))+8)%8,diff=(sector-selected+8)%8,direction=diff<=4?'RIGHT':'LEFT';
    for(let k=0;k<Math.min(diff,8-diff);k++){await key(direction,true);await wait(25);await key(direction,false);await wait(90);}selected=sector;
    await key('DOWN',true);await wait(25);await key('DOWN',false);await wait(90);await key('SPACE',true);await wait(25);await key('SPACE',false);await wait(90);
   }
   if(!positive&&await score()<=0)throw Error('No original SpaceCOM positive hit score observed');console.log(plan.name+': visible score '+await score());
  }
  else if(step.op==='snake'){
   const observation=await page.evaluate(async goal=>{
    const canvas=document.querySelector('canvas'),ctx=canvas.getContext('2d');
    if(canvas.width!==1024||canvas.height!==768)throw Error('Snake observer expects the original 1024×768 board');
    const sleep=ms=>new Promise(r=>setTimeout(r,ms));const state=window.arcadeSnakeObserver||{previous:[],last:'D'};let started=performance.now();
    const id=p=>p.join(',');
    while(performance.now()-started<70000){
     const pixels=ctx.getImageData(212,159,600,450).data,body=[];let food;
     for(let y=0;y<45;y++)for(let x=0;x<60;x++){let n=((y*10+5)*600+x*10+5)*4;if(pixels[n]===255&&pixels[n+1]===226&&pixels[n+2]===0)body.push([x,y]);if(pixels[n]===255&&pixels[n+1]===64&&pixels[n+2]===0)food=[x,y];}
     if(body.length>=goal){window.arcadeSnakeObserver=state;return {length:body.length,elapsedMs:Math.round(performance.now()-started)};}
     const fresh=body.filter(p=>!state.previous.some(q=>id(p)===id(q)));
     if(fresh.length&&food){const head=fresh[0],occupied=new Set(body.map(id)),seen=new Set([id(head)]),queue=[[head,null]];let choice;
      while(queue.length){const [position,first]=queue.shift();if(id(position)===id(food)){choice=first;break;}for(const [dx,dy,k]of[[1,0,'D'],[-1,0,'A'],[0,1,'S'],[0,-1,'W']]){if(!first&&({D:'A',A:'D',S:'W',W:'S'})[state.last]===k)continue;const next=[position[0]+dx,position[1]+dy],key=id(next);if(next[0]<0||next[1]<0||next[0]>=60||next[1]>=45||occupied.has(key)||seen.has(key))continue;seen.add(key);queue.push([next,first||k]);}}
      if(choice){arcadeGalleryPlayer.module.ccall('lcom_web_key',null,['string','number'],[choice,1]);arcadeGalleryPlayer.module.ccall('lcom_web_key',null,['string','number'],[choice,0]);state.last=choice;}
     }
     state.previous=body;await sleep(16);
    }
    throw Error('Snake did not reach the authored food objective');
   },step.goal);console.log(plan.name+': observed '+observation.length+' snake cells');
  }
  else if(step.op==='get-lost-star'){
   // Read only the rendered original maze. Normal arrow presses move the ball.
   const observe=()=>page.locator('canvas').evaluate(canvas=>{
    if(canvas.width!==1152||canvas.height!==864)throw Error('Get Lost observer expects the original 1152×864 board');
    const pixels=canvas.getContext('2d').getImageData(0,0,canvas.width,canvas.height).data;
    const rgb=(x,y)=>{const i=(y*canvas.width+x)*4;return [pixels[i],pixels[i+1],pixels[i+2]];};
    const clusters=predicate=>{
     const points=new Set(),groups=[];
     for(let y=240;y<580;y++)for(let x=410;x<755;x++)if(predicate(...rgb(x,y)))points.add(x+','+y);
     while(points.size){
      const first=points.values().next().value;points.delete(first);
      const queue=[first],group=[];
      while(queue.length){
       const [x,y]=queue.pop().split(',').map(Number);group.push([x,y]);
       for(const [dx,dy]of[[1,0],[-1,0],[0,1],[0,-1]]){
        const next=(x+dx)+','+(y+dy);if(points.delete(next))queue.push(next);
       }
      }
      if(group.length<=10)continue;
      const xs=group.map(p=>p[0]),ys=group.map(p=>p[1]);
      groups.push({count:group.length,x:Math.min(...xs),y:Math.min(...ys),cx:(Math.min(...xs)+Math.max(...xs))/2,cy:(Math.min(...ys)+Math.max(...ys))/2});
     }
     return groups;
    };
    const ball=clusters((r,g,b)=>r>100&&g<100&&b<100).sort((a,b)=>b.count-a.count)[0];
    const stars=clusters((r,g,b)=>r>200&&g>150&&b<80);
    if(!ball||!stars.length)throw Error('No visible Get Lost ball or stars');
    const open=(x,y)=>{
     if(x<420||x>742||y<250||y>563)return false;
     for(let yy=y;yy<=y+11;yy++)for(let xx=x;xx<=x+11;xx++){
      const [r,g,b]=rgb(xx,yy);if(r===230&&g===255&&b===255)return false;
     }
     return true;
    };
    const queue=[[ball.x,ball.y,[]]],seen=new Set([ball.x+','+ball.y]);
    for(let index=0;index<queue.length;index++){
     const [x,y,keys]=queue[index];
     if(stars.some(s=>Math.abs(s.cx-(x+5))<=8&&Math.abs(s.cy-(y+5))<=8))return {ball,stars,keys};
     for(const [dx,dy,key]of[[10,0,'RIGHT'],[-10,0,'LEFT'],[0,10,'DOWN'],[0,-10,'UP']]){
      const nx=x+dx,ny=y+dy,id=nx+','+ny;
      if(!seen.has(id)&&open(nx,ny)){seen.add(id);queue.push([nx,ny,[...keys,key]]);}
     }
    }
    throw Error('No visible Get Lost passage leads to a star');
   });
   const before=await observe();console.log(plan.name+': visible star route '+JSON.stringify(before));
   for(const direction of before.keys){await key(direction,true);await wait(80);await key(direction,false);await wait(100);}
   const after=await observe();
   if(after.stars.length>=before.stars.length)throw Error('Get Lost: no visible star removed after normal arrow inputs');
   console.log(plan.name+': visible stars '+before.stars.length+' -> '+after.stars.length);
  }
  else if(step.op==='piano-visible-hit'){
   // Read the original falling note at its authored hit line, then send one ordinary key.
   const end=Date.now()+(step.timeout||14000);let hit=null;
   while(Date.now()<end){
    hit=await page.locator('canvas').evaluate(canvas=>{
     if(canvas.width!==800||canvas.height!==600)throw Error('Unexpected Dr. Minix framebuffer');
     const context=canvas.getContext('2d'),y=485;
     for(let column=0;column<7;column++){
      const x=Math.floor((column+.5)*Math.floor(canvas.width/7));
      const pixel=context.getImageData(x,y,1,1).data;
      if(pixel[0]>30&&pixel[0]<100&&pixel[1]>90&&pixel[1]<180&&pixel[2]>220)return {column,pixel:[...pixel],x,y};
     }
     return null;
    });
    if(hit){const letter='ASDFGHJ'[hit.column];await key(letter,true);await wait(100);await key(letter,false);await wait(150);console.log(plan.name+': visible blue note '+JSON.stringify({...hit,key:letter}));break;}
    await wait(20);
   }
   if(!hit)throw Error('No visible Dr. Minix note reached the original hit line');
  }
  else if(step.op==='brick-chase-visible'){
   if(!['move','hit'].includes(step.goal))throw Error('Expected a visible paddle move or brick hit');
   const until=Date.now()+18000;let active=null,baseline=null,complete=false;
   const hold=async code=>{if(code===active)return;if(active)await key(active,false);active=code;if(code)await key(code,true);};
   try{while(Date.now()<until){
const observed=await page.locator('canvas').evaluate(canvas=>{
  const w=canvas.width,h=canvas.height,d=canvas.getContext('2d').getImageData(0,0,w,h).data;const white=i=>d[i*4]>240&&d[i*4+1]>240&&d[i*4+2]>240;
  let px=[];for(let y=550;y<580;y++)for(let x=0;x<w;x++)if(white(y*w+x))px.push(x);
  const paddle=px.length?{x:Math.min(...px),width:Math.max(...px)-Math.min(...px)+1}:null;
  const seen=new Uint8Array(w*h),clusters=[];
  for(let y=250;y<545;y++)for(let x=0;x<w;x++){
   const i=y*w+x;if(seen[i]||!white(i))continue;const q=[i];seen[i]=1;let minx=x,maxx=x,miny=y,maxy=y;
   for(let z=0;z<q.length;z++){const v=q[z],cx=v%w,cy=Math.floor(v/w);minx=Math.min(minx,cx);maxx=Math.max(maxx,cx);miny=Math.min(miny,cy);maxy=Math.max(maxy,cy);for(const n of[v-1,v+1,v-w,v+w])if(n>=250*w&&n<545*w&&!seen[n]&&white(n)){seen[n]=1;q.push(n);}}
   if(q.length>=10&&maxx-minx<22&&maxy-miny<22)clusters.push({x:minx,y:miny,width:maxx-minx+1,height:maxy-miny+1,count:q.length});
  }
  let bricks=0;for(let row=0;row<3;row++)for(let column=0;column<10;column++){const i=((150+row*32+16)*w+64*(column+1)+32)*4;if(d[i]===255&&d[i+1]===51&&d[i+2]===51)bricks++;}
  return{paddle,ball:clusters.sort((a,b)=>b.count-a.count)[0]||null,bricks};
 });
   if(!baseline){if(observed.bricks<28||!observed.paddle)throw Error('Expected original Breakout court');baseline=observed;}
   if(step.goal==='move'&&observed.paddle&&Math.abs(observed.paddle.x-baseline.paddle.x)>=30){complete=true;break;}
   if(step.goal==='hit'&&observed.bricks<baseline.bricks){complete=true;break;}
   if(observed.ball&&observed.paddle){const delta=observed.ball.x+observed.ball.width/2-observed.paddle.x-observed.paddle.width/2;await hold(delta>12?'D':delta< -12?'A':null);}
   await wait(15);
   }}finally{await hold(null);}
   if(!complete)throw Error('No original visible Breakout '+step.goal+' through normal A/D input');
   console.log(plan.name+': original visible '+step.goal+' completed');
  }
  else if(step.op==='rodolfo-sentence-visible'){
   // Identify the first two rendered words using the original public bitmap font.
   // Selection reads visible pixels only; every answer is ordinary guest input.
   if(!rodolfoSentence){
    const source=path.resolve(process.cwd(),step.sourceDirectory),font=readFileSync(path.join(source,'Modules/sprites.h'),'utf8'),glyphs={};
    for(const match of font.matchAll(/static xpm_row_t const (\w+)\[\] = \{([\s\S]*?)\};/g)){
     const lines=[...match[2].matchAll(/"([^"]*)"/g)].map(m=>m[1]);
     if(!lines.length)continue;const [width,height,colors]=lines[0].split(/\s+/).map(Number);
     const ink=new Set(lines.slice(1,colors+1).filter(v=>Number(v.slice(1).trim())!==0).map(v=>v[0]));
     glyphs[match[1]]={width,height,rows:lines.slice(colors+1,colors+1+height).map(row=>[...row].map(v=>ink.has(v)?1:0))};
    }
    glyphs[' ']=glyphs.space;glyphs['.']=glyphs.dot;glyphs[',']=glyphs.comma;
    const bank=readFileSync(path.join(source,'controller/game_controller.h'),'utf8').split('static char *easy_phrases[] = {')[1]?.split('};')[0];
    const sentences=bank?[...bank.matchAll(/"([^"]+)"/g)].map(m=>m[1]):[];
    if(sentences.length!==10)throw Error('Expected original ten easy sentences');
    const sentence=await page.locator('canvas').evaluate((canvas,{glyphs,sentences})=>{
     if(canvas.width!==1024||canvas.height!==768)throw Error('Original sentence frame must be1024x768');
     const pixels=canvas.getContext('2d').getImageData(0,0,canvas.width,canvas.height).data;
     const ranked=sentences.map(sentence=>{const text=sentence.split(' ').slice(0,2).join('  ');let width=0;for(const ch of text){if(!glyphs[ch])throw Error('Missing original font glyph '+ch);width+=glyphs[ch].width;}
      const mask=new Uint8Array(width*60);let x=0;for(const ch of text){const g=glyphs[ch];for(let row=0;row<g.height;row++)for(let col=0;col<g.width;col++)if(g.rows[row][col])mask[(60-g.height+row)*width+x+col]=1;x+=g.width;}
      let error=0;for(let y=0;y<60;y++)for(let x=0;x<width;x++){const i=((40+y)*1024+50+x)*4;const ink=pixels[i]>40||pixels[i+1]>40||pixels[i+2]>40?1:0;error+=ink!==mask[y*width+x];}return{sentence,error};
     }).sort((a,b)=>a.error-b.error);
     if(ranked[0].error!==0||ranked[1].error<20)throw Error('Ambiguous rendered original sentence '+JSON.stringify(ranked.slice(0,2)));
     return ranked[0].sentence;
    },{glyphs,sentences});
    rodolfoSentence={words:sentence.split(' '),index:0};console.log(plan.name+': screenshot-observed original sentence '+sentence);
   }
   const count=step.finish?rodolfoSentence.words.length-rodolfoSentence.index:step.count;
   if(!Number.isInteger(count)||count<1||rodolfoSentence.index+count>rodolfoSentence.words.length)throw Error('Invalid observed word count');
   for(let n=0;n<count;n++){
    const word=rodolfoSentence.words[rodolfoSentence.index++];
    for(const ch of word){const code=ch===','?'COMMA':ch==='.'?'PERIOD':ch.toUpperCase();await key(code,true);await wait(80);await key(code,false);await wait(70);}
    await key('SPACE',true);await wait(100);await key('SPACE',false);await wait(120);
   }
  }
  else if(step.op==='number-grid-empty'){
   const cell=await page.locator('canvas').evaluate(canvas=>{
    if(canvas.width!==1152||canvas.height!==864)throw Error('Expected original Number Grid frame');
    const pixels=canvas.getContext('2d').getImageData(0,0,1152,864).data;
    for(let row=0;row<9;row++)for(let column=0;column<9;column++){
     let ink=0;for(let y=191+55*row;y<231+55*row;y++)for(let x=344+55*column;x<369+55*column;x++){const i=(y*1152+x)*4;if(pixels[i]<50&&pixels[i+1]<50&&pixels[i+2]>160)ink++;}
     if(ink===0)return{row,column};
    }
    throw Error('No visibly empty cell in the generated board');
   });
   const tap=async code=>{await key(code,true);await wait(100);await key(code,false);await wait(100);};
   for(let i=0;i<cell.row;i++)await tap('DOWN');for(let i=0;i<cell.column;i++)await tap('RIGHT');await tap('1');
   console.log(plan.name+': normal digit input into visible empty cell '+JSON.stringify(cell));
  }
  else if(step.op==='candy-maze-visible-route'){
   if(!candyRouteState){
    const font=readFileSync(path.resolve(process.cwd(),step.sourceDirectory,'images/player.xpm'),'utf8');
    const lines=[...font.matchAll(/^\s*("(?:[^"\\]|\\.)*")\s*,?\s*$/gm)].map(m=>JSON.parse(m[1].replace(/\t/g,'\\t')));
    const [width,height,colors,cpp]=lines[0].trim().split(/\s+/).map(Number);if(width!==40||height!==38||cpp!==2)throw Error('Expected original40x38ghost');
    const palette=new Map(lines.slice(1,colors+1).map(line=>{const color=line.slice(cpp).trim().split(/\s+/).at(-1);return[line.slice(0,cpp),color==='None'?null:[1,3,5].map(i=>parseInt(color.slice(i,i+2),16))];}));
    const samples=[];for(let y=4;y<35;y+=8)for(let x=4;x<37;x+=8){const color=palette.get(lines[colors+1+y].slice(x*cpp,(x+1)*cpp));if(color)samples.push([x,y,...color]);}
    if(samples.length<12)throw Error('Insufficient original ghost landmarks');candyRouteState={samples,target:[60,540]};
   }
   const observe=()=>page.locator('canvas').evaluate((canvas,samples)=>{
    if(canvas.width!==800||canvas.height!==600)throw Error('Expected original800x600maze');const d=canvas.getContext('2d').getImageData(0,0,800,600).data;
    for(let y=0;y<=562;y++)for(let x=0;x<=760;x++){let match=true;for(const[dx,dy,r,g,b]of samples){const i=((y+dy)*800+x+dx)*4;if(d[i]!==r||d[i+1]!==g||d[i+2]!==b){match=false;break;}}if(match)return{x,y};}return null;
   },candyRouteState.samples);
   const routes={candy:[['RIGHT',24],['UP',8],['RIGHT',92],['UP',220],['RIGHT',96],['UP',32],['RIGHT',192],['DOWN',84]],exit:[['UP',164],['RIGHT',104],['UP',172],['RIGHT',104],['UP',4]]};
   const route=routes[step.through];if(!route)throw Error('Unknown original maze route');let transitioned=false;
   for(let index=0;index<route.length;index++){
    const[direction,distance]=route[index],dx=direction==='RIGHT'?1:direction==='LEFT'?-1:0,dy=direction==='DOWN'?1:direction==='UP'?-1:0;candyRouteState.target[0]+=dx*distance;candyRouteState.target[1]+=dy*distance;let arrived=false;
    for(let attempt=0;attempt<80;attempt++){
     let point=await observe();for(let retry=0;!point&&retry<20;retry++){await wait(50);point=await observe();}if(!point)throw Error('Original ghost absent beyond bounded level repaint');
     if(step.through==='exit'&&index===route.length-1&&Math.abs(point.x-30)<=8&&Math.abs(point.y-280)<=8){transitioned=arrived=true;break;}
     const ex=candyRouteState.target[0]-point.x,ey=candyRouteState.target[1]-point.y;if(Math.abs(ex)<=1&&Math.abs(ey)<=1){arrived=true;break;}
     const code=Math.abs(ex)>1?(ex>0?'RIGHT':'LEFT'):(ey>0?'DOWN':'UP'),remaining=Math.abs(ex)>1?Math.abs(ex):Math.abs(ey);await key(code,true);await wait(Math.max(8,Math.min(100,(remaining-4)/240*1000)));await key(code,false);await wait(40);
    }
    if(!arrived)throw Error('Ordinary arrows did not reach visible maze waypoint '+JSON.stringify(candyRouteState.target));if(transitioned)break;
   }
   await wait(200);console.log(plan.name+': screenshot-observed ordinary route '+step.through+' target '+candyRouteState.target);
  }
  else if(step.op==='sentence-sprint-visible'){
   // Read original public assets and visible pixels only. All typing is guest input.
   if(!sentenceSprint){
    const source=path.resolve(process.cwd(),step.sourceDirectory),glyphs={};
    for(const letter of "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.,' "){
     const file=/[a-z]/.test(letter)?'little/'+letter+'.xpm':/[A-Z]/.test(letter)?'big/'+letter+'.xpm':({'.':'pontofinal.xpm',',':'virgula.xpm',"'":'apost.xpm',' ':'space.xpm'})[letter];
     const text=readFileSync(path.join(source,'sprites/Abc',file),'utf8');
     const lines=[...text.matchAll(/"((?:\\.|[^"\\])*)"/g)].map(m=>JSON.parse('"'+m[1].replace(/\t/g,'\\t')+'"'));
     const[width,height,colors,cpp]=lines[0].split(/\s+/).map(Number);
     if(height!==40||cpp!==1)throw Error('Unexpected Sentence Sprint original font geometry');
     const black=new Set(lines.slice(1,colors+1).filter(v=>/c\s+#000000$/i.test(v)).map(v=>v[0]));
     glyphs[letter]={width,height,ink:lines.slice(colors+1).flatMap((row,y)=>[...row].flatMap((v,x)=>black.has(v)?[[x,y]]:[]))};
    }
    const sentences=[...readFileSync(path.join(source,'sentences.h'),'utf8').matchAll(/\{"([^"\n]+)"\}/g)].slice(0,30).map(m=>m[1]);
    if(sentences.length!==30)throw Error('Expected original30 public sentences');
    const sentence=await page.locator('canvas').evaluate((canvas,{glyphs,sentences})=>{
     if(canvas.width!==1024||canvas.height!==768)throw Error('Sentence observer expects original1024x768');
     const pixels=canvas.getContext('2d').getImageData(162,134,680,40).data;
     const actual=Array.from({length:680*40},(_,i)=>pixels[4*i]<20&&pixels[4*i+1]<20&&pixels[4*i+2]<20?1:0);
     const scores=sentences.map(sentence=>{let width=0,ink=[];for(const ch of sentence.slice(0,20)){const glyph=glyphs[ch];ink.push(...glyph.ink.map(([x,y])=>[width+x,y]));width+=glyph.width+1;}const mask=new Uint8Array(width*40);for(const[x,y]of ink)mask[y*width+x]=1;let error=0;for(let y=0;y<40;y++)for(let x=0;x<width;x++)error+=mask[y*width+x]!==actual[y*680+x];return{sentence,error};}).sort((a,b)=>a.error-b.error);
     if(scores[0].error!==0||scores[1].error<100)throw Error('Ambiguous visible original sentence '+JSON.stringify(scores.slice(0,2)));
     return scores[0].sentence;
    },{glyphs,sentences});
    sentenceSprint={sentence,index:0};console.log(plan.name+': screenshot-observed original sentence '+sentence);
   }
   if(!Number.isInteger(step.count)||step.count<1||sentenceSprint.index+step.count>=sentenceSprint.sentence.length)throw Error('Sentence gallery must stop before original terminal period');
   for(let count=0;count<step.count;count++){
    const ch=sentenceSprint.sentence[sentenceSprint.index],capital=/[A-Z]/.test(ch);
    if(capital){await key('LSHIFT',true);await wait(35);}
    const code=ch===' '?'SPACE':ch==="'"?'MINUS':ch===','?'COMMA':ch==='.'?'PERIOD':ch.toUpperCase();
    await key(code,true);await wait(65);await key(code,false);await wait(75);
    if(capital){await key('LSHIFT',false);await wait(35);}sentenceSprint.index++;
   }
   console.log(plan.name+': typed '+sentenceSprint.index+' visible original characters');
  }
  else if(step.op==='labwars-visible-opponent'){
   // Observe only original visible health-bar pixels. Send normal aim/fire input.
   const targets=await page.locator('canvas').evaluate(c=>{
    if(c.width!==1024||c.height!==768)throw Error('Unexpected original LabWars geometry');
    const d=c.getContext('2d').getImageData(0,0,c.width,c.height).data,groups=[];
    for(let y=20;y<c.height-60;y++){
     let left=c.width,right=-1,count=0;for(let x=10;x<c.width-10;x++){const i=4*(y*c.width+x);if(d[i]===0&&d[i+1]===153&&d[i+2]===0){left=Math.min(left,x);right=Math.max(right,x);count++;}}
     if(count<=20)continue;const last=groups.at(-1);
     if(last&&y===last.bottom+1&&Math.abs(left-last.left)<5)last.bottom=y;
     else groups.push({top:y,bottom:y,left,right});
    }
    return groups.filter(g=>g.bottom-g.top>2&&Math.abs((g.left+g.right)/2-512)>60).map(g=>({x:Math.round((g.left+g.right)/2),y:g.top+40}));
   });
   if(targets.length!==1)throw Error('Expected one visible stationary campaign opponent '+JSON.stringify(targets));
   const target=targets[0],gain=plan.gain||1,dx=Math.round((target.x-cursor[0])/gain),dy=Math.round((cursor[1]-target.y)/gain),n=Math.max(1,Math.ceil(Math.max(Math.abs(dx),Math.abs(dy))/64));let ox=0,oy=0;
   for(let k=1;k<=n;k++){const x=Math.round(dx*k/n),y=Math.round(dy*k/n);await call('lcom_web_mouse',[x-ox,y-oy,0],['number','number','number']);ox=x;oy=y;await wait(25);}
   cursor=[target.x,target.y];await wait(160);
   if(!Number.isInteger(step.shots)||step.shots<1||step.shots>8)throw Error('Bound LabWars gallery to at most eight normal shots');
   console.log(plan.name+': visible original opponent '+JSON.stringify(target));
   for(let i=0;i<step.shots;i++){await key('MOUSE1',true);await wait(65);await key('MOUSE1',false);await wait(350);}
  }
  else if(step.op==='stick-hero-visible'){
   // Observe the original orange platform, then hold/release the authored bridge key.
   const platform=await page.locator('canvas').evaluate(c=>{
    if(c.width!==800||c.height!==600)throw Error('Unexpected original Stick Hero canvas');
    const d=c.getContext('2d').getImageData(0,550,c.width,1).data,runs=[];
    for(let x=100;x<c.width;x++){const i=x*4;if(d[i]>240&&d[i+1]>120&&d[i+1]<190&&d[i+2]<20){if(!runs.length||x!==runs.at(-1).at(-1)+1)runs.push([]);runs.at(-1).push(x);}}
    const target=runs.find(a=>a.length>=18&&a.length<=105);if(!target)throw Error('No confident visible original landing platform');return [target[0],target.at(-1)];
   });
   const duration=Math.round(((platform[0]+platform[1])/2-80)*1000/300);
   if(duration<250||duration>2400)throw Error('Visible original platform outside supported first bridge range');
   console.log(plan.name+': visible platform '+JSON.stringify(platform)+', ordinary hold '+duration+'ms');
   await key('MOUSE1',true);await wait(duration);await key('MOUSE1',false);await wait(100);
  }
  else if(step.op==='super-cold-hit'){
   // Read only the original enemy colors and rendered score; send ordinary input.
   const observed=()=>page.locator('canvas').evaluate(c=>{
    if(c.width!==800||c.height!==600)throw Error('Super Cold observer expects original800x600');
    const d=c.getContext('2d').getImageData(0,0,800,600).data;let scoreInk=0;const groups=[];
    for(let y=10;y<26;y++)for(let x=122;x<136;x++){const i=(y*800+x)*4;if(d[i]!==255||d[i+1]!==255||d[i+2]!==255)scoreInk++;}
    for(let y=35;y<600;y++)for(let x=0;x<800;x++){const i=(y*800+x)*4;if(!((d[i]===134&&d[i+1]===8&&d[i+2]===8)||(d[i]===80&&d[i+1]===24&&d[i+2]===24)))continue;let g=groups.find(g=>Math.abs(g.x-x)<30&&Math.abs(g.y-y)<30);if(!g){g={x,y,n:0,sx:0,sy:0};groups.push(g);}g.n++;g.sx+=x;g.sy+=y;g.x=g.sx/g.n;g.y=g.sy/g.n;}
    return{positive:scoreInk>10,enemies:groups.filter(g=>g.n>20).sort((a,b)=>b.n-a.n)};
   });
   let success=false;
   for(let attempt=0;attempt<16;attempt++){
    const state=await observed();if(state.positive){console.log(plan.name+': actual original three-digit score after hit');success=true;break;}
    if(state.enemies.length){const target=state.enemies[0],aim=[Math.round(target.x),Math.round(target.y)],dx=aim[0]-cursor[0],dy=cursor[1]-aim[1],n=Math.max(1,Math.ceil(Math.max(Math.abs(dx),Math.abs(dy))/64));let ox=0,oy=0;for(let k=1;k<=n;k++){const x=Math.round(dx*k/n),y=Math.round(dy*k/n);await call('lcom_web_mouse',[x-ox,y-oy,0],['number','number','number']);ox=x;oy=y;await wait(15);}cursor=aim;await wait(60);await key('MOUSE1',true);await wait(120);await key('MOUSE1',false);await wait(80);}
    const direction=attempt%2?'A':'D';await key(direction,true);await wait(200);await key(direction,false);await wait(80);
   }
   if(!success&&!(await observed()).positive)throw Error('Super Cold normal aim/fire did not reach visible positive score within bounded volley');
  }
  else if(step.op==='typo-visible-phrase'){
   // Read only the displayed gold sentence, then send ordinary keyboard input.
   if(typoSentence===null){
   const folder=path.join(output,plan.id);mkdirSync(folder,{recursive:true});
   const file=path.join(folder,'visible-phrase-observation.png');
   const data=await page.locator('canvas').evaluate(c=>c.toDataURL().split(',')[1]);
   writeFileSync(file,Buffer.from(data,'base64'));
   const source=path.resolve(step.sourceDirectory||'examples/community/Xouteiro--TypeRacer-in-C/proj/src');
   const observed=spawnSync('python3',[path.join(process.cwd(),'scripts/observe_typo_phrase.py'),file,'--source-directory',source],{encoding:'utf8'});
   if(observed.status!==0)throw Error('Visible Typo Racer phrase observer failed: '+observed.stderr);
   const result=JSON.parse(observed.stdout);writeFileSync(path.join(folder,'visible-phrase-observation.json'),JSON.stringify(result,null,2));
   if(!result.accepted)throw Error('Visible Typo Racer phrase confidence rejected');
   typoSentence=result.best.phrase;
   }
   const count=step.characters===undefined?typoSentence.length:step.characters;
   if(!Number.isInteger(count)||count<=typoTyped||count>typoSentence.length)throw Error('Invalid visible-phrase character count');
   for(const ch of typoSentence.slice(typoTyped,count)){
    const code=ch===' '?'SPACE':ch.toUpperCase();await key(code,true);await wait(35);await key(code,false);await wait(75);
   }
   typoTyped=count;
  }
  else if(step.op==='lc-typer-visible-phrase'){
   // Read only visible original glyph pixels and public phrase data, then type normal keys.
   if(step.stage==='prefix'){
    const source=path.resolve(process.cwd(),step.sourceDirectory),font=readFileSync(path.join(source,'assets/font/alphabet.h'),'utf8'),glyphs={};
    for(const letter of 'abcdefghijklmnopqrstuvwxyz'){
     const match=font.match(new RegExp('\\b'+letter+'_xpm\\[\\]\\s*=\\s*\\{([\\s\\S]*?)\\};'));
     if(!match)throw Error('Missing original LC Typer glyph '+letter);
     const rows=[...match[1].matchAll(/"([^"]*)"/g)].map(m=>m[1]),[w,h,n,cpp]=rows[0].split(/\s+/).map(Number);
     if(w!==30||h!==57||cpp!==1)throw Error('Unexpected original LC Typer font geometry');
     const ink=new Set(rows.slice(1,n+1).filter(row=>/c\s+#000000$/i.test(row)).map(row=>row[0]));
     glyphs[letter]=rows.slice(n+1,n+1+h).map(row=>Array.from(row,ch=>ink.has(ch)?1:0));
    }
    glyphs[' ']=Array.from({length:57},()=>Array(30).fill(0));
    const phrases=readFileSync(path.join(source,'assets/phrases/phrases.txt'),'utf8').trim().split(/\r?\n/);
    const observed=await page.locator('canvas').evaluate((c,{glyphs,phrases})=>{
     if(c.width!==800||c.height!==600)throw Error('LC Typer observer expects original800x600');
     const data=c.getContext('2d').getImageData(0,0,c.width,c.height).data,rank=[];
     for(const phrase of phrases){let wrong=0,total=0;for(let i=0;i<12;i++)for(let y=0;y<57;y++)for(let x=6;x<30;x++){
      const at=((100+y)*c.width+15+i*35+x)*4,r=data[at],g=data[at+1],b=data[at+2],ink=(r>210&&g>180&&b>180)||(g>100&&g>r+60);
      wrong+=Number(ink)!==glyphs[phrase[i]][y][x]?1:0;total++;
     }rank.push({phrase,error:wrong/total});}
     rank.sort((a,b)=>a.error-b.error);if(rank[0].error>.025||rank[1].error-rank[0].error<.004)throw Error('Visible phrase is not uniquely identified: '+JSON.stringify(rank.slice(0,2)));return rank.slice(0,2);
    },{glyphs,phrases});
    plan._lcTyperObserved={phrase:observed[0].phrase,typed:0};console.log(plan.name+': matched visible public phrase '+JSON.stringify(observed));
   }else if(step.stage!=='finish')throw Error('Unknown LC Typer observation stage');
   const observed=plan._lcTyperObserved;if(!observed)throw Error('LC Typer visible prefix must be observed first');
   const end=step.stage==='prefix'?Math.min(18,observed.phrase.length-1):observed.phrase.length;
   for(;observed.typed<end;observed.typed++){const ch=observed.phrase[observed.typed],code=ch===' '?'SPACE':ch.toUpperCase();await key(code,true);await wait(step.keyMs||70);await key(code,false);await wait(25);}await wait(150);
  }
  else if(step.op==='capture'){await wait(60);const canvas=await page.locator('canvas').evaluate(c=>({data:c.toDataURL('image/png'),width:c.width,height:c.height}));const folder=path.join(output,plan.id);mkdirSync(folder,{recursive:true});const filename=path.join(folder,step.slug+'.png');writeFileSync(filename,Buffer.from(canvas.data.split(',')[1],'base64'));records.push({file:filename,slug:step.slug,caption:step.caption||step.slug,width:canvas.width,height:canvas.height,crop:step.crop});}
  else throw Error('Unknown action '+step.op);
 }
 for(const k of held)await key(k,false);
 const status=await page.locator('#status').textContent();if(status!=='Game running.')throw Error('Unexpected end status: '+status);
 const gameDir=path.join(dist,plan.url);const gameFiles=['game.wasm','game.js'].filter(f=>{try{return statSync(path.join(gameDir,f)).isFile()}catch{return false}});results.push({id:plan.id,name:plan.name,captures:records,errors,status,gameFiles:gameFiles.map(f=>({path:path.join(gameDir,f)}))});console.log(plan.name+': '+records.length+' captures');
 }catch(error){results.push({id:plan.id,name:plan.name,captures:records,errors,error:String(error)});console.log(plan.name+': ERROR '+error);}
 await context.close();writeFileSync(path.join(output,'capture-results.json'),JSON.stringify(results,null,2));
}}finally{await browser.close();await new Promise(resolve=>server.close(resolve));}
if(results.some(r=>r.error||r.errors.length))process.exitCode=1;
