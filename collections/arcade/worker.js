// Modules and data are stored gzip-compressed to fit the host's asset limit. Mark their
// bytes as already encoded so Workers does not compress them a second time.
const SHARE_PATH = '/share/';
const SHARE_SLUG = /^[a-z][a-z0-9]*(?:-[a-z0-9]+)*$/;

const escapeAttribute = value => String(value)
  .replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;').replace(/"/g, '&quot;');

// Shared links travel far from the site, so the shell carries the game's own
// title, description and artwork to whoever opens or previews it.
async function shareShell(env, slug, shell, url) {
  let catalog;
  try {
    const response = await env.ASSETS.fetch(new Request(new URL('/collection.json', url)));
    if (!response.ok) return shell;
    catalog = await response.json();
  } catch { return shell; }
  const game = (catalog.games || []).find(item => item.slug === slug);
  if (!game) return shell;
  const canonical = new URL(SHARE_PATH + slug, url).href;
  const authors = Array.isArray(game.authors) && game.authors.length
    ? game.authors.map(author => typeof author === 'string' ? author : author?.name)
      .filter(Boolean).join(', ')
    : null;
  const description = [game.description, authors && `by ${authors}`].filter(Boolean).join(' · ')
    || `${catalog.name} game`;
  const image = game.screenshotThumbnail || game.iconThumbnail || game.icon;
  const tags = [
    ['name', 'description', description],
    ['property', 'og:type', 'website'],
    ['property', 'og:site_name', catalog.name],
    ['property', 'og:title', `Play ${game.name} · ${catalog.name}`],
    ['property', 'og:description', description],
    ['property', 'og:url', canonical],
    ...(image ? [['property', 'og:image', new URL(image, new URL(url).origin).href]] : []),
    ['name', 'twitter:card', image ? 'summary_large_image' : 'summary'],
  ];
  const metadata = `<title>${escapeAttribute(`Play ${game.name} · ${catalog.name}`)}</title>`
    + tags.map(([key, name, content]) => `<meta ${key}="${name}" content="${escapeAttribute(content)}">`).join('')
    + `<link rel="canonical" href="${escapeAttribute(canonical)}">`;
  return shell.replace(/<title>[^<]*<\/title>/, metadata);
}

export default {
  async fetch(request, env) {
    const pathname = new URL(request.url).pathname;
    if (pathname.startsWith('/android/')) {
      const key = pathname.slice('/android/'.length);
      if (!key || key.includes('..')) return new Response('Not found', {status: 404});
      const object = await env.ANDROID.get(key);
      if (!object) return new Response('Not found', {status: 404});
      const headers = new Headers();
      object.writeHttpMetadata(headers);
      headers.set('ETag', object.httpEtag);
      const cacheControl = key === 'catalog.json' ? 'public, max-age=60'
        : key.endsWith('.apk') || key.endsWith('.apk.sha256') ? 'public, max-age=300'
        : 'public, max-age=31536000, immutable';
      headers.set('Cache-Control', cacheControl);
      headers.set('Access-Control-Allow-Origin', '*');
      return new Response(object.body, {headers});
    }
    if (pathname.startsWith(SHARE_PATH)) {
      const slug = pathname.slice(SHARE_PATH.length).replace(/\/+$/, '');
      if (!SHARE_SLUG.test(slug)) return new Response('Not found', {status: 404});
      const shell = await env.ASSETS.fetch(new Request(new URL('/', request.url)));
      if (!shell.ok) return shell;
      // The shared address is one level below the launcher, so its relative
      // game, catalog and asset URLs keep resolving from the site root.
      const html = (await shareShell(env, slug, await shell.text(), request.url))
        .replace(/<head>/i, '<head><base href="/">');
      const headers = new Headers(shell.headers);
      headers.set('Content-Type', 'text/html; charset=utf-8');
      // Short enough to pick up a corrected slug or catalog edit promptly.
      headers.set('Cache-Control', 'public, max-age=60');
      return new Response(html, {headers});
    }
    const contentType = pathname.endsWith('/game.wasm') ? 'application/wasm' :
      pathname.endsWith('/game.data') ? 'application/octet-stream' : null;
    if (!contentType) return env.ASSETS.fetch(request);
    const assetRequest = new Request(request);
    assetRequest.headers.delete('Range');
    assetRequest.headers.delete('If-Range');
    const response = await env.ASSETS.fetch(assetRequest);
    if (![200, 304].includes(response.status)) return response;
    const acceptEncoding = request.cf?.clientAcceptEncoding ?? request.headers.get('Accept-Encoding') ?? '';
    const encodings = acceptEncoding.split(',').map(part => {
      const [encoding, ...parameters] = part.trim().toLowerCase().split(';');
      const quality = parameters.find(value => value.trim().startsWith('q='));
      return {encoding, quality: quality ? Number(quality.trim().slice(2)) : 1};
    });
    const gzip = (encodings.find(value => value.encoding === 'gzip') ??
      encodings.find(value => value.encoding === '*'))?.quality > 0;
    const headers = new Headers(response.headers);
    headers.set('Content-Type', contentType);
    headers.set('Vary', 'Accept-Encoding');
    headers.set('Cache-Control', 'public, max-age=0, must-revalidate, no-transform');
    const etag = headers.get('ETag');
    if (etag && !etag.startsWith('W/')) headers.set('ETag', 'W/' + etag);
    headers.delete('Content-Length');
    if (gzip) headers.set('Content-Encoding', 'gzip');
    let body = response.body;
    if (!gzip && body) body = body.pipeThrough(new DecompressionStream('gzip'));
    return new Response(body, {
      status: response.status,
      headers,
      encodeBody: 'manual',
    });
  },
};
