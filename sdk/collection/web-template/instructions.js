const platformNames = {macos:'macOS',windows:'Windows',linux:'Linux',ios:'iOS',android:'Android'};
function hostPlatform() {
  const agent = navigator.userAgent;
  if (/Android/i.test(agent)) return 'android';
  if (/iPhone|iPad|iPod/i.test(agent) || (navigator.platform === 'MacIntel' && navigator.maxTouchPoints > 1)) return 'ios';
  if (/Mac/i.test(navigator.platform)) return 'macos';
  if (/Win/i.test(navigator.platform)) return 'windows';
  if (/Linux/i.test(navigator.platform)) return 'linux';
  return null;
}
const keyLabels = {
  up:['↑','Up Arrow'],arrowup:['↑','Up Arrow'],down:['↓','Down Arrow'],arrowdown:['↓','Down Arrow'],
  left:['←','Left Arrow'],arrowleft:['←','Left Arrow'],right:['→','Right Arrow'],arrowright:['→','Right Arrow'],
  command:['⌘','Command'],cmd:['⌘','Command'],option:['⌥','Option'],shift:['⇧','Shift'],escape:['Esc','Escape'],
};
function element(tag, text, className) {
  const node = document.createElement(tag);
  if (text) node.textContent = text;
  if (className) node.className = className;
  return node;
}

// These are authored labels, not executable shortcuts or input remappings.
export function renderInstructions(container, instructions) {
  container.replaceChildren();
  const platform = hostPlatform();
  for (const section of instructions?.sections || []) {
    const content = element('section', '', 'instruction-section');
    if (section.title) content.append(element('h3', section.title));
    if (section.text) content.append(element('p', section.text));
    const controls = element('dl', '', 'instruction-controls');
    for (const control of section.controls || []) {
      const bindings = control.bindings.filter(binding => !binding.platforms || !platform || binding.platforms.includes(platform));
      if (!bindings.length) continue;
      const row = element('div', '', 'instruction-control');
      const action = element('dt', control.action);
      if (control.note) action.append(element('small', control.note));
      const inputs = element('dd');
      bindings.forEach((binding, index) => {
        if (index) inputs.append(element('span', 'or', 'instruction-or'));
        const combo = element('span', '', 'instruction-binding');
        if (binding.label) combo.append(element('span', binding.label, 'instruction-gesture'));
        for (const [index, key] of (binding.keys || []).entries()) {
          if (index) combo.append(element('span', '+', 'instruction-plus'));
          const [label, accessible] = keyLabels[key.toLowerCase()] || [key, key];
          const cap = element('kbd', label);
          cap.setAttribute('aria-label', accessible);
          combo.append(cap);
        }
        if (!platform && binding.platforms) combo.append(element('small', binding.platforms.map(name => platformNames[name]).join(' / ')));
        inputs.append(combo);
      });
      row.append(action, inputs); controls.append(row);
    }
    if (controls.children.length) content.append(controls);
    if (section.text || controls.children.length) container.append(content);
  }
  container.hidden = !container.children.length;
  return !container.hidden;
}
