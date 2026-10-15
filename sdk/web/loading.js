// Count decoded bytes against the export's actual file sizes. Content-Length can
// describe compressed transport bytes, so it is deliberately not the denominator.
export async function prepareGame(downloads, report) {
  const files = Object.entries(downloads || {}).filter(([name, size]) =>
    ['game.wasm', 'game.data'].includes(name) && Number.isFinite(size) && size > 0);
  if (!files.some(([name]) => name === 'game.wasm')) return {};
  const total = files.reduce((sum, [,size]) => sum + size, 0);
  let loaded = 0;
  report({phase:'download', loaded, total});
  async function response(name) {
    const result = await fetch(new URL(name, import.meta.url));
    if (!result.ok) throw new Error(`${name} could not be downloaded (${result.status})`);
    const stream = result.body.pipeThrough(new TransformStream({
      transform(chunk, controller) {
        loaded += chunk.byteLength;
        report({phase:'download', loaded, total});
        controller.enqueue(chunk);
      },
    }));
    return new Response(stream, {headers:{'Content-Type':name.endsWith('.wasm') ? 'application/wasm' : 'application/octet-stream'}});
  }
  const [compiled, assets] = await Promise.all([
    response('game.wasm').then(async result => WebAssembly.compileStreaming ?
      WebAssembly.compileStreaming(result) : WebAssembly.compile(await result.arrayBuffer())),
    files.some(([name]) => name === 'game.data') ? response('game.data').then(result => result.arrayBuffer()) : null,
  ]);
  report({phase:'starting'});
  return {compiled, assets};
}

export async function instantiateGame(create, options, prepared) {
  if (!prepared.compiled) return create(options);
  // Emscripten's callback cannot reject its module promise. Race the instance
  // failure explicitly so an invalid module offers Retry instead of hanging.
  let rejectInstance;
  const failure = new Promise((_, reject) => { rejectInstance = reject; });
  const module = create({...options,
    getPreloadedPackage: () => prepared.assets,
    instantiateWasm(imports, receive) {
      WebAssembly.instantiate(prepared.compiled, imports)
        .then(instance => receive(instance, prepared.compiled)).catch(rejectInstance);
      return {};
    },
  });
  return Promise.race([module, failure]);
}
