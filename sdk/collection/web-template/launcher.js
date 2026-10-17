import {GameFilters} from './filters.js';
import {renderGameTags} from './tags.js';
import {Surroundings} from './surroundings.js';
import {Mosaic} from './mosaic.js';
import {Previews} from './previews.js';
import {bindSearchShortcuts} from './search.js';
import {Surprise} from './surprise.js';
import {cue} from './audio.js';
import {bindGridNavigation} from './navigation.js';

const library = document.querySelector('#library');
const play = document.querySelector('#play');
const games = document.querySelector('#games');
const count = document.querySelector('#count');
const host = document.querySelector('#frame-host');
const surroundings = new Surroundings(host, link => launch(link));
let catalog = [], selected = null, lastLink = null, surprise = null, filters = null, visibleCatalog = [], taxonomy = [];
let playerLayout = null;
const previews = new Previews();
const back=document.querySelector('#back');
const libraryHint=document.querySelector('#library-hint');
let hintDelay=null,hintTimeout=null;
function resetLibraryHint() {
  clearTimeout(hintDelay);clearTimeout(hintTimeout);libraryHint.hidden=true;
}
function pointerCaptureChanged(captured) {
  resetLibraryHint();
  if(!captured)return;
  hintDelay=setTimeout(()=>{
    libraryHint.hidden=false;
    hintTimeout=setTimeout(()=>{libraryHint.hidden=true;},4000);
  },1000);
}

function placeReturn(layout) {
  const frame=host.querySelector('iframe');
  if(!frame || !layout.game || !Array.isArray(layout.occupied)) return;
  const bounds=frame.getBoundingClientRect(), style=getComputedStyle(play);
  const sx=bounds.width/layout.width,sy=bounds.height/layout.height;
  if(!Number.isFinite(sx) || !Number.isFinite(sy)) return;
  playerLayout=layout;
  const transform=r=>({x:bounds.x+r.x*sx,y:bounds.y+r.y*sy,width:r.width*sx,height:r.height*sy});
  const game=transform(layout.game), occupied=layout.occupied.map(transform);
  occupied.push(document.querySelector('#player-fullscreen').getBoundingClientRect());
  surroundings.update(game,occupied);
  if(!back.offsetWidth) return;
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
const runningControls = document.querySelector('#load-running');
const consoleOutput = document.querySelector('#load-console');
let ready = false, running = false, progressStep = -1, loadStarted = 0, readyTimer = null;
const minimumLoading = 2000;
function showLoading() {
  loading.hidden = false;
  if (!loading.open) loading.showModal();
}
function hideLoading() {
  if (loading.open) loading.close();
  loading.hidden = true;
}
function loadingState(state) {
  if (state.phase === 'output') {
    if (!ready && typeof state.text === 'string') {
      consoleOutput.textContent = state.text.slice(-4096);
      consoleOutput.hidden = !consoleOutput.textContent;
      consoleOutput.scrollTop = consoleOutput.scrollHeight;
    }
  } else if (state.phase === 'running') {
    if (ready || !document.querySelector('#load-error').hidden) return;
    running = true;
    loading.classList.add('running');
    progress.hidden = true;
    runningControls.hidden = false;
    document.querySelector('#load-status').textContent = 'Opening game…';
  } else if (state.phase === 'error') {
    clearTimeout(readyTimer);
    running = false; runningControls.hidden = true;
    loading.classList.remove('running');
    showLoading();
    document.querySelector('#load-error').hidden = false;
    document.querySelector('#load-progress').hidden = true;
    document.querySelector('#retry').focus({preventScroll:true});
  } else if (state.phase === 'ready') {
    const remaining=minimumLoading-(performance.now()-loadStarted);
    if(remaining>0){clearTimeout(readyTimer);readyTimer=setTimeout(()=>loadingState(state),remaining);return;}
    const loadingHadFocus = loading.contains(document.activeElement);
    runningControls.hidden = true;
    ready = true; library.hidden = true; play.classList.remove('starting'); hideLoading(); host.inert = false;
    surroundings.start(selected,visibleCatalog);
    if (loadingHadFocus || !play.contains(document.activeElement)) host.querySelector('iframe')?.contentWindow.focus();
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
document.querySelector('#show-controls').onclick = () => {
  if (!running || ready || !document.querySelector('#load-error').hidden) return;
  // User chooses to interact early; only an actual draw completes readiness.
  library.hidden = true; play.classList.remove('starting'); hideLoading(); host.inert = false;
  const frame = host.querySelector('iframe');
  frame?.contentWindow.focus();
  frame?.contentDocument?.querySelector('#game')?.focus({preventScroll:true});
};
window.addEventListener('message', event => {
  if(event.origin !== location.origin || event.source !== host.querySelector('iframe')?.contentWindow) return;
  if(event.data?.type === 'lcom-player-layout') placeReturn(event.data);
  if(event.data?.type === 'lcom-player-loading') loadingState(event.data);
  if(event.data?.type === 'lcom-player-library') returnToLibrary();
  if(event.data?.type === 'lcom-player-pointer' && typeof event.data.captured === 'boolean') pointerCaptureChanged(event.data.captured);
  if(event.data?.type === 'lcom-player-input' && typeof event.data.keyboard === 'boolean') {
    play.classList.toggle('keyboard-player',event.data.keyboard);
    if(!event.data.keyboard && playerLayout) placeReturn(playerLayout);
  }
  if(event.data?.type === 'lcom-player-exit' && event.data.code === 0) location.hash = '';
});

async function fullscreen() {
  try { if (!document.fullscreenElement) await document.documentElement.requestFullscreen?.(); }
  catch { /* Mobile browsers without Fullscreen API still use the complete viewport. */ }
}
async function toggleFullscreen() {
  const before = !!document.fullscreenElement;
  try {
    if (document.fullscreenElement) await document.exitFullscreen(); else await fullscreen();
    if (before !== !!document.fullscreenElement) cue('toggle',{emphasis:'subtle'});
  } catch { /* Fullscreen can be unavailable under browser or display policy. */ }
}
for (const id of ['fullscreen','player-fullscreen']) document.querySelector('#'+id).onclick = toggleFullscreen;
document.addEventListener('fullscreenchange', () => {
  const label=document.fullscreenElement ? 'Exit fullscreen' : 'Enter fullscreen';
  for (const id of ['fullscreen','player-fullscreen']) {
    document.querySelector('#'+id).setAttribute('aria-label',label);
    if (id === 'player-fullscreen') document.querySelector('#'+id).title=label;
  }
});
function releasePointer() {
  if (document.pointerLockElement) document.exitPointerLock?.();
  const child=host.querySelector('iframe')?.contentDocument;
  if (child?.pointerLockElement) child.exitPointerLock?.();
}
function returnToLibrary() {
  if (play.hidden) return;
  releasePointer();
  cue('navigate',{direction:'back',emphasis:'subtle'});
  location.hash = '';
}
document.addEventListener('keydown', event => {
  if (!play.hidden) play.classList.add('keyboard-player');
  if (!play.hidden && event.key === 'Escape' && event.shiftKey) {
    event.preventDefault();returnToLibrary();
  }
});

function launch(link) {
  lastLink = link.closest('.game-rail') ? games.querySelector('[data-id="'+link.dataset.id+'"]') || document.querySelector('#browse-toggle') : link;
  filters?.close(false);
  cue('navigate',{emphasis:'subtle'});
  if (matchMedia('(pointer:coarse)').matches) fullscreen();
}
function render(tags) {
  taxonomy=tags;
  visibleCatalog = catalog;
  const tiles = catalog.map(game => previews.tile(game, launch));
  filters = new GameFilters(catalog,taxonomy,matches => {
    visibleCatalog = matches;
    const ids = new Set(matches.map(game=>game.id));
    mosaic.setTiles(tiles.filter(tile=>ids.has(tile.game.id)),filters.query.trim().length>0 || Object.values(filters.selected).some(values=>values.size));
    surprise.catalog = matches; document.querySelector('#surprise').disabled = !matches.length;
    document.querySelector('#filter-empty').hidden = !!matches.length;
    count.textContent = `${matches.length} of ${catalog.length} games`;
  });
  const mosaic = new Mosaic(document.querySelector('#grid'), games, tiles, () => previews.stop());
  document.querySelector('#shuffle').onclick = () => {
    mosaic.shuffle();
    cue('select',{emphasis:'subtle'});
    count.textContent = `${visibleCatalog.length} games, shuffled`;
  };
  bindSearchShortcuts(filters);
  surprise = new Surprise(catalog, game => {
    launch(document.querySelector('#surprise'));
    location.hash = 'game=' + encodeURIComponent(game.id);
  },taxonomy);
  document.querySelector('#surprise').addEventListener('click', () => previews.stop());
  bindGridNavigation(document.querySelector('#grid'),'.identity, .library-controls button, #browse-toggle, #search, .game-launch',{
    geometry: element => element.closest('.game') || element,
  });
  count.textContent = `${catalog.length} games, ready to play`;
  filters.apply(filters.query.trim().length>0 || Object.values(filters.selected).some(values=>values.size));
}
function route(force = false) {
  const id = new URLSearchParams(location.hash.slice(1)).get('game');
  const game = catalog.find(game => game.id === id);
  if (!game) play.classList.remove('keyboard-player');
  if (!force && game && selected?.id === game.id) return;
  surprise?.close();
  previews.stop();
  surroundings.reset();
  resetLibraryHint();
  clearTimeout(readyTimer);
  playerLayout=null;
  releasePointer();
  host.replaceChildren(); selected = game || null; ready = false; running = false; progressStep = -1;
  runningControls.hidden = true; consoleOutput.hidden = true; consoleOutput.textContent = ''; progress.hidden = false;
  loading.classList.remove('running');
  hideLoading(); host.inert = !!game;
  library.hidden = !!game && !lastLink; play.hidden = !game;
  play.classList.toggle('starting',!!game);
  document.body.style.overflow = game ? 'hidden' : '';
  if (game) {
    loadStarted=performance.now();
    back.style.left='';back.style.top='';
    document.querySelector('#load-title').textContent = game.name;
    document.querySelector('#load-description').textContent = game.description || '';
    renderGameTags(document.querySelector('#load-tags'),game,taxonomy);
    const icon = document.querySelector('.load-logo');
    icon.onerror = () => { icon.onerror = null; icon.src = 'assets/arcade.svg'; };
    icon.src = game.iconThumbnail || game.icon || 'assets/arcade.svg';
    document.querySelector('#load-error').hidden = true;
    document.querySelector('#load-progress').hidden = false;
    progress.removeAttribute('value');
    document.querySelector('#load-status').textContent = 'Loading game…';
    const frame = document.createElement('iframe'); frame.title = game.name; frame.src = game.url + '?collection=1&v=' + encodeURIComponent(game.revision || '') + '&startAt=' + (Date.now()+minimumLoading);
    frame.allow = 'autoplay; fullscreen'; host.append(frame);
    document.title = `${game.name} · ${library.querySelector('h1').textContent}`;
    frame.onerror = () => loadingState({phase:'error'});
    showLoading();
  } else {
    document.title = library.querySelector('h1').textContent;
    lastLink?.focus({preventScroll:true});
  }
}
back.onclick = returnToLibrary;
window.addEventListener('hashchange', () => route());
document.querySelector('#retry').onclick = () => route(true);
document.querySelector('#cancel').onclick = returnToLibrary;
document.querySelector('#load-dismiss').onclick = returnToLibrary;
loading.addEventListener('cancel',event => {event.preventDefault();returnToLibrary();});
try {
  const response = await fetch('collection.json', {cache:'no-cache'});
  if (!response.ok) throw new Error('The game library could not be loaded.');
  const data = await response.json(); catalog = data.games;
  library.querySelector('h1').textContent = data.name;
  render(data.taxonomy || []); route();
} catch (error) {
  count.textContent = 'Library unavailable';
  const message = document.querySelector('#error'); message.textContent = `${error.message} Reload to try again.`; message.hidden = false;
}
