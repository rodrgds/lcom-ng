"""Check the displayed VRAM page through the actual runtime frame artifacts."""

from pathlib import Path
import sys


for name in sys.argv[1:]:
    data = Path(name).read_bytes()
    magic, resolution, maximum, pixels = data.split(b"\n", 3)
    assert (magic, resolution, maximum) == (b"P6", b"800 600", b"255"), name
    assert len(pixels) == 800 * 600 * 3, name
    # compat_client writes blue=0x12 on page 0 and blue=0xab on page 1,
    # selects page 1 with INT 10h 4f07, then restores text mode before exiting.
    assert pixels[:3] == bytes([0, 0, 0xAB]), (name, pixels[:3])
    assert pixels[3:] == bytes(len(pixels) - 3), name
