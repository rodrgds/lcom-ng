import {facts} from './previews.js';
import {bindGridNavigation} from './navigation.js';
import {cue} from './audio.js';

export class GameSearch {
  constructor(catalog, launch) {
    this.catalog = catalog;
    this.launch = launch;
    this.dialog = document.querySelector('#search-panel');
    this.input = document.querySelector('#search');
    this.results = document.querySelector('#search-results');
    this.toggle = document.querySelector('#search-toggle');
    this.toggle.onclick = () => this.open();
    this.dialog.querySelector('form').onsubmit = event => {
      event.preventDefault(); this.results.querySelector('a')?.click();
    };
    this.dialog.addEventListener('click', event => {
      if (event.target !== this.dialog) return;
      const rect = this.dialog.getBoundingClientRect();
      if (event.clientX < rect.left || event.clientX > rect.right || event.clientY < rect.top || event.clientY > rect.bottom) this.dismiss();
    });
    this.dialog.addEventListener('cancel', event => {
      event.preventDefault(); this.dismiss();
    });
    this.dialog.addEventListener('keydown', event => {
      // A search input otherwise consumes its first Escape to clear the query.
      if (event.key === 'Escape') { event.preventDefault(); this.dismiss(); }
    });
    this.dialog.addEventListener('close', () => {
      this.toggle.setAttribute('aria-expanded', 'false');
      if (!document.querySelector('#library').hidden) this.opener?.focus({preventScroll: true});
    });
    this.input.oninput = () => this.render();
    this.input.onkeydown = event => {
      if (event.key === 'ArrowDown' && !event.ctrlKey && !event.metaKey && !event.altKey && !event.shiftKey) {
        event.preventDefault(); this.results.querySelector('a')?.focus();
      }
    };
    bindGridNavigation(this.results, 'a', {onUpBoundary: () => this.input.focus()});
    document.addEventListener('keydown', event => {
      if (event.defaultPrevented || event.altKey || document.querySelector('#library').hidden ||
          [...document.querySelectorAll('dialog[open]')].some(dialog => dialog !== this.dialog)) return;
      const command = event.key.toLowerCase() === 'k' && (event.ctrlKey || event.metaKey);
      const slash = event.key === '/' && !event.ctrlKey && !event.metaKey &&
        !event.target.closest('input,textarea,select,[contenteditable="true"]');
      if (!command && !slash) return;
      event.preventDefault();
      if (this.dialog.open) this.input.focus();
      else this.open();
    });
  }

  dismiss() {
    if (!this.dialog.open) return;
    cue('close', {emphasis:'subtle'});
    this.dialog.close();
  }

  open(restoreId = null) {
    if (this.dialog.open) return;
    this.opener = restoreId || document.activeElement === document.body ? this.toggle : document.activeElement;
    this.render(); this.dialog.showModal();
    if (restoreId === null) cue('open', {emphasis:'subtle'});
    this.toggle.setAttribute('aria-expanded', 'true');
    const link = [...this.results.querySelectorAll('a')].find(link => link.dataset.id === restoreId);
    (link || this.input).focus();
  }

  render() {
    const query = this.input.value.trim().toLocaleLowerCase();
    const matches = this.catalog.filter(game => game.name.toLocaleLowerCase().includes(query));
    this.results.replaceChildren(...matches.map(game => {
      const item = document.createElement('div'); item.setAttribute('role', 'listitem');
      const link = document.createElement('a'); link.href = '#game=' + encodeURIComponent(game.id); link.dataset.id = game.id;
      const icon = document.createElement('img'); icon.src = game.icon || 'assets/arcade.svg'; icon.alt = ''; icon.width = 52; icon.height = 52; icon.loading = 'lazy';
      icon.onerror = () => { icon.onerror = null; icon.src = 'assets/arcade.svg'; };
      const copy = document.createElement('span'), name = document.createElement('strong'), meta = document.createElement('span');
      name.textContent = game.name; meta.textContent = facts(game); copy.append(name, meta);
      link.append(icon, copy); item.append(link);
      link.onclick = () => { this.dialog.close(); this.launch(link, game); };
      return item;
    }));
    document.querySelector('#search-count').textContent = query ? `${matches.length} of ${this.catalog.length} games` : `${this.catalog.length} games`;
    document.querySelector('#empty').hidden = matches.length > 0;
  }
}
