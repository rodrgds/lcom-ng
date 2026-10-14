# Paired original-project evidence

The current public CLI ran original student programs as distinct left/right processes. No student source or runtime implementation was changed. Full commands, scripts, compiler/source hashes and observed statuses are in [the paired evidence](../tests/compat-paired-evidence.json).

DGoiana chat passes actual bidirectional text exchange. In both the capture and cleanup run, the right process prints `received message: HELLO.` while the left process prints `received message: WORLD.`. Only the left script types HELLO and only the right script types WORLD. The inspected left frame displays both messages and both usernames. The right frame confirms both usernames; its message area was blank in the bounded snapshot, so the receiver stdout is the independent receipt proof. Capture ends deliberately with guard 1 at 1200. A separate identical run continues to original Escape cleanup and exits 0. Original source hashes remain unchanged.

This case requires supported `--realtime` and a host `--timeout 75`. Deterministic paired execution otherwise times out 124 while startup UART data remains pending and the original menu ignores UART IRQs. The source writes divisor bytes without first setting DLAB. The driver drains queued keyboard bytes and retains only the last, so the scripts use physical key makes/releases on separate ticks. Incoming and outgoing text reuse the same array/index; concurrent username typing reproduced `ALPHABETA.` and `strlen(NULL)` crashes. Staggered original username entry avoids that collision. Uninitialized cursor locals remain a source defect; the script uses a saturating mouse route rather than changing source initialization. This is verified bidirectional communication under those explicit input conditions, not unrestricted chat correctness.

The editor was separately tested with its actual Portuguese colon mapping, shift+PERIOD. The left buffer contains HELLOA and the right contains only its local B. Collaboration is not verified. Its UART driver masks IIR with 0x0E but compares against 0x02/0x06 rather than 0x04 RX available and 0x0C FIFO timeout. Both processes still exit 0 under realtime after Escape. The first US-layout text-colon attempt did not activate the command and is excluded from the UART conclusion.

Survive the Blast was also run as a pair using staggered multiplayer entry, guarded capture and a separate cleanup run. The left frame contains two players and a populated board; the right bounded frame contains only the back arrow. Both processes exit 0. This additional probe exercises the original connection and cleanup paths but does not prove a fully rendered, synchronized multiplayer game on both peers.

The public `run-pair` currently provides no runtime `--trace` option. This addendum uses source-tagged receiver stdout, fresh independent frames and exact source hashes. The single-chat failure trace is preserved at `build/compat-recent/paired/chat/single-debug.jsonl`. Do not describe these artifacts as a complete UART wire trace.

Reproduce the successful chat capture after building the pinned project:

```sh
lcom-ng compat build build/compat-recent/corpus/DGoiana__feup-lcom/proj/src \
  --output build/compat-recent/paired/chat-staggered/build
lcom-ng run-pair --headless --realtime --max-ticks 1200 --timeout 75 \
  --left-script scripts/compat-corpus/paired/chat-left.mlabscript \
  --right-script scripts/compat-corpus/paired/chat-right.mlabscript \
  --left-dump-frame build/compat-recent/paired/chat-staggered/capture-left.ppm \
  --right-dump-frame build/compat-recent/paired/chat-staggered/capture-right.ppm \
  build/compat-recent/paired/chat-staggered/build/proj \
  --right build/compat-recent/paired/chat-staggered/build/proj
```

For the cleanup contract use `--max-ticks 1700` with the same scripts and separate artifact filenames. The scripts press Escape at 1500; the expected pair status is 0. The evidence JSON records the original commands and portable replay commands. Archived outcomes remain in `build/compat-recent/paired/chat-staggered/results.json`.

The older leorsd pin is the exact same full commit as Gomes. A fresh default build, populated single-player capture and cleanup exit 0 now pass; its old NONE declaration gap is resolved. `build/compat-recent/paired/corrections/leorsd-survey-row.json` and `build/compat-recent/paired/corrections/iamaccosta-survey-row.json` replace those precise old survey rows. `compat-leorsd-extension.json`, `compat-iam-extension.json` and `doc-replacements.json` provide the associated persistent contracts and prose corrections. Within the recent batch there are 20 source families, but only 19 are additional to the earlier corpus because Gomes duplicates old leorsd.
