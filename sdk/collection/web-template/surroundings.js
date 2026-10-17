import {Previews} from './previews.js';

// Suggestions occupy reported empty gutters, never the game or its controls.
export class Surroundings {
  constructor(host, launch) {
    this.launch = launch;
    this.rails = ['left','right'].map(side => {
      const rail = document.createElement('aside');
      rail.className = `game-rail rail-${side}`;
      rail.setAttribute('aria-label', 'More games'); rail.hidden = true;
      host.before(rail);
      return {element:rail, previews:new Previews({idPrefix:`rail-${side}`})};
    });
  }
  start(game,catalog) {
    this.active = true;
    const loaded = new Set([...document.querySelectorAll('#games .game-icon')]
      .filter(image => image.complete && image.naturalWidth && image.getAttribute('src'))
      .map(image => image.src));
    const candidates = catalog.filter(item => item.id !== game.id && item.icon);
    // Reuse decoded library art first; direct links fetch at most sixteen icons.
    this.games = candidates.sort((a,b) => Number(loaded.has(new URL(b.icon,location.href).href)) - Number(loaded.has(new URL(a.icon,location.href).href))).slice(0,16);
    this.render();
  }
  update(game,occupied) {
    this.layout = {game,occupied};
    this.render();
  }
  render() {
    if (!this.active || !this.layout) return;
    const {game,occupied} = this.layout;
    const gutters = [{x:0,width:game.x},{x:game.x+game.width,width:innerWidth-game.x-game.width}];
    this.rails.forEach(({element:rail,previews},index) => {
      const gutter = gutters[index],columns = gutter.width >= 208 ? 2 : 1;
      const width = Math.min(columns*104+(columns-1)*12,gutter.width-32);
      const size = (width-(columns-1)*12)/columns, x = gutter.x+(gutter.width-width)/2;
      let segments = [{top:72,bottom:innerHeight-24}];
      for (const rect of occupied) {
        if (rect.x >= x+width+8 || rect.x+rect.width <= x-8) continue;
        segments = segments.flatMap(segment => [
          {top:segment.top,bottom:Math.min(segment.bottom,rect.y-12)},
          {top:Math.max(segment.top,rect.y+rect.height+12),bottom:segment.bottom}
        ]).filter(segment => segment.bottom > segment.top);
      }
      const region = segments.sort((a,b) => (b.bottom-b.top)-(a.bottom-a.top))[0];
      const rows = Math.min(6,Math.floor(((region?.bottom-region?.top || 0)+12)/(size+12)));
      rail.hidden = size < 64 || rows < 2 || !this.games.length;
      if (rail.hidden) { previews.stop(); return; }
      const count = Math.min(rows*columns,this.games.length);
      Object.assign(rail.style,{left:`${x}px`,top:`${region.top}px`,width:`${width}px`,height:`${Math.ceil(count/columns)*(size+12)-12}px`});
      rail.style.setProperty('--rail-columns',columns);
      const signature = `${count}:${columns}:${this.games.map(game=>game.id).join(',')}`;
      if (rail.dataset.sequence === signature) return;
      previews.stop(); rail.dataset.sequence = signature;
      const list = document.createElement('div'); list.className = 'rail-group'; list.setAttribute('role','list');
      for (let item=0;item<count;item++) {
        const tile = previews.tile(this.games[(item+index*8)%this.games.length],link => this.launch(link));
        list.append(tile.element);
      }
      rail.replaceChildren(list);
    });
  }
  reset() {
    this.active = false; this.layout = null; this.games = [];
    for (const {element:rail,previews} of this.rails) {
      previews.stop(); rail.hidden = true; rail.replaceChildren(); delete rail.dataset.sequence;
    }
  }
}
