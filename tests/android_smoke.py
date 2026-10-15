#!/usr/bin/env python3
"""Install and exercise the native fixture APK in an already running Android emulator."""
import io
import re
import shlex
from pathlib import Path
import subprocess
import time
from PIL import Image

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / 'build/android-qa'
PACKAGE = 'pt.rgo.lcomng.smoke'


def adb(*args, binary=False):
    return subprocess.check_output(['adb', *args], text=not binary)


def wait_for(expected):
    deadline = time.monotonic() + 30
    while time.monotonic() < deadline:
        log = adb('logcat','-d','-s','lcom-ng:I','*:S')
        if expected in log: return log
        time.sleep(0.2)
    raise AssertionError(f'Missing {expected!r} in Android logcat:\n{log}')


def screenshot(name):
    data = adb('exec-out','screencap','-p',binary=True)
    (OUT / name).write_bytes(data)
    return Image.open(io.BytesIO(data)).convert('RGB')


def count_color(frame,color):
    return sum(count for count,value in frame.getcolors(frame.width*frame.height) if value==color)


def wait_for_frame(name,expected):
    deadline=time.monotonic()+10
    while True:
        frame=screenshot(name)
        if expected(frame): return frame
        assert time.monotonic()<deadline, f'Expected game frame did not appear, last size {frame.size}'
        time.sleep(0.1)


def wait_for_input(package,x,y):
    component=package+'/pt.rgo.lcomng.GameActivity'
    deadline=time.monotonic()+10
    while True:
        state=adb('shell','dumpsys','input')
        focused=re.search(r'(?ms)^  FocusedWindows:\n(.*?)(?=^  \S|\Z)',state)
        focus=focused.group(1) if focused else ''
        focused_game=re.search(r"name='[^']* "+re.escape(component)+r"'",focus)
        # InputDispatcher regions use physical coordinates. Its display matrix
        # maps those to the logical screenshot/adb coordinates, including wm
        # size overrides and rotation. Invert that affine transform first.
        display=re.search(r'(?ms)^  Display: 0\n(.*?)(?=^  \S|\Z)',state)
        matrix=re.search(r'transform[^\n]*\n\s+([^\n]+)\n\s+([^\n]+)',
                         display.group(1) if display else '')
        point=None
        if matrix:
            try:
                a,b,tx=map(float,matrix.group(1).split())
                c,d,ty=map(float,matrix.group(2).split())
                determinant=a*d-b*c
                if determinant:
                    point=((d*(x-tx)-b*(y-ty))/determinant,
                           (a*(y-ty)-c*(x-tx))/determinant)
            except ValueError: pass
        windows=[line for line in state.splitlines()
                 if re.search(r'\bname=[^,]* '+re.escape(component)+r',',line)]
        for window in windows:
            config=re.search(r'inputConfig=([^,]+)',window)
            region=re.search(r'touchableRegion=([^\n]+?)(?:, \w+=|$)',window)
            rectangles=re.findall(r'\[(-?\d+),(-?\d+)\]\[(-?\d+),(-?\d+)\]',
                                  region.group(1) if region else '')
            contains=point is not None and any(
                int(left)<=point[0]<int(right) and int(top)<=point[1]<int(bottom)
                for left,top,right,bottom in rectangles)
            if (focused_game and 'DispatchEnabled: true' in state and
                'DispatchFrozen: false' in state and config and
                not any(flag in config.group(1) for flag in
                        ('NOT_TOUCHABLE','NOT_VISIBLE','NO_INPUT_CHANNEL')) and contains):
                (OUT/'android-direct-input-ready.txt').write_text(state)
                return
        assert time.monotonic()<deadline, (
            f'Game input not ready at logical ({x},{y}), physical {point}:\n{focus}\n'+
            '\n'.join(windows))
        time.sleep(0.1)


def direct_diagnostics():
    # Capture before force-stop, including failures before the tap is sent.
    commands={'logcat':('logcat','-d'), 'input':('shell','dumpsys','input'),
              'window':('shell','dumpsys','window','windows'),
              'activity':('shell','dumpsys','activity','activities')}
    for name,command in commands.items():
        try: output=adb(*command)
        except subprocess.CalledProcessError as error: output=str(error)
        (OUT/f'android-direct-failure-{name}.txt').write_text(output)
    try: screenshot('android-direct-failure.png')
    except subprocess.CalledProcessError: pass


def main():
    OUT.mkdir(parents=True,exist_ok=True)
    adb('install','-r',str(OUT / 'smoke/game.apk'))
    adb('logcat','-c')
    rotation={key:adb('shell','settings','get','system',key).strip()
              for key in ('accelerometer_rotation','user_rotation')}
    try:
        adb('shell','settings','put','system','accelerometer_rotation','0')
        adb('shell','settings','put','system','user_rotation','0')
        adb('shell','am','start','-W','-n',PACKAGE+'/pt.rgo.lcomng.GameActivity')
        wait_for('asset:browser asset loaded')
        wait_for('tick60')
        frame = screenshot('android-controls.png')
        # Independent fixture color proves actual rendering, not merely Activity startup.
        assert count_color(frame,(17,34,51)) > frame.width*frame.height/12
        # Find the first complete four-button row in the pixels. It contains
        # Esc, Space, Enter and Keyboard in either portrait or landscape.
        target=None
        for y in range(frame.height):
            runs=[]; begin=None
            for x in range(frame.width):
                filled=frame.getpixel((x,y))==(34,34,34)
                if filled and begin is None: begin=x
                if not filled and begin is not None:
                    if x-begin>=30: runs.append((begin,x))
                    begin=None
            if len(runs)>=4:
                space=runs[-4:][1]
                target=((space[0]+space[1])//2,y+2); break
        assert target is not None,'Native quick-key row not visible'
        adb('shell','input','tap',str(target[0]),str(target[1]))
        wait_for('key:39')  # Guest PS/2 Space make code from an Android touch.
        wait_for('key:b9')
        adb('shell','input','keyevent','KEYCODE_A')
        wait_for('key:1e')
        wait_for('key:9e')
        adb('shell','input','keyevent','KEYCODE_HOME')
        time.sleep(0.5)
        adb('shell','am','start','-W','-n',PACKAGE+'/pt.rgo.lcomng.GameActivity')
        # Resume the same game, rather than starting the student program a second time.
        assert wait_for('ready').count('asset:browser asset loaded') == 1
        # The A key made the fixture's first guest pixel green. The rest of
        # its static frame must survive a replaced Android display surface.
        wait_for_frame('android-resumed.png',lambda frame: count_color(frame,(17,34,51))>frame.width*frame.height/12)
        adb('shell','settings','put','system','accelerometer_rotation','0')
        adb('shell','settings','put','system','user_rotation','1')
        time.sleep(1)
        screenshot('android-landscape.png')
        adb('shell','input','keyevent','KEYCODE_ESCAPE')
        log = wait_for('game exited with status 0')
        assert 'Cannot prepare game assets' not in log
        (OUT / 'android-logcat.txt').write_text(log)
        print('Android native rendering, touch, hardware keyboard, resume and exit passed')
    finally:
        adb('shell','am','force-stop',PACKAGE)
        for key,value in rotation.items():
            if value=='null': adb('shell','settings','delete','system',key)
            else: adb('shell','settings','put','system',key,value)
    direct_touch()
    initialized_coordinates()


def initialized_coordinates():
    package=PACKAGE+'.coordinates'
    adb('install','-r',str(OUT/'coordinates/game.apk'))
    adb('logcat','-c')
    try:
        adb('shell','am','start','-W','-n',package+'/pt.rgo.lcomng.GameActivity')
        log=wait_for('game exited with status 0')
        assert 'fixture row:200' in log,log
        (OUT/'android-coordinates-logcat.txt').write_text(log)
        print('Android initialized data-only coordinate source survived linking')
    finally:
        adb('shell','am','force-stop',package)


def direct_touch():
    package=PACKAGE+'.direct'
    adb('install','-r',str(OUT/'direct/game.apk'))
    adb('logcat','-c')
    confirmation='immersive_mode_confirmations'
    previous=adb('shell','settings','get','secure',confirmation).strip()
    try:
        # Android's first-use fullscreen tutorial owns focus and pauses SDL.
        # Establish a confirmed test environment, restoring it after the probe.
        adb('shell','settings','put','secure',confirmation,'confirmed')
        assert adb('shell','settings','get','secure',confirmation).strip()=='confirmed'
        adb('shell','am','start','-W','-n',package+'/pt.rgo.lcomng.GameActivity')
        wait_for('ready')
        frame=wait_for_frame('android-direct-landscape.png',lambda frame:
            frame.width>frame.height and all(frame.getpixel((frame.width//2,y))==(17,34,51)
                for y in (0,frame.height//2,frame.height-1)))
        # Locate the fixture's solid game rectangle, independently of SDL's
        # layout calculations, system bars and the emulator's screen size.
        pixels=frame.load()
        points=[(x,y) for y in range(frame.height) for x in range(frame.width) if pixels[x,y]==(17,34,51)]
        assert points
        left=min(p[0] for p in points); top=min(p[1] for p in points)
        width=max(p[0] for p in points)-left+1; height=max(p[1] for p in points)-top+1
        fit=min(frame.width/800,frame.height/600)
        assert abs(width-800*fit)<=2 and abs(height-600*fit)<=2,(frame.size,left,top,width,height)
        assert abs(left-(frame.width-width)/2)<=1 and abs(top-(frame.height-height)/2)<=1
        x,y=round(left+width*0.125),round(top+height*0.25)
        # A first rendered frame can precede the focused input window's
        # launch/rotation transition. Wait for this actual point to be touchable.
        wait_for_input(package,x,y)
        adb('shell','input','tap',str(x),str(y))
        deadline=time.monotonic()+5
        positions=[]
        while time.monotonic()<deadline:
            log=adb('logcat','-d','-s','lcom-ng:I','*:S')
            data=[int(value,16) for value in re.findall(r'mouse:([0-9a-f]{2})',log)]
            if data and data[0]==0xfa: data.pop(0)
            x,y=400,300; positions=[]
            for offset in range(0,len(data)-2,3):
                flags,dx,dy=data[offset:offset+3]
                x=max(0,min(799,x+dx-(256 if flags&16 else 0)))
                y=max(0,min(599,y-dy+(256 if flags&32 else 0)))
                positions.append((x,y,flags&7))
            # Startup neutral packets and coordinate travel precede the press.
            if any(p[2]==1 for p in positions) and positions[-1][2]==0: break
            time.sleep(0.1)
        (OUT/'android-direct-logcat.txt').write_text(log)
        presses=[p for p in positions if p[2]==1]
        assert len(presses)==1,positions
        assert abs(presses[0][0]-100)<=2 and abs(presses[0][1]-150)<=2,positions
        assert positions[-1][2]==0,positions
        # A fullscreen static game has no control redraws to mask a missing
        # foreground event. Resume it without sending any further game input.
        for cycle in range(3):
            adb('shell','input','keyevent','KEYCODE_HOME')
            time.sleep(0.5)
            adb('shell','am','start','-W','-n',package+'/pt.rgo.lcomng.GameActivity')
            wait_for_frame(f'android-direct-resumed-{cycle}.png',lambda frame:
                count_color(frame,(17,34,51))>frame.width*frame.height/3)
        print('Android landscape lock, direct tap ordering and fullscreen static resume passed')
    except Exception:
        direct_diagnostics()
        raise
    finally:
        (OUT/'android-direct-logcat.txt').write_text(adb('logcat','-d','-s','lcom-ng:I','*:S'))
        adb('shell','am','force-stop',package)
        # adb joins remote shell arguments, so an empty value needs quotes.
        if previous=='null': adb('shell','settings','delete','secure',confirmation)
        else: adb('shell','settings','put','secure',confirmation,shlex.quote(previous))


if __name__ == '__main__':
    main()
