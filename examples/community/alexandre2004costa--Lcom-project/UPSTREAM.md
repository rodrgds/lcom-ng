# Upstream provenance

Original repository: https://github.com/alexandre2004costa/Lcom-project

Pinned revision: `c7953bfe270678361ce71e9a1b28f466d35d7c40`

Original sources and game art are retained. Exact source hashes and verification records are in UPSTREAM.json.

License: No license file found upstream. Public source availability does not imply a reuse license.

## Recorded source corrections

devices/graphic.c: Original Close menu reproduces SIGABRT: freeVideoBuffers passes two vm_map_phys framebuffer mappings to free. Release each mapping through vm_unmap_phys using the existing mode dimensions.

Exact original/patched SHA256 and SOURCE_PATCH.diff are recorded in UPSTREAM.json.

## Controls

Original controller.c maps A/D movement and SPACE jump, exposed as explicit quick keys; mouse presses shoot and select menus. Mouse is processed after each packet byte with repeated deltas, so relative pointer preserves source behavior. No direct preset.
