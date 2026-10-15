# Gameplay previews

These are real game captures from the mobile gameplay review on 2026-10-15.
`sources.json` records each input capture, its hash and any crop. Images retain
original game pixels, resized to at most 800×600. The web library crops them to
wide tiles; Android gameplay and student source folders are unaffected.

To refresh a preview, capture the actual game's canvas, remove host controls and
letterboxing, and keep the filename stable. Put its relative path in that game's
`lcom.json` as `preview`. Keep its short gameplay `description` there too.
Do not manufacture gameplay imagery or modify imported student assets.
