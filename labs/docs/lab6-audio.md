# Lab 6: AC97-lite PCM Audio

Generated folder: `labs/audio/`

Lab 6 adds audio as a new lcom-ng device area. Students map a PCM buffer, write
signed 16-bit stereo samples, configure sample-rate/playback registers, and use
headless `.wav` capture to test audio deterministically.

## Student Files

- `labs/audio/include/audio_lib.h`
- `labs/audio/audio_lab.c`

## Requested Functions

- `int audio_map_buffer(void)`
- `int audio_fill_square_wave(uint32_t hz, uint32_t ms)`
- `int audio_play(size_t byte_count)`
- `int audio_stop(void)`

## What Students Learn

- Memory-mapped audio buffers and sample layout.
- Signed PCM sample generation and simple wave synthesis.
- Sample rate, byte count, and playback control registers.
- Start/stop behavior that students implement themselves.
- Testing audio by rendering a deterministic `.wav` file.

## Test And Reference

```sh
lcom test audio
lcom run --headless --audio-wav build/tone.wav -- build/examples/audio_tone
```
