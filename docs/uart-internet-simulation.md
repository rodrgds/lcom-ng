# Arcade UART over the internet: local simulation

Tested on 15–16 October 2026 with Chrome 155, macOS arm64 and Emscripten 5.0.7.
The initial seven browser builds use revision `87b2f834a0255597f37a2477203c595eb110578f`
plus the existing project profiles in the checkout. No game logic was changed
for this experiment. Ninjix was tested separately on 16 October from revision
`ba9a02d53a6815de740c09a515240fb1008a2f5a`, with its source and bundle hashes recorded.

**Ninjix, LCOMaze and Aerial Ambush are the best initial candidates.** The other five
games need protocol, synchronisation or gameplay fixes before public matchmaking.
Transporting UART bytes is viable, but a serial driver or a connected screen does
not establish working online multiplayer.

## Results

Every game ran with two separate WebAssembly instances at configured round-trip
delays of **0, 20, 50, 100 and 200 ms**, plus **50 ms with ±20 ms one-way jitter**.
These are short startup and input smoke tests, not completed matches.

| Game | Observed result | Online decision |
| --- | --- | --- |
| **Ninjix** | Attacker/defender setup, tower placement, two enemy spawns and shared pause/resume worked at every delay, with jitter, and with a simulated recovery stall. Every transmitted byte reached the other game's UART reader. Some enemy-position drift was visible at higher delay. | Strong first prototype candidate. Full matches need a synchronisation check. |
| **LCOMaze** | Both host/client instances entered the running multiplayer state and accepted movement at every tested delay and with jitter. Its serial protocol coordinates race start and loss, rather than continuously synchronising player positions. | Strong candidate. Race completion still needs a separate check. |
| **Aerial Ambush** | Both roles entered gameplay and exchanged movement and jump inputs at every tested delay and with jitter. Small position differences remained between independently simulated copies. | Suitable for an experimental prototype. Add state correction before promising consistent matches. |
| **Survive the Blast** | Working, matching boards at 0, 100 and 200 ms and in the jitter case. At 20 and 50 ms the peers repeatedly exchanged HELLO/REPLY and returned to the connection screen. | Fix the startup race first. This is not a simple maximum-ping threshold. |
| **Pixel Football** | Both peers started at every delay, but one peer failed to apply the other's movement correctly, including at zero added delay. | Fix existing receive-queue handling before assessing a latency limit. |
| **Minhoca** | Both peers completed readiness and consumed the other player's key make/break bytes at every delay. Their visible snake paths diverged, particularly at 200 ms and with jitter. | Needs synchronisation or state correction. Connection alone passes. |
| **Portal 2D** | Zero-delay key exchange worked. At 20 ms, both peers reported unacknowledged bytes and failed keyboard sends. At 50–200 ms, ACK timeouts and failed sends were frequent. | The existing blocking serial protocol is unsuitable as-is. |
| **BreakPong** | Entered multiplayer and exchanged mouse bytes at every delay, but the remote paddle coordinates remained at their initial values. The multiplayer renderer and update loop provide no ball gameplay. | Multiplayer is incomplete independently of latency. |

The per-game screenshots and raw logs are under
[`build/uart-internet/runs-final/`](../build/uart-internet/runs-final/).
The [machine-readable summary](../build/uart-internet/summary.json) records every
case, measured delays, transmitted/delivered/read bytes, browser errors, protocol
errors and available game-state observations.

### Ninjix follow-up

The initial scan covered seven community games and omitted the built-in Ninjix
example. The follow-up runs the same delayed browser UART bridge in seven
conditions: the five RTT values, jitter, and 50 ms RTT with occasional 150 ms
ordered delivery stalls. Each run places one defender tower, waits for the first
wave, spawns two attacker enemies, pauses both peers, then resumes both peers.

All seven runs delivered the expected action packets with valid checksums,
matching payloads and no browser errors. At 200 ms RTT, complete action packets
reached the receiving C code after approximately **107–115 ms**. In the stall
case, one enemy-spawn packet took **185 ms** and still arrived intact. Screenshots
confirm the tower and enemies appear on both sides and shared pause/resume works.
Enemy positions can differ because each peer advances its own simulation.

Ninjix sends framed action packets and incrementally parses incoming bytes,
without a blocking ACK exchange for every byte. This makes it a better starting
point than Portal 2D's current protocol. Passing these short action tests does
not establish that damage, wave transitions or final results remain consistent
through a complete match. See [`multiplayer.c`](../examples/ninjix/multiplayer.c).

Evidence: [screenshots and raw logs](../build/uart-internet/runs-ninjix/),
[packet and timing summary](../build/uart-internet/ninjix-summary.json), and
[source/bundle provenance](../build/uart-internet/ninjix-provenance.json).

## What the failures mean

**Survive the Blast:** the connection code can send multiple `0xC1` HELLO bytes
before startup completes. Receiving a HELLO starts gameplay, but subsequent
handshake bytes also have the gameplay exit bit set. The captured failing cases
contain repeated HELLO/REPLY sequences. See
[`mm_connection.c`](../examples/community/gomesidk--Survive-The-Blast/proj/src/model/multiMode/connection_state/mm_connection.c)
and [`mm_game.c`](../examples/community/gomesidk--Survive-The-Blast/proj/src/model/multiMode/game_state/mm_game.c).

**Pixel Football:** the driver drains incoming bytes into a software queue, while
the game takes only one queue item per serial interrupt. Staggered entry leaves
startup synchronisation bytes ahead of subsequent key events. The zero-delay
capture already shows the inconsistent player positions. See
[`proj.c`](../examples/community/gsilva00--LCOM-Project/proj/src/proj.c) and
[`uart.c`](../examples/community/gsilva00--LCOM-Project/proj/src/devices/serial_port/uart.c).

**Minhoca and Aerial Ambush:** inputs drive independent local simulations without
ongoing position correction. A delayed turn or a different number of movement
ticks can leave lasting differences. Minhoca's corresponding visible snake
segments differed by approximately 10 pixels in the zero-delay capture, 87 pixels
at 200 ms, and 69 pixels in the jitter case. Those are image measurements from
this input sequence, not general latency thresholds. See
[`input.c`](../examples/community/Forreca05--LCOM-Snake-Game-Projeto-2025/proj/src/input/input.c)
and [`model.c`](../examples/community/alexandre2004costa--Lcom-project/model.c).

**Portal 2D:** `uart_write_message()` waits for an ACK after each byte, with bounded
polling and retries. Keyboard handling blocks on that exchange before applying
the local action. Reliable transport cannot remove those waits or prevent the
game's own deadlines expiring. See
[`uart.c`](../examples/community/BrunoRosendo--Portal2D/proj/Src/uart.c).

**BreakPong:** the multiplayer case draws two paddles, while ball updates and
rendering are confined to single player. The observed remote paddle coordinates
also failed to follow incoming mouse data. See
[`game.c`](../examples/community/Vidal322--BreakPong/proj/src/controller/game/game.c)
and [`view.c`](../examples/community/Vidal322--BreakPong/proj/src/view/view.c).

## Simulation and controls

The experiment uses a private copy of the SDK/runtime under ignored `build/`.
It disconnects local UART loopback, forwards real emulator TX bytes through an
ordered JavaScript delivery queue, and injects them into the other emulator's RX
port. Both games run continuously in the same browser page. The shell omits
focus/visibility pauses and audio. Read-only observers expose existing state in
five games; original student C files are not edited.

An independent C calibration sends and echoes **all 256 byte values on both COM1
and COM2**, checking every value and its order at the receiver. It passes at all
five delays, under jitter, and with simulated 150 ms recovery stalls. Each peer
reads 512 bytes and logs `PASS complete`. Disconnected controls receive no bytes.
Portal's disconnected control still displays its co-op board, but fails serial
sends, demonstrating why a screenshot of the board alone is insufficient.

Jitter uses seed `123456789`; one-way target delay is RTT/2 ±20 ms, bounded below
by zero. Ordered delivery holds following bytes behind delayed bytes. Raw bytes
are not dropped, matching the proposed reliable transport. The stall case adds
150 ms on 1% of calibration and Ninjix bytes to represent retransmission recovery. Actual
delivery times are recorded because browser scheduling adds overhead.

These tests exercise game behaviour under delayed reliable delivery. They do not
establish WebRTC signalling/NAT/TURN connectivity, actual internet routing,
background-tab behaviour, mobile performance, or complete-match correctness.
One run per condition establishes reproducible examples, not a failure-rate
estimate or a certified maximum ping.

LabJack's UART feature shares result notifications rather than a shared blackjack
match. FEUP Hero uses UART for an external audio helper. Classic Chess and Petty
Cats contain serial code without an active multiplayer integration. These were
excluded from the seven-game matrix.

## Reproduce and inspect

From the repository root, replay an existing frozen browser build:

```sh
UART_SIM_RESULTS=runs-replay devenv shell -- \
  node build/uart-internet/lab.mjs portal 50
UART_SIM_RESULTS=runs-replay devenv shell -- \
  node build/uart-internet/lab.mjs aerial 50 jitter
UART_SIM_RESULTS=runs-replay devenv shell -- \
  node build/uart-internet/lab.mjs ninjix 200
```

The [experiment README](../build/uart-internet/README.md) describes preparation,
replay modes and artifacts. Accepted results are in `runs-final/` for the initial
matrix and `runs-ninjix/` for the follow-up; earlier `runs/` contains development probes. Calibration caught and
eliminated an early per-byte timer ordering error before accepted jitter tests.

The [environment record](../build/uart-internet/environment.json) stores tool
versions and browser-bundle hashes. All 169 compiled student C units are preserved
under `build/uart-internet/frozen-sources/`, matching the build manifests. Three
Portal C files received unrelated concurrent cleanup edits after export. Those
edits were preserved; the tests use the earlier frozen browser build. Rebuilding
from the current checkout therefore requires comparing source hashes first.
The 45 compiled Ninjix C units are separately preserved in
`build/uart-internet/ninjix-frozen/`, matching its export manifest and provenance.

No production runtime, game or Arcade source was changed by this experiment.
Nothing was committed, pushed or deployed.
