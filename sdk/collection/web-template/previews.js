const motion = matchMedia('(prefers-reduced-motion: reduce)');
const finePointer = matchMedia('(hover: hover) and (pointer: fine)');
const interval = 3000;

export function downloadSize(bytes) {
  if (!Number.isFinite(bytes) || bytes <= 0) return '';
  return bytes < 1000000 ? `${Math.ceil(bytes / 1000)} KB` : `${(bytes / 1000000).toFixed(1)} MB`;
}

export function facts(game) {
  return [game.yearLabel, downloadSize(game.downloadBytes)].filter(Boolean).join(' · ');
}

export class Previews {
  constructor() {
    this.active = null;
    this.timer = null;
    document.addEventListener('visibilitychange', () => { if (document.hidden) this.stop(); });
    window.addEventListener('blur', () => this.stop());
    window.addEventListener('resize', () => this.stop());
    window.addEventListener('scroll', () => this.stop(), {passive: true});
    motion.addEventListener('change', () => this.stop());
  }

  tile(game, index, launch) {
    const element = document.createElement('div');
    element.className = 'game' + (game.featured ? ' featured' : '');
    element.setAttribute('role', 'listitem');
    const link = document.createElement('a');
    link.href = '#game=' + encodeURIComponent(game.id);
    link.dataset.id = game.id;
    link.setAttribute('aria-label', 'Play ' + game.name);
    const description = document.createElement('span');
    description.className = 'sr-only';
    description.id = 'facts-' + game.id;
    description.textContent = facts(game);
    link.setAttribute('aria-describedby', description.id);
    const icon = document.createElement('img');
    icon.className = 'game-icon';
    icon.src = game.icon || game.preview || 'assets/arcade.svg';
    icon.alt = ''; icon.width = 256; icon.height = 256;
    icon.loading = index < 16 ? 'eager' : 'lazy'; icon.decoding = 'async';
    icon.onerror = () => { icon.onerror = null; icon.src = 'assets/arcade.svg'; };
    const window_ = document.createElement('span');
    window_.className = 'preview-window';
    window_.setAttribute('aria-hidden', 'true');
    const track = document.createElement('span'); track.className = 'preview-track';
    const copy = document.createElement('span'); copy.className = 'preview-copy';
    const name = document.createElement('span'); name.className = 'game-name'; name.textContent = game.name;
    const stats = document.createElement('span'); stats.className = 'game-facts'; stats.textContent = facts(game);
    stats.title = [game.yearSource?.detail, game.downloadBytes ? 'Approximate first-load download, including the game and player. Previews excluded; caching may reduce it.' : ''].filter(Boolean).join(' ');
    copy.append(name, stats);
    const play = document.createElement('span'); play.className = 'preview-play';
    play.innerHTML = '<svg viewBox="0 0 24 24"><path d="m8 4 12 8-12 8z"/></svg>';
    const steps = document.createElement('span'); steps.className = 'preview-steps';
    const sources = [...new Set(game.screenshots?.length ? game.screenshots : [game.preview || game.icon].filter(Boolean))];
    const slides = sources.map(() => null);
    const bars = sources.map(() => {
      const bar = document.createElement('i'); steps.append(bar); return bar;
    });
    steps.hidden = sources.length < 2;
    window_.append(track, steps, play, copy);
    link.append(icon, window_, description); element.append(link);
    const tile = {game, element, link, track, slides, sources, bars, index: 0, token: 0};
    link.addEventListener('pointerenter', event => { if (event.pointerType !== 'touch' && finePointer.matches) this.start(tile); });
    link.addEventListener('pointerleave', () => { if (this.active === tile && document.activeElement !== link) this.stop(); });
    link.addEventListener('focus', () => {
      // Native keyboard focus scrolls first, so measure the final viewport position.
      requestAnimationFrame(() => {
        if (document.activeElement === link && (finePointer.matches || link.matches(':focus-visible'))) this.start(tile);
      });
    });
    link.addEventListener('blur', () => { if (this.active === tile) this.stop(); });
    link.addEventListener('keydown', event => { if (event.key === 'Escape') this.stop(); });
    link.addEventListener('click', () => { this.stop(); launch(link); });
    return tile;
  }

  async image(tile, index) {
    if (tile.slides[index]) return tile.slides[index];
    const image = new Image();
    image.className = 'preview-frame'; image.alt = ''; image.decoding = 'async';
    image.src = tile.sources[index];
    try { await image.decode(); } catch { return null; }
    tile.slides[index] = image;
    return image;
  }

  async start(tile) {
    if (this.active === tile) return;
    this.stop();
    this.active = tile;
    clearTimeout(tile.closingTimer);
    tile.element.classList.remove('preview-closing');
    const token = ++tile.token;
    const rect = tile.element.getBoundingClientRect(), edge = 8;
    const width = Math.min(innerWidth - edge * 2, rect.width * 1.85);
    const height = Math.min(innerHeight - edge * 2, rect.height * 1.12);
    const left = Math.max(edge, Math.min(innerWidth - edge - width, rect.x + rect.width / 2 - width / 2));
    const top = Math.max(edge, Math.min(innerHeight - edge - height, rect.y + rect.height / 2 - height / 2));
    tile.link.style.setProperty('--preview-width', `${width}px`);
    tile.link.style.setProperty('--preview-height', `${height}px`);
    tile.link.style.setProperty('--preview-left', `${left - rect.x}px`);
    tile.link.style.setProperty('--preview-top', `${top - rect.y}px`);
    tile.element.classList.add('previewing');
    tile.index = 0;
    tile.track.replaceChildren();
    const image = await this.image(tile, 0);
    if (this.active !== tile || tile.token !== token) return;
    if (image) tile.track.replaceChildren(image);
    else tile.track.replaceChildren(tile.link.querySelector('.game-icon').cloneNode());
    tile.element.classList.add('preview-ready');
    this.progress(tile);
    this.schedule(tile);
  }

  progress(tile) {
    tile.bars.forEach((bar, index) => {
      bar.className = index === tile.index ? 'current' : index < tile.index ? 'seen' : '';
      bar.style.setProperty('--duration', `${interval}ms`);
    });
  }

  schedule(tile) {
    clearTimeout(this.timer);
    if (motion.matches || tile.sources.length < 2) return;
    this.timer = setTimeout(() => this.next(tile), interval);
  }

  async next(tile) {
    if (this.active !== tile) return;
    const token = tile.token, index = (tile.index + 1) % tile.sources.length;
    const image = await this.image(tile, index);
    if (this.active !== tile || tile.token !== token) return;
    if (image) {
      tile.index = index;
      tile.track.append(image);
      tile.track.classList.add('sliding');
      const finish = () => {
        if (this.active !== tile || tile.token !== token) return;
        tile.track.classList.remove('sliding');
        tile.track.replaceChildren(image);
        this.progress(tile);
        this.schedule(tile);
      };
      this.timer = setTimeout(finish, 560);
    } else {
      // An unavailable image must not trap the gallery on a broken frame.
      tile.index = index;
      this.schedule(tile);
    }
  }

  stop() {
    clearTimeout(this.timer);
    if (!this.active) return;
    this.active.token++;
    const tile = this.active;
    tile.element.classList.add('preview-closing');
    tile.element.classList.remove('previewing', 'preview-ready');
    tile.track.classList.remove('sliding');
    tile.closingTimer = setTimeout(() => tile.element.classList.remove('preview-closing'), 340);
    this.active = null;
  }
}
