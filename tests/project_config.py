#!/usr/bin/env python3
"""Public lcom.json contract: defaults, validation, paths, overrides and bundles."""
import argparse
import importlib.util
import json
import os
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

    def test_actionable_validation(self):
        for data,field in [({'mobile':{'controls':{'keyboard':'false'}}},'keyboard'),
                           ({'mobile':{'orentation':'landscape'}},'orentation'),
                           ({'mobile':{'orientation':'sideways'}},'orientation'),
                           ({'mobile':{'theme':{'accent':'red'}}},'accent'),
                           ({'mobile':{'controls':{'quickKeys':['BOGUS']}}},'quickKeys'),
                           ({'mobile':{'controls':{'look':{'sensitivity':False}}}},'sensitivity'),
                           ({'mobile':{'controls':{'cursor':{'x':1.5}}}},'cursor.x'),
                           ({'mobile':{'controls':{'cursor':{'x':True}}}},'cursor.x'),
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

    def test_pads_do_not_overlap_the_optional_mouse_stick(self):
        self.write({'mobile':{'controls':{'dpad':True}}})
        self.assertFalse(config.load(self.project)['mobile']['controls']['joystick'])
        self.write({'mobile':{'controls':{'dpad':True,'joystick':True}}})
        self.assertEqual(config.load(self.project)['mobile']['controls']['joystick'],{'mode':'mouse','side':'right'})

    def test_direct_cursor_contract_reaches_native_configuration(self):
        self.write({'mobile':{'controls':{'pointer':'direct','cursor':{'x':25,'maxX':800,'maxY':600}}}})
        s=config.load(self.project)
        self.assertEqual(s['mobile']['controls']['cursor'],{'x':25,'y':-1,'maxX':800,'maxY':600,'minX':0,'minY':0,'bounds':'clamp'})
        # Compile the generated profile, proving it remains an executable
        # configuration rather than checking its emitted source spelling.
        output=self.root/'profile.cpp'
        config.write_native(s,output)
        with output.open('a') as f:
            f.write('\nint main() { auto &c=game::controls; return c.pointer!="direct" || c.cursor.x!=25 || c.cursor.y!=-1 || c.cursor.maxX!=800 || c.cursor.maxY!=600; }\n')
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
        self.write({'mobile':{'controls':{'touch':'hold-drag','pointer':'direct','repeatKeys':['W','KP8'],
          'toolbar':'top-right','keyboardPosition':'left','tapDuration':60,
          'cursor':{'x':1,'y':1,'minX':1,'minY':1,'maxX':799,'maxY':599,'bounds':'reject'},
          'pads':[{'side':'left','mode':'wasd','style':'stick','axes':'horizontal'},
                  {'side':'right','mode':'keypad','keys':{'left':'KP4','up':'KP8','right':'KP6','down':'KP2'}}],
          'actions':[{'position':'top-left','label':'P1 Fire','key':'SPACE'},
                     {'side':'right','label':'P2 Fire','key':'KP0'},
                     {'side':'right','label':'Aim','mouseButton':'right'}]}}})
        settings=config.load(self.project)
        self.assertFalse(settings['mobile']['controls']['joystick'])
        output=self.root/'multiplayer.cpp';config.write_native(settings,output)
        with output.open('a') as f:
            f.write('int main() { auto &c=game::controls; return c.touch!="hold-drag" || c.toolbar!="top-right" || c.keyboardPosition!="left" || c.tapDuration!=60 || c.repeatKeys.size()!=2 || c.repeatKeys[1]!="KP8" || !c.cursor.rejectBounds || c.cursor.minX!=1 || c.pads.size()!=2 || c.pads[0].mode!="wasd" || c.pads[0].axes!="horizontal" || !c.pads[1].directional || c.pads[1].keys[1]!="KP8" || c.actions.size()!=3 || c.actions[0].position!="top-left" || c.actions[1].key!="KP0" || c.actions[2].mouseButton!=2; }')
        subprocess.run(['c++','-std=c++17','-I',str(ROOT/'sdk/compat/include'),'-I',str(ROOT/'runtime'),str(output),'-o',str(self.root/'multiplayer')],check=True)
        subprocess.run([self.root/'multiplayer'],check=True)
        invalid=[{'touch':'sometimes'}, {'tapDuration':True}, {'tapDuration':-1}, {'tapDuration':251}, {'toolbar':'middle'}, {'keyboardPosition':'center'}, {'pads':[{'axes':'diagonal'}]},
                 {'actions':[{'position':'nowhere','label':'Fire','key':'SPACE'}]},
                 {'repeatKeys':['W','W']}, {'repeatKeys':['BOGUS']}, {'pads':[{'side':'left'},{'side':'left'}]},
                 {'pads':[{'mode':'mouse'}]}, {'pads':[{'keys':{'up':'W'}}]},
                 {'actions':[{'side':'left','label':'Fire','key':'SPACE','mouseButton':'left'}]},
                 {'actions':[{'side':'left','label':'Fire'}]},
                 {'actions':[{'side':'left','label':'Fire','key':'SPACE'}]*5},
                 {'cursor':{'minX':2,'x':1}}, {'cursor':{'minY':-1}}]
        for controls in invalid:
            self.write({'mobile':{'controls':controls}})
            with self.subTest(controls=controls), self.assertRaises(ValueError):config.load(self.project)

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
        for aliases in ([], {'relative':'.'}, {'/':'.'}, {'//':'.'}, {'/old\tname':'.'}, {'/old':'../escape'}, {'/old':'/absolute'}, {'/old':False}, {'/old\\name':'.'}, {'/old\nname':'.'}):
            self.write({'build':{'pathAliases':aliases}})
            with self.subTest(aliases=aliases), self.assertRaisesRegex(ValueError,'pathAliases'):
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
