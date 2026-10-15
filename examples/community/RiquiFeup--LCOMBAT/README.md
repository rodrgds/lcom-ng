<div align="center">
  <h1>LCOMBAT</h1>
  <p>Worms-style 2-player turn-based artillery game running bare-metal on MINIX 3</p>

  [![Language](https://img.shields.io/badge/language-C-blue?style=for-the-badge&logo=c)](https://en.wikipedia.org/wiki/C_(programming_language))
  [![OS](https://img.shields.io/badge/OS-MINIX%203-orange?style=for-the-badge)](https://www.minix3.org/)
  [![Build](https://img.shields.io/badge/build-make-green?style=for-the-badge)](https://www.gnu.org/software/make/)
  [![License](https://img.shields.io/badge/license-MIT-yellow?style=for-the-badge)](LICENSE)
  [![GitHub Stars](https://img.shields.io/github/stars/RiquiFeup/LCOMBAT?style=for-the-badge)](https://github.com/RiquiFeup/LCOMBAT/stargazers)

  <!-- TODO: add images/screenshot.png -->
</div>

---

## About The Project

**LCOMBAT** is a fully playable Worms-style artillery game built for the LCOM course at FEUP. It runs directly on **MINIX 3** using the `liblcf` bare-metal library — no OS services, no standard I/O, only raw hardware interrupts.

Two teams of worms take turns firing weapons on a **procedurally generated, destructible terrain**. The match ends when all enemy worms are eliminated. A 30-second per-turn timer, wind that affects projectile physics, and rising water that kills instantly keep every match tense.

**Key features:**
- 2 teams × 2 worms, each with 100 HP
- Two weapons: **Bazooka** (hold Space to charge power) and **Grenade**
- **Wind** displayed in HUD affects projectile trajectory each turn
- **Destructible terrain** — explosions carve holes in the landscape
- **Rising water** — fallen worms die instantly
- Procedural terrain seeded from the hardware **RTC**
- Full sprite animation system, custom XPM assets, and bitmap font renderer
- MVC architecture throughout: every game entity has a model, controller, and viewer
- Event-driven main loop driven by **8254 Timer** at 60 Hz

---

## Built With

[![C](https://img.shields.io/badge/C-A8B9CC?style=for-the-badge&logo=c&logoColor=white)](https://en.wikipedia.org/wiki/C_(programming_language))
[![MINIX 3](https://img.shields.io/badge/MINIX%203-microkernel%20OS-orange?style=for-the-badge)](https://www.minix3.org/)
[![liblcf](https://img.shields.io/badge/liblcf-LCOM%20framework-lightgrey?style=for-the-badge)](https://web.fe.up.pt/~jca/courses/lcom/)
[![Make](https://img.shields.io/badge/Make-build%20system-6D8764?style=for-the-badge)](https://www.gnu.org/software/make/)

---

## Getting Started

### Prerequisites

- MINIX 3 environment with `liblcf` installed
- Lab drivers (`lab1`–`lab5`) checked out at `../lab1` through `../lab5` relative to this repo
- `make` (comes with the LCOM MINIX image)

### Build & Run

```sh
make
lcom_run proj
```

---

## Project Structure

```
shared/
├── worms/                  — main game project
│   └── src/
│       ├── config.h            — all constants (resolution 800×600, FPS, balancing)
│       ├── main.c              — entry point: hardware init, asset load, state machine
│       ├── engine/
│       │   ├── event/          — event queue (timer ticks, keyboard, mouse)
│       │   ├── graphics/       — renderer, sprite, animated sprite, font
│       │   ├── input/          — keyboard/mouse poll abstraction
│       │   ├── physics/        — AABB hitbox
│       │   └── ui/             — reusable panel, bar, menu widgets
│       ├── game/
│       │   ├── state/          — state machine: all game state transitions
│       │   ├── game.c/h        — GameContext: worms, weapons, terrain, turn state
│       │   ├── worm/           — worm model, controller, viewer
│       │   ├── weapons/        — bazooka and grenade (model/controller/viewer)
│       │   ├── terrain/        — procedural terrain, destructibility
│       │   ├── explosion/      — explosion radius + damage
│       │   ├── water/          — rising water, instant-kill zone
│       │   ├── hud/            — health bars, timer, wind indicator, hotbar
│       │   └── cursor/         — mouse cursor
│       ├── menu/               — splash, main menu, controls, pause, game over
│       └── assets/             — XPM sprites and font bitmaps
├── lab1/ … lab5/           — individual LCOM lab driver projects
└── experiment/             — I/O experimentation sandbox
```

---

## Controls

| Key | Action |
|---|---|
| Arrow keys | Move worm |
| Hold Space | Charge shot power |
| Release Space | Fire |
| `1` | Select Bazooka |
| `2` | Select Grenade |
| `Esc` | Pause |
| `Q` | Quit |

Mouse is used to aim.

---

## Hardware Drivers

| Device | Use |
|---|---|
| VBE MODE 0x115 | 800×600 32 bpp framebuffer |
| 8254 Timer | 60 Hz game tick (interrupt-driven) |
| PS/2 Keyboard | Player input |
| PS/2 Mouse | Aiming cursor |
| RTC | Seed for procedural terrain generation |

---

## License

Distributed under the MIT License. See [LICENSE](LICENSE) for details.

---

## Contact

GitHub: [@RiquiFeup](https://github.com/RiquiFeup)

---

[language-shield]: https://img.shields.io/badge/language-C-blue?style=for-the-badge&logo=c
[license-shield]: https://img.shields.io/badge/license-MIT-yellow?style=for-the-badge
[stars-shield]: https://img.shields.io/github/stars/RiquiFeup/LCOMBAT?style=for-the-badge
[stars-url]: https://github.com/RiquiFeup/LCOMBAT/stargazers