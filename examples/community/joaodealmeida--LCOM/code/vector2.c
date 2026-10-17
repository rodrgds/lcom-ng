#include <math.h>
#include "vector2.h"

/** @defgroup vector2 vector2
 * @{*/

const float pi = 3.141592653589793f;///<pi value
const float oneDegreeInRads = 0.017453292519943f;///<pi/180
const vector2  v2_zero = {0,0};
const vector2  v2_unitX = {1,0};
const vector2  v2_unitY = {0,1};
const vector2  v2_One = {1,1};
const float epsilon = 0.001f;///<consider numbers equal to zero in some funcs when smaller than this

/*vector2 Vector2(float xy)
{
	vector2 vec2={xy,xy};
	return vec2;
}*/
vector2 Vector2(float x,float y)
{
	vector2 vec2={x,y};
	return vec2;
}
vector2 Vector2Copy(vector2 v) {
	vector2 vec2={v.x,v.y};
	return vec2;
}

char v2_equal(vector2 vec1,vector2 vec2)
{
	return ( abs(vec1.x-vec2.x)<epsilon && abs(vec1.y-vec2.y)<epsilon );
}
vector2 v2add (vector2 a,vector2 b)
{
	return Vector2(a.x+b.x,a.y+b.y);
}
vector2 v2sub (vector2 a,vector2 b)
{
	return Vector2(a.x-b.x,a.y-b.y);
}
void v2mulRef(vector2 *aRef,float f)
{
	aRef->x*=f;aRef->y*=f;
}
vector2 v2mul(vector2 a,float f)
{
	return Vector2(a.x*f,a.y*f);
}
float v2_lenght_squared(vector2 v)
{
	return (v.x*v.x+v.y*v.y);
}
float v2_lenght(vector2 v)
{
	return sqrt( v.x*v.x+v.y*v.y );
}
char normalizeRef(vector2 *v)
{
	//check if is near zero, if it's the normalization will fail die and produce some random result
	if (abs(v->x)>epsilon && abs(v->y)>epsilon){
	float comp=v2_lenght(*v);
	v->x= v->x/comp;
	v->y= v->y/comp;
	return 0;
	}
	return 1;
}
//Anti Clock Wise, angle in degrees
vector2 v2_rotateACW(vector2 v, float rotAng)
{
	float lenght = v2_lenght(v);
	float angle = atan2(v.y,v.x);
	rotAng *= oneDegreeInRads;//converts to radians
	angle+=rotAng;
	
	vector2 vr = {
		cos(angle)*lenght,
		sin(angle)*lenght};

		return vr;
}
void v2_rotateACWref(vector2 *v, float rotAng)
{
	float lenght = v2_lenght(*v);
	float angle = atan2(v->y,v->x);
	rotAng *= oneDegreeInRads;//converts to radians
	angle+=rotAng;
	v->y = sin(angle)*lenght;
	v->x = cos(angle)*lenght;
}
/** @} end of vector2 */
