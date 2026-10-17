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
      return {element:rail, previews:new Previews({idPrefix:`rail-${side}`,root:rail})};
    });
    document.addEventListener('visibilitychange',()=>{
      for(const {element} of this.rails)element.classList.toggle('page-hidden',document.hidden);
    });
  }
  start(game,catalog) {
    this.active = true;
    const loaded = new Set([...document.querySelectorAll('#games .game-icon')]
      .filter(image => image.complete && image.naturalWidth && image.getAttribute('src'))
      .map(image => image.closest('.game').querySelector('.game-launch').dataset.id));
    const candidates = catalog.filter(item => item.id !== game.id && item.icon);
    // Reuse library art first. Repeated tracks share a bounded thumbnail deck.
    this.games = candidates.sort((a,b) => Number(loaded.has(b.id)) - Number(loaded.has(a.id))).slice(0,48);
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
      const gutter = gutters[index],columns = Math.max(1,Math.min(4,Math.floor((gutter.width-20+12)/116)));
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
      const height=region?.bottom-region?.top || 0;
      const rows = Math.ceil((height+12)/(size+12));
      rail.hidden = size < 64 || rows < 2 || !this.games.length;
      if (rail.hidden) { previews.stop(); return; }
      rail.classList.toggle('narrow',width<180);
      Object.assign(rail.style,{left:`${x}px`,top:`${region.top}px`,width:`${width}px`,height:`${height}px`});
      rail.style.setProperty('--rail-columns',columns);
      const signature = `${rows}:${columns}:${size}:${this.games.map(game=>game.id).join(',')}`;
      if (rail.dataset.sequence === signature) return;
      previews.clear(); rail.dataset.sequence = signature;
      const list = document.createElement('div'); list.className = 'rail-group';
      const count=rows+1;
      for(let column=0;column<columns;column++) {
        const viewport=document.createElement('div');viewport.className='rail-column';
        const track=document.createElement('div');track.className='rail-track';
        track.style.setProperty('--rail-duration',`${count*9}s`);
        track.style.setProperty('--rail-distance',`${count*(size+12)}px`);
        for(let copy=0;copy<2;copy++) {
          const group=document.createElement('div');group.className='rail-cycle';group.setAttribute('role','list');
          for(let item=0;item<count;item++) {
            const game=this.games[(index*24+column*count+item)%this.games.length];
            const tile=previews.tile(game,link=>this.launch(link),`${column}-${copy}-${item}`);
            tile.link.title=game.name;
            tile.element.classList.remove('featured');
            group.append(tile.element);
          }
          track.append(group);
        }
        viewport.append(track);list.append(viewport);
      }
      const pause=document.createElement('button');pause.type='button';pause.className='rail-pause';
      const paused=rail.classList.contains('paused');
      pause.setAttribute('aria-label',paused?'Resume suggestions':'Pause suggestions');pause.setAttribute('aria-pressed',String(paused));
      pause.innerHTML='<svg viewBox="0 0 24 24" aria-hidden="true"><path d="M8 5v14M16 5v14"/></svg>';
      pause.onclick=()=>{
        const paused=rail.classList.toggle('paused');pause.setAttribute('aria-pressed',String(paused));
        pause.setAttribute('aria-label',paused?'Resume suggestions':'Pause suggestions');
      };
      rail.replaceChildren(list,pause);
    });
  }
  reset() {
    this.active = false; this.layout = null; this.games = [];
    for (const {element:rail,previews} of this.rails) {
      previews.clear(); rail.hidden = true; rail.replaceChildren(); delete rail.dataset.sequence;
    }
  }
}
