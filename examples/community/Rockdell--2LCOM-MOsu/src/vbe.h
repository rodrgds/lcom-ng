#ifndef __VBE_H
#define __VBE_H

#include <stdint.h>

/** @defgroup vbe VBE
 * @{
 *
 * Functions related to the VBE standard
 */

/** @name VBE Mode Info Block */
/** @{
 *
 *  Packed VBE Mode Info Block
 */ 

typedef struct {
  /*  Mandatory information for all VBE revisions */
  uint16_t ModeAttributes; 			/**< Mode attributes */
  uint8_t WinAAttributes; 			/**< Window A attributes */
  uint8_t WinBAttributes; 			/**< Window B attributes */
  uint16_t WinGranularity; 			/**< Window granularity */
  uint16_t WinSize;					/**< Window size */
  uint16_t WinASegment;				/**< Window A start segment */
  uint16_t WinBSegment;				/**< Window B start segment */
  phys_bytes WinFuncPtr;			/**< Real mode/far pointer to window function */
  uint16_t BytesPerScanLine; 		/**< Bytes per scan line */

  /* Mandatory information for VBE 1.2 and above */

  uint16_t XResolution;      		/**< Horizontal resolution in pixels/characters */
  uint16_t YResolution;      		/**< Vertical resolution in pixels/characters */
  uint8_t XCharSize; 				/**< Character cell width in pixels */
  uint8_t YCharSize; 				/**< Character cell height in pixels */
  uint8_t NumberOfPlanes; 			/**< Number of memory planes */
  uint8_t BitsPerPixel; 			/**< Bits per pixel */
  uint8_t NumberOfBanks;			/**< Number of banks */
  uint8_t MemoryModel;				/**< Memory model type */
  uint8_t BankSize;					/**< Bank size in KB */
  uint8_t NumberOfImagePages;		/**< Number of images */
  uint8_t Reserved1;				/**< Reserved for page function */

  /* Direct Color fields (required for direct/6 and YUV/7 memory models) */
  
  uint8_t RedMaskSize;				/**< Size of direct color red mask in bits */
  uint8_t RedFieldPosition;			/**< Bit position of lsb of red mask */
  uint8_t GreenMaskSize;			/**< Size of direct color green mask in bits */
  uint8_t GreenFieldPosition;		/**< Bit position of lsb of green mask */
  uint8_t BlueMaskSize; 			/**< Size of direct color blue mask in bits */
  uint8_t BlueFieldPosition;		/**< Bit position of lsb of blue mask */
  uint8_t RsvdMaskSize;				/**< Size of direct color reserved mask in bits */
  uint8_t RsvdFieldPosition;		/**< Bit position of lsb of reserved mask */
  uint8_t DirectColorModeInfo;		/**< Direct color mode attributes */

  /* Mandatory information for VBE 2.0 and above */
  phys_bytes PhysBasePtr;      		/**< Physical address for flat memory frame buffer */
  uint8_t Reserved2[4]; 			/**< Reserved - always set to 0 */
  uint8_t Reserved3[2]; 			/**< Reserved - always set to 0 */

  /* Mandatory information for VBE 3.0 and above */
  uint16_t LinBytesPerScanLine;		/**< Bytes per scan line for linear modes */
  uint8_t BnkNumberOfImagePages; 	/**< Number of images for banked modes */
  uint8_t LinNumberOfImagePages; 	/**< Number of images for linear modes */
  uint8_t LinRedMaskSize; 	        /**< Size of direct color red mask (linear modes) */
  uint8_t LinRedFieldPosition; 		/**< Bit position of lsb of red mask (linear modes) */
  uint8_t LinGreenMaskSize; 		/**< Size of direct color green mask (linear modes) */
  uint8_t LinGreenFieldPosition; 	/**< Bit position of lsb of green mask (linear  modes) */
  uint8_t LinBlueMaskSize; 			/**< Size of direct color blue mask (linear modes) */
  uint8_t LinBlueFieldPosition; 	/**< Bit position of lsb of blue mask (linear modes ) */
  uint8_t LinRsvdMaskSize; 			/**< Size of direct color reserved mask (linear modes) */
  uint8_t LinRsvdFieldPosition;	 	/**< Bit position of lsb of reserved mask (linear modes) */
  uint32_t MaxPixelClock; 	        /**< Maximum pixel clock (in Hz) for graphics mode */
  uint8_t Reserved4[190]; 		 	/**< Remainder of ModeInfoBlock */
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
int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p);

 /** @} end of vbe */

#endif /* __VBE_H */
