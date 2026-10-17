// Search stays in the library so results and the query share one context.
export function bindSearchShortcuts(filters) {
  filters.input.addEventListener('keydown',event=>{
    if(event.key==='ArrowDown'&&!event.altKey&&!event.ctrlKey&&!event.metaKey&&!event.shiftKey){
      event.preventDefault();filters.close(false);document.querySelector('#games .game-launch')?.focus();
    }
  });
  document.addEventListener('keydown', event => {
    if (event.defaultPrevented || event.altKey || document.querySelector('#library').hidden ||
        document.querySelector('dialog[open]')) return;
    const command = event.key.toLowerCase() === 'k' && (event.ctrlKey || event.metaKey);
    const slash = event.key === '/' && !event.ctrlKey && !event.metaKey &&
      !event.target.closest('input,textarea,select,[contenteditable="true"]');
    if (!command && !slash) return;
    event.preventDefault();filters.close(false);filters.input.focus();
  });
}
