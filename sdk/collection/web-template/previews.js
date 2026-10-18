import {Thumbnails} from './thumbnails.js';

const motion = matchMedia('(prefers-reduced-motion: reduce)');
const finePointer = matchMedia('(hover: hover) and (pointer: fine)');
const interval = 2400;

export function downloadSize(bytes) {
  if (!Number.isFinite(bytes) || bytes <= 0) return '';
  return bytes < 1000000 ? `${Math.ceil(bytes / 1000)} KB` : `${(bytes / 1000000).toFixed(1)} MB`;
}

export function facts(game) {
  const year = 'academicYear' in game ? game.academicYear || '' : String(game.yearLabel || game.year || '').replace(/^Updated\s+/, '');
  return [year, downloadSize(game.downloadBytes)].filter(Boolean).join(' · ');
}

export function repositoryIcon(url) {
  return /^https?:\/\/(?:www\.)?github\.com(?:[/:]|$)/i.test(url)
    ? '<svg viewBox="0 0 24 24" aria-hidden="true"><path fill="currentColor" stroke="none" d="M12 .75a11.25 11.25 0 0 0-3.56 21.92c.56.1.77-.24.77-.54v-2.1c-3.14.68-3.8-1.33-3.8-1.33-.51-1.3-1.25-1.65-1.25-1.65-1.03-.71.08-.7.08-.7 1.14.08 1.74 1.17 1.74 1.17 1.01 1.73 2.65 1.23 3.3.94.1-.73.4-1.23.72-1.51-2.51-.29-5.15-1.26-5.15-5.59 0-1.23.44-2.23 1.16-3.02-.12-.29-.5-1.43.11-2.98 0 0 .95-.3 3.1 1.15a10.8 10.8 0 0 1 5.64 0c2.15-1.46 3.1-1.15 3.1-1.15.61 1.55.23 2.69.11 2.98.72.79 1.16 1.79 1.16 3.02 0 4.34-2.64 5.3-5.16 5.58.41.35.77 1.03.77 2.08v3.1c0 .3.21.65.78.54A11.25 11.25 0 0 0 12 .75Z"/></svg>'
    : '<svg viewBox="0 0 24 24" aria-hidden="true"><path d="M14 4h6v6M20 4 10 14M10 6H4v14h14v-6"/></svg>';
}

export class Previews {
  constructor({idPrefix = 'facts', root = null} = {}) {
    this.idPrefix = idPrefix;
    this.root = root;
    this.active = null;
    this.timer = null;
    this.thumbnails = new Thumbnails();
    this.tiles = new WeakMap();
    this.thumbnailObserver = !root && finePointer.matches && typeof IntersectionObserver !== 'undefined' ? new IntersectionObserver(entries => {
      for (const entry of entries) {
        const tile = this.tiles.get(entry.target);
        if (!tile) continue;
        tile.visible = entry.isIntersecting;
        if (!tile.visible) this.thumbnails.queue.delete(tile.game.screenshotThumbnail);
        else {
          const icon = tile.link.querySelector('.game-icon');
          if (icon.complete && icon.naturalWidth) this.thumbnails.warm(tile.game.screenshotThumbnail);
        }
      }
    }) : null;
    // Native lazy loading can fetch several screens ahead. Observe the final
    // shuffled tile positions and only assign sources near the viewport.
    this.iconObserver = typeof IntersectionObserver === 'undefined' ? null : new IntersectionObserver(entries => {
      for (const entry of entries) {
        if(root)entry.target.querySelector('.game-launch').tabIndex=entry.isIntersecting?0:-1;
        if(entry.isIntersecting)this.loadIcon(entry.target);
      }
    }, {root,rootMargin:root?'0px':'160px 0px'});
    document.addEventListener('visibilitychange', () => {
      if (document.hidden) this.stop();
      else this.thumbnails.schedule();
    });
    window.addEventListener('blur', () => this.stop());
    window.addEventListener('resize', () => this.stop());
    window.addEventListener('scroll', () => {
      if (this.active?.element.contains(document.activeElement)) {
        const rect = this.position(this.active);
        if (rect.bottom > 0 && rect.top < innerHeight && rect.right > 0 && rect.left < innerWidth) return;
      }
      this.stop();
    }, {passive: true});
    motion.addEventListener('change', () => this.stop());
  }

  clear() {
    this.stop(); this.iconObserver?.disconnect(); this.thumbnailObserver?.disconnect(); this.thumbnails.clear();
  }

  loadIcon(element) {
    const icon = element.querySelector('.game-icon');
    if (!icon.dataset.src) return;
    if(!this.root)this.iconObserver?.unobserve(element);
    if (this.iconObserver) icon.loading = 'eager';
    const bounds = element.getBoundingClientRect();
    const visible = bounds.top < innerHeight && bounds.bottom > 0;
    icon.fetchPriority = this.root || !visible ? 'low' : element.classList.contains('featured') ? 'high' : 'auto';
    icon.sizes = `${Math.ceil(bounds.width)}px`;
    if(icon.dataset.srcset) {icon.srcset=icon.dataset.srcset;delete icon.dataset.srcset;}
    icon.src = icon.dataset.src;
    delete icon.dataset.src;
  }

  tile(game, launch, key = game.id) {
    const element = document.createElement('div');
    element.className = 'game' + (game.featured ? ' featured' : '');
    element.setAttribute('role', 'listitem');
    const link = document.createElement('a');
    link.className = 'game-launch';
    link.href = '#game=' + encodeURIComponent(game.id);
    link.dataset.id = game.id;
    if(this.root&&this.iconObserver)link.tabIndex=-1;
    link.setAttribute('aria-label', 'Play ' + game.name);
    const description = document.createElement('span');
    description.className = 'sr-only';
    description.id = this.idPrefix + '-' + key;
    description.textContent = facts(game);
    link.setAttribute('aria-describedby', description.id);
    const icon = document.createElement('img');
    icon.className = 'game-icon pending';
    icon.dataset.src = game.iconThumbnail || game.icon || game.preview || 'assets/arcade.svg';
    if(game.iconSources) {
      icon.dataset.srcset=game.iconSources.map(source=>`${source.path} ${source.width}w`).join(', ');
      icon.sizes='(max-width:600px) 34vw, 300px';
    }
    icon.alt = ''; icon.width = 256; icon.height = 256;
    icon.loading = 'lazy'; icon.decoding = 'async';
    icon.onload = async () => {
      try { await icon.decode(); } catch { /* A loaded fallback can still be shown. */ }
      icon.classList.remove('pending');
      if (tile.visible) this.thumbnails.warm(game.screenshotThumbnail);
    };
    icon.onerror = () => { icon.onerror = null; icon.removeAttribute('srcset'); icon.src = 'assets/arcade.svg'; };
    link.append(icon,description); element.append(link);
    const tile = {game,element,link,index:0,token:0};
    this.tiles.set(element, tile);
    this.thumbnailObserver?.observe(element);
    element.addEventListener('pointerenter', event => {
      // Keep keyboard actions reachable while the pointer crosses other tiles.
      if (event.pointerType !== 'touch' && finePointer.matches && !this.hasFocusedAction()) this.start(tile);
    });
    element.addEventListener('pointerleave', () => { if (this.active === tile && !element.contains(document.activeElement)) this.stop(); });
    link.addEventListener('focus', () => {
      // Native keyboard focus scrolls first, so measure the final viewport position.
      const active = this.active, token = active?.token;
      requestAnimationFrame(() => {
        if ((!this.active || this.active === active && active?.token === token) && document.activeElement === link &&
            (finePointer.matches || link.matches(':focus-visible'))) this.start(tile);
      });
    });
    element.addEventListener('focusout', event => { if (this.active === tile && !element.contains(event.relatedTarget)) this.stop(); });
    element.addEventListener('keydown', event => { if (event.key === 'Escape') this.stop(); });
    link.addEventListener('click', () => { this.stop(); launch(link); });
    if (this.iconObserver) this.iconObserver.observe(element);
    else this.loadIcon(element);
    return tile;
  }

  details(tile) {
    if(tile.track)return;
    const {game,element,link}=tile;
    const window_ = document.createElement('span');
    window_.className = 'preview-window';
    window_.setAttribute('aria-hidden', 'true');
    const track = document.createElement('span'); track.className = 'preview-track';
    const overlay = document.createElement('span'); overlay.className = 'preview-overlay';
    const copy = document.createElement('span'); copy.className = 'preview-copy';
    const name = document.createElement('span'); name.className = 'game-name'; name.textContent = game.name;
    name.setAttribute('aria-hidden', 'true');
    const stats = document.createElement('span'); stats.className = 'game-facts'; stats.textContent = facts(game);
    stats.setAttribute('aria-hidden', 'true');
    stats.title = [game.academicYearSource?.detail || game.yearSource?.detail, game.downloadBytes ? 'Approximate first-load download, including the game and player. Previews excluded; caching may reduce it.' : ''].filter(Boolean).join(' ');
    const meta = document.createElement('span'); meta.className = 'game-meta'; meta.append(stats);
    if (game.repository) {
      const repo = document.createElement('a'); repo.className = 'game-repo';
      repo.href = game.repository; repo.target = '_blank'; repo.rel = 'noopener noreferrer';
      repo.setAttribute('aria-label', 'Repository for ' + game.name);
      repo.innerHTML = repositoryIcon(game.repository);
      repo.title = 'Repository for ' + game.name;
      overlay.append(repo);
    }
    copy.append(name, meta); overlay.append(copy);
    const play = document.createElement('span'); play.className = 'preview-play';
    play.innerHTML = '<svg viewBox="0 0 24 24"><path d="m8 4 12 8-12 8z"/></svg>';
    const steps = document.createElement('span'); steps.className = 'preview-steps';
    const sources = [...new Set(game.screenshots?.length ? game.screenshots : [game.preview || game.icon].filter(Boolean))];
    const slides = sources.map(() => null);
    const bars = sources.map(() => {
      const bar = document.createElement('i'); steps.append(bar); return bar;
    });
    steps.hidden = sources.length < 2;
    window_.append(track, steps, play);
    link.append(window_); element.append(overlay);
    Object.assign(tile,{track,slides,sources,bars});
  }

  async image(tile, index, priority = 'high') {
    const slides = tile.slides;
    if (slides[index]) return slides[index];
    const image = new Image();
    image.className = 'preview-frame'; image.alt = ''; image.decoding = 'async'; image.fetchPriority = priority;
    image.src = tile.sources[index];
    slides[index] = image.decode().then(() => tile.slides === slides ? image : null, () => null);
    return slides[index];
  }

  hasFocusedAction() {
    return !!this.active && document.activeElement !== this.active.link && this.active.element.contains(document.activeElement);
  }

  position(tile) {
    const rect = tile.element.getBoundingClientRect(), edge = 8;
    const rail = tile.element.closest('.game-rail');
    const zone = rail ? rail.getBoundingClientRect() : {left:edge,top:edge,width:innerWidth-edge*2,height:innerHeight-edge*2};
    const style = getComputedStyle(tile.element);
    const ordinary = parseFloat(style.getPropertyValue('--unit')) * 2 + parseFloat(style.getPropertyValue('--gap'));
    const size = !rail && !tile.element.classList.contains('featured') && Number.isFinite(ordinary) ? ordinary : rect.width;
    const height = Math.min(zone.height, rail ? Math.max(size * 1.344,228) : size);
    const width = Math.min(zone.width, rail ? Math.max(size * 2.035,286) : height * 2.035 / 1.344);
    const left = Math.max(zone.left, Math.min(zone.left+zone.width-width, rect.x+rect.width/2-width/2));
    const top = Math.max(zone.top, Math.min(zone.top+zone.height-height, rect.y+rect.height/2-height/2));
    tile.element.style.setProperty('--preview-width', `${width}px`);
    tile.element.style.setProperty('--preview-height', `${height}px`);
    tile.element.style.setProperty('--preview-left', `${left - rect.x}px`);
    tile.element.style.setProperty('--preview-top', `${top - rect.y}px`);
    return rect;
  }

  async start(tile) {
    if (this.active === tile) return;
    this.stop();
    // Filtering may still be moving this card. Anchor the preview to its final
    // position so keyboard focus cannot leave it offset after the move ends.
    tile.motion?.cancel();
    this.active = tile;
    this.details(tile);
    this.loadIcon(tile.element);
    clearTimeout(tile.closingTimer);
    tile.element.classList.remove('preview-closing');
    const token = ++tile.token;
    this.position(tile);
    tile.element.classList.add('previewing');
    tile.index = 0;
    const thumbnail = this.thumbnails.get(tile.game.screenshotThumbnail);
    const placeholder = (thumbnail || tile.link.querySelector('.game-icon')).cloneNode();
    placeholder.className = 'preview-frame preview-placeholder';
    if (!thumbnail && tile.game.screenshotPlaceholder) {
      placeholder.removeAttribute('srcset');
      placeholder.src = tile.game.screenshotPlaceholder;
    }
    tile.track.replaceChildren(placeholder);
    tile.element.classList.add('preview-ready');
    this.thumbnails.load(tile.game.screenshotThumbnail).then(image => {
      if (image && this.active === tile && tile.token === token && placeholder.isConnected) placeholder.src = image.src;
    });
    const image = await this.image(tile, 0);
    if (this.active !== tile || tile.token !== token) return;
    if (image) tile.track.replaceChildren(image);
    else {
      const fallback = tile.link.querySelector('.game-icon').cloneNode();
      fallback.className = 'preview-frame';
      tile.track.replaceChildren(fallback);
    }
    tile.element.classList.add('preview-ready');
    this.progress(tile);
    this.schedule(tile);
  }

  progress(tile) {
    tile.bars.forEach((bar, index) => {
      bar.className = index === tile.index ? 'current' : index < tile.index ? 'seen' : '';
      bar.style.setProperty('--duration', `${interval}ms`);
      // Re-entering can reuse the current bar after its animation has finished.
      if (index === tile.index) for (const animation of bar.getAnimations({subtree:true})) animation.currentTime = 0;
    });
  }

  schedule(tile) {
    clearTimeout(this.timer);
    if (motion.matches || tile.sources.length < 2) return;
    // Warm only the next frame of the open gallery, never every game's gallery.
    this.image(tile, (tile.index + 1) % tile.sources.length, 'low');
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
    tile.closingTimer = setTimeout(() => {
      tile.element.classList.remove('preview-closing');
      tile.link.querySelector('.preview-window')?.remove();
      tile.element.querySelector('.preview-overlay')?.remove();
      tile.track = tile.slides = tile.sources = tile.bars = null;
      tile.closingTimer = null;
    }, 340);
    this.active = null;
  }
}
