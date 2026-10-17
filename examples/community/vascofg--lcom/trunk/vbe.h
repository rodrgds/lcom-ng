#ifndef __VBE_H
#define __VBE_H
/** @defgroup vbe vbe
 * @{
 *
 * Functions related to the VBE standard
 */

/** @name VBE Mode Info Block */
/**@{
 *
 * Unpacked VBE Mode Info Block for ease of access
 */ 

typedef struct {
  /*  Mandatory information for all VBE revisions */
  u16_t ModeAttributes; 	/**< @brief mode attributes */
  u8_t WinAAttributes; 		/**< @brief window A attributes */
  u8_t WinBAttributes; 		/**< @brief window B attributes */
  u16_t WinGranularity; 	/**< @brief window granularity */
  u16_t WinSize;		/**< @brief window size */
  u16_t WinASegment;		/**< @brief window A start segment */
  u16_t WinBSegment;		/**< @brief window B start segment */
  phys_bytes WinFuncPtr;	/**< @brief real mode/far pointer to window function */
  u16_t BytesPerScanLine; 	/**< @brief bytes per scan line */

  /* Mandatory information for VBE 1.2 and above */

  u16_t XResolution;      	/**< @brief horizontal resolution in pixels/characters */
  u16_t YResolution;      	/**< @brief vertical resolution in pixels/characters */
  u8_t XCharSize; 		/**< @brief character cell width in pixels */
  u8_t YCharSize; 		/**< @brief character cell height in pixels */
  u8_t NumberOfPlanes; 		/**< @brief number of memory planes */
  u8_t BitsPerPixel; 		/**< @brief bits per pixel */
  u8_t NumberOfBanks;		/**< @brief number of banks */
  u8_t MemoryModel;		/**< @brief memory model type */
  u8_t BankSize;		/**< @brief bank size in KB */
  u8_t NumberOfImagePages;	/**< @brief number of images */
  u8_t Reserved1;		/**< @brief reserved for page function */

  /* Direct Color fields (required for direct/6 and YUV/7 memory models) */
  
  u8_t RedMaskSize;		/* size of direct color red mask in bits */
  u8_t RedFieldPosition;	/* bit position of lsb of red mask */
  u8_t GreenMaskSize;		/* size of direct color green mask in bits */
  u8_t GreenFieldPosition;	/* bit position of lsb of green mask */
  u8_t BlueMaskSize; 		/* size of direct color blue mask in bits */
  u8_t BlueFieldPosition;	/* bit position of lsb of blue mask */
  u8_t RsvdMaskSize;		/* size of direct color reserved mask in bits */
  u8_t RsvdFieldPosition;	/* bit position of lsb of reserved mask */
  u8_t DirectColorModeInfo;	/* direct color mode attributes */

  /* Mandatory information for VBE 2.0 and above */
  phys_bytes PhysBasePtr;       /**< @brief physical address for flat memory frame buffer */
  u8_t Reserved2[4]; 		/**< @brief Reserved - always set to 0 */
  u8_t Reserved3[2]; 		/**< @brief Reserved - always set to 0 */

  /* Mandatory information for VBE 3.0 and above */
  u16_t LinBytesPerScanLine;    /* bytes per scan line for linear modes */
  u8_t BnkNumberOfImagePages; 	/* number of images for banked modes */
  u8_t LinNumberOfImagePages; 	/* number of images for linear modes */
  u8_t LinRedMaskSize; 	        /* size of direct color red mask (linear modes) */
  u8_t LinRedFieldPosition; 	/* bit position of lsb of red mask (linear modes) */
  u8_t LinGreenMaskSize; 	/* size of direct color green mask (linear modes) */
  u8_t LinGreenFieldPosition; /* bit position of lsb of green mask (linear  modes) */
  u8_t LinBlueMaskSize; 	/* size of direct color blue mask (linear modes) */
  u8_t LinBlueFieldPosition; 	/* bit position of lsb of blue mask (linear modes ) */
  u8_t LinRsvdMaskSize; 	/* size of direct color reserved mask (linear modes) */
  u8_t LinRsvdFieldPosition;	 /* bit position of lsb of reserved mask (linear modes) */
  u32_t MaxPixelClock; 	         /* maximum pixel clock (in Hz) for graphics mode */
  u8_t Reserved4[189]; 		 /* remainder of ModeInfoBlock */
} vbe_mode_info_t; 

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
#endif
