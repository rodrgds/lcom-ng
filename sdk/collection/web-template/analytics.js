// The launcher also runs without analytics in local and student exports.
export function track(name, data) {
  try { window.umami?.track?.(name, data); }
  catch { /* A blocked or broken tracker must not interrupt play. */ }
}
