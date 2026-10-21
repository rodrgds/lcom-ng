#!/usr/bin/env python3
"""Collection folder-list contracts, including relocation and validation."""
import json
import base64
from pathlib import Path
import shutil
import sys
import tempfile
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'sdk'))
from collection import manifest
from collection import share
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
            (args.output / 'game.js').write_bytes(b'loader')
            (args.output / 'lcom-web.json').write_text(json.dumps({'files': ['index.html', 'game.wasm']}))
        with patch.object(web.exporter, 'build', side_effect=compile_game):
            web.build(entries, output, 'Class games')
            image=base64.b64decode('UklGRh4AAABXRUJQVlA4TBEAAAAvAUAAAAdQwGIUu/+BiOh/AAA=')
            (self.collection/'shot.webp').write_bytes(image)
            (self.collection/'gallery.json').write_text(json.dumps({'version':1,'games':{entry['id']:['shot.webp'] for entry in entries}}))
            (self.collection/'web.json').write_text(json.dumps({'version':1,'gallery':'gallery.json'}))
            catalog=web.write_launcher(entries,output,'Class games',presentation=self.collection/'web.json')
            screenshots=[output/game['screenshots'][0] for game in catalog]
            removed = output / 'games' / entries[1]['id']
            (removed / 'my-notes.txt').write_text('keep this')
            (removed/'screenshots'/'personal.webp').write_bytes(b'keep personal image')
            unknown = output / 'games' / 'personal'
            unknown.mkdir()
            (unknown / 'index.html').write_text('personal game')
            with patch.object(web.exporter, 'build', side_effect=RuntimeError('interrupted')):
                with self.assertRaises(RuntimeError): web.build(entries[:1], output, 'Class games')
            self.assertTrue((removed / 'index.html').is_file())
            web.build(entries[:1], output, 'Class games')
        self.assertTrue(all(not image.exists() for image in screenshots))
        self.assertEqual((removed/'screenshots'/'personal.webp').read_bytes(),b'keep personal image')
        self.assertFalse((removed / 'index.html').exists())
        self.assertFalse((removed / 'game.wasm').exists())
        self.assertEqual((removed / 'my-notes.txt').read_text(), 'keep this')
        self.assertEqual((unknown / 'index.html').read_text(), 'personal game')
        self.assertEqual(len(json.loads((output / 'collection.json').read_text())['games']), 1)


    def test_web_presentation_relocates_images_and_measures_required_game_files(self):
        entries=manifest.load(self.collection)['entries']
        identifier=entries[0]['id']
        image=base64.b64decode('UklGRh4AAABXRUJQVlA4TBEAAAAvAUAAAAdQwGIUu/+BiOh/AAA=')
        (self.collection/'capture.webp').write_bytes(image)
        (self.collection/'metadata.json').write_text(json.dumps({'version':1,'taxonomy':[{'id':'snake','label':'Snake','facet':'genre'}],'games':{identifier:{'tags':['snake'],'slug':'aa-same-title','authors':['Ana Alves','André Antunes'],'academicYear':'2023/24','academicYearSource':{'kind':'readme','confidence':'explicit','detail':'Original README names the 2023/24 class.'},'year':2024,'yearLabel':'2024','yearSource':{'kind':'repository','url':'https://github.com/example/game','createdAt':'2024-05-12T00:00:00Z','detail':'GitHub repository creation date'},'featured':True,'featuredPool':True,'repository':'https://github.com/example/game'}}}))
        (self.collection/'gallery.json').write_text(json.dumps({'version':1,'games':{identifier:['capture.webp']}}))
        favicon=b'<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 10 10"><path d="M0 0h10v10H0z"/></svg>'
        (self.collection/'favicon.svg').write_bytes(favicon)
        (self.collection/'web.json').write_text(json.dumps({'version':1,'metadata':'metadata.json','gallery':'gallery.json','favicon':'favicon.svg'}))
        output=self.root/'export'
        for entry in entries:
            game=output/'games'/entry['id'];game.mkdir(parents=True)
            for name,data in {'index.html':b'shell','game.js':b'loader','game.wasm':b'wasm','game.data':b'assets','player.js':b'player','player.css':b'styles','loading.js':b'loading','driving.js':b'wheel','game.json':b'{}','preview.png':b'excluded artwork'}.items():
                (game/name).write_bytes(data)
        games=web.write_launcher(entries,output,'Class games',presentation=self.collection/'web.json')
        game=games[0]
        self.assertEqual((game['year'],game['yearLabel'],game['yearSource']['kind'],game['featured']),(2024,'2024','repository',True))
        self.assertEqual(game['repository'],'https://github.com/example/game')
        self.assertEqual((game['tags'],game['academicYear']),(['snake'],'2023/24'))
        self.assertEqual((game['slug'],game['authors']),('aa-same-title',['Ana Alves','André Antunes']))
        self.assertEqual(json.loads((output/'collection.json').read_text())['taxonomy'][0]['label'],'Snake')
        self.assertEqual((output/'favicon.svg').read_bytes(),favicon)
        self.assertIn('href="favicon.svg', (output/'index.html').read_text())
        self.assertNotIn('year',games[1])
        self.assertEqual(game['downloadBytes'],47)
        self.assertEqual(sum(item['bytes'] for item in game['downloadFiles']),47)
        self.assertNotIn('preview.png',[Path(item['path']).name for item in game['downloadFiles']])
        self.assertEqual(len(game['screenshots']),1)
        screenshot=output/game['screenshots'][0]
        self.assertEqual(screenshot.read_bytes(),image)
        self.assertTrue(screenshot.name.endswith('.webp'))
        self.assertTrue(screenshot.is_relative_to(output/'games'/identifier/'screenshots'))
        moved=self.root/'moved';shutil.move(self.collection,moved)
        games=web.write_launcher(entries,output,'Class games',presentation=moved/'web.json')
        self.assertEqual(games[0]['screenshots'],game['screenshots'])

    def test_web_presentation_rejects_paths_outside_collection_and_invalid_metadata(self):
        entries=manifest.load(self.collection)['entries'];identifier=entries[0]['id']
        output=self.root/'export'
        for entry in entries:
            game=output/'games'/entry['id'];game.mkdir(parents=True)
            for name in ['index.html','game.js','game.wasm']:(game/name).write_bytes(b'game')
        (self.root/'outside.webp').write_bytes(b'not an image')
        (self.collection/'escape.webp').symlink_to(self.root/'outside.webp')
        (self.collection/'fake.webp').write_bytes(b'not an image')
        cases=[({'version':1,'metadata':'metadata.json'},{'version':1,'games':{identifier:{'year':2025,'yearSource':{'kind':'repository','createdAt':'2024-05-12T00:00:00Z','detail':'Created in 2024'}}}},None),({'version':1,'metadata':'metadata.json'},{'version':1,'games':{identifier:{'year':2024,'yearSource':{'kind':'repository','detail':'Missing creation date'}}}},None),({'version':1,'metadata':'metadata.json'},{'version':1,'games':{identifier:{'year':2024,'yearLabel':'2024','yearSource':{'kind':'commit','detail':'Commit date must keep its provenance label'}}}},None),({'version':1,'metadata':'metadata.json'},{'version':1,'games':{identifier:{'repository':'javascript:alert(1)'}}},None),({'version':1,'gallery':'gallery.json'},None,{'version':1,'games':{identifier:['fake.webp']}}),({'version':1,'metadata':'../first/lcom.json'},None,None),
               ({'version':1,'gallery':'gallery.json'},None,{'version':1,'games':{identifier:['escape.webp']}}),
               ({'version':1,'gallery':'gallery.json'},None,{'version':1,'games':{identifier:['../outside.webp']}}),
               ({'version':1,'metadata':'metadata.json'},{'version':1,'games':{'unknown':{'year':2025}}},None),
               ({'version':1,'metadata':'metadata.json'},{'version':1,'games':{identifier:{'year':True}}},None),
               ({'version':1,'metadata':'metadata.json'},{'version':1,'games':{identifier:{'featured':'yes'}}},None),
               ({'version':1,'metadata':'metadata.json'},{'version':1,'games':{identifier:{'slug':'Class Game'}}},None),
               ({'version':1,'metadata':'metadata.json'},{'version':1,'games':{identifier:{'slug':'9leading-digit'}}},None),
               ({'version':1,'metadata':'metadata.json'},{'version':1,'games':{identifier:{'authors':[]}}},None),
               ({'version':1,'metadata':'metadata.json'},{'version':1,'games':{identifier:{'authors':['Ana Alves',42]}}},None),
               ({'version':1,'metadata':'metadata.json'},{'version':1,'games':{entry['id']:{'slug':'same-slug'} for entry in entries}},None),
               ({'version':1,'metadata':'metadata.json'},{'version':1,'games':{entry['id']:{'unsupported':True} for entry in entries}},None)]
        for sidecar,metadata,gallery in cases:
            with self.subTest(sidecar=sidecar,metadata=metadata,gallery=gallery):
                (self.collection/'web.json').write_text(json.dumps(sidecar))
                if metadata:(self.collection/'metadata.json').write_text(json.dumps(metadata))
                if gallery:(self.collection/'gallery.json').write_text(json.dumps(gallery))
                with self.assertRaises(ValueError):web.write_launcher(entries,output,'Class games',presentation=self.collection/'web.json')

        (self.collection/'web.json').unlink()
        (self.root/'outside.json').write_text('{}')
        (self.collection/'web.json').symlink_to(self.root/'outside.json')
        with self.assertRaises(ValueError):web.write_launcher(entries,output,'Class games',presentation=self.collection/'web.json')


    def test_leaf_module_changes_invalidate_the_launcher_and_are_exported(self):
        entries=manifest.load(self.collection)['entries'];output=self.root/'export'
        for entry in entries:
            folder=output/'games'/entry['id'];folder.mkdir(parents=True)
            for name in ['index.html','game.js','game.wasm']:(folder/name).write_bytes(b'game')
        module=self.root/'installed-sdk'/'collection';template=module/'web-template';(template/'assets').mkdir(parents=True)
        (template/'index.html').write_text('<title>@NAME@</title><script src="launcher.js"></script><link href="launcher.css">')
        (template/'launcher.js').write_text("import {color} from './widget.js'; console.log(color);")
        (template/'launcher.css').write_text('body{color:blue}')
        (template/'widget.js').write_text("export const color='blue';")
        with patch.object(web,'__file__',str(module/'web.py')):
            web.write_launcher(entries,output,'Class games')
            first=(output/'index.html').read_text();first_module=(output/'launcher.js').read_text()
            self.assertEqual((output/'widget.js').read_text(),"export const color='blue';")
            self.assertIn('./widget.js?v=',first_module)
            (template/'widget.js').write_text("export const color='red';")
            web.write_launcher(entries,output,'Class games')
            self.assertNotEqual(first,(output/'index.html').read_text())
            self.assertNotEqual(first_module,(output/'launcher.js').read_text())
            self.assertEqual((output/'widget.js').read_text(),"export const color='red';")


class ShareSlugDerivation(unittest.TestCase):
    """Initials come from evidenced first names, and names stay URL-safe."""

    def test_slugs_combine_sorted_initials_with_a_normalized_name(self):
        cases = [([['Ana Alves', 'Filipe Faria', 'Gonçalo Gomes'], 'Ninjix'], 'afg-ninjix'),
                 ([['José Santos', 'Luís Du', 'Madalena Ye', 'Rodrigo Moucho'], 'Bamboozlers'], 'jlmr-bamboozlers'),
                 ([['Rui Ribeiro', 'Rita Reis'], 'Tetris'], 'r-tetris'),
                 ([[], 'Plants vs. Zombies'], 'plants-vs-zombies'),
                 ([['Ana Alves'], '4 in a row'], 'a-game-4-in-a-row')]
        for (authors, name), slug in cases:
            with self.subTest(authors=authors, name=name):
                self.assertEqual(share.share_slug(authors, name), slug)
                self.assertRegex(slug, share.SHARE_SLUG)

    def test_long_names_leave_room_for_their_initials(self):
        slug = share.share_slug(['Ana Alves', 'Bruno Braga'], 'x' * 80)
        self.assertLessEqual(len(slug), share.MAX_SLUG_LENGTH)
        self.assertTrue(slug.startswith('ab-'))

    def test_collisions_gain_deterministic_numeric_suffixes(self):
        assigned = share.assign_slugs([{'id': 'b', 'name': 'Snake', 'authors': []},
                                       {'id': 'a', 'name': 'Snake', 'authors': []}])
        self.assertEqual(sorted(assigned.values()), ['snake', 'snake-2'])
        self.assertEqual(assigned['a'], 'snake')


class ArcadeShareSlugs(unittest.TestCase):
    """Published share addresses stay derivable from the credited authors."""

    def test_every_arcade_game_publishes_its_derived_share_slug(self):
        collection = ROOT / 'collections/arcade'
        entries = manifest.load(collection)['entries']
        metadata = json.loads((collection / 'game-metadata.json').read_text())['games']
        games = [{'id': entry['id'], 'name': entry['name'],
                  'authors': metadata.get(entry['id'], {}).get('authors', [])} for entry in entries]
        self.assertEqual(len(games), len(entries))
        missing = [game['id'] for game in games if 'slug' not in metadata.get(game['id'], {})]
        self.assertFalse(missing, f'Arcade games without a share slug: {missing[:5]}')
        for identifier, slug in share.assign_slugs(games).items():
            with self.subTest(game=identifier):
                self.assertEqual(metadata[identifier]['slug'], slug)
                self.assertRegex(slug, share.SHARE_SLUG)


if __name__ == '__main__': unittest.main()
