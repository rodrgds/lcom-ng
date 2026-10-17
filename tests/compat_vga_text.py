"""Exercise libvt ABI and shared text mappings through both desktop hosts."""
from pathlib import Path
import subprocess
import sys
runtime, client, destination = sys.argv[1:]
output = Path(destination)
output.mkdir(parents=True, exist_ok=True)
def run(*args):
    subprocess.run([runtime, *map(str, args)], check=True, timeout=10)
def check(name):
    magic, resolution, maximum, pixels = (output / name).read_bytes().split(b"\n", 3)
    assert (magic, resolution, maximum) == (b"P6", b"720 400", b"255")
    assert len(pixels) == 720 * 400 * 3
    def rgb(x, y):
        at = (y * 720 + x) * 3
        return tuple(pixels[at:at + 3])
    assert rgb(0, 0) == rgb(8, 15) == (255, 255, 85)
    assert rgb(9, 0) == (0, 0, 170)
    assert rgb(719, 399) == (170, 0, 0)
run("run", "--headless", "--timeout", "5", "--dump-frame", output / "single.ppm", "--", client)
check("single.ppm")
run("run-pair", "--headless", "--timeout", "5", "--left-dump-frame", output / "left.ppm",
    "--right-dump-frame", output / "right.ppm", client, "--right", client)
check("left.ppm")
check("right.ppm")
