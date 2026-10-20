# Lab 6: AC97-lite PCM audio

Generated folder: `labs/audio/`

Lab 6 introduces audio as a stream of samples. You fill an audio buffer with PCM
data and ask the runtime to play it.

The analogy with graphics helps. A framebuffer becomes an image; an audio buffer
becomes pressure changes in the air. In both cases the device consumes bytes in a
format, and the program must produce those bytes consistently.

| Concept | Meaning |
| --- | --- |
| PCM sample | numeric amplitude at one point in time |
| sample rate | samples per second |
| channel count | mono or stereo layout |
| buffer | mapped memory for samples |
| byte count | amount the device should consume |

Learn [PCM audio](/labs/lab6/pcm), complete the [tasks](/labs/lab6/tasks), run
the [checks](/labs/lab6/check), and compare with
[modern audio stacks](/labs/lab6/modern).

> [!TIP]
> Audio bugs are often unit bugs: samples, frames, channels, and bytes are not
> interchangeable.
