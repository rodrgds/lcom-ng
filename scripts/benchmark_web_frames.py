#!/usr/bin/env python3
"""Measure browser-host framebuffer conversion without changing guest timing."""
import argparse
import os
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[1]

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source-root', type=Path, default=ROOT, help='Runtime/SDK checkout to measure')
    parser.add_argument('--output', type=Path, default=ROOT/'build/web-frame-benchmark')
    parser.add_argument('--max-ms', type=float, help='Optional local conversion budget; never enabled in CI')
    parser.add_argument('--cpu-rate', type=float, default=1, help='Chrome CPU slowdown factor, at least 1')
    args = parser.parse_args()
    if args.cpu_rate < 1 or (args.max_ms is not None and args.max_ms <= 0):
        parser.error('CPU rate must be at least 1 and the optional budget must be positive')
    source, output = args.source_root.resolve(), args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    env = os.environ.copy()
    cache = Path(env.get('XDG_CACHE_HOME', Path.home()/'.cache'))/'lcom-ng/web/emscripten'
    env.setdefault('EM_CACHE', str(cache))
    def run(command):
        subprocess.run([str(value) for value in command], env=env, check=True)
    runtime = output/'runtime'
    run(['emcmake','cmake','-S',source/'sdk/web','-B',runtime,'-G','Ninja','-DCMAKE_BUILD_TYPE=Release'])
    run(['cmake','--build',runtime,'--parallel','4'])
    run(['em++','-O2','-std=c++17','-I',source/'runtime','-I',source/'sdk/include',
         ROOT/'tests/web_frame_benchmark.cpp',runtime/'libmachinelab.a',
         '--js-library',source/'sdk/web/library.js','-sASYNCIFY','-sALLOW_MEMORY_GROWTH',
         '-sSTACK_SIZE=2097152','-sMODULARIZE','-sEXPORT_ES6','-sENVIRONMENT=web',
         '-sEXPORTED_RUNTIME_METHODS=ccall','--no-entry','-o',output/'frames.js'])
    command = ['node',ROOT/'scripts/benchmark_web_frames.mjs',output,
               str(args.max_ms) if args.max_ms is not None else '', str(args.cpu_rate)]
    run(command)

if __name__ == '__main__':
    main()
