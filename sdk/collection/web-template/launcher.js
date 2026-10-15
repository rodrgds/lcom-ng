const library = document.querySelector('#library');
const play = document.querySelector('#play');
const games = document.querySelector('#games');
const search = document.querySelector('#search');
const count = document.querySelector('#count');
const host = document.querySelector('#frame-host');
let catalog = [], selected = null, lastLink = null;
const back=document.querySelector('#back');
function placeReturn(layout) {
  const frame=host.querySelector('iframe');
  if(!frame || !layout.game || !Array.isArray(layout.occupied)) return;
  const bounds=frame.getBoundingClientRect(), style=getComputedStyle(play);
  const sx=bounds.width/layout.width,sy=bounds.height/layout.height;
  if(!Number.isFinite(sx) || !Number.isFinite(sy)) return;
  const transform=r=>({x:bounds.x+r.x*sx,y:bounds.y+r.y*sy,width:r.width*sx,height:r.height*sy});
  const game=transform(layout.game), occupied=layout.occupied.map(transform);
  if(layout.theme) {
    back.style.background=layout.theme.button;back.style.color=layout.theme.text;
    back.style.fontFamily=layout.theme.font==='mono'?'ui-monospace,monospace':'system-ui,sans-serif';
  }
  const w=back.offsetWidth,h=back.offsetHeight;
  const left=parseFloat(style.paddingLeft),top=parseFloat(style.paddingTop);
  const right=innerWidth-parseFloat(style.paddingRight)-w,bottom=innerHeight-parseFloat(style.paddingBottom)-h;
  const overlap=(a,b,gap=0)=>Math.max(0,Math.min(a.x+a.width,b.x+b.width+gap)-Math.max(a.x,b.x-gap))*Math.max(0,Math.min(a.y+a.height,b.y+b.height+gap)-Math.max(a.y,b.y-gap));
  const xs=[left,right,game.x-w-8,game.x+game.width+8,...occupied.flatMap(r=>[r.x-w-8,r.x+r.width+8])];
  const ys=[top,bottom,game.y-h-8,game.y+game.height+8,...occupied.flatMap(r=>[r.y-h-8,r.y+r.height+8])];
  let best=null;
  for(const x0 of xs) for(const y0 of ys) {
    const x=Math.max(left,Math.min(right,x0)),y=Math.max(top,Math.min(bottom,y0));
    const candidate={x,y,width:w,height:h};
    const collisions=occupied.reduce((sum,r)=>sum+overlap(candidate,r,4),0);
    const score=[collisions,overlap(candidate,game),x-left+y-top];
    if(!best || score.some((n,i)=>n<best.score[i] && score.slice(0,i).every((v,j)=>v===best.score[j]))) best={x,y,score};
  }
  if(best) { back.style.left=`${best.x}px`;back.style.top=`${best.y}px`; }
}
window.addEventListener('message',event=>{
  if(event.origin!==location.origin || event.source!==host.querySelector('iframe')?.contentWindow || event.data?.type!=='lcom-player-layout') return;
  placeReturn(event.data);
});

async function fullscreen() {
  try { if (!document.fullscreenElement) await document.documentElement.requestFullscreen?.(); }
  catch { /* Mobile browsers without Fullscreen API still use the complete viewport. */ }
}
document.querySelector('#fullscreen').onclick = async () => {
  if (document.fullscreenElement) await document.exitFullscreen(); else await fullscreen();
};
document.addEventListener('fullscreenchange', () => {
  document.querySelector('#fullscreen').setAttribute('aria-label', document.fullscreenElement ? 'Exit fullscreen' : 'Enter fullscreen');
});
function render() {
  const query = search.value.trim().toLocaleLowerCase();
  const visible = catalog.filter(game => game.name.toLocaleLowerCase().includes(query));
  games.replaceChildren(...visible.map(game => {
    const item = document.createElement('div'); item.className = 'game'; item.setAttribute('role','listitem');
    const link = document.createElement('a'); link.href = '#game=' + encodeURIComponent(game.id);
    link.setAttribute('aria-label', 'Play ' + game.name);
    const art = document.createElement('span'); art.className = 'art';
    if (game.icon) {
      const image = document.createElement('img'); image.src = game.icon; image.alt = ''; image.loading = 'lazy'; image.width = 256; image.height = 256; art.append(image);
    } else {
      const fallback = document.createElement('span'); fallback.className = 'fallback'; fallback.textContent = game.name[0]; fallback.setAttribute('aria-hidden','true'); art.append(fallback);
    }
    const mark = document.createElement('span'); mark.className = 'play-mark'; mark.textContent = '▶'; mark.setAttribute('aria-hidden','true'); art.append(mark);
    const label = document.createElement('span'); label.className = 'game-name'; label.textContent = game.name;
    link.append(art,label); item.append(link);
    link.onclick = () => { lastLink = link; if (matchMedia('(pointer:coarse)').matches) fullscreen(); };
    return item;
  }));
  count.textContent = query ? `${visible.length} of ${catalog.length} games` : `${catalog.length} games, ready to play`;
  document.querySelector('#empty').hidden = visible.length > 0;
}
function route() {
  const id = new URLSearchParams(location.hash.slice(1)).get('game');
  const game = catalog.find(game => game.id === id);
  if (game && selected?.id === game.id) return;
  host.replaceChildren(); selected = game || null;
  library.hidden = !!game; play.hidden = !game;
  document.body.style.overflow = game ? 'hidden' : '';
  if (game) {
    back.style.left='';back.style.top='';
    const frame = document.createElement('iframe'); frame.title = game.name; frame.src = game.url + "?collection=1";
    frame.allow = 'autoplay; fullscreen'; host.append(frame);
    document.title = `${game.name} · ${library.querySelector('h1').textContent}`;
    frame.onload = () => frame.contentWindow.focus();
  } else {
    document.title = library.querySelector('h1').textContent;
    lastLink?.focus({preventScroll:true});
  }
}
document.querySelector('#back').onclick = () => { location.hash = ''; };
window.addEventListener('hashchange',route);
search.oninput = render;
try {
  const response = await fetch('collection.json');
  if (!response.ok) throw new Error('The game library could not be loaded.');
  const data = await response.json(); catalog = data.games;
  library.querySelector('h1').textContent = data.name;
  render(); route();
} catch (error) {
  count.textContent = 'Library unavailable';
  const message = document.querySelector('#error'); message.textContent = `${error.message} Reload to try again.`; message.hidden = false;
}
