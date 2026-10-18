// Fetch the decorative font after the catalog and page load. A slow font must
// not compete with the catalog or keep the browser's loading indicator running.
export function loadFont() {
  const load = async () => {
    try {
      const response = await fetch('assets/bricolage.woff2', {priority:'low'});
      if (!response.ok) return;
      const font = new FontFace('Bricolage', await response.arrayBuffer(), {weight:'200 800'});
      await font.load();
      document.fonts.add(font);
    } catch { /* Keep the system fallback if the optional font is unavailable. */ }
  };
  if (document.readyState === 'complete') void load();
  else window.addEventListener('load', load, {once:true});
}
