# Recent LCOM compatibility corpus

This snapshot adds 23 pinned public repositories beyond the earlier corpus. They contain 20 distinct C source families. GitHub creation dates are ten in 2026, twelve in 2025 and one in 2024. Repository age is not course age: Luana explicitly identifies 2023/24, Knight describes 2022/23, Wombat identifies 2024/25 and the editor identifies 2025/26.

The default importer builds 21 repositories. One more builds unchanged with an explicit include directory. Nineteen repositories, covering 16 source families, reach a normal exit through verified paths. Eleven source families have populated gameplay captures and a separate successful game cleanup exit. Four more games have guarded gameplay captures and original exit defects. The remaining repository has an invalid callback declaration.

These are bounded headless checks on Apple Silicon macOS. A guard exit 1 is intentional capture termination, never a successful callback exit. This survey does not establish complete games, every device path, editor save, timing on student hardware or real SDL responsiveness.

Run the persistent manifest after its scripts are installed:

```sh
devenv shell -- python3 scripts/check_lcom_corpus.py \
  --manifest tests/compat-recent.json --corpus build/compat-recent --fetch
```

The manifest pins complete commits, rebuilds original Makefiles, checks source hashes, exit status, guard state, trace progress and nonblank captures. Input scripts were derived from the original controls. Populated frames were inspected to distinguish gameplay from menus, countdowns and instructions. Original code, Makefiles and XPM source hashes remain unchanged. Once LCF output logging was enabled, WombatMaze, BounceBall and Head Football wrote their declared tracked `output.txt` files. Those runtime output changes are separate from original program sources.

| Repository and pin | Project | Verified scope |
| --- | --- | --- |
| [Yuka-Sky/LCOM@e333e84a](https://github.com/Yuka-Sky/LCOM/tree/e333e84a361140f7ba410bf219133d7b2fd92860) | `Project/src` | gameplay and clean exit |
| [luanalima27/LCOM-FEUP@8a51f478](https://github.com/luanalima27/LCOM-FEUP/tree/8a51f4788f459f16e832668dd988ec4f42552fdf) | `proj/src` | menu and clean exit |
| [Eduarda34/FEUP-LCOM@0534e832](https://github.com/Eduarda34/FEUP-LCOM/tree/0534e832b7fe199763e65218688ea093881830c4) | `proj/src` | original callback contract compile error |
| [DGoiana/feup-lcom@74f0bdac](https://github.com/DGoiana/feup-lcom/tree/74f0bdacb955639ee774963aba6969e7beba3d57) | `proj/src` | menu and clean exit |
| [mariajvieira/feup-lcom@e364f4d9](https://github.com/mariajvieira/feup-lcom/tree/e364f4d91606371c5660f28bcd11048d02f8a57e) | `proj/src` | gameplay and clean exit |
| [simaopsbarbosa/feup-lcom@b2489e24](https://github.com/simaopsbarbosa/feup-lcom/tree/b2489e2420eeea858df8d9164df267a52e12e8fa) | `proj/src` | gameplay and clean exit |
| [MiguelMirandaRocha/FEUP-LEIC-LCOM@78fa8fdf](https://github.com/MiguelMirandaRocha/FEUP-LEIC-LCOM/tree/78fa8fdf30638554416dd1e7d96cc747c93c5217) | `proj/code` | editor typing and clean exit |
| [filipecruz06/FEUP-LCOM-proj@b9981f12](https://github.com/filipecruz06/FEUP-LCOM-proj/tree/b9981f12376b9dc5d2b95f066b9cacdb171ceb88) | `Pacocalypse` | gameplay and clean exit |
| [Vasco888888/feup-leic-lcom@cdf16689](https://github.com/Vasco888888/feup-leic-lcom/tree/cdf1668915aaebb900a9ef8eca3252e2414324cb) | `src` | gameplay and clean exit |
| [guystavo7/feup-lcom-project@ad0703be](https://github.com/guystavo7/feup-lcom-project/tree/ad0703bebf8ab8fd00ede7f690667e2034ca682d) | `project/src` | menu and clean exit |
| [RodriguesS2/LCOM-HangMan@6c76df07](https://github.com/RodriguesS2/LCOM-HangMan/tree/6c76df070f321dd20349a6e6429bee2797223b9c) | `proj/src` | gameplay and clean exit |
| [leticia-003/2ndYear_FEUP_LCom@fd08be7e](https://github.com/leticia-003/2ndYear_FEUP_LCom/tree/fd08be7ee796e28742bb07ed2a0171319a0de5e3) | `proj/src` | guarded gameplay, original exit defect |
| [gsilva00/LCOM-Project@4e793123](https://github.com/gsilva00/LCOM-Project/tree/4e793123ae3cd5ad486e1472fa715cff6567c66e) | `proj/src` | guarded gameplay, original exit defect |
| [Cast-10/LCOMProject@e895b1df](https://github.com/Cast-10/LCOMProject/tree/e895b1dfe6aa0269cafc73010da990a195ab1ffd) | `proj` | gameplay and clean exit |
| [antoniocrego/lcom-t01-g3@77307d41](https://github.com/antoniocrego/lcom-t01-g3/tree/77307d413bd8ed873575ef7a8edc6523460f6401) | `proj/src` | gameplay and clean exit |
| [pedroRibas372/MINIX-SpaceInvaders@b34d4cf9](https://github.com/pedroRibas372/MINIX-SpaceInvaders/tree/b34d4cf932aa50fca013b5b70676f615ebcc91af) | `.` | gameplay and clean exit |
| [TM-1-3/WombatMaze@34c66b39](https://github.com/TM-1-3/WombatMaze/tree/34c66b392c0c692b7430b5982fb6324e9864c322) | `proj/src` | guarded gameplay, original exit defect |
| [RiquiFeup/LCOMBAT@6981e429](https://github.com/RiquiFeup/LCOMBAT/tree/6981e4295329ff2f8dfd9e32e51ceaa45a6b2bf4) | `worms` | gameplay and clean exit |
| [Hugo-mgs/LCOM-Space-Invaders@7b554e11](https://github.com/Hugo-mgs/LCOM-Space-Invaders/tree/7b554e113754ffdd5b26b269a0efafea3227468e) | `proj/src` | guarded gameplay and clean menu exit |
| [scrr-az/skess-lcom@08626275](https://github.com/scrr-az/skess-lcom/tree/08626275da972c82291efcab0fbed8a57b0eb854) | `proj/src` | mirror repository |
| [diogopmac/LCOM-Minix_Invaders@c68c985a](https://github.com/diogopmac/LCOM-Minix_Invaders/tree/c68c985ac877eddb0bfb75ecbb28bedcb1c1230a) | `proj/src` | mirror repository |
| [gomesidk/Survive-The-Blast@b73f072f](https://github.com/gomesidk/Survive-The-Blast/tree/b73f072fdcd4d189002e1d376f0fbafa95d25c1e) | `proj/src` | gameplay and clean exit |
| [T0m2sT/LCode@862ec4aa](https://github.com/T0m2sT/LCode/tree/862ec4aa8a2d361f304b78beb3815c4a3ae8d4f1) | `proj/code` | mirror repository |

The machine-readable [survey](../tests/compat-recent-survey.json) contains full SHAs, importer arguments, compiler evidence, exact run contracts and artifact paths. Runtime evidence is under `build/compat-recent/verification/<case-id>/`; the archived 22-case result is `build/compat-recent/recent-results.json`. Each new invocation writes its latest result to `build/compat-recent/verification/results.json`. Search metadata is preserved under `build/compat-recent-*-search.json`.

Three pairs are mirrors, rather than six independent implementations: Yuka/SKESS share 9 C files, Miguel/T0m2sT share 19, and Hugo/diogopmac share 16. Their different repository layouts were built separately. The empty FrozenHart repository is excluded. Gomes and the earlier leorsd pin share the exact same full commit. Across the previous and recent batches, this survey adds 19 source families and 15 additional source families with verified normal exit paths.

Importer fixes found by this batch now rebuild direct framework object dependencies, rebase `/home/lcom/labs` library paths and expose Minix MIN/MAX macros. Default rebuilds confirm the editor, chat and Pacocalypse no longer need library overrides. VBE 0x11B now renders the two 1280x1024 RGB888 games.

Wombat keeps one explicit workaround: `--include ../assets/maze`. Its original `../../assets/maze/map_forest.xpm` include does not resolve from `proj/src/main.c`. No original source or Makefile was patched.

Original defects limit these cases:

- Eduarda declares `int(proj_main_loop)()` at `proj/src/proj.c:24`, conflicting with the current two-argument LCF callback. The compiler explains the conflict; the program was not run.
- Head Football maps VRAM in `devices/gpu/gpu.c:57`, then calls `free(video_mem)` at line 81. macOS aborts in `free_buffers`; mapped VRAM is not malloc memory.
- Wombat destroys sprites in `main.c:592-595`, then destroys the same current sprite through `destroyWombat` at line 600 and `wombat.c:170`. macOS traps in the allocator.
- Hugo shares `barrier_st0` across four barriers in `barrier.c:12`, but each destructor frees that sprite at line 19. Gameplay Escape aborts in `destroy_sprite -> destroyBarrier -> exit_game`. Its original menu Quit remains a clean exit. The diogopmac mirror has the same source ownership defect, but the abort was reproduced on Hugo only.
- Leticia keeps receiving in `Game/game.c:61` after its state sets `running=false`; the inner loop does not test that flag. The ball and brick capture is verified, cleanup is not.

Exact crash report paths, observed statuses and code locations are in the survey. No automatic zero initialization, allocator workaround or student source patch was added.

An older IAM memory-game pin was also exercised independently: the 4x4 card board, timer transition, pause and E exit were verified. Mouse delivery was traced, but a card flip was not confirmed, so the report makes no card-flip claim. A fresh default rebuild and two-run replay also pass in `build/compat-recent/verification/iamaccosta__FEUP-LCOM-memory-gameplay/`, archived in `build/compat-recent/iam-results.json`. Its optional manifest is `tests/compat-iam-extension.json`; it is excluded from the 23 new repository count.

The [paired-project addendum](compatibility-paired.md) verifies bidirectional
DGoiana chat messages under explicit source/input conditions. It also records
the editor's unresolved collaboration defect and the narrower Survive the Blast
connection/cleanup probe. These paired results do not establish complete
multiplayer correctness.
