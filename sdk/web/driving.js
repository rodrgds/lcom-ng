// Driving widgets emit ordinary held guest keys through the player's input owner.
export function createSteeringWheel({left, right, maxAngle = 135, deadZone = 8}, key, unlockAudio) {
  const element = document.createElement('div');
  element.className = 'steering-wheel'; element.tabIndex = 0;
  element.setAttribute('role', 'slider'); element.setAttribute('aria-label', 'Steering wheel');
  element.setAttribute('aria-orientation', 'horizontal');
  element.setAttribute('aria-valuemin', -maxAngle); element.setAttribute('aria-valuemax', maxAngle);
  element.innerHTML = `<svg class="wheel-rotor" viewBox="0 0 160 160" aria-hidden="true">
    <circle cx="80" cy="80" r="67" fill="#11171b" stroke="#070b0d" stroke-width="12"/>
    <circle cx="80" cy="80" r="68" fill="none" stroke="#77818a" stroke-width="2"/>
    <circle cx="80" cy="80" r="55" fill="none" stroke="#3f484f" stroke-width="3"/>
    <path d="M64 76 29 51 22 65 60 92ZM96 76 131 51 138 65 100 92ZM71 98 68 134 92 134 89 98Z" fill="#9ba5ad" stroke="#444e56" stroke-width="2"/>
    <circle cx="80" cy="80" r="24" fill="#282f35" stroke="#525e68" stroke-width="2"/>
    <circle cx="80" cy="80" r="16" fill="#151d22"/>
    <rect class="wheel-marker" x="76" y="10" width="8" height="16" rx="3"/>
  </svg>`;
  const rotor = element.firstElementChild;
  let pointer = null, previous = null, angle = 0, held = null;
  const keyboard = new Set();
  function setAngle(value) {
    angle = Math.max(-maxAngle, Math.min(maxAngle, value));
    const next = angle < -deadZone ? left : angle > deadZone ? right : null;
    if (held !== next) { if (held) key(held, false); if (next) key(next, true); held = next; }
    rotor.style.transform = `rotate(${angle}deg)`;
    const degrees = Math.round(angle);
    element.setAttribute('aria-valuenow', degrees);
    element.setAttribute('aria-valuetext', degrees ? `${Math.abs(degrees)} degrees ${degrees < 0 ? 'left' : 'right'}` : 'Centred');
  }
  function bearing(event) {
    const rect = element.getBoundingClientRect();
    const x = event.clientX - rect.left - rect.width/2, y = event.clientY - rect.top - rect.height/2;
    return {degrees: Math.atan2(y, x)*180/Math.PI, distance: Math.hypot(x, y), radius: rect.width/2};
  }
  function reset() {
    const captured = pointer; pointer = previous = null; keyboard.clear();
    element.classList.remove('steering-held'); setAngle(0);
    if (captured !== null && element.hasPointerCapture(captured)) element.releasePointerCapture(captured);
  }
  element.addEventListener('pointerdown', event => {
    if (pointer !== null) return;
    const point = bearing(event); if (point.distance > point.radius) return;
    event.preventDefault(); unlockAudio(); pointer = event.pointerId;
    previous = point.distance >= point.radius*.15 ? point.degrees : null;
    keyboard.clear(); setAngle(0); element.classList.add('steering-held');
    element.setPointerCapture(pointer);
  });
  element.addEventListener('pointermove', event => {
    if (event.pointerId !== pointer) return;
    const point = bearing(event); if (point.distance < point.radius*.15) { previous = null; return; }
    if (previous !== null) setAngle(angle + ((point.degrees - previous + 540)%360 - 180));
    previous = point.degrees;
  });
  for (const name of ['pointerup', 'pointercancel', 'lostpointercapture'])
    element.addEventListener(name, event => { if (event.pointerId === pointer) reset(); });
  function keyboardAngle() { setAngle(keyboard.size === 1 ? (keyboard.has('ArrowLeft') ? -1 : 1)*maxAngle : 0); }
  for (const name of ['keydown', 'keyup']) element.addEventListener(name, event => {
    if (!['ArrowLeft', 'ArrowRight', 'Home'].includes(event.code)) return;
    event.preventDefault(); event.stopPropagation();
    if (pointer !== null) return;
    unlockAudio();
    if (event.code === 'Home') keyboard.clear();
    else if (name === 'keydown') keyboard.add(event.code); else keyboard.delete(event.code);
    keyboardAngle();
  });
  element.addEventListener('blur', reset);
  setAngle(0);
  return {element, reset};
}

export function decoratePedal(button, kind, label) {
  button.classList.add('pedal'); button.dataset.pedal = kind;
  const artwork = document.createElementNS('http://www.w3.org/2000/svg', 'svg');
  artwork.setAttribute('viewBox', '0 0 64 72'); artwork.setAttribute('aria-hidden', 'true');
  artwork.innerHTML = kind === 'accelerator'
    ? '<path d="M24 6Q24 3 28 3H46Q50 3 49 8L40 64Q39 68 35 68H18Q14 68 15 63Z"/><path class="pedal-grip" d="M28 16H43M26 26H41M24 36H39M22 46H37M20 56H35"/>'
    : '<path d="M8 20Q5 20 5 24L8 57Q8 61 12 61H52Q56 61 56 57L59 24Q59 20 55 20Z"/><path class="pedal-grip" d="M15 31H49M16 41H48M17 51H47"/>';
  const caption = document.createElement('span'); caption.textContent = label; caption.setAttribute('aria-hidden', 'true');
  button.replaceChildren(artwork, caption);
}
