#!/usr/bin/env python3
"""Build per-game Android native bundles and a catalog for on-demand download."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import sys
import tempfile
import zipfile

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'sdk'))
from collection import android, manifest

ABI_NAMES = ('arm64-v8a', 'x86_64', 'armeabi-v7a')


def digest(data):
    return hashlib.sha256(data).hexdigest()


def zip_digest(entries):
    h = hashlib.sha256()
    for name, data in sorted(entries):
        h.update(name.encode())
        h.update(b'\0')
        h.update(data)
        h.update(b'\0')
    return h.hexdigest()


def build_apk(collection, output, entries, abis):
    android.build(entries, output, collection['name'], None, 'pt.rgo.lcomng.catalog', abis, project_only=False)
    return output / 'collection.apk'


def bundle_game(apk, game, abi, catalog_entry, out_dir, base_url):
    library = game['library']
    so_name = f'lib/{abi}/lib{library}.so'
    with zipfile.ZipFile(apk) as archive:
        so = archive.read(so_name)
        assets = []
        prefix = f'assets/games/{game["id"]}/'
        for name in archive.namelist():
            if not name.startswith(prefix) or name.endswith('/'):
                continue
            rel = name[len(prefix):]
            assets.append((f'assets/{rel}', archive.read(name)))
        icon_data = None
        icon_name = game.get('icon')
        if icon_name:
            try: icon_data = archive.read('assets/' + icon_name)
            except KeyError: icon_data = None
    entries = [(f'lib/{abi}/lib{library}.so', so), *assets]
    version = zip_digest(entries)[:16]
    manifest_data = {
        'format': 1,
        'id': game['id'],
        'name': game['name'],
        'library': library,
        'abi': abi,
        'version': version,
        'orientation': game.get('orientation') or 'auto',
        'archetype': game.get('archetype'),
    }
    entries.append(('manifest.json', json.dumps(manifest_data, indent=2).encode()))
    game_dir = out_dir / 'games' / game['id']
    game_dir.mkdir(parents=True, exist_ok=True)
    zip_name = f'{version}-{abi}.zip'
    target = game_dir / zip_name
    with zipfile.ZipFile(target, 'w', zipfile.ZIP_DEFLATED, compresslevel=9) as archive:
        for name, data in entries:
            archive.writestr(name, data)
    data = target.read_bytes()
    sha = digest(data)
    if icon_data:
        icon_dir = out_dir / 'icons'
        icon_dir.mkdir(exist_ok=True)
        (icon_dir / f'{game["id"]}.png').write_bytes(icon_data)
        catalog_entry['icon'] = f'icons/{game["id"]}.png'
    catalog_entry.setdefault('abis', {})[abi] = {
        'url': f'{base_url}/games/{game["id"]}/{zip_name}',
        'bytes': len(data),
        'sha256': sha,
        'version': version,
    }
    catalog_entry['version'] = version
    catalog_entry['bytes'] = len(data)
    catalog_entry['sha256'] = sha
    return {'id': game['id'], 'version': version, 'bytes': len(data), 'sha256': sha, 'path': target}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--collection', type=Path, default=ROOT / 'collections/arcade')
    parser.add_argument('--output', type=Path, default=ROOT / 'build/android-publish')
    parser.add_argument('--apk', type=Path)
    parser.add_argument('--build-apk', action='store_true')
    parser.add_argument('--ids', nargs='*')
    parser.add_argument('--abis', nargs='+', default=['arm64-v8a'])
    parser.add_argument('--base-url', default='https://arcade.rgo.pt/android')
    args = parser.parse_args()
    args.output = args.output.resolve()
    if args.output.exists():
        shutil.rmtree(args.output)
    args.output.mkdir(parents=True)
    collection = manifest.load(args.collection.resolve())
    entries = collection['entries']
    if args.ids:
        wanted = set(args.ids)
        entries = [e for e in entries if e['id'] in wanted]
        missing = wanted - {e['id'] for e in entries}
        if missing: parser.error(f'unknown game ids: {sorted(missing)}')
    if any(abi not in ABI_NAMES for abi in args.abis):
        parser.error(f'abis must be one of {", ".join(ABI_NAMES)}')
    if not entries:
        parser.error('no games selected')
    if args.apk is None:
        args.apk = build_apk(collection, ROOT / 'build/android-apk', entries, args.abis)
    elif args.build_apk:
        args.apk = build_apk(collection, ROOT / 'build/android-apk', entries, args.abis)
    else:
        args.apk = args.apk.resolve()
    if not args.apk.is_file():
        parser.error(f'APK not found: {args.apk}')
    with zipfile.ZipFile(args.apk) as archive:
        catalog_games = json.loads(archive.read('assets/collection.json'))['games']
    by_id = {g['id']: g for g in catalog_games}
    source_by_id = {e['id']: e for e in entries}
    catalog = {
        'format': 1,
        'name': collection['name'],
        'baseUrl': args.base_url,
        'games': [],
    }
    written = []
    for entry in entries:
        game = by_id.get(entry['id'])
        if game is None:
            raise SystemExit(f'APK does not contain {entry["id"]}')
        settings = entry['settings']
        item = {
            'id': entry['id'],
            'name': entry['name'],
            'description': settings.get('description', ''),
            'archetype': settings.get('archetype'),
            'orientation': settings.get('mobile', {}).get('orientation', 'auto'),
        }
        if 'instructions' in settings:
            item['instructions'] = settings['instructions']
        for abi in args.abis:
            written.append(bundle_game(args.apk, game, abi, item, args.output, args.base_url.rstrip('/')))
        catalog['games'].append(item)
    (args.output / 'catalog.json').write_text(json.dumps(catalog, ensure_ascii=False, indent=2) + '\n')
    with (args.output / 'SHA256SUMS').open('w') as out:
        for item in written:
            rel = item['path'].relative_to(args.output).as_posix()
            out.write(f'{item["sha256"]}  {rel}\n')
    total = sum(item['bytes'] for item in written)
    print(f'wrote {len(written)} bundles for {len(entries)} games, {total} bytes')
    print(args.output)


if __name__ == '__main__':
    main()
