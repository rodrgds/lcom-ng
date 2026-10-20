# Notes

A plain text editor with real files, selection, clipboard and edit history. Save
state and the file path stay visible.

## Run it

Build examples with `devenv shell -- lcom-ng-build`, then:

```sh
lcom-ng run -- build/examples/word_processor build/notes.txt
```

## Controls

| Control | Action |
| --- | --- |
| Cmd/Ctrl+S or F2 | Save the document |
| Cmd/Ctrl+Z / Cmd/Ctrl+Shift+Z (Ctrl+Y) | Undo / redo |
| Cmd/Ctrl+A / C / X / V | Select all / copy / cut / paste |
| Arrows / Shift+arrows | Move / extend selection |
| Mouse click / drag | Place caret / select text |
| Home / End / Page Up / Down | Navigate document |
| F1 | Shortcuts |
| Esc / Cmd/Ctrl+W | Close, with save prompt for unsaved work |

## In the app

<ExampleGallery app="word-processor" />

## What it demonstrates

PS/2 input, text selection, bounded document storage, edit snapshots, atomic file
replacement and framebuffer rendering. Notes supports printable ASCII and line
breaks up to 4 KB. Host clipboard text must fit the document and supported
character set.

Source: [examples/word_processor.c](https://github.com/rodrgds/lcom-ng/blob/main/examples/word_processor.c).

Refresh captures with the [screenshot workflow](./screenshots).
