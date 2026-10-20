# VESA BIOS extensions

VBE is a compatibility interface for discovering and setting graphics modes.
lcom-ng exposes a VBE-shaped API so you can practice the same flow without BIOS
calls from a VM.

A typical setup asks for mode information, chooses a supported mode, requests and
maps a linear framebuffer, draws, and presents. The order matters. Drawing before
mode selection or mapping is not meaningful.

## Mode information

Mode information gives horizontal resolution, vertical resolution, bits per
pixel, pitch, and framebuffer address. Treat it as the source of truth for drawing
helpers. A rectangle function should not assume a hardcoded width because the
first test uses a familiar mode.

The linear framebuffer flag exposes the frame as a flat memory region rather than
an older banked model, keeping the focus on address calculation and clipping.

> [!IMPORTANT]
> Do not hardcode mode dimensions in drawing helpers. Use the mode information
> returned by the runtime.

## Indexed colors

Mode `0x105` uses 8-bit color indices. The runtime starts with the standard VGA
BIOS 256-color palette: 16 EGA colors at indices 0 through 15, 16 grays at 16
through 31, nine 24-color hue rings at 32 through 247, and black at 248 through
255.

`lcom_vbe_dac_format` selects 6-bit or 8-bit DAC components, and each mode set
resets the width to 6 bits. The DAC index and data ports at `0x3C8` and `0x3C9`
are available through `lcom_port_write8`, so a game can program its own palette.
For full RGB without palette work, choose a direct-color mode.
