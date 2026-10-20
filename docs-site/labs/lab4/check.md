# Lab 4 check

Run the mouse tests with deterministic scripts before debugging movement
visually. Packet failures can look like random cursor motion, so verify byte
assembly, synchronization, and sign conversion with fixed input first.

```sh
lcom-ng test mouse
lcom-ng run --headless --script scripts/mouse_move.mlabscript -- build/examples/mouse_packet
```

## Discussion prompts

- Why does packet byte 0 include a constant synchronization bit?
- How would you implement drag detection from button state and deltas?
- Which helpers would a drawing program need before using the mouse library?

## External reading

The LCOM [mouse introduction](https://pages.up.pt/~up748353/classes/lcom/lab-guides/lab4/intro.html)
and [mouse packet chapter](https://pages.up.pt/~up748353/classes/lcom/lab-guides/lab4/reading-mouse-packets.html)
are useful companions. For protocol details, read OSDev on
[mouse input](https://wiki.osdev.org/Mouse_Input) and the
[PS/2 controller](https://wiki.osdev.org/%228042%22_PS/2_Controller), plus Adam
Chapweske's archived
[PS/2 Mouse Interface](https://web.archive.org/web/20180102104023/http://www.computer-engineering.org/ps2mouse/).
