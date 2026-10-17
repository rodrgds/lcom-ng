#!/usr/bin/env python3
"""A mixed collection must preserve imported provenance and authored sources."""
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'scripts'))
import check_collection_sources as checker


class MixedCollectionSources(unittest.TestCase):
    def test_authored_and_pinned_sources_remain_distinct(self):
        environment = os.environ.copy()
        # The verification caller may supply a real proposed index or Git directory.
        # The temporary repository and checker must share only their own Git state.
        for name in subprocess.check_output(
                ['git', 'rev-parse', '--local-env-vars'], text=True).splitlines():
            environment.pop(name, None)
        with (tempfile.TemporaryDirectory() as temporary,
              patch.dict(os.environ, environment, clear=True)):
            repo = Path(temporary).resolve()
            local = repo / 'examples/local'
            imported = repo / 'examples/community/imported'
            collection = repo / 'collections/arcade'
            for folder in (local, imported, collection):
                folder.mkdir(parents=True)
            for folder, code in ((local, 'int main(void) { return 0; }'), (imported, 'int main(void) { return 1; }')):
                (folder / 'main.c').write_text(code)
                (folder / 'lcom.json').write_text(json.dumps({'name': folder.name}))
            (local / '.gitignore').write_text('.lcom-ng/\n')
            upstream = imported / 'UPSTREAM.json'
            upstream.write_text(json.dumps({'sourceHashes': {'main.c': hashlib.sha256((imported / 'main.c').read_bytes()).hexdigest()}}))
            (collection / 'lcom.collection.json').write_text(json.dumps({'projects': ['../../examples/local', '../../examples/community/imported']}))
            subprocess.run(['git', 'init', '-q', str(repo)], check=True, capture_output=True)
            subprocess.run(['git', '-C', str(repo), 'add', '.'], check=True, capture_output=True)
            with patch.object(checker, 'ROOT', repo):
                checker.check(collection, require_tracked=True)
                generated = local / '.lcom-ng/android/native'
                generated.mkdir(parents=True)
                (generated / 'GameConfig.hpp').write_text('int generated;')
                checker.check(collection, require_tracked=True)
                (local / 'missing.c').write_text('int missing;')
                with self.assertRaisesRegex(ValueError, 'not tracked'):
                    checker.check(collection, require_tracked=True)
                (local / 'missing.c').unlink()
                upstream.unlink()
                with self.assertRaisesRegex(ValueError, 'missing UPSTREAM'):
                    checker.check(collection, require_tracked=True)


    def test_temporary_fixture_preserves_inherited_proposed_index(self):
        environment = os.environ.copy()
        local_names = subprocess.check_output(
            ['git', 'rev-parse', '--local-env-vars'], text=True).splitlines()
        for name in local_names:
            environment.pop(name, None)
        with tempfile.TemporaryDirectory() as temporary:
            outer = Path(temporary).resolve()
            subprocess.run(['git', 'init', '-q', str(outer)], env=environment,
                           check=True, capture_output=True)
            (outer / 'proposed.c').write_text('int proposed;')
            index = outer / 'proposed.index'
            proposed = {**environment, 'GIT_INDEX_FILE': str(index)}
            subprocess.run(['git', '-C', str(outer), 'add', 'proposed.c'],
                           env=proposed, check=True, capture_output=True)
            before = index.read_bytes()
            routes = ({}, {'GIT_DIR': str(outer / '.git'),
                          'GIT_WORK_TREE': str(outer),
                          'GIT_COMMON_DIR': str(outer / '.git')})
            for routing in routes:
                with self.subTest(routing=sorted(routing)):
                    index.write_bytes(before)
                    result = subprocess.run(
                        [sys.executable, str(Path(__file__).resolve()),
                         'MixedCollectionSources.test_authored_and_pinned_sources_remain_distinct'],
                        env={**proposed, **routing}, capture_output=True, text=True)
                    self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
                    self.assertEqual(index.read_bytes(), before,
                                     'temporary fixture rewrote the caller proposed Git index')
                    tracked = subprocess.check_output(
                        ['git', '-C', str(outer), 'ls-files'], env=proposed, text=True)
                    self.assertEqual(tracked, 'proposed.c\n')


if __name__ == '__main__':
    unittest.main()
