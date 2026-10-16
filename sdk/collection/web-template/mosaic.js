// Pack square tiles, reserving separate rows for the larger featured games.
function shuffled(items) {
  const result = [...items];
  for (let i = result.length - 1; i > 0; i--) {
    const j = Math.floor(Math.random() * (i + 1));
    [result[i], result[j]] = [result[j], result[i]];
  }
  return result;
}

export class Mosaic {
  constructor(grid, games, tiles, onShuffle) {
    this.grid = grid;
    this.games = games;
    this.tiles = tiles;
    this.onShuffle = onShuffle;
    this.shuffle(false);
    this.observer = new ResizeObserver(() => this.layout());
    this.observer.observe(grid);
  }

  shuffle(animate = true) {
    this.onShuffle?.();
    this.featured = shuffled(this.tiles.filter(tile => tile.game.featured));
    // Jitter one target in each horizontal band, then mix their order. Keep
    // these choices until Shuffle so responsive reflow stays predictable.
    this.featuredTargets = shuffled(this.featured.map((_, index) => (index + Math.random()) / this.featured.length));
    this.ordinary = shuffled(this.tiles.filter(tile => !tile.game.featured));
    this.width = 0;
    this.layout(animate);
  }

  layout(animate = false) {
    const width = this.grid.clientWidth;
    if (!width || width === this.width) return;
    this.width = width;
    const mobile = matchMedia('(max-width:600px)').matches;
    const gap = mobile ? 8 : 12;
    const columns = mobile ? 3 : Math.max(2, Math.min(18, Math.floor((width + gap) / 144)));
    const tracks = columns * 2;
    const unit = (width - (tracks - 1) * gap) / tracks;
    const before = new Map(this.tiles.map(tile => [tile.element, tile.element.getBoundingClientRect()]));
    this.grid.style.setProperty('--tracks', tracks);
    this.grid.style.setProperty('--unit', `${unit}px`);
    this.grid.style.setProperty('--gap', `${gap}px`);
    const occupied = [];
    const free = (x, y, size) => {
      for (let row = y; row < y + size; row++) {
        for (let col = x; col < x + size; col++) if (occupied[row]?.[col]) return false;
      }
      return true;
    };
    const reserve = (tile, x, y, size) => {
      for (let row = y; row < y + size; row++) {
        occupied[row] ??= [];
        for (let col = x; col < x + size; col++) occupied[row][col] = true;
      }
      if (tile) {
        Object.assign(tile, {x, y});
        tile.element.style.gridColumn = `${x + 1} / span ${size}`;
        tile.element.style.gridRow = `${y + 1} / span ${size}`;
      }
    };
    // Phones place the two utility tiles above the separate game grid.
    if (!mobile) { reserve(null, 0, 0, 2);reserve(null, 2, 0, 2); }
    const rows = Math.max(2, Math.ceil(((mobile ? 0 : 2) + this.ordinary.length + this.featured.length * 4) / columns));
    const usedColumns = new Set();
    let previous = null;
    this.featured.forEach((tile, index) => {
      let y = Math.round(index * Math.max(0, rows - 2) / Math.max(1, this.featured.length - 1)) * 2;
      if (previous) y = Math.max(y, previous.y + 2);
      let candidates;
      do {
        candidates = [];
        for (let x = 0; x <= tracks - 4; x += 2) if (free(x, y, 4)) candidates.push(x);
        if (!candidates.length) y += 2;
      } while (!candidates.length);
      // The nearest free slot may be central or near an edge. Stratified
      // targets spread highlights across the board without an edge zigzag.
      // Prefer unused columns when reserved tiles displace several targets.
      const target = this.featuredTargets[index] * (tracks - 4);
      candidates.sort((a, b) => Number(usedColumns.has(a)) - Number(usedColumns.has(b)) || Math.abs(a - target) - Math.abs(b - target));
      const x = candidates[0];
      usedColumns.add(x);
      reserve(tile, x, y, 4);
      previous = {x, y};
    });
    for (const tile of this.ordinary) {
      let placed = false;
      for (let y = 0; !placed; y += 2) {
        for (let x = 0; x <= tracks - 2; x += 2) {
          if (!free(x, y, 2)) continue;
          reserve(tile, x, y, 2);
          placed = true;
          break;
        }
      }
    }
    const focus = document.activeElement;
    const ordered = [...this.tiles].sort((a, b) => a.y - b.y || a.x - b.x);
    this.games.append(...ordered.map(tile => tile.element));
    if (focus && this.games.contains(focus)) focus.focus({preventScroll: true});
    if (animate && !matchMedia('(prefers-reduced-motion: reduce)').matches) {
      for (const tile of ordered) {
        const rect = tile.element.getBoundingClientRect(), old = before.get(tile.element);
        if (!old?.width || rect.top > innerHeight || rect.bottom < 0) continue;
        tile.element.animate([
          {transform: `translate(${old.x - rect.x}px, ${old.y - rect.y}px)`, opacity: .6},
          {transform: 'none', opacity: 1}
        ], {duration: 540, easing: 'cubic-bezier(.16,1,.3,1)'});
      }
    }
  }
}
