# PCM audio

PCM stores sound as a sequence of numeric sample values. Each sample is the
waveform amplitude at one point in time. At 48 kHz, the device expects 48,000
samples per second per channel.

| Term | Meaning |
| --- | --- |
| sample rate | samples per second |
| channel | mono/stereo layout |
| frame | one sample per channel |
| buffer | memory region sent to the device |
| amplitude | numeric signal level |

The device consumes bytes, not musical intent. Translate frequency and duration
into sample counts, frame counts, and byte counts. A small unit mistake makes
audio too short, too long, distorted, or silent.

## Square wave

A square wave alternates between positive and negative amplitude:

```text
positive for half period
negative for half period
repeat
```

It is easy to generate and test. The sound is simple, but the expected buffer is
easy to inspect. After the square wave works, a sine wave, envelope, melody, or
tracker pattern is a higher-level transformation that still ends in PCM samples.
