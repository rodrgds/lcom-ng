#!/usr/bin/env python3
"""Verify pinned collection files and reverse every documented student patch."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'sdk'))
from collection import manifest


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def check(folder):
    entries = manifest.load(folder)['entries']
    fingerprints = {}
    total_files = patched_files = patched_games = 0
    for entry in entries:
        project = entry['project']
        root = next((path for path in (project, *project.parents)
                     if (path / 'UPSTREAM.json').is_file()), None)
        if root is None:
            raise ValueError(f'{project}: missing UPSTREAM.json')
        provenance = json.loads((root / 'UPSTREAM.json').read_text())
        hashes = provenance.get('sourceHashes', provenance.get('sha256'))
        if not hashes:
            raise ValueError(f'{root}: missing original file hashes')
        changes = {}
        for patch in provenance.get('sourceChanges', provenance.get('sourcePatches', [])):
            for change in patch.get('files', [patch]):
                name = change.get('file', change.get('path'))
                original = change.get('originalSha256', change.get('upstreamSha256'))
                if not name or original != hashes.get(name) or not change.get('patchedSha256'):
                    raise ValueError(f'{root}: incomplete original/patched hash record: {name}')
                if name in changes:
                    raise ValueError(f'{root}: duplicate patch record: {name}')
                changes[name] = change
        for name, original in hashes.items():
            path = root / name
            if not path.resolve().is_relative_to(root.resolve()) or not path.is_file():
                raise ValueError(f'{root}: missing or invalid pinned file: {name}')
            expected = changes[name]['patchedSha256'] if name in changes else original
            if digest(path) != expected:
                raise ValueError(f'{root}: unrecorded file change: {name}')
        for asset in provenance.get('derivedAssets', []):
            path = root / asset['path']
            if not path.resolve().is_relative_to(root.resolve()) or not path.is_file():
                raise ValueError(f'{root}: missing or invalid derived asset: {path}')
            if 'copyOf' in asset:
                expected = hashes.get(asset['copyOf'])
            elif asset.get('kind') == 'empty runtime data file':
                expected = hashlib.sha256(b'').hexdigest()
            else:
                expected = asset.get('sha256')
            if not expected or digest(path) != expected or asset.get('sha256', expected) != expected:
                raise ValueError(f'{root}: derived asset differs from its recorded source: {path}')
        code = sorted(path for path in root.rglob('*') if path.is_file()
                      and path.suffix.lower() in ('.c', '.h', '.cpp', '.hpp'))
        for path in code:
            if path.relative_to(root).as_posix() not in hashes:
                raise ValueError(f'{root}: source has no pinned hash: {path}')
        fingerprint = hashlib.sha256(''.join(sorted(digest(path) for path in code)).encode()).hexdigest()
        if fingerprint in fingerprints:
            raise ValueError(f'Duplicate source set: {root} and {fingerprints[fingerprint]}')
        fingerprints[fingerprint] = root
        if changes:
            patch = root / 'SOURCE_PATCH.diff'
            if not patch.is_file():
                raise ValueError(f'{root}: missing SOURCE_PATCH.diff')
            with tempfile.TemporaryDirectory(prefix='lcom-source-patch-') as temporary:
                copy = Path(temporary)
                for name in changes:
                    target = copy / name
                    target.parent.mkdir(parents=True, exist_ok=True)
                    shutil.copy2(root / name, target)
                result = subprocess.run(['git', 'apply', '--reverse', str(patch.resolve())],
                                        cwd=copy, capture_output=True, text=True)
                if result.returncode:
                    raise ValueError(f'{root}: patch cannot be reversed: {result.stderr.strip()}')
                for name in changes:
                    if digest(copy / name) != hashes[name]:
                        raise ValueError(f'{root}: reversed patch differs from pinned original: {name}')
            patched_games += 1
            patched_files += len(changes)
        total_files += len(hashes)
    print(f'{len(entries)} distinct games, {total_files} pinned files verified; '
          f'{patched_games} documented source corrections across {patched_files} files.')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('collection', type=Path, nargs='?', default=ROOT / 'collections/arcade')
    try:
        check(parser.parse_args().collection)
    except (ValueError, OSError, KeyError) as error:
        sys.exit(str(error))
