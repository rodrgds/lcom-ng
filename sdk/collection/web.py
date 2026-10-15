"""Static responsive launcher. Each game lives in its own disposable document."""
import html
import hashlib
import errno
import importlib.util
import json
from pathlib import Path
import shutil
import re

SDK = Path(__file__).resolve().parents[1]
_spec = importlib.util.spec_from_file_location('lcom_collection_web_game', SDK / 'web/export.py')
exporter = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(exporter)


def _prune_game(output, identifier):
    folder = output / 'games' / identifier
    metadata = folder / 'lcom-web.json'
    if folder.is_symlink() or not folder.resolve().is_relative_to(output) or not metadata.is_file() or metadata.is_symlink():
        return
    files = json.loads(metadata.read_text()).get('files', [])
    if not isinstance(files, list) or any(not isinstance(name, str) for name in files):
        raise ValueError(f'Invalid browser export file inventory: {metadata}')
    for name in [*files, 'lcom-web.json']:
        if not name or name in ('.', '..') or '/' in name or '\\' in name:
            raise ValueError(f'Invalid browser export filename: {name}')
        path = folder / name
        if path.is_file() or path.is_symlink(): path.unlink()
    try:
        folder.rmdir()
    except OSError as error:
        if error.errno not in (errno.ENOTEMPTY, errno.EEXIST): raise


def build(entries, output, name, icon=None):
    output = Path(output).resolve()
    if any(Path(e['project']).resolve().is_relative_to(output) for e in entries):
        raise ValueError('Collection output cannot contain a game source folder')
    marker = output / 'lcom-collection-web.json'
    if output.exists() and (not output.is_dir() or (any(output.iterdir()) and not marker.is_file())):
        raise ValueError('Output is not an existing web collection; choose an empty directory')
    output.mkdir(parents=True, exist_ok=True)
    previous = json.loads(marker.read_text()).get('games', []) if marker.is_file() else []
    owned = {game['id']: game for game in previous if isinstance(game, dict)
             and re.fullmatch(r'[a-z][a-z0-9_-]*', str(game.get('id', '')))}
    # Retain old ownership through interrupted builds so removed games can still
    # be cleaned on a later successful export. Unknown files remain untouched.
    pending = {**owned, **{entry['id']: {'id': entry['id']} for entry in entries}}
    marker.write_text(json.dumps({'format': 1, 'games': list(pending.values())}) + '\n')
    for entry in entries:
        print('Exporting collection game: ' + entry['name'], flush=True)
        destination = output / 'games' / entry['id']
        args = exporter.argument_parser().parse_args([str(entry['project']), '--output', str(destination)])
        exporter.build(args)
    catalog = write_launcher(entries, output, name, icon)
    current = {game['id'] for game in catalog}
    for identifier in owned.keys() - current:
        _prune_game(output, identifier)
    marker.write_text(json.dumps({'format': 1, 'games': catalog}, indent=2) + '\n')
    return output


def write_launcher(entries, output, name, icon=None):
    """Package the library UI and metadata for already exported games."""
    output = Path(output)
    catalog = [{'id': entry['id'], 'name': entry['name'],
                'description': entry['settings']['description'],
                'revision': hashlib.sha256((output / 'games' / entry['id'] / 'index.html').read_bytes()).hexdigest()[:12],
                'url': f'games/{entry["id"]}/',
                'preview': f'games/{entry["id"]}/preview.png' if entry['settings']['preview'] else None,
                'icon': f'games/{entry["id"]}/icon.png' if entry['icon'] else None}
               for entry in entries]
    (output / 'collection.json').write_text(json.dumps({'name': name, 'games': catalog}, ensure_ascii=False) + '\n')
    for filename in ('index.html', 'launcher.css', 'launcher.js'):
        shutil.copy2(Path(__file__).parent / 'web-template' / filename, output / filename)
    shell = (output / 'index.html').read_text().replace('@NAME@', html.escape(name))
    for filename in ('launcher.css', 'launcher.js'):
        digest = hashlib.sha256((output / filename).read_bytes()).hexdigest()[:12]
        shell = shell.replace(f'"{filename}"', f'"{filename}?v={digest}"')
    if icon:
        shutil.copy2(icon, output / 'icon.png')
        shell = shell.replace('</head>', '<link rel="icon" href="icon.png"><link rel="apple-touch-icon" href="icon.png"></head>')
    else:
        (output / 'icon.png').unlink(missing_ok=True)
    (output / 'index.html').write_text(shell)
    manifest = {'name': name, 'short_name': name, 'start_url': './', 'scope': './',
                'display': 'fullscreen', 'background_color': '#101021', 'theme_color': '#101021'}
    if icon:
        size = int.from_bytes(Path(icon).read_bytes()[16:20], 'big')
        manifest['icons'] = [{'src': 'icon.png', 'sizes': f'{size}x{size}', 'type': 'image/png'}]
    (output / 'manifest.webmanifest').write_text(json.dumps(manifest, ensure_ascii=False) + '\n')
    shutil.copytree(Path(__file__).parent / 'web-template/assets', output / 'assets', dirs_exist_ok=True)
    return catalog
