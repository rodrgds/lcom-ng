#include <lcom/lcf.h>

#include "images/truck.xpm"
#include "images/car.xpm"

/**
 * @brief Vehicle struct that has the image of both the truck and car and has a bool that lets it know what it is (car or truck) as well as the position
 * 
 */
typedef struct {
  xpm_image_t img;
  xpm_image_t img2;
  int x, y, speed;
  bool car;

} Vehicle;

/**
 * @brief Initializes the vehicles
 * 
 * @param vehicle Struct vehicle 
 * @param count count to know which vehicle it is
 */
void initVehicle(Vehicle * vehicle, int count);
