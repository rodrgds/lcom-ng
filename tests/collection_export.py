#!/usr/bin/env python3
"""Collection folder-list contracts, including relocation and validation."""
import json
from pathlib import Path
import shutil
import sys
import tempfile
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'sdk'))
from collection import manifest
from collection import web


class CollectionManifest(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix='lcom collection ')
        self.root = Path(self.temp.name)
        self.collection = self.root / 'library'
        self.collection.mkdir()
        for name in ('first', 'second'):
            folder = self.root / name
            folder.mkdir()
            (folder / 'lcom.json').write_text(json.dumps({'name': 'Same title', 'mobile': {'fullscreen': True}}))
        self.write(['../first', '../second'])

    def tearDown(self): self.temp.cleanup()

    def write(self, projects):
        (self.collection / manifest.FILENAME).write_text(json.dumps({'name': 'Class games', 'projects': projects}))

    def test_project_settings_and_ids_survive_relocation(self):
        initial = manifest.load(self.collection)
        self.assertEqual(initial['name'], 'Class games')
        self.assertEqual(len({entry['id'] for entry in initial['entries']}), 2)
        self.assertTrue(all(entry['settings']['mobile']['fullscreen'] for entry in initial['entries']))
        (self.root/'first/lcom.json').write_text(json.dumps({'name':'Renamed game','mobile':{'fullscreen':True}}))
        self.assertEqual([entry['id'] for entry in manifest.load(self.collection)['entries']], [entry['id'] for entry in initial['entries']])
        moved = self.root / 'relocated'
        moved.mkdir()
        for name in ('library', 'first', 'second'): shutil.move(self.root / name, moved / name)
        relocated = manifest.load(moved / 'library')
        self.assertEqual([entry['id'] for entry in relocated['entries']], [entry['id'] for entry in initial['entries']])
        self.assertEqual(relocated['entries'][0]['project'], (moved / 'first').resolve())

    def test_rejects_duplicates_nested_and_missing_projects(self):
        for projects, error in [([], ValueError), (['../first', '../first'], ValueError), (['../missing'], FileNotFoundError), (['.'], ValueError)]:
            with self.subTest(projects=projects):
                self.write(projects)
                with self.assertRaises(error): manifest.load(self.collection)

    def test_reexport_removes_owned_files_and_preserves_unknown_files(self):
        output = self.root / 'export'
        entries = manifest.load(self.collection)['entries']
        def compile_game(args):
            args.output.mkdir(parents=True, exist_ok=True)
            (args.output / 'index.html').write_text('game')
            (args.output / 'game.wasm').write_bytes(b'game')
            (args.output / 'lcom-web.json').write_text(json.dumps({'files': ['index.html', 'game.wasm']}))
        with patch.object(web.exporter, 'build', side_effect=compile_game):
            web.build(entries, output, 'Class games')
            removed = output / 'games' / entries[1]['id']
            (removed / 'my-notes.txt').write_text('keep this')
            unknown = output / 'games' / 'personal'
            unknown.mkdir()
            (unknown / 'index.html').write_text('personal game')
            with patch.object(web.exporter, 'build', side_effect=RuntimeError('interrupted')):
                with self.assertRaises(RuntimeError): web.build(entries[:1], output, 'Class games')
            self.assertTrue((removed / 'index.html').is_file())
            web.build(entries[:1], output, 'Class games')
        self.assertFalse((removed / 'index.html').exists())
        self.assertFalse((removed / 'game.wasm').exists())
        self.assertEqual((removed / 'my-notes.txt').read_text(), 'keep this')
        self.assertEqual((unknown / 'index.html').read_text(), 'personal game')
        self.assertEqual(len(json.loads((output / 'collection.json').read_text())['games']), 1)


if __name__ == '__main__': unittest.main()
