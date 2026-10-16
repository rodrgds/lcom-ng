import {facts} from './previews.js';

export class GameSearch {
  constructor(catalog, launch) {
    this.catalog = catalog;
    this.launch = launch;
    this.dialog = document.querySelector('#search-panel');
    this.input = document.querySelector('#search');
    this.results = document.querySelector('#search-results');
    this.toggle = document.querySelector('#search-toggle');
    this.toggle.onclick = () => this.open();
    document.querySelector('#search-close').onclick = () => this.dialog.close();
    this.dialog.querySelector('form').onsubmit = event => {
      event.preventDefault(); this.results.querySelector('a')?.click();
    };
    this.dialog.addEventListener('click', event => {
      if (event.target !== this.dialog) return;
      const rect = this.dialog.getBoundingClientRect();
      if (event.clientX < rect.left || event.clientX > rect.right || event.clientY < rect.top || event.clientY > rect.bottom) this.dialog.close();
    });
    this.dialog.addEventListener('close', () => {
      this.toggle.setAttribute('aria-expanded', 'false');
      if (!document.querySelector('#library').hidden) this.opener?.focus({preventScroll: true});
    });
    this.input.oninput = () => this.render();
    this.input.onkeydown = event => {
      if (event.key === 'ArrowDown') { event.preventDefault(); this.results.querySelector('a')?.focus(); }
    };
    this.results.onkeydown = event => {
      if (!['ArrowDown', 'ArrowUp'].includes(event.key)) return;
      const links = [...this.results.querySelectorAll('a')], index = links.indexOf(document.activeElement);
      if (index < 0) return;
      event.preventDefault();
      if (event.key === 'ArrowUp' && index === 0) this.input.focus();
      else links[Math.max(0, Math.min(links.length - 1, index + (event.key === 'ArrowDown' ? 1 : -1)))].focus();
    };
    document.addEventListener('keydown', event => {
      if (event.key === '/' && !event.ctrlKey && !event.metaKey && !event.altKey && !this.dialog.open && !document.querySelector('#library').hidden) {
        event.preventDefault(); this.open();
      }
    });
  }

  open(restoreId = null) {
    if (this.dialog.open) return;
    this.opener = restoreId || document.activeElement === document.body ? this.toggle : document.activeElement;
    this.render(); this.dialog.showModal();
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
