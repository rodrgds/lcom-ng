---
name: LCOM Arcade Web
description: Original square game icons form a full-width playable mosaic.
colors:
  ink-violet: "#101021"
  warm-white: "#fff8ed"
  acid-lime: "#c5ff62"
  identity-ink: "#17182c"
  muted-lilac: "#bcb9d5"
  utility: "#30294a"
  utility-hover: "#463b68"
  search-tile: "#7359af"
  search-tile-hover: "#8066ba"
  search-tile-text: "#ffffff"
  shuffle: "#27243b"
  shuffle-hover: "#39324f"
  search-surface: "#1e1c32"
  search-field: "#111022"
  search-result-hover: "#322c4a"
  progress-track: "#34304c"
  secondary-action: "#302b48"
  library-error: "#ffb9b3"
  loading-error: "#ffd0c8"
typography:
  identity:
    fontFamily: "Bricolage, system-ui, sans-serif"
    fontSize: "clamp(25px, 26cqw, 48px)"
    fontWeight: 800
    lineHeight: 0.94
    letterSpacing: "-0.035em"
  display:
    fontFamily: "Bricolage, system-ui, sans-serif"
    fontSize: "clamp(26px, 4vw, 44px)"
    fontWeight: 800
    lineHeight: 1.05
    letterSpacing: "-0.035em"
  title:
    fontFamily: "Bricolage, system-ui, sans-serif"
    fontSize: "clamp(17px, 1.45vw, 25px)"
    fontWeight: 800
    lineHeight: 1.05
    letterSpacing: "-0.02em"
  body:
    fontFamily: "Bricolage, system-ui, sans-serif"
    fontSize: "16px"
    lineHeight: 1.5
  utility:
    fontFamily: "Bricolage, system-ui, sans-serif"
    fontSize: "12px"
    fontWeight: 650
  facts:
    fontFamily: "Bricolage, system-ui, sans-serif"
    fontSize: "12px"
    fontWeight: 550
  status:
    fontFamily: "Bricolage, system-ui, sans-serif"
    fontSize: "13px"
rounded:
  progress: "8px"
  action: "10px"
  field: "12px"
  mobile-tile: "13px"
  tile: "16px"
  dialog: "20px"
spacing:
  phone: "8px"
  desktop: "12px"
  field-inline: "14px"
  dialog: "24px"
  loading-gap: "44px"
components:
  identity-tile:
    backgroundColor: "{colors.acid-lime}"
    textColor: "{colors.identity-ink}"
    typography: "{typography.identity}"
    rounded: "{rounded.tile}"
    padding: "clamp(12px, 1.3vw, 22px)"
  utility-tile:
    backgroundColor: "{colors.utility}"
    textColor: "{colors.warm-white}"
    rounded: "{rounded.tile}"
    typography: "{typography.utility}"
  utility-tile-hover:
    backgroundColor: "{colors.utility-hover}"
  search-tile:
    backgroundColor: "{colors.search-tile}"
    textColor: "{colors.search-tile-text}"
    rounded: "{rounded.tile}"
  search-tile-hover:
    backgroundColor: "{colors.search-tile-hover}"
  shuffle-tile:
    backgroundColor: "{colors.shuffle}"
    textColor: "{colors.warm-white}"
    rounded: "{rounded.tile}"
  shuffle-tile-hover:
    backgroundColor: "{colors.shuffle-hover}"
  search-field:
    backgroundColor: "{colors.search-field}"
    textColor: "{colors.warm-white}"
    rounded: "{rounded.field}"
    height: "54px"
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

**Creative North Star: "The game art is the arcade"**

Original square icons supply the color and identity of a dense, screen-filling
mosaic. Ink-violet, warm Bricolage lettering and the acid-lime identity tile frame
the games. Hover or keyboard focus opens a wider window into real gameplay.
The user's CrazyGames and Poki references establish this direction.

This system covers the web library and startup screen in
sdk/collection/web-template/. Native Android keeps its independent black, white
and mint palette, adaptive compact launcher and platform controls. Original games
retain their own art, type and control themes.

**Key Characteristics:**
- Original square game icons at rest.
- Identity and compact tools belong to the mosaic.
- Captured gameplay and metadata appear on hover or keyboard focus.
- Neutral preview depth, with no colorful library glow.
- Real transfer progress followed by the first presented frame.

## Colors

Acid-lime identifies the collection and active affordances. Ink-violet and warm
white frame the original artwork. Frontmatter tokens are normative.

### Primary

- **Acid Lime:** identity tile, play disc, focus outline, text selection and measured download fill.

### Secondary

- **Search Tile / Search Tile Hover:** the distinct purple Search control uses pure-white type. Its rest and hover combinations provide approximately 5.56:1 and 4.64:1 contrast respectively.
- **Utility / Utility Hover / Shuffle / Shuffle Hover:** subdued purple utility tiles remain subordinate to game art.

### Neutral

- **Ink Violet / Warm White / Identity Ink:** gallery ground, primary copy and dark type on lime.
- **Muted Lilac:** descriptions, transfer status and supporting search copy.
- **Search Surface / Search Field / Search Result Hover:** dialog, inset input and result interaction surface.
- **Progress Track / Secondary Action:** startup rail and return action after a failed load.
- **Library Error / Loading Error:** readable errors in their corresponding dark surfaces.

**The Artwork Rule.** Keep original game icons visible at rest; reveal captured gameplay and names through interaction rather than permanent overlays.

## Typography

**Display and Body Font:** self-hosted Bricolage Grotesque, registered as
`Bricolage`, with system-ui and sans-serif fallbacks. The variable font supports
weights 200 to 800 with font-display swap. Preserve its adjacent OFL license.

- **Identity:** container-relative sizing follows the actual tile, with balanced wrapping and tight leading. It remains a real h1.
- **Loading display:** the display role becomes 30px in stacked mobile and short landscape layouts.
- **Preview title:** the title role becomes 18px at the phone breakpoint; names remain hidden visually at rest while accessible link names remain available.
- **Body:** descriptions reduce to 15px on phones.
- **Utility:** compact icon labels reduce to 11px on phones. Shuffle uses 14px on desktop and 12px on phones.
- **Facts and status:** tabular numerals support year and size comparisons; preview facts reduce to 11px on phones.

## Layout

The library fills the available width and has at least one small-viewport screen
of height. There is no centered maximum-width wrapper or separate header. Safe
area insets are respected with desktop and phone minimum edge spacing from the
frontmatter.

The packer divides each ordinary column into two square microtracks. It chooses
2 to 14 ordinary columns from the available grid width, using a 142px target
below 600px grid width and 168px otherwise. Microtrack gaps are 8px below that
threshold and 12px otherwise. An ordinary icon and the identity each occupy
2 by 2 microcells. Search and Full screen are adjacent 1 by 1 squares; Shuffle
occupies the 2 by 1 rectangle beneath them. Featured tiles occupy 4 by 4
microcells on desktop and phones alike.

Plants vs. Zombies, Sushi Slash, Teclas and Ninjix are featured. Initial layout
and Shuffle randomize game order. Featured starts are distributed across distinct
rows and alternate across the board; ordinary icons fill the remaining space.
The DOM order follows visual reading order after packing.

Startup centers artwork and copy side by side with the loading gap. Artwork is
at most 420px wide with a 4:3 crop; copy is at most 380px wide. At 600px and below
it stacks with a 28px gap and a 16:10 crop at min(66vw, 320px). At heights up to
500px and widths from 600px it uses a compact horizontal layout. Loading scrolls
when needed.

The game iframe fills the viewport. The home button uses reported game and
control bounds to prefer free space and letterboxing, respecting safe areas.
Native Android retains its separate compact adaptive grid and player behavior.

## Elevation & Depth

The mosaic is flat at rest. An expanded preview uses a neutral black shadow to
separate it from adjacent icons, without a colorful ambient field or card glow.
A dark lower scrim protects preview metadata. Startup artwork retains its
existing soft violet shadow; that treatment is scoped to startup.

A preview grows to 1.85 times the original width and 1.12 times its height,
clamped inside the viewport with an 8px margin. Its geometry transitions over
320ms using cubic-bezier(.16, 1, .3, 1). Captured frames push horizontally every
3 seconds, with a 520ms slide and a 560ms completion window. Reduced motion
removes transitions, animations and automatic frame cycling while retaining the
static preview and metadata. Shuffle movement takes 540ms when motion is enabled.

**The Quiet Depth Rule.** Use neutral elevation for library previews; do not restore colorful hover glow.

## Shapes

Game and utility tiles use the tile radius, switching to the mobile-tile radius
at 600px. Square icon crops remain square at both ordinary and featured sizes.
Expanded previews keep the same rounded boundary. The original icon becomes a
34px footer badge with 8px corners. The play disc is circular. Search uses a
rounded dialog, reducing to the tile radius on phones; its input and result rows
use the field radius. Failure actions use the action radius.

## Components

### Identity and utility tiles

The acid-lime tile combines the L/play mark and collection h1 inside the grid.
Search and Full screen each use a compact SVG and label; Shuffle sits beneath
both. All controls have accessible names. Focus uses a 3px lime outline with a
4px offset. Fine-pointer interaction uses the supplied cursor artwork.

### Game icon and gameplay preview

Each tile has a Play link with an accessible name and available year/size
facts. Only its launcher icon is visible at rest; missing artwork falls back to
the collection mark. Hover with a fine pointer or keyboard focus opens the wider
preview. The icon moves into the footer beside the name and available facts;
Arcade year labels use GitHub repository creation years. Public projects add an underlined Repo link
after those facts. It opens in a separate tab and is a sibling of the Play link,
so following a repository never launches a game. Authored replacement icons are
documented in `icons/generation.json`; original in-game artwork stays unchanged.

A lime play disc and segmented frame timer appear above. Gameplay frames are
real captures, contained without cropping. Multiple frames push-slide on a
3-second cycle. Escape, blur, scroll, resize and leaving the page stop the preview.
Touch activates the game directly.

### Search dialog

Search opens a native modal dialog. The input receives focus; Escape, Close and
clicking the backdrop dismiss it. The slash shortcut opens search from the
library. Results show original icons, names, sourced year labels and approximate
first-load sizes. Name matching is case-insensitive, counts are announced, and
an empty result message suggests another name. Arrow Down moves from input to
results; Arrow Up from the first result returns to input. Enter launches the
first result. Closing preserves the query and returns focus to the opener.
Returning from a search-launched game reopens the dialog with its query and
selected result focused.

### Startup and transfer

Startup shows the selected game's artwork, name, short description and progress
rail. Measured loaded/total bytes drive progress when the total is known;
otherwise the rail is indeterminate. Starting game follows transfer. Startup
stays visible and the iframe inert until the first presented frame, then the
iframe receives focus. Progress announcements use meaningful steps.

### Failure and return

A failed load replaces progress with concise error copy, Try again and Back to
games. Retry receives focus and restarts the same game. The home icon remains
available during loading and play. During play it follows reported control
colors and free-space layout. Returning restores the originating tile or search
result focus and retains search state.

## Do's and Don'ts

### Do:
- **Do** retain the ink-violet, warm-white and acid-lime identity with the self-hosted Bricolage font and its license.
- **Do** preserve original square icons at rest and use real gameplay captures for interactive previews.
- **Do** keep identity, Search, Full screen and Shuffle inside the full-width mosaic.
- **Do** preserve visible keyboard focus, modal search behavior, query restoration and safe-area spacing.
- **Do** show real transfer progress and wait for the first presented frame before hiding startup.
- **Do** preserve native Android styling and original game presentation within their platform scope.

### Don't:
- **Don't** restore a separate masthead, wide screenshot cards or permanent name overlays in the resting library.
- **Don't** add colorful library hover glow, promotional sections or technical settings to the game selection flow.
- **Don't** invent creation years, download sizes or timed progress percentages.
- **Don't** dismiss startup on download completion alone or apply the web identity to original game controls.
