import createLcomGame from './game.js';

const canvas = document.querySelector('#game');
const status = document.querySelector('#status');
const area = document.querySelector('main');
const touch = matchMedia('(pointer: coarse)');
const keyboard = document.querySelector('#keyboard');
const functionToggle = document.createElement('button');
functionToggle.id = 'function-toggle';
functionToggle.type = 'button';
functionToggle.className = 'key';
functionToggle.textContent = 'Fn';
functionToggle.setAttribute('aria-label', 'Function keys');
functionToggle.setAttribute('aria-pressed', 'false');

const aliases = {
  Escape: 'ESC', Space: 'SPACE', Enter: 'ENTER', Backspace: 'BACKSPACE', Tab: 'TAB',
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
  module: null, paused: false, held: new Map(), buttons: 0, image: null,
  audioContext: null, audioSource: null, lastPointer: null,
  attach(module) { this.module = module; this.releaseAll(); },
  key(code, down, source) {
    const key = guestKey(code);
    if (!this.module || !key) return;
    const sources = this.held.get(code) || new Set();
    const wasHeld = sources.size > 0;
    if (down) sources.add(source); else sources.delete(source);
    if (sources.size) this.held.set(code, sources); else this.held.delete(code);
    if (wasHeld !== (sources.size > 0)) this.module.ccall('lcom_web_key', null, ['string', 'number'], [key, down ? 1 : 0]);
    for (const button of document.querySelectorAll(`[data-code="${code}"]`)) button.classList.toggle('held', sources.size > 0);
  },
  releaseAll() {
    for (const [code, sources] of [...this.held]) for (const source of [...sources]) this.key(code, false, source);
    for (const button of document.querySelectorAll('.key[data-code][aria-pressed]')) button.setAttribute('aria-pressed', 'false');
    if (this.buttons) this.module?.ccall('lcom_web_mouse', null, ['number', 'number', 'number'], [0, 0, 0]);
    this.buttons = 0;
    this.lastPointer = null;
  },
  present(bytes, width, height) {
    if (!this.image || canvas.width !== width || canvas.height !== height) {
      canvas.width = width; canvas.height = height;
      resizeGame();
      this.image = canvas.getContext('2d').createImageData(width, height);
    }
    this.image.data.set(bytes);
    canvas.getContext('2d').putImageData(this.image, 0, 0);
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
}
new ResizeObserver(resizeGame).observe(area);

let finished = false;
function failed(message) {
  player.module = null; player.held.clear(); player.stopAudio();
  status.textContent = `Could not run the game: ${message}.`;
  document.querySelector('#error-message').textContent = status.textContent;
  document.querySelector('#error').hidden = false;
  document.querySelector('#reload').onclick = () => location.reload();
  document.exitPointerLock?.();
}
function captureMouse() {
  if (touch.matches || document.pointerLockElement === canvas) return;
  const request = canvas.requestPointerLock?.();
  request?.catch(error => console.warn(`Mouse capture unavailable: ${error.message}`));
}
let mouseCaptured = false;
let releaseOnly = false;
let escapeForwarded = false;
document.addEventListener('pointerlockchange', () => {
  const captured = document.pointerLockElement === canvas;
  // Native browsers can consume both Escape events while releasing pointer lock.
  // Forward that gesture once, unless we released capture or already sent the key.
  const escape = mouseCaptured && !captured && !releaseOnly && !escapeForwarded &&
    document.hasFocus() && !document.hidden && document.activeElement === canvas;
  player.releaseAll();
  mouseCaptured = captured; releaseOnly = false; escapeForwarded = false;
  if (escape) {
    player.key('Escape', true, 'browser');
    player.key('Escape', false, 'browser');
  }
});

document.addEventListener('keydown', event => {
  if (finished && event.code === 'Enter') { location.reload(); return; }
  if (event.code === 'ControlRight' && document.pointerLockElement === canvas) {
    event.preventDefault(); releaseOnly = true; document.exitPointerLock(); return;
  }
  if (!player.module || document.activeElement !== canvas || !guestKey(event.code)) return;
  event.preventDefault();
  player.unlockAudio();
  if (!event.repeat) player.key(event.code, true, 'physical');
  if (event.code === 'Escape' && document.pointerLockElement === canvas) {
    escapeForwarded = true; document.exitPointerLock();
  }
});
document.addEventListener('keyup', event => {
  if (player.held.get(event.code)?.has('physical')) {
    event.preventDefault(); player.key(event.code, false, 'physical');
  }
});
canvas.addEventListener('blur', () => player.releaseAll());
window.addEventListener('blur', () => { if (mouseCaptured) releaseOnly = true; player.releaseAll(); player.paused = true; player.stopAudio(); });
window.addEventListener('focus', () => { player.paused = document.hidden; });
document.addEventListener('visibilitychange', () => { if (mouseCaptured && document.hidden) releaseOnly = true; player.releaseAll(); player.paused = document.hidden; player.stopAudio(); });

function mouse(event) {
  if (!player.module) return;
  const rect = canvas.getBoundingClientRect();
  const point = [(event.clientX - rect.left) * canvas.width / rect.width, (event.clientY - rect.top) * canvas.height / rect.height];
  const previous = player.lastPointer || [canvas.width / 2, canvas.height / 2];
  const locked = document.pointerLockElement === canvas;
  const dx = Math.round(locked ? event.movementX * canvas.width / rect.width : point[0] - previous[0]);
  const dy = Math.round(locked ? -event.movementY * canvas.height / rect.height : previous[1] - point[1]);
  player.lastPointer = point;
  // Browser button bits are left/right/middle, just like a PS/2 packet.
  player.buttons = event.buttons;
  player.module.ccall('lcom_web_mouse', null, ['number', 'number', 'number'], [dx, dy, player.buttons]);
}
canvas.addEventListener('pointerdown', event => {
  if (finished) { location.reload(); return; }
  canvas.focus({ preventScroll: true }); player.unlockAudio();
  if (!document.pointerLockElement) canvas.setPointerCapture(event.pointerId);
  captureMouse(); mouse(event);
});
canvas.addEventListener('pointermove', event => {
  if (!document.pointerLockElement && (event.pointerType === 'mouse' || event.buttons)) mouse(event);
});
canvas.addEventListener('mousemove', event => { if (document.pointerLockElement === canvas) mouse(event); });
canvas.addEventListener('pointerup', mouse);
canvas.addEventListener('pointercancel', () => player.releaseAll());
canvas.addEventListener('lostpointercapture', () => { if (player.buttons && !document.pointerLockElement) player.releaseAll(); });
canvas.addEventListener('contextmenu', event => event.preventDefault());
area.addEventListener('pointerdown', event => { if (event.target === area) canvas.focus({ preventScroll: true }); });

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
function renderKeyboard() {
  player.releaseAll();
  const container = document.querySelector('#keys');
  container.replaceChildren();
  for (const keys of functionToggle.getAttribute('aria-pressed') === 'true' ? functionRows : rows) {
    const row = document.createElement('div'); row.className = 'key-row';
    for (const [label, code, size = ''] of keys) {
      const button = document.createElement('button');
      button.type = 'button'; button.textContent = label; button.dataset.code = code; button.className = `key ${size}`;
      button.setAttribute('aria-label', aliases[code] || label);
      const modifier = ['ShiftLeft', 'ControlLeft', 'AltLeft'].includes(code);
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
      row.append(button);
    }
    if (container.children.length === 4) row.prepend(functionToggle);
    container.append(row);
  }
}
function updateKeyboard() {
  player.releaseAll();
  keyboard.hidden = !touch.matches;
  if (touch.matches) renderKeyboard(); else document.querySelector('#keys').replaceChildren();
}
functionToggle.onclick = () => { functionToggle.setAttribute('aria-pressed', String(functionToggle.getAttribute('aria-pressed') !== 'true')); renderKeyboard(); };
updateKeyboard();
touch.addEventListener('change', updateKeyboard);

try {
  const response = await fetch('./game.json');
  if (!response.ok) throw new Error('game settings could not be loaded');
  const config = await response.json();
  document.title = `${config.name} · lcom-ng`;
  const module = await createLcomGame({ player, print: text => console.log(text), printErr: text => console.warn(text),
    onAbort: reason => failed(String(reason)),
    onExit: code => {
      player.releaseAll(); player.stopAudio(); player.module = null; finished = true;
      document.exitPointerLock?.();
      status.textContent = code ? `Game exited with status ${code}.` : 'Game finished. Press Enter or tap the game to restart.';
    },
  });
  module.FS.mkdirTree(config.workingDirectory);
  module.FS.chdir(config.workingDirectory);
  canvas.focus({ preventScroll: true }); player.paused = document.hidden;
  status.textContent = 'Game running.';
  try { module.callMain(config.arguments); } catch (error) { failed(error.message || String(error)); }
} catch (error) { failed(error.message || String(error)); }
