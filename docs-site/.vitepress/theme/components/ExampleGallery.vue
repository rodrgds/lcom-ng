<script setup lang="ts">
import { computed, ref } from 'vue';
import captures from '../../../../docs/assets/screenshots/gallery/index.json';

const props = defineProps<{ app: string }>();
const frames = computed(() => Object.entries(captures)
  .filter(([key]) => key.startsWith(props.app + '/'))
  .map(([key, value]) => ({ key, ...value }))
  .sort((left, right) => left.order - right.order));
const dialog = ref<HTMLDialogElement>();
const active = ref(0);
const current = computed(() => frames.value[active.value]);
function open(index: number, event: MouseEvent) {
  if (event.metaKey || event.ctrlKey || event.shiftKey || event.altKey) return;
  event.preventDefault();
  active.value = index;
  dialog.value?.showModal();
}
function move(delta: number) {
  active.value = (active.value + delta + frames.value.length) % frames.value.length;
}
</script>

<template>
  <div class="example-gallery">
    <figure v-for="(frame, index) in frames" :key="frame.key">
      <a :href="frame.src" @click="open(index, $event)" :aria-label="'Enlarge: ' + frame.title">
        <img :src="frame.src" :alt="frame.caption" :width="frame.width" :height="frame.height" loading="lazy">
      </a>
      <figcaption><strong>{{ frame.title }}</strong><span>{{ frame.caption }}</span></figcaption>
    </figure>
  </div>
  <dialog ref="dialog" class="example-lightbox" aria-label="Example screenshots"
    @keydown.left.prevent="move(-1)" @keydown.right.prevent="move(1)"
    @click="($event.target === dialog) && dialog?.close()">
    <template v-if="current">
      <div class="gallery-controls">
        <button type="button" @click="move(-1)" aria-label="Previous screenshot">← Previous</button>
        <span>{{ active + 1 }} / {{ frames.length }}</span>
        <button type="button" @click="move(1)" aria-label="Next screenshot">Next →</button>
        <button type="button" @click="dialog?.close()" autofocus>Close</button>
      </div>
      <img :src="current.src" :alt="current.caption" :width="current.width" :height="current.height">
      <p aria-live="polite"><strong>{{ current.title }}</strong> · {{ current.caption }}</p>
    </template>
  </dialog>
</template>

<style scoped>
.example-gallery { display: grid; grid-template-columns: repeat(2, minmax(0, 1fr)); gap: 24px 16px; margin: 24px 0; }
.example-gallery figure { margin: 0; min-width: 0; }
.example-gallery a { display: block; border-radius: 8px; overflow: hidden; border: 1px solid var(--vp-c-divider); }
.example-gallery a:focus-visible, button:focus-visible { outline: 3px solid var(--vp-c-brand-1); outline-offset: 4px; }
.example-gallery img { display: block; width: 100%; height: auto; }
figcaption { font-size: 13px; line-height: 1.5; padding-top: 10px; }
figcaption strong, figcaption span { display: block; }
figcaption span { color: var(--vp-c-text-2); margin-top: 3px; }
.example-lightbox { padding: 20px; border-radius: 12px; border: 1px solid var(--vp-c-divider); background: var(--vp-c-bg); color: var(--vp-c-text-1); max-width: min(1100px, 96vw); max-height: 94vh; overflow: auto; }
.example-lightbox::backdrop { background: #000b; }
.example-lightbox img { display: block; width: auto; max-width: 100%; height: auto; max-height: 70vh; margin: auto; object-fit: contain; }
.example-lightbox p { max-width: 80ch; font-size: 14px; }
.gallery-controls { display: flex; align-items: center; flex-wrap: wrap; gap: 12px; margin-bottom: 16px; }
.gallery-controls span { margin-inline: auto; font-variant-numeric: tabular-nums; }
.gallery-controls button { padding: 6px 12px; border-radius: 6px; border: 1px solid var(--vp-c-divider); cursor: pointer; }
.gallery-controls button:hover { background: var(--vp-c-bg-soft); }
@media (max-width: 640px) { .example-gallery { grid-template-columns: 1fr; } }
</style>
