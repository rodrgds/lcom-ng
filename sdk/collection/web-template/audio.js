import {play, setEnabled, setVolume, unlock} from './cuelume.js';

const supported = !!(window.AudioContext || window.webkitAudioContext);
setEnabled(true);
setVolume(0.35);

// Cuelume creates audio only after an interaction. The browser tab owns mute.
export function cue(name = 'tap', options) {
  if (supported) play(name, options);
}

export async function unlockSound() {
  return supported ? unlock() : false;
}
