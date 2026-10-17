#!/usr/bin/env python3
"""Check configured physical maps through relocated SDK and CLI run owners."""
import argparse
import importlib.util
import os
from pathlib import Path
import shlex
import shutil
import subprocess
import tempfile


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source',type=Path,default=Path(__file__).resolve().parents[1])
    parser.add_argument('--build',type=Path,required=True)
    args=parser.parse_args()
    root=args.source.resolve(); build=args.build.resolve()
    spec=importlib.util.spec_from_file_location('hardware_sdk_package',root/'scripts/package_student.py')
    package=importlib.util.module_from_spec(spec);spec.loader.exec_module(package)
    with tempfile.TemporaryDirectory(prefix='lcom hardware configuration ') as temporary:
        folder=Path(temporary)
        installed=folder/'original-sdk'
        package.copy_student_payload(root,build,installed)
        (installed/'bin').mkdir()
        shutil.copy2(build/'machinelab',installed/'bin/lcom-ng')
        moved=folder/'relocated-sdk';installed.rename(moved)
        fixture=folder/'physical-map.c'
        shutil.copy2(root/'tests/fixtures/hardware-config/main.c',fixture)
        binary=folder/'physical-map'
        compiler=shlex.split(os.environ.get('CC','cc'))
        subprocess.run([*compiler,'-std=gnu11','-I',str(moved/'sdk/include'),'-I',str(moved/'sdk/compat/include'),str(fixture),str(moved/'sdk/liblcom_minix.a'),str(moved/'sdk/libmachinelab.a'),'-lpthread','-o',str(binary)],check=True)
        fixture.unlink()
        cli=str(moved/'bin/lcom-ng')
        environment=os.environ.copy();environment.pop('LCOM_VBE_PHYSICAL_BASE',None);environment.pop('LCOM_AC97_PCM_BASE',None)
        def run(command,env=environment):
            result=subprocess.run([cli,*command],env=env,capture_output=True,text=True,timeout=8)
            assert result.returncode==0,result.stdout+result.stderr
            return result.stdout+result.stderr
        default=run(['run','--headless','--timeout','3','--',str(binary),'0xE0000000'])
        assert 'HARDWARE_CONFIG_OK e0000000' in default,default
        changed={**environment,'LCOM_VBE_PHYSICAL_BASE':'0xF0000000'}
        selected=run(['run','--headless','--timeout','3','--',str(binary),'0xF0000000'],changed)
        assert 'HARDWARE_CONFIG_OK f0000000' in selected,selected
        paired=run(['run-pair','--headless','--timeout','3',str(binary),'0xF0000000','--right',str(binary),'0xF0000000'],changed)
        assert paired.count('HARDWARE_CONFIG_OK f0000000')==2,paired
        replay=folder/'empty.mlabscript';replay.write_text('# No input needed for physical maps.\n')
        replayed=run(['replay','--headless','--timeout','3',str(replay),str(binary),'0xF0000000'],changed)
        assert 'HARDWARE_CONFIG_OK f0000000' in replayed,replayed
        dual={**environment,'LCOM_VBE_PHYSICAL_BASE':'0xD0000000','LCOM_AC97_PCM_BASE':'0xC0000000'}
        relocated=run(['run','--headless','--timeout','3','--',str(binary),'0xD0000000','0xC0000000'],dual)
        assert 'HARDWARE_CONFIG_OK d0000000 PCM c0000000' in relocated,relocated
        both=run(['run-pair','--headless','--timeout','3',str(binary),'0xD0000000','0xC0000000','--right',str(binary),'0xD0000000','0xC0000000'],dual)
        assert both.count('HARDWARE_CONFIG_OK d0000000 PCM c0000000')==2,both
        for bad in ('0x100000000','0xf0000001','0xd0000000'):
            invalid=subprocess.run([cli,'run','--headless','--',str(binary)],env={**environment,'LCOM_VBE_PHYSICAL_BASE':bad},capture_output=True,text=True,timeout=3)
            assert invalid.returncode==1 and 'LCOM_VBE_PHYSICAL_BASE' in invalid.stderr,(bad,invalid.stdout,invalid.stderr)
            assert 'Starting lcom-ng' not in invalid.stdout
    print('Relocated SDK physical maps, BIOS address, default, run, replay and both paired slots passed')


if __name__=='__main__':
    main()
