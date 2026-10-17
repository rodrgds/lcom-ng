#include<stdint.h>
#include"frames.h"

/**
 * @brief Creates a new frame from the given XPM image.
 *
 * This function loads an XPM image using the LCF library and creates
 * a Frame structure containing the image data and its dimensions.
 *
 * @param xpm The XPM image to load (as an array of strings).
 * @return Pointer to the created Frame structure. Returns NULL on failure.
 */
Frame *createFrame(const char  *xpm[]){
  Frame *frame=(Frame *)malloc(sizeof(frame));
  xpm_image_t img;
  frame->map=(char *)xpm_load(xpm,XPM_8_8_8_8,&img);
  frame->width=img.width;
  frame->height=img.height;
  return frame;
}

/**
 * @brief Frees the memory associated with a Frame.
 *
 * This function deallocates the memory used by a Frame.
 * If the frame is NULL, the function does nothing.
 *
 * @param frame Pointer to the Frame to be destroyed.
 */
void destoryFrame(Frame *frame){
  if(frame!=NULL){
      free(frame);
  }
}

/**
 * @brief Draws a frame on the screen at the given coordinates.
 *
 * This function draws the contents of a Frame at the specified position
 * using the vg_draw_xpm function.
 *
 * @param xi The X coordinate of the top-left corner.
 * @param yi The Y coordinate of the top-left corner.
 * @param frame Pointer to the Frame to draw.
 */
void drawframe(uint16_t xi, uint16_t yi,Frame *frame){
  vg_draw_xpm(xi,yi,(uint8_t *)frame->map,frame->width,frame->height);
}


