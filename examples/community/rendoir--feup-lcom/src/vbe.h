/**
  @file vbe.h
  @author Daniel Marques
  @author João Almeida
  @author Pedro Ferreira Souto

  Imported from lab5
*/

#ifndef __VBE_H
#define __VBE_H

#include <stdint.h>

#define GET_MODE_INFO  0x4F01
#define SET_DISPLAY_START  0x4F07
#define SET_DURING_V_RETRACE 0x0080


/** @defgroup vbe vbe
 * @{
 *
 * Functions related to the VBE standard
 */

/** @name VBE Mode Info Block */
/**@{
 *
 * Packed VBE Mode Info Block
 */

/**
@struct vbe_mode_info_t
  @brief Struct to hold information about the VBE
  @var ModeAttributes mode attributes
  @var WinAAttributes window A attributes
  @var WinBAttributes window B attributes
  @var WinGranularity window granularity
  @var WinSize window size
  @var WinASegment window A start segment
  @var WinBSegment window B start segment
  @var WinFuncPtr real mode/far pointer to window function
  @var BytesPerScanLine bytes per scan line
  @var XResolution  horizontal resolution in pixels/characters
  @var YResolution  vertical resolution in pixels/characters
  @var XCharSize  character cell width in pixels
  @var YCharSize  character cell height in pixels
  @var NumberOfPlanes  number of memory planes
  @var BitsPerPixel  bits per pixel
  @var NumberOfBanks  number of banks
  @var MemoryModel  memory model type
  @var BankSize  bank size in KB
  @var NumberOfImagePage  number of images
  @var Reserved1  reserved for page function
  @var RedMaskSize		size of direct color red mask in bits
  @var RedFieldPosition	bit position of lsb of red mask
  @var GreenMaskSize		size of direct color green mask in bits
  @var GreenFieldPosition	bit position of lsb of green mask
  @var BlueMaskSize 		size of direct color blue mask in bits
  @var BlueFieldPosition	bit position of lsb of blue mask
  @var RsvdMaskSize		size of direct color reserved mask in bits
  @var RsvdFieldPosition	bit position of lsb of reserved mask
  @var DirectColorModeInfo	direct color mode attributes
  @var PhysBasePtr     physical address for flat memory frame buffer
  @var Reserved2[4] Reserved - always set to 0
  @var Reserved3[2] Reserved - always set to 0
  @var  LinBytesPerScanLine   bytes per scan line for linear modes
  @var BnkNumberOfImagePages number of images for banked modes
  @var LinNumberOfImagePages number of images for linear modes
  @var LinRedMaskSize 	       size of direct color red mask (linear modes)
  @var LinRedFieldPosition bit position of lsb of red mask (linear modes)
  @var LinGreenMaskSize size of direct color green mask (linear modes)
  @var LinGreenFieldPositionbit position of lsb of green mask (linear  modes)
  @var LinBlueMaskSize size of direct color blue mask (linear modes)
  @var LinBlueFieldPosition bit position of lsb of blue mask (linear modes )
  @var LinRsvdMaskSize 	size of direct color reserved mask (linear modes)
  @var LinRsvdFieldPosition	bit position of lsb of reserved mask (linear modes)
  @var MaxPixelClock 	        maximum pixel clock (in Hz) for graphics mode
  @var Reserved4[190] 		remainder of ModeInfoBlock

  @details Imported from Lab5
*/
typedef struct VbeMode{
  /*  Mandatory information for all VBE revisions */
  uint16_t ModeAttributes;
  uint8_t WinAAttributes;
  uint8_t WinBAttributes;
  uint16_t WinGranularity;
  uint16_t WinSize;
  uint16_t WinASegment;
  uint16_t WinBSegment;
  phys_bytes WinFuncPtr;
  uint16_t BytesPerScanLine;

  /* Mandatory information for VBE 1.2 and above */

  uint16_t XResolution;
  uint16_t YResolution;
  uint8_t XCharSize;
  uint8_t YCharSize;
  uint8_t NumberOfPlanes;
  uint8_t BitsPerPixel;
  uint8_t NumberOfBanks;
  uint8_t MemoryModel;
  uint8_t BankSize;
  uint8_t NumberOfImagePages;
  uint8_t Reserved1;

  /* Direct Color fields (required for direct/6 and YUV/7 memory models) */

  uint8_t RedMaskSize;
  uint8_t RedFieldPosition;
  uint8_t GreenMaskSize;
  uint8_t GreenFieldPosition;
  uint8_t BlueMaskSize;
  uint8_t BlueFieldPosition;
  uint8_t RsvdMaskSize;
  uint8_t RsvdFieldPosition;
  uint8_t DirectColorModeInfo;

  /* Mandatory information for VBE 2.0 and above */
  phys_bytes PhysBasePtr;
  uint8_t Reserved2[4];
  uint8_t Reserved3[2];

  /* Mandatory information for VBE 3.0 and above */
  uint16_t LinBytesPerScanLine;
  uint8_t BnkNumberOfImagePages;
  uint8_t LinNumberOfImagePages;
  uint8_t LinRedMaskSize;
  uint8_t LinRedFieldPosition;
  uint8_t LinGreenMaskSize;
  uint8_t LinGreenFieldPosition;
  uint8_t LinBlueMaskSize;
  uint8_t LinBlueFieldPosition;
  uint8_t LinRsvdMaskSize;
  uint8_t LinRsvdFieldPosition;
  uint32_t MaxPixelClock;
  uint8_t Reserved4[190];
} __attribute__((packed)) vbe_mode_info_t;
/** @} end of vbe_mode_info_t*/


/**
 * @brief Returns information on the input VBE mode, including screen dimensions, color depth and VRAM physical address
 *
 * Initializes unpacked vbe_mode__info_t structure passed as an address with
 *  the information of the input mode, by calling VBE function 0x01
 *  Return VBE Mode Information and unpacking the ModeInfoBlock struct
 *  returned by that function.
 *
 * @param mode mode whose information should be returned
 * @param vmi_p address of vbe_mode_info_t structure to be initialized
 * @return 0 on success, non-zero otherwise
 */
int vbe_get_mode_info(uint16_t mode, vbe_mode_info_t *vmi_p);
 /** @} end of vbe */

#endif /* __VBE_H */
