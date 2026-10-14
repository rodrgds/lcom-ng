# Minix compatibility trial results

The initial compatibility trial ran locally on macOS arm64 on 13 October 2026. The [expanded corpus and lab report](compatibility-expanded.md), [recent projects](compatibility-recent.md), and [course audit](course-compatibility-audit.md), and [paired-project addendum](compatibility-paired.md) contain the latest validation across 55 additional repository pins. Both supplied archives and six complete public GitHub projects compile with their original C sources, headers, imports, and Makefiles. The current rerun verifies normal gameplay exits for both supplied archives and five public projects. BomberLab now has a confirmed original GUI use-after-free; its earlier gameplay pass is withdrawn. Compiled source hashes remain unchanged.

## Project matrix

| Project | Source | Build | Gameplay smoke | Clean exit | Additional check or requirement |
| --- | --- | --- | --- | --- | --- |
| Plants vs Zombies | Supplied `PvZ.zip` | Pass | Pass | 0 | Live SDL: deck selection, sunflower placement, resource spending, zombies, pause and quit |
| Ninjix | Supplied `grupo_2leic09_3-main.zip` | Pass | Pass | 0 | Live SDL: tower placement, moving enemies, pause, return to menu and quit |
| MINIX Grand Prix | [m1ke26/MINIX-Grand-Prix](https://github.com/m1ke26/MINIX-Grand-Prix) | Pass | Pass | 0 | Source lists supplied for six local archives; race, car movement, minimap |
| BomberLab | [TiagoRibeiro2006/BomberLab](https://github.com/TiagoRibeiro2006/BomberLab) | Pass | Source defect | Menu exit 0, gameplay 139 | Start callback frees the active view while widget_tick still traverses it. GuardMalloc confirms use-after-free; earlier gameplay pass withdrawn. |
| Minixsweeper | [casemiromjm/minixsweeper](https://github.com/casemiromjm/minixsweeper) | Pass | Pass | 0 | Mouse navigation and revealed cells |
| Guitar Hero | [martimcferreira/lcom-project](https://github.com/martimcferreira/lcom-project) | Pass | Pass | 0 | Track selection and note board; external UART audio helper remains untested |
| Space Shooter | [SamBlack569/feup-lcom-2425](https://github.com/SamBlack569/feup-lcom-2425) | Pass | Pass with explicit mouse option | 0 | `LCOM_COMPAT_MOUSE_REPORTING=1`; source omits the reporting enable command |
| Bomberman | [peucastro/lcom](https://github.com/peucastro/lcom) | Pass | Pass | 0 | Explicit object rules, nested asset root, populated arena and bomb input |
| Incomplete driver game | [ban08/lcom-minix-driver-game](https://github.com/ban08/lcom-minix-driver-game) | Rejected | Unavailable | Unavailable | Original Makefile declares missing `mouse.c` and `sprite.c`; importer reports the missing source |

The public corpus is pinned by full commit SHA in [compat-projects.json](../tests/compat-projects.json), including course-year evidence, build inputs, original controls, and inspected frame paths. Sources and generated evidence stay under ignored `build/compat-corpus/`. The incomplete repository is an expected rejection, not a passing game.

The supplied archive identities are:

```text
PvZ.zip
56ed06a0524442aa83b756a9e6ce049c8cb8f5676adbffaa9037028d0409e929

grupo_2leic09_3-main.zip
a3a0607d79ee9fb5d2c96711fa0c066376d807170321884531b0f585db6a1f31
```

## Repeat the checks

Build the runtime, then run the pinned corpus. `--fetch` downloads missing checkouts. Existing checkouts at another revision are preserved and rejected.

```sh
devenv shell -- lcom-ng-build
devenv shell -- python3 scripts/check_minix_projects.py --fetch \
  --pvz /path/to/PvZ.zip --ninjix /path/to/grupo_2leic09_3-main.zip
```

The JSON result, compiler logs, input traces, and frame artifacts are under `build/compat-corpus/verification/`. A host timeout also terminates the test's student process. Replays have separate successful application exits; a guard-triggered screenshot capture is not a successful application run.

The runtime gate is:

```sh
devenv shell -- lcom-ng-verify
```

The final local gate passes all 28 CTests, the docs build and assets, Remotion typechecking, and whitespace checks. The expanded replay manifest passes 28 cases; the recent manifest passes 22 cases and 36 runs. Original source hashes remain unchanged. Known negative cases retain their explicit classifications.

The initial gate passed all 18 CTests, including native device/unit tests, protocol tests, seven lab solution tests, the integration suite, and eight compatibility checks. The gate also passed the docs build and asset checks, Remotion typechecking, and `git diff --check`. Installed compatibility builds work through `PATH`, relative paths, and external symlinks with a relocated SDK.

Regressions were reproduced before fixes for VRAM mapping, disabled IRQ starvation, unread ACK delays, VBE mode/color correctness, XPM lookup performance, importer inputs, and installed SDK discovery. Single and paired runtime frame artifacts independently check the selected VRAM page after text-mode restoration.

## What this establishes

The compatibility SDK preserves source contracts and the tested device/game flows. It adds portable build syntax around unchanged Minix project files. Public documentation, package identity, CLI help, completion and release artifact names now use `lcom-ng`; existing command aliases and hosted URLs remain available.

This is a gameplay smoke trial, not a full completion of every game or an emulation of all Minix services. Every level, save/load scenario, multiplayer protocol and external serial device remains outside the completed checks. Live student input was inspected for the two supplied games; the six public projects have deterministic frame and input checks. Linux and Windows release workflows were updated, but were not executed locally.

The [compatibility guide](../docs-site/developers/from-minix.md) records exact supported interfaces and remaining limits, including unsupported Minix services and original LCF student-function wrapper tracing. Built-in XPM command arguments and VGA DAC programming were added in the expanded trial. The [decision trail](compatibility-decisions.tsv) records why the changes were made.

Changes remain local and uncommitted. Existing unrelated work was preserved. No release or deployment was performed.
