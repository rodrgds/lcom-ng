// The Python CLI owns image conversion, manifest checks and selection.
import {createServer} from 'node:http';
import {readFileSync,writeFileSync,mkdirSync,statSync} from 'node:fs';
import path from 'node:path';
import {createRequire} from 'node:module';
const require=createRequire(path.join(process.cwd(),'package.json'));
const {chromium}=require('playwright');
const [dist,planPath,output,...selected]=process.argv.slice(2);
const plans=JSON.parse(readFileSync(planPath)).games;
const mime={'.js':'text/javascript','.wasm':'application/wasm','.html':'text/html','.json':'application/json','.css':'text/css','.png':'image/png','.webp':'image/webp'};
const server=createServer((req,res)=>{try{const pathname=decodeURIComponent(new URL(req.url,'http://localhost').pathname);let file=path.resolve(dist,'.'+pathname);if(!file.startsWith(path.resolve(dist)+path.sep))throw Error('outside export');if(statSync(file).isDirectory())file=path.join(file,'index.html');let data=readFileSync(file);if(path.basename(file)==='player.js')data=Buffer.concat([data,Buffer.from('\nwindow.arcadeGalleryPlayer=player;\n')]);res.setHeader('Content-Type',mime[path.extname(file)]||'application/octet-stream');res.end(data);}catch{res.writeHead(404).end();}});
await new Promise(resolve=>server.listen(0,'127.0.0.1',resolve));
const browser=await chromium.launch({channel:'chrome',headless:true});const results=[];
try{for(const plan of plans){if(selected.length&&!selected.includes(plan.id)&&!selected.includes(plan.name))continue;
 const context=await browser.newContext({viewport:{width:1440,height:1080}});const page=await context.newPage();const errors=[];page.on('pageerror',e=>errors.push(String(e)));const records=[];const held=new Set();let cursor=plan.cursor||[0,0],buttons=0;
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
  else if(step.op==='wait-food')await page.waitForFunction(()=>{const c=document.querySelector('canvas'),d=c.getContext('2d').getImageData(80,100,c.width-160,c.height-160).data;let count=0;for(let i=0;i<d.length;i+=16)if(d[i]>210&&d[i+1]>180&&d[i+2]>140)count++;return count>100;},null,{timeout:4000});
  else if(step.op==='wait-pixel')await page.waitForFunction(({x,y,condition})=>{const c=document.querySelector('canvas'),p=c.getContext('2d').getImageData(x,y,1,1).data;return condition==='green'&&p[1]>p[0]*1.5&&p[1]>80;},step,{timeout:15000});
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
