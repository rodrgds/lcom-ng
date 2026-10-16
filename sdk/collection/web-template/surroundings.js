// Ambient catalogue artwork lives behind the embedded player, never over it.
export class Surroundings {
  constructor(host) {
    this.rails = ['left','right'].map(side => {
      const rail = document.createElement('div');
      rail.className = `game-rail rail-${side}`;
      rail.setAttribute('aria-hidden','true'); rail.hidden = true;
      host.before(rail);
      return rail;
    });
    document.addEventListener('visibilitychange', () => {
      for (const rail of this.rails) rail.classList.toggle('paused',document.hidden);
    });
  }
  start(game,catalog) {
    this.active = true;
    // Borrow decoded library images first. A direct game link needs at most
    // eight new icons, and only if a gutter is wide enough to show them.
    const cached = [...document.querySelectorAll('#games .game-icon')]
      .filter(image => image.complete && image.naturalWidth && image.getAttribute('src'))
      .map(image => image.src);
    this.sources = [...new Set(cached.length ? cached :
      catalog.filter(item => item.id !== game.id && item.icon).map(item => item.icon))].slice(0,8);
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
    this.rails.forEach((rail,index) => {
      rail.classList.toggle('paused',document.hidden);
      const gutter = gutters[index],size = Math.min(104,gutter.width-48);
      const x = gutter.x+(gutter.width-size)/2,top = 72,bottom = innerHeight-24;
      const blocked = occupied.some(rect => rect.x < x+size+12 && rect.x+rect.width > x-12 && rect.y < bottom && rect.y+rect.height > top);
      rail.hidden = size < 64 || bottom-top < 160 || blocked || !this.sources.length;
      if (rail.hidden) return;
      rail.style.left = `${x}px`; rail.style.width = `${size}px`;
      const count = Math.ceil((bottom-top)/(size+16))+1;
      const signature = `${count}:${size}`;
      if (rail.dataset.sequence === signature) return;
      rail.dataset.sequence = signature;
      const track = document.createElement('div'); track.className = 'rail-track';
      for (let copy=0;copy<2;copy++) {
        const group = document.createElement('div'); group.className = 'rail-group';
        for (let item=0;item<count;item++) {
          const image = document.createElement('img'); image.alt = ''; image.decoding = 'async';
          image.width = size; image.height = size;
          image.src = this.sources[(item+index*3)%this.sources.length];
          image.onerror = () => { image.onerror = null; image.src = 'assets/arcade.svg'; };
          group.append(image);
        }
        track.append(group);
      }
      rail.replaceChildren(track);
    });
  }
  reset() {
    this.active = false; this.layout = null; this.sources = [];
    for (const rail of this.rails) {
      rail.hidden = true; rail.replaceChildren(); delete rail.dataset.sequence;
    }
  }
}
