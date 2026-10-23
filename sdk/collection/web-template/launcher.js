import {GameFilters} from './filters.js';
import {renderGameTags} from './tags.js';
import {Surroundings} from './surroundings.js';
import {Mosaic} from './mosaic.js';
import {Previews} from './previews.js';
import {GameSearch} from './search.js';
import {Surprise} from './surprise.js';
import {cue} from './audio.js';
import {bindGridNavigation} from './navigation.js';
import {loadFont} from './fonts.js';
import {renderInstructions} from './instructions.js';
import {renderCredits, renderRepository} from './credits.js';
import {sharedSlug, homeUrl, shareUrl, copyText} from './share.js';

const library = document.querySelector('#library');
const play = document.querySelector('#play');
const games = document.querySelector('#games');
const count = document.querySelector('#count');
const host = document.querySelector('#frame-host');
const surroundings = new Surroundings(host, link => launch(link));
const shared = sharedSlug();
let catalog = [], selected = null, lastLink = null, surprise = null, gameSearch = null, filters = null, visibleCatalog = [], taxonomy = [];
let playerLayout = null, shareMode = false;
const previews = new Previews();
const back=document.querySelector('#back');
const help=document.querySelector('#player-help');
const libraryHint=document.querySelector('#library-hint');
const shareButton=document.querySelector('#player-share');
const shareStatus=document.querySelector('#share-status');
let shareTimeout=null;
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
  for (const control of [document.querySelector('#player-share'), help]) if (!control.hidden && control.offsetWidth) occupied.push(control.getBoundingClientRect());
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
const instructionContent = document.querySelector('#game-instructions');
const loadFooter = document.querySelector('#load-footer');
const creditsSection = document.querySelector('#load-credits');
const repositoryLink = document.querySelector('#load-repository');
const compactActions = [document.querySelector('#compact-message'), document.querySelector('#show-controls')];
const setCompactActions = hidden => compactActions.forEach(element => { element.hidden = hidden; });
const startButton = document.querySelector('#start-game');
let ready = false, running = false, started = false, hasInstructions = false, progressStep = -1;
function showLoading() {
  loading.hidden = false;
  if (!loading.open) loading.showModal();
}
function hideLoading() {
  if (loading.open) loading.close();
  loading.hidden = true;
}
function enterGame() {
  started = true;
  library.hidden = true; play.classList.remove('starting'); hideLoading(); host.inert = false;
  if (ready && !shareMode) surroundings.start(selected,catalog);
  const frame = host.querySelector('iframe');
  frame?.contentWindow.postMessage({type:'lcom-player-instructions',paused:false},location.origin);
  frame?.contentWindow.focus();
  frame?.contentDocument?.querySelector('#game')?.focus({preventScroll:true});
}
function showInstructions() {
  if (!hasInstructions || !started) return;
  host.querySelector('iframe')?.contentWindow.postMessage({type:'lcom-player-instructions',paused:true},location.origin);
  host.inert = true;
  surroundings.reset(); resetLibraryHint();
  startButton.firstChild.textContent = 'Resume game';
  document.querySelector('#instruction-state').textContent = 'Game paused.';
  document.querySelector('#load-dismiss').setAttribute('aria-label','Return to game');
  showLoading(); startButton.focus({preventScroll:true});
}
startButton.onclick = enterGame;
help.onclick = showInstructions;
function allowStart() {
  const becameReady = startButton.disabled;
  startButton.disabled = false;
  // The download status already says "Opening game…"; the note covers a paused game.
  document.querySelector('#instruction-state').textContent = started ? 'Game paused.' : '';
  if (becameReady && loading.contains(document.activeElement)) startButton.focus({preventScroll:true});
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
    setCompactActions(false);
    document.querySelector('#load-status').textContent = 'Opening game…';
    // Only the instructions dialog keeps a status line beside its actions.
    document.querySelector('#load-progress').hidden = !hasInstructions;
    if (hasInstructions) {
      runningControls.hidden = true;
      setCompactActions(true);
      allowStart();
    }
  } else if (state.phase === 'error') {
    running = false; runningControls.hidden = true; setCompactActions(true);
    loading.classList.remove('running'); loading.classList.remove('ready');
    showLoading();
    document.querySelector('#load-error').hidden = false;
    document.querySelector('#load-progress').hidden = true;
    loadFooter.hidden = true;
    document.querySelector('#retry').focus({preventScroll:true});
  } else if (state.phase === 'ready') {
    const loadingHadFocus = loading.contains(document.activeElement);
    loading.classList.add('ready');
    runningControls.hidden = true;
    setCompactActions(true);
    // A reopened dialog shows the note, never a stale download status.
    document.querySelector('#load-progress').hidden = true;
    ready = true;
    if (hasInstructions && (!started || loading.open)) {
      allowStart();
    } else {
      library.hidden = true; play.classList.remove('starting'); hideLoading(); host.inert = false;
      if (!shareMode) surroundings.start(selected,catalog);
      if (loadingHadFocus || !play.contains(document.activeElement)) host.querySelector('iframe')?.contentWindow.focus();
    }
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
  if(event.data?.type === 'lcom-player-exit' && event.data.code === 0) returnToLibrary();
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
function updateKeyboardLock() {
  if(document.fullscreenElement && selected)navigator.keyboard?.lock?.(['Escape']).catch(()=>{});
  else navigator.keyboard?.unlock?.();
}
document.addEventListener('fullscreenchange', () => {
  updateKeyboardLock();
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
  // A shared game has no library behind it; its clean address leads back home.
  if (shareMode) { location.assign(homeUrl()); return; }
  location.hash = '';
}
document.addEventListener('keydown', event => {
  if (!play.hidden) play.classList.add('keyboard-player');
  if (!play.hidden && event.key === 'Escape' && event.shiftKey) {
    event.preventDefault();
    if(event.repeat)return;
    const child=host.querySelector('iframe')?.contentDocument;
    if(child?.pointerLockElement)child.defaultView.postMessage({type:'lcom-player-release'},location.origin);
    else if(document.pointerLockElement)document.exitPointerLock?.();
    else returnToLibrary();
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
    mosaic.setTiles(tiles.filter(tile=>ids.has(tile.game.id)),Object.values(filters.selected).some(values=>values.size));
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
  gameSearch = new GameSearch(catalog, launch, () => {filters.close(false);previews.stop();});
  surprise = new Surprise(catalog, game => {
    launch(document.querySelector('#surprise'));
    location.hash = 'game=' + encodeURIComponent(game.id);
  },taxonomy);
  document.querySelector('#surprise').addEventListener('click', () => previews.stop());
  bindGridNavigation(document.querySelector('#grid'),'.identity, .library-controls button, #browse-toggle, .game-launch',{
    geometry: element => element.closest('.game') || element,
  });
  count.textContent = `${catalog.length} games, ready to play`;
  filters.apply(Object.values(filters.selected).some(values=>values.size));
}
function route(force = false) {
  const id = new URLSearchParams(location.hash.slice(1)).get('game');
  const game = catalog.find(game => game.id === id) || (shared ? catalog.find(game => game.slug === shared) : null);
  shareMode = !!game && !id;
  if (!game) {
    play.classList.remove('keyboard-player');
    // An unknown share address still lands on the library, not on a dead URL.
    if (shared) history.replaceState(null, '', homeUrl());
  }
  if (!force && game && selected?.id === game.id) return;
  surprise?.close();
  previews.stop();
  surroundings.reset();
  resetLibraryHint();
  clearTimeout(shareTimeout); shareStatus.hidden = true; shareStatus.textContent = '';
  playerLayout=null;
  releasePointer();
  host.replaceChildren(); selected = game || null; ready = false; running = false; started = false; progressStep = -1;
  hasInstructions = renderInstructions(instructionContent, game?.instructions);
  help.hidden = !hasInstructions; startButton.hidden = !hasInstructions;
  loading.classList.toggle('with-instructions',hasInstructions);
  startButton.disabled = true; startButton.firstChild.textContent = 'Start game';
  document.querySelector('#instruction-state').textContent = '';
  document.querySelector('#load-dismiss').setAttribute('aria-label','Back to games');
  updateKeyboardLock();
  runningControls.hidden = true; setCompactActions(true); consoleOutput.hidden = true; consoleOutput.textContent = ''; progress.hidden = false;
  loadFooter.hidden = false;
  loading.classList.remove('running'); loading.classList.remove('ready');
  hideLoading(); host.inert = !!game;
  library.hidden = !!game && !lastLink; play.hidden = !game;
  play.classList.toggle('starting',!!game);
  document.body.style.overflow = game ? 'hidden' : '';
  if (game) {
    back.style.left='';back.style.top='';
    document.querySelector('#load-title').textContent = game.name;
    document.querySelector('#load-description').textContent = game.description || '';
    renderGameTags(document.querySelector('#load-tags'),game,taxonomy);
    renderCredits(creditsSection,game);
    renderRepository(repositoryLink,game);
    const icon = document.querySelector('.load-logo');
    icon.onerror = () => { icon.onerror = null; icon.src = 'assets/arcade.svg'; };
    icon.src = game.iconThumbnail || game.icon || 'assets/arcade.svg';
    document.querySelector('#load-error').hidden = true;
    document.querySelector('#load-progress').hidden = false;
    progress.removeAttribute('value');
    document.querySelector('#load-status').textContent = 'Loading game…';
    const frame = document.createElement('iframe'); frame.title = game.name; frame.src = game.url + '?collection=1&v=' + encodeURIComponent(game.revision || '') + (hasInstructions ? '&instructions=1' : '');
    frame.allow = 'autoplay; fullscreen'; host.append(frame);
    document.title = `${game.name} · ${library.querySelector('h1').textContent}`;
    frame.onerror = () => loadingState({phase:'error'});
    showLoading();
  } else {
    renderCredits(creditsSection,null);
    renderRepository(repositoryLink,null);
    document.title = library.querySelector('h1').textContent;
    if (lastLink?.closest('#search-panel')) gameSearch?.open(lastLink.dataset.id);
    else lastLink?.focus({preventScroll:true});
  }
}
back.onclick = returnToLibrary;
shareButton.onclick = async () => {
  const url = shareUrl(selected);
  const copied = await copyText(url);
  clearTimeout(shareTimeout);
  shareStatus.textContent = (copied ? 'Link copied · ' : 'Copy this link · ') + url;
  shareStatus.hidden = false;
  cue('select', {emphasis: 'subtle'});
  shareTimeout = setTimeout(() => { shareStatus.hidden = true; shareStatus.textContent = ''; }, 4000);
};
window.addEventListener('hashchange', () => route());
document.querySelector('#retry').onclick = () => route(true);
document.querySelector('#cancel').onclick = returnToLibrary;
const dismissLoading = () => {
  if (started && hasInstructions && document.querySelector('#load-error').hidden) enterGame();
  else returnToLibrary();
};
document.querySelector('#load-dismiss').onclick = dismissLoading;
// A modal dialog targets backdrop clicks at the dialog itself. Only a point
// truly outside its box dismisses, so padding and scrollbar clicks stay put.
loading.addEventListener('click', event => {
  if (event.target !== loading) return;
  const bounds = loading.getBoundingClientRect();
  if (event.clientX < bounds.left || event.clientX > bounds.right
      || event.clientY < bounds.top || event.clientY > bounds.bottom) dismissLoading();
});
loading.addEventListener('cancel',event => {event.preventDefault();dismissLoading();});
try {
  const response = await fetch(document.querySelector('link[data-catalog]')?.href || 'collection.json');
  if (!response.ok) throw new Error('The game library could not be loaded.');
  const data = await response.json(); catalog = data.games;
  library.querySelector('h1').textContent = data.name;
  render(data.taxonomy || []); route(); loadFont();
} catch (error) {
  count.textContent = 'Library unavailable';
  const message = document.querySelector('#error'); message.textContent = `${error.message} Reload to try again.`; message.hidden = false;
}
