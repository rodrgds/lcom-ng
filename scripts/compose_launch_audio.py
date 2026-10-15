#!/usr/bin/env python3
"""Original ambient score for the 40-second launch. No percussion or sharp attacks.

Pure Python, deterministic, stereo PCM. Slow overlapping sine voicings keep the
sound behind the footage. No sampled or third-party audio is used.
"""
import array
import math
from pathlib import Path
import sys
import wave

RATE = 48000
DURATION = 40
TAU = math.tau
# Fmaj7, Dm9, Bbmaj7, C6. Open voicings stay out of the speech/presence range.
CHORDS = ((41, 53, 60, 64, 69), (38, 53, 57, 60, 64),
          (34, 53, 57, 62, 65), (36, 52, 57, 60, 67))

def smooth(value):
    value = max(0.0, min(1.0, value))
    return value * value * (3 - 2 * value)

def compose(path):
    samples = array.array('h')
    for frame in range(RATE * DURATION):
        time = frame / RATE
        left = right = 0.0
        for index, chord in enumerate(CHORDS):
            start = index * 9.5 - 1
            age = time - start
            if age < 0 or age > 12:
                continue
            envelope = smooth(age / 2.5) * smooth((12 - age) / 3.5)
            for voice, note in enumerate(chord):
                hz = 440 * 2 ** ((note - 69) / 12)
                phase = voice * 0.71
                # Gentle detuning adds width; the high harmonics are very quiet.
                gain = 0.017 * envelope / (1 + voice * 0.2)
                left += gain * (math.sin(TAU * hz * time + phase) + 0.08 * math.sin(TAU * hz * 2 * time))
                right += gain * (math.sin(TAU * hz * 1.0006 * time + phase) + 0.08 * math.sin(TAU * hz * 2.0003 * time))
        master = smooth(time / 1.5) * smooth((DURATION - time) / 3)
        samples.extend((round(left * master * 32767), round(right * master * 32767)))
    if sys.byteorder != 'little':
        samples.byteswap()
    path.parent.mkdir(parents=True, exist_ok=True)
    with wave.open(str(path), 'wb') as output:
        output.setnchannels(2)
        output.setsampwidth(2)
        output.setframerate(RATE)
        output.writeframes(samples.tobytes())

if __name__ == '__main__':
    compose(Path(sys.argv[1] if len(sys.argv) > 1 else 'build/launch-clips/ambient.wav'))
