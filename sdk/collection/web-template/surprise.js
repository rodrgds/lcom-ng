import {cue, unlockSound} from './audio.js';

const duration = 5200;
const reducedMotion = matchMedia('(prefers-reduced-motion: reduce)');

// A short acceleration, a fast run, then a long, deliberate settling phase.
function travel(time) {
  const accelerate = .12, cruise = .30;
  const total = accelerate / 2 + cruise - accelerate + (1 - cruise) / 4;
  if (time < accelerate) return time * time / (2 * accelerate * total);
  if (time < cruise) return (accelerate / 2 + time - accelerate) / total;
  const tail = (time - cruise) / (1 - cruise);
  return (accelerate / 2 + cruise - accelerate + (1 - cruise) * (1 - (1 - tail) ** 4) / 4) / total;
}

export class Surprise {
  constructor(catalog, launch) {
    this.catalog = catalog;
    this.launch = launch;
    this.opener = document.querySelector('#surprise');
    this.dialog = document.querySelector('#surprise-panel');
    this.viewport = document.querySelector('#surprise-reel');
    this.track = this.viewport.querySelector('.roulette-track');
    this.result = document.querySelector('#surprise-result');
    this.play = document.querySelector('#surprise-play');
    this.again = document.querySelector('#surprise-again');
    this.skip = document.querySelector('#surprise-skip');
    this.frame = null;
    this.opener.disabled = !catalog.length;
    this.opener.onclick = () => {
      this.dialog.showModal();
      unlockSound();
      this.spin();
    };
    document.querySelector('#surprise-close').onclick = () => this.close();
    this.dialog.addEventListener('click', event => {
      if (event.target !== this.dialog) return;
      const r = this.dialog.getBoundingClientRect();
      if (event.clientX < r.left || event.clientX > r.right || event.clientY < r.top || event.clientY > r.bottom) this.close();
    });
    this.dialog.addEventListener('cancel', () => this.stop());
    this.dialog.addEventListener('close', () => {
      this.stop();
      if (!document.querySelector('#library').hidden) this.opener.focus({preventScroll:true});
    });
    this.skip.onclick = () => this.reveal();
    this.again.onclick = () => this.spin();
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
      this.measure();
      this.position(this.fraction);
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
    this.winnerIndex = 34 + Math.floor(Math.random() * 5);
    const deck = [];
    for (let index = 0; index < this.winnerIndex + 5; index++) {
      let game = this.catalog[Math.floor(Math.random() * this.catalog.length)];
      if (index === this.winnerIndex) game = this.winner;
      const card = document.createElement('div');
      card.className = 'roulette-card';
      const image = new Image();
      image.src = game.icon || 'assets/arcade.svg';
      image.alt = '';image.width = 160;image.height = 160;
      image.onerror = () => {image.onerror = null;image.src = 'assets/arcade.svg';};
      card.append(image);
      if (index === this.winnerIndex) card.dataset.selected = 'true';
      deck.push(card);
    }
    this.track.replaceChildren(...deck);
    this.viewport.classList.remove('landed');
    this.result.textContent = 'Finding your next game…';
    this.play.hidden = this.again.hidden = true;
    this.skip.hidden = false;
    this.measure();this.position(0);
    if (reducedMotion.matches) { this.reveal();return; }
    this.skip.focus({preventScroll:true});
    this.track.classList.add('rolling');
    this.started = performance.now();this.lastTick = -Infinity;this.lastCard = 2;
    cue('tap',{emphasis:'subtle',theme:'mech'});
    const animate = now => {
      const progress = Math.min(1,(now - this.started) / duration);
      this.position(travel(progress));
      const card = Math.round(2 + (this.winnerIndex - 2) * this.fraction);
      if (card !== this.lastCard && now - this.lastTick >= 125) {
        cue('select',{emphasis:'subtle',theme:'mech',volume:.45});
        this.lastTick = now;
      }
      this.lastCard = card;
      if (progress < 1) this.frame = requestAnimationFrame(animate);
      else this.reveal();
    };
    this.frame = requestAnimationFrame(animate);
  }

  measure() {
    const width = this.track.firstElementChild.getBoundingClientRect().width;
    const step = width + parseFloat(getComputedStyle(this.track).columnGap);
    const center = (this.viewport.clientWidth - width) / 2;
    this.start = center - 2 * step;
    this.end = center - this.winnerIndex * step;
  }

  position(fraction) {
    this.fraction = fraction;
    this.track.style.transform = `translate3d(${this.start + (this.end - this.start) * fraction}px,0,0)`;
  }

  reveal(sound = true) {
    this.stop();this.measure();this.position(1);
    this.viewport.classList.add('landed');
    this.result.textContent = this.winner.name;
    this.skip.hidden = true;
    this.play.hidden = this.again.hidden = false;
    this.play.setAttribute('aria-label','Play ' + this.winner.name);
    this.play.focus({preventScroll:true});
    if (sound) cue('ready',{theme:'bubble',emphasis:'normal'});
  }
}
