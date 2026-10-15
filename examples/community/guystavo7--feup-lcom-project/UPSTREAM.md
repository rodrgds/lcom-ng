# Fruit Defense

Original student project: https://github.com/guystavo7/feup-lcom-project

Pinned revision: `ad0703bebf8ab8fd00ede7f690667e2034ca682d`.

Student source and required art are preserved byte for byte except explicitly recorded corrections below. lcom.json and icon.png are collection additions. Unrelated reports, generated documentation, build products and Git history are omitted.

License: No license file was supplied by this upstream revision; source availability does not grant redistribution permission.

## Confirmed source correction

Direct touch combines the destination and a click in one frame. Original gameStateUpdate consumed the click in state->act before applying accumulated cursor deltas, so tapping Play only hovered and could select an old-position action. The small SOURCE_PATCH.diff applies coordinates before game state input handlers while retaining the final cursor draw. A deterministic actual-input replay produced the black selector background before correction and the game field after.
