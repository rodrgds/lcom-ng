#!/usr/bin/env python3
"""Selected original assets must reach standalone and collection APKs unchanged."""
import hashlib
import json
from pathlib import Path
import subprocess
import shutil
import sys
import tempfile
import zipfile

ROOT = Path(__file__).resolve().parents[1]


def main():
    # These legal names are used by student games. Android's default asset
    # filters silently remove dot files and underscore-prefixed directories.
    payloads = {
        'images/characters/..bmp': b'original punctuation glyph\x00\xff',
        'images/characters/.bmp': b'original dot glyph\x01\xfe',
        '_sprites/.tile': b'original hidden tile\x02\xfd',
        'ordinary.dat': b'ordinary asset control\x03\xfc',
    }
    metadata = {'Thumbs.db', '.DS_Store', '.git/config', 'CVS/Entries', 'picasa.ini', 'scratch~', 'old.scc'}
    failures = []
    with tempfile.TemporaryDirectory(prefix='lcom Android asset packaging ') as temporary:
        work = Path(temporary)
        project = work/'original game'
        project.mkdir()
        (project/'main.c').write_text('int main(void) { return 0; }\n')
        for name, data in payloads.items():
            asset = project/'assets'/name
            asset.parent.mkdir(parents=True, exist_ok=True)
            asset.write_bytes(data)
        for name in metadata:
            asset = project/'assets'/name
            asset.parent.mkdir(parents=True, exist_ok=True)
            asset.write_bytes(b'OS or VCS metadata')
        collection = work/'collection'
        collection.mkdir()
        (collection/'lcom.collection.json').write_text(json.dumps({'projects':['../original game']}))
        for kind, source in (('standalone', project), ('collection', collection)):
            output = work/('export-'+kind)
            subprocess.run([sys.executable, str(ROOT/'sdk/android/export.py'), str(source),
                            '--output', str(output), '--abi', 'x86_64',
                            '--app-id', 'pt.rgo.lcomng.assets.'+kind], check=True)
            apk = output/('game.apk' if kind == 'standalone' else 'collection.apk')
            with apk.open('rb') as stream:
                print(kind, 'APK SHA256', hashlib.file_digest(stream, 'sha256').hexdigest(), flush=True)
            prefix = 'assets/'
            if kind == 'collection':
                catalog = json.loads((output/'game-assets/collection.json').read_text())
                prefix += 'games/'+catalog['games'][0]['id']+'/'
            with zipfile.ZipFile(apk) as archive:
                declared = archive.read(prefix+'lcom-assets.txt').decode().splitlines()
                for name in metadata:
                    assert 'assets/'+name not in declared, (kind, name, 'metadata listed for unavailable extraction')
                for name, expected in payloads.items():
                    relative = 'assets/'+name
                    assert relative in declared, (kind, relative, 'not declared')
                    try:
                        actual = archive.read(prefix+relative)
                    except KeyError:
                        failures.append(f'{kind}: declared original asset missing from APK: {relative}')
                        continue
                    assert actual == expected, (kind, relative, 'changed bytes')
                    print(kind, relative, hashlib.sha256(actual).hexdigest(), flush=True)
            # No generated native objects survive this packaging regression.
            shutil.rmtree(output)
    assert not failures, '\n'.join(failures)
    print('Standalone and collection APKs preserve every declared test asset byte.')


if __name__ == '__main__':
    main()
