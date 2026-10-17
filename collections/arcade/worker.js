// Modules and data are stored gzip-compressed to fit the host's asset limit. Mark their
// bytes as already encoded so Workers does not compress them a second time.
export default {
  async fetch(request, env) {
    const pathname = new URL(request.url).pathname;
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
