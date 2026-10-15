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
const loading = document.querySelector('#loading');
const progress = loading.querySelector('progress');
let ready = false, progressStep = -1;
function loadingState(state) {
  if (state.phase === 'error') {
    loading.hidden = false;
    document.querySelector('#load-error').hidden = false;
    document.querySelector('#load-progress').hidden = true;
    document.querySelector('#retry').focus({preventScroll:true});
  } else if (state.phase === 'ready') {
    ready = true; loading.hidden = true; host.inert = false;
    host.querySelector('iframe')?.contentWindow.focus();
  } else if (!ready && document.querySelector('#load-error').hidden) {
    const fraction = state.total > 0 ? Math.min(1, state.loaded / state.total) : null;
    if (fraction === null) progress.removeAttribute('value');
    else { progress.max = state.total; progress.value = state.loaded; }
    // Announce meaningful progress, not every network chunk.
    const step = fraction === null ? -1 : Math.floor(fraction * 10);
    if (step !== progressStep || state.phase !== 'download') {
      progressStep = step;
      document.querySelector('#load-status').textContent = state.phase === 'starting' ? 'Starting game…' :
        fraction === null ? 'Loading game…' : `Downloading · ${(state.loaded/1048576).toFixed(1)} / ${(state.total/1048576).toFixed(1)} MB`;
    }
  }
}
window.addEventListener('message', event => {
  if(event.origin !== location.origin || event.source !== host.querySelector('iframe')?.contentWindow) return;
  if(event.data?.type === 'lcom-player-layout') placeReturn(event.data);
  if(event.data?.type === 'lcom-player-loading') loadingState(event.data);
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
  games.replaceChildren(...visible.map((game, index) => {
    const item = document.createElement('div'); item.className = 'game' + (!query && index === 0 ? ' featured' : '');
    item.style.setProperty('--hue', String((index * 43 + 245) % 360)); item.setAttribute('role','listitem');
    const link = document.createElement('a'); link.href = '#game=' + encodeURIComponent(game.id);
    link.setAttribute('aria-label', 'Play ' + game.name);
    const art = document.createElement('span'); art.className = 'art';
    if (game.preview || game.icon) {
      const image = document.createElement('img'); image.src = game.preview || game.icon; image.alt = ''; image.loading = index < 12 ? 'eager' : 'lazy'; image.decoding = 'async'; image.width = 800; image.height = 600; image.onerror = () => { image.onerror = null; if(game.icon && image.getAttribute('src') !== game.icon) image.src = game.icon; }; art.append(image);
    } else {
      const fallback = document.createElement('span'); fallback.className = 'fallback'; fallback.textContent = game.name[0]; fallback.setAttribute('aria-hidden','true'); art.append(fallback);
    }
    const mark = document.createElement('span'); mark.className = 'play-mark'; mark.innerHTML = '<svg viewBox="0 0 24 24"><path d="m8 4 13 8-13 8z"/></svg>'; mark.setAttribute('aria-hidden','true'); art.append(mark);
    const label = document.createElement('span'); label.className = 'game-name'; label.textContent = game.name;
    link.append(art,label); item.append(link);
    link.onclick = () => { lastLink = link; if (matchMedia('(pointer:coarse)').matches) fullscreen(); };
    return item;
  }));
  measureCards();
  count.textContent = query ? `${visible.length} of ${catalog.length} games` : `${catalog.length} games, ready to play`;
  document.querySelector('#empty').hidden = visible.length > 0;
}
function route(force = false) {
  const id = new URLSearchParams(location.hash.slice(1)).get('game');
  const game = catalog.find(game => game.id === id);
  if (!force && game && selected?.id === game.id) return;
  host.replaceChildren(); selected = game || null; ready = false; progressStep = -1;
  loading.hidden = !game; host.inert = !!game;
  library.hidden = !!game; play.hidden = !game;
  document.body.style.overflow = game ? 'hidden' : '';
  if (game) {
    back.style.left='';back.style.top='';
    document.querySelector('#load-title').textContent = game.name;
    document.querySelector('#load-description').textContent = game.description || '';
    const preview = document.querySelector('#load-preview');
    preview.src = game.preview || game.icon || 'assets/arcade.svg';
    document.querySelector('#load-error').hidden = true;
    document.querySelector('#load-progress').hidden = false;
    progress.removeAttribute('value');
    document.querySelector('#load-status').textContent = 'Loading game…';
    const frame = document.createElement('iframe'); frame.title = game.name; frame.src = game.url + '?collection=1&v=' + encodeURIComponent(game.revision || '');
    frame.allow = 'autoplay; fullscreen'; host.append(frame);
    document.title = `${game.name} · ${library.querySelector('h1').textContent}`;
    frame.onerror = () => loadingState({phase:'error'});
  } else {
    document.title = library.querySelector('h1').textContent;
    lastLink?.focus({preventScroll:true});
  }
}
document.querySelector('#back').onclick = () => { location.hash = ''; };
window.addEventListener('hashchange', () => route());
document.querySelector('#retry').onclick = () => route(true);
document.querySelector('#cancel').onclick = () => { location.hash = ''; };
search.oninput = render;
const searchToggle = document.querySelector('#search-toggle');
function toggleSearch(open) {
  searchToggle.setAttribute('aria-expanded', String(open));
  document.querySelector('#search-panel').hidden = !open;
  if (open) search.focus();
  else { search.value = ''; render(); searchToggle.focus(); }
}
searchToggle.onclick = () => toggleSearch(searchToggle.getAttribute('aria-expanded') !== 'true');
search.onkeydown = event => { if (event.key === 'Escape') { event.preventDefault(); toggleSearch(false); } };

let cardBounds = [], pointerFrame = null;
function measureCards() { cardBounds = [...games.children].map(element => ({element, rect:element.getBoundingClientRect()})); }
new ResizeObserver(measureCards).observe(games);
window.addEventListener('scroll', measureCards, {passive:true});
const fineMotion = matchMedia('(pointer:fine) and (prefers-reduced-motion:no-preference)');
games.addEventListener('pointermove', event => {
  if (!fineMotion.matches || pointerFrame !== null) return;
  const {clientX:x,clientY:y} = event;
  pointerFrame = requestAnimationFrame(() => {
    pointerFrame = null;
    document.querySelector('#ambience').style.setProperty('--x', `${x}px`);
    document.querySelector('#ambience').style.setProperty('--y', `${y}px`);
    for (const {element,rect} of cardBounds) {
      const distance = Math.hypot(x - rect.x - rect.width/2, y - rect.y - rect.height/2);
      element.style.setProperty('--near', String(Math.max(0, 1 - distance/340)));
    }
  });
});
games.addEventListener('pointerleave', () => { for (const {element} of cardBounds) element.style.setProperty('--near','0'); });
try {
  const response = await fetch('collection.json', {cache:'no-cache'});
  if (!response.ok) throw new Error('The game library could not be loaded.');
  const data = await response.json(); catalog = data.games;
  library.querySelector('h1').textContent = data.name;
  render(); route();
} catch (error) {
  count.textContent = 'Library unavailable';
  const message = document.querySelector('#error'); message.textContent = `${error.message} Reload to try again.`; message.hidden = false;
}
