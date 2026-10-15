# Build a game collection

Combine a class's projects into one Android APK and a browser library. The native
Android menu adapts its columns to the screen and text size. The web menu adapts to phones, tablets and desktop
screens. Games keep their original presentation and their own mobile controls.
Android's **Find a game** field filters titles as you type and keeps the query
when you return from playing.

## Try the student arcade

From the repository checkout:

```sh
lcom-ng web collections/arcade
lcom-ng export web collections/arcade --output dist/lcom-arcade-web
lcom-ng export android collections/arcade --output dist/lcom-arcade-android
```

The web output is a static folder. Serve it over HTTP or HTTPS. The Android output
contains `collection.apk` and its relocatable Gradle project. Android prerequisites
and architecture switches are the same as [single-game exports](../students/android).
The included arcade has pinned student games under `examples/community/`.
Their catalog records upstream URLs, revisions, control choices and verification.

## Make your own collection

Create a folder with an `lcom.collection.json` file:

```json
{
  "version": 1,
  "name": "Our class arcade",
  "icon": "icon.png",
  "projects": [
    "../../students/alice/proj",
    "../../students/bob/src"
  ]
}
```

Paths are relative to the collection folder and point to each game's build folder,
usually the folder containing its Makefile and `lcom.json`. `name` and `icon` are
optional. When supplied, the icon must be a square PNG of at least 48 pixels.
Folders must be unique; nested collections are rejected. Moving the collection
and games together preserves the folder list and game identities.

Configure each game's name, icon, description, preview screenshot, source/assets and controls in its own
[`lcom.json`](../students/project-config). Both exporters reuse that profile.
Collection-wide `--name` and `--icon` override the launcher identity. Source,
asset, argument and orientation switches belong in the individual game profiles.

Re-exporting to the same web output removes generated bundles for games you took
out of the list. Files you added yourself are preserved.

The web library uses wide screenshot tiles with names over the artwork. Search
opens from the magnifying glass. Games without a `preview` use their icon.
A loading screen shows the game's `description` and download progress; failed
downloads offer a retry or a return to the library.

## Playing and returning

The mobile launcher fills the screen. On the web, tapping a game requests
fullscreen when the browser supports it; the player also fills the viewport
without that API. A **Games** button returns to the library. It occupies unused
letterboxing when space permits and avoids the game's input controls. Browser history
works too. Escape remains game input.

Android runs each game in a separate native process. **Games**, Android Back,
normal game exit and a game crash return to the existing launcher. Launching again
starts fresh game globals. Files written in that game's private storage survive
relaunch. Backgrounding pauses the embedded runtime.

Choose each game's touch behavior, movement pads and named action buttons in
its [`lcom.json`](../students/project-config). Board games can use a separate
control panel to keep every square touchable; local two-player games can give
each player their own pad and actions.

## Third-party projects

The repository's MIT license does not grant rights to the imported students'
source or artwork. Each game's `UPSTREAM.json` records its provenance and license
status; original notices are retained. Many public student repositories have no
explicit license. Obtain the authors' permission before redistributing those
projects or publishing a class APK or website.

Original student C and headers stay unchanged except recorded corrections for
reproduced defects. Each correction has a `SOURCE_PATCH.diff` and original and
patched hashes in `UPSTREAM.json`. Runtime adaptations live in the shared
compatibility SDK.
