# How it works today: audio

Modern audio systems use DMA, ring buffers, mixers, and operating-system audio
servers. Applications rarely write to a hardware buffer and command the device
like a tiny lab runtime.

## What changed

Applications use APIs such as WASAPI, Core Audio, ALSA, PulseAudio, PipeWire,
SDL audio, or browser Web Audio. The OS mixes applications, handles device
changes, negotiates formats, and balances latency, power, and stability.

Professional audio may need very low latency and stable buffer scheduling.
Everyday applications care more about compatibility and avoiding glitches. Both
rest on the same fact: audio is time-sensitive sample data.

## What stayed useful

Sample-rate and duration conversion, byte counts versus sample counts, buffer
ownership, clipping and amplitude, and deterministic audio artifacts for tests.
The music maker example applies the same idea: application state becomes PCM
samples.
