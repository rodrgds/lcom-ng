#!/usr/bin/env python3
"""Public lcom.json contract: defaults, validation, paths, overrides and bundles."""
import argparse
import importlib.util
import json
import os
import shutil
from pathlib import Path
import struct
import subprocess
import sys
import tempfile
import unittest
import zlib

ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'sdk/export'))
import config


def png(path):
    def chunk(kind,data): return struct.pack('>I',len(data))+kind+data+struct.pack('>I',zlib.crc32(kind+data))
    path.write_bytes(b'\x89PNG\r\n\x1a\n'+chunk(b'IHDR',struct.pack('>IIBBBBB',64,64,8,2,0,0,0))+chunk(b'IDAT',zlib.compress((b'\0'+b'\x10\x80\x20'*64)*64))+chunk(b'IEND',b''))


class ProjectConfig(unittest.TestCase):
    def setUp(self):
        self.tmp=tempfile.TemporaryDirectory(prefix='lcom config ')
        self.root=Path(self.tmp.name); self.project=self.root/'game'; self.project.mkdir()
    def tearDown(self): self.tmp.cleanup()
    def write(self,data): (self.project/'lcom.json').write_text(json.dumps(data))

    def test_missing_is_optional_but_explicit_missing_is_error(self):
        settings=config.load(self.project)
        self.assertEqual(settings['name'],'game')
        self.assertEqual(settings['mobile']['orientation'],'auto')
        self.assertTrue(settings['mobile']['controls']['keyboard'])
        with self.assertRaises(FileNotFoundError): config.load(self.project,self.root/'missing.json')
        settings['mobile']['controls']['quickKeys'].clear()
        self.assertEqual(config.load(self.project)['mobile']['controls']['quickKeys'],['ESC','SPACE','ENTER'])

    def test_partial_objects_and_false_values_preserve_other_defaults(self):
        self.write({'name':'Sushi Slash','mobile':{'fullscreen':True,'controls':{'keyboard':False,'joystick':False,'mouseButtons':False,'quickKeys':[]},'theme':{'accent':'#ff3344','font':'mono'}}})
        s=config.load(self.project)
        self.assertFalse(s['mobile']['controls']['keyboard'])
        self.assertFalse(s['mobile']['controls']['joystick'])
        self.assertEqual(s['mobile']['controls']['pointer'],'drag')
        self.assertEqual(s['mobile']['controls']['quickKeys'],[])
        self.assertEqual(s['mobile']['theme']['keyboardAccent'],'#ff3344')

    def test_punctuation_actions_preserve_guest_key_names(self):
        for key in ('MINUS','EQUAL','LBRACKET','RBRACKET','BACKSLASH','SEMICOLON',
                    'APOSTROPHE','BACKQUOTE','COMMA','PERIOD','SLASH'):
            with self.subTest(key=key):
                self.write({'mobile':{'controls':{'quickKeys':[key],
                    'actions':[{'position':'top-right','label':'Punctuation','key':key}]}}})
                controls=config.load(self.project)['mobile']['controls']
                self.assertEqual(controls['quickKeys'],[key])
                self.assertEqual(controls['actions'][0]['key'],key)

    def test_actionable_validation(self):
        for data,field in [({'mobile':{'controls':{'keyboard':'false'}}},'keyboard'),
                           ({'mobile':{'orentation':'landscape'}},'orentation'),
                           ({'mobile':{'orientation':'sideways'}},'orientation'),
                           ({'mobile':{'theme':{'accent':'red'}}},'accent'),
                           ({'mobile':{'controls':{'quickKeys':['BOGUS']}}},'quickKeys'),
                           ({'mobile':{'controls':{'look':{'sensitivity':False}}}},'sensitivity'),
                           ({'mobile':{'controls':{'cursor':{'x':1.5}}}},'cursor.x'),
                           ({'mobile':{'controls':{'cursor':{'x':True}}}},'cursor.x'),
                           ({'mobile':{'controls':{'cursor':{'desktopScale':True}}}},'desktopScale'),
                           ({'mobile':{'controls':{'cursor':{'desktopFractional':1}}}},'desktopFractional'),
                           ({'mobile':{'controls':{'cursor':{'desktopScale':0}}}},'desktopScale'),
                           ({'mobile':{'controls':{'cursor':{'desktopScale':0.7}}}},'desktopScale'),
                           ({'mobile':{'controls':{'cursor':{'x':401,'maxX':400}}}},'cursor.x'),
                           ({'mobile':{'controls':{'joystick':None}}},'joystick'),
                           ({'mobile':{'controls':{'joystick':{'side':'right'},'look':True}}},'side'),
                           ({'description':False},'description'),
                           ({'description':'x'*501},'description'),
                           ({'preview':'missing.png'},'preview'),
                           ({'version':2},'version')]:
            with self.subTest(data=data):
                self.write(data)
                with self.assertRaisesRegex(ValueError,field): config.load(self.project)

    def test_config_icon_path_and_cli_overrides(self):
        (self.root/'settings').mkdir(); png(self.root/'settings/icon.png')
        settings_path=self.root/'settings/custom.json'
        settings_path.write_text(json.dumps({'name':'File title','icon':'icon.png','preview':'icon.png','description':'Slice sushi before it falls.','mobile':{'orientation':'landscape'},'build':{'sources':['main.c'],'arguments':['--practice']}}))
        args=argparse.Namespace(project=self.project,config=settings_path,name='CLI title',icon=None,orientation='portrait',source=[],include=[],asset=[],asset_root=None,arg=['--cli'],library=[])
        s=config.resolve(args)
        self.assertEqual(args.icon,(self.root/'settings/icon.png').resolve())
        self.assertEqual(s['name'],'CLI title')
        self.assertEqual(s['preview'], (self.root/'settings/icon.png').resolve())
        self.assertEqual(config.public(s)['description'], 'Slice sushi before it falls.')
        self.assertEqual(config.public(s)['preview'], 'preview.png')
        self.assertEqual(s['mobile']['orientation'],'portrait')
        self.assertEqual(args.arg,['--cli'])
        self.assertEqual(args.source,['main.c'])

    def test_archetype_is_optional_and_validated(self):
        self.assertIsNone(config.load(self.project)['archetype'])
        self.write({'archetype': ' Space Invaders '})
        self.assertEqual(config.public(config.load(self.project))['archetype'], 'Space Invaders')
        for value in (False, '', '  ', 'x' * 65, 'Snake\nPong'):
            self.write({'archetype': value})
            with self.subTest(value=value), self.assertRaisesRegex(ValueError, 'archetype'):
                config.load(self.project)

    def test_pads_do_not_overlap_the_optional_mouse_stick(self):
        self.write({'mobile':{'controls':{'dpad':True}}})
        self.assertFalse(config.load(self.project)['mobile']['controls']['joystick'])
        self.write({'mobile':{'controls':{'dpad':True,'joystick':True}}})
        self.assertEqual(config.load(self.project)['mobile']['controls']['joystick'],{'mode':'mouse','side':'right'})

    def test_coarse_text_mouse_sensitivity_reaches_both_exports(self):
        self.write({'mobile': {'controls': {'joystick': False, 'pads': [
            {'side': 'right', 'mode': 'mouse', 'style': 'stick', 'sensitivity': 10}]}}})
        settings = config.load(self.project)
        self.assertEqual(config.public(settings)['mobile']['controls']['pads'][0]['sensitivity'], 10)
        output = self.root / 'coarse_mouse.cpp'
        config.write_native(settings, output)
        with output.open('a') as file:
            file.write('\nint main() { const auto &p = game::controls.pads.at(0); return p.mode!="mouse" || p.sensitivity!=10; }\n')
        subprocess.run(['c++', '-std=c++17', '-I', str(ROOT/'sdk/compat/include'),
                        '-I', str(ROOT/'runtime'), str(output), '-o', str(self.root/'coarse_mouse')], check=True)
        subprocess.run([self.root/'coarse_mouse'], check=True)
        for sensitivity in (False, 0, 10.1):
            with self.subTest(sensitivity=sensitivity):
                self.write({'mobile': {'controls': {'pads': [
                    {'mode': 'mouse', 'style': 'stick', 'sensitivity': sensitivity}]}}})
                with self.assertRaisesRegex(ValueError, 'sensitivity'):
                    config.load(self.project)

    def test_direct_cursor_contract_reaches_native_configuration(self):
        self.write({'mobile':{'controls':{'pointer':'direct','cursor':{'x':25,'maxX':800,'maxY':600,'desktopScale':0.5,'desktopFractional':True}}}})
        s=config.load(self.project)
        self.assertEqual(s['mobile']['controls']['cursor'],{'x':25,'y':-1,'maxX':800,'maxY':600,'minX':0,'minY':0,'bounds':'clamp','desktopScale':0.5,'desktopFractional':True})
        self.assertEqual(config.public(s)['mobile']['controls']['cursor']['desktopScale'], 0.5)
        # Compile the generated profile, proving it remains an executable
        # configuration rather than checking its emitted source spelling.
        output=self.root/'profile.cpp'
        config.write_native(s,output)
        with output.open('a') as f:
            f.write('\nint main() { auto &c=game::controls; return c.pointer!="direct" || c.cursor.x!=25 || c.cursor.y!=-1 || c.cursor.maxX!=800 || c.cursor.maxY!=600 || c.cursor.pixelsPerUnit!=1 || c.cursor.fractional; }\n')
        subprocess.run(['c++','-std=c++17','-I',str(ROOT/'sdk/compat/include'),'-I',str(ROOT/'runtime'),str(output),'-o',str(self.root/'profile')],check=True)
        subprocess.run([self.root/'profile'],check=True)

    def test_steering_wheel_and_pedal_profile_reaches_native_host(self):
        self.write({'mobile':{'controls':{'joystick':False,
            'pads':[{'style':'wheel','mode':'wasd','maxAngle':120,'deadZone':6,
                     'keys':{'left':'A','right':'D'}}],
            'actions':[{'side':'right','label':'Accelerate','key':'W','icon':'accelerator'},
                       {'side':'right','label':'Brake / reverse','key':'S','icon':'brake'}]}}})
        settings=config.load(self.project)
        self.assertEqual(settings['mobile']['controls']['pads'][0]['axes'],'horizontal')
        output=self.root/'driving.cpp';config.write_native(settings,output)
        with output.open('a') as f:
            f.write('int main() { auto &c=game::controls; return c.pads.size()!=1 || !c.pads[0].wheel || c.pads[0].maxAngle!=120 || c.pads[0].deadZone!=6 || c.pads[0].keys[0]!="A" || c.pads[0].keys[2]!="D" || c.pads[0].axes!="horizontal" || c.actions[0].icon!="accelerator" || c.actions[1].icon!="brake"; }')
        subprocess.run(['c++','-std=c++17','-I',str(ROOT/'sdk/compat/include'),'-I',str(ROOT/'runtime'),str(output),'-o',str(self.root/'driving')],check=True)
        subprocess.run([self.root/'driving'],check=True)
        for pad in ({'maxAngle':True},{'maxAngle':44},{'maxAngle':271},{'deadZone':False},
                    {'deadZone':-1},{'deadZone':31},{'axes':'vertical'},{'mode':'mouse'},
                    {'keys':{'left':'A'}}):
            self.write({'mobile':{'controls':{'pads':[{'style':'wheel',**pad}]}}})
            with self.subTest(pad=pad),self.assertRaises(ValueError):config.load(self.project)
        self.write({'mobile':{'controls':{'actions':[{'side':'right','label':'Gas','key':'W','icon':'unknown'}]}}})
        with self.assertRaisesRegex(ValueError,'icon'):config.load(self.project)

    def test_multiplayer_touch_profile_reaches_native_host(self):
        self.write({'mobile':{'controls':{'touch':'hold-drag','pointer':'direct','repeatKeys':['W','KP8'],'repeatReleaseKeys':['LEFT','SPACE'],
          'toolbar':'top-right','keyboardPosition':'left','tapDuration':60,'touchClick':False,
          'cursor':{'x':1,'y':1,'minX':1,'minY':1,'maxX':799,'maxY':599,'bounds':'reject'},
          'pads':[{'side':'left','mode':'wasd','style':'stick','axes':'horizontal'},
                  {'side':'right','mode':'keypad','keys':{'left':'KP4','up':'KP8','right':'KP6','down':'KP2'}}],
          'actions':[{'position':'top-left','label':'P1 Fire','key':'SPACE'},
                     {'side':'right','label':'P2 Fire','key':'KP0'},
                     {'side':'right','label':'Aim','mouseButton':'right'},
                     {'side':'left','label':'Slow time','wheel':2}]}}})
        settings=config.load(self.project)
        self.assertFalse(settings['mobile']['controls']['joystick'])
        output=self.root/'multiplayer.cpp';config.write_native(settings,output)
        with output.open('a') as f:
            f.write('int main() { auto &c=game::controls; return c.touchClick || c.touch!="hold-drag" || c.toolbar!="top-right" || c.keyboardPosition!="left" || c.tapDuration!=60 || c.repeatReleaseKeys.size()!=2 || c.repeatReleaseKeys[1]!="SPACE" || c.repeatKeys.size()!=2 || c.repeatKeys[1]!="KP8" || !c.cursor.rejectBounds || c.cursor.minX!=1 || c.pads.size()!=2 || c.pads[0].mode!="wasd" || c.pads[0].axes!="horizontal" || !c.pads[1].directional || c.pads[1].keys[1]!="KP8" || c.actions.size()!=4 || c.actions[3].wheel!=2 || c.actions[0].position!="top-left" || c.actions[1].key!="KP0" || c.actions[2].mouseButton!=2; }')
        subprocess.run(['c++','-std=c++17','-I',str(ROOT/'sdk/compat/include'),'-I',str(ROOT/'runtime'),str(output),'-o',str(self.root/'multiplayer')],check=True)
        subprocess.run([self.root/'multiplayer'],check=True)
        invalid=[{'touchClick':0}, {'touch':'sometimes'}, {'tapDuration':True}, {'tapDuration':-1}, {'tapDuration':251}, {'toolbar':'middle'}, {'keyboardPosition':'center'}, {'pads':[{'axes':'diagonal'}]},
                 {'actions':[{'position':'nowhere','label':'Fire','key':'SPACE'}]},
                 {'repeatReleaseKeys':['W','W']}, {'repeatReleaseKeys':['BOGUS']}, {'repeatKeys':['W'],'repeatReleaseKeys':['W']}, {'repeatKeys':['W','W']}, {'repeatKeys':['BOGUS']}, {'pads':[{'side':'left'},{'side':'left'}]},
                 {'pads':[{'mode':'mouse'}]}, {'pads':[{'keys':{'up':'W'}}]},
                 {'actions':[{'side':'left','label':'Fire','key':'SPACE','mouseButton':'left'}]},
                 *({'actions':[{'side':'right','label':'Scroll','wheel':v}]} for v in [False,0,128,-128]),
                 {'actions':[{'side':'right','label':'Scroll','wheel':2,'key':'SPACE'}]},
                 {'actions':[{'side':'left','label':'Fire'}]},
                 {'actions':[{'side':'left','label':'Fire','key':'SPACE'}]*5},
                 {'cursor':{'minX':2,'x':1}}, {'cursor':{'minY':-1}}]
        for controls in invalid:
            self.write({'mobile':{'controls':controls}})
            with self.subTest(controls=controls), self.assertRaises(ValueError):config.load(self.project)

    def test_native_export_preserves_case_distinct_sources_and_assets(self):
        import project as source_project
        # A source checkout can be on a case-sensitive volume while the Android
        # staging directory uses the host's default, case-insensitive volume.
        with tempfile.TemporaryDirectory(prefix='lcom case source ',
                dir=os.environ.get('LCOM_TEST_CASE_SENSITIVE_ROOT')) as directory:
            project=Path(directory)/'game';project.mkdir()
            (project/'Glyph.h').write_text('static int upper=1;\n')
            (project/'glyph.h').write_text('static int lower=2;\n')
            if (project/'Glyph.h').read_text() == (project/'glyph.h').read_text():
                self.skipTest('Case-sensitive source volume required; set LCOM_TEST_CASE_SENSITIVE_ROOT')
            (project/'main.c').write_text('#include "Glyph.h"\n#include "glyph.h"\nint main(void){return upper-lower+1;}\n')
            (project/'assets').mkdir()
            # Identical bytes still require two distinct names.
            (project/'assets/Art.bin').write_bytes(b'original art')
            (project/'assets/art.bin').write_bytes(b'original art')
            for kind in ('sources','asset-files','asset-directories'):
                with self.subTest(kind=kind):
                    if kind!='sources':
                        (project/'main.c').write_text('int main(void){return 0;}\n')
                    if kind=='asset-directories':
                        (project/'assets/Art.bin').unlink();(project/'assets/art.bin').unlink()
                        (project/'assets/Empty').mkdir();(project/'assets/empty').mkdir()
                    output=self.root/('case-'+kind);output.mkdir()
                    probe=output/'CaseProbe';probe.write_bytes(b'')
                    case_sensitive=not (output/'caseprobe').exists();probe.unlink()
                    args=source_project.argument_parser(__doc__).parse_args([str(project)])
                    config.resolve(args)
                    if not case_sensitive:
                        with self.assertRaisesRegex(source_project.compat.BuildError,
                                'cannot preserve exact.*case-sensitive filesystem'):
                            source_project.write_native_project(args,output)
                    else:
                        source_project.write_native_project(args,output)
                        if kind=='sources':
                            copied={p.name:p.read_bytes() for p in (output/'native/sources').rglob('*.h')}
                            self.assertEqual(copied,{'Glyph.h':b'static int upper=1;\n','glyph.h':b'static int lower=2;\n'})
                        copied={p.relative_to(output/'game-assets').as_posix():p.read_bytes()
                                for p in (output/'game-assets').rglob('*') if p.is_file()}
                        if kind=='asset-directories':
                            self.assertEqual(copied,{})
                            names={p.name for p in (output/'game-assets/assets').iterdir()}
                            self.assertEqual(names,{'Empty','empty'})
                        else:
                            self.assertEqual(copied,{'assets/Art.bin':b'original art','assets/art.bin':b'original art'})

    def test_game_c_standard_validation_and_relocated_native_objects(self):
        import project as source_project
        self.assertEqual(config.load(self.project)['build'].get('cStandard','gnu11'),'gnu11')
        for standard in ('gnu89','gnu99','gnu11','gnu17'):
            self.write({'build':{'cStandard':standard}})
            self.assertEqual(config.load(self.project)['build']['cStandard'],standard)
        for value in ('c++17','gnu23','-O0',None,False,[],{}):
            self.write({'build':{'cStandard':value}})
            with self.subTest(value=value), self.assertRaisesRegex(ValueError,'cStandard'):
                config.load(self.project)
        for kind in ('minix','flat'):
            project=self.root/kind;project.mkdir()
            # Keep a different default profile: the explicit alternate config must win.
            (project/'lcom.json').write_text(json.dumps({'build':{'cStandard':'gnu11'}}))
            alternate=self.root/(kind+'-original-toolchain.json')
            alternate.write_text(json.dumps({'build':{'cStandard':'gnu89'}}))
            (project/'main.c').write_text('#include <stdio.h>\ninline int value(void){return 42;}\nint(*volatile fn)(void)=value;int main(void){printf("%d\\n",fn());return 0;}\n')
            if kind=='minix': (project/'Makefile').write_text('PROG=proj\nSRCS=main.c\n')
            args=source_project.argument_parser(__doc__).parse_args([str(project),'--config',str(alternate)])
            config.resolve(args)
            export=self.root/(kind+'-export');export.mkdir()
            source_project.write_native_project(args,export)
            shutil.copytree(ROOT/'sdk',export/'sdk')
            moved=self.root/(kind+'-relocated');export.rename(moved)
            shutil.rmtree(project);alternate.unlink()
            (moved/'entry.c').write_text('_Static_assert(__STDC_VERSION__==201112L,"app standard stays C11");int lcom_student_main(void);int main(void){return lcom_student_main();}\n')
            # Android's app sets C11 globally. Game objects must retain GNU89.
            (moved/'CMakeLists.txt').write_text('cmake_minimum_required(VERSION 3.22)\nproject(dialect C)\nset(CMAKE_C_STANDARD 11)\nset(LCOM_SDK "${CMAKE_CURRENT_LIST_DIR}/sdk")\ninclude(native/student.cmake)\nadd_executable(check entry.c)\ntarget_link_libraries(check PRIVATE student)\n')
            build=self.root/(kind+'-build')
            subprocess.run(['cmake','-S',str(moved),'-B',str(build),'-G','Ninja'],check=True,capture_output=True,text=True)
            result=subprocess.run(['cmake','--build',str(build)],capture_output=True,text=True)
            self.assertEqual(result.returncode,0,result.stdout+result.stderr)
            result=subprocess.run([str(build/'check')],check=True,capture_output=True,text=True)
            self.assertEqual(result.stdout.strip(),'42')

    def test_runtime_environment_and_keypad_enter(self):
        self.write({'build':{'environment':{'LCOM_COMPAT_MOUSE_REPORTING':'1','EMPTY':''}},
                    'mobile':{'controls':{'quickKeys':['KP_ENTER']}}})
        settings=config.load(self.project)
        self.assertEqual(config.public(settings)['environment'],{'LCOM_COMPAT_MOUSE_REPORTING':'1','EMPTY':''})
        output=self.root/'environment.cpp'
        config.write_native(settings,output)
        with output.open('a') as f:
            f.write('int main() { return game::environment.size()!=2 || std::string(game::environment[0].first)!="LCOM_COMPAT_MOUSE_REPORTING" || std::string(game::environment[0].second)!="1" || game::controls.quickKeys[0]!="KP_ENTER"; }')
        subprocess.run(['c++','-std=c++17','-I',str(ROOT/'sdk/compat/include'),'-I',str(ROOT/'runtime'),str(output),'-o',str(self.root/'environment')],check=True)
        subprocess.run([self.root/'environment'],check=True)
        for value in ({'BAD=NAME':'x'},{'VALID':False},{'VALID':'x\0y'},[]):
            self.write({'build':{'environment':value}})
            with self.subTest(value=value), self.assertRaisesRegex(ValueError,'environment'):
                config.load(self.project)

    def test_legacy_absolute_path_aliases_are_validated(self):
        self.write({'build':{'pathAliases':{'/home/lcom/labs/team/':'.','/home/lcom/labs/team/assets':'resources'}}})
        settings=config.load(self.project)
        self.assertEqual(settings['build']['pathAliases'],{'/home/lcom/labs/team':'.','/home/lcom/labs/team/assets':'resources'})
        # Cross the real generated-profile/SDK seam: the same aliases exported
        # to browser ENV must also let a native game write its original path.
        output=self.root/'aliases.cpp'
        config.write_native(settings,output)
        with output.open('a') as f:
            f.write('\n#include <cstdio>\n#include <cstdlib>\n#include <lcom/paths.h>\n'
                    'int main(int argc, char **argv) { if(argc<2)return 1; setenv("LCOM_COMPAT_PROJECT_ROOT",argv[1],1); '
                    'if(argc==2)for(auto &item:game::environment)setenv(item.first,item.second,1); '
                    'FILE *file=fopen("/home/lcom/labs/team/save.txt","w"); '
                    'if(!file)return 2; fputs("99",file); return fclose(file); }')
        subprocess.run(['c++','-std=c++17','-I',str(ROOT/'sdk/compat/include'),'-I',str(ROOT/'runtime'),str(output),str(ROOT/'build/liblcom_minix.a'),
                        str(ROOT/'build/libmachinelab.a'),'-pthread','-o',str(self.root/'aliases')],check=True)
        subprocess.run([self.root/'aliases',self.project],check=True)
        self.assertEqual((self.project/'save.txt').read_text(),'99')
        (self.project/'save.txt').unlink()
        subprocess.run([self.root/'aliases',self.project,'browser'],
                       env={**os.environ,**config.public(settings)['environment']},check=True)
        self.assertEqual((self.project/'save.txt').read_text(),'99')
        for aliases in ([], {'':'.'}, {'/':'.'}, {'//':'.'}, {'/old\tname':'.'}, {'/old':'../escape'}, {'/old':'/absolute'}, {'/old':False}, {'/old\\name':'.'}, {'/old\nname':'.'}):
            self.write({'build':{'pathAliases':aliases}})
            with self.subTest(aliases=aliases), self.assertRaisesRegex(ValueError,'pathAliases'):
                config.load(self.project)

    def test_explicit_relative_path_aliases_keep_their_relative_origin(self):
        self.write({'build':{'pathAliases':{'home/lcom/team/':'resources','legacy':'.'}}})
        settings=config.load(self.project)
        self.assertEqual(settings['build']['pathAliases'],{'home/lcom/team':'resources','legacy':'.'})
        environment=config.public(settings)['environment']
        self.assertEqual(environment['LCOM_COMPAT_PATH_ALIAS_0_FROM'],'home/lcom/team')
        self.assertEqual(environment['LCOM_COMPAT_PATH_ALIAS_0_TO'],'resources')
        for origin in ('', '.', './', '..', '../legacy', 'home/../legacy'):
            self.write({'build':{'pathAliases':{origin:'.'}}})
            with self.subTest(origin=origin), self.assertRaisesRegex(ValueError,'pathAliases'):
                config.load(self.project)

    def test_existing_examples_have_valid_profiles(self):
        for source in (ROOT/'examples').glob('*.c'):
            profile=source.with_suffix('')/'lcom.json'
            self.assertTrue(profile.is_file(),str(profile))
            settings=config.load(profile.parent)
            self.assertEqual(settings['build']['sources'],['../'+source.name])
        for folder in ('flappy','ninjix','pvz','sushi-slash'):
            self.assertTrue(config.load(ROOT/'examples'/folder)['mobile']['fullscreen'])

    def test_bundle_identity_and_icon_survive_relocation(self):
        png(self.project/'icon.png')
        self.write({'name':'Bundle & friends','icon':'icon.png'})
        out=self.root/'bundle'
        cli=ROOT/'build/machinelab'
        subprocess.run([cli,'bundle',self.project,'--program',ROOT/'build/examples/flappy_bird','--format','dir','--output',out,'--headless'],check=True,stdout=subprocess.DEVNULL)
        metadata=json.loads((out/'bundle.json').read_text())
        self.assertEqual(metadata['name'],'Bundle & friends')
        self.assertEqual((out/metadata['icon']).read_bytes(),(self.project/'icon.png').read_bytes())
        import shutil
        shutil.rmtree(self.project)
        moved=self.root/'moved bundle'; out.rename(moved)
        self.assertTrue((moved/'run.sh').is_file())
        self.assertEqual(json.loads((moved/'bundle.json').read_text())['icon'],'icon.png')

if __name__=='__main__': unittest.main()
