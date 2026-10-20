#!/usr/bin/env python3
"""Upload the generated Android catalog and game bundles to the Arcade R2 bucket."""
import argparse
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
WRANGLER = ROOT / 'node_modules/.bin/wrangler'
if not WRANGLER.is_file():
    WRANGLER = 'wrangler'


def content_type(path):
    if path.suffix == '.json': return 'application/json'
    if path.suffix == '.png': return 'image/png'
    return 'application/octet-stream'


def upload(source, bucket, path):
    key = path.relative_to(source).as_posix()
    command = [str(WRANGLER), 'r2', 'object', 'put', f'{bucket}/{key}', '--file', str(path),
               '--content-type', content_type(path), '--remote']
    result = subprocess.run(command, cwd=source.parent, capture_output=True, text=True)
    if result.returncode:
        raise RuntimeError(result.stderr.strip() or result.stdout.strip())
    return key


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, default=Path('build/android-publish'))
    parser.add_argument('--bucket', default='lcom-arcade-android')
    parser.add_argument('--workers', type=int, default=8)
    args = parser.parse_args()
    source = args.source.resolve()
    if not (source / 'catalog.json').is_file():
        parser.error('source has no catalog.json')
    files = sorted(path for path in source.rglob('*') if path.is_file())
    failed = []
    with ThreadPoolExecutor(max_workers=args.workers) as pool:
        jobs = {pool.submit(upload, source, args.bucket, path): path for path in files}
        for job in as_completed(jobs):
            try:
                print('uploaded', job.result(), flush=True)
            except Exception as error:
                failed.append((jobs[job], error))
                print('failed', jobs[job], error, file=sys.stderr, flush=True)
    if failed:
        raise SystemExit(f'{len(failed)} uploads failed')
    print(f'uploaded {len(files)} files to {args.bucket}')


if __name__ == '__main__':
    main()
