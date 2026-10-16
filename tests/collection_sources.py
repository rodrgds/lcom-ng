#!/usr/bin/env python3
"""A mixed collection must preserve imported provenance and authored sources."""
import hashlib
import json
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
        with tempfile.TemporaryDirectory() as temporary:
            repo = Path(temporary).resolve()
            local = repo / 'examples/local'
            imported = repo / 'examples/community/imported'
            collection = repo / 'collections/arcade'
            for folder in (local, imported, collection):
                folder.mkdir(parents=True)
            for folder, code in ((local, 'int main(void) { return 0; }'), (imported, 'int main(void) { return 1; }')):
                (folder / 'main.c').write_text(code)
                (folder / 'lcom.json').write_text(json.dumps({'name': folder.name}))
            upstream = imported / 'UPSTREAM.json'
            upstream.write_text(json.dumps({'sourceHashes': {'main.c': hashlib.sha256((imported / 'main.c').read_bytes()).hexdigest()}}))
            (collection / 'lcom.collection.json').write_text(json.dumps({'projects': ['../../examples/local', '../../examples/community/imported']}))
            subprocess.run(['git', 'init', '-q', str(repo)], check=True, capture_output=True)
            subprocess.run(['git', '-C', str(repo), 'add', '.'], check=True, capture_output=True)
            with patch.object(checker, 'ROOT', repo):
                checker.check(collection, require_tracked=True)
                (local / 'missing.c').write_text('int missing;')
                with self.assertRaisesRegex(ValueError, 'not tracked'):
                    checker.check(collection, require_tracked=True)
                (local / 'missing.c').unlink()
                upstream.unlink()
                with self.assertRaisesRegex(ValueError, 'missing UPSTREAM'):
                    checker.check(collection, require_tracked=True)


if __name__ == '__main__':
    unittest.main()
