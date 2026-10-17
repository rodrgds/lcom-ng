"""Check logical row pitch and both adjacent pages through real runtime hosts."""
from pathlib import Path
import subprocess
import sys
runtime, client, output = sys.argv[1:]
output = Path(output)
output.mkdir(parents=True, exist_ok=True)
def run(*args):
    subprocess.run([runtime, *map(str, args)], check=True)
def check(name, left=False):
    data = (output / name).read_bytes()
    magic, resolution, maximum, pixels = data.split(b"\n", 3)
    assert (magic, resolution, maximum) == (b"P6", b"800 600", b"255")
    assert len(pixels) == 800 * 600 * 3
    for row in range(600):
        color = [0xbb, 0, 0] if left else [0, 0xaa if row % 2 else 0xee, 0x33]
        assert pixels[row * 2400:(row + 1) * 2400] == bytes(color) * 800, (name, row)
run("run", "--headless", "--max-ticks", "100", "--dump-frame", output / "single.ppm", "--", client)
check("single.ppm")
run("run-pair", "--headless", "--max-ticks", "100", "--left-dump-frame", output / "left.ppm",
    "--right-dump-frame", output / "right.ppm", client, "left", "--right", client)
check("left.ppm", left=True)
check("right.ppm")
