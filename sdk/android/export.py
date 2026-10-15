#!/usr/bin/env python3
"""Rebuild a C game as a native SDL3 Android app and signed debug APK."""
import hashlib
import html
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tempfile
import urllib.request
import zipfile
import xml.etree.ElementTree as ET

ANDROID = Path(__file__).resolve().parent
SDK = ANDROID.parent
sys.path.insert(0, str(SDK / 'export'))
import project as source_project

SDL_VERSION = '3.2.28'
SDL_SHA256 = 'd815aec61fa62e5e633a17410af70716a84bd5a302e2c39070ee471d31045590'
NDK_VERSION = '27.1.12297006'


def argument_parser():
    parser = source_project.argument_parser(__doc__)
    parser.add_argument('--app-id', help='Android application ID (default: pt.rgo.lcomng.game.PROJECT)')
    parser.add_argument('--abi', action='append', choices=['arm64-v8a', 'x86_64', 'armeabi-v7a'], help='native architecture; repeatable (default: arm64-v8a)')
    parser.add_argument('--project-only', action='store_true', help='generate the relocatable Android project without building its APK')
    return parser


def sdl_archive():
    cache = Path(os.environ.get('XDG_CACHE_HOME', Path.home() / '.cache')) / 'lcom-ng/android'
    cache.mkdir(parents=True, exist_ok=True)
    archive = cache / f'SDL3-devel-{SDL_VERSION}-android.zip'
    if not archive.is_file() or hashlib.sha256(archive.read_bytes()).hexdigest() != SDL_SHA256:
        with tempfile.NamedTemporaryFile(dir=cache, delete=False) as target:
            temporary = Path(target.name)
        try:
            print(f'Downloading SDL3 {SDL_VERSION} Android libraries...', flush=True)
            urllib.request.urlretrieve(f'https://github.com/libsdl-org/SDL/releases/download/release-{SDL_VERSION}/{archive.name}', temporary)
            if hashlib.sha256(temporary.read_bytes()).hexdigest() != SDL_SHA256:
                raise source_project.compat.BuildError('SDL3 archive checksum mismatch')
            temporary.replace(archive)
        finally:
            temporary.unlink(missing_ok=True)
    return archive


def build(args):
    if (args.project / 'lcom.collection.json').is_file():
        sys.path.insert(0, str(SDK))
        from collection import manifest
        return manifest.build(args, 'android')
    settings = source_project.project_config.resolve(args)
    project = args.project.resolve(strict=True)
    output = (args.output or project / '.lcom-ng/android').resolve()
    if not project.is_dir():
        raise source_project.compat.BuildError('Project must be a source directory, not a compiled executable')
    if output == project or project.is_relative_to(output):
        raise source_project.compat.BuildError('Output must be a separate directory, not the project or its parent')
    if output.exists() and (not output.is_dir() or
            (any(output.iterdir()) and not (output / 'lcom-android.json').is_file())):
        raise source_project.compat.BuildError('Output is not an existing Android export; choose an empty directory')
    slug = re.sub('[^a-z0-9_]', '_', project.name.lower())
    app_id = args.app_id or 'pt.rgo.lcomng.game.g_' + slug
    if not re.fullmatch(r'[A-Za-z][A-Za-z0-9_]*(?:\.[A-Za-z][A-Za-z0-9_]*)+', app_id):
        raise source_project.compat.BuildError('Invalid Android application ID; use at least two dot-separated names starting with letters')
    icon_bytes = None
    if args.icon:
        icon_bytes = args.icon.resolve(strict=True).read_bytes()
    if not args.project_only:
        sdk = os.environ.get('ANDROID_HOME') or os.environ.get('ANDROID_SDK_ROOT')
        if not sdk or not (Path(sdk) / 'platforms/android-36/android.jar').is_file():
            raise source_project.compat.BuildError('Install Android SDK Platform 36 and set ANDROID_HOME. Use --project-only to export without Android tools.')
        if not (Path(sdk) / 'ndk' / NDK_VERSION / 'source.properties').is_file():
            raise source_project.compat.BuildError(f'Install Android NDK {NDK_VERSION}. Use --project-only to export without Android tools.')
        if not shutil.which('java'):
            raise source_project.compat.BuildError('APK builds need JDK 17 or newer (JAVA_HOME/PATH).')
    asset_root = (project / (args.asset_root or Path('.'))).resolve()
    assets = args.asset if args.assets_configured else args.asset or ([(project / 'assets').relative_to(asset_root).as_posix()] if (project / 'assets').is_dir() and project.is_relative_to(asset_root) else [])
    for item in assets:
        source = (asset_root / item).resolve()
        if output.is_relative_to(source):
            raise source_project.compat.BuildError('Output cannot be inside a selected asset directory')
    output.parent.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix='.lcom-android-', dir=output.parent) as temporary:
        work = Path(temporary)
        shutil.copytree(ANDROID / 'template', work, dirs_exist_ok=True)
        info = source_project.write_native_project(args, work)
        source_project.project_config.write_native(settings, work / 'native/GameConfig.hpp')
        (work / 'native-CMakeLists.txt').replace(work / 'native/CMakeLists.txt')
        libraries = work / 'app/libs'
        libraries.mkdir()
        with zipfile.ZipFile(sdl_archive()) as archive:
            (libraries / 'SDL3.aar').write_bytes(archive.read(f'SDL3-{SDL_VERSION}.aar'))
            (libraries / 'SDL3-LICENSE.txt').write_bytes(archive.read('LICENSE.txt'))
        for folder in ('include', 'compat', 'client'):
            shutil.copytree(SDK / folder, work / 'sdk' / folder, ignore=shutil.ignore_patterns('__pycache__', '*.pyc'))
        runtime = SDK.parent / 'runtime'
        if not (runtime / 'core/Machine.cpp').is_file(): runtime = SDK / 'runtime'
        for folder in ('core', 'devices', 'backends', 'embedded'):
            shutil.copytree(runtime / folder, work / 'sdk/runtime' / folder)
        shutil.copy2(SDK.parent / 'LICENSE' if (SDK.parent / 'LICENSE').is_file() else SDK.parent / 'licenses/lcom-ng-LICENSE', work / 'sdk/LICENSE')
        build_file = work / 'app/build.gradle'
        build_file.write_text(build_file.read_text().replace('@APP_ID@', app_id)
                              .replace('@NDK_VERSION@', NDK_VERSION)
                              .replace('@ABIS@', ', '.join("'" + abi + "'" for abi in (args.abi or ['arm64-v8a']))))
        name = info['name']
        label = name.replace('\\', '\\\\').replace('"', '\\"').replace("'", "\\'").replace('\n', '\\n')
        resources = work / 'app/src/main/res'
        (resources / 'values').mkdir(parents=True, exist_ok=True)
        (resources / 'values/strings.xml').write_text(
            '<resources><string name="app_name">"' + html.escape(label, quote=False) + '"</string></resources>\n')
        if icon_bytes:
            (work / 'icon.png').write_bytes(icon_bytes)
            (resources / 'drawable-nodpi').mkdir(parents=True)
            (resources / 'drawable-nodpi/app_icon.png').write_bytes(icon_bytes)
            (resources / 'drawable').mkdir(exist_ok=True)
            (resources / 'drawable/ic_launcher_foreground.xml').write_text(
                '<inset xmlns:android="http://schemas.android.com/apk/res/android" android:inset="12dp">'
                '<bitmap android:src="@drawable/app_icon" android:gravity="fill" /></inset>\n')
            (resources / 'mipmap-anydpi-v26').mkdir(exist_ok=True)
            (resources / 'mipmap-anydpi-v26/ic_launcher.xml').write_text(
                '<adaptive-icon xmlns:android="http://schemas.android.com/apk/res/android">'
                '<background android:drawable="@android:color/black" />'
                '<foreground android:drawable="@drawable/ic_launcher_foreground" /></adaptive-icon>\n')
        if icon_bytes or args.orientation != 'auto':
            manifest = work / 'app/src/main/AndroidManifest.xml'
            ET.register_namespace('android', 'http://schemas.android.com/apk/res/android')
            tree = ET.parse(manifest)
            app = tree.find('application')
            if icon_bytes:
                for attribute in ('icon', 'roundIcon'):
                    app.set('{http://schemas.android.com/apk/res/android}' + attribute, '@mipmap/ic_launcher')
            if args.orientation != 'auto':
                value = 'sensorLandscape' if args.orientation == 'landscape' else 'sensorPortrait'
                app.find('activity').set('{http://schemas.android.com/apk/res/android}screenOrientation', value)
            tree.write(manifest, encoding='utf-8', xml_declaration=True)
        assets = work / 'game-assets'
        names = sorted(p.relative_to(assets).as_posix() for p in assets.rglob('*') if p.is_file())
        if any('\n' in name or '\r' in name for name in names) or 'lcom-assets.txt' in names or 'lcom-assets-version.txt' in names:
            raise source_project.compat.BuildError('Asset filenames conflict with the Android asset manifest')
        version = hashlib.sha256()
        for name in names:
            version.update(name.encode() + b'\0')
            version.update(hashlib.sha256((assets / name).read_bytes()).digest())
        (assets / 'lcom-assets.txt').write_text('\n'.join(names) + ('\n' if names else ''))
        (assets / 'lcom-assets-version.txt').write_text(version.hexdigest())
        (work / 'gradlew').chmod(0o755)
        if settings['preview']:
            shutil.copy2(settings['preview'], work / 'preview.png')
        (work / 'project.json').write_text(json.dumps(source_project.project_config.public(settings), indent=2) + '\n')
        metadata = {'format': 1, 'runtime': 'native-sdl3', 'applicationId': app_id,
                    'abis': args.abi or ['arm64-v8a'], **info, 'apk': None, 'orientation': args.orientation,
                    'iconSha256': hashlib.sha256(icon_bytes).hexdigest() if icon_bytes else None}
        if not args.project_only:
            subprocess.run([str(work / 'gradlew'), '--no-daemon', '--console=plain', 'assembleDebug'], cwd=work, check=True)
            shutil.copy2(work / 'app/build/outputs/apk/debug/app-debug.apk', work / 'game.apk')
            metadata['apk'] = 'game.apk'
            digest = hashlib.sha256((work / 'game.apk').read_bytes()).hexdigest()
            (work / 'game.apk.sha256').write_text(digest + '  game.apk\n')
            for folder in ('.gradle', 'app/build', 'app/.cxx'): shutil.rmtree(work / folder, ignore_errors=True)
        for source, digest in info['sources'].items():
            if hashlib.sha256(Path(source).read_bytes()).hexdigest() != digest:
                raise source_project.compat.BuildError(f'Source changed during export: {source}')
        (work / 'lcom-android.json').write_text(json.dumps(metadata, indent=2) + '\n')
        # Stage next to the output, replacing it only after every compiler succeeds.
        # A failed rebuild preserves the last successful APK and source project.
        backup = work.parent / (work.name + '-previous')
        if output.exists(): output.rename(backup)
        try:
            work.rename(output)
        except OSError:
            if backup.exists(): backup.rename(output)
            raise
        shutil.rmtree(backup, ignore_errors=True)
    return output


def main():
    args = argument_parser().parse_args()
    try:
        output = build(args)
        if args.project_only:
            print(f'Created native Android project at {output}\nBuild its APK with ./gradlew assembleDebug from that directory.')
        else:
            apk = output / ("collection.apk" if (output / "lcom-collection-android.json").is_file() else "game.apk")
            print(f'Created signed native debug APK at {apk}\nInstall with: adb install -r "{apk}"')
        return 0
    except (source_project.compat.BuildError, OSError, ValueError, subprocess.CalledProcessError) as error:
        print(f'lcom-ng android: {error}', file=sys.stderr)
        return 1


if __name__ == '__main__':
    sys.exit(main())
