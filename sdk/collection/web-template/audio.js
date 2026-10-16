import {play, setEnabled, setVolume, unlock} from './cuelume.js';

const preferenceKey = 'lcom-arcade-sound';
const buttons = new Set();
const supported = typeof window !== 'undefined' && !!(window.AudioContext || window.webkitAudioContext);
let enabled = readPreference() && supported;
let change = 0;
let unavailable = !supported;

// Apply the preference before any interaction can request a cue.
setEnabled(enabled);
setVolume(0.35);

function readPreference() {
  try { return globalThis.localStorage?.getItem(preferenceKey) === 'true'; }
  catch { return false; }
}

function render() {
  for (const button of buttons) {
    button.setAttribute('aria-pressed', String(enabled));
    button.setAttribute('aria-label', unavailable ? 'UI sounds unavailable' : enabled ? 'Mute UI sounds' : 'Enable UI sounds');
    button.title = unavailable ? 'UI sounds could not start in this browser' : enabled ? 'Mute UI sounds' : 'Enable UI sounds';
    button.disabled = !supported;
    for (const icon of button.querySelectorAll('[data-sound-on]')) icon.toggleAttribute('hidden', !enabled);
    for (const icon of button.querySelectorAll('[data-sound-off]')) icon.toggleAttribute('hidden', enabled);
    const label = button.querySelector('span');
    if (label) label.textContent = enabled ? 'Sound on' : 'Sound off';
  }
}

function apply(value, persist) {
  enabled = value && supported;
  change++;
  // Cuelume also fades voices that are already ringing when muted.
  setEnabled(enabled);
  if (persist) try { globalThis.localStorage?.setItem(preferenceKey, String(enabled)); } catch { /* Keep the choice for this visit. */ }
  render();
  return enabled;
}

export function setSoundEnabled(value) {
  if (typeof value !== 'boolean') return enabled;
  unavailable = !supported;
  return apply(value, true);
}

export async function unlockSound() {
  if (!enabled) return false;
  return unlock();
}

export function cue(name = 'tap', options) {
  if (enabled) play(name, options);
}

async function toggle() {
  if (!setSoundEnabled(!enabled)) return;
  const requested = change;
  const ready = await unlockSound();
  if (requested !== change) return;
  if (!ready) {
    unavailable = true;
    apply(false, true);
  } else cue('toggle', {emphasis:'subtle'});
}

export function initSound(button) {
  setEnabled(enabled);
  if (button && !buttons.has(button)) {
    buttons.add(button);
    button.addEventListener('click', toggle);
  }
  render();
  return enabled;
}

if (typeof window !== 'undefined') window.addEventListener('storage', event => {
  if (event.key === preferenceKey || event.key === null) apply(readPreference(), false);
});
