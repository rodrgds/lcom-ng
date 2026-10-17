# Arcade gameplay galleries

These are real browser gameplay canvas captures from the Arcade games. The
original game artwork is unchanged. Each gallery has three distinct frames,
except Memory Quest and LabJack, which publish two useful states and document
why redundant candidates were omitted in `../gallery-captures.json`.

## Capture and review

Use the existing collection browser export. Chrome and the project's Playwright
and Pillow dependencies are required. Run from the repository root:

```sh
devenv shell -- python3 scripts/capture_arcade_galleries.py
# Capture a selected game, preserving other galleries:
devenv shell -- python3 scripts/capture_arcade_galleries.py --game Ninjix
# Validate receipts, images, embedded origins, duplicates and catalog coverage:
devenv shell -- python3 scripts/capture_arcade_galleries.py --check
# Create compact contact sheets for visual inspection:
devenv shell -- python3 scripts/capture_arcade_galleries.py --check --contact-sheets build/arcade-gallery-review
```

`--dist`, `--plan` and `--output` select an alternative existing export, capture
plan or collection directory. The Python CLI invokes its sibling JavaScript
runner. The runner serves the existing export locally, injects normal SDK guest
key and mouse events, and snapshots the actual canvas. It never changes game
state, random generators or framebuffer pixels. Wall-clock timing and the games'
random behavior can change exact images between runs; the authored inputs and
observable objectives are repeatable, rather than byte-identical game sessions.

The plan's `publish` list selects the reviewed screenshot slugs. Its `origin`
and optional `originalReplaySha256` record earlier replay inputs adapted for the
browser. Capture actions carry labels, not fabricated output. Keyboard movement,
mouse clicks and drags, and waits produce the captured states. The Snake Garden
observer follows actual rendered food using normal keys. The Teclas observer
reads the original bitmap font masks, recognizes the visible word and types it
through normal keys. Get Lost reads the visible ball, walls and randomized stars,
then follows an open passage using arrow keys. PandaType reads the original
bitmap glyphs before typing the visible words. Golf turns from the rendered
ball position. Gold Miner steers its visible claw to revealed gold and captures the scored
return. Green Circuit follows visible food and walls with normal arrow keys.
Sketch and Guess reads its original bitmap prompt before typing the recall.
These observers fail if their expected pixels or gameplay progress are missing.
LCOMaze uses normal movement through its original deterministic maze; three
explicit crops frame different traversed rooms so its small visible area is
legible. Crop rectangles and the hashes of the uncropped canvases are recorded.

## Files and provenance

- `../gallery.json` is the launcher's minimal versioned game-ID-to-path index.
- `../gallery-captures.json` owns the input sequences and publication selection.
- `../gallery-evidence.json` records captions, original canvas SHA-256, capture
  replay SHA-256, exported game module hashes, framebuffer size, crop and final
  WebP SHA-256 for every published image.
- Each WebP also embeds its actual origin, original canvas hash, replay reference
  and hash, module hashes, framebuffer size and crop as JSON in EXIF
  `ImageDescription` (tag 270). Metadata insertion preserves compressed image
  payloads and decoded pixels.

The images are at most 640 × 480 and 150 KB each. Validation rejects changed
receipts, stale replay inputs, missing embedded origins, catalog gaps and repeated
decoded image pixels within a gallery. These short captures demonstrate visible
play states, not completion of every game campaign.

The Impeccable helper's WebP writer uses a `.webp.json` sidecar fallback and its
scanner does not recognize embedded WebP EXIF. Therefore its scan reports these
files as missing a prompt. These sourced captures have no AI prompt; the embedded
origin is checked directly by the capture CLI instead. No sidecar fallback is
required to recover their provenance.
