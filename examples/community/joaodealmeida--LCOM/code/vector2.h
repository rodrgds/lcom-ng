#ifndef __VECTOR2_H
#define __VECTOR2_H

/** @defgroup vector2 vector2
 * @{
 * 	2 Dimensional vector struct and functions
 * 	Notes:
 * 	-the funcs v2_equal and normalizeRef use epsilon. be careful.
 * 	-every vector2 func starts as "v2"
 * */

/**
 * 2 dimensional vector
 */
typedef struct vector2d{
	float x;///<x component
	float y;///<y component
}vector2;

extern const vector2  v2_zero;
extern const vector2  v2_unitX;
extern const vector2  v2_unitY;
extern const vector2  v2_One;

//"construtors"
//vector2 Vector2(float xy);
vector2 Vector2(float x,float y);///<creates a vector (x,y)
vector2 Vector2Copy(vector2 v);///<copies a vector
//operations
/**@brief check if to vector are equal.
 * @return 0 if different, 1 if equal
 */
char v2_equal(vector2 vec1,vector2 vec2);
vector2 v2add (vector2 a,vector2 b);///<(a+b) add 2 vectors
vector2 v2sub (vector2 a,vector2 b);///<(a-b) subtract 2 vectors
void v2mulRef(vector2 *aRef,float f);///<a=(a*f) multiply given vector by f
vector2 v2mul(vector2 a,float f);///<(a*f) multiply a vector by f and return result
float v2_lenght_squared(vector2 v);///<find vector length squared
float v2_lenght(vector2 v);///<find vector length
char normalizeRef(vector2 *v);///<snaps vector length to size one
/**@brief find rotated vector anti-clockwise
 * @param vector original vector
 * @param angle to rotate in degrees
 * @return rotated vector
 */
vector2 v2_rotateACW(vector2 v, float rotAng);
/**@brief rotates given vector anti-clockwise
 * @param vector to rotate
 * @param angle to rotate in degrees
 * @return rotated vector
 */
void v2_rotateACWref(vector2 *v, float rotAng);

/** @} end of vector2 */
#endif //__VECTOR2_H
