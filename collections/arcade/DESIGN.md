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
  loading-gap: "16px"
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
weights 200 to 800 with font-display swap. The launcher loads a Latin and
punctuation WOFF2 subset. Preserve its adjacent OFL license and original TTF.

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

The packer divides each ordinary column into two square microtracks, with three
ordinary columns on phones and up to eighteen on desktop. Gaps are 8px on phones
and 12px otherwise. Ordinary icons and the identity occupy 2 by 2 microcells.
Search, Screen, Shuffle and Surprise form a two-by-two control tile. Filters
occupies a separate 2 by 2 tile, or a full-width phone row.
Featured tiles occupy 4 by 4 microcells.

Each visit and Shuffle selects ten highlights from the source-reviewed
28-game `featuredPool`, including Plants vs. Zombies, Sushi Slash, Teclas, Ninjix and Thin Ice. Picks remain stable on resize. The first two
normal-tile rows contain only tools and smaller games. Filtered results use
ordinary tiles without featured promotion or archetype clusters. The DOM order
follows visual reading order after packing.

Startup opens a compact native modal over the library, with a small selected-game
icon, title, description and measured transfer progress. Cancel restores focus
to the originating tile. Opening and closing use short opacity/scale transitions;
reduced motion removes them. The first presented frame replaces the modal
immediately, with no artificial minimum delay.

The game iframe fills the viewport. The home button uses reported game and
control bounds to prefer free space and letterboxing, respecting safe areas.
Native Android retains its separate compact adaptive grid and player behavior.

## Elevation & Depth

The mosaic is flat at rest. An expanded preview uses a neutral black shadow to
separate it from adjacent icons, without a colorful ambient field or card glow.
A dark lower scrim protects preview metadata. Startup artwork retains its
existing soft violet shadow; that treatment is scoped to startup.

Library previews retain a 1.514:1 aspect ratio. Normal and compact tiles share
the normal tile height; larger featured tiles use their own height. Previews
center on their source tile, remain above search and utility cards, and clamp
inside an 8px viewport margin. Its geometry transitions over
320ms using cubic-bezier(.16, 1, .3, 1). Captured frames push horizontally every
3 seconds, with a 520ms slide and a 560ms completion window. Reduced motion
removes transitions, animations and automatic frame cycling while retaining the
static preview and metadata. Shuffle and filtering move visible cards over 320ms.
New matches fade and scale into place; removed icons leave over 160ms as inert
copies. Interrupted movement is canceled, and equivalent result sets stay still.

**The Quiet Depth Rule.** Use neutral elevation for library previews; do not restore colorful hover glow.

## Shapes

Game and utility tiles use the tile radius, switching to the mobile-tile radius
at 600px. Square icon crops remain square at both ordinary and featured sizes.
Expanded previews keep the same rounded boundary. The original icon becomes a
34px footer badge with 8px corners. The play disc is circular. Search uses a
native dialog with an inset field, separate from the Filters tile. Failure actions use the action radius.

## Components

### Identity and utility tiles

The acid-lime tile combines the L/play mark and collection h1 inside the grid.
Screen and Shuffle each use a compact SVG and label; Surprise sits beneath
both. All controls have accessible names. Focus uses a 3px lime outline with a
4px offset. Fine-pointer interaction uses the supplied cursor artwork.

### Game icon and gameplay preview

Each tile has a Play link with an accessible name and available year/size
facts. Only its launcher icon is visible at rest; missing artwork falls back to
the collection mark. Hover with a fine pointer or keyboard focus opens the wider
preview. Normal and compact tiles share the normal tile height. Featured
previews retain their own larger height. All share the landscape aspect ratio
and center on the source tile within viewport or gutter bounds.
The icon moves into the footer beside the name and available facts;
Arcade school-year labels use original README/reports or game development
history. School-year labels stay plain, with inference confidence in their
evidence tooltip; unknown years stay hidden. Repository creation timestamps remain provenance. Public GitHub projects add an icon-only GitHub repository link
after those facts, with a named 44px hit target. It opens in a separate tab and is a sibling of the Play link,
so following a repository never launches a game. Authored replacement icons are
documented in `icons/generation.json`; original in-game artwork stays unchanged.

A lime play disc and segmented frame timer appear above. Gameplay frames are
real captures, contained without cropping. Multiple frames push-slide on a
2.4-second cycle. A 32-pixel embedded screenshot supplies the first frame without
a network round trip. After launcher icons decode, up to eight visible thumbnails
queue for low-priority warming, with two downloads at a time. Only fine-pointer
browsing warms them; Data Saver and 2G connections skip this work. The decoded
160-pixel thumbnail cache retains at most 48 images. Sharp gallery frames load on
interaction, with only the next frame warmed while a preview stays open.
Escape, blur, scroll, resize and leaving the page stop the preview.
Closing releases screenshot elements and cached slide references after the closing
transition; progress animations run only while a preview is open. Touch activates
the game directly.

### Library filters and side suggestions

A dedicated Filters tile opens one floating panel with genres, player modes
and school-year buttons. Counts respond to the other groups. Choices within a
group use OR; groups combine with AND. Clear resets all choices. Filtering,
shuffle and Surprise use the same subset. Search looks up individual games
across the complete catalog and never changes the mosaic. Header and footer stay visible
while the choices scroll on short screens. The panel returns focus when closed.

During play, suggestions use only reported empty side gutters, avoiding controls.
Each gutter fits one to four columns and hides when icons cannot fit at 64px.
Columns loop slowly in alternating vertical directions using CSS transforms and
a freshly randomized deck of at most 48 thumbnails from the complete catalog
on each launch, including after a narrow search. Hover or
keyboard focus pauses that side. Reduced motion disables drift.
Resting opacity is .78 and hover/focus restores full brightness. Previews remain
inside the gutter, with ordinary links to launch. Narrow gutters show screenshots
within the tile and expose its name as a tooltip instead of squeezing preview copy.

### Search dialog

Search opens the original native dialog for finding and launching an individual
game by name. Ctrl+K, Command+K and slash open it without intercepting guest input.
Enter launches the first result; Arrow Down moves into the result list. Escape
or clicking the backdrop dismisses it and restores the opener. Returning from a
searched game reopens the dialog with the query and selected result focused.
Search is independent of the mosaic's facets. There is no active-filter bar above
the grid. Facet selections persist across reloads in the same tab; clear controls
live inside the filter panel. Visible cards animate when filtering changes the
mosaic, and clearing restores its highlights and variant groups.
Names are indexed once, result nodes are reused, icons use lazy thumbnails, and
preview markup is created only on interaction.

The hosted launcher is bundled into one content-hashed script. Its versioned
catalog is preloaded alongside the script. Hosted scripts, styles, fonts, icons
and screenshots use content-hashed files under `static/` with immutable browser
caching. The HTML keeps revalidation so a new release selects fresh asset URLs.

### Startup and transfer

Startup shows a compact selected-game icon, name, short description and progress
rail in a modal. Measured loaded/total bytes drive progress when the total is known;
otherwise the rail is indeterminate. Starting game follows transfer. Startup
stays visible and the iframe inert until the first presented frame, then the
iframe receives focus. Progress announcements use meaningful steps.

### Failure and return

A failed load replaces progress with concise error copy, Try again and Back to
games. Retry receives focus and restarts the same game. Loading has a close
button. During play, the plain Home button follows reported control colors and
free-space layout. Shift+Escape releases capture first; another press returns
to the library. Fullscreen Keyboard Lock retains ordinary Escape for the game
where the browser supports it. Returning restores the originating tile or search
result focus and retains search state. A Shift/Esc hint appears at bottom center
after one second of mouse capture, fades over four seconds, and cancels on unlock
or navigation. Home remains visible for every input method.

## Do's and Don'ts

### Do:
- **Do** retain the ink-violet, warm-white and acid-lime identity with the self-hosted Bricolage font and its license.
- **Do** preserve original square icons at rest and use real gameplay captures for interactive previews.
- **Do** keep identity, Search, Full screen and Shuffle inside the full-width mosaic.
- **Do** preserve visible keyboard focus, separate search and filters, query restoration and safe-area spacing.
- **Do** show real transfer progress and wait for the first presented frame before hiding startup.
- **Do** preserve native Android styling and original game presentation within their platform scope.

### Don't:
- **Don't** restore a separate masthead, wide screenshot cards or permanent name overlays in the resting library.
- **Don't** add colorful library hover glow, promotional sections or technical settings to the game selection flow.
- **Don't** invent creation years, download sizes or timed progress percentages.
- **Don't** dismiss startup on download completion alone or apply the web identity to original game controls.
