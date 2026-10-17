#ifndef __VBE_H
#define __VBE_H

/** @file vbe.h
 * @brief file containing the function declaritions to handle the vbe standard
 */

/** @defgroup vbe vbe
 * @{
 *
 * @brief functions related to the VBE standard
 */

#include <stdint.h>


#define BIT(n) 			(0x01<<(n))

/* VBE operating modes */
#define VBE_MODE_256_640x480		0x101		/**< @brief VBE 256 colors mode 640x480 screen resolution */
#define VBE_MODE_256_800x600		0x103		/**< @brief VBE 256 colors mode 800x600 screen resolution */
#define VBE_MODE_256_1024x768		0x105		/**< @brief VBE 256 colors mode 1024x768 screen resolution */
#define VBE_MODE_256_1280x1024		0x107		/**< @brief VBE 256 colors mode 1280x1024 screen resolution */

/* VBE functions */
#define VBE_BIOS_SERV				0x10		/**< @brief software interrupt instruction BIOS video services */
#define VBE_INV_AH_REG				0x4F		/**< @brief AH register to invoke VBE function */
#define VBE_SET_VBE_MODE			0x02		/**< @brief set VBE mode function */
#define VBE_GET_MODE_INFO			0x01		/**< @brief get VBE mode information function */
#define VBE_GET_CTRL_INFO			0x00 		/**< @brief get VBE controller information function */
#define VBE_BX_LINEAR				BIT(14)		/**< @brief VBE set bit 14 of BX register to use linear frame buffer model */

/* VBE replies */
#define VBE_REP_AL_REG_SUP			0x4F		/**< @brief VBE reply in AH register function supported */
#define VBE_REP_AH_REG_OK			0x00		/**< @brief VBE reply in AH register OK */
#define VBE_REP_AH_REG_FAIL			0x01		/**< @brief VBE reply in AH register failed */
#define VBE_REP_AH_REG_HDW			0x02		/**< @brief VBE reply in AH register not supported in current hardware */
#define VBE_REP_AH_REG_IVM			0x03		/**< @brief VBE reply in AH register invalid in current video mode */

/** @name VBE Mode Info Block */
/**@{
 *
 * Packed VBE Mode Info Block 
 */ 
typedef struct {
  /*  Mandatory information for all VBE revisions */
  uint16_t ModeAttributes; 	/**< @brief mode attributes */
  uint8_t WinAAttributes; 		/**< @brief window A attributes */
  uint8_t WinBAttributes; 		/**< @brief window B attributes */
  uint16_t WinGranularity; 	/**< @brief window granularity */
  uint16_t WinSize;		/**< @brief window size */
  uint16_t WinASegment;		/**< @brief window A start segment */
  uint16_t WinBSegment;		/**< @brief window B start segment */
  phys_bytes WinFuncPtr;	/**< @brief real mode/far pointer to window function */
  uint16_t BytesPerScanLine; 	/**< @brief bytes per scan line */

  /* Mandatory information for VBE 1.2 and above */

  uint16_t XResolution;      	/**< @brief horizontal resolution in pixels/characters */
  uint16_t YResolution;      	/**< @brief vertical resolution in pixels/characters */
  uint8_t XCharSize; 		/**< @brief character cell width in pixels */
  uint8_t YCharSize; 		/**< @brief character cell height in pixels */
  uint8_t NumberOfPlanes; 		/**< @brief number of memory planes */
  uint8_t BitsPerPixel; 		/**< @brief bits per pixel */
  uint8_t NumberOfBanks;		/**< @brief number of banks */
  uint8_t MemoryModel;		/**< @brief memory model type */
  uint8_t BankSize;		/**< @brief bank size in KB */
  uint8_t NumberOfImagePages;	/**< @brief number of images */
  uint8_t Reserved1;		/**< @brief reserved for page function */

  /* Direct Color fields (required for direct/6 and YUV/7 memory models) */
  
  uint8_t RedMaskSize;		/* size of direct color red mask in bits */
  uint8_t RedFieldPosition;	/* bit position of lsb of red mask */
  uint8_t GreenMaskSize;		/* size of direct color green mask in bits */
  uint8_t GreenFieldPosition;	/* bit position of lsb of green mask */
  uint8_t BlueMaskSize; 		/* size of direct color blue mask in bits */
  uint8_t BlueFieldPosition;	/* bit position of lsb of blue mask */
  uint8_t RsvdMaskSize;		/* size of direct color reserved mask in bits */
  uint8_t RsvdFieldPosition;	/* bit position of lsb of reserved mask */
  uint8_t DirectColorModeInfo;	/* direct color mode attributes */

  /* Mandatory information for VBE 2.0 and above */
  phys_bytes PhysBasePtr;       /**< @brief physical address for flat memory frame buffer */
  uint8_t Reserved2[4]; 		/**< @brief Reserved - always set to 0 */
  uint8_t Reserved3[2]; 		/**< @brief Reserved - always set to 0 */

  /* Mandatory information for VBE 3.0 and above */
  uint16_t LinBytesPerScanLine;    /* bytes per scan line for linear modes */
  uint8_t BnkNumberOfImagePages; 	/* number of images for banked modes */
  uint8_t LinNumberOfImagePages; 	/* number of images for linear modes */
  uint8_t LinRedMaskSize; 	        /* size of direct color red mask (linear modes) */
  uint8_t LinRedFieldPosition; 	/* bit position of lsb of red mask (linear modes) */
  uint8_t LinGreenMaskSize; 	/* size of direct color green mask (linear modes) */
  uint8_t LinGreenFieldPosition; /* bit position of lsb of green mask (linear  modes) */
  uint8_t LinBlueMaskSize; 	/* size of direct color blue mask (linear modes) */
  uint8_t LinBlueFieldPosition; 	/* bit position of lsb of blue mask (linear modes ) */
  uint8_t LinRsvdMaskSize; 	/* size of direct color reserved mask (linear modes) */
  uint8_t LinRsvdFieldPosition;	 /* bit position of lsb of reserved mask (linear modes) */
  uint32_t MaxPixelClock; 	         /* maximum pixel clock (in Hz) for graphics mode */
  uint8_t Reserved4[190]; 		 /* remainder of ModeInfoBlock */
} __attribute__((packed)) vbe_mode_info_t;
/** @} end of vbe_mode_info_t*/

/** @name VBE Info Block */
/**@{
 *
 * Packed VBE Info Block
 */
typedef struct {
	uint8_t	VbeSignature[4]; 		/**< @brief VBE Signature - 'VESA' */
	uint16_t VbeVersion;			/**< @brief VBE Version - '0x0200' */
	phys_bytes OemStringPtr;		/**< @brief Pointer to OEM String */
	uint8_t Capabilities[4];		/**< @brief Capabilities of graphics controller */
	phys_bytes VideoModePtr;		/**< @brief Pointer to VideoModeList */
	uint16_t TotalMemory;			/**< @brief Number of 64kb memory blocks */

	uint16_t OemSoftwareRev;		/**< @brief VBE implementation Software revision */
	phys_bytes OemVendorNamePtr;	/**< @brief Pointer to Vendor Name String */
	phys_bytes OemProductNamePtr;	/**< @brief Pointer to Product Name String */
	phys_bytes OemProductRevPtr;	/**< @brief Pointer to Product Revision String */
	uint8_t Reserved[222];			/**< @brief Reserved for VBE implementation scratch area */

	uint8_t OemData[256];			/**< @brief Data Area for OEM Strings */
} __attribute__((packed)) vbe_info_block_t;
/** @} end of vbe_info_block_t*/


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
 *
 * @return 0 on success, non-zero otherwise
 */
int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p);

/**
 * @brief Sets the required VBE mode
 *
 * @param mode mode whose information should be returned
 *
 * @return 0 on success, non-zero otherwise
 */
int vbe_set_mode(unsigned short mode);

/**
 * @brief Checks if the a VBE function was successfully applied
 *
 * @param reg86 struct with the mode information we want to check
 *
 * @return 0 on success, non-zero otherwise
 */
int vbe_check_reply(struct reg86u reg86);

/**
 * @brief Returns information on VBE controller capabilities
 *
 * Initializes unpacked vbe_mode__info_t structure passed as an address with
 *  the information of the input mode, by calling VBE function 0x01
 *  Return VBE Controller Information and unpacking the ModeInfoBlock struct
 *  returned by that function.
 *
 * @param vib_p address of vbe_info_block_t structure to be initialized
 * @param vbe_modes address of address of video modes array to be filed
 * @param nbr_vbe_modes address of the number of video modes to be initialized
 *
 * @return 0 on success, non-zero otherwise
 */
int vbe_get_ctrl_info(vbe_info_block_t *vib_p, uint16_t **vbe_modes, unsigned *nbr_vbe_modes);

/**
 * @brief Prints the information on VBE controller capabilities in a user friendly way
 *
 * @param vib_p address of vbe_info_block_t structure to be printed
 * @param vbe_modes address of address of video modes array to be printed
 * @param nbr_vbe_modes address of the number of video modes to be printed
 */
void print_controller_info(vbe_info_block_t *vib_p, uint16_t *vbe_modes, unsigned nbr_vbe_modes);

/** @} end of vbe */

#endif /* __VBE_H */
