#include "collision.h"

char circlesColide(vector2 v1, ColisionRadius r1,vector2 v2,ColisionRadius r2)
{
	return v2_lenght_squared(v2sub(v1,v2)) < (r1+r2)*(r1+r2) ; 
}

char pointInsideCircle(vector2 point, vector2 circleCenter,ColisionRadius r1)
{
	return v2_lenght_squared(v2sub(point,circleCenter))<r1*r1;
}
