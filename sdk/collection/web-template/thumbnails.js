// Only tiny first screenshots are retained across previews. Full gallery images
// belong to the open card and are released by Previews after it closes.
export class Thumbnails {
  constructor() {
    this.cache = new Map();
    this.queue = new Set();
    this.running = 0;
    this.timer = null;
    this.ready = document.readyState === 'complete';
    if (!this.ready) window.addEventListener('load', () => { this.ready = true; this.schedule(); }, {once:true});
  }

  get(url) {
    const entry = this.cache.get(url);
    if (!entry) return null;
    this.cache.delete(url); this.cache.set(url, entry);
    return entry.image;
  }

  load(url, priority = 'high') {
    if (!url) return Promise.resolve(null);
    if (this.cache.has(url)) return this.cache.get(url).promise;
    const image = new Image();
    image.decoding = 'async'; image.fetchPriority = priority; image.src = url;
    const entry = {image:null};
    entry.promise = image.decode().then(() => {
      entry.image = image;
      return image;
    }, () => {
      if (this.cache.get(url) === entry) this.cache.delete(url);
      return null;
    });
    this.cache.set(url, entry);
    while (this.cache.size > 48) this.cache.delete(this.cache.keys().next().value);
    return entry.promise;
  }

  warm(url) {
    const connection = navigator.connection;
    if (!url || document.hidden || connection?.saveData || /(^|-)2g$/.test(connection?.effectiveType || '') || this.cache.has(url) || this.queue.size >= 8) return;
    this.queue.add(url);
    this.schedule();
  }

  schedule() {
    if (!this.ready || this.timer !== null || !this.queue.size) return;
    this.timer = setTimeout(() => {
      this.timer = null;
      if (document.hidden) return;
      while (this.running < 2 && this.queue.size) {
        const url = this.queue.values().next().value;
        this.queue.delete(url); this.running++;
        this.load(url, 'low').finally(() => { this.running--; this.schedule(); });
      }
    }, 80);
  }

  clear() {
    clearTimeout(this.timer); this.timer = null;
    this.queue.clear(); this.cache.clear();
  }
}
