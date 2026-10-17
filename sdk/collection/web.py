"""Static responsive launcher. Each game lives in its own disposable document."""
import html
import hashlib
import errno
import importlib.util
import json
from datetime import datetime
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
    _copy_screenshots(output, identifier, [])
    try:
        (folder / 'screenshots').rmdir()
    except FileNotFoundError:
        pass
    except OSError as error:
        if error.errno not in (errno.ENOTEMPTY, errno.EEXIST): raise
    for name in [*files, 'lcom-web.json']:
        if not name or name in ('.', '..') or '/' in name or '\\' in name:
            raise ValueError(f'Invalid browser export filename: {name}')
        path = folder / name
        if path.is_file() or path.is_symlink(): path.unlink()
    try:
        folder.rmdir()
    except OSError as error:
        if error.errno not in (errno.ENOTEMPTY, errno.EEXIST): raise


def build(entries, output, name, icon=None, presentation=None):
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
        image_inventory = destination / 'lcom-web.json'
        screenshots = json.loads(image_inventory.read_text()).get('screenshots', []) if image_inventory.is_file() else []
        exporter.build(args)
        if screenshots:
            metadata = json.loads(image_inventory.read_text())
            metadata['screenshots'] = screenshots
            image_inventory.write_text(json.dumps(metadata, indent=2) + '\n')
    catalog = write_launcher(entries, output, name, icon, presentation)
    current = {game['id'] for game in catalog}
    for identifier in owned.keys() - current:
        _prune_game(output, identifier)
    marker.write_text(json.dumps({'format': 1, 'games': catalog}, indent=2) + '\n')
    return output


def write_launcher(entries, output, name, icon=None, presentation=None):
    """Package the library UI and metadata for already exported games."""
    output = Path(output)
    catalog = [{'id': entry['id'], 'name': entry['name'],
                'description': entry['settings']['description'],
                'archetype': entry['settings']['archetype'],
                'revision': hashlib.sha256((output / 'games' / entry['id'] / 'index.html').read_bytes()).hexdigest()[:12],
                'url': f'games/{entry["id"]}/',
                'preview': f'games/{entry["id"]}/preview.png' if entry['settings']['preview'] else None,
                'icon': f'games/{entry["id"]}/icon.png' if entry['icon'] else None}
               for entry in entries]
    details, galleries, options = _presentation(presentation, {entry['id'] for entry in entries})
    for game in catalog:
        game.update(details.get(game['id'], {}))
        game.update(download_metadata(output / 'games' / game['id'], f"games/{game['id']}/"))
        game['screenshots'] = _copy_screenshots(output, game['id'], galleries.get(game['id'], []))
    (output / 'collection.json').write_text(json.dumps({'name': name, 'games': catalog,
        'taxonomy': options.get('taxonomy', [])}, ensure_ascii=False) + '\n')
    template = Path(__file__).parent / 'web-template'
    assets = sorted(path for path in template.iterdir()
                    if path.is_file() and (path.suffix in ('.js', '.css') or path.name == 'index.html'))
    # Version the module graph together. A leaf-only edit must also invalidate
    # cached importers, including graphs with cyclic JavaScript imports.
    revision = hashlib.sha256(b''.join(path.name.encode() + b'\0' + path.read_bytes()
                                       for path in assets if path.suffix == '.js')).hexdigest()[:12]
    modules = {path.name for path in assets if path.suffix == '.js'}
    for asset in assets:
        if asset.suffix == '.js':
            def version_import(match):
                if match[2][2:] not in modules: return match[0]
                return match[1] + match[2] + '?v=' + revision + match[3]
            source = re.sub(r'''((?:\bfrom\s*|\bimport\s*(?:\(\s*)?)["'])(\./[a-zA-Z0-9_.-]+\.js)(["'])''',
                            version_import, asset.read_text())
            (output / asset.name).write_text(source)
        else:
            shutil.copy2(asset, output / asset.name)
    shell = (output / 'index.html').read_text().replace('@NAME@', html.escape(name))
    for filename in ('launcher.css', 'launcher.js'):
        digest = hashlib.sha256((output / filename).read_bytes()).hexdigest()[:12]
        shell = shell.replace(f'"{filename}"', f'"{filename}?v={digest}"')
    if icon:
        shutil.copy2(icon, output / 'icon.png')
        favicon_link = '' if options.get('favicon') else '<link rel="icon" href="icon.png">'
        shell = shell.replace('</head>', favicon_link + '<link rel="apple-touch-icon" href="icon.png"></head>')
    else:
        (output / 'icon.png').unlink(missing_ok=True)
    if options.get('favicon'):
        favicon = options['favicon']
        filename = 'favicon' + favicon.suffix.lower()
        shutil.copy2(favicon, output / filename)
        digest = hashlib.sha256(favicon.read_bytes()).hexdigest()[:12]
        shell = shell.replace('</head>', f'<link rel="icon" href="{filename}?v={digest}"></head>')
    (output / 'index.html').write_text(shell)
    manifest = {'name': name, 'short_name': name, 'start_url': './', 'scope': './',
                'display': 'fullscreen', 'background_color': '#101021', 'theme_color': '#101021'}
    if icon:
        size = int.from_bytes(Path(icon).read_bytes()[16:20], 'big')
        manifest['icons'] = [{'src': 'icon.png', 'sizes': f'{size}x{size}', 'type': 'image/png'}]
    (output / 'manifest.webmanifest').write_text(json.dumps(manifest, ensure_ascii=False) + '\n')
    shutil.copytree(Path(__file__).parent / 'web-template/assets', output / 'assets', dirs_exist_ok=True)
    return catalog


DOWNLOAD_FILES = ('index.html', 'game.js', 'game.wasm', 'game.data', 'game.json',
                  'player.js', 'player.css', 'loading.js', 'driving.js')


def download_metadata(folder, prefix='', wasm_bytes=None, data_bytes=None):
    """Measured game/player payload, excluding library artwork and HTTP overhead.

    Hosting may compress the other files too. Their stored sizes intentionally
    remain an upper bound rather than pretending to measure a particular wire.
    Optional publishing sizes record measured gzip bytes for the two payloads.
    """
    folder = Path(folder)
    files = []
    for name in DOWNLOAD_FILES:
        path = folder / name
        if not path.is_file():
            if name in ('game.js', 'game.wasm'): raise FileNotFoundError(path)
            continue
        size = path.stat().st_size
        record = {'path': prefix + name, 'bytes': size, 'encoding': 'identity'}
        if name == 'game.wasm' and wasm_bytes is not None:
            record.update(bytes=wasm_bytes, encoding='gzip', decodedBytes=size)
        if name == 'game.data' and data_bytes is not None:
            record.update(bytes=data_bytes, encoding='gzip', decodedBytes=size)
        files.append(record)
    compressed = [name for name, size in (('wasm', wasm_bytes), ('data', data_bytes)) if size is not None]
    return {'downloadBytes': sum(item['bytes'] for item in files),
            'downloadBasis': 'gzip-' + '-'.join(compressed) + '-identity-rest' if compressed else 'identity',
            'downloadFiles': files}


def _confined(root, relative):
    if not isinstance(relative, str) or not relative or '\\' in relative:
        raise ValueError('Web presentation paths must be relative filenames')
    name = Path(relative)
    if name.is_absolute() or '..' in name.parts:
        raise ValueError(f'Web presentation path escapes collection: {relative}')
    path = (root / name).resolve(strict=True)
    if not path.is_relative_to(root) or not path.is_file():
        raise ValueError(f'Web presentation path escapes collection: {relative}')
    return path


def _presentation(path, identifiers):
    if path is None: return {}, {}, {}
    path = Path(path).absolute()
    root = path.parent.resolve(strict=True)
    path = path.resolve(strict=True)
    if not path.is_relative_to(root):
        raise ValueError('Web presentation sidecar escapes collection')
    config = exporter.source_project.project_config
    sidecar = config.object_value(json.loads(path.read_text()), str(path), ['version', 'metadata', 'gallery', 'favicon'])
    if type(sidecar.get('version', 1)) is not int or sidecar.get('version', 1) != 1:
        raise ValueError('Web presentation version must be 1')
    details, galleries, options = {}, {}, {}
    if 'favicon' in sidecar:
        favicon = _confined(root, sidecar['favicon'])
        if favicon.suffix.lower() not in ('.png', '.svg'):
            raise ValueError('Web favicon must be PNG or SVG')
        options['favicon'] = favicon
    for kind in ('metadata', 'gallery'):
        if kind not in sidecar: continue
        source = _confined(root, sidecar[kind])
        data = config.object_value(json.loads(source.read_text()), str(source), ['version', 'games', 'taxonomy'] if kind == 'metadata' else ['version', 'games'])
        if type(data.get('version', 1)) is not int or data.get('version', 1) != 1:
            raise ValueError('Web presentation version must be 1')
        games = data.get('games')
        if not isinstance(games, dict) or games.keys() - identifiers:
            raise ValueError(f'{source}: unknown or invalid game identifiers')
        tags = set()
        if kind == 'metadata':
            taxonomy = data.get('taxonomy', [])
            if not isinstance(taxonomy, list) or len(taxonomy) > 128:
                raise ValueError('Game taxonomy must be an array of at most 128 tags')
            for tag in taxonomy:
                config.object_value(tag, 'taxonomy tag', ['id', 'label', 'facet', 'description'])
                identifier = config.string(tag.get('id'), 'taxonomy.id')
                if not re.fullmatch(r'[a-z][a-z0-9-]*', identifier) or identifier in tags:
                    raise ValueError('Taxonomy tag IDs must be unique slugs')
                tags.add(identifier)
                config.string(tag.get('label'), 'taxonomy.label')
                config.choice(tag.get('facet'), 'taxonomy.facet', ['genre', 'players'])
                if 'description' in tag: config.string(tag['description'], 'taxonomy.description')
            options['taxonomy'] = taxonomy
        for identifier, value in games.items():
            if kind == 'gallery':
                if not isinstance(value, list) or len(value) > 12:
                    raise ValueError('A game gallery must be an array of at most 12 images')
                images = [_confined(root, name) for name in value]
                for image in images:
                    data = image.read_bytes()
                    header = data[:24]
                    if not (image.suffix.lower() == '.webp' and len(header) >= 20
                            and header[:4] == b'RIFF' and int.from_bytes(header[4:8], 'little') == len(data) - 8
                            and header[8:12] == b'WEBP'
                            and header[12:16] in (b'VP8 ', b'VP8L', b'VP8X')):
                        raise ValueError(f'Gallery image must be WebP: {image}')
                galleries[identifier] = images
                continue
            config.object_value(value, identifier, ['year', 'yearLabel', 'yearSource', 'featured', 'featuredPool', 'repository', 'tags', 'academicYear', 'academicYearSource'])
            if 'tags' in value:
                selected = value['tags']
                if not isinstance(selected, list) or any(not isinstance(tag, str) or tag not in tags for tag in selected) or len(set(selected)) != len(selected):
                    raise ValueError('Game tags must be unique IDs from the collection taxonomy')
            if 'academicYear' in value:
                year = value['academicYear']
                if year is not None:
                    match = re.fullmatch(r'(\d{4})/(\d{2})', str(year))
                    if not match or not 1900 <= int(match[1]) <= 2100 or int(match[2]) != (int(match[1]) + 1) % 100:
                        raise ValueError('Academic year must identify consecutive years, such as 2023/24')
                evidence = config.object_value(value.get('academicYearSource'), 'academicYearSource', ['kind', 'confidence', 'detail', 'url', 'date'])
                config.string(evidence.get('kind'), 'academicYearSource.kind')
                config.string(evidence.get('detail'), 'academicYearSource.detail')
                config.choice(evidence.get('confidence'), 'academicYearSource.confidence', ['explicit', 'inferred', 'unknown'])
                if (year is None) != (evidence['confidence'] == 'unknown'):
                    raise ValueError('An unknown academic year must be null with unknown confidence')
                if 'url' in evidence and not re.match(r'^https?://[^\s]+$', config.string(evidence['url'], 'academicYearSource.url')):
                    raise ValueError('Academic year evidence must use an HTTP URL')
                if 'date' in evidence: datetime.fromisoformat(config.string(evidence['date'], 'academicYearSource.date'))
            elif 'academicYearSource' in value:
                raise ValueError('academicYearSource requires academicYear')
            if 'repository' in value:
                config.string(value['repository'], 'repository')
                if not re.match(r'^https?://[^\s]+$', value['repository']):
                    raise ValueError('repository must be an HTTP URL')
            if 'year' in value and (type(value['year']) is not int or not 1900 <= value['year'] <= 2100):
                raise ValueError('Game year must be an integer from 1900 to 2100')
            if 'yearLabel' in value:
                config.string(value['yearLabel'], 'yearLabel')
                if 'year' not in value: raise ValueError('yearLabel requires an evidenced year')
            if 'year' in value and 'yearSource' not in value: raise ValueError('Game year requires yearSource')
            if 'yearSource' in value:
                evidence = config.object_value(value['yearSource'], 'yearSource', ['kind', 'url', 'detail', 'createdAt'])
                config.choice(evidence.get('kind'), 'yearSource.kind', ['repository', 'course', 'commit', 'unknown'])
                config.string(evidence.get('detail'), 'yearSource.detail')
                if 'url' in evidence:
                    config.string(evidence['url'], 'yearSource.url')
                    if not re.match(r'^https?://[^\s]+$', evidence['url']):
                        raise ValueError('yearSource.url must be an HTTP URL')
                if evidence['kind'] == 'commit' and value.get('yearLabel') != f"Updated {value.get('year')}":
                    raise ValueError('Commit dates must be labelled Updated YYYY')
                if evidence['kind'] == 'repository':
                    created = config.string(evidence.get('createdAt'), 'yearSource.createdAt')
                    if datetime.fromisoformat(created.replace('Z', '+00:00')).year != value.get('year'):
                        raise ValueError('Repository year must match its creation date')
                    if 'yearLabel' in value and value['yearLabel'] != str(value['year']):
                        raise ValueError('Repository year labels must use the creation year')
                if evidence['kind'] == 'unknown' and 'year' in value:
                    raise ValueError('Unknown yearSource cannot establish a year')
            for flag in ('featured','featuredPool'):
                if flag in value: config.boolean(value[flag], flag)
            details[identifier] = value
    return details, galleries, options


def _copy_screenshots(output, identifier, sources):
    folder = output / 'games' / identifier
    inventory = folder / 'lcom-web.json'
    record = json.loads(inventory.read_text()) if inventory.is_file() else {}
    previous = record.get('screenshots', [])
    names = []
    for source in sources:
        data = source.read_bytes()
        filename = hashlib.sha256(data).hexdigest()[:20] + '.webp'
        destination = folder / 'screenshots' / filename
        destination.parent.mkdir(parents=True, exist_ok=True)
        destination.write_bytes(data)
        names.append(filename)
    for filename in previous:
        if not isinstance(filename, str) or not re.fullmatch(r'[0-9a-f]{20}\.webp', filename):
            raise ValueError('Invalid owned screenshot filename')
        if filename not in names: (folder / 'screenshots' / filename).unlink(missing_ok=True)
    if inventory.is_file():
        record['screenshots'] = names
        inventory.write_text(json.dumps(record, indent=2) + '\n')
    return [f'games/{identifier}/screenshots/{filename}' for filename in names]
