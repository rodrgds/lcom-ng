#!/usr/bin/env python3
"""A built APK must be publishable without another APK-sized write allocation."""
import hashlib
import importlib.util
from pathlib import Path
import signal
import tempfile

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('android_export', ROOT/'sdk/android/export.py')
exporter = importlib.util.module_from_spec(spec)
spec.loader.exec_module(exporter)

with tempfile.TemporaryDirectory(prefix='lcom APK publication ') as temporary:
    work = Path(temporary)
    original = work/'app/build/outputs/apk/debug/app-debug.apk'
    original.parent.mkdir(parents=True)
    payload = b'An already built signed APK must not require another full copy.\n' * 4096
    original.write_bytes(payload)
    # Use the OS write limit rather than mocking copy/rename operations. The
    # completed input is larger than the remaining per-file write budget.
    try:
        import resource
    except ImportError:
        resource = None
    if resource:
        previous = resource.getrlimit(resource.RLIMIT_FSIZE)
        signal.signal(signal.SIGXFSZ, signal.SIG_IGN)
        resource.setrlimit(resource.RLIMIT_FSIZE, (65536, previous[1]))
    try:
        exporter.finalize_apk(work, 'collection.apk')
    finally:
        if resource:
            resource.setrlimit(resource.RLIMIT_FSIZE, previous)
    assert (work/'collection.apk').read_bytes() == payload
    assert (work/'collection.apk.sha256').read_text() == hashlib.sha256(payload).hexdigest()+'  collection.apk\n'
print('Completed APK publishes within the bounded write budget, with exact bytes and checksum.')
