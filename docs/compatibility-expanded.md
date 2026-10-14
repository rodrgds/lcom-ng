# Expanded Minix compatibility results

The expanded trial downloaded 32 additional public LCOM repositories. The current replay gate passes 28 cases across 19 of those repositories and four lab cases from the previously tested peucastro repository. These checks cover unchanged student sources, imports and Makefiles on macOS arm64.

Use [compat-expanded.json](../tests/compat-expanded.json) to repeat positive cases and [compat-survey.json](../tests/compat-survey.json) to inspect all 32 surveyed repositories. The survey includes full commit SHAs, build inputs, artifact paths and failures.

```sh
devenv shell -- lcom-ng-build
devenv shell -- python3 scripts/check_lcom_corpus.py --fetch
```

The checker verifies source hashes, callback exits, declared guards, output and trace contracts, and frame assertions where requested. It writes evidence under ignored `build/compat-expanded/verification/`. A guard-triggered capture is a passing bounded capture only when the manifest explicitly expects it.

The [recent-project trial](compatibility-recent.md) adds another 23 repositories,
with 22 runnable cases and explicit source-family and cleanup limits. The
[course audit](course-compatibility-audit.md) covers all Resumos lessons.

## Project replay coverage

| Repository | Current project check | Scope |
| --- | --- | --- |
|[diogoalves12/feup-lcom](https://github.com/diogoalves12/feup-lcom)|Menu and clean exit|The Bomberman menu renders and Escape exits normally. Its startup sample-rate command passes after the PS/2 F3 fix. Full gameplay is not in the current replay gate.|
|[Adriano-7/feup-lcom](https://github.com/Adriano-7/feup-lcom)|Gameplay and clean exit|Space Invaders enters gameplay, moves, fires and exits normally. A separate guarded capture checks the populated frame. The standard 256-color VGA palette and DAC now replace the earlier grayscale limitation.|
|[DSV8/FEUP-LCom](https://github.com/DSV8/FEUP-LCom)|Gameplay and clean exit|Cosmic Garden starts with key 1, handles movement and mouse shooting, returns to its menu and exits normally. Lab 2 passes.|
|[josesereno/Bamboozlers](https://github.com/josesereno/Bamboozlers)|Gameplay and clean exit|Enter opens level selection. Mouse selection enters level 1, arrow movement/jumping runs, and Escape exits normally.|
|[bersanots/FEUP-LCOM](https://github.com/bersanots/FEUP-LCOM)|Menu and clean exit|StickyBall menu renders and exits through its mouse control. Earlier guarded match captures exist, but the current replay gate checks the menu exit only.|
|[andrenasx/FEUP-LCOM](https://github.com/andrenasx/FEUP-LCOM)|Menu and clean exit|Asteroids reaches its populated menu and exits with Escape. Lab 2 passes. Gameplay is outside the current gate.|
|[FrozenHart/LCOM_2025](https://github.com/FrozenHart/LCOM_2025)|Menu and clean exit|Blackjack menu renders and exits normally. Starting a single-player deal fails because player 1 has zero hands, an original game-state defect.|
|[raulnasoliveira/FEUP-LCOM-2025](https://github.com/raulnasoliveira/FEUP-LCOM-2025)|Guarded gameplay|The verbal memory game reaches its word, New/Seen controls and lives display. Its original while (1) loop has no application exit, so the replay ends at the declared guard. Lab 2 passes.|
|[IMS-up2021/Star-Wars-Defend-the-Republic-LCOM-2025-](https://github.com/IMS-up2021/Star-Wars-Defend-the-Republic-LCOM-2025-)|Menu and clean exit|Star Wars renders its menu and exits with Escape. Gameplay and multiplayer are outside the current gate.|
|[zefluxo/feup-lcom](https://github.com/zefluxo/feup-lcom)|Guarded gameplay|Tetris handles piece movement/rotation in a guarded gameplay capture. A separate mouse menu exit finishes normally. Lab 2 passes.|

Cosmic Garden, Bamboozlers and Space Invaders have gameplay input and clean application exits. Tetris has a guarded gameplay capture and a separate clean menu exit. The verbal memory application deliberately has no exit path. StickyBall, Asteroids, Blackjack, Bomberman and Star Wars have current menu checks; those checks do not establish complete gameplay.

The earlier Lerbirinto gameplay claim is withdrawn. Correct RTC update interrupts expose an original 17-byte buffer that `sprintf` fills with a 21-byte date string. Its Lab 2 case still passes. The crash evidence is `build/compat-expanded/verification/bdmendes__feup-lcom-project/upstream-rtc-overflow.json`.

## Student lab coverage

Fourteen newly downloaded repositories pass their original Lab 2 `timer_test_int 2` callback. Each checks two elapsed messages, a normal exit and the expected final trace tick. The repositories are biromiro, bdmendes, xico2001pt, Adriano-7, leorsd, RicardoSilvaBW, manuelcralves, DSV8, andrenasx, raulnasoliveira, pedrojfs17, Telmo465, liac1983 and zefluxo.

| Original lab | Current peucastro callbacks | Checks |
| --- | --- | --- |
|lab2|`timer_test_read_config`, `timer_test_time_base`, `timer_test_int`|Normal callback exits, output or device traces as declared|
|lab3|`kbd_test_scan`, `kbd_test_poll`, `kbd_test_timed_scan`|Normal callback exits, output or device traces as declared|
|lab4|`mouse_test_packet`, `mouse_test_async`, `mouse_test_gesture`, `mouse_test_remote`|Normal callback exits, output or device traces as declared|
|lab5|`video_test_init`, `video_test_rectangle`, `video_test_pattern`, `video_test_xpm`, `video_test_move`|Normal exits plus independent rectangle, pattern and XPM frame assertions|

These are original Minix lab implementations, compiled through the compatibility builder and dispatched by LCF-compatible argument handling. Native generated labs remain a separate workflow. The supplied Ninjix Lab 1 additionally passes all 20 original assertions. RTC and serial variants have narrower evidence in the [course audit](course-compatibility-audit.md), including a repeatable serial configuration gate. They do not establish broad original RTC/serial application coverage.

## Changes driven by real failures

- The builder accepts header entries in `SRCS`, declarative BSD source loops and local archive object declarations without executing student shell recipes.
- Compatibility declarations support student-local timer headers and current Minix register/message layouts. The SDK provides common mouse startup helpers.
- PS/2 sample-rate and resolution commands, remote reads, device identity and status queries now follow the tested command/ACK behavior.
- RTC interrupt status, periodic timing and update-ended notifications have dedicated regressions. Correct delivery can expose bugs in unchanged student code.
- The standard 256-color VGA palette and six-bit DAC ports now display indexed games in color. Adriano Space Invaders no longer has the earlier grayscale limitation.
- Built-in XPM callback arguments and typed Lab 2 through Lab 5 dispatch run original callbacks with their expected arguments.

## Full repository survey

The classifications separate source defects, incompatible old API contracts, host limits and unresolved SDK behavior. A successful build is not a successful game run. Some repositories have passing labs while their final projects remain unsupported.

| Repository and pinned commit | Positive coverage | Remaining project result |
| --- | --- | --- |
|[literallysofia/feup-lcom](https://github.com/literallysofia/feup-lcom/tree/1eeba23e4f48bd7fc96501f5f09dce03a0f882f5)<br>`1eeba23e4f48bd7fc96501f5f09dce03a0f882f5`|None in positive gate|Project uses older VBE/low memory declarations and struct reg86u. It needs a separate older SDK contract.|
|[biromiro/feup-lcom](https://github.com/biromiro/feup-lcom/tree/256b46081a43be783ad77b7b167fd8d301a3d83e)<br>`256b46081a43be783ad77b7b167fd8d301a3d83e`|Lab 2|Git contains distinct A.xpm and a.xpm files, and equivalent pairs for other letters. Default case-insensitive macOS volumes collapse those paths. The source symbols are valid in the Git tree. Lab 2 passes.|
|[joaomalheiro/FEUP-LCOM](https://github.com/joaomalheiro/FEUP-LCOM/tree/4ecd190ee35d26308e48fc7d3be3469b302c52a5)<br>`4ecd190ee35d26308e48fc7d3be3469b302c52a5`|None in positive gate|Final project includes x86 rtc_assembly.S. The arm64 C importer does not translate assembly. No unchanged project run is claimed.|
|[bdmendes/feup-lcom](https://github.com/bdmendes/feup-lcom/tree/c62b7ffe669602024eed3697d69ec3272a48bbe8)<br>`c62b7ffe669602024eed3697d69ec3272a48bbe8`|Lab 2|Project RTC code allocates current_time[17], then sprintf writes 21 bytes. Correct update-ended IRQ delivery exposes SIGTRAP at tick 60. Earlier gameplay and menu claims are withdrawn. Lab 2 passes.|
|[nm-marques/FEUP-LCOM](https://github.com/nm-marques/FEUP-LCOM/tree/f2f8e08e8a55a5d6d4e35bf9612907b6693fd6ec)<br>`f2f8e08e8a55a5d6d4e35bf9612907b6693fd6ec`|None in positive gate|Bare liblm.h now resolves. The next failure is obsolete struct reg86u in vcard.c. The old Lab 2 callback also unconditionally returns 1 after two elapsed seconds.|
|[joaooliveira-11/feup-lcom](https://github.com/joaooliveira-11/feup-lcom/tree/a85d28cfdb6b7260b081a6fad1a28eede1364f84)<br>`a85d28cfdb6b7260b081a6fad1a28eede1364f84`|None in positive gate|mouse.c declares mouse_answer without initialization and reads it in the loop condition. Clang emits a trap before gameplay starts.|
|[RodolfoF31/FEUP_LCOM](https://github.com/RodolfoF31/FEUP_LCOM/tree/07bef179a624ab19df05092006bd87d3980dd966)<br>`07bef179a624ab19df05092006bd87d3980dd966`|None in positive gate|The original Makefile lists vg.c, which is absent from the pinned repository.|
|[xico2001pt/feup-lcom](https://github.com/xico2001pt/feup-lcom/tree/ee3eadfe89ce470a35c0ad20c9f9e6b233f84844)<br>`ee3eadfe89ce470a35c0ad20c9f9e6b233f84844`|Lab 2|Lab 2 passes. Lab 5 uses sys_readbios to read real-mode BIOS data, which the compatibility SDK does not implement.|
|[diogoalves12/feup-lcom](https://github.com/diogoalves12/feup-lcom/tree/14cf5aaefacf17cde4e309f5c6dc18d6195bb49a)<br>`14cf5aaefacf17cde4e309f5c6dc18d6195bb49a`|Menu and clean exit|The Bomberman menu renders and Escape exits normally. Its startup sample-rate command passes after the PS/2 F3 fix. Full gameplay is not in the current replay gate.|
|[Adriano-7/feup-lcom](https://github.com/Adriano-7/feup-lcom/tree/e0b46670ed2122e2ad303a7d5ed83e51b517d215)<br>`e0b46670ed2122e2ad303a7d5ed83e51b517d215`|Lab 2, Gameplay and clean exit|Space Invaders enters gameplay, moves, fires and exits normally. A separate guarded capture checks the populated frame. The standard 256-color VGA palette and DAC now replace the earlier grayscale limitation.|
|[leorsd/FEUP-LCOM](https://github.com/leorsd/FEUP-LCOM/tree/b73f072fdcd4d189002e1d376f0fbafa95d25c1e)<br>`b73f072fdcd4d189002e1d376f0fbafa95d25c1e`|Lab 2 and project replay|Lab 2 passes. Fresh default project build, populated single-player board and original Escape/menu exit pass with unchanged sources. Minix NONE and VBE 0x11B gaps are resolved. This is the same full commit as gomesidk/Survive-The-Blast.|
|[afonsojramos/feup-lcom](https://github.com/afonsojramos/feup-lcom/tree/5200ee9db709971f9d9ca15ab344413f3e45ab4a)<br>`5200ee9db709971f9d9ca15ab344413f3e45ab4a`|None in positive gate|Local VBE declarations use an older type contract and collide with the current SDK. The host clock-name collision was resolved separately.|
|[RicardoSilvaBW/FEUP--LCOM](https://github.com/RicardoSilvaBW/FEUP--LCOM/tree/b9b0bec2a8c39005e7a673d8d3a36fad71671f38)<br>`b9b0bec2a8c39005e7a673d8d3a36fad71671f38`|Lab 2|Pom Power renders its menu. menu.c frees menu_bg_buf, then delete_static_background frees the same pointer again on exit. Lab 2 passes.|
|[bernardobelchior/feup-lcom](https://github.com/bernardobelchior/feup-lcom/tree/14a410320637918a4b3df90177c5d319802da501)<br>`14a410320637918a4b3df90177c5d319802da501`|None in positive gate|Project contains x86 set_pixel.S. The arm64 compatibility builder rejects architecture-specific assembly.|
|[manuelcralves/FEUP-LCOM](https://github.com/manuelcralves/FEUP-LCOM/tree/b1269689b1eb2dab1626c64d684bc7d204073494)<br>`b1269689b1eb2dab1626c64d684bc7d204073494`|Lab 2|Original proj_main_loop(void) conflicts with the current LCF callback int proj_main_loop(int argc, char **argv). Lab 2 passes.|
|[andreiasilva66/FEUP-LCOM](https://github.com/andreiasilva66/FEUP-LCOM/tree/4d1afd00d67bf2023e2fc2c4cf15ba03726605ea)<br>`4d1afd00d67bf2023e2fc2c4cf15ba03726605ea`|None in positive gate|Project declares void proj_main_loop instead of the current int callback. Lab 2 has unresolved version-control conflict markers.|
|[DSV8/FEUP-LCom](https://github.com/DSV8/FEUP-LCom/tree/ca0515a861cc7c9b21d86f97290945cea1f7a487)<br>`ca0515a861cc7c9b21d86f97290945cea1f7a487`|Lab 2, Gameplay and clean exit|Cosmic Garden starts with key 1, handles movement and mouse shooting, returns to its menu and exits normally. Lab 2 passes.|
|[josesereno/Bamboozlers](https://github.com/josesereno/Bamboozlers/tree/8492b6ae42767efee595e3265c2852bba595268a)<br>`8492b6ae42767efee595e3265c2852bba595268a`|Gameplay and clean exit|Enter opens level selection. Mouse selection enters level 1, arrow movement/jumping runs, and Escape exits normally.|
|[bersanots/FEUP-LCOM](https://github.com/bersanots/FEUP-LCOM/tree/04e6ad63181e05a801c8f71eedf440c6bda04fc0)<br>`04e6ad63181e05a801c8f71eedf440c6bda04fc0`|Menu and clean exit|StickyBall menu renders and exits through its mouse control. Earlier guarded match captures exist, but the current replay gate checks the menu exit only.|
|[iamaccosta/FEUP-LCOM](https://github.com/iamaccosta/FEUP-LCOM/tree/977a5cee882ba8016f0c6535512611a44bb64e12)<br>`977a5cee882ba8016f0c6535512611a44bb64e12`|Project replay|Fresh default project build passes. Guarded populated 4x4 memory board and timer transition, then original pause/E cleanup exit 0 pass with unchanged sources. A mouse card flip is not verified. Its old Lab 2 test uses IRQ_SETPOLICY as an interrupt mask.|
|[nunoMeloCastro/FEUP-LCOM](https://github.com/nunoMeloCastro/FEUP-LCOM/tree/88fb5ee000b99b51014ec06cf8420ecdbf65a7cd)<br>`88fb5ee000b99b51014ec06cf8420ecdbf65a7cd`|None in positive gate|Sudoku casts video_mem and allocated buffer pointers to 32-bit unsigned int before memcpy. The casts truncate 64-bit host addresses and cause SIGSEGV.|
|[andrenasx/FEUP-LCOM](https://github.com/andrenasx/FEUP-LCOM/tree/4c34535b3ed40a3f60dba1d2c63fbe6aec946883)<br>`4c34535b3ed40a3f60dba1d2c63fbe6aec946883`|Lab 2, Menu and clean exit|Asteroids reaches its populated menu and exits with Escape. Lab 2 passes. Gameplay is outside the current gate.|
|[FrozenHart/LCOM_2025](https://github.com/FrozenHart/LCOM_2025/tree/f1879939f7a9a8e9610b9dcfc228b6aee1723e66)<br>`f1879939f7a9a8e9610b9dcfc228b6aee1723e66`|Menu and clean exit|Blackjack menu renders and exits normally. Starting a single-player deal fails because player 1 has zero hands, an original game-state defect.|
|[raulnasoliveira/FEUP-LCOM-2025](https://github.com/raulnasoliveira/FEUP-LCOM-2025/tree/a94d7998e4a0ed234aa0e13f8444195ffc9d332a)<br>`a94d7998e4a0ed234aa0e13f8444195ffc9d332a`|Lab 2, Guarded gameplay|The verbal memory game reaches its word, New/Seen controls and lives display. Its original while (1) loop has no application exit, so the replay ends at the declared guard. Lab 2 passes.|
|[IMS-up2021/Star-Wars-Defend-the-Republic-LCOM-2025-](https://github.com/IMS-up2021/Star-Wars-Defend-the-Republic-LCOM-2025-/tree/c2a8b571335769d971fa1128c2374834b299208d)<br>`c2a8b571335769d971fa1128c2374834b299208d`|Menu and clean exit|Star Wars renders its menu and exits with Escape. Gameplay and multiplayer are outside the current gate.|
|[pedrojfs17/FEUP-LCOM](https://github.com/pedrojfs17/FEUP-LCOM/tree/a4f91d0021bcf8cc863a5f3df37aa681db01fd07)<br>`a4f91d0021bcf8cc863a5f3df37aa681db01fd07`|Lab 2|Lab 2 passes. The project omits timer_get_conf, a student-implemented function in the current course contract. The runtime does not invent a lab solution to make it link.|
|[Telmo465/FEUP-LCOM](https://github.com/Telmo465/FEUP-LCOM/tree/d6988cce28502d874a5233a08da7f1c87483665b)<br>`d6988cce28502d874a5233a08da7f1c87483665b`|Lab 2|Lab 2 passes. Project code calls the older LCF proj_demo helper, which the compatibility SDK does not provide.|
|[JozePRamos/FEUP_LCOM](https://github.com/JozePRamos/FEUP_LCOM/tree/2cba5ed392d91bc0a7fae8abb2171f193b88a6a2)<br>`2cba5ed392d91bc0a7fae8abb2171f193b88a6a2`|None in positive gate|Local kbd_ih returns int instead of the current LCF void prototype. Lab 2 timer.c has an incomplete expression.|
|[liac1983/FEUP-LCOM](https://github.com/liac1983/FEUP-LCOM/tree/fd05d251af20b5325a6958c8d6ab030d3a1e94ad)<br>`fd05d251af20b5325a6958c8d6ab030d3a1e94ad`|Lab 2|Original proj_main_loop(void) conflicts with the current LCF argc/argv callback. Lab 2 passes.|
|[sirkotal/feup-lcom](https://github.com/sirkotal/feup-lcom/tree/8bff6e77a89f9c209b24762208575ee7c60994cc)<br>`8bff6e77a89f9c209b24762208575ee7c60994cc`|None in positive gate|Keyboard and timer drivers write 32 bits through addresses of 8-bit stack variables. macOS reports stack buffer overflow in kbc_write_command or timer_set_frequency.|
|[zefluxo/feup-lcom](https://github.com/zefluxo/feup-lcom/tree/e65ab1042229cd99acc5d21a69efbba9d6f53792)<br>`e65ab1042229cd99acc5d21a69efbba9d6f53792`|Lab 2, Guarded gameplay|Tetris handles piece movement/rotation in a guarded gameplay capture. A separate mouse menu exit finishes normally. Lab 2 passes.|
|[tomasnr13/FEUP-LCOM](https://github.com/tomasnr13/FEUP-LCOM/tree/6c600d791b13bf15c57575822e17625f7dd70757)<br>`6c600d791b13bf15c57575822e17625f7dd70757`|None in positive gate|The pinned Lab 5 source omits graphic_consts.h and Lab 2 omits i8254.h.|

## Remaining support limits

Architecture-specific x86 assembly needs an execution or translation strategy on arm64. Student casts of host pointers to 32-bit integers also need source adaptation or a 32-bit environment. A case-sensitive checkout is required when a repository contains both `A.xpm` and `a.xpm`; the runtime cannot restore files that the filesystem collapsed.

Older `reg86u`, VBE/low-memory declarations and LCF helper variants remain outside the current SDK contract. The survey records an unresolved `sys_readbios` case and a project missing its student-implemented `timer_get_conf` function. The earlier `NONE` declaration gap is resolved. The unresolved BIOS service and missing student function remain excluded from successful game claims.

Projects with missing files, merge conflicts, double frees, uninitialized values or buffer overflows retain those defects. The checker reports them instead of changing their sources or hiding the failures.

The trial does not establish every level, save/load path, multiplayer protocol or external serial device. These new corpus runs are deterministic headless checks. Live SDL keyboard/mouse testing, relocated macOS archive checks and native student-lab checks have separate evidence. See the [macOS student guide](../docs-site/students/macos.md) and [Minix compatibility guide](../docs-site/developers/from-minix.md) for installation and supported interfaces.

Additional unchanged-source project replays for leorsd and IAM pass through
`tests/compat-leorsd-extension.json` and `tests/compat-iam-extension.json`. Leorsd
and the recent Gomes repository share the same complete commit and count as one
source family.
