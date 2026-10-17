"""Exercise a real Minix export while a later build replaces mutable archives."""
import argparse
import importlib.util
import os
from pathlib import Path
import sys
import tempfile

parser = argparse.ArgumentParser(__doc__)
parser.add_argument('--sdk', type=Path, required=True)
parser.add_argument('--project', type=Path, required=True)
parser.add_argument('--output', type=Path, required=True)
args = parser.parse_args()
spec = importlib.util.spec_from_file_location('cache_export', args.sdk / 'web/export.py')
exporter = importlib.util.module_from_spec(spec)
spec.loader.exec_module(exporter)
run = exporter.run
removed = []
with tempfile.TemporaryDirectory(prefix='lcom-replaced-runtime-cache-') as temporary:
    cache = Path(temporary)
    os.environ['XDG_CACHE_HOME'] = str(cache)

    def replace_cache_before_link(command, **kwargs):
        if str(command[0]) == 'em++':
            # A concurrent CMake rebuild can remove these shared outputs after
            # the export took its snapshot. Leave that private snapshot intact.
            archives = [path for path in cache.rglob('*.a')
                        if path.name in ('liblcom_minix.a', 'libmachinelab.a')]
            assert len(archives) == 2, archives
            for path in archives:
                path.unlink()
                removed.append(str(path))
        return run(command, **kwargs)

    exporter.run = replace_cache_before_link
    sys.argv = [str(args.sdk / 'web/export.py'), str(args.project), '--output', str(args.output)]
    result = exporter.main()
    assert result == 0 and len(removed) == 2, (result, removed)
    assert (args.output / 'game.wasm').is_file()
    print('Minix export retained its runtime snapshot after shared cache replacement')
