#!/usr/bin/env python3
"""Verify source export, relocation and optional native APK contents/signature."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import tempfile
import zipfile
import struct
import zlib
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[1]


def run(command, **kwargs):
    return subprocess.run([str(v) for v in command], check=True, **kwargs)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build-apk', action='store_true')
    parser.add_argument('--abi', action='append')
    args = parser.parse_args()
    cli = ROOT / 'build/machinelab'
    with tempfile.TemporaryDirectory(prefix='lcom native Android ') as temporary:
        work = Path(temporary)
        project = work / 'my game'
        project.mkdir()
        (project / 'main.c').write_text('#include <lcom/lcom.h>\nint main(void) { return lcom_init(); }\n')
        (project / 'assets').mkdir()
        (project / 'assets/a').write_text('bc')
        # A small real PNG exercises the user-supplied launcher asset path.
        def chunk(kind, data):
            return struct.pack('>I',len(data))+kind+data+struct.pack('>I',zlib.crc32(kind+data))
        icon = work / 'launcher.png'
        icon.write_bytes(b'\x89PNG\r\n\x1a\n'+chunk(b'IHDR',struct.pack('>IIBBBBB',64,64,8,2,0,0,0))+
                         chunk(b'IDAT',zlib.compress((b'\0'+bytes([20,120,80])*64)*64))+chunk(b'IEND',b''))
        original = (project / 'main.c').read_bytes()
        output = work / 'Android project'
        command = [cli,'export','android',project,'--output',output,'--app-id','pt.rgo.exporttest','--name','Game & "friends"','--icon',icon,'--orientation','landscape']
        run([*command,'--project-only'])
        manifest = ET.parse(output / 'app/src/main/AndroidManifest.xml')
        assert manifest.find('application').get('{http://schemas.android.com/apk/res/android}appCategory') == 'game'
        assert manifest.find('application').get('{http://schemas.android.com/apk/res/android}icon') == '@mipmap/ic_launcher'
        assert manifest.find('application/activity').get('{http://schemas.android.com/apk/res/android}screenOrientation') == 'sensorLandscape'
        assert (output / 'app/src/main/res/drawable-nodpi/app_icon.png').read_bytes() == icon.read_bytes()
        first_version = (output / 'game-assets/lcom-assets-version.txt').read_text()
        (project / 'assets/a').unlink()
        (project / 'assets/ab').write_text('c')
        run([*command,'--project-only'])
        assert (output / 'game-assets/lcom-assets-version.txt').read_text() != first_version
        relocated = work / 'relocated project'
        output.rename(relocated)
        metadata = json.loads((relocated / 'lcom-android.json').read_text())
        assert metadata['runtime'] == 'native-sdl3' and metadata['apk'] is None
        assert metadata['sources'][str((project / 'main.c').resolve())] == hashlib.sha256(original).hexdigest()
        # Delete the input to prove a fresh build uses only exported source/SDK files.
        shutil.rmtree(project)
        run(['cmake','-S',relocated / 'native','-B',work / 'desktop-build','-G','Ninja','-DCMAKE_BUILD_TYPE=Release'])
        run(['cmake','--build',work / 'desktop-build','--parallel','4'])
        protected = work / 'protected'
        protected.mkdir()
        (protected / 'keep.txt').write_text('keep me')
        project.mkdir()
        (project / 'main.c').write_bytes(original)
        for extra, expected in [(['--output',str(protected)],'empty directory'),(['--app-id','invalid'],'application ID')]:
            result = subprocess.run([str(v) for v in [*command,'--project-only',*extra]],capture_output=True,text=True)
            assert result.returncode != 0 and expected in result.stderr, result.stderr
        assert (protected / 'keep.txt').read_text() == 'keep me'
        # A creator profile supplies identity and controls, while CLI values win.
        (project / 'icon.png').write_bytes(icon.read_bytes())
        (project / 'lcom.json').write_text(json.dumps({'name':'Configured title 🎮','icon':'icon.png','preview':'icon.png','description':'A native game with portable artwork.','mobile':{'orientation':'portrait','fullscreen':True,'controls':{'keyboard':False,'mouseButtons':False,'quickKeys':[],'joystick':{'mode':'wasd','side':'left'},'look':{'side':'right'},'layout':'overlay'},'theme':{'font':'mono','accent':'#ff8844'}},'build':{'assets':[]}}))
        configured=work / 'configured Android'
        run([cli,'export','android',project,'--project-only','--output',configured,'--name','CLI title 🎮','--orientation','landscape'])
        assert json.loads((configured/'lcom-android.json').read_text())['name']=='CLI title 🎮'
        assert ET.parse(configured/'app/src/main/AndroidManifest.xml').find('application/activity').get('{http://schemas.android.com/apk/res/android}screenOrientation')=='sensorLandscape'
        resolved=json.loads((configured/'project.json').read_text())
        assert (configured/resolved['icon']).read_bytes()==icon.read_bytes()
        assert resolved['description']=='A native game with portable artwork.'
        assert (configured/resolved['preview']).read_bytes()==icon.read_bytes()
        assert not (configured/'game-assets/assets').exists()
        assert resolved['mobile']['controls']['keyboard'] is False and resolved['mobile']['theme']['font']=='mono'
        run(['cmake','-S',configured/'native','-B',work/'configured-build','-G','Ninja','-DCMAKE_BUILD_TYPE=Release'])
        run(['cmake','--build',work/'configured-build','--parallel','4'])

        # Installed CLI resources must suffice, including shared SDK source payload.
        import sys
        sys.path.insert(0,str(ROOT / 'scripts'))
        from package_student import copy_student_payload
        installed = work / 'installed'
        copy_student_payload(ROOT,ROOT / 'build',installed)
        (installed / 'bin').mkdir()
        installed_cli = installed / 'bin/lcom-ng'
        shutil.copy2(cli,installed_cli)
        # A relocated installed SDK exports multiple projects with colliding
        # source names. Each game's objects and initialized data remain separate.
        collection=work/'class collection'; collection.mkdir()
        folders=[]
        for number in (41,42):
            folder=work/('class game '+str(number)); folder.mkdir()
            (folder/'main.c').write_text('extern const int answer; int main(void) { return answer; }\n')
            (folder/'data.c').write_text('const int answer='+str(number)+';\n')
            (folder/'lcom.json').write_text(json.dumps({'name':'Game '+str(number)}))
            folders.append(folder)
        (collection/'lcom.collection.json').write_text(json.dumps({'projects':['../'+folder.name for folder in folders]}))
        collection_output=work/'collection export'
        run([installed_cli,'export','android',collection,'--project-only','--output',collection_output])
        for folder in [collection,*folders]: shutil.rmtree(folder)
        moved=work/'relocated collection'; collection_output.rename(moved)
        host_build=work/'collection host build'
        run(['cmake','-S',moved/'native','-B',host_build,'-G','Ninja','-DCMAKE_BUILD_TYPE=Release'])
        run(['cmake','--build',host_build,'--parallel','4'])
        catalog=json.loads((moved/'game-assets/collection.json').read_text())
        for number,game in zip((41,42),catalog['games']):
            library=next(path for path in host_build.rglob('lib'+game['library']+'.*') if path.suffix in ('.so','.dylib'))
            run([sys.executable,'-c','import ctypes,sys; assert ctypes.CDLL(sys.argv[1]).lcom_student_main()==int(sys.argv[2])',library,number])
        run([installed_cli,'export','android',ROOT / 'tests/fixtures/web/minix','--asset-root','..',
             '--asset','minix/assets','--project-only','--output',output])
        run(['cmake','-S',output / 'native','-B',work / 'legacy-build','-G','Ninja','-DCMAKE_BUILD_TYPE=Release'])
        run(['cmake','--build',work / 'legacy-build','--parallel','4'])
        assert (project / 'main.c').read_bytes() == original
        # Per-source Makefile includes may resolve the same header name differently.
        scoped = work / 'scoped includes'
        scoped.mkdir()
        (scoped / 'one').mkdir(); (scoped / 'two').mkdir()
        (scoped / 'one/value.h').write_text('#define VALUE 1\n')
        (scoped / 'two/value.h').write_text('#define VALUE 2\n')
        for name in ('one','two'):
            (scoped / (name+'.c')).write_text('#include <value.h>\nint '+name+'(void) { return VALUE; }\n')
        # Legacy headers can contain tentative const definitions. An initialized
        # data-only source must still reach the final app, as in the Makefile's
        # original object-file link. PvZ's board coordinates use this pattern.
        (scoped / 'constants.h').write_text('const int rows[3];\n')
        (scoped / 'constants.c').write_text('#include "constants.h"\nconst int rows[3]={100,200,300};\n')
        with (scoped / 'one.c').open('a') as source:
            source.write('#include "constants.h"\nint row_value(void) { return rows[1]; }\n')
        (scoped / 'main.c').write_text('#include <stdio.h>\n#ifndef FIRST_OPTION\n#error missing first split define\n#endif\n#ifndef SECOND_OPTION\n#error missing second split define\n#endif\nint one(void); int two(void); int row_value(void);\nint main(void) { int row=row_value(); printf("student row=%d\\n",row); return one()+two()==3 && row==200 && FIRST_OPTION+SECOND_OPTION==42 ? 0 : 9; }\n')
        (scoped / 'Makefile').write_text('PROG=scope\nSRCS=main.c one.c two.c constants.c\nCFLAGS += -D FIRST_OPTION=19 -D SECOND_OPTION=23\none.o: one.c\none.o: CFLAGS += -Ione\ntwo.o: two.c\ntwo.o: CFLAGS += -Itwo\n')
        scoped_export = work / 'scoped export'
        run([cli,'export','android',scoped,'--project-only','--output',scoped_export])
        shutil.rmtree(scoped)
        run(['cmake','-S',scoped_export / 'native','-B',work / 'scoped-build','-G','Ninja','-DCMAKE_BUILD_TYPE=Release'])
        run(['cmake','--build',work / 'scoped-build','--parallel','4'])
        run([work / 'scoped-build/main',scoped_export / 'game-assets'])
    if args.build_apk:
        output = ROOT / 'build/android-qa/smoke'
        command = [cli,'export','android',ROOT / 'tests/fixtures/web/native','--output',output,'--app-id','pt.rgo.lcomng.smoke']
        for abi in args.abi or ['arm64-v8a']: command += ['--abi',abi]
        run(command)
        machine_ids = {'arm64-v8a':183,'x86_64':62,'armeabi-v7a':40}
        with zipfile.ZipFile(output / 'game.apk') as archive:
            assert not any(n.endswith(('.wasm','.js','.html')) for n in archive.namelist())
            assert archive.read('assets/assets/message.txt') == (ROOT / 'tests/fixtures/web/native/assets/message.txt').read_bytes()
            for abi in args.abi or ['arm64-v8a']:
                binary = archive.read(f'lib/{abi}/libmain.so')
                assert binary[:4] == b'\x7fELF' and int.from_bytes(binary[18:20],'little') == machine_ids[abi]
                assert f'lib/{abi}/libSDL3.so' in archive.namelist()
        sdk = Path(os.environ.get('ANDROID_HOME') or os.environ['ANDROID_SDK_ROOT'])
        tools = sdk / 'build-tools/36.0.0'
        run([tools / 'apksigner','verify','--verbose',output / 'game.apk'])
        run([tools / 'zipalign','-c','-P','16','-v','4',output / 'game.apk'],stdout=subprocess.DEVNULL)
        direct = ROOT / 'build/android-qa/direct.json'
        direct.write_text(json.dumps({'mobile':{'orientation':'landscape','fullscreen':True,'controls':{
            'pointer':'direct','layout':'overlay','keyboard':False,'mouseButtons':False,'quickKeys':[],'joystick':False}}}))
        direct_command=[cli,'export','android',ROOT/'tests/fixtures/web/native','--output',ROOT/'build/android-qa/direct',
                        '--app-id','pt.rgo.lcomng.smoke.direct','--config',direct]
        for abi in args.abi or ['arm64-v8a']: direct_command += ['--abi',abi]
        run(direct_command)
        run([cli,'export','android',ROOT/'tests/fixtures/android/coordinates',
             '--output',ROOT/'build/android-qa/coordinates','--app-id','pt.rgo.lcomng.smoke.coordinates',
             *[value for abi in args.abi or ['arm64-v8a'] for value in ('--abi',abi)]])
    print('Native Android export checks passed')


if __name__ == '__main__':
    main()
