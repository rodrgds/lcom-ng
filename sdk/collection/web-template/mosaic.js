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

  setTiles(tiles, filtered = true) {
    this.filtered = filtered;
    const before = new Map(this.tiles.map(tile => [tile.element, tile.element.getBoundingClientRect()]));
    const visible = new Set(tiles);
    for (const tile of this.tiles) if (!visible.has(tile)) tile.element.remove();
    this.tiles = tiles;
    this.shuffle(true, before);
  }

  shuffle(animate = true, before = null) {
    before ??= new Map(this.tiles.map(tile => [tile.element, tile.element.getBoundingClientRect()]));
    this.onShuffle?.();
    const candidates = shuffled(this.tiles.filter(tile => !tile.game.featured));
    const sparse = this.filtered || this.tiles.length < 12;
    const picks = sparse ? [] : shuffled(candidates.filter(tile=>tile.game.featuredPool)).slice(0,5);
    this.featured = sparse ? [] : shuffled([...this.tiles.filter(tile=>tile.game.featured),...picks]);
    // Extra highlights belong to this arrangement, never to the game metadata.
    // Refresh their classes on Shuffle and keep the picks stable on resize.
    const highlights = new Set(this.featured);
    this.tiles.forEach(tile => tile.element.classList.toggle('featured', highlights.has(tile)));
    // Jitter one target in each horizontal band, then mix their order. Keep
    // these choices until Shuffle so responsive reflow stays predictable.
    this.featuredTargets = shuffled(this.featured.map((_, index) => (index + Math.random()) / this.featured.length));
    // Only full sets of nine become compact blocks. A featured game keeps its
    // own space, and the remaining variants stay ordinary playable tiles.
    this.tiles.forEach(tile => tile.element.classList.remove('compact'));
    const families = new Map();
    const ordinary = this.tiles.filter(tile=>!highlights.has(tile));
    for (const tile of ordinary) {
      const key = tile.game.archetype;
      if (key && !sparse) {
        if (!families.has(key)) families.set(key, []);
        families.get(key).push(tile);
      }
    }
    const grouped = new Set(), clusters = [];
    for (const [name, family] of families) {
      const members = shuffled(family);
      while (members.length >= 9) {
        const tiles = members.splice(0, 9);
        const element = document.createElement('div');
        element.className = 'game-cluster';
        element.setAttribute('role', 'listitem');
        const grid = document.createElement('div');
        grid.className = 'game-cluster-grid';
        grid.setAttribute('role', 'list');
        grid.setAttribute('aria-label', name + ' games');
        for (const tile of tiles) {
          grouped.add(tile);
          tile.element.classList.add('compact');
          tile.element.style.gridColumn = '';
          tile.element.style.gridRow = '';
          grid.append(tile.element);
        }
        element.append(grid);
        clusters.push({element, tiles, size: 4});
      }
    }
    this.grid.querySelectorAll('.game-cluster').forEach(group => group.remove());
    this.ordinary = shuffled([...ordinary.filter(tile => !grouped.has(tile)), ...clusters]);
    this.width = 0;
    this.layout(animate,before);
  }

  layout(animate = false, before = null) {
    const width = this.grid.clientWidth;
    if (!width || width === this.width) return;
    this.width = width;
    const mobile = matchMedia('(max-width:600px)').matches;
    const gap = mobile ? 8 : 12;
    const columns = mobile ? 3 : Math.max(2, Math.min(18, Math.floor((width + gap) / 136)));
    const tracks = columns * 2;
    const unit = (width - (tracks - 1) * gap) / tracks;
    before ??= new Map(this.tiles.map(tile => [tile.element, tile.element.getBoundingClientRect()]));
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
    const tools=[...this.grid.querySelector('.library-tools').children].filter(element=>!element.hidden);
    const utilities=tools.reduce((sum,element)=>sum+(element.classList.contains('browse-tile')?2:1),0);
    if(!mobile){let x=0;for(const element of tools){const span=element.classList.contains('browse-tile')?4:2;for(let dx=0;dx<span;dx+=2)reserve(null,x+dx,0,2);x+=span;}}
    const rows = Math.max(2, Math.ceil(((mobile ? 0 : utilities) + this.ordinary.reduce((sum, block) => sum + (block.tiles ? 4 : 1), 0) + this.featured.length * 4) / columns));
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
      const size = tile.size || 2;
      let placed = false;
      for (let y = 0; !placed; y += 2) {
        for (let x = 0; x <= tracks - size; x += 2) {
          if (!free(x, y, size)) continue;
          reserve(tile, x, y, size);
          tile.tiles?.forEach((member, index) => {
            member.x = x + index % 3 * size / 3;
            member.y = y + Math.floor(index / 3) * size / 3;
          });
          placed = true;
          break;
        }
      }
    }
    const focus = document.activeElement;
    const ordered = [...this.tiles].sort((a, b) => a.y - b.y || a.x - b.x);
    const blocks = [...this.featured, ...this.ordinary].sort((a, b) => a.y - b.y || a.x - b.x);
    this.games.append(...blocks.map(tile => tile.element));
    if (focus && this.games.contains(focus)) focus.focus({preventScroll: true});
    if (animate && !matchMedia('(prefers-reduced-motion: reduce)').matches) {
      for (const tile of ordered) {
        const rect = tile.element.getBoundingClientRect(), old = before.get(tile.element);
        if (rect.top > innerHeight || rect.bottom < 0) continue;
        tile.element.animate([
          {transform: old?.width ? `translate(${old.x - rect.x}px, ${old.y - rect.y}px)` : 'scale(.96)', opacity: old?.width ? .7 : 0},
          {transform: 'none', opacity: 1}
        ], {duration: 540, easing: 'cubic-bezier(.16,1,.3,1)'});
      }
    }
  }
}
