import {Mosaic} from './mosaic.js';
import {Previews} from './previews.js';
import {GameSearch} from './search.js';
import {Surprise} from './surprise.js';
import {cue} from './audio.js';
import {bindGridNavigation} from './navigation.js';

const library = document.querySelector('#library');
const play = document.querySelector('#play');
const games = document.querySelector('#games');
const count = document.querySelector('#count');
const host = document.querySelector('#frame-host');
let catalog = [], selected = null, lastLink = null, gameSearch = null, surprise = null;
const previews = new Previews();
const back=document.querySelector('#back');
function placeReturn(layout) {
  const frame=host.querySelector('iframe');
  if(!frame || !layout.game || !Array.isArray(layout.occupied)) return;
  const bounds=frame.getBoundingClientRect(), style=getComputedStyle(play);
  const sx=bounds.width/layout.width,sy=bounds.height/layout.height;
  if(!Number.isFinite(sx) || !Number.isFinite(sy)) return;
  const transform=r=>({x:bounds.x+r.x*sx,y:bounds.y+r.y*sy,width:r.width*sx,height:r.height*sy});
  const game=transform(layout.game), occupied=layout.occupied.map(transform);
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
  if(event.data?.type === 'lcom-player-exit' && event.data.code === 0) location.hash = '';
});

async function fullscreen() {
  try { if (!document.fullscreenElement) await document.documentElement.requestFullscreen?.(); }
  catch { /* Mobile browsers without Fullscreen API still use the complete viewport. */ }
}
document.querySelector('#fullscreen').onclick = async () => {
  const before = !!document.fullscreenElement;
  if (document.fullscreenElement) await document.exitFullscreen(); else await fullscreen();
  if (before !== !!document.fullscreenElement) cue('toggle',{emphasis:'subtle'});
};
document.addEventListener('fullscreenchange', () => {
  document.querySelector('#fullscreen').setAttribute('aria-label', document.fullscreenElement ? 'Exit fullscreen' : 'Enter fullscreen');
});
function render() {
  const launch = link => {
    lastLink = link;
    cue('navigate',{emphasis:'subtle'});
    if (matchMedia('(pointer:coarse)').matches) fullscreen();
  };
  const tiles = catalog.map((game, index) => previews.tile(game, index, launch));
  const mosaic = new Mosaic(document.querySelector('#grid'), games, tiles, () => previews.stop());
  document.querySelector('#shuffle').onclick = () => {
    mosaic.shuffle();
    cue('select',{emphasis:'subtle'});
    count.textContent = `${catalog.length} games, shuffled`;
  };
  gameSearch = new GameSearch(catalog, link => {
    previews.stop(); launch(link);
  });
  surprise = new Surprise(catalog, game => {
    launch(document.querySelector('#surprise'));
    location.hash = 'game=' + encodeURIComponent(game.id);
  });
  document.querySelector('#surprise').addEventListener('click', () => previews.stop());
  bindGridNavigation(document.querySelector('#grid'),'.grid-utility, .game-launch',{
    geometry: element => element.closest('.game') || element,
  });
  count.textContent = `${catalog.length} games, ready to play`;
}
function route(force = false) {
  const id = new URLSearchParams(location.hash.slice(1)).get('game');
  const game = catalog.find(game => game.id === id);
  if (!force && game && selected?.id === game.id) return;
  surprise?.close();
  previews.stop();
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
    const icon = document.querySelector('.load-logo');
    icon.onerror = () => { icon.onerror = null; icon.src = 'assets/arcade.svg'; };
    icon.src = game.icon || 'assets/arcade.svg';
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
    if (lastLink?.closest('dialog')) gameSearch?.open(lastLink.dataset.id);
    else lastLink?.focus({preventScroll:true});
  }
}
document.querySelector('#back').onclick = () => { cue('navigate',{direction:'back',emphasis:'subtle'});location.hash = ''; };
window.addEventListener('hashchange', () => route());
document.querySelector('#retry').onclick = () => route(true);
document.querySelector('#cancel').onclick = () => { location.hash = ''; };
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
