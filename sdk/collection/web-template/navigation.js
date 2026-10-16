const directions = {ArrowLeft: [-1, 0], ArrowRight: [1, 0], ArrowUp: [0, -1], ArrowDown: [0, 1]};

// Use the resting tile as geometry when focus expands its interactive preview.
// The caller's selector defines navigation nodes, leaving secondary links in
// the normal Tab order without turning them into extra grid cells.
export function bindGridNavigation(container, selector, {geometry = element => element, onUpBoundary} = {}) {
  const navigate = event => {
    const direction = directions[event.key];
    if (!direction || event.defaultPrevented || event.altKey || event.ctrlKey || event.metaKey || event.shiftKey ||
        event.target.closest('input,textarea,select,[contenteditable="true"]')) return;
    const current = event.target.closest(selector);
    if (!current || !container.contains(current)) return;
    const rect = geometry(current).getBoundingClientRect();
    const horizontal = direction[0] !== 0, sign = direction[horizontal ? 0 : 1];
    const primary = box => horizontal ? [box.left, box.right] : [box.top, box.bottom];
    const cross = box => horizontal ? [box.top, box.bottom] : [box.left, box.right];
    const origin = primary(rect), lane = cross(rect);
    const candidates = [...container.querySelectorAll(selector)].flatMap(element => {
      if (element === current || element.matches(':disabled,[aria-disabled="true"]') || !element.getClientRects().length) return [];
      const box = geometry(element).getBoundingClientRect();
      if (!box.width || !box.height) return [];
      const axis = primary(box), across = cross(box);
      const gap = sign > 0 ? axis[0] - origin[1] : origin[0] - axis[1];
      if (gap < -1) return [];
      const overlap = Math.min(lane[1], across[1]) - Math.max(lane[0], across[0]);
      const crossGap = Math.max(0, lane[0] - across[1], across[0] - lane[1]);
      const center = Math.abs(across[0] + across[1] - lane[0] - lane[1]) / 2;
      return [{element, beam: overlap > 1, distance: Math.hypot(Math.max(0, gap), crossGap), center}];
    });
    candidates.sort((a, b) => Number(b.beam) - Number(a.beam) || a.distance - b.distance || a.center - b.center);
    if (candidates.length) {
      event.preventDefault();
      candidates[0].element.focus();
    } else if (event.key === 'ArrowUp' && onUpBoundary) {
      event.preventDefault();
      onUpBoundary();
    }
  };
  container.addEventListener('keydown', navigate);
  return () => container.removeEventListener('keydown', navigate);
}
