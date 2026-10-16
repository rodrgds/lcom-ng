# LCOM Arcade

A teacher can combine existing student game folders into one browsable showcase.
Students and visitors recognize an original game by its icon, play, and return to
choose another. Mobile and desktop browsers use a responsive, full-width web
mosaic. Android has an independent native launcher with adaptive, compact tiles.

The collection manifest owns identity and project folder paths. Each game's
lcom.json owns its title, icon, description, preview, assets and controls. Web
collection metadata adds sourced year labels, featured selection and captured
gameplay frames; packaging measures the approximate first-load download size.
Original game artwork and presentation stay intact. Unknown years are omitted;
Arcade years come from GitHub repository creation timestamps, recorded in the
catalog provenance.

The web library makes game art the arcade. Square icons fill the screen at rest;
hover or keyboard focus reveals captured gameplay, the name and available year
and download size, plus a public Repo link when available. The collection identity and Search, Full screen and Shuffle
controls belong to the mosaic. Search opens a modal with names and metadata.
Returning from a search-launched game restores the query and selected result.
Loading explains the game and shows real transfer progress until its first
presented frame. The home icon and a normal game exit return to the library.
