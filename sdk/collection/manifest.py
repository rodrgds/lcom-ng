"""A collection owns its folder list; each game owns its export settings."""
import hashlib
import json
import os
from pathlib import Path
import re
import sys

SDK = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(SDK / 'export'))
import config

FILENAME = 'lcom.collection.json'


def load(folder):
    folder = Path(folder).resolve(strict=True)
    path = folder / FILENAME
    data = json.loads(path.read_text())
    config.object_value(data, str(path), ['$schema', 'version', 'name', 'icon', 'projects'])
    if type(data.get('version', 1)) is not int or data.get('version', 1) != 1:
        raise ValueError('Collection version must be 1')
    name = config.string(data.get('name', folder.name), 'collection name')
    icon = config.icon_path(folder / config.string(data['icon'], 'collection icon')) if 'icon' in data else None
    projects = data.get('projects')
    if not isinstance(projects, list) or not projects or any(not isinstance(p, str) or not p.strip() for p in projects):
        raise ValueError('Collection projects must be a non-empty array of folder paths')
    entries, seen = [], set()
    for item in projects:
        project = (folder / item).resolve(strict=True)
        if not project.is_dir(): raise ValueError(f'Collection project must be a folder: {item}')
        if project in seen: raise ValueError(f'Duplicate collection project: {item}')
        if (project / FILENAME).exists(): raise ValueError(f'Nested collections are not supported: {item}')
        seen.add(project)
        settings = config.load(project)
        relative = Path(os.path.relpath(project,folder)).as_posix()
        parts = [part for part in Path(relative).parts if part.lower() not in ('.','..','src','proj','project')]
        slug = re.sub('[^a-z0-9]+', '-', (parts[-1] if parts else 'game').lower()).strip('-')[:48] or 'game'
        if not slug[0].isalpha(): slug = 'game-' + slug
        # Stable across relocation, independent of absolute build-host paths.
        identifier = slug + '-' + hashlib.sha256(relative.encode()).hexdigest()[:12]
        entries.append({'id': identifier, 'project': project, 'name': settings['name'],
                        'icon': settings['icon'], 'settings': settings})
    return {'name': name, 'icon': icon, 'entries': entries}


def build(args, target):
    collection = load(args.project)
    # Per-game CLI switches would silently replace authored controls/source lists.
    if any(getattr(args, key, None) for key in ('source', 'include', 'asset', 'asset_root', 'arg', 'library', 'config', 'orientation')):
        raise ValueError('Configure each collection game in its own lcom.json; collection exports accept --name and --icon')
    name = args.name or collection['name']
    icon = config.icon_path(args.icon) if args.icon else collection['icon']
    output = (args.output or args.project / '.lcom-ng' / target).resolve()
    if Path(args.project).resolve().is_relative_to(output):
        raise ValueError('Output must be separate from the collection folder and its parents')
    if target == 'web':
        from collection import web
        return web.build(collection['entries'], output, name, icon)
    from collection import android
    return android.build(collection['entries'], output, name, icon,
                         args.app_id or 'pt.rgo.lcomng.arcade', args.abi,
                         args.project_only)
