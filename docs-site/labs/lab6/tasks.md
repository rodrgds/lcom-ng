# Lab 6 tasks

Work in `labs/audio/`. Produce a valid audio buffer and hand it to the runtime
with the correct byte count. Treat the buffer as device-facing memory.

## Requested functions

- `int audio_map_buffer(void)`
- `int audio_fill_square_wave(uint32_t hz, uint32_t ms)`
- `int audio_play(size_t byte_count)`
- `int audio_stop(void)`

## Guided gaps

Reject zero frequency or duration, compute samples per half-period, choose an
amplitude that does not clip, keep byte count and sample count separate, and make
`audio_stop` safe when nothing is playing. Keep the square-wave function small
enough to verify the period arithmetic directly.

> [!IMPORTANT]
> Do not write past the mapped audio buffer. Duration, sample rate, channels,
> and bytes per sample must agree before writing.

## Common mistakes

Using frequency where period is required, treating milliseconds as samples,
writing one channel when stereo is expected, and starting playback before filling
the buffer. If the output duration is wrong, inspect unit conversion before
changing the playback call.

Next: [checks and references](/labs/lab6/check).
