#!/usr/bin/env python3
"""Real-device collection launch, return, process isolation and saved-file smoke test."""
import argparse
import io
import json
import os
from pathlib import Path
import re
import shutil
import shlex
import subprocess
import time
import xml.etree.ElementTree as ET
from PIL import Image, ImageDraw, ImageFont

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('export', type=Path)
parser.add_argument('--serial', default=os.environ.get('ANDROID_SERIAL', 'emulator-5556'))
parser.add_argument('--output', type=Path, default=Path('build/android-collection-qa'))
parser.add_argument('--all', action='store_true', help='launch every catalog entry and review a contact sheet')
parser.add_argument('--game', action='append', help='game ID to test; default: first two games')
args = parser.parse_args()
args.output.mkdir(parents=True, exist_ok=True)
metadata = json.loads((args.export / 'lcom-collection-android.json').read_text())
catalog = json.loads((args.export / 'game-assets/collection.json').read_text())
package = metadata['applicationId']
sdk_home = os.environ.get('ANDROID_HOME') or os.environ.get('ANDROID_SDK_ROOT')
adb_path = shutil.which('adb') or (str(Path(sdk_home) / 'platform-tools/adb') if sdk_home else 'adb')

def adb(*command, check=True):
    return subprocess.run([adb_path, '-s', args.serial, *command], stdout=subprocess.PIPE,
                          stderr=subprocess.STDOUT, check=check).stdout

def wait(probe, message, timeout=15):
    deadline = time.monotonic() + timeout
    while time.monotonic() < deadline:
        result = probe()
        if isinstance(result, ET.Element) or result:
            return result
        time.sleep(.25)
    raise AssertionError(message)

def pid(process):
    return adb('shell', 'pidof', process, check=False).decode().strip()

last_library = None

def ui():
    global last_library
    adb('shell', 'uiautomator', 'dump', '/sdcard/lcom-collection-ui.xml')
    tree = ET.fromstring(adb('exec-out', 'cat', '/sdcard/lcom-collection-ui.xml'))
    if any(n.get('content-desc', '').startswith('Play ') for n in tree.iter('node')):
        last_library = tree
    return tree

def find(description):
    return next((node for node in ui().iter('node') if node.get('content-desc') == description), None)

def click_node(node):
    x1, y1, x2, y2 = map(int, re.findall(r'\d+', node.get('bounds')))
    adb('shell', 'input', 'tap', str((x1+x2)//2), str((y1+y2)//2))

def click(description):
    click_node(wait(lambda: find(description), f'Missing control: {description}'))

def screenshot(name):
    data = adb('exec-out', 'screencap', '-p')
    (args.output / (name + '.png')).write_bytes(data)
    return Image.open(io.BytesIO(data)).convert('RGB')

def library_visible():
    return any(node.get('content-desc', '').startswith('Play ') for node in ui().iter('node'))

def click_game(game):
    # Use the persistent native search rather than making the size of artwork
    # determine whether a game near the end of a large catalog is reachable.
    word = max(re.findall(r'[A-Za-z]+', game['name']), key=len)
    search_library(word)
    description = 'Play ' + game['name']
    order = {'Play ' + entry['name']: index for index, entry in enumerate(catalog['games'])}
    for attempt in range(24):
        tree = last_library if attempt == 0 and last_library is not None else ui()
        node = next((node for node in tree.iter('node') if node.get('content-desc') == description), None)
        if node is not None:
            x1, y1, x2, y2 = map(int, re.findall(r'\d+', node.get('bounds')))
            if y2 - y1 >= 48:
                adb('shell', 'input', 'tap', str((x1+x2)//2), str((y1+y2)//2))
                return
        bounds = next(tree.iter('node')).get('bounds')
        _, _, width, height = map(int, re.findall(r'\d+', bounds))
        visible = [order[n.get('content-desc')] for n in tree.iter('node') if n.get('content-desc') in order]
        upward = bool(visible) and order[description] < (min(visible) + max(visible)) / 2
        # Keep successive viewports overlapping, including Android's swipe inertia.
        start, end = (height*2//5, height*7//10) if upward else (height*7//10, height*2//5)
        adb('shell', 'input', 'swipe', str(width//2), str(start), str(width//2), str(end), '650')
    raise AssertionError('Game tile is not reachable: ' + game['name'])

adb('install', '-r', str(args.export / metadata['apk']))
adb('shell', 'am', 'force-stop', package)
adb('logcat', '-c')
adb('shell', 'input', 'keyevent', 'KEYCODE_WAKEUP')
adb('shell', 'wm', 'dismiss-keyguard')
adb('shell', 'am', 'start', '-W', '-n', package + '/pt.rgo.lcomng.CollectionActivity')
wait(library_visible, 'Native game library failed to appear')
launcher_pid = pid(package)
assert launcher_pid, 'No launcher process'

# Exercise the native search field through real input, including recovery from
# zero matches. Pick a real catalog word instead of hard-coding a game title.
search_query = ''
def search_library(value):
    global search_query
    click('Find a game')
    adb('shell', 'input', 'keyevent', 'KEYCODE_MOVE_END')
    if search_query:
        adb('shell', 'input', 'keyevent', *(['KEYCODE_DEL'] * len(search_query)))
    if value:
        adb('shell', 'input', 'text', value)
    search_query = value
    adb('shell', 'input', 'keyevent', 'KEYCODE_ENTER')

query = max(re.findall(r'[A-Za-z]+', catalog['games'][0]['name']), key=len).upper()
expected = {'Play ' + game['name'] for game in catalog['games'] if query.lower() in game['name'].lower()}
search_library(query)
filtered = {node.get('content-desc') for node in ui().iter('node') if node.get('content-desc', '').startswith('Play ')}
assert filtered and filtered <= expected, ('Native search includes an unrelated game', filtered, expected)
screenshot('collection-search')
search_library('ZZZ_NO_ARCADE_MATCH_14')
tree = ui()
assert not any(node.get('content-desc', '').startswith('Play ') for node in tree.iter('node'))
assert any(node.get('text') == 'No games match. Try another name.' for node in tree.iter('node'))
# The clear action remains reachable even when the result grid is empty.
click('Clear search')
search_query = ''
wait(library_visible, 'Clearing native search did not restore the library')
assert find('Choose a random game') is not None, 'Random-game action is inaccessible'
# The search header stays available while browsing deep in a larger library.
if len(catalog['games']) > 12:
    viewport = screenshot('collection-search-sticky-before').size
    adb('shell', 'input', 'swipe', str(viewport[0]//2), str(viewport[1]*4//5), str(viewport[0]//2), str(viewport[1]//3), '600')
    assert find('Find a game') is not None, 'Search scrolls out of reach with game cards'
    search_library(query)
    assert any(node.get('content-desc', '').startswith('Play ') for node in ui().iter('node')), 'Sticky search did not filter the scrolled library'
    click('Clear search')
    search_query = ''
    wait(library_visible, 'Clearing sticky search did not restore the library')
# am start can retain keyboard navigation from a previous Back-key probe.
# Enter touch mode on empty title padding before checking authored colors.
adb('shell', 'input', 'tap', '1', '1')
menu = screenshot('collection-menu')
assert menu.getpixel((0, 0)) == (8, 10, 9), 'Launcher does not fill the display edge'
assert menu.getpixel((menu.width-1, menu.height-1)) == (8, 10, 9), 'Launcher leaves space for system bars'
selected = args.game or [entry['id'] for entry in (catalog['games'] if args.all else catalog['games'][:2])]
by_id = {game['id']: game for game in catalog['games']}
# Independently inspect actual native hit rectangles, not the declared column count.
visible = [node for node in ui().iter('node') if node.get('content-desc', '').startswith('Play ')]
if len(catalog['games']) >= 3:
    rectangles = [list(map(int,re.findall(r'\d+',node.get('bounds')))) for node in visible]
    first_row = [rect for rect in rectangles if rect[1] == rectangles[0][1]]
    assert 2 <= len(first_row) <= 6, 'Native library lost its adaptive columns'
    assert all(left[2] <= right[0] for left,right in zip(first_row,first_row[1:])), 'Native game tiles overlap'
previous_pids = set()
shots = []
for index, identifier in enumerate([*selected, selected[0]]):
    game = by_id[identifier]
    if index == len(selected):
        # Return to the top before the repeated first-game save/reset probe.
        # Scrolling upward is faster and preserves the launcher process.
        for _ in range(12):
            if find('Play ' + game['name']) is not None: break
            image_size = screenshot('scroll-position').size
            adb('shell', 'input', 'swipe', str(image_size[0]//2), str(image_size[1]//4), str(image_size[0]//2), str(image_size[1]*4//5), '160')
    click_game(game)
    game_pid = wait(lambda: pid(package + ':game'), 'No isolated game process')
    assert game_pid != launcher_pid and game_pid not in previous_pids, 'Game reused a native process'
    previous_pids.add(game_pid)
    return_control = wait(lambda: find('Return to game library'), 'Native game return control failed to appear')
    time.sleep(1)
    image = screenshot(identifier + '-' + str(index))
    assert len(image.getcolors(image.width * image.height)) > 2, 'Game did not render pixels'
    shots.append((game['name'], image.copy()))
    assert pid(package) == launcher_pid, 'Launching a game replaced the launcher process'
    marker = 'files/games/' + identifier + '/collection-save-proof.txt'
    if index == 0:
        adb('shell', 'run-as ' + package + ' sh -c ' + shlex.quote('printf saved > ' + marker))
    if index == len(selected):
        assert adb('shell', 'run-as', package, 'cat', marker).strip() == b'saved', 'Relaunch discarded saved files'
        # SIGKILL models a student abort/exit without a cooperative SDL shutdown.
        adb('shell', 'run-as', package, 'kill', '-9', game_pid)
    elif index % 2 == 0:
        click_node(return_control)
    else:
        adb('shell', 'input', 'keyevent', 'KEYCODE_BACK')
    wait(lambda: not pid(package + ':game'), 'Game process survived return')
    wait(library_visible, 'Returning from a game lost the library')
    assert pid(package) == launcher_pid, 'Game termination killed or restarted the launcher'
    print(f'{identifier}: fresh process {game_pid}, native render, return and launcher isolation passed', flush=True)
log = adb('logcat', '-d')
(args.output / 'collection-logcat.txt').write_bytes(log)
assert not re.search(rb'Fatal signal|FATAL EXCEPTION|Cannot prepare game assets|dlopen failed', log), 'Native launch or assets failed, inspect collection-logcat.txt'
if args.all:
    columns, width, height = 5, 384, 236
    sheet = Image.new('RGB', (columns*width, ((len(shots)+columns-1)//columns)*height), '#080a09')
    draw = ImageDraw.Draw(sheet)
    for index, (name, picture) in enumerate(shots):
        picture.thumbnail((width-16, height-42))
        x, y = index % columns * width, index // columns * height
        sheet.paste(picture, (x + (width-picture.width)//2, y))
        draw.text((x+8, y+height-34), name[:50], fill='#f4f7f5')
    sheet.save(args.output / 'all-games-contact-sheet.png')
screenshot('collection-returned')
print('Android native collection fullscreen, launch, back, crash containment and saved-file checks passed')
