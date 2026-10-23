import {cue, unlockSound} from './audio.js';
import {facts, repositoryIcon} from './previews.js';
import {renderGameTags} from './tags.js';
import {track} from './analytics.js';

const duration = 8800;
const reducedMotion = matchMedia('(prefers-reduced-motion: reduce)');

// Integrate a brief acceleration, a fast run and a long quadratic slowdown.
// Position and velocity meet at each boundary, so the reel never changes gear.
function travel(time) {
  const accelerate = .08, cruise = .22;
  const total = accelerate / 2 + cruise - accelerate + (1 - cruise) / 3;
  if (time < accelerate) return time * time / (2 * accelerate * total);
  if (time < cruise) return (accelerate / 2 + time - accelerate) / total;
  const tail = (time - cruise) / (1 - cruise);
  return (accelerate / 2 + cruise - accelerate + (1 - cruise) * (1 - (1 - tail) ** 3) / 3) / total;
}

export class Surprise {
  constructor(catalog, launch, taxonomy = []) {
    this.catalog = catalog;
    this.taxonomy = taxonomy;
    this.launch = launch;
    this.opener = document.querySelector('#surprise');
    this.dialog = document.querySelector('#surprise-panel');
    this.viewport = document.querySelector('#surprise-reel');
    this.track = this.viewport.querySelector('.roulette-track');
    this.detail = document.querySelector('#surprise-detail');
    this.status = document.querySelector('#surprise-status');
    this.play = document.querySelector('#surprise-play');
    this.frame = null;
    this.opener.disabled = !catalog.length;
    this.opener.onclick = () => {
      this.dialog.classList.remove('revealed');
      this.detail.inert = true;
      this.play.hidden = true;
      this.dialog.showModal();
      unlockSound();
      this.spin();
    };
    this.dialog.addEventListener('click', event => {
      if (event.target !== this.dialog) return;
      const r = this.dialog.getBoundingClientRect();
      if (event.clientX < r.left || event.clientX > r.right || event.clientY < r.top || event.clientY > r.bottom) this.close();
    });
    this.dialog.addEventListener('cancel', () => this.stop());
    this.dialog.addEventListener('close', () => {
      // Native close events can arrive after a rapid reopen.
      if (this.dialog.open) return;
      this.stop();
      if (!document.querySelector('#library').hidden) this.opener.focus({preventScroll:true});
    });
    this.play.onclick = () => {
      if (this.frame !== null || !this.winner) return;
      const game = this.winner;
      this.dialog.close();
      this.launch(game);
    };
    document.addEventListener('visibilitychange', () => {
      if (document.hidden && this.frame !== null) this.reveal(false);
    });
    reducedMotion.addEventListener('change', () => {
      if (reducedMotion.matches && this.frame !== null) this.reveal(false);
    });
    new ResizeObserver(() => {
      if (!this.dialog.open || !this.track.children.length) return;
      this.measure();this.position(this.fraction);
      this.lastCard = this.cardAtMarker();
    }).observe(this.viewport);
  }

  stop() {
    if (this.frame !== null) cancelAnimationFrame(this.frame);
    this.frame = null;
    this.track.classList.remove('rolling');
  }

  close() {
    this.stop();
    if (this.dialog.open) this.dialog.close();
  }

  spin() {
    this.stop();
    this.winner = this.catalog[Math.floor(Math.random() * this.catalog.length)];
    track('surprise_spun',{results:this.catalog.length,game:this.winner.name});
    this.winnerIndex = 44 + Math.floor(Math.random() * 9);
    // Keep a small inset from both edges. The marker always belongs to exactly
    // one game, including after resizing, rather than landing in a gap.
    this.landing = .08 + Math.random() * .84;
    const deck = [];
    for (let index = 0; index < this.winnerIndex + 6; index++) {
      const game = index === this.winnerIndex ? this.winner : this.catalog[Math.floor(Math.random() * this.catalog.length)];
      const card = document.createElement('div');
      card.className = 'roulette-card';
      const image = new Image();
      image.src = game.iconSources?.find(source=>source.width>=256)?.path || game.icon || 'assets/arcade.svg';
      image.alt = '';image.width = 160;image.height = 160;
      image.onerror = () => {image.onerror = null;image.src = 'assets/arcade.svg';};
      card.append(image);
      if (index === this.winnerIndex) card.dataset.selected = 'true';
      deck.push(card);
    }
    this.track.replaceChildren(...deck);
    this.status.textContent = 'Choosing a game…';
    this.prepareResult();
    this.measure();this.position(0);
    if (reducedMotion.matches) { this.reveal();return; }
    this.track.classList.add('rolling');
    this.started = performance.now();this.lastCard = this.cardAtMarker();
    cue('tap',{emphasis:'subtle',theme:'mech'});
    const animate = now => {
      const progress = Math.min(1,(now - this.started) / duration);
      this.position(travel(progress));
      const card = this.cardAtMarker();
      // Crossings, rather than a time throttle, own the ticks. A dropped frame
      // still accounts for each passed card, with no timers left after close.
      while (this.lastCard < card) {
        cue('select',{emphasis:'subtle',theme:'mech',volume:.65});
        this.lastCard++;
      }
      // Let the stopped marker register before the game artwork opens.
      if (now - this.started < duration + 420) this.frame = requestAnimationFrame(animate);
      else this.reveal();
    };
    this.frame = requestAnimationFrame(animate);
  }

  prepareResult() {
    const game = this.winner;
    document.querySelector('#surprise-result').textContent = game.name;
    document.querySelector('#surprise-facts').textContent = facts(game);
    renderGameTags(document.querySelector('#surprise-tags'),game,this.taxonomy);
    const description = document.querySelector('#surprise-description');
    description.textContent = game.description || '';
    description.hidden = !game.description;
    const repo = document.querySelector('#surprise-repo');
    repo.hidden = !game.repository;
    if (game.repository) {
      repo.href = game.repository;
      repo.innerHTML = repositoryIcon(game.repository);
      repo.title = 'Repository for ' + game.name;
      repo.setAttribute('aria-label','Repository for ' + game.name);
    } else repo.removeAttribute('href');
    for (const [selector,source] of [
      ['.surprise-scene',game.screenshots?.[Math.floor(game.screenshots.length / 2)] || game.preview || game.icon],
      ['.surprise-icon',game.iconThumbnail || game.icon],
    ]) {
      const image = this.detail.querySelector(selector);
      image.onerror = () => {
        image.onerror = () => {image.onerror = null;image.src = 'assets/arcade.svg';};
        image.src = selector === '.surprise-scene' && game.icon ? game.icon : 'assets/arcade.svg';
      };
      image.src = source || 'assets/arcade.svg';
    }
    this.play.setAttribute('aria-label','Play ' + game.name);
  }

  measure() {
    // The reel scales away during the reveal. Measure its layout width so
    // visual transforms cannot shift the selected game during expansion.
    this.cardWidth = parseFloat(getComputedStyle(this.track.firstElementChild).width);
    this.gap = parseFloat(getComputedStyle(this.track).columnGap);
    this.step = this.cardWidth + this.gap;
    this.marker = this.viewport.clientWidth / 2;
    this.start = this.marker - this.cardWidth / 2 - 3 * this.step;
    this.end = this.marker - this.winnerIndex * this.step - this.cardWidth * this.landing;
  }

  position(fraction) {
    this.fraction = fraction;
    this.offset = this.start + (this.end - this.start) * fraction;
    this.track.style.transform = `translate3d(${this.offset}px,0,0)`;
  }

  cardAtMarker() {
    return Math.floor((this.marker - this.offset + this.gap / 2) / this.step);
  }

  reveal(sound = true) {
    this.stop();this.dialog.classList.add('revealed');
    this.measure();this.position(1);
    this.detail.inert = false;
    this.status.textContent = this.winner.name;
    this.play.hidden = false;
    this.play.focus({preventScroll:true});
    if (sound) cue('ready',{theme:'bubble',emphasis:'normal'});
  }
}
