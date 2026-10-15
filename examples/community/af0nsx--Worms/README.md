# LCOMBAT

A Worms-style 2-player turn-based artillery game running on **MINIX 3** via **liblcf** (bare-metal, no OS services). Developed as a LCOM lab project.

---

## Gameplay

Two teams of two worms take turns firing weapons at each other on a procedurally generated destructible terrain. The team that eliminates all enemy worms wins. Each turn lasts **30 seconds**. Worms that fall into the water die instantly.

- 2 teams × 2 worms, each with 100 HP
- Wind affects projectile trajectory each turn (shown in the HUD)
- Weapons: **Bazooka** and **Grenade**

---

## Controls

| Key | Action |
|---|---|
| Arrow keys | Move worm |
| Hold Space | Charge shot power |
| Release Space | Fire |
| 1 | Select Bazooka |
| 2 | Select Grenade |
| Esc | Pause |
| Q | Quit |

Mouse is used to aim.

---

## How to build and run

Requires the LCOM MINIX environment with `liblcf` and the companion lab drivers (`lab1`–`lab5`) at `../lab1` through `../lab5`.

```sh
make
lcom_run proj
```

---

## Project structure

```
src/
  config.h              — all constants (resolution, FPS, balancing values)
  main.c                — entry point: hardware init, asset load, state machine launch
  engine/
    event/              — event queue (timer ticks, keyboard, mouse)
    graphics/           — renderer, sprite, animated sprite, font
    input/              — keyboard/mouse poll abstraction
    physics/            — AABB hitbox
    ui/                 — reusable panel, bar, menu widgets
  game/
    state/state_machine.c — main game loop, all state transitions
    game.c / game.h     — GameContext: worms, weapons, terrain, turn state
    worm/               — worm model, controller, viewer
    weapons/            — bazooka and grenade (model/controller/viewer)
    terrain/            — procedural terrain, destructibility
    explosion/          — explosion radius + damage
    water/              — rising water, instant-kill zone
    hud/                — health bars, timer, wind indicator, hotbar
    cursor/             — mouse cursor
  menu/                 — splash, main menu, controls screen, pause, game over
  assets/               — XPM sprites and font
```

---

## Hardware used

| Device | Use |
|---|---|
| VBE MODE_115 | 800×600 32bpp framebuffer |
| 8254 Timer | 60 Hz game tick |
| PS/2 Keyboard | Player input |
| PS/2 Mouse | Aiming cursor |
| RTC | Seed for terrain generation |