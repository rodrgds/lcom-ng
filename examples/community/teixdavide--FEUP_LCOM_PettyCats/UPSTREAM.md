# Upstream provenance

Original repository: https://github.com/teixdavide/FEUP_LCOM_PettyCats

Pinned revision: `678372ac889677fb6241a6daa100ec7d56dcd6b0`

Original sources and game art are retained. Exact source hashes and verification records are in UPSTREAM.json.

License: No license file found upstream. Public source availability does not imply a reuse license.

## Recorded source corrections

src/main.c: Original callback definition has no parameters, conflicting with the required LCF int(argc,argv) callback signature. Compile failed at main.c:49; only signature corrected, behavior unchanged.

src/models/Room.c: Original startup SIGSEGV while no cat is within selection radius. getSelectedCat returns and dereferences an uninitialized closestCat when all candidates are distant. Initialize NULL and mark selection only when a cat was found. Original deleteRoom also frees uninitialized walls pointers; the real gameplay exit reproduces SIGSEGV under MallocScribble=1. Initialize the four unused wall slots to NULL.

Exact original/patched SHA256 and SOURCE_PATCH.diff are recorded in UPSTREAM.json.

## Controls

Original WASD moves player; E begins petting nearest cat, mouse gestures perform petting, ESC opens menu then exits. Relative pointer preserves its gesture controller.
