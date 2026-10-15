"""Exercise editor persistence and primary shortcuts through real guest PS/2 input."""
import pathlib
import re
import subprocess
import sys
import tempfile

cli, examples = map(pathlib.Path, sys.argv[1:])
with tempfile.TemporaryDirectory(prefix='lcom-studio-') as directory:
    root = pathlib.Path(directory)
    script = root / 'save.mlabscript'
    script.write_text('''at 1 key H down
at 2 key H up
at 3 key I down
at 4 key I up
at 5 key LGUI down
at 6 key S down
at 7 key S up
at 8 key LGUI up
at 12 key ESC down
at 13 key ESC up
''')
    document = root / 'note.txt'
    subprocess.run([str(cli), 'run', '--headless', '--script', str(script),
                    '--max-ticks', '120', '--', str(examples / 'word_processor'),
                    str(document)], check=True, capture_output=True, text=True)
    assert document.exists(), 'Command-S must write the document to disk'
    assert document.read_text() == 'hi', 'Save must persist actual typed text'
    # Reopening and saving must preserve the file rather than start empty.
    script.write_text('''at 1 key LCTRL down
at 2 key S down
at 3 key S up
at 4 key LCTRL up
at 7 key ESC down
at 8 key ESC up
''')
    result = subprocess.run([str(cli), 'run', '--headless', '--script', str(script),
                             '--max-ticks', '120', '--', str(examples / 'word_processor'),
                             str(document)], check=True, capture_output=True, text=True)
    assert 'word processor text hi' in result.stdout, result.stdout
    assert document.read_text() == 'hi'

    def replay(program, actions, artifact, discard=True):
        events = []
        tick = 1
        for key, down in actions:
            events.append(f'at {tick} key {key} {"down" if down else "up"}')
            tick += 3
        events += [f'at {tick + 2} key ESC down', f'at {tick + 5} key ESC up']
        if discard:
            events += [f'at {tick + 8} key D down', f'at {tick + 11} key D up']
        script.write_text('\n'.join(events) + '\n')
        result = subprocess.run([str(cli), 'run', '--headless', '--rtc', '2026-08-12T14:35:42',
                                 '--script', str(script), '--max-ticks', '240', '--',
                                 str(examples / program), str(artifact)],
                                check=True, capture_output=True, text=True)
        return result

    def tap(key):
        return [(key, True), (key, False)]

    def chord(*keys):
        return [(key, True) for key in keys] + [(key, False) for key in reversed(keys)]

    protected = replay('music_maker', tap('ENTER') + tap('F1') + chord('LGUI', 'Z') + tap('ESC'), root / 'unused')
    assert 'active=32 ' in protected.stdout, 'Help must block sequencer undo: ' + protected.stdout
    protected = replay('music_maker', tap('ENTER') + tap('RIGHT') + tap('F1') + tap('RIGHT') + tap('ESC') + tap('ENTER'), root / 'unused')
    assert 'active=33 ' in protected.stdout, 'Help must keep the sequencer cursor in place: ' + protected.stdout

    # Restoring the saved composition must also restore its clean close state.
    replay('music_maker', tap('M') + tap('M'), root / 'clean-pattern.mlpat', discard=False)

    navigation = root / 'navigation.txt'
    navigation.write_text('hello world')
    replay('word_processor', tap('HOME') + tap('RIGHT') * 5 + chord('LCTRL', 'RIGHT') +
           chord('LSHIFT', '1') + chord('LGUI', 'S'), navigation)
    assert navigation.read_text() == 'hello !world', 'Ctrl-Right from whitespace must stop at the next word'
    navigation.write_text('first\nsecond')
    replay('word_processor', tap('HOME') + chord('LGUI', 'LEFT') + tap('X') +
           chord('LGUI', 'S'), navigation)
    assert navigation.read_text() == 'first\nxsecond', 'Command-Left at line start must stay on that line'

    history = root / 'history.txt'
    replay('word_processor', chord('LSHIFT', '2') + chord('LGUI', 'S'), history)
    assert history.read_text() == '@', 'Shift-2 must insert the shifted symbol'
    history.write_text('')
    replay('word_processor', tap('A') + tap('B') + tap('C') + chord('LGUI', 'Z') * 2 +
           chord('LGUI', 'LSHIFT', 'Z') + chord('LGUI', 'S'), history)
    assert history.read_text() == 'ab', 'History must retain more than one edit and support redo'
    history.write_text('hello')
    replay('word_processor', chord('LGUI', 'A') + chord('LGUI', 'C') + tap('END') +
           tap('ENTER') + chord('LGUI', 'V') + chord('LGUI', 'S'), history)
    assert history.read_text() == 'hello\nhello', 'Notes must paste the selected text through the host bridge'

    pattern = root / 'persisted.mlpat'
    replay('music_maker', tap('ENTER') + chord('LGUI', 'S'), pattern)
    saved_pattern = pattern.read_text()
    assert saved_pattern.startswith('LCOM-PATTERN 1 126\n'), saved_pattern
    reopened = replay('music_maker', chord('LGUI', 'S'), pattern)
    assert pattern.read_text() == saved_pattern, 'Composer must reopen the actual stored pattern'
    assert 'active=32 ' in reopened.stdout, reopened.stdout
    pattern.write_text('malformed composition\n')
    replay('music_maker', chord('LGUI', 'S'), pattern)
    assert pattern.read_text() == 'malformed composition\n', 'Invalid existing files must survive attempted save'

    tilemap = root / 'draft.mlmap'
    replay('tilemap_editor', tap('1') + tap('SPACE') + chord('LGUI', 'S'), tilemap)
    design = tilemap.read_text().split('size 10 8\n')[1]
    assert design[0] == '.', 'Keyboard painting must edit the selected map cell'
    replay('tilemap_editor', chord('LGUI', 'S'), tilemap)
    assert tilemap.read_text().split('size 10 8\n')[1] == design, 'Tile Forge must reopen draft map data'
    tilemap.write_text('malformed map\n')
    replay('tilemap_editor', chord('LGUI', 'S'), tilemap)
    assert tilemap.read_text() == 'malformed map\n', 'Invalid existing maps must survive attempted save'

    original = root / 'original.h'
    restored = root / 'restored.h'
    cleared = root / 'cleared.h'
    replay('pixel_studio', tap('S'), original)
    replay('word_processor', tap('F1') + tap('H') + tap('ESC') + tap('F2'), document)
    assert document.read_text() == 'hi', 'Typing beneath help must not edit the document'
    replay('pixel_studio', tap('F1') + tap('C') + tap('ESC') + tap('S'), restored)
    assert re.findall(r'0x[0-9A-F]{8}u', original.read_text()) == re.findall(r'0x[0-9A-F]{8}u', restored.read_text()), 'Help must block clear actions'

    replay('pixel_studio', tap('C') + chord('LGUI', 'Z') + chord('LGUI', 'C') + tap('S'), restored)
    # Ignore the timestamp comment, compare the actual reusable C pixel data.
    colors = lambda path: re.findall(r'0x[0-9A-F]{8}u', path.read_text())
    assert colors(original) == colors(restored), 'Command-Z must restore the sprite; Command-C must not clear it'
    replay('pixel_studio', tap('C') + chord('LGUI', 'Z') + chord('LGUI', 'LSHIFT', 'Z') + tap('S'), cleared)
    assert colors(cleared) == ['0x00000000u'] * 256, 'Command-Shift-Z must redo the clear operation'

    script.write_text("""at 1 key F1 down
at 2 key F1 up
at 5 mouse 0 0 1
at 6 mouse 50 0 1
at 7 mouse 0 0 0
at 10 key ESC down
at 11 key ESC up
at 14 key S down
at 15 key S up
at 18 key ESC down
at 19 key ESC up
""")
    subprocess.run([str(cli), 'run', '--headless', '--script', str(script), '--max-ticks', '120',
                    '--', str(examples / 'pixel_studio'), str(restored)], check=True, capture_output=True)
    assert colors(original) == colors(restored), 'Help must block mouse painting and dragging'

    script.write_text("""at 2 mouse 37 26 0
at 5 mouse 0 0 1
at 6 mouse 0 0 0
at 10 key SPACE down
at 11 key SPACE up
at 90 key ESC down
at 91 key ESC up
at 95 key D down
at 96 key D up
""")
    result = subprocess.run([str(cli), 'run', '--headless', '--script', str(script),
                             '--max-ticks', '120', '--', str(examples / 'music_maker'), str(root / 'mouse-pattern.mlpat')],
                            check=True, capture_output=True, text=True)
    summary = re.search(r'active=(\d+) tempo=\d+ submitted=(\d+)', result.stdout)
    assert summary and int(summary[1]) == 34 and int(summary[2]) > 0, result.stdout
    for program, actions, expected in [
        ('pomodoro_clock', tap('F1') + tap('D') + tap('SPACE') + tap('ESC'), 'running=0 active_ticks=0'),
        ('ambient_aquarium', tap('F1') + tap('F') + tap('SPACE') + tap('L') + tap('ESC'), 'feeds=0'),
        ('interrupt_scope', tap('F1') + tap('SPACE') + tap('L') + tap('ESC'), 'frozen=0 loopback=1'),
    ]:
        result = replay(program, actions, root / 'unused-device-path')
        assert expected in result.stdout, f'{program} must consume editing input while help is open: {result.stdout}'

    source = pathlib.Path(__file__).resolve().parents[1]
    left = root / 'left.mlabscript'
    left.write_text((source / 'scripts/serial_whiteboard_left.mlabscript').read_text() + '''
at 10s key LGUI down
at 10.1s key C down
at 10.2s key C up
at 10.3s key LGUI up
''')
    result = subprocess.run([str(cli), 'run-pair', '--headless', '--left-script', str(left),
                             '--right-script', str(source / 'scripts/serial_whiteboard_right.mlabscript'),
                             '--max-ticks', '1000', str(examples / 'serial_whiteboard'), 'left',
                             '--right', str(examples / 'serial_whiteboard'), 'right'],
                            check=True, capture_output=True, text=True)
    assert result.stdout.count('local_segments=120 remote_segments=120') == 2, result.stdout
    left.write_text('at 10 key LSHIFT down\n' + ''.join(
        f'at {20 + index * 3} key RIGHT down\nat {21 + index * 3} key RIGHT up\n'
        for index in range(12)) + 'at 60 key LSHIFT up\nat 200 key ESC down\nat 201 key ESC up\n')
    right = root / 'right.mlabscript'
    right.write_text('at 200 key ESC down\nat 201 key ESC up\n')
    result = subprocess.run([str(cli), 'run-pair', '--headless', '--left-script', str(left),
                             '--right-script', str(right), '--max-ticks', '300',
                             str(examples / 'serial_whiteboard'), 'left', '--right',
                             str(examples / 'serial_whiteboard'), 'right'],
                            check=True, capture_output=True, text=True)
    assert 'local_segments=12 remote_segments=0' in result.stdout, result.stdout
    assert 'local_segments=0 remote_segments=12' in result.stdout, result.stdout
print('studio save/reopen, Command undo/redo, safe modifier chords, mouse notes and Space playback passed')
