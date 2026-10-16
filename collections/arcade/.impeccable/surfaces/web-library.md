# Web library

Mode: Experience. Scope: responsive web library and game startup. Native Android
and original gameplay retain their platform behavior.

## THESIS

The game art is the arcade. Original square icons fill the screen and invite
immediate play. Names and real gameplay appear on hover or keyboard focus.

## OWN-WORLD

The user's revised CrazyGames and Poki references supersede the earlier Friv,
wide-card and glow direction. Retain the ink-violet ground, warm-white
self-hosted Bricolage type and acid-lime L/play identity. Original game art
provides the color. Library hover depth is neutral, without colorful glow.

## STORY

Browse the icon mosaic or open modal Search. Hover or focus reveals captured
gameplay, name, available sourced year and approximate first-load size. Choose a
game, read its premise during measured transfer, and enter on its first presented
frame. Home returns to the originating tile or restores Search with its query
and selected result. Failed transfer offers retry or return.

## FIRST VIEWPORT

The full-width mosaic begins at the safe-area edge. Its lime identity is a real
grid tile. Search and Full screen are adjacent half-size square tiles with
Shuffle directly beneath. Ordinary icons are square; Plants vs. Zombies, Sushi
Slash, Teclas and Ninjix are larger squares, randomized and distributed across
distinct rows. Names remain visually hidden at rest, including on phones.

## FORM

Code-led. Ordinary tiles span 2 by 2 microcells; featured tiles span 4 by 4.
The grid has two microtracks per ordinary column, with 8px phone gaps and 12px
desktop gaps. Safe-area minimum spacing is 8px on phones and 12px on desktop.
Game corners are 16px, reducing to 13px at 600px. The identity h1 sizes relative
to its tile container. There is no separate header or maximum-width wrapper.

Hover or keyboard focus expands a tile to 1.85 times its width and 1.12 times its
height, clamped to an 8px viewport margin. The icon becomes a footer badge beside
the name and available year/size. Real gameplay frames push-slide on a 3-second
cycle, with a static preview under reduced motion. Search is a native dialog
with keyboard focus, name matching, result metadata and retained query state.

## QUALITY BAR

No generated or approved decision comp exists. The user's pinned CrazyGames
and Poki references set the direction; source and actual rendered output verify
its implementation. Review captures live under ../review/: desktop.png,
mobile.png, user-480.png, hover.png and search.png. The desktop capture includes
the final Search contrast correction; earlier phone captures retain the previous
purple and do not define that token. DESIGN.md frontmatter records the current
Search rest and hover colors, whose white-text contrast is approximately 5.56:1
and 4.64:1. Player/loading acceptance still requires real transfer progress and
first-frame readiness. These web decisions do not redefine native Android.
