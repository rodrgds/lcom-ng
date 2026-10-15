---
name: LCOM Arcade Web
description: A dense gallery of original student games on an ink-violet ground.
colors:
  ink-violet: "#101021"
  warm-white: "#fff8ed"
  acid-lime: "#c5ff62"
  muted-lilac: "#bcb9d5"
  control-hover: "#29263e"
  search-surface: "#252338"
  search-border: "#77718f"
  progress-track: "#34304c"
  secondary-action: "#302b48"
  library-error: "#ffb9b3"
  loading-error: "#ffd0c8"
typography:
  identity:
    fontFamily: "Bricolage, system-ui, sans-serif"
    fontSize: "27px"
    fontWeight: 800
    lineHeight: 1
    letterSpacing: "-0.035em"
  display:
    fontFamily: "Bricolage, system-ui, sans-serif"
    fontSize: "clamp(26px, 4vw, 44px)"
    fontWeight: 800
    lineHeight: 1.05
    letterSpacing: "-0.035em"
  title:
    fontFamily: "Bricolage, system-ui, sans-serif"
    fontSize: "17px"
    fontWeight: 800
    lineHeight: 1.12
  body:
    fontFamily: "Bricolage, system-ui, sans-serif"
    fontSize: "16px"
    lineHeight: 1.5
  status:
    fontFamily: "Bricolage, system-ui, sans-serif"
    fontSize: "13px"
rounded:
  progress: "8px"
  action: "10px"
  mobile-tile: "11px"
  control: "12px"
  tile: "14px"
  loading-art: "20px"
spacing:
  mobile-grid-gap: "9px"
  grid-gap: "12px"
  search-bottom: "14px"
  library-edge: "20px"
  loading-gap: "44px"
components:
  icon-button:
    textColor: "{colors.muted-lilac}"
    rounded: "{rounded.control}"
    width: "44px"
    height: "44px"
  icon-button-hover:
    backgroundColor: "{colors.control-hover}"
    textColor: "{colors.warm-white}"
  search:
    backgroundColor: "{colors.search-surface}"
    textColor: "{colors.warm-white}"
    rounded: "{rounded.control}"
    height: "44px"
    padding: "0 14px"
  game-card:
    rounded: "{rounded.tile}"
  button-primary:
    backgroundColor: "{colors.acid-lime}"
    textColor: "{colors.ink-violet}"
    rounded: "{rounded.action}"
    padding: "10px 16px"
  button-secondary:
    backgroundColor: "{colors.secondary-action}"
    textColor: "{colors.warm-white}"
    rounded: "{rounded.action}"
    padding: "10px 16px"
---

# Design System: LCOM Arcade Web

## Overview

**Creative North Star: "The artwork is the arcade"**

The web library uses dense gameplay scenes, warm heavy lettering and an acid-lime L/play mark on an ink-violet ground. The user supplied Friv references establish the gallery density and names over wide images; original student artwork supplies the color and character. Colorful hover glow is an explicit user-pinned requirement.

This document records the web launcher and startup screen from sdk/collection/web-template/. Native Android remains a separate platform scope: its existing black, white and mint palette, compact adaptive tiles and platform controls continue independently. Web tokens and Bricolage typography do not redefine native mobile control colors.

**Key Characteristics:**
- Gameplay artwork carries the identity.
- Compact icon tools leave the gallery in view.
- Warm, heavy type and rounded wide tiles.
- Measured loading progress followed by the first presented game frame.

## Colors

The primary accent is acid-lime; the neutral field is ink-violet with warm-white type and muted-lilac supporting copy. Frontmatter values are normative for the web surface.

### Primary

- **Acid Lime:** identity mark, play affordance, focus outline, text selection and measured download fill.

### Neutral

- **Ink Violet / Warm White:** web gallery and loading ground, identity and primary copy.
- **Muted Lilac:** icon tools, descriptions, status and empty states.
- **Control Hover / Search Surface / Search Border:** quiet utility controls and the revealed search field.
- **Progress Track / Secondary Action:** startup progress rail and the return action after a failed load.
- **Library Error / Loading Error:** readable error copy within the corresponding dark surface.

**The Artwork Rule.** Use original game previews or icons as the visual content; keep names over a dark contrast scrim.

## Typography

**Display and Body Font:** self-hosted Bricolage Grotesque, registered as `Bricolage`, with system-ui and sans-serif fallbacks. The variable asset supports weights 200–800 and uses font-display swap. Preserve `sdk/collection/web-template/assets/OFL.txt` beside it.

Heavy compact headings and softer supporting copy give the library its own voice without replacing a game's type.

- **Identity:** frontmatter identity role; reduces to 23px at 700px and 21px at 480px.
- **Loading display:** frontmatter display role; becomes 30px on the stacked mobile and short landscape layouts.
- **Tile title:** frontmatter title role, balanced wrapping and a dark text shadow. The enlarged desktop tile uses clamp(22px, 2.2vw, 32px); all titles become 14px at 700px.
- **Description:** frontmatter body role; reduces to 15px at 700px.
- **Transfer status:** frontmatter status role with tabular numerals.

## Layout

The library is centered with a maximum width of 1920px. Its desktop edges and bottom use 20px minimum safe-area padding. The header is 84px high; the icon tools are 44px squares with a 6px gap. The grid starts immediately beneath it.

Desktop uses auto-filled columns with a 190px minimum and 12px gaps. At 1600px the minimum becomes 218px. Without a search query, the first game spans two columns and two rows. Ordinary artwork has a 1.7:1 aspect ratio and uses cover cropping.

At 700px and below, the library has 10px minimum horizontal padding, a 70px header, three equal columns and 9px gaps. The featured tile becomes ordinary. At 480px and below, the header is 66px and the grid has two equal columns. Names stay on the image at every size.

Startup centers artwork and copy side by side with a 44px gap. Artwork is at most 420px wide with a 4:3 crop; copy is at most 380px wide. At 700px and below it stacks with a 28px gap and a 16:10 crop at min(66vw, 320px). At heights up to 500px and widths from 600px it returns to a compact horizontal layout. Loading can scroll when needed.

The game iframe fills the viewport. The home button uses reported game and control bounds to prefer available letterboxing and avoid occupied regions. Safe areas remain respected.

**The Responsive Gallery Rule.** Keep the implemented wide tile proportions and responsive column changes; the enlarged first tile belongs to the desktop gallery only.

Native Android retains its independently adaptive compact grid, square game imagery and native player behavior. Its existing launcher palette remains black, white and mint.

## Elevation & Depth

Game images supply most of the depth. A dark top scrim and text shadow maintain title contrast. Fine-pointer movement adds a soft violet ambient field and nearby card glow. Each card receives a hue from its rendered index, rather than a fixed game-brand palette.

Hover and keyboard focus raise a card by 3px, scale its image to 1.075 and reveal a lime circular play mark. The colored shadow is `0 12px 32px -8px hsl(var(--hue) 90% 65% / .7)`. Loading artwork has a soft violet shadow. None of these shadows is a hard offset frame.

Motion uses cubic-bezier(.16, 1, .3, 1): card lift/glow takes .32s, image scale .6s, and the play affordance .25s. Reduced motion removes transitions, hover/focus transforms and the ambient field. The proximity glow also runs only with a fine pointer and no reduced-motion preference.

**The Requested Glow Rule.** Preserve colorful card hover and keyboard-focus glow. It is a user-pinned interaction, with reduced-motion behavior respected.

## Shapes

Wide screenshot tiles have 14px corners, reducing to 11px at the mobile breakpoint. Utility buttons and search use 12px corners; retry/return actions use 10px. The loading preview has 20px corners and a small rotated identity mark overlapping its lower-right corner. The mark itself is a rounded lime square with an ink L/play symbol. The progress rail is a slim rounded strip.

## Components

### Identity and icon tools

A linked L/play mark and heavy collection name form one compact masthead. Search and fullscreen are outlined SVG icons with accessible names, quiet default color and a darker hover/expanded surface. Fine pointers use the provided custom cursor. Keyboard focus receives a 3px lime outline with a 4px offset.

### Revealed search

Search is hidden until its icon is activated. The field is 44px high and at most 420px wide. Opening moves focus into it. Escape or closing the icon clears the query, restores the gallery and returns focus to the toggle. Filtering matches game names and removes the desktop feature span; an unobtrusive empty state handles no matches. The result count is announced to assistive technology.

### Game tile

The whole tile is one link with a Play accessible name. Preview artwork falls back to its icon; games without either use a colored initial. Names sit near the top-left over a dark scrim. The lime play disc appears on hover/focus above 700px. The press state scales the card to .98. Original game imagery remains unchanged.

### Startup and transfer

Startup shows the selected game's artwork, name, short description and progress rail. The rail uses measured loaded/total bytes when the total is known; otherwise it remains indeterminate. Status changes to Starting game after transfer and startup remains visible until the first presented frame. Announcements are throttled to meaningful progress steps. The iframe receives focus when ready.

### Failure actions

A failed load replaces progress with concise error copy, a lime Try again button and a muted Back to games button. Retry receives focus and restarts that game. Return clears the game route; the library restores focus to the selected tile where available.

### Game return

The home icon stays available during loading and play. During play it follows the player's reported control theme and free-space layout, rather than imposing the web gallery theme on game controls. Returning preserves the search state and restores the prior tile's focus.

## Do's and Don'ts

### Do:
- **Do** retain the ink-violet, warm-white and acid-lime web identity.
- **Do** use the self-hosted Bricolage Grotesque font and keep its OFL license with the asset.
- **Do** preserve original game imagery, readable overlaid names, visible keyboard focus and safe-area spacing.
- **Do** keep search behind its icon and restore focus after returning from a game.
- **Do** show real transfer progress, then wait for the first presented frame before hiding startup.
- **Do** preserve native Android styling and gameplay control colors within their platform scope.

### Don't:
- **Don't** restore square artwork or permanently visible search as web rules.
- **Don't** replace the gallery with promotional sections, large introductions or technical settings.
- **Don't** invent timed progress percentages or dismiss startup on download completion alone.
- **Don't** apply the web palette or display font to original game presentation or native mobile controls.
