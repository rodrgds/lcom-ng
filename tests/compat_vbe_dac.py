"""Verify independent indexed DACs through both desktop runtime owners."""
from pathlib import Path
import subprocess
import sys
runtime, client, destination = sys.argv[1:]
out=Path(destination);out.mkdir(parents=True,exist_ok=True)
def run(*arguments):
    result=subprocess.run([runtime,*map(str,arguments)],check=True,capture_output=True,text=True)
    return result.stdout+result.stderr
def frame(name,color):
    magic,size,maximum,pixels=(out/name).read_bytes().split(b'\n',3)
    assert (magic,size,maximum)==(b'P6',b'1024 768',b'255')
    assert pixels==bytes(color)*1024*768,name
single=run('run','--headless','--max-ticks','100','--dump-frame',out/'single.ppm','--',client)
assert 'VBE_DAC_OK' in single,single
frame('single.ppm',(227,66,129))
pair=run('run-pair','--headless','--max-ticks','100','--left-dump-frame',out/'left.ppm','--right-dump-frame',out/'right.ppm',client,'left','--right',client)
assert pair.count('VBE_DAC_OK')==2,pair
frame('left.ppm',(255,85,12))
frame('right.ppm',(227,66,129))
print('Single and independent paired BIOS DAC precision, palette transfers and presented RGB passed')
