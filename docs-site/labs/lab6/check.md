# Lab 6 check

Run the audio tests before judging by ear. The tests verify byte counts, buffer
bounds, and deterministic sample output even when local playback differs. They
do not prove that the sound is pleasant: listen to the generated WAV as well,
and check for clicks, clipping, and a matching sample rate.

```sh
lcom-ng test audio
lcom-ng run --headless --audio-wav build/tone.wav -- build/examples/audio_tone
```

## Discussion prompts

- What changes if you generate sine waves instead of square waves?
- How could a tracker represent notes, tempo, and instruments?
- Why do audio APIs often use ring buffers?

## External reading

Read OSDev's [sound overview](https://wiki.osdev.org/Sound), the
[PCM audio overview](https://en.wikipedia.org/wiki/Pulse-code_modulation), and
the OSDev [AC97 notes](https://wiki.osdev.org/AC97).
