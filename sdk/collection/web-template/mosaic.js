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
    if(this.filtered===filtered && tiles.length===this.tiles.length && tiles.every((tile,index)=>tile===this.tiles[index]))return;
    const animate = !matchMedia('(prefers-reduced-motion: reduce)').matches;
    const before = animate ? this.positions() : null;
    this.grid.querySelectorAll('.game-exit').forEach(element=>element.remove());
    this.filtered = filtered;
    const visible = new Set(tiles);
    const exits = [];
    for (const tile of this.tiles) if (!visible.has(tile)) {
      const rect = before?.get(tile.element);
      if (rect) exits.push([tile,rect]);
      tile.element.remove();
    }
    this.tiles = tiles;
    this.arrange(animate,before);
    if(exits.length)this.fadeOut(exits);
  }

  positions() {
    const positions = new Map();
    for (const tile of this.tiles) {
      const rect = tile.element.getBoundingClientRect();
      if (rect.width && rect.bottom > 0 && rect.top < innerHeight) positions.set(tile.element,rect);
    }
    // Read the interrupted visual positions before canceling previous movement.
    for (const tile of this.tiles) tile.motion?.cancel();
    return positions;
  }

  fadeOut(exits) {
    // An inert, short-lived copy leaves the real result list immediately.
    const fragment = document.createDocumentFragment(), ghosts = [];
    for(const [tile,rect] of exits) {
      if(!tile.element.querySelector('.game-icon').currentSrc)continue;
      const ghost = document.createElement('div'), icon = tile.element.querySelector('.game-icon').cloneNode();
      ghost.className = 'game-exit'; ghost.inert = true; ghost.setAttribute('aria-hidden','true');
      icon.removeAttribute('srcset'); icon.src = tile.element.querySelector('.game-icon').currentSrc;
      icon.className = 'game-icon';
      Object.assign(ghost.style,{left:`${rect.x}px`,top:`${rect.y}px`,width:`${rect.width}px`,height:`${rect.height}px`});
      ghost.append(icon); fragment.append(ghost); ghosts.push(ghost);
    }
    this.grid.append(fragment);
    for(const ghost of ghosts)ghost.animate([{opacity:.65,transform:'none'},{opacity:0,transform:'scale(.92)'}],{duration:160,easing:'ease-out'}).finished.then(()=>ghost.remove(),()=>ghost.remove());
  }

  shuffle(animate = true, before = null) {
    this.order = shuffled(this.order || this.tiles);
    this.highlights = null;
    this.arrange(animate,before);
  }

  arrange(animate = false, before = null) {
    if(animate)before ??= this.positions();
    this.onShuffle?.();
    const sparse = this.filtered || this.tiles.length < 12;
    if(!sparse && !this.highlights) {
      const picks=shuffled(this.tiles.filter(tile=>!tile.game.featured && tile.game.featuredPool)).slice(0,10);
      this.highlights=shuffled([...this.tiles.filter(tile=>tile.game.featured),...picks]);
      this.highlightTargets=shuffled(this.highlights.map((_,index)=>(index+Math.random())/this.highlights.length));
    }
    this.featured = sparse ? [] : this.highlights;
    // Extra highlights belong to this arrangement, never to the game metadata.
    // Refresh their classes on Shuffle and keep the picks stable on resize.
    const highlights = new Set(this.featured);
    this.tiles.forEach(tile => tile.element.classList.toggle('featured', highlights.has(tile)));
    // Jitter one target in each horizontal band, then mix their order. Keep
    // these choices until Shuffle so responsive reflow stays predictable.
    this.featuredTargets = sparse ? [] : this.highlightTargets;
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
    const rank = new Map(this.order.map((tile,index)=>[tile,index]));
    for (const [name, family] of families) {
      const members = family.sort((a,b)=>rank.get(a)-rank.get(b));
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
    this.ordinary = [...ordinary.filter(tile => !grouped.has(tile)), ...clusters].sort((a,b)=>
      (rank.get(a.tiles?.[0]||a)??0)-(rank.get(b.tiles?.[0]||b)??0));
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
    if(animate)before ??= this.positions();
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
        if(!tile.tiles)tile.element.querySelector('.game-icon').sizes=`${Math.ceil(unit*size+gap*(size-1))}px`;
      }
    };
    // Phones place the library tools above the separate game grid.
    const tools=[...this.grid.querySelector('.library-tools').children].filter(element=>!element.hidden);
    const utilities=tools.length;
    if(!mobile)tools.forEach((element,index)=>reserve(null,index*2,0,2));
    const rows = Math.max(2, Math.ceil(((mobile ? 0 : utilities) + this.ordinary.reduce((sum, block) => sum + (block.tiles ? 4 : 1), 0) + this.featured.length * 4) / columns));
    const usedColumns = new Set();
    let previous = null;
    this.featured.forEach((tile, index) => {
      // Keep the first two normal-tile rows for tools and smaller games.
      let y = Math.max(4, Math.round(index * Math.max(0, rows - 2) / Math.max(1, this.featured.length - 1)) * 2);
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
            member.element.querySelector('.game-icon').sizes=`${Math.ceil((unit*size+gap*(size-1)-gap*2)/3)}px`;
          });
          placed = true;
          break;
        }
      }
    }
    const focus = document.activeElement;
    const ordered = [...this.tiles].sort((a, b) => a.y - b.y || a.x - b.x);
    const blocks = [...this.featured, ...this.ordinary].sort((a, b) => a.y - b.y || a.x - b.x);
    // Move only changed nodes. Unchanged results keep decoded artwork, focus
    // and their DOM position while the query is refined.
    let next=this.games.firstElementChild;
    for(const {element} of blocks) {
      if(element===next)next=next.nextElementSibling;
      else this.games.insertBefore(element,next);
    }
    if (focus && this.games.contains(focus)) focus.focus({preventScroll: true});
    if (animate && !matchMedia('(prefers-reduced-motion: reduce)').matches) {
      // Batch geometry reads before creating animations to avoid forcing style
      // and layout once per card while typing through a large catalog.
      const positions = ordered.map(tile=>[tile,tile.element.getBoundingClientRect()]);
      for (const [tile,rect] of positions) {
        const old = before.get(tile.element);
        if (rect.top > innerHeight || rect.bottom < 0) continue;
        if(old && Math.abs(old.x-rect.x)<.5 && Math.abs(old.y-rect.y)<.5 && Math.abs(old.width-rect.width)<.5)continue;
        tile.motion = tile.element.animate([
          {transform: old?.width ? `translate(${old.x - rect.x}px, ${old.y - rect.y}px) scale(${old.width/rect.width},${old.height/rect.height})` : 'scale(.92)', opacity: old?.width ? 1 : 0},
          {transform: 'none', opacity: 1}
        ], {duration: 320, easing: 'cubic-bezier(.16,1,.3,1)'});
      }
    }
  }
}
