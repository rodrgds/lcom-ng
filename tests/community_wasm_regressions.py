#!/usr/bin/env python3
"""Student source boundaries that require the WASM32 ABI and linker wrapping."""
import argparse
import subprocess
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
FIXTURES = ROOT / "tests/fixtures/community/camoes"
PROJECT = ROOT / "examples/community/Ca-moes--LCOM/proj/src"


class CommunityWasmRegressions(unittest.TestCase):
    def compile_and_run(self, fixture, sources, source_flags=(), wraps=(), assets=(), no_inline=False):
        flags = ["emcc", "-std=gnu11", "-O2", "-flto", "-fcommon",
                 "-I" + str(ROOT / "sdk/compat/include"),
                 "-I" + str(ROOT / "sdk/include"), "-I" + str(PROJECT)]
        if no_inline:
            flags.append("-fno-inline")
        with tempfile.TemporaryDirectory(prefix="lcom-community-wasm-") as temporary:
            folder = Path(temporary)
            objects = []
            for index, (source, extra) in enumerate([
                    *((PROJECT / name, source_flags) for name in sources),
                    (FIXTURES / fixture, ())]):
                output = folder / f"{index}.bc"
                self.run_command([*flags, *extra, "-c", str(source), "-o", str(output)])
                objects.append(str(output))
            llvm_root = Path(subprocess.check_output(["em-config", "LLVM_ROOT"], text=True).strip())
            merged = folder / "merged.bc"
            self.run_command([str(llvm_root / "llvm-link"), *objects, "-o", str(merged)])
            binary = folder / "regression.cjs"
            asset_flags = [argument for source, destination in assets
                           for argument in ("--embed-file", str(PROJECT / source) + "@" + destination)]
            self.run_command([*flags, str(merged),
                              *("-Wl,--wrap=" + name for name in wraps), *asset_flags,
                              "-sENVIRONMENT=node", "-sSINGLE_FILE", "-o", str(binary)])
            self.run_command(["node", str(binary)])

    def run_command(self, command):
        result = subprocess.run(command, capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)

    def test_sentence_numeric_conversion_respects_allocation_and_integer_bounds(self):
        self.compile_and_run("conversion.c", ["sentences.c"], ["-Dmalloc=checked_malloc"])

    def test_sentence_cleanup_releases_only_owned_rows(self):
        self.compile_and_run("cleanup.c", ["sentences.c"],
                             ["-Dmalloc=checked_malloc", "-Dfree=checked_free"])

    def test_sentence_game_constructs_and_saves_terminated_ten_letter_names(self):
        self.compile_and_run("names.c", ["game.c", "highscores.c"],
                             ["-Dmalloc=checked_malloc"],
                             ["gameStateHandler", "get_highscores"], no_inline=True)

    def test_sentence_scores_keep_records_aligned_after_short_declared_name(self):
        self.compile_and_run("score-file.c", ["highscores.c"],
                             wraps=["lcom_compat_fopen"],
                             assets=[("highscores.txt", "/fixture.txt")])


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--project", type=Path, default=PROJECT,
                        help="original or repaired Sentence Sprint source directory")
    options, remaining = parser.parse_known_args()
    PROJECT = options.project.resolve()
    unittest.main(argv=[__file__, *remaining])
