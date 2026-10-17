"""Package a library of unchanged C projects into one native Android application."""
import copy
import hashlib
import html
import importlib.util
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tempfile
import zipfile

SDK = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(SDK / 'export'))
import project as source_project

_spec = importlib.util.spec_from_file_location('lcom_android_export', SDK / 'android/export.py')
android_export = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(android_export)


def build(entries, output: Path, name: str, icon: Path | None, app_id: str,
          abis: list[str], project_only: bool = False) -> Path:
    """Build once, sharing SDK compilation and isolating each game's process/state."""
    output = Path(output).resolve()
    name = source_project.project_config.string(name, 'collection name')
    if not re.fullmatch(r'[A-Za-z][A-Za-z0-9_]*(?:\.[A-Za-z][A-Za-z0-9_]*)+', app_id):
        raise ValueError('Invalid Android application ID')
    if not entries:
        raise ValueError('An Android collection needs at least one game')
    ids = [entry['id'] for entry in entries]
    if len(set(ids)) != len(ids) or any(not re.fullmatch(r'[a-z][a-z0-9_-]*', item) for item in ids):
        raise ValueError('Collection IDs must be unique lowercase slugs beginning with a letter')
    # Underscore-escaped target names remain unique even for IDs such as a-b/a_b.
    libraries = {item: 'lcom_game_' + item.encode().hex() for item in ids}
    abis = abis or ['arm64-v8a']
    if any(abi not in ('arm64-v8a', 'x86_64', 'armeabi-v7a') for abi in abis):
        raise ValueError('Unsupported Android ABI')
    if any(Path(entry['project']).resolve().is_relative_to(output) for entry in entries):
        raise ValueError('The collection output cannot contain a source project')
    if output.exists() and (not output.is_dir() or
            (any(output.iterdir()) and not (output / 'lcom-collection-android.json').is_file())):
        raise ValueError('Output is not an existing Android collection; choose an empty directory')
    if not project_only:
        sdk_home = os.environ.get('ANDROID_HOME') or os.environ.get('ANDROID_SDK_ROOT')
        if not sdk_home or not (Path(sdk_home) / 'platforms/android-36/android.jar').is_file():
            raise ValueError('Install Android SDK Platform 36 and set ANDROID_HOME')
        if not (Path(sdk_home) / 'ndk' / android_export.NDK_VERSION / 'source.properties').is_file():
            raise ValueError(f'Install Android NDK {android_export.NDK_VERSION}')
        if not shutil.which('java'):
            raise ValueError('Android builds need JDK 17 or newer')
    icon_bytes = source_project.project_config.icon_path(icon).read_bytes() if icon else None
    output.parent.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix='.lcom-collection-', dir=output.parent) as temporary:
        work = Path(temporary)
        shutil.copytree(SDK / 'android/template', work, dirs_exist_ok=True)
        # Collection-specific Java and manifest replace only the standalone host.
        shutil.copytree(Path(__file__).parent / 'android-template', work, dirs_exist_ok=True)
        (work / 'native-CMakeLists.txt').unlink()
        runtime = SDK.parent / 'runtime'
        if not (runtime / 'core/Machine.cpp').is_file():
            runtime = SDK / 'runtime'
        for folder in ('include', 'compat', 'client'):
            shutil.copytree(SDK / folder, work / 'sdk' / folder,
                            ignore=shutil.ignore_patterns('__pycache__', '*.pyc'))
        for folder in ('core', 'devices', 'backends', 'embedded'):
            shutil.copytree(runtime / folder, work / 'sdk/runtime' / folder)
        license_path = SDK.parent / 'LICENSE'
        if not license_path.is_file(): license_path = SDK.parent / 'licenses/lcom-ng-LICENSE'
        shutil.copy2(license_path, work / 'sdk/LICENSE')
        libs = work / 'app/libs'
        libs.mkdir(exist_ok=True)
        with zipfile.ZipFile(android_export.sdl_archive()) as archive:
            (libs / 'SDL3.aar').write_bytes(archive.read(f'SDL3-{android_export.SDL_VERSION}.aar'))
            (libs / 'SDL3-LICENSE.txt').write_bytes(archive.read('LICENSE.txt'))
        assets = work / 'game-assets'
        assets.mkdir()
        (assets / 'icons').mkdir()
        catalog, metadata = [], []
        games = work / 'native/games'
        games.mkdir(parents=True)
        for entry in entries:
            identifier = entry['id']
            print(f'Preparing native collection game: {identifier}', flush=True)
            settings = copy.deepcopy(entry['settings'])
            settings['name'] = entry['name']
            # Collections are immersive; authored control placement/orientation stays intact.
            settings['mobile']['fullscreen'] = True
            args = source_project.argument_parser('').parse_args([str(entry['project'])])
            source_project.project_config.resolve(args)
            asset_root = (args.project / (args.asset_root or Path('.'))).resolve()
            selected_assets = args.asset if args.assets_configured else args.asset or (['assets'] if (args.project / 'assets').is_dir() else [])
            if any(output.is_relative_to((asset_root / item).resolve()) for item in selected_assets):
                raise ValueError('Collection output cannot be inside a selected game asset directory')
            args.name = entry['name']
            scratch = work / ('entry-' + identifier)
            scratch.mkdir()
            prefix = libraries[identifier] + '_'
            info = source_project.write_native_project(args, scratch, target_prefix=prefix)
            source_project.project_config.write_native(settings, scratch / 'native/GameConfig.hpp')
            game_native = games / identifier
            (scratch / 'native').rename(game_native)
            (assets / 'games').mkdir(exist_ok=True)
            (scratch / 'game-assets').rename(assets / 'games' / identifier)
            shutil.rmtree(scratch)
            android_export.write_asset_manifest(assets / 'games' / identifier)
            library = libraries[identifier]
            (game_native / 'CMakeLists.txt').write_text(
                'include(student.cmake)\n'
                f'add_library({library} SHARED "${{LCOM_RUNTIME}}/embedded/Player.cpp")\n'
                f'target_include_directories({library} PRIVATE "${{CMAKE_CURRENT_LIST_DIR}}")\n'
                'if(ANDROID)\n'
                f'  target_link_options({library} PRIVATE "-Wl,-z,max-page-size=16384" "-Wl,--no-undefined")\n'
                f'  target_link_libraries({library} PRIVATE log)\n'
                'endif()\n'
                f'target_link_libraries({library} PRIVATE {prefix}student compat m)\n')
            icon_name = None
            if entry.get('icon'):
                icon_path = source_project.project_config.icon_path(entry['icon'])
                icon_name = f'icons/{identifier}.png'
                shutil.copy2(icon_path, assets / icon_name)
            catalog.append({'id': identifier, 'name': entry['name'], 'icon': icon_name,
                            'library': library, 'archetype': settings['archetype'], 'orientation': settings['mobile']['orientation']})
            metadata.append({'id': identifier, **info})
        with (work / 'native/CMakeLists.txt').open('a') as cmake:
            for identifier in ids:
                cmake.write(f'add_subdirectory(games/{identifier})\n')
        (assets / 'collection.json').write_text(json.dumps({'name': name, 'games': catalog}, ensure_ascii=False, indent=2) + '\n')
        build_file = work / 'app/build.gradle'
        build_file.write_text(build_file.read_text().replace('@APP_ID@', app_id)
                              .replace('@NDK_VERSION@', android_export.NDK_VERSION)
                              .replace('@ABIS@', ', '.join(repr(abi) for abi in abis)))
        label = name.replace('\\', '\\\\').replace('"', '\\"').replace("'", "\\'").replace('\n', '\\n')
        resources = work / 'app/src/main/res'
        (resources / 'values').mkdir(parents=True, exist_ok=True)
        (resources / 'values/strings.xml').write_text(
            '<resources><string name="app_name">"' + html.escape(label, quote=False) + '"</string></resources>\n')
        if icon_bytes:
            (resources / 'drawable-nodpi').mkdir(exist_ok=True)
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
            manifest = work / 'app/src/main/AndroidManifest.xml'
            manifest.write_text(manifest.read_text().replace('<application ', '<application android:icon="@mipmap/ic_launcher" android:roundIcon="@mipmap/ic_launcher" '))
        (work / 'gradlew').chmod(0o755)
        result = {'format': 1, 'runtime': 'native-sdl3-collection', 'name': name,
                  'applicationId': app_id, 'abis': abis, 'games': metadata, 'apk': None}
        if not project_only:
            subprocess.run([str(work / 'gradlew'), '--no-daemon', '--console=plain', 'assembleDebug'], cwd=work, check=True)
            android_export.finalize_apk(work, 'collection.apk')
            result['apk'] = 'collection.apk'
            for folder in ('.gradle', 'app/build', 'app/.cxx'):
                shutil.rmtree(work / folder, ignore_errors=True)
        for game in metadata:
            for path, digest in game['sources'].items():
                if hashlib.sha256(Path(path).read_bytes()).hexdigest() != digest:
                    raise ValueError(f'Source changed during export: {path}')
        (work / 'lcom-collection-android.json').write_text(json.dumps(result, indent=2) + '\n')
        backup = work.parent / (work.name + '-previous')
        if output.exists(): output.rename(backup)
        try:
            work.rename(output)
        except OSError:
            if backup.exists(): backup.rename(output)
            raise
        shutil.rmtree(backup, ignore_errors=True)
    return output
