import {prepareGame, instantiateGame} from './loading.js';

const canvas = document.querySelector('#game');
const status = document.querySelector('#status');
const area = document.querySelector('main');
const touch = matchMedia('(pointer: coarse)');
const keyboard = document.querySelector('#keyboard');
const controls = document.querySelector('#touch-controls');
const controlsToggle = document.querySelector('#controls-toggle');
const landscape = matchMedia('(orientation: landscape)');
let controlsCollapsed = false;
let mobile = null;
const fullscreenButton = document.querySelector("#fullscreen");
// The collection owns fullscreen so its Games return button stays in view.
const collectionPlayer = window.parent !== window && new URLSearchParams(location.search).get('collection') === '1';
let firstFrame = false;
function reportLoading(state) {
  if (collectionPlayer) parent.postMessage({type:'lcom-player-loading', ...state}, location.origin);
}
const keyboardToggle = document.querySelector('#keyboard-toggle');
const joystickToggle = document.querySelector('#joystick-toggle');
const joystick = document.querySelector('#joystick');
const joystickMapping = document.querySelector('#joystick-mapping');
let touchPointer = null;
let touchClickEnabled = true;
let tapTimer = null;
const mouseSources = new Map();
const functionToggle = document.createElement('button');
functionToggle.id = 'function-toggle';
functionToggle.type = 'button';
functionToggle.className = 'key';
functionToggle.textContent = 'Fn';
functionToggle.setAttribute('aria-label', 'Function keys');
functionToggle.setAttribute('aria-pressed', 'false');

const aliases = {
  Escape: 'ESC', Space: 'SPACE', Enter: 'ENTER', NumpadEnter: 'KP_ENTER', Backspace: 'BACKSPACE', Tab: 'TAB',
  ...Object.fromEntries(Array.from({length:10},(_,n)=>['Numpad'+n,'KP'+n])),
  ArrowUp: 'UP', ArrowDown: 'DOWN', ArrowLeft: 'LEFT', ArrowRight: 'RIGHT',
  ShiftLeft: 'LSHIFT', ShiftRight: 'RSHIFT', ControlLeft: 'LCTRL', ControlRight: 'RCTRL',
  AltLeft: 'LALT', AltRight: 'RALT', MetaLeft: 'LGUI', MetaRight: 'RGUI',
  Minus: 'MINUS', Equal: 'EQUAL', BracketLeft: 'LBRACKET', BracketRight: 'RBRACKET',
  Backslash: 'BACKSLASH', Semicolon: 'SEMICOLON', Quote: 'APOSTROPHE', Backquote: 'BACKQUOTE',
  Comma: 'COMMA', Period: 'PERIOD', Slash: 'SLASH', CapsLock: 'CAPSLOCK',
  Delete: 'DELETE', Insert: 'INSERT', Home: 'HOME', End: 'END', PageUp: 'PAGEUP', PageDown: 'PAGEDOWN',
};
const guestKey = code => aliases[code] || (/^Key[A-Z]$/.test(code) ? code.slice(3) : /^Digit[0-9]$/.test(code) ? code.slice(5) : /^F([1-9]|1[0-2])$/.test(code) ? code : null);

const player = {
  module: null, paused: false, held: new Map(), repeats: new Map(), buttons: 0, image: null,
  audioContext: null, audioSource: null, lastPointer: null,
  attach(module) {
    this.module = module;
    const c = mobile?.controls.cursor || {};
    module.ccall('lcom_web_cursor', null, Array(7).fill('number'), [c.x ?? -1,c.y ?? -1,c.maxX ?? -1,c.maxY ?? -1,c.minX ?? 0,c.minY ?? 0,c.bounds === 'reject' ? 1 : 0]);
    this.releaseAll();
  },
  key(code, down, source) {
    const key = guestKey(code);
    if (!this.module || !key) return;
    const sources = this.held.get(code) || new Set();
    const wasHeld = sources.size > 0;
    if (down) sources.add(source); else sources.delete(source);
    if (sources.size) this.held.set(code, sources); else this.held.delete(code);
    if (wasHeld !== (sources.size > 0)) this.module.ccall('lcom_web_key', null, ['string', 'number'], [key, down ? 1 : 0]);
    if (!wasHeld && sources.size && mobile?.controls.repeatKeys?.includes(key)) {
      const timer={id:null};
      const repeat=()=>{
        if(!this.module || !this.held.has(code) || this.repeats.get(code)!==timer) { this.repeats.delete(code);return; }
        this.module.ccall('lcom_web_key',null,['string','number'],[key,1]);
        timer.id=setTimeout(repeat,100);
      };
      timer.id=setTimeout(repeat,350);this.repeats.set(code,timer);
    }
    if(!sources.size) { clearTimeout(this.repeats.get(code)?.id);this.repeats.delete(code); }
    for (const button of document.querySelectorAll(`[data-code="${code}"]`)) button.classList.toggle('held', sources.size > 0);
  },
  releaseAll() {
    for (const [code, sources] of [...this.held]) for (const source of [...sources]) this.key(code, false, source);
    for (const button of document.querySelectorAll('.key[data-code][aria-pressed]')) button.setAttribute('aria-pressed', 'false');
    if (this.buttons) this.module?.ccall('lcom_web_mouse', null, ['number', 'number', 'number'], [0, 0, 0]);
    this.buttons = 0;
    this.lastPointer = null;
    mouseSources.clear();
    clearTimeout(tapTimer); tapTimer = null;
    clearTimeout(touchPointer?.holdTimer);
    touchPointer = null;
    resetJoystick();
    for (const button of controls.querySelectorAll('[data-mouse]')) button.setAttribute('aria-pressed', 'false');
  },
  present(bytes, width, height) {
    if (!this.image || canvas.width !== width || canvas.height !== height) {
      canvas.width = width; canvas.height = height;
      resizeGame();
      this.image = canvas.getContext('2d').createImageData(width, height);
    }
    this.image.data.set(bytes);
    canvas.getContext('2d').putImageData(this.image, 0, 0);
    if (!firstFrame) { firstFrame = true; reportLoading({phase:'ready'}); }
  },
  unlockAudio() {
    this.audioContext ||= new AudioContext();
    this.audioContext.resume().catch(error => console.warn(error.message));
  },
  audio(samples, rate, channels) {
    this.stopAudio();
    if (!this.audioContext || !samples.length) return;
    const frames = samples.length / channels;
    const buffer = this.audioContext.createBuffer(channels, frames, rate);
    for (let channel = 0; channel < channels; channel++) {
      const target = buffer.getChannelData(channel);
      for (let i = 0; i < frames; i++) target[i] = samples[i * channels + channel] / 32768;
    }
    this.audioSource = this.audioContext.createBufferSource();
    this.audioSource.buffer = buffer;
    this.audioSource.connect(this.audioContext.destination);
    this.audioSource.start();
  },
  stopAudio() { if (this.audioSource) { this.audioSource.stop(); this.audioSource.disconnect(); this.audioSource = null; } },
};

function resizeGame() {
  const scale = Math.min(area.clientWidth / canvas.width, area.clientHeight / canvas.height);
  canvas.style.width = `${canvas.width * scale}px`;
  canvas.style.height = `${canvas.height * scale}px`;
  publishLayout();
}
let layoutFrame=null, previousLayout='';
function publishLayout() {
  if(layoutFrame!==null) return;
  layoutFrame=requestAnimationFrame(()=>{
    layoutFrame=null;
    placeFloatingControls();
    if(!collectionPlayer) return;
    const rect=element=>{const r=element.getBoundingClientRect();return {x:r.x,y:r.y,width:r.width,height:r.height};};
    const occupied=[...document.querySelectorAll('#touch-controls button,#touch-controls .joystick,#controls-toggle,#fullscreen')]
      .filter(element=>element.getClientRects().length && !element.closest('[hidden]')).map(rect);
    const layout={type:'lcom-player-layout',width:innerWidth,height:innerHeight,game:rect(canvas),occupied,theme:mobile?.theme};
    const encoded=JSON.stringify(layout);
    if(encoded!==previousLayout) { previousLayout=encoded; parent.postMessage(layout,location.origin); }
  });
}
function placeFloatingControls() {
  if(!mobile || !touch.matches || controls.hidden || !keyboard.hidden) return;
  const rect=element=>{const r=element.getBoundingClientRect();return {x:r.x,y:r.y,width:r.width,height:r.height};};
  const visible=element=>element.getClientRects().length && !element.closest('[hidden]');
  const game=rect(canvas);
  // A narrow letterbox can hold a two-button toolbar without covering the game.
  const toolbarSide=controls.dataset.toolbar?.endsWith('left')?'left':'right';
  const gutter=toolbarSide==='left'?game.x:innerWidth-game.x-game.width;
  toolbar.style.maxWidth=gutter>=156?`${Math.min(296,gutter-12)}px`:'';
  const nodes=[...controls.querySelectorAll('.joystick-control,#dpad,.thumb-controls'),toolbar]
    .filter(element=>visible(element) && getComputedStyle(element).position==='fixed');
  // Movement controls keep their thumb positions; utility groups fit around them.
  nodes.sort((a,b)=>Number(a===toolbar || a.classList.contains('corner-actions'))-Number(b===toolbar || b.classList.contains('corner-actions')));
  for(const node of nodes) for(const name of ['left','right','top','bottom','width']) node.style[name]='';
  const obstacles=[...controls.querySelectorAll('button,.joystick'),controlsToggle,fullscreenButton]
    .filter(element=>visible(element) && !nodes.some(node=>node.contains(element))).map(rect);
  const overlap=(a,b,gap=0)=>Math.max(0,Math.min(a.x+a.width,b.x+b.width+gap)-Math.max(a.x,b.x-gap))*Math.max(0,Math.min(a.y+a.height,b.y+b.height+gap)-Math.max(a.y,b.y-gap));
  for(const node of nodes) {
    const desired=rect(node),w=desired.width,h=desired.height;
    const xs=[desired.x,8,innerWidth-w-8,game.x-w-4,game.x+game.width+4,...obstacles.flatMap(r=>[r.x-w-6,r.x+r.width+6])];
    const ys=[desired.y,8,innerHeight-h-8,game.y-h-4,game.y+game.height+4,...obstacles.flatMap(r=>[r.y-h-6,r.y+r.height+6])];
    let best=null;
    for(const x0 of xs) for(const y0 of ys) {
      const candidate={x:Math.max(8,Math.min(innerWidth-w-8,x0)),y:Math.max(8,Math.min(innerHeight-h-8,y0)),width:w,height:h};
      const score=[obstacles.reduce((sum,r)=>sum+overlap(candidate,r,4),0),overlap(candidate,game),Math.abs(candidate.x-desired.x)+Math.abs(candidate.y-desired.y)];
      if(!best || score.some((n,i)=>n<best.score[i] && score.slice(0,i).every((v,j)=>v===best.score[j]))) best={...candidate,score};
    }
    if(best) {
      Object.assign(node.style,{left:best.x+'px',top:best.y+'px',right:'auto',bottom:'auto',width:w+'px'});
      obstacles.push(best);
    }
  }
}
new ResizeObserver(resizeGame).observe(area);
new MutationObserver(publishLayout).observe(controls,{subtree:true,childList:true,attributes:true,attributeFilter:['hidden','class']});

let finished = false;
function failed(message) {
  reportLoading({phase:'error'});
  player.module = null; player.held.clear(); player.stopAudio();
  status.textContent = `Could not run the game: ${message}.`;
  document.querySelector('#error-message').textContent = status.textContent;
  document.querySelector('#error').hidden = false;
  document.querySelector('#reload').onclick = () => location.reload();
  canvas.classList.add('mouse-released');
}
const macHost = /Mac|iPhone|iPad|iPod/.test(navigator.platform);
const isMouseReleaseKey = code => code === 'ControlRight' || (macHost && code === 'AltRight');
let mouseReleased = false;
function releaseMouse() {
  player.releaseAll();
  mouseReleased = true;
  canvas.classList.add('mouse-released');
}
function resumeMouse() {
  if (mouseReleased) player.lastPointer = null;
  mouseReleased = false;
  canvas.classList.remove('mouse-released');
}

document.addEventListener('keydown', event => {
  if (finished && event.code === 'Enter') { location.reload(); return; }
  if (!player.module || document.activeElement !== canvas) return;
  if (isMouseReleaseKey(event.code)) {
    event.preventDefault(); releaseMouse(); return;
  }
  if (!guestKey(event.code)) return;
  event.preventDefault();
  player.unlockAudio();
  if (!event.repeat) player.key(event.code, true, 'physical');
});
document.addEventListener('keyup', event => {
  if (document.activeElement === canvas && isMouseReleaseKey(event.code)) {
    event.preventDefault(); return;
  }
  if (player.held.get(event.code)?.has('physical')) {
    event.preventDefault(); player.key(event.code, false, 'physical');
  }
});
canvas.addEventListener('blur', releaseMouse);
window.addEventListener('blur', () => { releaseMouse(); player.paused = true; player.stopAudio(); });
window.addEventListener('focus', () => { player.paused = document.hidden; });
document.addEventListener('visibilitychange', () => {
  if (document.hidden) releaseMouse();
  player.paused = document.hidden; player.stopAudio();
});

function sendMouse(dx = 0, dy = 0) {
  player.module?.ccall('lcom_web_mouse', null, ['number', 'number', 'number'], [dx, dy, player.buttons]);
}
function canvasPoint(event) {
  const rect = canvas.getBoundingClientRect();
  return [(event.clientX-rect.left)*canvas.width/rect.width, (event.clientY-rect.top)*canvas.height/rect.height];
}
function sendPointer(point) {
  player.module?.ccall('lcom_web_pointer', null, ['number','number','number'], [Math.round(point[0]),Math.round(point[1]),player.buttons]);
}
function setMouseButtons(source, buttons, dx = 0, dy = 0, point = null) {
  if(touchPointer && !source.startsWith('screen-') && buttons) touchPointer.blocked=true;
  if((buttons&2) && source!=='screen-latch') {
    mouseSources.delete('screen-latch');
    clearTimeout(tapTimer); tapTimer=null; mouseSources.delete('screen-tap');
  }
  if (buttons) mouseSources.set(source, buttons); else mouseSources.delete(source);
  const previous = player.buttons;
  player.buttons = [...mouseSources.values()].reduce((mask, value) => mask | value, 0);
  if (point) sendPointer(point);
  else if (previous !== player.buttons || dx || dy) sendMouse(dx, dy);
  for (const button of controls.querySelectorAll('[data-mouse]')) button.setAttribute('aria-pressed', String(Boolean(player.buttons & Number(button.dataset.mouse))));
}
function mouse(event) {
  if (!player.module || mouseReleased) return;
  const point = canvasPoint(event);
  if (mobile?.controls.pointer === 'direct') {
    setMouseButtons('physical', event.buttons, 0, 0, point);
    return;
  }
  const previous = player.lastPointer || [canvas.width / 2, canvas.height / 2];
  const dx = Math.round(point[0] - previous[0]);
  const dy = Math.round(previous[1] - point[1]);
  player.lastPointer = point;
  // Browser button bits are left/right/middle, just like a PS/2 packet.
  setMouseButtons('physical', event.buttons, dx, dy);
}
canvas.addEventListener('pointerdown', event => {
  if (event.pointerType === 'touch') return;
  if (finished) { location.reload(); return; }
  canvas.focus({ preventScroll: true }); player.unlockAudio();
  resumeMouse();
  canvas.setPointerCapture(event.pointerId);
  mouse(event);
});
canvas.addEventListener('pointermove', event => {
  if (event.pointerType !== 'touch' && (event.pointerType === 'mouse' || event.buttons)) mouse(event);
});
canvas.addEventListener('pointerup', event => { if (event.pointerType !== 'touch') mouse(event); });
canvas.addEventListener('pointercancel', event => { if (event.pointerType !== 'touch') setMouseButtons('physical', 0); });
canvas.addEventListener('lostpointercapture', event => { if (event.pointerType !== 'touch') setMouseButtons('physical', 0); });
canvas.addEventListener('contextmenu', event => event.preventDefault());
area.addEventListener('pointerdown', event => {
  if (event.target !== area && event.target !== canvas) return;
  if (finished) { location.reload(); return; }
  canvas.focus({ preventScroll: true }); player.unlockAudio();
  if (event.pointerType !== 'touch' || touchPointer) return;
  if (mobile?.controls.pointer === 'off') return;
  const point = canvasPoint(event);
  if (mobile?.controls.pointer === 'direct' && (point[0]<0 || point[1]<0 || point[0]>=canvas.width || point[1]>=canvas.height)) return;
  event.preventDefault();
  clearTimeout(tapTimer); tapTimer=null;
  setMouseButtons('screen-tap',0);
  touchPointer = { id: event.pointerId, x: event.clientX, y: event.clientY,
    originX:event.clientX, originY:event.clientY, moved:false,
    blocked:[...mouseSources].some(([source,mask])=>source!=='screen-latch' && mask), dx: 0, dy: 0 };
  area.setPointerCapture(event.pointerId);
  // An explicit right-button hold takes precedence for right-button dragging.
  const policy=touchClickEnabled ? mobile?.controls.touch || 'auto' : 'move';
  const press=policy==='press' || (policy==='auto' && mobile?.controls.pointer !== 'move');
  setMouseButtons(`screen-${event.pointerId}`, press && !(player.buttons & 2) ? 1 : 0,
    0, 0, mobile?.controls.pointer === 'direct' ? point : null);
  if(policy==='hold-drag') {
    const gesture=touchPointer;
    gesture.holdTimer=setTimeout(()=>{
      if(touchPointer!==gesture || gesture.moved || gesture.blocked || player.buttons) return;
      gesture.holding=true;
      setMouseButtons(`screen-${gesture.id}`,1);
    },180);
  }
});
area.addEventListener('pointermove', event => {
  if (event.pointerId !== touchPointer?.id) return;
  touchPointer.moved ||= Math.hypot(event.clientX-touchPointer.originX,event.clientY-touchPointer.originY)>8;
  if(touchPointer.moved && !touchPointer.holding) clearTimeout(touchPointer.holdTimer);
  if (mobile?.controls.pointer === 'direct') { sendPointer(canvasPoint(event)); return; }
  const rect = canvas.getBoundingClientRect();
  touchPointer.dx += (event.clientX - touchPointer.x) * canvas.width / rect.width;
  touchPointer.dy += (touchPointer.y - event.clientY) * canvas.height / rect.height;
  touchPointer.x = event.clientX; touchPointer.y = event.clientY;
  const dx = Math.round(touchPointer.dx), dy = Math.round(touchPointer.dy);
  touchPointer.dx -= dx; touchPointer.dy -= dy;
  if (dx || dy) sendMouse(dx, dy);
});
for (const event of ['pointerup', 'pointercancel', 'lostpointercapture']) area.addEventListener(event, event => {
  if (event.pointerId === touchPointer?.id) {
    const policy=touchClickEnabled ? mobile?.controls.touch : 'move';
    clearTimeout(touchPointer.holdTimer);
    const tap=event.type==='pointerup' && ['tap','tap-hold','hold-drag'].includes(policy) && !touchPointer.moved && !touchPointer.blocked &&
      (player.buttons===0 || (policy==='tap-hold' && mouseSources.get('screen-latch')===player.buttons)) &&
      Math.hypot(event.clientX-touchPointer.originX,event.clientY-touchPointer.originY)<=8;
    if (event.type === 'pointerup' && mobile?.controls.pointer === 'direct') sendPointer(canvasPoint(event));
    if (tap) {
      const duration=mobile?.controls.tapDuration || 0;
      const source=policy==='tap-hold'?'screen-latch':duration?'screen-tap':`screen-${event.pointerId}`;
      setMouseButtons(source,policy==='tap-hold' && mouseSources.has(source)?0:1,0,0,mobile?.controls.pointer==='direct'?canvasPoint(event):null);
      if(source==='screen-tap') tapTimer=setTimeout(()=>{tapTimer=null;setMouseButtons(source,0);},duration);
    }
    if(event.type!=='pointerup') setMouseButtons('screen-latch',0);
    touchPointer = null;
    setMouseButtons(`screen-${event.pointerId}`, 0);
  }
});

function bindMouseButton(button) {
  const mask = Number(button.dataset.mouse);
  button.addEventListener('pointerdown', event => {
    event.preventDefault(); button.setPointerCapture(event.pointerId); player.unlockAudio();
    setMouseButtons(`button-${event.pointerId}`, mask);
  });
  for (const event of ['pointerup', 'pointercancel', 'lostpointercapture']) button.addEventListener(event, event => setMouseButtons(`button-${event.pointerId}`, 0));
  button.addEventListener('click', event => {
    if (event.detail !== 0) return;
    player.unlockAudio(); setMouseButtons('activation', mask);
    setTimeout(() => setMouseButtons('activation', 0), 80);
  });
  button.addEventListener('contextmenu', event => event.preventDefault());
}
for (const button of controls.querySelectorAll('[data-mouse]')) bindMouseButton(button);

function createJoystick(pad) {
  const knob = pad.querySelector('.joystick-knob');
  const source = `joystick-${pad.id}`;
  let pointer = null, x = 0, y = 0, frame = null, lastTime = null, dx = 0, dy = 0;
  const stick = {
    mode: 'mouse', sensitivity: 1, codes:null, axes:'both', held:new Set(),
    reset() {
      pointer = null; x = y = dx = dy = 0; lastTime = null;
      cancelAnimationFrame(frame); frame = null;
      for (const code of stick.held) player.key(code, false, source);
      stick.held.clear();
      knob.style.transform = '';
    },
  };
  function moveMouse(time) {
    if (pointer === null || !player.module || player.paused) { stick.reset(); return; }
    // Deflection controls cursor velocity. Cap elapsed time to avoid resume jumps.
    const elapsed = lastTime === null ? 0 : Math.min((time - lastTime) / 1000, 1 / 30);
    lastTime = time;
    const distance = Math.hypot(x, y);
    if (distance > 0.15) {
      const speed = Math.max(canvas.width, canvas.height) * 0.75 * stick.sensitivity * (distance - 0.15) / (0.85 * distance);
      dx += x * speed * elapsed; dy -= y * speed * elapsed;
      const stepX = Math.round(dx), stepY = Math.round(dy);
      dx -= stepX; dy -= stepY;
      if (stepX || stepY) sendMouse(stepX, stepY);
    }
    frame = requestAnimationFrame(moveMouse);
  }
  function move(event) {
    const rect = pad.getBoundingClientRect();
    x = (event.clientX - rect.left - rect.width / 2) / (rect.width / 2);
    y = (event.clientY - rect.top - rect.height / 2) / (rect.height / 2);
    if(stick.axes==='horizontal') y=0;
    if(stick.axes==='vertical') x=0;
    const distance = Math.hypot(x, y);
    if (distance > 1) { x /= distance; y /= distance; }
    knob.style.transform = `translate(${x * (rect.width / 2 - 22)}px, ${y * (rect.height / 2 - 22)}px)`;
    if (stick.mode === 'mouse') return;
    const codes = stick.codes || (stick.mode === 'wasd' ? ['KeyA', 'KeyD', 'KeyW', 'KeyS'] : ['ArrowLeft', 'ArrowRight', 'ArrowUp', 'ArrowDown']);
    const next=new Set(codes.filter((_,i)=>[x < -0.3, x > 0.3, y < -0.3, y > 0.3][i]));
    for (const code of stick.held) if(!next.has(code)) player.key(code,false,source);
    for (const code of next) if(!stick.held.has(code)) player.key(code,true,source);
    stick.held=next;
  }
  pad.addEventListener('pointerdown', event => {
    if (pointer !== null) return;
    event.preventDefault(); pad.setPointerCapture(event.pointerId); player.unlockAudio();
    pointer = event.pointerId; move(event);
    if (stick.mode === 'mouse') frame = requestAnimationFrame(moveMouse);
  });
  pad.addEventListener('pointermove', event => { if (event.pointerId === pointer) move(event); });
  for (const event of ['pointerup', 'pointercancel', 'lostpointercapture']) pad.addEventListener(event, event => { if (event.pointerId === pointer) stick.reset(); });
  return stick;
}
const primaryJoystick = createJoystick(joystick);
const mouseJoystick = createJoystick(document.querySelector('#mouse-joystick'));
const extraSticks=[];
function resetJoystick() { primaryJoystick.reset(); mouseJoystick.reset(); for(const stick of extraSticks) stick.reset(); }
function updateJoystickMode() {
  player.releaseAll();
  const dual = joystickMapping.value.startsWith('mouse-');
  primaryJoystick.mode = dual ? joystickMapping.value.slice(6) : joystickMapping.value;
  document.querySelector('#mouse-joystick-control').hidden = !dual;
  controls.classList.toggle('dual-joysticks', dual);
  const label = primaryJoystick.mode === 'mouse' ? 'Mouse' : primaryJoystick.mode === 'wasd' ? 'WASD' : 'Arrow keys';
  document.querySelector('#joystick-label').textContent = label;
  joystick.setAttribute('aria-label', primaryJoystick.mode === 'mouse' ? 'Mouse joystick' : 'Movement joystick');
  canvas.focus({ preventScroll: true });
}
joystickMapping.addEventListener('change', updateJoystickMode);
function togglePanel(button, panel) {
  player.releaseAll();
  panel.hidden = !panel.hidden;
  button.setAttribute('aria-expanded', String(!panel.hidden));
  if (!panel.hidden) {
    const other = button === keyboardToggle ? joystickToggle : keyboardToggle;
    document.getElementById(other.getAttribute('aria-controls')).hidden = true;
    other.setAttribute('aria-expanded', 'false');
  }
  document.querySelector('.compact-controls').hidden = !keyboard.hidden;
  controls.classList.toggle('keyboard-open', !keyboard.hidden);
  updateKeyboardDock();
  controls.classList.toggle('dual-joysticks', !document.querySelector('#joystick-controls').hidden && joystickMapping.value.startsWith('mouse-'));
  if(button===keyboardToggle && panel.hidden) applyControlSettings();
  canvas.focus({ preventScroll: true });
}
keyboardToggle.onclick = () => togglePanel(keyboardToggle, keyboard);
joystickToggle.onclick = () => togglePanel(joystickToggle, document.querySelector('#joystick-controls'));

const rows = [
  [['Esc', 'Escape'], ...'1234567890'.split('').map(n => [n, `Digit${n}`])],
  [...'QWERTYUIOP'].map(n => [n, `Key${n}`]),
  [...'ASDFGHJKL'].map(n => [n, `Key${n}`]).concat([['⌫', 'Backspace', 'wide']]),
  [['Shift', 'ShiftLeft', 'wide'], ...[...'ZXCVBNM'].map(n => [n, `Key${n}`]), ['Enter', 'Enter', 'wide']],
  [['Ctrl', 'ControlLeft', 'wide'], ['Alt', 'AltLeft', 'wide'], ['Space', 'Space', 'space'], ['←', 'ArrowLeft'], ['↑', 'ArrowUp'], ['↓', 'ArrowDown'], ['→', 'ArrowRight']],
];
const functionRows = [
  Array.from({ length: 6 }, (_, i) => [`F${i + 1}`, `F${i + 1}`]),
  Array.from({ length: 6 }, (_, i) => [`F${i + 7}`, `F${i + 7}`]),
  [['Tab', 'Tab'], ['Home', 'Home'], ['End', 'End'], ['PgUp', 'PageUp'], ['PgDn', 'PageDown'], ['Del', 'Delete']],
  [['-', 'Minus'], ['=', 'Equal'], ['[', 'BracketLeft'], [']', 'BracketRight'], [';', 'Semicolon'], ["'", 'Quote'], [',', 'Comma'], ['.', 'Period'], ['/', 'Slash'], ['\\', 'Backslash'], ['`', 'Backquote']],
  rows[4],
];
function keyButton([label, code, size = ''], latch = true) {
  const button = document.createElement('button');
  button.type = 'button'; button.textContent = label; button.dataset.code = code; button.className = `key ${size}`;
  button.setAttribute('aria-label', code === 'NumpadEnter' ? 'Keypad Enter' : aliases[code] || label);
  const modifier = latch && ['ShiftLeft', 'ControlLeft', 'AltLeft'].includes(code);
  if (modifier) button.setAttribute('aria-pressed', 'false');
  button.addEventListener('pointerdown', event => {
    event.preventDefault(); button.setPointerCapture(event.pointerId); player.unlockAudio();
    if (modifier) {
      const down = button.getAttribute('aria-pressed') !== 'true';
      button.setAttribute('aria-pressed', String(down)); player.key(code, down, 'latch');
    } else player.key(code, true, `touch-${event.pointerId}`);
  });
  const release = event => { if (!modifier) player.key(code, false, `touch-${event.pointerId}`); };
  for (const event of ['pointerup', 'pointercancel', 'lostpointercapture']) button.addEventListener(event, release);
  // Buttons also work with keyboard activation and assistive technology.
  button.addEventListener('click', event => {
    if (event.detail !== 0) return;
    if (modifier) {
      const down = button.getAttribute('aria-pressed') !== 'true';
      button.setAttribute('aria-pressed', String(down)); player.key(code, down, 'latch');
    } else { player.key(code, true, 'activation'); setTimeout(() => player.key(code, false, 'activation'), 80); }
  });
  return button;
}
function renderKeyboard() {
  player.releaseAll();
  const container = document.querySelector('#keys');
  container.replaceChildren();
  for (const keys of functionToggle.getAttribute('aria-pressed') === 'true' ? functionRows : rows) {
    const row = document.createElement('div'); row.className = 'key-row';
    for (const [label, code, size = ''] of keys) {
      row.append(keyButton([label, code, size]));
    }
    if (container.children.length === 4) row.prepend(functionToggle);
    container.append(row);
  }
}
function updateKeyboard() {
  player.releaseAll();
  updateTouchControls();
  keyboard.hidden = true;
  document.querySelector('.compact-controls').hidden = false;
  document.querySelector('#joystick-controls').hidden = true;
  controls.classList.remove('dual-joysticks');
  keyboardToggle.setAttribute('aria-expanded', 'false');
  joystickToggle.setAttribute('aria-expanded', 'false');
  document.querySelector('#quick-keys').replaceChildren();
  if (touch.matches) {
    if (mobile?.controls.keyboard !== false) renderKeyboard();
    document.querySelector('#quick-keys').append(...(mobile?.controls.quickKeys || ['ESC','SPACE','ENTER']).map(key => keyButton([keyLabel(key), browserKey(key)])));
    applyControlSettings();
  } else document.querySelector('#keys').replaceChildren();
}
functionToggle.onclick = () => { functionToggle.setAttribute('aria-pressed', String(functionToggle.getAttribute('aria-pressed') !== 'true')); renderKeyboard(); };
touch.addEventListener('change', updateKeyboard);
function updateTouchControls() {
  const c = mobile?.controls;
  const available = !c || c.keyboard || c.mouseButtons || c.joystick || c.look || c.dpad || c.quickKeys.length || c.pads?.length || c.actions?.length;
  controls.hidden = !touch.matches || !available || controlsCollapsed;
  controlsToggle.hidden = !touch.matches || !available;
  controlsToggle.setAttribute('aria-expanded', String(!controls.hidden));
  controlsToggle.setAttribute('aria-label', controls.hidden ? 'Controls' : 'Hide controls');
  controlsToggle.title = controlsToggle.getAttribute('aria-label');
  icon(controlsToggle, controls.hidden ? 'controls' : 'hide');
  fullscreenButton.hidden = !touch.matches || collectionPlayer;
  updateOrientationHint();
  updateKeyboardDock();
  publishLayout();
}
controlsToggle.onclick = () => {
  player.releaseAll();
  controlsCollapsed = !controlsCollapsed;
  updateTouchControls();
  canvas.focus({ preventScroll: true });
};
landscape.addEventListener('change', () => { player.releaseAll(); updateTouchControls(); });

const iconPaths = {
  keyboard: '<rect x="2" y="5" width="20" height="14" rx="2"/><path d="M5 9h1m3 0h1m3 0h1m3 0h1M5 12h1m3 0h1m3 0h1m3 0h1M7 16h10"/>',
  'mouse-left': '<rect x="6" y="2" width="12" height="20" rx="6"/><path d="M12 2v8M6 10h12"/><circle cx="9" cy="7" r="1.6" fill="currentColor" stroke="none"/>',
  'mouse-right': '<rect x="6" y="2" width="12" height="20" rx="6"/><path d="M12 2v8M6 10h12"/><circle cx="15" cy="7" r="1.6" fill="currentColor" stroke="none"/>',
  stick: '<circle cx="12" cy="6" r="4"/><path d="M12 10v7M8 14H5l-2 7h18l-2-7h-3"/>',
  hide: '<path d="m6 9 6 6 6-6"/>', controls: '<path d="m6 15 6-6 6 6"/>',
  fullscreen: '<path d="M8 3H3v5m13-5h5v5M3 16v5h5m13-5v5h-5"/>',
  move: '<path d="M5 3v16l4-4 4 7 3-2-4-6h6L5 3Z"/>',
  drag: '<path d="M5 3v16l4-4 4 7 3-2-4-6h6L5 3Z M18 3v5m-2.5-2.5h5"/>',
  tap: '<rect x="6" y="2" width="12" height="20" rx="6"/><path d="M12 2v8M6 10h12"/><circle cx="9" cy="7" r="1.6" fill="currentColor" stroke="none"/>',
};
function icon(button, name) {
  const caption = ['mouse-left', 'mouse-right', 'stick', 'keyboard'].includes(name) ? button.getAttribute('aria-label') : '';
  button.classList.toggle('labelled-icon', Boolean(caption));
  button.innerHTML = `<svg viewBox="0 0 24 24" aria-hidden="true" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">${iconPaths[name]}</svg>${caption ? `<span aria-hidden="true">${caption}</span>` : ''}`;
}
for (const button of document.querySelectorAll('[data-icon]')) icon(button, button.dataset.icon);
icon(fullscreenButton, 'fullscreen');
function browserKey(key) {
  return Object.keys(aliases).find(code => aliases[code] === key) || (/^[A-Z]$/.test(key) ? 'Key'+key : /^[0-9]$/.test(key) ? 'Digit'+key : key);
}
function keyLabel(key) { return ({LEFT:'←',RIGHT:'→',UP:'↑',DOWN:'↓',SPACE:'␣',ENTER:'↵',KP_ENTER:'Num Enter',BACKSPACE:'⌫',ESC:'Esc'})[key] || key; }
function directionLabel(key) { return key.startsWith('KP') ? 'Num '+key.slice(2) : keyLabel(key); }
function activeDirections(pad) {
  return pad.axes==='horizontal'?['left','right']:pad.axes==='vertical'?['up','down']:['left','up','right','down'];
}
function padKeys(pad) {
  return pad.keys || (pad.mode==='wasd'?{left:'A',up:'W',right:'D',down:'S'}:
    pad.mode==='keypad'?{left:'KP4',up:'KP8',right:'KP6',down:'KP2'}:{left:'LEFT',up:'UP',right:'RIGHT',down:'DOWN'});
}
const customControls=document.createElement('div');
customControls.id='custom-controls'; controls.append(customControls);
const toolbar=document.createElement('div');toolbar.id='control-toolbar';
toolbar.append(controls.querySelector('.control-bar'),controls.querySelector('.quick-controls'));controls.prepend(toolbar);
const pointerToggle=document.createElement('button');pointerToggle.type='button';pointerToggle.id='pointer-toggle';
pointerToggle.setAttribute('aria-pressed','true');controls.querySelector('.control-bar').append(pointerToggle);
pointerToggle.onclick=()=>{player.releaseAll();touchClickEnabled=!touchClickEnabled;updatePointerToggle();canvas.focus({preventScroll:true});};
function updatePointerToggle() {
  const policy=mobile?.controls.touch;
  const label=!touchClickEnabled?'Move only':policy==='hold-drag'?'Hold to drag':policy==='tap'?'Tap on':'Drag on';
  pointerToggle.setAttribute('aria-label',label);
  icon(pointerToggle,!touchClickEnabled?'move':policy==='tap'?'tap':'drag');
  pointerToggle.setAttribute('aria-pressed',String(touchClickEnabled));
  pointerToggle.title=touchClickEnabled?'Switch to moving without clicking':'Enable touch clicks and dragging';
  const c=mobile?.controls;
  pointerToggle.hidden=c?.pointer==='off' || c?.touch==='move' || (c?.touch==='auto' && c?.pointer==='move');
}
function updateKeyboardDock() {
  const position=mobile?.controls.keyboardPosition || 'auto';
  const dock=position==='auto'?(landscape.matches?'right':'bottom'):position;
  controls.dataset.keyboardPosition=dock;
  document.body.dataset.keyboardDock=!keyboard.hidden && touch.matches?dock:'';
  resizeGame();
}
const toolbarSize=new ResizeObserver(()=>{
  controls.style.setProperty('--toolbar-height',toolbar.getBoundingClientRect().height+'px');
  publishLayout();
});toolbarSize.observe(toolbar);
function renderCustomControls(c) {
  for(const stick of extraSticks) stick.reset();
  extraSticks.length=0; customControls.replaceChildren();
  const toolbarPosition=c.toolbar==='auto'||!c.toolbar?(c.layout==='overlay'?'top-right':'panel'):c.toolbar;
  const opposite=toolbarPosition.endsWith('left')?toolbarPosition.replace('left','right'):toolbarPosition.replace('right','left');
  const oppositeActions=(c.actions || []).some(a=>(a.position || 'bottom-'+a.side)===opposite);
  const oppositePad=opposite.startsWith('bottom') && [...(c.pads || []),c.dpad,c.look,c.joystick].some(p=>p?.side===opposite.split('-')[1]);
  controls.style.setProperty('--toolbar-reserve',oppositeActions || oppositePad?'180px':'80px');
  for(const position of ['top-left','top-right','bottom-left','bottom-right']) {
    const side=position.endsWith('left')?'left':'right';
    const group=document.createElement('div'); group.className='thumb-controls';group.dataset.position=position;group.dataset.side=side;
    group.classList.toggle('same-toolbar',position===toolbarPosition);
    if(position.startsWith('top') || (c.actions || []).some(a=>a.position===position)) group.classList.add('corner-actions');
    const actions=document.createElement('div'); actions.className='game-actions';
    for(const action of (c.actions || []).filter(a=>(a.position || 'bottom-'+a.side)===position)) {
      let button;
      if(action.key) button=keyButton([action.label,browserKey(action.key)],false);
      else {
        button=document.createElement('button'); button.type='button'; button.className='key'; button.textContent=action.label;
        button.dataset.mouse=({left:1,right:2,middle:4})[action.mouseButton];
        button.setAttribute('aria-label',action.label); button.setAttribute('aria-pressed','false'); bindMouseButton(button);
      }
      button.setAttribute('aria-label',action.label); actions.append(button);
    }
    group.append(actions);
    for(const [index,pad] of (c.pads || []).entries()) if(pad.side===side && position.startsWith('bottom')) {
      if(pad.style==='stick') {
        const element=document.createElement('div'); element.id='custom-pad-'+index; element.className='joystick';
        element.setAttribute('aria-label',pad.mode==='mouse'?'Mouse joystick':side+' movement joystick');
        const knob=document.createElement('span'); knob.className='joystick-knob'; element.append(knob); group.append(element);
        const stick=createJoystick(element); stick.mode=pad.mode; stick.sensitivity=pad.sensitivity;stick.axes=pad.axes || 'both';
        const keys=padKeys(pad); stick.codes=['left','right','up','down'].map(k=>browserKey(keys[k])); extraSticks.push(stick);
      } else {
        const element=document.createElement('div'); element.className='custom-dpad';element.dataset.axes=pad.axes || 'both';element.setAttribute('aria-label',side+' direction pad');
        for(const direction of activeDirections(pad)) {
          const key=padKeys(pad)[direction];
          const button=keyButton([directionLabel(key),browserKey(key)],false);
          button.dataset.direction=direction; element.append(button);
        }
        group.append(element);
      }
    }
    // Legacy pads can share the thumb area with actions while staying API compatible.
    const legacy=position.startsWith('bottom') && [c.joystick,c.look,c.dpad].some(p=>typeof p==='object' && p?.side===side);
    group.classList.toggle('above-legacy-pad',legacy);
    group.style.setProperty('--legacy-pad-height',c.dpad?.side===side?'156px':'132px');
    if(actions.children.length || group.children.length>1) customControls.append(group);
  }
}
function applyControlSettings() {
  if (!mobile) return;
  const c=mobile.controls;
  controls.dataset.toolbar=c.toolbar==='auto'||!c.toolbar?(c.layout==='overlay'?'top-right':'panel'):c.toolbar;
  updatePointerToggle();
  keyboardToggle.hidden = !c.keyboard;
  joystickToggle.hidden = c.joystick !== true;
  for (const b of controls.querySelectorAll('.control-bar [data-mouse]')) b.hidden = !c.mouseButtons;
  document.querySelector('#touch-hint').hidden = Boolean(mobile) || c.pointer === 'off';
  const configured = typeof c.joystick === 'object';
  const dual = configured && Boolean(c.look);
  document.querySelector('#joystick-controls').hidden = !configured && !c.look;
  document.querySelector('#joystick-mapping').hidden = configured || Boolean(c.look) || !c.joystick;
  joystick.parentElement.hidden = !configured && Boolean(c.look);
  if (configured) {
    primaryJoystick.mode = c.joystick.mode;
    primaryJoystick.axes = c.joystick.axes || 'both';
    joystick.parentElement.dataset.side=c.joystick.side;
    joystick.setAttribute('aria-label', c.joystick.mode === 'mouse' ? 'Mouse joystick' : 'Movement joystick');
    document.querySelector('#joystick-label').textContent=c.joystick.mode === 'wasd' ? 'WASD' : c.joystick.mode === 'arrows' ? '← ↑ ↓ →' : 'Mouse';
  }
  const look=document.querySelector('#mouse-joystick-control');
  look.hidden = !c.look;
  if (c.look) { look.dataset.side=c.look.side; mouseJoystick.sensitivity=c.look.sensitivity; }
  controls.classList.toggle('dual-joysticks', dual);
  controls.classList.toggle('configured-overlay', c.layout === 'overlay');
  const dpad=document.querySelector('#dpad');
  dpad.hidden = !c.dpad;
  dpad.replaceChildren();
  if (c.dpad) {
    dpad.dataset.side=c.dpad.side;
    dpad.dataset.axes=c.dpad.axes || 'both';
    const keys=c.dpad.keys==='wasd'?{up:'W',left:'A',down:'S',right:'D'}:{up:'UP',left:'LEFT',down:'DOWN',right:'RIGHT'};
    for (const direction of activeDirections(c.dpad)) {
      const key=keys[direction];
      const b=keyButton([directionLabel(key),browserKey(key)],false);
      b.dataset.direction=direction; dpad.append(b);
    }
  }
  renderCustomControls(c);
  updateKeyboardDock();
  updateTouchControls();
}
function updateOrientationHint() {
  const hint=document.querySelector('#rotation-hint');
  const orientation=mobile?.orientation || 'auto';
  hint.hidden = !touch.matches || orientation === 'auto' || (orientation === 'landscape') === landscape.matches;
  hint.textContent = `Rotate your device to ${orientation}.`;
}
async function enterFullscreen() {
  if (collectionPlayer) return;
  try {
    if (!document.fullscreenElement) await document.documentElement.requestFullscreen?.();
    if (mobile?.orientation !== 'auto') await screen.orientation?.lock?.(mobile.orientation);
  } catch { /* Browsers may require a user gesture or may not support locking. */ }
}
fullscreenButton.onclick = async () => {
  if (document.fullscreenElement) { await document.exitFullscreen(); return; }
  await enterFullscreen();
};
document.addEventListener('pointerdown', () => { if (touch.matches && mobile?.fullscreen) enterFullscreen(); }, { once: true });

try {
  const response = await fetch('./game.json', {cache:'no-cache'});
  if (!response.ok) throw new Error('game settings could not be loaded');
  const config = await response.json();
  document.title = `${config.name} · lcom-ng`;
  mobile = config.mobile || null;
  if (mobile) {
    for (const [key,value] of Object.entries(mobile.theme)) {
      if (key !== 'font') document.documentElement.style.setProperty('--'+key.replace(/[A-Z]/g,m=>'-'+m.toLowerCase()),value);
    }
    document.documentElement.style.setProperty('--control-font', mobile.theme.font === 'mono' ? 'ui-monospace, monospace' : 'system-ui, sans-serif');
    document.body.dataset.orientation = mobile.orientation;
    for (const [name,color] of [['accent',mobile.theme.accent],['keyboard-accent',mobile.theme.keyboardAccent]]) {
      const rgb=Number.parseInt(color.slice(1),16);
      const light=((rgb>>16)*299+((rgb>>8)&255)*587+(rgb&255)*114)/1000;
      document.documentElement.style.setProperty('--'+name+'-text',light>140?'#101010':'#ffffff');
    }
  }
  updateKeyboard();
  const [{default:createLcomGame}, prepared] = await Promise.all([import('./game.js'), prepareGame(config.downloads, reportLoading)]);
  const module = await instantiateGame(createLcomGame, { player, preRun: [module => Object.assign(module.ENV,config.environment || {})], print: text => console.log(text), printErr: text => console.warn(text),
    onAbort: reason => failed(String(reason)),
    onExit: code => {
      if (!firstFrame) reportLoading({phase:'error'});
      player.releaseAll(); player.stopAudio(); player.module = null; finished = true;
      canvas.classList.add('mouse-released');
      status.textContent = code ? `Game exited with status ${code}.` : 'Game finished. Press Enter or tap the game to restart.';
    },
  }, prepared);
  module.FS.mkdirTree(config.workingDirectory);
  module.FS.chdir(config.workingDirectory);
  canvas.focus({ preventScroll: true }); player.paused = document.hidden;
  status.textContent = 'Game running.';
  try { module.callMain(config.arguments); } catch (error) { failed(error.message || String(error)); }
} catch (error) { failed(error.message || String(error)); }
