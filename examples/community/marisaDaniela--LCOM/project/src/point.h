#define TILE_SIZE 20

#define BOARD_X 242
#define BOARD_Y 114


/**
 * @brief point info struct
 */
typedef struct {
	unsigned int x;
	unsigned int y;
} Point;


/**
 * @brief Initializes a new point
 * @param x point x value
 * @param y point y value
 * @return pointer to the new point created
 */
Point* getPoint(int x, int y);

/**
 * @brief Converts a point to coordinates
 * @param point pointer to point to be converted
 * @return pointer to the new coordinate created
 */
Point* pointToCoord(Point* point);


/**
 * @brief Compares two points
 * @param a pointer to point to be compared
 * @param b pointer to point to be compared
 * @return 0 if different, 1 if equal
 */
int comparePoints(Point* a, Point* b);