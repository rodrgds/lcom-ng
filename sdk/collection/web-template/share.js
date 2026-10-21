// Clean share addresses live under /share/<slug>; ordinary browsing keeps hash routing.
export function sharedSlug() {
  const match = location.pathname.match(/\/share\/([^/]+?)\/?$/);
  return match ? decodeURIComponent(match[1]) : null;
}

export function homeUrl() {
  const path = location.pathname
    .replace(/\/index\.html?$/i, '/')
    .replace(/\/share\/[^/]+\/?$/, '/');
  return path.endsWith('/') ? path : path + '/';
}

export function shareUrl(game) {
  if (!game?.slug) return location.href;
  return new URL('share/' + encodeURIComponent(game.slug), location.origin + homeUrl()).href;
}

export async function copyText(text) {
  try {
    await navigator.clipboard.writeText(text);
    return true;
  } catch {
    /* Clipboard access needs a permission grant or a secure context. */
  }
  const field = document.createElement('textarea');
  field.value = text;
  field.setAttribute('readonly', '');
  field.style.cssText = 'position:fixed;top:0;left:0;opacity:0';
  document.body.append(field);
  field.select();
  let copied = false;
  try { copied = document.execCommand('copy'); } catch { /* Unsupported in this browser. */ }
  field.remove();
  return copied;
}
